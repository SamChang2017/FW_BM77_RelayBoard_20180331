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
    // initialize IO
    // site 0
    IO_RA0_SetLow(); //CD1_1 
    IO_RA1_SetLow(); //CD1_2 
    IO_RA2_SetLow(); //CD1_3 
    IO_RA3_SetLow(); //CD1_4 
    IO_RA4_SetLow(); //CD1_5 
    IO_RA5_SetLow(); //CD1_6 
    IO_RE0_SetLow(); //CD1_7 
    IO_RE1_SetLow(); //CD1_8 
    IO_RE2_SetLow(); //CD2_9 
    IO_RA6_SetLow(); //CD2_10 
    IO_RC0_SetLow(); //CD2_11 
    IO_RC1_SetLow(); //CD2_12 
    IO_RC2_SetLow(); //CD2_13 
    IO_RD0_SetLow(); //CD2_14 
    IO_RD1_SetLow(); //CD2_15 
    //==============================
    while (1)
    {
        /* for test
         printf("\r\nLED_D0 toggle\r\n");
        // delay 500ms
        for (int count=0; count<20; count++){
            __delay_ms(25);
        } // end for
         */
        // UART RX interrupt
        while(EUSART1_DataReady){ //check if any data is received
            // UART interrupt test
            RxData = EUSART1_Read();		// Read data received
            
            /* for test
             for (int flashCount=0; flashCount<20; flashCount++){
            IO_RD1_Toggle(); // D7 toggle
            for (int count=0; count<20; count++){
                __delay_ms(15);
            } // end inner for
            } // end outer for
             */
            // Echo back data for test
            PIE1bits.TX1IE=0; // disable UART TX interrupt
            EUSART1_Write(RxData);			// Echo back the data received
            printf("\r\n");
            // end echo
            
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
        case 88: // 'X' all relay open 
            // site 0
            IO_RA0_SetLow(); //CD1_1 
            IO_RA1_SetLow(); //CD1_2 
            IO_RA2_SetLow(); //CD1_3 
            IO_RA3_SetLow(); //CD1_4 
            IO_RA4_SetLow(); //CD1_5 
            IO_RA5_SetLow(); //CD1_6 
            IO_RE0_SetLow(); //CD1_7 
            IO_RE1_SetLow(); //CD1_8 
            IO_RE2_SetLow(); //CD2_9 
            IO_RA6_SetLow(); //CD2_10 
            IO_RC0_SetLow(); //CD2_11 
            IO_RC1_SetLow(); //CD2_12 
            IO_RC2_SetLow(); //CD2_13 
            IO_RD0_SetLow(); //CD2_14 
            IO_RD1_SetLow(); //CD2_15 
            IO_RB6_SetLow(); 
            break;
        case 2: // site 0; DUT UART open
            IO_RA0_SetLow(); //CD1_1 
            break;
        case 34: // site 0; DUT UART close
            IO_RA0_SetHigh(); //CD1_1
            break;
        
        //======IBDK mode=============
        case 4: // site 0; P20 open 
            IO_RA1_SetLow(); //CD1_2
            break;
        case 36: // site 0; P20 close
            IO_RA1_SetHigh(); //CD1_2
            break;
        case 5: // site 0; EAN open 
            IO_RB6_SetLow(); 
            break;
        case 37: // site 0; EAN close
            IO_RB6_SetHigh(); 
            break;
        //===========================
        case 6: // site 0; RST_N open
            IO_RA2_SetLow();  //CD1_3
            break;
        case 38: // site 0; RST_N close
            IO_RA2_SetHigh(); //CD1_3
            break;
        case 8: // site 0; LDO33=/=NC
            IO_RA3_SetLow(); // CD1_4
            break;
        case 40: // site 0; LDO33==NC
            IO_RA3_SetHigh(); //CD1_4
            break;
         case 10: // site 0; K_ChanB==LDO33
            IO_RA4_SetLow(); // CD1_5
            break;
        case 42: // site 0; K_ChanB==NC
            IO_RA4_SetHigh(); //CD1_5
            break;
        case 12: // site 0; LDO18
            IO_RA5_SetLow(); // CD1_6
            break;
        case 44: // site 0; PMUDLO
            IO_RA5_SetHigh(); //CD1_6
            break;
        // Date: 2017/05/26
        case 14: // site 0; K_CanB==PMULDO
            IO_RE0_SetLow(); // CD1_7
            break;
        case 46: // site 0; K_ChanB==LDO18
            IO_RE0_SetHigh(); //CD1_7
            break;
        case 16: // site 0; K_ChanB=\=P34
            IO_RE1_SetLow(); // CD1_8
            break;
        case 48: // site 0 K_ChanB==P34
            IO_RE1_SetHigh(); //CD1_8
            break;
        case 18: // site 0; GPIO=/=OSGANG
            IO_RE2_SetLow(); // CD2_9
            break;
        case 50: // site 0; GPIO==OSGANG
            IO_RE2_SetHigh(); //CD2_9
            break;
        case 20: // site 0; K_ChanB=/=P04
            IO_RA6_SetLow(); // CD2_10
            break;
        case 52: // site 0; K_ChanB==P04
            IO_RA6_SetHigh(); //CD2_10
            break;
        case 22: // site 0; K_ChanB=/=P05
            IO_RC0_SetLow(); // CD2_11
            break;
        case 54: // site 0; K_ChanB==P05
            IO_RC0_SetHigh(); //CD2_11
            break;
        case 24: // site 0; K_ChanB=/=P31
            IO_RC1_SetLow(); // CD2_12
            break;
        case 56: // site 0: K_ChanB==P31
            IO_RC1_SetHigh(); //CD2_12
            break;
        case 26: // site 0; K_ChanB=/=P32
            IO_RC2_SetLow(); // CD2_13
            break;
        case 58: // site 0; K_ChanB==P32
            IO_RC2_SetHigh(); //CD2_13
            break;
        case 28: // site 0; K_ChanB=/=P33
            IO_RD0_SetLow(); // CD2_14
            break;
        case 60: // site 0; K_ChanB==P33
            IO_RD0_SetHigh(); //CD2_14
            break;
        case 30: // site 0
            IO_RD1_SetLow(); // CD2_15
            break;
        case 62: // site 0
            IO_RD1_SetHigh(); //CD2_15
            break;
        case 120: // site 0
            IO_RA0_SetLow(); // CD1_1
            IO_RA1_SetLow(); // CD1_2
            IO_RA2_SetLow(); // CD1_3
            IO_RA3_SetLow(); // CD1_4
            IO_RA4_SetLow(); // CD1_5
            IO_RA5_SetLow(); // CD1_6
            IO_RE0_SetLow(); // CD1_7
            IO_RE1_SetLow(); // CD1_8
            IO_RE2_SetLow(); // CD2_9
            IO_RA6_SetLow(); // CD2_10
            IO_RC0_SetLow(); // CD2_11
            IO_RD0_SetLow(); // CD2_14
            IO_RD1_SetLow(); // CD2_15
            break;    
        default:
            EUSART1_Write('U');
            break;
    } // end switch
} // end function
/**
 End of File
*/