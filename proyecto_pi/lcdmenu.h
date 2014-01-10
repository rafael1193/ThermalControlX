#ifndef LCDMENU_H
#define LCDMENU_H
#include "buttons.h"

typedef enum lcdmenu_page_level {
  LCD_PAGE_LEVEL_FIRST,
  LCD_PAGE_LEVEL_SECOND
};

typedef struct lcdmenu_page {
  lcdmenu_page_level level;
  char title_row[16];
  char content_row[16];
  void (*on_click)(button);
};
 
#endif /* LCDMENU_H */
