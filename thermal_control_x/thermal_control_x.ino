/* 
 * Thermal Control X. An inteligent boiler control system
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

#include "thermal_control_x.h"
#include <LiquidCrystal.h>
#include <Time.h>
#include "lcdmenu.h"
#include "buttons.h"
#include <OneWire.h>
#include <Wire.h>
#include "dht11.h"
#include <DS1307RTC.h>
#include "external_eeprom.h"

LiquidCrystal lcd(12, 11, 7, 6, 5, 4);
lcdmenu_page main_pages[MAIN_PAGES_COUNT];
lcdmenu_page about_pages[ABOUT_PAGES_COUNT];
lcdmenu_page sensor_pages[SENSOR_PAGES_COUNT];
lcdmenu_page setdatetime_pages[SETDATETIME_PAGES_COUNT];
lcdmenu_page order_pages[ORDER_PAGES_COUNT]; //Warning: order_pages array is common for all order pages. There is no isolation between diferent orders.
#ifdef ENABLE_PLUS
lcdmenu_page statistics_pages[STATISTICS_PAGES_COUNT];
#endif /* ENABLE_PLUS */

/* Menu postion data */
int first_active_menu = 0;
int second_active_menu = -1; //-1 disables secondary menu view
int tag = 0; //Tag is a multipurpose datum to store something that a page must maintain, like an index.
button button_pressed = BUTTON_NONE;

/* Update timings */
unsigned long last_lcd_refresh = 0;
const unsigned long lcd_refresh_interval = 250;
unsigned long last_weather_refresh = 0;
const unsigned long weather_refresh_interval = 30000; // 30 seconds
unsigned long last_boiler_refresh = 0;
const unsigned long boiler_refresh_interval = 600000; // 10 minutes
unsigned long last_order_refresh = 0;
const unsigned long order_refresh_interval = 30000; // 30 seconds

/* Weather */
int temperature_air = 22;
int temperature_water = 35;
int top_temperature_water = 0;
int humidity = 99;
int max_temp_air = -100;
int min_temp_air = -100;

/* Orders */
order_t order[NUM_ORDERS];

/* 1-Wire addresses */
#ifdef PHYSICAL_IMPLEMENTATION
byte air_temp_adress[8] = {0x10, 0x70, 0xc7, 0xcd, 0x02, 0x08, 0x00, 0x8D}; //ID in real life
byte water_temp_adress[8] = {0x10, 0x3e, 0xa4, 0xcd, 0x02, 0x08, 0x00, 0xB4}; //ID in real life
#else /* PHYSICAL_IMPLEMENTATION */
byte air_temp_adress[8] = {0x10, 0xcd, 0xa4, 0x3e, 0x00, 0x00, 0x00, 0x5f}; //ID in real life
byte water_temp_adress[8] = {0x10, 0xcd, 0xc7, 0x70, 0x00, 0x00, 0x00, 0xf5}; //ID in real life
#endif /* PHYSICAL_IMPLEMENTATION */


/* Boiler */
int relay_status = LOW;

/*****************/
/* setup methods */
/*****************/

/***
  Arduino setup()
***/
void setup() {
  lcd.begin(16,2);
  lcd.setCursor(0, 0);
  #ifdef ENABLE_PLUS
    lcd.write("ThermalControlX+");
  #else /* ENABLE_PLUS */
    lcd.write("ThermalControlX");
  #endif /* ENABLE_PLUS */
  lcd.setCursor(0, 1);
  lcd.write("Inicializando...");
  
  Serial.begin(9600);
  
  Wire.begin();
  
  //Relay setup
  pinMode(RELAY_PIN, OUTPUT);
  
  // Time setup
  setTime(1,10,0,16,12,2014);
  setSyncProvider(RTC.get);
  setSyncInterval(300); //5 minutes
  
  // Load from eeprom
  byte magic_code_buffer[sizeof(MAGIC_CODE)];
  boolean magic_code_ok = true;
  
  // orders initialization
  // when EEPROM is not present, these are the default values
  for(int i = 0; i < NUM_ORDERS; ++i)
  {
    order[0].start_hour = 0;
    order[0].start_minute = 0;
    order[0].end_hour = 0;
    order[0].end_minute = 0;
    order[0].air_temperature = 0;
    order[0].active_days = 0;
  }
   
  i2c_eeprom_read_buffer(MAGIC_CODE_ADDR, magic_code_buffer, sizeof(magic_code_buffer));
  
  for(int i  = 0; i < sizeof(MAGIC_CODE); ++i)
  {
    if(magic_code_buffer[i] != MAGIC_CODE[i])
    {
      magic_code_ok = false;
      break;
    }
  }
  
  //Magic code is used to detect if stored values can be trusted
  if(magic_code_ok == true)
  {
    //Serial.println("mc|OK");
    for(int j = 0; j < NUM_ORDERS; ++j)
    {
      order_t *ptr = &order[j];
      i2c_eeprom_read_buffer(ORDER_ADDR + j * 12, (byte*)ptr, 12);
    }
  }
  else
  {
    //Serial.println("mc|NO");
    for(int k = 0; k < NUM_ORDERS; ++k)
    {
      order_t *ptr = &order[k];
      i2c_eeprom_write_page(ORDER_ADDR + k * 12, (byte*)ptr, 12);
    }
    
    //If magic code is not present, write it.
    i2c_eeprom_write_page(MAGIC_CODE_ADDR, (byte*)MAGIC_CODE, sizeof(MAGIC_CODE));
    //EEPROM is now trustful
  }
  
  // Setup main pages content        "_-_-_-_-_-_-_-_-"
  
  // sensor subpages
  strcpy(sensor_pages[0].title_row , "Fecha:00/00/0000");
  strcpy(sensor_pages[0].content_row,"Hora: 00:00    ~");
  sensor_pages[0].on_click = &on_sensor_submenu_click;
  sensor_pages[0].draw = &draw_datetime;
  
  //strcpy(sensor_pages[1].title_row , "    Ta=00oC    ");
  //strcpy(sensor_pages[1].content_row,"Tc=00oC   HU=00%");
  sensor_pages[1].on_click = &on_sensor_submenu_click;
  sensor_pages[1].draw = &draw_temperature_humidity;

  // sensor main page                                   
  strcpy(main_pages[0].title_row ,  "      Menu    ~ ");
  strcpy(main_pages[0].content_row, "    Sensores    ");
  main_pages[0].on_click = &on_menu_click;
  main_pages[0].draw = NULL;
  main_pages[0].children_pages = &sensor_pages[0];
  main_pages[0].children_length = SENSOR_PAGES_COUNT;
  
  // Order 0 submenu
  //strcpy(order_pages[0].title_row , " 00: 00  00: 00 ");
  //strcpy(order_pages[0].content_row," _______   00oC ");
  order_pages[0].on_click = &on_order_submenu_click;
  order_pages[0].draw = draw_order;
  
  // Order 0 menu
  strcpy(main_pages[1].title_row ,  "     Menu    ~ ");
  strcpy(main_pages[1].content_row, "   Consigna 0   ");
  main_pages[1].on_click = &on_menu_click;
  main_pages[1].draw = NULL;
  main_pages[1].children_pages = &order_pages[0];
  main_pages[1].children_length =  1;
  
  // Order 1 submenu
  //strcpy(order_pages[1].title_row , " 00: 00  00: 00 ");
  //strcpy(order_pages[1].content_row," _______   00oC ");
  order_pages[1].on_click = &on_order_submenu_click;
  order_pages[1].draw = draw_order;
  
  // Order 1 menu
  strcpy(main_pages[2].title_row ,  "     Menu    ~ ");
  strcpy(main_pages[2].content_row, "   Consigna 1   ");
  main_pages[2].on_click = &on_menu_click;
  main_pages[2].draw = NULL;
  main_pages[2].children_pages = &order_pages[0];
  main_pages[2].children_length =  1;
  
  // Order 2 submenu
  //strcpy(order_pages[2].title_row , " 00: 00  00: 00 ");
  //strcpy(order_pages[2].content_row," _______   00oC ");
  order_pages[2].on_click = &on_order_submenu_click;
  order_pages[2].draw = draw_order;
  
  // Order 2 menu
  strcpy(main_pages[3].title_row ,  "     Menu    ~ ");
  strcpy(main_pages[3].content_row, "   Consigna 2   ");
  main_pages[3].on_click = &on_menu_click;
  main_pages[3].draw = NULL;
  main_pages[3].children_pages = &order_pages[0];
  main_pages[3].children_length =  1;
  
  // Order 3 submenu
  //strcpy(order_pages[3].title_row , " 00: 00  00: 00 ");
  //strcpy(order_pages[3].content_row," _______   00oC ");
  order_pages[3].on_click = &on_order_submenu_click;
  order_pages[3].draw = draw_order;
  
  // Order 3 menu
  strcpy(main_pages[4].title_row ,  "     Menu    ~ ");
  strcpy(main_pages[4].content_row, "   Consigna 3   ");
  main_pages[4].on_click = &on_menu_click;
  main_pages[4].draw = NULL;
  main_pages[4].children_pages = &order_pages[0];
  main_pages[4].children_length =  1;
  
  // set date and time subpages
  strcpy(setdatetime_pages[0].title_row , "  00/ 00/ 0000  ");
  strcpy(setdatetime_pages[0].content_row,"     00: 00     ");
  setdatetime_pages[0].on_click = &on_setdatetime_submenu_click;
  setdatetime_pages[0].draw = &draw_setdatetime;
  
  strcpy(main_pages[5].title_row ,  "     Menu    ~ ");
  strcpy(main_pages[5].content_row, "   Fijar  hora  ");
  main_pages[5].on_click = &on_menu_click;
  main_pages[5].draw = NULL;
  main_pages[5].children_pages = &setdatetime_pages[0];
  main_pages[5].children_length = SETDATETIME_PAGES_COUNT;
  
#ifdef ENABLE_PLUS
  // statistics subpages
  strcpy(statistics_pages[0].title_row , "Max temp: 00oC  ");
  strcpy(statistics_pages[0].content_row,"Min temp: 00oC  ");
  statistics_pages[0].on_click = &on_about_submenu_click; //Return only
  statistics_pages[0].draw = &draw_statistics;
  
  strcpy(main_pages[6].title_row ,  "     Menu    ~ ");
  strcpy(main_pages[6].content_row, "  Estadisticas  ");
  main_pages[6].on_click = &on_menu_click;
  main_pages[6].draw = NULL;
  main_pages[6].children_pages = &statistics_pages[0];
  main_pages[6].children_length = STATISTICS_PAGES_COUNT;
  
  // about subpage
  strcpy(about_pages[0].title_row , " (C) rafael1193 ");
  strcpy(about_pages[0].content_row," GPLv3+ ");
  strcat(about_pages[0].content_row, PROGRAM_VERSION);
  about_pages[0].on_click = &on_about_submenu_click;
  about_pages[0].draw = NULL;
  
  // about page
  strcpy(main_pages[7].title_row ,  "     Menu      ");
  strcpy(main_pages[7].content_row, "   Acerca de... ");
  main_pages[7].on_click = &on_menu_click;
  main_pages[7].draw = NULL;
  main_pages[7].children_pages = &about_pages[0];
  main_pages[7].children_length = ABOUT_PAGES_COUNT;
#else /* ENABLE_PLUS */
  // about subpage
  strcpy(about_pages[0].title_row , " (C) rafael1193 ");
  strcpy(about_pages[0].content_row," GPLv3+ ");
  strcat(about_pages[0].content_row, PROGRAM_VERSION);
  about_pages[0].on_click = &on_about_submenu_click;
  about_pages[0].draw = NULL;
  
  // about page
  strcpy(main_pages[6].title_row ,  "     Menu      ");
  strcpy(main_pages[6].content_row, "   Acerca de... ");
  main_pages[6].on_click = &on_menu_click;
  main_pages[6].draw = NULL;
  main_pages[6].children_pages = &about_pages[0];
  main_pages[6].children_length = ABOUT_PAGES_COUNT;
#endif /* ENABLE_PLUS */
  
  //Update everything
  last_weather_refresh = weather_refresh_interval;
  last_lcd_refresh = lcd_refresh_interval;
  last_order_refresh = order_refresh_interval;
}


/*****************************/
/* on_click handling methods */
/*****************************/

/***
  Main menu on_click handler method
***/
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
      switch(first_active_menu)
      {
        // in case of order_pages, second_active_menu works different
        case 1:
        case 2:
        case 3:
        case 4:
          second_active_menu = first_active_menu - 1;
          break;
        default:
          second_active_menu = 0;
          break;
      }
    default:
      break;
  }
}

/***
  sensor submenu on_click handler method
***/
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

void on_order_submenu_click(button but)
{
  int check_high = order[second_active_menu].end_hour * 60 + order[second_active_menu].end_minute > order[second_active_menu].start_hour * 60 + order[second_active_menu].start_minute;
  switch (but)
  {
    case BUTTON_LEFT:
      if(tag > 0)
      {
        tag--;
      }
      break;
    case BUTTON_RIGHT:
      if(tag < 11)
      {
        tag++;
      }
      break;
    case BUTTON_PLUS:
      switch (tag)
      {
        case 0: //start hour
          if(check_high) {++order[second_active_menu].start_hour;}
          break;
        case 1: //start minute
          if(check_high) {++order[second_active_menu].start_minute;}
          break;
        case 2: //start hour
          if(order[second_active_menu].end_hour < 23) {++order[second_active_menu].end_hour;}
          break;
        case 3: //start minute
          if(order[second_active_menu].end_minute < 59) {++order[second_active_menu].end_minute;}
          break;
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
          bitSet(order[second_active_menu].active_days, tag - 4);
          break;
        case 11:
          if(order[second_active_menu].air_temperature < 30) {++order[second_active_menu].air_temperature;}
          break;
      }
      break;
    case BUTTON_MINUS:
      switch (tag)
      {
        case 0: //start hour
          if(order[second_active_menu].start_hour > 0) {--order[second_active_menu].start_hour;}
          break;
        case 1: //start minute
          if(order[second_active_menu].start_minute > 0) {--order[second_active_menu].start_minute;}
          break;
        case 2: //start hour
          if(check_high) {--order[second_active_menu].end_hour;}
          break;
        case 3: //start minute
          if(check_high) {--order[second_active_menu].end_minute;}
          break;
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
        case 9:
        case 10:
          bitClear(order[second_active_menu].active_days, tag - 4);
          break;
        case 11:
          if(order[second_active_menu].air_temperature > 0) {--order[second_active_menu].air_temperature;}
          break;
      }
      break;
    case BUTTON_RETURN:
      write_order(second_active_menu); //Update order data on eeprom
      tag = 0;
      second_active_menu = -1;
    default:
      break;
  }
}

/***
  set date & time on_click handler method
***/
void on_setdatetime_submenu_click(button but)
{
  tmElements_t tm;
  switch (but)
  {
    case BUTTON_LEFT:
      if(tag > 0)
      {
        tag--;
      }
      break;
    case BUTTON_RIGHT:
      if(tag < 4)
      {
        tag++;
      }
      break;
    case BUTTON_PLUS:
      RTC.read(tm);
      switch (tag)
      {
        case 0: //day
          switch (days_in_month(tm.Month, tm.Year + 1970))
          {
            case 31:
              if(tm.Day < 31) {++tm.Day;}
              break;
            case 30: //30-days months
              if(tm.Day < 30) {++tm.Day;}
              break;
            case 28: //February non lap
              if(tm.Day < 28) {++tm.Day;}
              break;
            case 29: //February lap
              if(tm.Day < 29) {++tm.Day;}
              break;
            default: //If Month is garbage due to an error, let's be it 1.
              tm.Month = 1;
              tm.Day = 1;
              break;
          }
          break;
        case 1: //month
          if(tm.Month < 12) {++tm.Month;}
          switch (days_in_month(tm.Month, tm.Year)) //Check if day is still valid for current month
          {
            case 30: //30-days months
              if(tm.Day > 30) {tm.Day = 30;}
              break;
            case 28: //February
              if(tm.Day > 28) {tm.Day = 28;}
              break;
            case 29: //February lap year
              if(tm.Day > 29) {tm.Day = 29;}
              break;
          }
          break;
        case 2: //year
          if(tm.Year < 99 /*Y2k38!*/) {++tm.Year;}
          // On year change it's necesary to check if day is still valid
          if(tm.Day == 29 && tm.Month == 2 && !islapyear(tm.Year))
          {
            tm.Day = 28;
          }
          break;
        case 3: //hour
          if(tm.Minute < 23) {++tm.Hour;}
          break;
        case 4: //minute
          if(tm.Minute < 59) {++tm.Minute;}
          break;
      }
      setTime(makeTime(tm));
      RTC.write(tm);
      break;
    case BUTTON_MINUS:
      RTC.read(tm);
      switch (tag)
      {
        case 0: //day
          if(tm.Day > 1) {--tm.Day;}
          break;
        case 1: //month
          if(tm.Month > 1) {--tm.Month;}
          switch (days_in_month(tm.Month, tm.Year)) //Check if day is still valid for current month
          {
            case 30: //30-days months
              if(tm.Day > 30) {tm.Day = 30;}
              break;
            case 28: //February
              if(tm.Day > 28) {tm.Day = 28;}
              break;
            case 29: //February lap year
              if(tm.Day > 29) {tm.Day = 29;}
              break;
          }
          break;
        case 2: //year
          if(tm.Year > 0) {--tm.Year;}
          if(tm.Day == 29 && tm.Month == 2 && !islapyear(tm.Year))
          {
            tm.Day = 28;
          }
          break;
        case 3: //hour
          if(tm.Minute > 0) {--tm.Hour;}
          break;
        case 4: //minute
          if(tm.Minute > 0) {--tm.Minute;}
          break;
      }
      setTime(makeTime(tm));
      RTC.write(tm);
      break;
    case BUTTON_RETURN:
      second_active_menu = -1;
      tag = 0;
      break;
    default:
      break;
  }
  
  return;
}

/***
  About on_click handler method
  All pushed buttons trigger "get back to main menu"
***/
void on_about_submenu_click(button but)
{
  if(but != BUTTON_NONE)
  {
    second_active_menu = -1; //This means, get back to main menu
  }
}


/******************/
/* helper methods */
/******************/

/* 
  Calculates the number of days in the specified month 
  Returns -1 on error
 */
int days_in_month(int mon, int ye)
{
  switch(mon)
  {
    //31-days months
    case 1:
    case 3:
    case 5:
    case 7:
    case 8:
    case 10:
    case 12:
      return 31;
    
    //30-days months
    case 4:
    case 6:
    case 9:
    case 11: 
      return 30; 
    case 2: 
      if(islapyear(ye) == true) //February
      {
        return 29;
      } else 
      {
        return 28; // Not lap year
      }
    default:
      return -1;
  } 
}

/* 
  Returns "true" the specified year is a lap year, else, returns "false"
 */
boolean islapyear(int ye)
{
  if (ye % 4 == 0  && (ye % 100 != 0  ||  ye % 400 == 0))
  {
    return true;
  } else {
    return false;
  }
}

void write_order(int n)
{
  if(0 <= n && n <= NUM_ORDERS - 1)
  {
    //Serial.print("dg|");
    //Serial.print("page ");
    //Serial.print(n);
    //Serial.println(" written");
    order_t *ptr = &order[n];
    i2c_eeprom_write_page(ORDER_ADDR + n * 12, (byte*)ptr, 12);
  }
}


/****************************/
/* draw handling methods */
/****************************/

/***
  Date & Time sensor draw handler method
***/
void draw_datetime()
{
  String str_day = String(day(), 10);
  String str_month = String(month(), 10);
  String str_year = String(year(), 10);
  String str_hour = String(hour(), 10);
  String str_minute = String(minute(), 10);

  lcd.setCursor(0, 0);
  lcd.print(sensor_pages[0].title_row);
  lcd.setCursor(0, 1);
  lcd.print(sensor_pages[0].content_row);
  
  if(str_day.length() <= 1) // Padding
  {
    lcd.setCursor(7, 0);
    lcd.print(str_day);
  }
  else
  {
    lcd.setCursor(6, 0);
    lcd.print(str_day);
  }
  
  if(str_month.length() <= 1) // Padding
  {
    lcd.setCursor(10, 0);
    lcd.print(str_month);
  }
  else
  {
    lcd.setCursor(9, 0);
    lcd.print(str_month);
  }
  
  lcd.setCursor(12, 0);
  lcd.print(str_year);
  
  if(str_hour.length() <= 1) // Padding
  {
    lcd.setCursor(7, 1);
    lcd.print(str_hour);
  }
  else
  {
    lcd.setCursor(6, 1);
    lcd.print(str_hour);
  }
  
  if(str_minute.length() <= 1) // Padding
  {
    lcd.setCursor(10, 1);
    lcd.print(str_minute);
  }
  else
  {
    lcd.setCursor(9, 1);
    lcd.print(str_minute);
  }

  return;
}

/***
  Temperature & Humidity sensor draw handler method
***/
void draw_temperature_humidity()
{
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

  lcd.setCursor(0,0);
  lcd.print(str_arriba);
  lcd.setCursor(0, 1);
  lcd.print(str_abajo);
  
  return;
}

/***
  Date & Time setter draw handler method
***/
void draw_setdatetime()
{
  String str_day = String(day(), 10);
  String str_month = String(month(), 10);
  String str_year = String(year(), 10);
  String str_hour = String(hour(), 10);
  String str_minute = String(minute(), 10);

  lcd.setCursor(0,0);
  lcd.print(setdatetime_pages[0].title_row);
  lcd.setCursor(0,1);
  lcd.print(setdatetime_pages[0].content_row);
  if(str_day.length() <= 1) // Padding
  {
    lcd.setCursor(3, 0);
    lcd.print(str_day);
  }else
  {
    lcd.setCursor(2, 0);
    lcd.print(str_day);
  }
  if(str_month.length() <= 1) // Padding
  {
    lcd.setCursor(7, 0);
    lcd.print(str_month);
  }
  else
  {
    lcd.setCursor(6, 0);
    lcd.print(str_month);
  }
  lcd.setCursor(10, 0);
  lcd.print(str_year);
  if(str_hour.length() <= 1) // Padding
  {
    lcd.setCursor(6, 1);
    lcd.print(str_hour);
  }else
  {
    lcd.setCursor(5, 1);
    lcd.print(str_hour);
  }
  if(str_minute.length() <= 1) // Padding
  {
    lcd.setCursor(10, 1);
    lcd.print(str_minute);
  }
  else
  {
    lcd.setCursor(9, 1);
    lcd.print(str_minute);
  }

  switch (tag)
  {
    case 0:
      lcd.setCursor(1, 0);
      lcd.write(INDEX_MARK);
      break;
    case 1:
      lcd.setCursor(5, 0);
      lcd.write(INDEX_MARK);
      break;
    case 2:
      lcd.setCursor(9, 0);
      lcd.write(INDEX_MARK);
      break;
    case 3:
      lcd.setCursor(4, 1);
      lcd.write(INDEX_MARK);
      break;
    case 4:
      lcd.setCursor(8, 1);
      lcd.write(INDEX_MARK);
      break;
  }
  return;
}

/***
  Draw order setter pages
***/

void draw_order()
{
  //second_active_menu means order currently being modified
  String str_start_hour = String(order[second_active_menu].start_hour, 10);
  String str_start_minute = String(order[second_active_menu].start_minute, 10);
  String str_end_hour = String(order[second_active_menu].end_hour, 10);
  String str_end_minute = String(order[second_active_menu].end_minute, 10);
  String str_air_temperature = String(order[second_active_menu].air_temperature, 10);
  
  lcd.print(" 00: 00  00: 00 ");
  lcd.setCursor(0, 1);
  lcd.print(" _______   00oC ");

  if(str_start_hour.length() <= 1) // Padding
  {
    lcd.setCursor(2, 0);
    lcd.print(str_start_hour);
  }else
  {
    lcd.setCursor(1, 0);
    lcd.print(str_start_hour);
  }
  
  if(str_start_minute.length() <= 1) // Padding
  {
    lcd.setCursor(6, 0);
    lcd.print(str_start_minute);
  }
  else
  {
    lcd.setCursor(5, 0);
    lcd.print(str_start_minute);
  }
  
  if(str_end_hour.length() <= 1) // Padding
  {
    lcd.setCursor(10, 0);
    lcd.print(str_end_hour);
  }else
  {
    lcd.setCursor(9, 0);
    lcd.print(str_end_hour);
  }
  
  if(str_end_minute.length() <= 1) // Padding
  {
    lcd.setCursor(14, 0);
    lcd.print(str_end_minute);
  }
  else
  {
    lcd.setCursor(13, 0);
    lcd.print(str_end_minute);
  }
  
  for(int d = 0; d < 7; ++d) 
  {
    lcd.setCursor(1 + d * 1, 1);
    
    if(bitRead(order[second_active_menu].active_days, d) == 1)
    {
      lcd.print(char_day[d]);
    }
    else 
    {
      lcd.print('_');
    }
  }
  
  if(str_air_temperature.length() <= 1) // Padding
  {
    lcd.setCursor(12, 1);
    lcd.print(str_air_temperature);
  }
  else
  {
    lcd.setCursor(11, 1);
    lcd.print(str_air_temperature);
  }
  
  switch (tag)
  {
    case 0:
    case 1:
      lcd.setCursor(0 + tag * 4, 0);
      lcd.write(INDEX_MARK);
      break;
    case 2:
    case 3:
      lcd.setCursor(8 + (tag-2) * 4, 0);
      lcd.write(INDEX_MARK);
      break;
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
      if(bitRead(millis(),9) == 0) //Sort of character blink mechanism
      {
        lcd.setCursor(tag-3, 1);
        lcd.write(INDEX_MARK);
      }
      break;
    case 11:
      lcd.setCursor(10, 1);
      lcd.write(INDEX_MARK);
      break;
  }
  return; 
}
#ifdef ENABLE_PLUS
void draw_statistics ()
{
  String str_max = String(max_temp_air, 10);
  String str_min = String(min_temp_air, 10);
  lcd.setCursor(0, 0);
  lcd.print(statistics_pages[0].title_row);
  lcd.setCursor(0, 1);
  lcd.print(statistics_pages[0].content_row);
  if(str_max.length() <= 1) // Padding
  {
    lcd.setCursor(11, 0);
    lcd.print(str_max);
  }
  else
  {
    lcd.setCursor(10, 0);
    lcd.print(str_max);
  }
  
  if(str_min.length() <= 1) // Padding
  {
    lcd.setCursor(11, 1);
    lcd.print(str_min);
  }
  else
  {
    lcd.setCursor(10, 1);
    lcd.print(str_min);
  }
}
#endif /* ENABLE_PLUS */

/*******************/
/* looping methods */
/*******************/

/***
  Arduino loop()
***/
void loop() 
{
  ////////////////////
  // BUTTON PRESING //
  //////////////////// 
  
  button_pressed = buttons_read();
  //Serial.write(button_pressed); // DEBUG:
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
    
    float t_air_f = getTemp(air_temp_adress);
    temperature_air = round_macro(t_air_f);
    Serial.print("ta|");
    Serial.println(t_air_f);
    
    if(max_temp_air == -100 && min_temp_air == -100) // this means "there is no max or min temperature"
    {
      max_temp_air = temperature_air;
      min_temp_air = temperature_air;
    }
    
    if(temperature_air > max_temp_air)
    {
      max_temp_air = temperature_air;
    }
    else if (temperature_air < min_temp_air)
    {
      min_temp_air = temperature_air;
    }
    
    float t_wat_f = getTemp(water_temp_adress);
    temperature_water = round_macro(t_wat_f);
    Serial.print("tc|");
    Serial.println(t_wat_f);
    #ifdef ENABLE_PLUS
      pinMode(DHT_DATA_PIN, OUTPUT);
      digitalWrite(DHT_DATA_PIN, HIGH);
      delay(100);
      read_dht11(DHT_DATA_PIN);
      humidity = dht11_sensor_humidity;
    #endif /* ENABLE_PLUS */
    
    last_weather_refresh = millis();
  }
  
  
  //////////////////
  // ORDER UPDATE //
  //////////////////
  
  if(abs(millis() - last_order_refresh) > order_refresh_interval) 
  {
    tmElements_t tm_now;
    breakTime(now(), tm_now);
    
    int relay_status = LOW;
    int order_on = false;
    
    //It's time to check orders!
    for(int i = 0; i < NUM_ORDERS; ++i)
    {
      int time_check = (order[i].start_hour * 60 + order[i].start_minute < tm_now.Hour * 60 + tm_now.Minute) && (tm_now.Hour * 60 + tm_now.Minute < order[i].end_hour * 60 + order[i].end_minute);
      int week_day_check = 0; 
      
      ///////////////////DEBUG///////////////////
      
      //Serial.print("s_h");
      //Serial.println(order[i].start_hour);
      //Serial.print("s_m");
      //Serial.println(order[i].start_minute);
      //Serial.print("e_h");
      //Serial.println(order[i].end_hour);
      //Serial.print("e_m");
      //Serial.println(order[i].end_minute);
      //Serial.print("week day ");
      //Serial.println(tm_now.Wday);
      //Serial.print("time check: ");
      //Serial.println(time_check);
        
      ///////////////////////////////////////////
      
      //Get weekday
      week_day_check = bitRead(order[i].active_days, tm_now.Wday - 1);
      
      // If we between in time span and we havent activated any order yet
      if(order_on == false)
      {
        if(week_day_check)
        {
          if(time_check) 
          {
            if(temperature_air < order[i].air_temperature)
            {
              relay_status = HIGH;
              
              // top_temperature_water stores the max temeprature registered at water pipes.
              // This is supossed to be the steady state temperature.
              // The following algorithm is in charge of maintaining water temperature on this
              // steady state temperature.
              // If water temp doesn't change from last check, we have arrived to saturation
              // temperature and boiler is stopped. (it's assumible that temperature change 
              // rate is faster than checking time rate)
              // When water temperature changes, we cannot stop. If temperature is higher, we 
              // are effectively heating; and when temperature is lower, we have to start again
              // in order to heat again the system until it reachs the max temperature.
              // In short words: 
              //   *  If water temp is *inside* TOP_WATER_TEMP_INTERVAL, boiler is 
              //      stopped.
              //   *  If water temp is *out of* TOP_WATER_TEMP_INTERVAL, boiler is 
              //      started.
              // (Checking cadence is lower because tube heating is slower)
              if(abs(millis() - last_boiler_refresh) > boiler_refresh_interval)
              {
                //ENABLE_PLUSSerial.print("dg|wat ");
                //Serial.println(temperature_water);
                //Serial.print("dg|top_wat ");
                //Serial.println(top_temperature_water);
                
                if(temperature_water > top_temperature_water)
                {
                  top_temperature_water = temperature_water;
                  //Serial.println("dg|bigger");
                  relay_status = HIGH;
                  top_temperature_water = temperature_water;
                }
                else if(top_temperature_water - temperature_water < TOP_WATER_TEMP_INTERVAL)
                {
                  //Serial.println("dg|on interval");
                  relay_status = LOW;
                }
                //      below interval
                else
                {
                  //Serial.println("dg|below");
                  relay_status = HIGH;
                }
                last_boiler_refresh = millis();
              }
              order_on = true;
            }
            else // When we don't want to heat, because we hit the target,
                 // reset water temperature controller
            {
              //Serial.println("dg|reset");
              top_temperature_water = 0;
            }
          }else{top_temperature_water = 0;}
        }else{top_temperature_water = 0;}
      }
    }
    
    Serial.print("ti|");
    Serial.println(now());
    Serial.print("ca|");
    Serial.println(relay_status);
    
    digitalWrite(RELAY_PIN, relay_status);
    last_order_refresh = millis();
  }
  
  
  ////////////////////
  // SCREEN REFRESH //
  ////////////////////
  
  //Screen refresh rate must be limited
  if(abs(millis() - last_lcd_refresh) > lcd_refresh_interval)
  {
    //Serial.write(0x30+first_active_menu);Serial.write("  ");Serial.write(0x30+second_active_menu);Serial.write('\n');
    if(second_active_menu != -1)
    {
      if(main_pages[first_active_menu].children_pages[second_active_menu].draw == NULL) // No se ha especificado una rutina de dibujado especial
      {
        lcd.clear();
        lcd.setCursor(0,0);
        //Serial.println(main_pages[first_active_menu].children_pages[second_active_menu].title_row);
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
