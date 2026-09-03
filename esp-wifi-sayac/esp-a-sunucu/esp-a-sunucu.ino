#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <LittleFS.h>
#include <stdarg.h>

const char* AP_SSID = "ESP-SAYAC";
const char* AP_PASS = "12345678";
const uint32_t ARALIK = 1000;

ESP8266WebServer server(80);

int sayac = 0;
bool calisiyor = true;
uint32_t sonArtis =0;

#define LOG_SATIR 24
#define LOG_UZUNLUK 80

char logBuf[LOG_SATIR][LOG_UZUNLUK];
uint8_t logSonraki = 0;
bool logDoldu = false;

void logla(const char* fmt, ...){
    char govde[LOG_UZUNLUK -12];
    va_list ap;
    va_start(ap,fmt);
    vsnprintf(govde, sizeof(govde), fmt, ap);
    va_end(ap);

    snprintf(logBuf[logSonraki], LOG_UZUNLUK, "[%6lu] %s", (unsigned long)(millis()/1000), govde);
    logSonraki = (logSonraki +1)% LOG_SATIR;

    if(logSonraki == 0) logDoldu = true;
}

void setup(){

    Serial.begin(115200);
    delay(200);

    logla("Acilis reset sebebi %s", ESP.getResetReason().c_str());

    if(!LittleFS.begin()){
        logla("Hata: LittleFS baslatilamadi");
    } else{
        FSInfo fs;
        LittleFS.info(fs);
        logla("FS ok:%lu / %lu bayt", (unsigned long)fs.usedBytes, (unsigned long)fs.totalBytes);

    }

    WiFi.mode(WIFI_AP);
    WiFi.softAP(AP_SSID,AP_PASS);
    logla("AP Hazir");

    server.on("/veri", HTTP_GET, [](){
        server.send(200, "text/plain",String(sayac) + ";" + String(calisiyor?1:0));
    });

    server.on("/komut", HTTP_GET, [](){
        String c = server.arg("c");
        if (c == "basla") calisiyor = true;
        else if (c=="dur") calisiyor = false;
        else if (c=="sifirla") sayac=0;
        logla("komut : %s", c.c_str());
        server.send(200, "text/plain", "ok");

    });

    server.on("/log", HTTP_GET, [](){
        uint8_t bas = logDoldu ? logSonraki:0;
        uint8_t adet = logDoldu ? LOG_SATIR : logSonraki;

        String cikti;

        for(uint8_t i =0; i<adet; i++){
            cikti += logBuf[(bas+i)%LOG_SATIR];
            cikti += '\n';
        }
        server.send(200,"text/plain", cikti);
    });

   server.serveStatic("/", LittleFS, "/");

   server.begin();
   logla("Sunucu Hazir"); 
}

void loop(){
    server.handleClient();

    if(calisiyor && millis() - sonArtis >= ARALIK){
        sonArtis = millis();
        sayac++;

        if(sayac%30==0){
            logla("sayac: %d, bosRAM: %lu", sayac, (unsigned long)ESP.getFreeHeap());
        }
    }
}