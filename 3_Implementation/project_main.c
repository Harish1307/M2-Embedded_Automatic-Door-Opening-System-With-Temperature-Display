#ifndef __AVR_ATmega328__
    #define __AVR_ATmega328__
#endif
#define LTHRES 500
#define RTHRES 500

#include <avr/io.h>
//#include <util/delay.h>
#include "adc.h"
#include "project_config.h"
#include "DHT.h"
#include "uart.h"
#include "user_utils.h"

void adc_init()
{
    // AREF = AVcc
    ADMUX = (1<<REFS0);
 
    // ADC Enable and prescaler of 128
    // 16000000/128 = 125000
    ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}

void USARTInit(uint16_t ubrr_value)
{


    //set Baud rate


    UBRR0L = ubrr_value;

    UBRR0H = (ubrr_value>>8) & 0x00ff;

    UCSR0C = (1<<UMSEL00) | (1<<UCSZ01) | (1<<UCSZ00);


    // Enable the receiver and transmitter


    UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0)|(1<<TXCIE0);


}

void initDHT(void)
{
    /* Set LED as output */
    SET_BIT(DDRB,LED);
    
    /* According to the DHT11's datasheet, the sensor needs about
       1-2 seconds to get ready when first getting power, so we
       wait
     */
    _delay_ms(2000);
}

 
int main() 

{
   uint16_t adc_result0, adc_result1;
   uint8_t data [4];
    
    int8_t i;
    DDRB|=(1<<PORTB0);           // to connect led to PC0
 
    // initialize adc
    adc_init();
    
 
    while(1)
    {
        adc_result0 = adc_read(0);      // read adc value at PA0
        adc_result1 = adc_read(1);      // read adc value at PA1
 
        // condition for led to glow
        if (adc_result0 < LTHRES && adc_result1 < RTHRES)
        {
            USARTInit(103);
            PORTB = 0x01;
            USARTWriteChar(PORTB);
            initDHT();
            if(fetchData(data))
            {
                for(i = data[2]; i >= 0; --i)
                {       
                    SET_BIT(PORTB,LED);
                    _delay_ms(100);
            
                    CLEAR_BIT(PORTB,LED);
                    _delay_ms(500);
                }
            }
            PORTB |=(1<<PORTB0);
        
        } 

        else
            PORTB &=~(1<<PORTB0);
}

}

