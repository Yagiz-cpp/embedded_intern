#include <ESP8266WiFi.h>
#include <espnow.h>
#include <LCD_ST7032.h>

uint8_t ESP_A_MAC[6] = { 0xC4, 0xD8, 0xD5, 0x1A, 0x51, 0x90 };  

LCD_ST7032 lcd;

uint32_t gonderilenSayac=0;
uint32_t gelenSayac = 0;

void lcdGuncelle(){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Ben(B)");
    lcd.print(gonderilenSayac);

    lcd.setCursor(1,0);
    lcd.print("A'dan");
    lcd.print(gelenSayac);
}

void onVeriGeldi(uint8_t* mac, uint8_t* veri, uint8_t uzunluk){
    if(uzunluk != sizeof(uint32_t)) return;
    memcpy(&gelenSayac, veri, sizeof(gelenSayac));
    Serial.printf("A'dan geldi: %lu\n", gelenSayac);
    lcdGuncelle();
}

void setup(){
    
    pinMode(12,OUTPUT);
    digitalWrite(12,LOW);

    Serial.begin(115200);
    delay(200);

    lcd.begin();
    lcd.setcontrast(50);
    lcd.print("ESP-B hazir");

    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    esp_now_init();
    esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
    esp_now_add_peer(ESP_A_MAC, ESP_NOW_ROLE_COMBO,1,NULL,0);
    esp_now_register_recv_cb(onVeriGeldi);
    
    Serial.println("ESP-B Hazir");
}

void loop(){
    static uint32_t sonGonderim = 0;
    if(millis()-sonGonderim > 2000) {
        sonGonderim = millis();
        gonderilenSayac++;
        esp_now_send(ESP_A_MAC, (uint8_t*)&gonderilenSayac, sizeof(gonderilenSayac));
        Serial.printf("B'den gonderildi: %lu\n", gonderilenSayac);
        lcdGuncelle();
    }
}