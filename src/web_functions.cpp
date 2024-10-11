#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <WiFiAP.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <SdFat.h>
#include <ArduinoOTA.h>
#include <HTTPClient.h>
#include <TimeLib.h>

#include <global_vars.h>
#include <version.h>
#include <sd_functions.h>
#include <TrackerSD.h>

const char *host = "tracker";

WebServer server(80);

WiFiClientSecure wifiClient;
bool firstWifiConnect = true;

const char* urlNewGpxFile = "https://192.168.0.101:14000/app/rest/gpxfiles";


void handleIndex()
{
    String htmlHeader = "<!DOCTYPE html><html lang=\"de\"><head><meta charset=\"utf-8\"/><style>* { box-sizing: border-box;}body { font-family: Arial; padding: 10px; background: #f1f1f1;}/* Header/Blog Title */.header { padding: 5px; text-align: center; background: white;}.header h1 { font-size: 40px;}.version {text-align: right; font-size: 12px;}/* Style the top navigation bar */.topnav { overflow: hidden; background-color: #669999;}/* Style the topnav links */.topnav a { float: left; display: block; color: #f2f2f2; text-align: center; padding: 14px 16px; text-decoration: none;}/* Change color on hover */.topnav a:hover { background-color: #ddd; color: black;}/* Change color on hover */.topnav a.active { background-color: #ddd; color: black;}/* Create two unequal columns that floats next to each other *//* Left column */.leftcolumn { float: left; width: 100%;}/* Add a card effect for articles */.card { background-color: white; padding: 20px; margin-top: 20px;}.button { background-color: #669999; border: none; color: white; padding: 5px 10px; text-align: center; text-decoration: none; display: inline-block; font-size: 16px; margin: 4px 2px; cursor: pointer;}/* Responsive layout - when the screen is less than 800px wide, make the two columns stack on top of each other instead of next to each other */@media screen and (max-width: 800px) { .leftcolumn, .rightcolumn { width: 100%; padding: 0; }}/* Responsive layout - when the screen is less than 400px wide, make the navigation links stack on top of each other instead of next to each other */@media screen and (max-width: 400px) { .topnav a { float: none; width: 100%; }}table { border-collapse: collapse; width: 100%;}th, td { text-align: left; padding: 8px;}tr:nth-child(even){background-color: #f2f2f2}th { background-color: white; color: #669999;}}</style></head>";
    String htmlNavigation = "<div class=\"topnav\"> <a class=\"active\" href=\"#\">Tracks</a> <a href=\"#\">Config</a> <a href=\"#\">Waypoints</a> <a href=\"#\" style=\"float:right\">About</a></div>";
    String htmlBody = "<body><div class=\"header\"> <h1>GpsRouteTracker</h1> <p class=\"version\">Version: "+version+" - "+versionDate+"</p></div>"+htmlNavigation+"<div class=\"row\"> <div class=\"leftcolumn\"> <div class=\"card\">";
    String htmlFooter = " </div> </div></div></body></html>";

    String output = htmlHeader+htmlBody+"<h2>Tracks</h2><table><tr><th>&nbsp;</th><th>Filename</th><th>Size</th></tr>";

    SdFile root;
    if (!root.open("/", O_READ))
    {
        Serial.println("open root failed");
    }

    SdFile fileInPath;
    char filename[13];
    while (fileInPath.openNext(&root, O_RDONLY))
    {
        fileInPath.getName(filename, 13);

        if (strstr(filename, ".gpx"))
        {
            Serial.println(filename);
            output += "<tr><td>";
            output += "<a class=\"button\" href='/delete?file=";
            output += filename;
            output += "' onclick='return confirm(\"Wirklich löschen?\")'>Löschen</a>";
            output += "</td><td>";
            output += "<a href='/download/";
            output += filename;
            output += "'>";
            output += filename;
            output += "</a>";
            output += "</td><td>";
            output += fileInPath.fileSize();
            output += "</td></tr>";
        }

        fileInPath.close();
    }

    root.close();

    output += "</table>"+htmlFooter;
    server.send(200, "text/html", output);
}

bool handleDownload(String uri)
{
    Serial.println("handleDownload");
    Serial.print(uri);
    String filename = uri.substring(10);
    Serial.print("->");
    Serial.println(filename);

    File32 file = getTrackerSD()->getSd().open(filename, O_READ);
    if (!file)
    {
        Serial.println("Datei kann nicht lesend geöffnet werden!");
        return false;
    }

    char filenameGelesen[13];
    file.getName(filenameGelesen, 13);

    Serial.print(filename);
    Serial.print("->");
    Serial.print(filenameGelesen);
    Serial.print(" Size: ");
    Serial.println(file.size());

    server.streamFile(file, "application/gpx+xml");
    file.close();
    return true;
}

void handleDelete()
{
    if (server.args() == 0)
    {
        return server.send(500, "text/plain", "BAD ARGS");
    }
    String filename = server.arg(0);
    Serial.println("handleFileDelete: " + filename);
    
    File32 file = getTrackerSD()->getSd().open(filename, O_READ);
    if (!file)
    {
        return server.send(404, "text/plain", "FileNotFound");
    }
    file.close();

    char filenameChar[50];
    filename.toCharArray(filenameChar,50);
    getTrackerSD()->getSd().remove(filenameChar);

    server.sendHeader("Location", "/", true);   //Redirect to our html web page  
    server.send(302, "text/plane", "");
}

void webSetup()
{
    Serial.println("webSetup");

    if(strcmp(trackerConfig.wlanMode, "OFF") != 0) {
        server.begin();
        Serial.println("WebServer started");

        MDNS.begin(host);
        Serial.print("Open http://");
        Serial.print(host);
        Serial.println(".local");

        server.on("/", HTTP_GET, handleIndex);
        server.on("/delete", HTTP_GET, handleDelete);
        server.onNotFound([]() {
            if (!handleDownload(server.uri()))
            {
                server.send(404, "text/plain", "could not open file!");
            }
        });
    }
}

void sendFiles2Http() {
    SdFile root;
    if (!root.open("/", O_READ))
    {
        Serial.println("open root failed");
    }

    SdFile fileInPath;
    char filename[13];
    while (fileInPath.openNext(&root, O_RDONLY))
    {
        fileInPath.getName(filename, 13);

        Serial.print(filename);
        Serial.print(" -> ");
        Serial.println(fileInPath.fileSize());

        //Serial.println(filecontent);

        fileInPath.close();

        //TODO: ModifiactionDaten mit aufnehmen
        //TODO: FileSize mit aufnehmen
        //TODO: DeviceName / Motorradname aufnehmen?
        String message = "{";
        message += "\"filename\": \"";
        message.concat(filename);
        //TODO: http + IP Adresse
        message += "\", \"download\": \"http://";
        message.concat(WiFi.localIP().toString());
        message += "/download/";
        message.concat(filename);
        message += "\", \"delete\": \"http://";
        message.concat(WiFi.localIP().toString());
        message += "/delete?file=";
        message.concat(filename);
        message += "\"";
        message += ", \"device\": \"";
        message.concat(trackerConfig.device);
        message += "\"";
        message += "}";

        wifiClient.setInsecure();
        
        HTTPClient httpClient;
    
        // Your Domain name with URL path or IP address with path
        httpClient.begin(wifiClient, urlNewGpxFile);

        // Specify content-type header
        httpClient.addHeader("Content-Type", "application/json");         
        // Send HTTP POST request
        int httpResponseCode = httpClient.POST(message);
                
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
            
        // Free resources
        httpClient.end();
    }

    root.close();
}

void setupOTA()
{
    // Port defaults to 3232
    // ArduinoOTA.setPort(3232);

    // Hostname defaults to esp3232-[MAC]
    // ArduinoOTA.setHostname("myesp32");

    // No authentication by default
    // ArduinoOTA.setPassword("admin");

    // Password can be set with it's md5 value as well
    // MD5(admin) = 21232f297a57a5a743894a0e4a801fc3
    // ArduinoOTA.setPasswordHash("21232f297a57a5a743894a0e4a801fc3");

    if(strcmp(trackerConfig.wlanMode, "OFF") != 0) {
        //TODO: OTA absichern
        ArduinoOTA
            .onStart([]() {
                String type;
                if (ArduinoOTA.getCommand() == U_FLASH)
                    type = "sketch";
                else // U_SPIFFS
                    type = "filesystem";

                // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
                Serial.println("Start updating " + type);
            })
            .onEnd([]() {
                Serial.println("\nEnd");
            })
            .onProgress([](unsigned int progress, unsigned int total) {
                Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
            })
            .onError([](ota_error_t error) {
                Serial.printf("Error[%u]: ", error);
                if (error == OTA_AUTH_ERROR)
                    Serial.println("Auth Failed");
                else if (error == OTA_BEGIN_ERROR)
                    Serial.println("Begin Failed");
                else if (error == OTA_CONNECT_ERROR)
                    Serial.println("Connect Failed");
                else if (error == OTA_RECEIVE_ERROR)
                    Serial.println("Receive Failed");
                else if (error == OTA_END_ERROR)
                    Serial.println("End Failed");
            });

        ArduinoOTA.begin();
    }
}

void handleWebClient()
{
    server.handleClient();
}

void handleArduinoOTA()
{
    ArduinoOTA.handle();
}


void wifiSetup()
{
    Serial.println("wifiSetup");

    if(strcmp(trackerConfig.wlanMode, "CLIENT") == 0) {
        Serial.println("connectWifiHome");
        WiFi.mode(WIFI_STA);   
        WiFi.begin(trackerConfig.wlanClientSsid, trackerConfig.wlanClientPassword);
        firstWifiConnect=true;
    } else if (strcmp(trackerConfig.wlanMode, "AP") == 0) {
        Serial.print("Wifi AP startup: ");
        Serial.println(trackerConfig.wlanApSsid);
        WiFi.softAP(trackerConfig.wlanApSsid, trackerConfig.wlanApPassword);
        IPAddress myIP = WiFi.softAPIP();
        Serial.print("AP IP address: ");
        Serial.println(myIP);
        Serial.println("Wifi AP started");

        webSetup();
        setupOTA();
    }
}


void loopWifiConnect()
{
    if(strcmp(trackerConfig.wlanMode, "CLIENT") == 0) {
        if (WiFi.status() != WL_CONNECTED) {
            Serial.println("Connecting to WiFi..");
        } else {
            if (firstWifiConnect) { 
                Serial.println("WiFi connected");
                Serial.println("IP address: ");
                Serial.println(WiFi.localIP());

                webSetup();
                setupOTA();

                firstWifiConnect=false;
            } 
        }
    }
}
