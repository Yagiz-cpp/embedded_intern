#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Wire.h>
#include <LCD_ST7032.h>

const char* AP_SSID = "ESP-SAYAC";
const char* AP_PASS  ="12345678";
const char* URL = "http://192.168.4.1/veri";

const uint32_t ARALIK = 500;

const uint8_t PIN_SDA = 4;
const uint8_t PIN_SCL = 5;
const uint8_t SUTUN = 16;

LCD_ST7032 lcd;
WiFiClient wifi;
HTTPClient http;

uint32_t sonSorgu = 0;          // <-- bu

void satirYaz(uint8_t satir, String metin) {    // <-- ve bu
    while (metin.length() < SUTUN) metin += ' ';
    lcd.setCursor(satir, 0);
    lcd.print(metin.substring(0, SUTUN));
}

void setup() {
    pinMode(12, OUTPUT) ;
	digitalWrite(12, LOW) ;
    Serial.begin(115200);
    delay(200);

    Wire.begin(PIN_SDA, PIN_SCL);
    lcd.begin();
    lcd.setcontrast(30);                 // ekran soluksa 20-45 arasi dene
    lcd.clear();
    satirYaz(0, "ESP-SAYAC");
    satirYaz(1, "baglaniyor");

    WiFi.mode(WIFI_STA);
    WiFi.begin(AP_SSID, AP_PASS);
    while (WiFi.status() != WL_CONNECTED) {
        delay(250);
        Serial.print(".");
    }
    Serial.println("\nbaglandi");
    satirYaz(1, "bagli");
}


void loop() {
    if (millis() - sonSorgu < ARALIK) return;
    sonSorgu = millis();

    http.begin(wifi, URL);
    int kod = http.GET();

    if (kod == 200) {
        String cevap = http.getString();          // "42;1"
        int p = cevap.indexOf(';');               // noktali virgul nerede

        String deger = cevap.substring(0, p);     // "42"
        String durum = cevap.substring(p + 1);    // "1"

        satirYaz(0, "Sayac:" + deger);
        satirYaz(1, durum == "1" ? "calisiyor" : "durdu");
        Serial.println(deger);
    } else {
        satirYaz(1, "HTTP " + String(kod));       // negatif kod = baglanti hatasi
    }

    http.end();                                   // cagirmazsan bellek sizar
}
