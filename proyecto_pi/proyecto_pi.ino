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

#include <LiquidCrystal.h>
#include "lcdmenu.h"
#include "proyecto_pi.h"
#include "buttons.h"

int first_active_menu = 7;
int second_active_menu = -1; //-1 disables secondary menu view
button button_pressed = BUTTON_NONE;
int last_lcd_refresh=0;
int lcd_refresh_interval = 250;

void setup() {
  lcd.begin(16,2);
  lcd.setCursor(0, 0);
  Serial.begin(9600);
  
  buttons_init();
  
  // Setup main pages content       "_-_-_-_-_-_-_-_"
  strcpy(main_pages[0].title_row ,  "      Menu   > ");
  strcpy(main_pages[0].content_row, "    Sensores   ");
  main_pages[0].on_click = &on_menu_click;
  
  strcpy(main_pages[1].title_row ,  " <    Menu   > ");
  strcpy(main_pages[1].content_row, "   Consigna 0  ");
  main_pages[1].on_click = &on_menu_click;
  
  strcpy(main_pages[2].title_row ,  " <    Menu   > ");
  strcpy(main_pages[2].content_row, "   Consigna 1  ");
  main_pages[2].on_click = &on_menu_click;
  
  strcpy(main_pages[3].title_row ,  " <    Menu   > ");
  strcpy(main_pages[3].content_row, "   Consigna 2  ");
  main_pages[3].on_click = &on_menu_click;
  
  strcpy(main_pages[4].title_row ,  " <    Menu   > ");
  strcpy(main_pages[4].content_row, "   Consigna 3  ");
  main_pages[4].on_click = &on_menu_click;
  
  strcpy(main_pages[5].title_row ,  " <    Menu   > ");
  strcpy(main_pages[5].content_row, "  Fijar  hora  ");
  main_pages[5].on_click = &on_menu_click;
  
  strcpy(main_pages[6].title_row ,  " <    Menu   > ");
  strcpy(main_pages[6].content_row, "  Estadisticas ");
  main_pages[6].on_click = &on_menu_click;
  
  //about subpage
  strcpy(about_pages[0].title_row , "(C) rafael1193 ");
  strcpy(about_pages[0].content_row," GPLv3 license ");
  about_pages[0].on_click = &on_about_click;
  
  //about page
  strcpy(main_pages[7].title_row ,  " <    Menu     ");
  strcpy(main_pages[7].content_row, "  Acerca de... ");
  main_pages[7].on_click = &on_menu_click;
  main_pages[7].children_pages = &about_pages[0];
  main_pages[7].children_length = ABOUT_PAGES_COUNT;
  
}

void on_menu_click(button but)
{
  switch (but)
  {
    case BUTTON_LEFT:
      if(first_active_menu > 0)
      {
        first_active_menu--;
      }
      break;
    case BUTTON_RIGHT:
      if(first_active_menu < MAIN_PAGES_COUNT-1)
      {
        first_active_menu++;
      }
      break;
    case BUTTON_RETURN:
      second_active_menu = 0;
      //last_lcd_refresh=0; //Necesario?
      
    default:
      break;
  }
}

void on_about_click(button but)
{
  if(but != BUTTON_NONE)
  {
    second_active_menu = -1; //This means, get back to main menu
  }
}

void loop() {
  
  //Screen refresh rate must be limited
  if(millis() - last_lcd_refresh > lcd_refresh_interval)
  {
    Serial.write(0x30+first_active_menu);Serial.write("  ");Serial.write(0x30+second_active_menu);Serial.write('\n');
    if(second_active_menu != -1)
    {
      lcd.clear();
      lcd.setCursor(0,0);
      Serial.println(main_pages[first_active_menu].children_pages[second_active_menu].title_row);
      lcd.print(main_pages[first_active_menu].children_pages[second_active_menu].title_row);
      lcd.setCursor(0,1);
      lcd.print(main_pages[first_active_menu].children_pages[second_active_menu].content_row);
    }else{
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(main_pages[first_active_menu].title_row);
      lcd.setCursor(0,1);
      lcd.print(main_pages[first_active_menu].content_row);
    }
    last_lcd_refresh=millis();
  }
  
  button_pressed = buttons_read();
  Serial.write(button_pressed); // DEBUG:
  if (button_pressed != 0){
    if(second_active_menu != -1)
    {
      last_lcd_refresh = 0; //Force LCD refresh
      main_pages[first_active_menu].children_pages[second_active_menu].on_click(button_pressed);
    }else{
      last_lcd_refresh = 0; //Force LCD refresh
      main_pages[first_active_menu].on_click(button_pressed);
    }

  }
  

  
}
