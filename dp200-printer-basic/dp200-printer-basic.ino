#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <LCD_ST7032.h>
#include <Wire.h>
#include "pins.h"

uint32_t fisNo = 0;

void fisBas(String s1, String s2){
    Serial1.write(0x1B); Serial1.write(0x40);
    delay(50);

    // Ortalama Pos Komutu
    Serial1.write(0x1B); Serial1.write(0x61); Serial.write(1);

    // Buyuk Yazi Boyutu Pos Komutu
    Serial1.write(0x1D); Serial1.write(0x21); Serial1.write(0x11);

    Serial1.print("DP200\n");

    // Normal Yazi Boyutu Pos Komutu
    Serial1.write(0x1D); Serial1.write(0x21); Serial1.write(0x00);

    // Sola Hizzalama Pos Komutu
    Serial1.write(0x1B); Serial1.write(0x61); Serial1.write(0);

    Serial1.print("===============================\n");

    fisNo++;

    Serial1.print("Fis No : "); Serial1.println(fisNo);
    Serial1.print("Sure : "); Serial1.print(millis()/1000); Serial1.println(" sn");

    
    Serial1.print("--------------------------------\n");
    Serial1.println(s1);
    Serial1.println(s2);
    Serial1.print("================================\n");

    Serial1.write(0x1B); Serial1.write(0x64); Serial1.write(4);
}

void setup(){

    pinMode(BUZZER, OUTPUT);
    digitalWrite(BUZZER, LOW);

    Serial.begin(9600);
    Serial1.begin(9600);
    delay(3000);

    Serial.println("Fis Basiliyor....");
    fisBas("TEST", "TEST-2");

}

void loop(){
    fisBas("LoopTest", "Yagiz");
    delay(1000);
}