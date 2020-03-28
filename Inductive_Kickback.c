/*
 * File:   Inductive_Kickback.c
 * Author: Carlos Oscar Torres Facio.
 * Description: Testing Inductive Kickback of Motors.
 * Created on May 1, 2019, 1:07 AM
 */
#include "xc.h"
#include "BOARD.h"
#include "AD.h"
#include "RC_Servo.h"
#include "serial.h"
#include "LED.h"
#include "pwm.h"
#include <stdio.h>
#define MINPULSE    1000
#define MAXPULSE    2000
#define ZERO        1015
#define MIN_PWM 0
#define MAX_PWM 1000

int main(void) {
    unsigned int Digital_Signal;
    unsigned int Digital_Val;
    unsigned int Shift; 
    unsigned int LEDs;
    unsigned int Result; 
    unsigned int Invert;

    LED_Init();   
    BOARD_Init();                // Initialization for the BOARD/UNO32.
    AD_Init();  
    PWM_Init();                  // Initialization for the Analog signal to Digital signal.
    AD_AddPins(AD_PORTW3);       // Adding pin used for analog signal input: AD_PORTW3.
    PWM_AddPins(PWM_PORTX11);    // Adding pin used for digital signal output: RC_PORTV03.
    
    LEDs = 0b111111111111;       // 12 LEDs that need to be control by the pontentiometer's values
    LED_AddBanks(LED_BANK1);     // Adding LEDs Bank 1, 2, & 3. 
    LED_AddBanks(LED_BANK2);
    LED_AddBanks(LED_BANK3);
    
    while (1){
        Digital_Val = AD_ReadADPin(AD_PORTW3)+ MINPULSE; // Value from the potentiometer ranges from 0 - 1023.
        Shift = AD_ReadADPin(AD_PORTW3)/79;              // Needs to add MINPULSE = 1000.
        Result = LEDs<<Shift;                            // Shift value required to move bits to left. 
        Invert = ~Result;                                 // Invert the value of the Result variable. 
        Digital_Signal = (AD_ReadADPin(AD_PORTW3)*1000)/1023;

        if (Digital_Signal >= MAX_PWM){
            Digital_Signal = MAX_PWM;
            //printf("YOU ARE AT THE 2000");
        }
        if (Digital_Signal <= MIN_PWM){
            Digital_Signal = 0;
        }

        PWM_SetDutyCycle(PWM_PORTX11,Digital_Signal);
        LED_SetBank(LED_BANK3,Invert>>8);                // Shifts Invert value by 8, 4, and 0. 
        LED_SetBank(LED_BANK2,Invert>>4);
        LED_SetBank(LED_BANK1,Invert);
    }
    return 0;
}

