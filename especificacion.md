# Control de caldera inteligente

## Especificación

El sistema debe activar y desactivar una caldera para calefacción en función de:

* Temperatura de la habitación
* Temperatura de retorno del agua
* Día y hora

### Interfaz
    | + |
| ← | x | → |
    | - |

El sistema posee una interfaz de entrada de 5 botones:
 * + Aumenta el parámetro seleccionado.
 * - Disminuye el parámetro seleccionado.
 * ← Selecciona el parámetro anterior
 * → Selecciona el parámetro posterior
 * x Acción/Retorno

La interfaz de salida consiste en texto sobre una pantalla lcd 2x16.

La ventana principal muestra el estado actual de los sensores. Pulsando cualquier botón se muestra el menú principal.

Mediante las teclas de selección se resalta una función, que se puede seleccionar con la tecla Acción/Retorno: 

 * Sensores
 * Consigna 0
 * Consigna 1
 * Consigna 2
 * Consigna 3
 * Fijar hora
 * Estadísticas
 * Acerca de...

*Ejemplo de menú principal*

`______Menu______`
`<_Estadisticas_>`

#### Menú Estadísticas

El menú estadísticas proporciona información histórica de los valores de los sensores:
 * T mínima ambiente de las últimas 24 horas
 * T máxima ambiente de las últimas 24 horas
 * T mínima ambiente de los últimos 7 días
 * T máxima ambiente de los últimos 7 días
 * %Humedad medio de los últimos 7 días
 * ¿?Exportar datos al ordenador¿?
 
#### Menús Consigna

Selección de la Temperatura de consigna con incrementos de 0.5ºC

#### Menú Sensores

Se mostrará la información actual de los sensores actualizada cada minuto

`Ta=20.5ºC_Hu=30%`
`Tc=30.0ºC_13:37_`

#### Menú Fijar Hora

`<_____Hora_____>`
`_-_____13_____+_`

`<____Minuto____>`
`_-_____37_____+_`

### Sensores y actuadores

El sistema leerá el estado de los sensores cada minuto, comparará con la consigna según la histéresis fijada y almacenará un dato cada hora.

### Programación horaria

El sistema permitirá fijar 4 franjas horarias según:
 * Hora de inicio
 * Hora final
 * Día de la semana
 
### Almacenamiento

### Consigna

El actuador de la caldera se activará cuando se cumplan las siguientes condiciones:

 * Tamb <\ TCamb && (Tret1 - Tret0) <\ TCret
 * Histeresis ¿?configurable¿? para Tamb y Tret

Donde:

 | Tamb  | Temperatura ambiente
 | TCamb | Consigna de temperatura ambiente
 | Tret1 | Temperatura de retorno del agua a la caldera actual
 | Tret0 | Temperatura de retorno del agua a la caldera anterior
 | TCret | Incremento-consigna de temperatura retorno del agua a la caldera

### Avisos y errores

El sistema dará un notificación luminosa cuando:

 * La humedad en el ambiente sea superior o inferior a la consigna
 * Se produzca un error en algún sensor

En caso de error en el sistema de control, la caldera debe quedar desconectada.

