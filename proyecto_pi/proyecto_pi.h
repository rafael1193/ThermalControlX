#ifndef PROYECTO_PI_H
#define PROYECTO_PI_H


LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
#define MAIN_PAGES_COUNT 8
lcdmenu_page main_pages[MAIN_PAGES_COUNT];
int active_page = 0;

#endif /* PROYECTO_PI_H */

