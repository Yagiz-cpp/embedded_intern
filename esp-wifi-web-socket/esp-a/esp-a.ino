#include <ESP8266WiFi.h>
#define BUZZER 12

const char* AP_SSID = "YOUR_WIFI_SSID";
const char* AP_PASS = "YOUR_WIFI_PASSWORD";

uint8_t denemeSayisi = 0;
uint32_t zaman = 0; 


void subnet(uint32_t* ag,uint32_t* host){
  IPAddress ip = WiFi.localIP();
  IPAddress mask = WiFi.subnetMask();
  
  uint32_t ip_sayi = (uint32_t)ip[0] << 24 | (uint32_t)ip[1] << 16 | (uint32_t)ip[2] << 8 | (uint32_t)ip[3] ; 
  uint32_t mask_sayi  = (uint32_t)mask[0] << 24 | (uint32_t)mask[1] << 16 | (uint32_t)mask[2] << 8 | (uint32_t)mask[3] ;


  *ag = ip_sayi & mask_sayi ;

  *host = (~mask_sayi) -1;

}






void setup(){

  pinMode(BUZZER, OUTPUT);
  digitalWrite(BUZZER,LOW);

  Serial.begin(115200);
  delay(200);
  Serial.print("=== esp-a ===\n");

  Serial.print("ESP Reset Sebebi = ");
  Serial.println(ESP.getResetReason());
  
  WiFi.mode(WIFI_STA);
  WiFi.persistent(false);
  WiFi.begin(AP_SSID, AP_PASS);
  while(WiFi.status()!=WL_CONNECTED && denemeSayisi<60){
    delay(250);
    Serial.println("Baglaniliyor");
    denemeSayisi++;
  }

  if(WiFi.status() == WL_CONNECTED){
    Serial.println("Baglanildi");
    WiFi.printDiag(Serial);

    Serial.print("Local IP = ");
    Serial.println(WiFi.localIP());
    
    Serial.print("Subnet Mask = ");
    Serial.println(WiFi.subnetMask());
    
    Serial.print("Gateway IP = ");
    Serial.println(WiFi.gatewayIP());
 
    Serial.printf("RSSI= %d \n", WiFi.RSSI());
  } 
  else {
    Serial.println("Baglanilamadi");
  }


}

void loop(){
  if (millis() - zaman > 5000){
    zaman = millis();
    Serial.print("Get Free Heap");
    Serial.println(ESP.getFreeHeap());
    
    Serial.print("WiFi Status");
    Serial.println(WiFi.status());
  }
}