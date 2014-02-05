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

#include "proyecto_pi.h"
#include <LiquidCrystal.h>
#include <Time.h>
#include "lcdmenu.h"
#include "buttons.h"
#include <OneWire.h>
#include <Wire.h>
#include <DS1307RTC.h>


LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
lcdmenu_page main_pages[MAIN_PAGES_COUNT];
lcdmenu_page about_pages[ABOUT_PAGES_COUNT];
lcdmenu_page sensor_pages[SENSOR_PAGES_COUNT];
lcdmenu_page setdatetime_pages[SETDATETIME_PAGES_COUNT];

/* Menu postion data */
int first_active_menu = 0;
int second_active_menu = -1; //-1 disables secondary menu view
int tag = 0; //Tag is a multipurpose datum to store something that a page must maintain, like an index.
button button_pressed = BUTTON_NONE;

/* Update timings */
unsigned long last_lcd_refresh = 0;
unsigned long lcd_refresh_interval = 250;
unsigned long last_time_refresh = 0;
unsigned long time_refresh_interval = 30000;
unsigned long last_weather_refresh = 0;
unsigned long weather_refresh_interval = 60000; //Each minute

/* Weather */
int temperature_air = 22;
int temperature_water = 35;
int humidity = 52;

/* Time */

/* 1-Wire addresses */
byte air_temp_adress[8] = {0x10, 0xcd, 0xa4, 0x3e, 0x00, 0x00, 0x00, 0x5f}; //ID in real life
byte water_temp_adress[8] = {0x10, 0xcd, 0xc7, 0x70, 0x00, 0x00, 0x00, 0xf5}; //ID in real life

//byte air_temp_adress[8] = {0x10, 0x70, 0xc7, 0xcd, 0x02, 0x08, 0x00, 0x8D}; //ID in real life
//byte tube_temp_adress[8] = {0x10, 0x3e, 0xa4, 0xcd, 0x02, 0x08, 0x00, 0xB4}; //ID in real life

void setup() {
  lcd.begin(16,2);
  lcd.setCursor(0, 0);
  lcd.write("  Bienvenido!  ");
  lcd.setCursor(0, 1);
  lcd.write("Inicializando...");
  
  Serial.begin(9600);
  
  buttons_init();
  
  //Time setup
  setTime(1,10,0,16,12,2014);
  setSyncProvider(RTC.get);
  
  // Setup main pages content        "_-_-_-_-_-_-_-_-"
  
  // sensor subpages
  strcpy(sensor_pages[0].title_row , "   01/01/1970  ~");
  strcpy(sensor_pages[0].content_row,"     13:37      ");
  sensor_pages[0].on_click = &on_sensor_submenu_click;
  sensor_pages[0].draw = &draw_datetime;
  
  strcpy(sensor_pages[1].title_row , "    Ta=12oC    ");
  strcpy(sensor_pages[1].content_row,"Tc=12oC   HU=34%");
  sensor_pages[1].on_click = &on_sensor_submenu_click;
  sensor_pages[1].draw = &draw_temperature_humidity;

  //sensor main page                                   
  strcpy(main_pages[0].title_row ,  "      Menu    ~ ");
  strcpy(main_pages[0].content_row, "    Sensores    ");
  main_pages[0].on_click = &on_menu_click;
  main_pages[0].draw = NULL;
  main_pages[0].children_pages = &sensor_pages[0];
  main_pages[0].children_length = SENSOR_PAGES_COUNT;
  
  strcpy(main_pages[1].title_row ,  "     Menu    ~ ");
  strcpy(main_pages[1].content_row, "   Consigna 0   ");
  main_pages[1].on_click = &on_menu_click;
  main_pages[1].draw = NULL;
  
  strcpy(main_pages[2].title_row ,  "     Menu    ~ ");
  strcpy(main_pages[2].content_row, "   Consigna 1   ");
  main_pages[2].on_click = &on_menu_click;
  main_pages[2].draw = NULL;
  
  strcpy(main_pages[3].title_row ,  "     Menu    ~ ");
  strcpy(main_pages[3].content_row, "   Consigna 2   ");
  main_pages[3].on_click = &on_menu_click;
  main_pages[3].draw = NULL;
  
  strcpy(main_pages[4].title_row ,  "     Menu    ~ ");
  strcpy(main_pages[4].content_row, "   Consigna 3   ");
  main_pages[4].on_click = &on_menu_click;
  main_pages[4].draw = NULL;
  
  // set date and time subpages
  strcpy(setdatetime_pages[0].title_row , " >01/>01/>1970  ");
  strcpy(setdatetime_pages[0].content_row,"    >13:>37     ");
  setdatetime_pages[0].on_click = &on_setdatetime_submenu_click;
  setdatetime_pages[0].draw = NULL;
  
  strcpy(main_pages[5].title_row ,  "     Menu    ~ ");
  strcpy(main_pages[5].content_row, "   Fijar  hora  ");
  main_pages[5].on_click = &on_menu_click;
  main_pages[5].draw = NULL;
  main_pages[5].children_pages = &setdatetime_pages[0];
  main_pages[5].children_length = SETDATETIME_PAGES_COUNT;
  
  strcpy(main_pages[6].title_row ,  "     Menu    ~ ");
  strcpy(main_pages[6].content_row, "  Estadisticas  ");
  main_pages[6].on_click = &on_menu_click;
  main_pages[6].draw = NULL;
  
  // about subpage
  strcpy(about_pages[0].title_row , " (C) rafael1193 ");
  strcpy(about_pages[0].content_row," GPLv3+ licence ");
  about_pages[0].on_click = &on_about_submenu_click;
  about_pages[0].draw = NULL;
  
  // about page
  strcpy(main_pages[7].title_row ,  "     Menu      ");
  strcpy(main_pages[7].content_row, "   Acerca de... ");
  main_pages[7].on_click = &on_menu_click;
  main_pages[7].draw = NULL;
  main_pages[7].children_pages = &about_pages[0];
  main_pages[7].children_length = ABOUT_PAGES_COUNT;
  
  //Update everything
  last_weather_refresh = weather_refresh_interval;
  last_lcd_refresh = lcd_refresh_interval;
  last_time_refresh = time_refresh_interval;
  
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
    default:
      break;
  }
}

void on_sensor_submenu_click(button but)
{
  switch (but)
  {
    case BUTTON_LEFT:
      if(second_active_menu > 0)
      {
        second_active_menu--;
      }
      break;
    case BUTTON_RIGHT:
      if(second_active_menu < SENSOR_PAGES_COUNT-1)
      {
        second_active_menu++;
      }
      break;
    case BUTTON_RETURN:
      second_active_menu = -1;
    default:
      break;
  }
}

void on_setdatetime_submenu_click(button but)
{
  switch (but)
  {
    case BUTTON_LEFT:
      if(second_active_menu > 0)
      {
        second_active_menu--;
      }
      break;
    case BUTTON_RIGHT:
      if(second_active_menu < SETDATETIME_PAGES_COUNT-1)
      {
        second_active_menu++;
      }
      break;
    case BUTTON_RETURN:
      second_active_menu = -1;
    default:
      break;
  }
}

void on_about_submenu_click(button but)
{
  if(but != BUTTON_NONE)
  {
    second_active_menu = -1; //This means, get back to main menu
  }
}

void draw_datetime()
{
    String str_arriba = "";
    String str_abajo = "";
    String str_day = String(day(), 10);
    String str_month = String(month(), 10);
    String str_year = String(year(), 10);
    String str_hour = String(hour(), 10);
    String str_minute = String(minute(), 10);
    
    if(str_day.length() <= 1) // Padding
    {
      str_arriba += "0";
    }
    str_arriba += str_day;
    str_arriba += "/";
    if(str_month.length() <= 1) // Padding
    {
      str_arriba += "0";
    }
    str_arriba += str_month;
    str_arriba += "/";
    str_arriba += str_year;
    
    if(str_hour.length() <= 1) // Padding
    {
      str_abajo += "0";
    }
    str_abajo += str_hour;
    str_abajo += ":";
    if(str_minute.length() <= 1) // Padding
    {
      str_abajo += "0";
    }
    str_abajo += str_minute;

    str_arriba.toCharArray(sensor_pages[0].title_row, 15);
    str_abajo.toCharArray(sensor_pages[0].content_row, 15);
    Serial.println(sensor_pages[0].title_row);
    Serial.println(sensor_pages[0].content_row);
    
    lcd.print("Fecha:"+ str_arriba);
    lcd.setCursor(0, 1);
    lcd.print("Hora:  "+ str_abajo);

    lcd.print("   ");
    lcd.print("~");
  
  return;
}

void draw_temperature_humidity()
{
//  Serial.println("he llegao");
  String str_arriba = "";
  String str_abajo = "";
  String str_air = String(temperature_air, 10);
  String str_water = String(temperature_water, 10);
  String str_humidity = String(humidity, 10);

  //Arriba
  str_arriba += "    Ta=";
  if(str_air.length() <= 1) // Padding
  {
    str_arriba += "0";
  }
  str_arriba += str_air;
  str_arriba += "oC   ";

  //Abajo
  str_abajo += "Tc=";
  if(str_water.length() <= 1) // Padding
  {
    str_abajo += "0";
  }
  str_abajo += str_water;
  str_abajo += "oC  HU=";

  if(str_humidity.length() <= 1) // Padding
  {
    str_abajo += "0";
  }
  str_abajo += str_humidity;
  str_abajo += "%";

  str_arriba.toCharArray(sensor_pages[1].title_row, 15);
  str_abajo.toCharArray(sensor_pages[1].content_row, 15);
//  Serial.println(sensor_pages[1].title_row);
//  Serial.println(sensor_pages[1].content_row);
  
  lcd.print(str_arriba);
  lcd.setCursor(0, 1);
  lcd.print(str_abajo);
  
  return;
}

void no_update()
{
  return;
}

void loop() 
{
  ////////////////////
  // BUTTON PRESING //
  //////////////////// 
  
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
  
  ////////////////////
  // WEATHER UPDATE //
  ////////////////////
  
  //Serial.println(last_weather_refresh);
  if(abs(millis() - last_weather_refresh) > weather_refresh_interval) 
  {
    //It's time to update weather info!
    
    temperature_air = round_macro(getTemp(air_temp_adress));
    Serial.write("Ta|");
    Serial.println(getTemp(air_temp_adress));
    
    temperature_water = round_macro(getTemp(water_temp_adress));
    Serial.write("Tc|");
    Serial.println(getTemp(water_temp_adress));
    
    //TODO: Humidity information
    //TODO: Error handling
    
    last_weather_refresh = millis();
  }
  
  /////////////////////
  // CONSIGNA UPDATE //
  /////////////////////
  
  //////////////////
  // TIME REFRESH //
  //////////////////
  
  
  
  ////////////////////
  // SCREEN REFRESH //
  ////////////////////
  
  //Screen refresh rate must be limited
  if(abs(millis() - last_lcd_refresh) > lcd_refresh_interval)
  {
    Serial.write(0x30+first_active_menu);Serial.write("  ");Serial.write(0x30+second_active_menu);Serial.write('\n');
    if(second_active_menu != -1)
    {
      if(main_pages[first_active_menu].children_pages[second_active_menu].draw == NULL) // No se ha especificado una rutina de dibujado especial
      {
        lcd.clear();
        lcd.setCursor(0,0);
        Serial.println(main_pages[first_active_menu].children_pages[second_active_menu].title_row);
        lcd.print(main_pages[first_active_menu].children_pages[second_active_menu].title_row);
        lcd.setCursor(0,1);
        lcd.print(main_pages[first_active_menu].children_pages[second_active_menu].content_row);
      }
      else
      {
        lcd.clear();
        lcd.setCursor(0,0);
        main_pages[first_active_menu].children_pages[second_active_menu].draw();
      }
    }else{
      if(main_pages[first_active_menu].draw == NULL) // No se ha especificado una rutina de dibujado especial
      {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(main_pages[first_active_menu].title_row);
      lcd.setCursor(0,1);
      lcd.print(main_pages[first_active_menu].content_row);
      }
      else
      {
        lcd.clear();
        lcd.setCursor(0,0);
        main_pages[first_active_menu].draw();
      }
    }
    last_lcd_refresh=millis();
  }
}


