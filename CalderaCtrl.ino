/** \file CalderaCtrl.ino
 * Control for Caldera. RF communication
 * This program runs in the TTINY hardware.
 * Miguel A. 10 Febrero 2018
 * Arduino Digispark ATTINY85
 * Default -16.5mhz
 */

/*! \mainpage A Brief Description.
\section my-intro Introduction
 P0 OUTPUT DATA TX
 P2 INPUT DATA RX

*/
#include <VirtualWire.h>
//#include <DigiCDC.h>    //To use the terminal
#include <avr/wdt.h>
 
#define LEN_BUFFER_RX  8
#define PIN_TX    0
#define PIN_LED   1
#define PIN_RX    2
#define PIN_RELE  3

uint8_t radio_msg[LEN_BUFFER_RX];  //Data transmission buffer
uint8_t buflen = sizeof(radio_msg);
double count=0;
double count_max=0;
#define SAFE_TIMEOUT 180 //segundos
double safe_counter=0;
bool in =false;
  
/** \fn void setup
	It initializes the variables and configures the hardware
*/
void setup()
{
  wdt_disable();    // No te olvides
  delay(1000);
  //SerialUSB.begin(); 
  vw_set_tx_pin(PIN_TX);
  vw_set_rx_pin(PIN_RX);

  vw_setup(2000);   // Bits per sec
  vw_rx_start();       // Start the receiver PLL running 

  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_RELE, OUTPUT);
  digitalWrite(PIN_RELE, false);
  digitalWrite(PIN_LED, false);

  wdt_enable(WDTO_1S);

}

void loop() 
{
  char ack_msg[12];
  if (vw_get_message(radio_msg, &buflen) && (buflen==8)) // Non-blocking
  //if (vw_have_message())
  {
    vw_rx_stop();
    if(comparar("0001-ON_", 8) == 0){
      digitalWrite(PIN_RELE, true);
      send("0100-ON-ACK");
      count_max = 25000;
      safe_counter = 0;
    }
    else if(comparar("0001-OFF", 8) == 0)
    {
      stop_heater();
    } 
    vw_rx_start();
  }
  else
  {
    if (count==0)
    {
      if (in == true)
        in = false;
      else
      {
       in = true;
       safe_counter++;
       if (safe_counter >= SAFE_TIMEOUT)
       {
        stop_heater();
       }
      }
      digitalWrite(PIN_LED, in); 
      count=count_max;
    }
    else count--;
  }
  wdt_reset();
}

void stop_heater()
{
  digitalWrite(PIN_RELE, false);
  send("0100-OFF-ACK");
  count_max = 50000; 
}
//Funcion para enviar el mensaje
void send (char *message)
{
  vw_send((uint8_t *)message, strlen(message)); //Envia el mensaje
  vw_wait_tx(); //Espera hasta que se haya acabado de transmitir todo
}

char comparar(char* cadena, int len) {
  //Esta funcion compara el string cadena con el mensaje recibido.
  //Si son iguales, devuelve 1. Si no, devuelve 0.
  
  for(int i = 0; i<len; i++)
  {
    if(radio_msg[i] != cadena[i])
    {
      return 1;
    }
  }
 
  return 0;
}

