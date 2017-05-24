/**
 

  File Name:
    main.c

  Summary:
    This is the BM70 relay board main file created on 2017/05/15 
 * always print LED_8 toggle 
 * alwyas LED_8 toggle
 * UART interrupt. When receive input by RX, LED_7 toggle

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  MPLAB(c) Code Configurator - 4.15
        Device            :  PIC18F45K22
        Driver Version    :  2.00
    The generated drivers are tested against the following:
        Compiler          :  XC8 1.35
        MPLAB             :  MPLAB X 3.40
*/

#include <stdio.h>
#include <string.h>
#include "mcc_generated_files/mcc.h"

void sendString(const char *dataPtr); // function prototype
/*
/*
                         Main application
 */
void main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    // Enable the Global Interrupts
	INTERRUPT_GlobalInterruptEnable();
   	INTERRUPT_PeripheralInterruptEnable();
    //printf("\nTis is a book\n");
	char data;		
    while (1)
    {
      
        // toggle LED array
        IO_RD7_Toggle();
        printf("\r\nLED_D8 toggle\r\n");
        // delay 500ms
        for (int count=0; count<20; count++){
            __delay_ms(25);
        } // end for
       
        // UART RX interrupt
        while(EUSART1_DataReady){ //check if any data is received
            data = EUSART1_Read();		// Read data received
            for (int flashCount=0; flashCount<20; flashCount++){
            IO_RD6_Toggle(); // D7 toggle
            for (int count=0; count<20; count++){
                __delay_ms(15);
            } // end inner for
            } // end outer for
            PIE1bits.TX1IE=0; // disable UART TX interrupt
            EUSART1_Write(data);			// Echo back the data received
            printf("\r\n");
        };	// end while(DataReady)
        
        
     
    } // end while(1))
    
} // end main

// get char stored in dataPtr
void sendString(const char *dataPtr)
{
    EUSART1_Initialize();
    while(*dataPtr!='\0'){
        EUSART1_Write(*dataPtr);
        dataPtr++;
    } // end while
} // end function
/**
 End of File
*/