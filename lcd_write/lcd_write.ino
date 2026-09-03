#include <Wire.h>
#include <LCD_ST7032.h>

#define BUZZER 12

LCD_ST7032 lcd ; 

void tara(int sda, int scl){
	

	// Hangi I2C adresinde ogrenmek icin
	Wire.begin(sda,scl) ; 
	Serial.printf("\r\n--SDA=%d  SCL%d--", sda,scl) ; 
	
	for( uint8_t adr = 1 ; adr < 127 ; adr++ ) {
	
		Wire.beginTransmission(adr) ; 
		if( Wire.endTransmission() == 0) {

			Serial.printf("Bulundu : 0x%02X\r\n", adr) ; 	
		
	}
	
	}
}

void setup(){
	pinMode(BUZZER, OUTPUT) ; 
	digitalWrite(BUZZER, LOW) ; 
	
	Wire.begin(4,5) ; 
	lcd.begin() ; 
	lcd.setcontrast(30) ; 
	lcd.print("Selam") ; 
	lcd.setCursor(1,0) ; 
	lcd.print("esp") ;  
}

void loop(){}
