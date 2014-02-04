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
 
#ifndef ORDER_TEMPERATURA_H
#define ORDER_TEMPERATURA_H

typedef struct order {
  days  on_days;
  int start_hour;
  int start_minute = 0;
  int end_hour;
  int end_minute;
  int air_temperature;
  int alert_  
}

typedef enum days {
  DAY_MONDAY = 1 << 0,
  DAY_TUESDAY = 1 << 1,
  DAY_WEDNESDAY = 1 << 2,
  DAY_THURSDAY = 1 << 3,
  DAY_FRIDAY = 1 << 4,
  DAY_SATURDAY = 1 << 5,
  DAY_SUNDAY = 1 << 6
};

#endif /*ORDER_TEMPERATURA_H*/
 
