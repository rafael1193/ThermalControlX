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

#define MAIN_PAGES_COUNT 8
#define ABOUT_PAGES_COUNT 1
#define SENSOR_PAGES_COUNT 2
#define SETDATETIME_PAGES_COUNT 1

#define round_macro(x) ((x)>=0?(long)((x)+0.5):(long)((x)-0.5)) //Temperature will never be near MAX_INT or MIN_INT, so there is no risk of overflow

typedef enum DAYOFWEEK
{
  DAY_SUNDAY = 1,
  DAY_MONDAY = 2,
  DAY_TUESDAY = 3,
  DAY_WEDNESDAY = 4,
  DAY_THURSDAY = 2,
  DAY_FRIDAY = 2,
  DAY_SATURDAY = 2
};

#endif /* PROYECTO_PI_H */

