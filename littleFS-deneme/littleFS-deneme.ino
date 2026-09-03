#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <LittleFS.h>
#define BUZZER 12

ESP8266WebServer server(80);

void setup(){

    pinMode(BUZZER, OUTPUT) ; 
	digitalWrite(BUZZER, LOW) ;

    Serial.begin(115200);
    delay(500);

    if(!LittleFS.begin()){
        Serial.println("LittleFS mount edilemedi");
        return;
    }

    FSInfo i;
    LittleFS.info(i);
    Serial.printf("FS toplam: %u  kullanilan %u\n", i.totalBytes, i.usedBytes);

    

    WiFi.mode(WIFI_AP);
    WiFi.softAP("LittleFS", "12345678");
    Serial.println(WiFi.softAPIP());

    server.serveStatic("/", LittleFS, "/sayfa.html");
    server.serveStatic("/static", LittleFS, "/");

    server.onNotFound([](){
        server.send(404, "text/plain", "yok");
    });

    server.begin();
}

void loop(){
    server.handleClient();
}