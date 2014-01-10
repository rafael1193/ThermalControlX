#include <LiquidCrystal.h>
#include "lcdmenu.h"
#include "proyecto_pi.h"
#include "buttons.h"

int active_menu = 0;
button button_pressed = BUTTON_NONE;

void setup() {
  lcd.begin(16,2);
  lcd.setCursor(0, 0);
  Serial.begin(9600);
  
  buttons_init();
  
  // Setup main pages content       "_-_-_-_-_-_-_-_"
  main_pages[0].level = LCD_PAGE_LEVEL_FIRST;
  strcpy(main_pages[0].title_row ,  "      Menu   > ");
  strcpy(main_pages[0].content_row, "    Sensores   ");
  main_pages[0].on_click = &on_menu_click;
  
  main_pages[1].level = LCD_PAGE_LEVEL_FIRST;
  strcpy(main_pages[1].title_row ,  " <    Menu   > ");
  strcpy(main_pages[1].content_row, "   Consigna 0  ");
  main_pages[1].on_click = &on_menu_click;
  
  main_pages[2].level = LCD_PAGE_LEVEL_FIRST;
  strcpy(main_pages[2].title_row ,  " <    Menu   > ");
  strcpy(main_pages[2].content_row, "   Consigna 1  ");
  main_pages[2].on_click = &on_menu_click;
  
  main_pages[3].level = LCD_PAGE_LEVEL_FIRST;
  strcpy(main_pages[3].title_row ,  " <    Menu   > ");
  strcpy(main_pages[3].content_row, "   Consigna 2  ");
  main_pages[3].on_click = &on_menu_click;
  
  main_pages[4].level = LCD_PAGE_LEVEL_FIRST;
  strcpy(main_pages[4].title_row ,  " <    Menu   > ");
  strcpy(main_pages[4].content_row, "   Consigna 3  ");
  main_pages[4].on_click = &on_menu_click;
  
  main_pages[5].level = LCD_PAGE_LEVEL_FIRST;
  strcpy(main_pages[5].title_row ,  " <    Menu   > ");
  strcpy(main_pages[5].content_row, "  Fijar  hora  ");
  main_pages[5].on_click = &on_menu_click;
  
  main_pages[6].level = LCD_PAGE_LEVEL_FIRST;
  strcpy(main_pages[6].title_row ,  " <    Menu   > ");
  strcpy(main_pages[6].content_row, "  Estadisticas ");
  main_pages[6].on_click = &on_menu_click;
  
  main_pages[7].level = LCD_PAGE_LEVEL_FIRST;
  strcpy(main_pages[7].title_row ,  " <    Menu     ");
  strcpy(main_pages[7].content_row, "  Acerca de... ");
  main_pages[7].on_click = &on_menu_click;
  
}

void on_menu_click(button but)
{
  switch (but)
  {
    
    case BUTTON_LEFT:
      if(active_menu > 0)
      {
        active_menu--;
      }
      break;
    case BUTTON_RIGHT:;
      if(active_menu < MAIN_PAGES_COUNT-1)
      {
        active_menu++;
      }
      break;
    default:
      break;
  }
}

void loop() {
  Serial.write("funcionando");
  button_pressed = buttons_read_non_blocking();
  Serial.write(button_pressed);
  if (button_pressed != 0){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print(main_pages[active_menu].title_row);
    lcd.setCursor(0,1);
    lcd.print(main_pages[active_menu].content_row);
    main_pages[active_menu].on_click(button_pressed);
  }
    delay(100);

  
}
