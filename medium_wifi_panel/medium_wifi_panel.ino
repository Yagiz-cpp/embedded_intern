#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <LCD_ST7032.h>
#include <Wire.h>

#define BUZZER 12

const uint8_t I2C_SDA = 4;
const uint8_t I2C_SCL = 5;
const uint8_t LCD_ADDR = 0x27;

ESP8266WebServer server(80);
LCD_ST7032 lcd;

String satir1 = "Merhaba";
String satir2 = "Dunya";
bool arkaIsik = true;

void lcdGuncelle(){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(satir1);
    lcd.setCursor(1,0);
    lcd.print(satir2);
}

String htmlDuzelt(String s){
    s.replace("&", "&amp;");
    s.replace("<", "&lt;");
    s.replace(">", "&gt;");
    s.replace("\"", "&quot;");
    s.replace("'", "&#39;");

    return s ;
}

void anaSayfa(){
    String h = "<!DOCTYPE html><html><head><meta charset='utf-8'>"
    "<meta name = 'viewport' content='width=device-width, initial-scale=1'>"
    "<title>LCD KONTROL</title></head><body>"
    "<h2>LCD KONTROL</h2>"
    "<form action='/set' method='POST'>"
    "Satir 1: <input name='l1' maxlength='16' value='" + htmlDuzelt(satir1) + "'> <br><br>"
    "Satir 2: <input name='l2' maxlength='16' value='" + htmlDuzelt(satir2) + "'> <br><br>"
    "<label><input type='checkbox' name='b1' value='1'";

    h+= (arkaIsik ? "checked":"");
    
    h+= "> Arka Isik </label><br><br>"
    "<button type='submit'> Kaydet </button></form>"
    "<p> Calisma Suresi:"+String(millis()/1000)+ " sn </p>"
    "</body></html>";

    server.send(200, "text/html", h);
}

void ayarla(){
    if (server.hasArg("l1")) satir1 = server.arg("l1").substring(0,16);
    if (server.hasArg("l2")) satir2 = server.arg("l2").substring(0,16);
    arkaIsik = server.hasArg("b1");

    lcdGuncelle();
    server.sendHeader("Location", "/");
    server.send(303);
}

void setup(){

    pinMode(BUZZER, OUTPUT);
    digitalWrite(BUZZER, LOW);

    Serial.begin(115200);
    Wire.begin(I2C_SDA, I2C_SCL);
    lcd.begin();
    lcd.setcontrast(30);
    lcdGuncelle();

    WiFi.mode(WIFI_AP);
    WiFi.softAP("ESP-TEST", "12345678");
    Serial.println(WiFi.softAPIP());

    server.on("/", HTTP_GET, anaSayfa);
    server.on("/set", HTTP_POST, ayarla);
    server.begin();
}

void loop(){
    server.handleClient();
}