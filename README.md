# CalderaCrtl
Aplicaci�n para controlar la caldera.
Es parte del proyecto dom�tico Home Control.
La aplicaci�n recibe por radio los comandos para encender y apagar la caldera.
Microntrolador:
 * Arduino Digispark ATTINY85
 * Default -16.5mhz
 
 Radio usada:
 485MHz AM
 
 Librerias:
Virtual Wire
 
 ## Configuraci�n hardware
 ### Arduino Digispark ATTINY85
 ENTRADAS
 Pin1 Led
 Pin2 Datos del receptor de radio
 
 SALIDAS:
 Pin0 Activaci�n de Rel�
 Pin3 Datos al trasnmisor de radio


Controla la caldera con las ordenes recibidas por radio.
0001-ON_	Activa la caldera
0001-OFF	Desactiva la caldera

Led con destello rapido para ON, y destello lento para OFF.

Sistema de apagado automatico cuando no recibe nueva orden en 3 minutos.

Watchdog de 8 segundos.
 
