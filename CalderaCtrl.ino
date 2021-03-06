/** \file CalderaCtrl.ino
 * Control for Caldera. RF communication
 * This program runs in the ATTINY85 hardware.
 * Miguel A. 10 Febrero 2018
 * Arduino Digispark ATTINY85
 * Default -16.5mhz
 */

/*! \mainpage Caldera control.
\section my-intro Introduction
 P3 OUTPUT DATA TX
 P2 INPUT DATA RX

*/
#include <VirtualWire.h>
//#include <DigiCDC.h>    //To use the terminal
#include <avr/wdt.h>
 
#define LEN_BUFFER_RX  8
#define PIN_RELE  0
#define PIN_LED   1
#define PIN_RX    2
#define PIN_TX    3

#define SAFE_TIMEOUT 4000 //segundos

uint8_t radio_msg[LEN_BUFFER_RX];  //Data transmission buffer
uint8_t buflen = sizeof(radio_msg);
double count=0;
double count_max=50000;    // 1 segundo;
double safe_counter=0;
  
/** \fn void setup
	It initializes the variables and configures the hardware
*/
void setup()
{
  pinMode(PIN_LED, OUTPUT);
  pinMode(PIN_RELE, OUTPUT);
  digitalWrite(PIN_RELE, false);
  digitalWrite(PIN_LED, false);
  
  wdt_disable();    // No te olvides
 
  //vw_set_tx_pin(PIN_TX);
  vw_set_rx_pin(PIN_RX);

  vw_setup(2000);   // Bits per sec
  vw_rx_start();       // Start the receiver PLL running 

  //wdt_enable(WDTO_1S);
}

void loop() 
{
  int n_loop;
  char ack_msg[12];
  buflen = LEN_BUFFER_RX;
  if (vw_get_message(radio_msg, &buflen) && (buflen==LEN_BUFFER_RX)) // Non-blocking
  {
    digitalWrite(PIN_LED, true); 
    //vw_rx_stop();
    if(comparar("0001-ON_", 8) == 0){
      digitalWrite(PIN_RELE, true);
      //send("0100-ON-ACK");
      safe_counter = 0;
      n_loop = 2;
    }
    else if(comparar("0001-OFF", 8) == 0)
    {
      stop_heater();
      n_loop = 1;
      safe_counter = 0;
    } 
    else
      n_loop = 0;
    //vw_rx_start();
    delay(250);
    digitalWrite(PIN_LED, false); 
    for (int x=0;x<n_loop;x++)
    {
      delay(250);
      digitalWrite(PIN_LED, true); 
      delay(250);
      digitalWrite(PIN_LED, false);  
    }
  }
  else
  {
    if (count==0)
    {
      safe_counter++;
      if (safe_counter >= SAFE_TIMEOUT)
      {
        stop_heater();
        safe_counter=0;
      }
      count=count_max;
    }
    else count--;
  }
  //wdt_reset();
}

void stop_heater()
{
  digitalWrite(PIN_RELE, false);
  //send("0100-OFF-ACK");
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

