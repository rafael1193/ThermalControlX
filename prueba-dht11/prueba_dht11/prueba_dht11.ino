#include <LiquidCrystal.h>
#include "dht.h"
#define DHT11_PIN 6
dht DHT;
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
}
void loop() {
  int chk;
  Serial.print("DHT11, \t");
  
  chk = DHT.read11(DHT11_PIN);
  switch (chk)
  {
    case DHTLIB_OK:  
		Serial.print("OK,\t"); 
		break;
    case DHTLIB_ERROR_CHECKSUM: 
		Serial.print("Checksum error,\t"); 
		break;
    case DHTLIB_ERROR_TIMEOUT: 
		Serial.print("Time out error,\t"); 
		break;
    default: 
		Serial.print("Unknown error,\t"); 
		break;
  }
  // DISPLAY DATA
  lcd.setCursor(0,0);
  lcd.print("Temp=");
  lcd.print(DHT.temperature);
  lcd.print(" *C");
  lcd.setCursor(0,1);
  lcd.print("Humidity=");
  lcd.print(DHT.humidity);
  lcd.print("% ");
  delay(500);
 }
