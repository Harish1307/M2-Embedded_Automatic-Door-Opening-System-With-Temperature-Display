
#ifndef __AVR_ATmega328__
    #define __AVR_ATmega328__
#endif



#include<avr/io.h>
#include "uart.h"





char USARTReadChar()
{


    // wait untill a data is available


    while(!(UCSR0A & (1<<RXC0)))
    {
        // DO NOTHING


    }
    
    return UDR0;


}


void USARTWriteChar(char data)
{


    //wait untill the transmitter is ready


    while(!(UCSR0A & (1<<UDRE0)))
    {
        //do nothing


    }


    // now write the data to USART buffer
    UDR0=data;

}