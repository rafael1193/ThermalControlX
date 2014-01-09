#include <LiquidCrystal.h>
#include "lcdmenu.h"
#include "proyecto_pi.h"

void setup() {
  lcd.begin(16,2);
  lcd.setCursor(0, 0);
  Serial.begin(9600);
  
  // Setup main pages content       "_-_-_-_-_-_-_-_"
  main_pages[0].level = FIRST;
  strcpy(main_pages[0].title_row ,  "      Menu   > ");
  strcpy(main_pages[0].content_row, "    Sensores   ");
  
  main_pages[1].level = FIRST;
  strcpy(main_pages[1].title_row ,  " <    Menu   > ");
  strcpy(main_pages[1].content_row, "   Consigna 0  ");
  
  main_pages[2].level = FIRST;
  strcpy(main_pages[2].title_row ,  " <    Menu   > ");
  strcpy(main_pages[2].content_row, "   Consigna 1  ");
  
  main_pages[3].level = FIRST;
  strcpy(main_pages[3].title_row ,  " <    Menu   > ");
  strcpy(main_pages[3].content_row, "   Consigna 2  ");
  
  main_pages[4].level = FIRST;
  strcpy(main_pages[4].title_row ,  " <    Menu   > ");
  strcpy(main_pages[4].content_row, "   Consigna 3  ");
  
  main_pages[5].level = FIRST;
  strcpy(main_pages[5].title_row ,  " <    Menu   > ");
  strcpy(main_pages[5].content_row, "  Fijar  hora  ");
  
  main_pages[6].level = FIRST;
  strcpy(main_pages[6].title_row ,  " <    Menu   > ");
  strcpy(main_pages[6].content_row, "  Estadisticas ");
  
  main_pages[7].level = FIRST;
  strcpy(main_pages[7].title_row ,  " <    Menu     ");
  strcpy(main_pages[7].content_row, "  Acerca de... ");
  
}

void loop() {
  int i = 0;
  
  for(i = 0; i < MAIN_PAGES_COUNT; ++i)
  {
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(main_pages[i].title_row);
    lcd.setCursor(0,1);
    lcd.print(main_pages[i].content_row);
    delay(1000);
  }
  
}
