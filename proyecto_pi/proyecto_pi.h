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

#ifndef PROYECTO_PI_H
#define PROYECTO_PI_H

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
#define MAIN_PAGES_COUNT 8
lcdmenu_page main_pages[MAIN_PAGES_COUNT];
int active_page = 0;

#endif /* PROYECTO_PI_H */

