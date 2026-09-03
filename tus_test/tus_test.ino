#include <Wire.h>

#define BUZZER 12 

void setup(){

	// Buzzer susturma
	pinMode(BUZZER, OUTPUT) ; 
	digitalWrite(BUZZER, LOW) ; 
	

	Serial.begin(115200) ; 
	delay(1000) ;
	Serial.println("\r\n---- tus testi ----") ; 


	pinMode(13, INPUT_PULLUP) ; 
	pinMode(15, INPUT) ; 
	pinMode(0, INPUT_PULLUP) ; 
	
}


void loop(){

	static uint32_t t = 0 ;
	if(millis() - t > 200){
		t = millis() ;
		Serial.printf("A0=%4d  13=%d  15=%d  0=%d\r\n",
		analogRead(A0), digitalRead(13), digitalRead(15), digitalRead(0)
		) ; 
	}
	
	yield() ; 
}
