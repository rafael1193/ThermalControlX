/* 
 * Thermal Control X. An inteligent boiler control system
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

#ifndef THERMAL_CONTROL_X_H
#define THERMAL_CONTROL_X_H

#define PROGRAM_VERSION "v1.0.1"

#define ENABLE_PLUS
#define PHYSICAL_IMPLEMENTATION

#define RELAY_PIN 9
#define DHT_DATA_PIN 8

#ifdef ENABLE_PLUS
  #define MAIN_PAGES_COUNT 8
#else /* ENABLE_PLUS */
  #define MAIN_PAGES_COUNT 7
#endif /* ENABLE_PLUS */
#define ABOUT_PAGES_COUNT 1
#define SENSOR_PAGES_COUNT 2
#define SETDATETIME_PAGES_COUNT 1
#define ORDER_PAGES_COUNT 4
#define STATISTICS_PAGES_COUNT 1

#define NUM_ORDERS 4

#define round_macro(x) ((x)>=0?(long)((x)+0.5):(long)((x)-0.5)) //Temperature will never be near MAX_INT or MIN_INT, so there is no risk of overflow

#define DAY_MONDAY 0
#define DAY_TUESDAY 1
#define DAY_WEDNESDAY 2
#define DAY_THURSDAY 3
#define DAY_FRIDAY 4
#define DAY_SATURDAY 5
#define DAY_SUNDAY 6

#define TOP_WATER_TEMP_INTERVAL 2

const char char_day[] = {'L', 'M', 'X', 'J', 'V', 'S', 'D'};

typedef struct order_t {
  int start_hour;
  int start_minute;
  int end_hour;
  int end_minute;
  int active_days;
  int air_temperature;
};

#endif /* THERMAL_CONTROL_X_H */

