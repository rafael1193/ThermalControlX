/* 
 * Proyecto PI. A boiler inteligent control system
 * Copyright (C) 2014  Rafael Bailón-Ruiz <rafaelbailon en ieee punto org> 
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

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
const int BUTTON1HIGH = 900; //866
const int BUTTON2LOW = 700;
const int BUTTON2HIGH = 750; //723
const int BUTTON3LOW = 550;
const int BUTTON3HIGH = 650; //575
const int BUTTON4LOW = 400;
const int BUTTON4HIGH = 500; //422
const int BUTTON5LOW = 200;
const int BUTTON5HIGH = 350; //236
 
// Variables will change:
int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

 // the following variables are long's because the time, measured in miliseconds,
 // will quickly become a bigger number than can be stored in an int.
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 200;    // the debounce time; increase if the output flickers

void buttons_init() {
  
}

button buttons_read() {
   // read the state of the switch into a local variable:
   int reading = analogRead(buttonPin);
   //Serial.println(reading); // DEBUG:
   
   if(reading>BUTTON5LOW && reading<BUTTON5HIGH){
     //Read switch 5
     buttonState = BUTTON5;
   }else if(reading>BUTTON4LOW && reading<BUTTON4HIGH){
     //Read switch 4
     buttonState = BUTTON4;
   }else if(reading>BUTTON3LOW && reading<BUTTON3HIGH){
     //Read switch 3
     buttonState = BUTTON3;
   }else if(reading>BUTTON2LOW && reading<BUTTON2HIGH){
     //Read switch 2
     buttonState = BUTTON2;
   }else if(reading>BUTTON1LOW && reading<BUTTON1HIGH){
     //Read switch 1
     buttonState = BUTTON1;
   }else{
     //No button is pressed;
     buttonState = LOW;
   }
   if(millis() - lastDebounceTime < debounceDelay)
   {
     return BUTTON_NONE;
   }
   
   //Else, enough time has passed
   lastButtonState = buttonState;
   lastDebounceTime = millis();
   
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
