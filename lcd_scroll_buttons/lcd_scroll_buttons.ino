#include <Wire.h>
#include <LCD_ST7032.h>

#define BUZZER 12
#define BTN_SOL 16
#define BTN_SAG 14
#define GENISLIK 16

LCD_ST7032 lcd; 
char metin[] = "Selamlar"; 
int pozisyon = 0; 

void ciz(){
    lcd.clear();
    lcd.setCursor(0,0);
    for (int i = 0; i < pozisyon; i++) lcd.print(" "); 
    lcd.print(metin);
}

bool basildi(int pin){
    static uint32_t son[17] = {0};
    if (digitalRead(pin) == LOW && millis() - son[pin] > 200){
        son[pin] = millis();
        return true;
    }
    return false;
}

void setup(){
    //Buzzer susturma kodlari 
    pinMode(BUZZER, OUTPUT);
    digitalWrite(BUZZER, LOW);

    // I2C yi baslatiyoruz
    Wire.begin(4, 5);
    lcd.begin();
    lcd.setcontrast(30);

    // Yaziyi kaydiricagimiz tuslari tanimliyoruz
    pinMode(BTN_SOL, INPUT);
    pinMode(BTN_SAG, INPUT_PULLUP);

    ciz();
}

void loop(){
    int eski = pozisyon;
    if (basildi(BTN_SOL) && pozisyon > 0) pozisyon--;
    if (basildi(BTN_SAG) && pozisyon < GENISLIK - (int)strlen(metin)) pozisyon++; 
    if (pozisyon != eski) ciz();
}