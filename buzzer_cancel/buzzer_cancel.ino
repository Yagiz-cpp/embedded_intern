#define BUZZER 12


void setup(){
	
	// Buzzer Susturme
	pinMode(BUZZER, OUTPUT) ; 
	digitalWrite(BUZZER, LOW) ;
 
	Serial.begin(115200) ; 
	
	// Buzzer Pinini Arama (GPIO12)
	delay(2000) ; 
	int pinler [] = {13, 12, 14, 16, 5, 4} ; 
	
	for(int p : pinler) {
		Serial.printf("GPIO%d \r\n", p) ; 
		Serial.flush() ; 
		pinMode(p, OUTPUT) ; 
		
		for (int i = 0; i < 200; i++){
		digitalWrite(p, HIGH) ; delayMicroseconds(500) ; 
		digitalWrite(p, LOW) ; delayMicroseconds(500) ; 
		}
		
		pinMode(p, INPUT) ; 
		delay(1500) ; 
	}
}

void loop(){}
