/*
 * File:   UARTBlutooth.c
 * Author: covin
 *
 * Created on May 2, 2022, 9:04 AM
 */


#include "xc.h"
#include "UARTBluetooth.h"
#include "I2CDisplay.h"

char buffer[1000];
int bufferPointer = 0;
int bufferRead = 0;
char lastReceived;
char b;

void __attribute__((__interrupt__, __auto_psv__)) _U1RXInterrupt(void) {
    buffer[bufferPointer] = U1RXREG;
    lastReceived = U1RXREG;
    _U1RXIF = 0;
    
    if(buffer[bufferPointer] == 49) {
        b = '1';
    } 
    else if(buffer[bufferPointer] == 50) {
        b = '2';
    }
    else if(buffer[bufferPointer] == 51) {
        b = '3';
    }
    else if(buffer[bufferPointer] == 52) {
        b = '4';
    }
    else if(buffer[bufferPointer] == 53) {
        b = '5';
    }
    else if(buffer[bufferPointer] == 54) {
        b = '6';
    }
    else if(buffer[bufferPointer] == 55) {
        b = '7';
    }
    else if(buffer[bufferPointer] == 56) {
        b = '8';
    }
    else if(buffer[bufferPointer] == 57) {
        b = '9';
    }
    bufferPointer++;
}

char returnReceivedValue(){
    char A=b;
    return A;
}

void initUart(void) {
    _TRISB6 = 1;
    _TRISB7 = 0;
    CNPU2bits.CN24PUE = 1;
    
    __builtin_write_OSCCONL(OSCCON & 0xbf); // unlock PPS
    _RP7R = 0x0003;   //RB6->UART1:U1TX; See Table 10-3 on P109 of the datasheet
    _U1RXR = 6;   //RB7->UART1:U1RX;
    __builtin_write_OSCCONL(OSCCON | 0x40); // lock   PPS
   
IPC3bits.U1TXIP2 = 1; //Set Uart TX Interrupt Priority
IPC3bits.U1TXIP1 = 0;
IPC3bits.U1TXIP0 = 0;
IPC2bits.U1RXIP2 = 1; //Set Uart RX Interrupt Priority
IPC2bits.U1RXIP1 = 0;
IPC2bits.U1RXIP0 = 0;
    
    U1BRG = 415;
    U1STA = 0;
    U1MODE = 0x8000;
    U1MODEbits.BRGH = 1;
    U1MODEbits.UEN = 0;
    U1MODEbits.PDSEL = 0;
    U1STAbits.UTXEN = 1; // Transmission
    
    _U1RXIE = 1;
    _U1RXIF = 0;
    _U1TXIE = 0;
    _U1TXIF = 0;
}