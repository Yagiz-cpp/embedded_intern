#include <ESP8266WiFi.h>
#include <espnow.h>

uint8_t ESP_B_MAC [6] = {0xC4, 0xD8, 0xD5, 0x1A, 0x4E, 0xA5};

uint32_t gonderilenSayac = 0;

void onVeriGeldi(uint8_t* mac, uint8_t* veri, uint8_t uzunluk){
    if(uzunluk != sizeof(uint32_t)) return;
    uint32_t gelenSayac;
    memcpy(&gelenSayac, veri, sizeof(gelenSayac));
    Serial.printf("B'den gelen: %lu", gelenSayac);
}

void setup(){
    pinMode(12,OUTPUT);
    digitalWrite(12,LOW);

    Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    esp_now_init();
    esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
    esp_now_add_peer(ESP_B_MAC, ESP_NOW_ROLE_COMBO,1,NULL,0);
    esp_now_register_recv_cb(onVeriGeldi);

    Serial.println("ESP-A hazir");

}

void loop(){
    static uint32_t sonGonderim =0;
    if(millis() - sonGonderim > 2000){
        sonGonderim = millis();
        gonderilenSayac++;
        esp_now_send(ESP_B_MAC, (uint8_t*)&gonderilenSayac, sizeof(gonderilenSayac));
        Serial.printf("A'dan gonderilen: %lu", gonderilenSayac);
    }
}