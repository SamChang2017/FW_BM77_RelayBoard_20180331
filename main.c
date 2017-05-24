/**
  Summary:
    This is the BM70 relay board main file created on 2017/05/24
 * always print LED_0 toggle 
 * UART interrupt. 
 * When receive 'T' write TFnum
 * When receive 'I' write loss
 * When receive 'a' set low RD_0
 * When receive 'b' set high RD_0
 * default write 'U'
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
void relayControl(char data); // function prototype
// define global variable
char TFNum; // Test Fixture number 
char site0_IL[16]; // jig loss

void main(void)
{
    // initialize the device
    SYSTEM_Initialize();
    // Enable the Global Interrupts
	INTERRUPT_GlobalInterruptEnable();
   	INTERRUPT_PeripheralInterruptEnable();
    //define variable
    char RxData; // character from UART_RX 
    // initialize variable 
    TFNum = '5';                        //Test Fixture #
    site0_IL[0] = '3';                  //TX Insertion Loss
    site0_IL[1] = '.';                  //COAX_IL + AntennaFP_TP_IL
    site0_IL[2] = '9';                  //2.402 GHz
    site0_IL[3] = '4';
    
    site0_IL[4] = '3';                  //RX Insertion Loss
    site0_IL[5] = '7';                  //2.402 GHx
    site0_IL[6] = '.';                  //AntennaFP_TP_IL + Shield
    site0_IL[7] = '9';
    
    site0_IL[8] = '1';                  //2.440 GHz
    site0_IL[9] = '5';                  //AntennaFP_TP_IL + Shield
    site0_IL[10] = '.';
    site0_IL[11] = '4';
    
    site0_IL[12] = '5';                 //2.480 GHz
    site0_IL[13] = '.';                 //AntennaFP_TP_IL + Shield 
    site0_IL[14] = '8';
    site0_IL[15] = '7';
    //==============================
    while (1)
    {
        printf("\r\nLED_D0 toggle\r\n");
        // delay 500ms
        for (int count=0; count<20; count++){
            __delay_ms(25);
        } // end for
       
        // UART RX interrupt
        while(EUSART1_DataReady){ //check if any data is received
            // UART interrupt test
            RxData = EUSART1_Read();		// Read data received
            for (int flashCount=0; flashCount<20; flashCount++){
            IO_RD1_Toggle(); // D7 toggle
            for (int count=0; count<20; count++){
                __delay_ms(15);
            } // end inner for
            } // end outer for
            PIE1bits.TX1IE=0; // disable UART TX interrupt
            EUSART1_Write(RxData);			// Echo back the data received
            printf("\r\n");
            // Relay board control
            relayControl(RxData);
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

// control relay board
void relayControl(char data){
    switch (data){
        case 84: //'T' write test fixture number by UART
            EUSART1_Write(TFNum);
            break;
        case 73: // 'I' write jig loss by UART
            for (int count=0; count<16; count++){
                EUSART1_Write(site0_IL[count]);
                __delay_ms(25);
            } // end for
            break;
        case 97: // 'a' set low RD_0
            IO_RD0_SetLow();
            break;
        case 98: // 'b' set high RD_0
            IO_RD0_SetHigh();
            break;
        default:
            EUSART1_Write('U');
            break;
    } // end switch
} // end function
/**
 End of File
*/