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

#define PROGRAM_VERSION "v0.1.00"

#define RELAY_PIN 9

#define MAIN_PAGES_COUNT 8
#define ABOUT_PAGES_COUNT 1
#define SENSOR_PAGES_COUNT 2
#define SETDATETIME_PAGES_COUNT 1
#define ORDER_PAGES_COUNT 4

#define NUM_ORDERS 4

#define round_macro(x) ((x)>=0?(long)((x)+0.5):(long)((x)-0.5)) //Temperature will never be near MAX_INT or MIN_INT, so there is no risk of overflow

typedef enum DAYOFWEEK
{
  DAY_SUNDAY = 1,
  DAY_MONDAY = 2,
  DAY_TUESDAY = 3,
  DAY_WEDNESDAY = 4,
  DAY_THURSDAY = 5,
  DAY_FRIDAY = 6,
  DAY_SATURDAY = 7
};

typedef struct order_t {
  int start_hour;
  int start_minute;
  int end_hour;
  int end_minute;
  int active_days;
  int air_temperature;
};

#endif /* PROYECTO_PI_H */

