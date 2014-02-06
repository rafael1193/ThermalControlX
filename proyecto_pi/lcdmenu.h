/* 
 * Proyecto PI. An inteligent boiler control system
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

typedef struct lcdmenu_page {
  char title_row[17];
  char content_row[17];
  void (*on_click)(button);
  void (*draw)();
  lcdmenu_page* children_pages;
  int children_length;
};
#define INDEX_MARK '?'
 
#endif /* LCDMENU_H */
