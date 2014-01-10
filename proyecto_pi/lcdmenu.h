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
