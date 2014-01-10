#include "buttons.h"

// constants won't change. They're used here to 
// set pin numbers:
const int buttonPin = 0;     // the number of the pushbutton pin

// TODO: Put these constants in a different file 
const int BUTTON1 = 1;
const int BUTTON2 = 2;
const int BUTTON3 = 3;
const int BUTTON4 = 4;
const int BUTTON5 = 5;
//Check constants
const int BUTTON1LOW = 800;
const int BUTTON1HIGH = 900;//866
const int BUTTON2LOW = 700;
const int BUTTON2HIGH = 750;//723
const int BUTTON3LOW = 550;
const int BUTTON3HIGH = 650;
const int BUTTON4LOW = 400;
const int BUTTON4HIGH = 500;
const int BUTTON5LOW = 200;
const int BUTTON5HIGH = 350;
 
// Variables will change:
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

 // the following variables are long's because the time, measured in miliseconds,
 // will quickly become a bigger number than can be stored in an int.
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 1000;    // the debounce time; increase if the output flickers

void buttons_init() {
  
}

button buttons_read_non_blocking() {
   // read the state of the switch into a local variable:
   int reading = analogRead(buttonPin);
   Serial.println(reading);
   
   /*
   Not needed while getting the values.
   */
   int tmpButtonState = LOW;             // the current reading from the input pin
   
   if(reading>BUTTON5LOW && reading<BUTTON5HIGH){
     //Read switch 5
     tmpButtonState = BUTTON5;
   }else if(reading>BUTTON4LOW && reading<BUTTON4HIGH){
     //Read switch 4
     tmpButtonState = BUTTON4;
   }else if(reading>BUTTON3LOW && reading<BUTTON3HIGH){
     //Read switch 3
     tmpButtonState = BUTTON3;
   }else if(reading>BUTTON2LOW && reading<BUTTON2HIGH){
     //Read switch 2
     tmpButtonState = BUTTON2;
   }else if(reading>BUTTON1LOW && reading<BUTTON1HIGH){
     //Read switch 1
     tmpButtonState = BUTTON1;
   }else{
     //No button is pressed;
     tmpButtonState = LOW;
   }

   // check to see if you just pressed the button 
   // (i.e. the input went from LOW to a buttonState),  and you've waited 
   // long enough since the last press to ignore any noise:  

   // If the switch changed, due to noise or pressing:
   if (tmpButtonState != lastButtonState) {
     // reset the debouncing timer
     lastDebounceTime = millis();
   } 

   if ((millis() - lastDebounceTime) > debounceDelay) {
     // whatever the reading is at, it's been there for longer
     // than the debounce delay, so take it as the actual current state:
     buttonState = tmpButtonState;
     Serial.println(buttonState);
   }

   // save the reading.  Next time through the loop,
   // it'll be the lastButtonState:
   lastButtonState = tmpButtonState;
   
   // set the LED using the state of the button for testing:
   switch(buttonState){
     case BUTTON1:
       //lcd.print('1');
       return BUTTON_PLUS;
       break;
     case BUTTON2:
       //lcd.print('2');
       return BUTTON_RETURN;
       break;
     case BUTTON3:
       //lcd.print('3');
       return BUTTON_MINUS;
       break;
     case BUTTON4:
       //lcd.print('4');
       return BUTTON_LEFT;
       break;
     case BUTTON5:
       //lcd.print('5');
       return BUTTON_RIGHT;
       break;
     default:
       //lcd.print('0');
       return BUTTON_NONE;
       break;
   }
 }