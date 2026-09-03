#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include "pins.h"

ESP8266WebServer server(80);

int sayac = 1;
uint32_t sonArtis = 0;

const char SAYFA[] PROGMEM = R"HTML(
    <!DOCTYPE html><html><head><meta chatset='utf-8'>
    <meta name='viewport' content='width=device-width, initial-scale=1'>
    <title>Sayac</title></head>
    <body>
    <h1 id='s'>--</h1>
    <script>
    setInterval(async () => {
        try{
            const r = await fetch('/veri');
            document.getElementById('s').textContent = await r.text();
        } catch (e){}
    }, 500);
    </script>
    </body>
    </html>
)HTML";

void setup(){
    
    pinMode(BUZZER, OUTPUT);
    digitalWrite(BUZZER, LOW);
    delay(1000);

    Serial.begin(115200);
    delay(200);

    WiFi.mode(WIFI_AP);
    WiFi.softAP("ESP-SAYAC", "12345678");
    Serial.println(WiFi.softAPIP());

    server.on("/", HTTP_GET, [](){
        server.send_P(200,"text/html", SAYFA);
    });

    server.on("/veri", HTTP_GET, [](){
        server.send(200, "text/plain", String(sayac));
    });

    server.begin();
}

void loop(){
    server.handleClient();

    if(millis() - sonArtis >= 1000){
        sonArtis = millis();
        Serial.println(sayac);
        if (sayac < 100) sayac++ ; 
    }
}