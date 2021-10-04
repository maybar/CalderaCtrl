# CalderaCrtl
Aplicación para controlar la caldera.
Es parte del proyecto domótico Home Control.
La aplicación recibe por radio los comandos para encender y apagar la caldera.
Microntrolador:
 * Arduino Digispark ATTINY85
 * Default -16.5mhz
 
 Radio usada:
 485MHz AM
 
 Librerias:
Virtual Wire
 
 ## Configuración hardware
 ### Arduino Digispark ATTINY85
 ENTRADAS
 - Pin1 Led  
 - Pin2 Datos del receptor de radio  
 
 SALIDAS:
 - Pin0 Activación de Relé  
 - Pin3 Datos al trasnmisor de radio  

## Comandos  
Controla la caldera con las ordenes recibidas por radio.  
| Comando  | Descripción |
| ------------- | ------------- |
| 0001-ON_	| Activa la caldera |  
| 0001-OFF	| Desactiva la caldera |  

Led con destello para confirmar la recepción del comando.  
- 3 Destellos para confirmar la recepción del comando de encendido  
- 2 Destellos para confirmar la recepción del comando de apagado  

Sistema de apagado automatico cuando no recibe nueva orden en 3 minutos.

Watchdog de 8 segundos.
 
