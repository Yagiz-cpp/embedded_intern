#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server(80);

void anaSayfa(){
    server.send(200, "text/html", "<h1>Merhaba</h1>") ;
}

void setup(){
    Serial.begin(115200);
    delay(500);
    
    WiFi.mode(WIFI_AP);
    WiFi.softAP("ESP-TEST", "12345678");
    Serial.print("IP: ");
    Serial.println(WiFi.softAPIP());

    server.on("/", anaSayfa);
    server.begin();
}

void loop(){
    server.handleClient();
    yield();
}