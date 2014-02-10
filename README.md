# Descripción/Description
_Thermal Control X is an intelligent heating control system desgined for Arduino Leonardo. You can also use it on Uno but you will need to remove some features in order to not to use much SRAM._

Thermal Control X es una gama de sistemas termostáticos inteligentes para control de calefacción.

Características generales:

* Interfaz gráfica interactiva
* Actuación sobre caldera mediante relé.
* 4 consignas programables según hora y día de la semana.
* Información sobre temperatura ambiental, temperatura de retorno a la caldera y humedad_(*)_.
* Estadística climática_(*)_.
* Reloj en tiempo real.
* Almacenamiento permanente de parámetros en caso de pérdida de energía.
* Conexión mediante USB al software Thermal Control X Monitor para PC.

(*) Humedad y estadística solo están disponibles en el modelo Thermal Control X+. Consulte la sección Compilación condicional y Thermal Control X+ para más información.

#Compilación/How to build it

First, import libraries from _libraries_ folder. Then fill your OneWire temperature sensor IDs and build it on Arduino IDE.

#License
```
Thermal Control X. An inteligent boiler control system
Copyright (C) 2014  Rafael Bailón-Ruiz <rafaelbailon en ieee punto org> 

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
```

