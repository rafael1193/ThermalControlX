#ifndef LCDMENU_H
#define LCDMENU_H

typedef enum lcdmenu_page_level {
  FIRST,
  SECOND
};

typedef struct lcdmenu_page {
  lcdmenu_page_level level;
  char title_row[16];
  char content_row[16];
};
 
#endif /* LCDMENU_H */
