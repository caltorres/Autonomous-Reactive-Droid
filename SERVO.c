/*
 * File:   SERVO.c
 * Author: Carlos Oscar Torres Facio.
 * Description: Testing Servo Motion.
 * Created on April 29, 2019, 9:56 PM
 */

#include "xc.h"
#include "BOARD.h"
#include "AD.h"
#include "RC_Servo.h"
#include "serial.h"
#include "LED.h"
#include <stdio.h>

#define DELAY(x)    for (wait = 0; wait <= x; wait++) {asm("nop");}
#define FlushPrintBuffer()       //while(!IsTransmitEmpty())
#define A_BIT       18300
#define A_LOT       183000
#define MINPULSE    1000
#define MAXPULSE    2000
#define ZERO        1015

int main(void) {
    unsigned int wait = 0;       // This unsigned wait variable is needed for the delay function.
    unsigned int Digital_Val;
    unsigned int Shift; 
    unsigned int LEDs;
    unsigned int Result; 
    unsigned int Invert;
    
    LED_Init();                  // Initialization for the LEDs.
    RC_Init();                   // Initialization for the RC_Servo.
    BOARD_Init();                // Initialization for the BOARD/UNO32.
    AD_Init();                   // Initialization for the Analog signal to Digital signal.
    AD_AddPins(AD_PORTW3);       // Adding pin used for analog signal input: AD_PORTW3.
    RC_AddPins(RC_PORTV03);      // Adding pin used for digital signal output: RC_PORTV03.
   
    LEDs = 0b111111111111;       // 12 LEDs that need to be control by the pontentiometer's values
    LED_AddBanks(LED_BANK1);     // Adding LEDs Bank 1, 2, & 3. 
    LED_AddBanks(LED_BANK2);
    LED_AddBanks(LED_BANK3);
   
    // The code below allows us to control the RC_Servo using the values from
    // the potentiometer.
    while (1){
        
        Digital_Val = AD_ReadADPin(AD_PORTW3)+ MINPULSE; // Value from the potentiometer ranges from 0 - 1023.
        Shift = AD_ReadADPin(AD_PORTW3)/79;              // Needs to add MINPULSE = 1000.
        Result = LEDs<<Shift;                            // Shift value required to move bits to left. 
        Invert = ~Result;                                // Invert the value of the Result variable. 
                       
        if (Digital_Val >= MAXPULSE){
            Digital_Val = MAXPULSE;
        }
        if (Digital_Val <= ZERO){
            Digital_Val = 0;

        }
        RC_SetPulseTime(RC_PORTV03, Digital_Val);        // Set pulse time to the output pin
                                            		     // Given the value from the potentiometer.
        LED_SetBank(LED_BANK3,Invert>>8);                // Shifts Invert value by 8, 4, and 0. 
        LED_SetBank(LED_BANK2,Invert>>4);
        LED_SetBank(LED_BANK1,Invert);
    }
    return 0;
}
