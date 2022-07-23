/**
 * LICENSE: Apache 2.0
 * Reference: github.com/zthurman/pocdaq
 *
 * Simple analog input scanner on the MSP430FR6989.
 *
 * This is a bastard amalgamation of:
 *  msp430_driverlib_2_91_13_01\examples\MSP430FR5xx_6xx\adc12_b\adc12_b_ex1_avccRef.c
 *  msp430_driverlib_2_91_13_01\examples\MSP430FR5xx_6xx\adc12_b\adc12_b_ex3_extRef.c
 *  msp430_driverlib_2_91_13_01\examples\MSP430F5xx_6xx\adc12_a\adc12_a_ex4_repeatedSeq.c
 *  OutOfBox_MSP430FR6989
 *
 * The purpose of this project is to provide basic
 * analog scanner functionality for proof-of-concept data
 * acquisition in the analog input space. Eventual
 * functionality expansions are more or less obvious to
 * those familiar with industrial control systems and
 * are willing to explore more cost effective options
 * than the "traditional" options.
 *
 * References:
 * 1 - MSP430FR698x(1), MSP430FR598x(1) Mixed-Signal Microcontrollers datasheet (Rev. D)
 * 2 - Designing With MSP430FR58xx/FR59xx/68xx/69xx ADC (Rev. A)
 * 3 - MSP430x5xx and MSP430x6xx Family User's Guide (Rev. Q)
 * 4 - msp430_driverlib_2_91_13_01
 */

#include <driverlib.h>
#include "hal_LCD.h"
#include "adc.h"

#define Num_of_Results  8
#define STARTUP_MODE    0
#define MSP6989_CONF    1

volatile uint16_t A0results[Num_of_Results];
volatile uint16_t A1results[Num_of_Results];
volatile uint16_t A2results[Num_of_Results];
volatile uint16_t A3results[Num_of_Results];
volatile uint16_t A4results[Num_of_Results];
volatile uint16_t A5results[Num_of_Results];
volatile uint16_t A6results[Num_of_Results];
volatile uint16_t A7results[Num_of_Results];
volatile uint16_t A8results[Num_of_Results];
volatile uint16_t A9results[Num_of_Results];
volatile uint16_t A10results[Num_of_Results];
volatile uint16_t A11results[Num_of_Results];
volatile uint16_t A12results[Num_of_Results];
volatile uint16_t A13results[Num_of_Results];
volatile uint16_t A14results[Num_of_Results];
volatile uint16_t A15results[Num_of_Results];
volatile unsigned char mode = STARTUP_MODE;
volatile unsigned char conf = MSP6989_CONF;

void main (void)
{
    /* Stop Watchdog Timer
     * Example code in Section 8.6.5 of Reference 3.
     * Wrapped by the wdt_a driver in this implementation.
     */
    WDT_A_hold(WDT_A_BASE);

    /*
     * Farmed out to adc suite.
     */
    Init_GPIO_For_ADC12_B_All_AI();

    /*
     * Disable the GPIO power-on default high-impedance mode to activate
     * previously configured port settings.
     * Comes from the Note in Section 6.11.1 in Reference 1.
     */
    PMM_unlockLPM5();

    // LCD Shenanigans
    __enable_interrupt();
    Init_LCD();
    GPIO_clearInterrupt(GPIO_PORT_P1, GPIO_PIN1);
    GPIO_clearInterrupt(GPIO_PORT_P1, GPIO_PIN2);
    __enable_interrupt();
    displayScrollText("WELCOME TO THE AI SCANNER");

    /*
     * Farmed out to adc suite.
     */
    Init_Enable_ADC12_B();

    /*
     * Farmed out to adc suite.
     */
    Config_Mem_Buffers();

    //Enable memory buffer 3 interrupt
    ADC12_B_clearInterrupt(ADC12_B_BASE,
        0,
        ADC12_B_IFG3
        );

    //Enable memory buffer 0 interrupt
    ADC12_B_enableInterrupt(ADC12_B_BASE,
      ADC12_B_IE3,
      0,
      0);

    // Enable/Start first sampling and conversion cycle
    /*
     * Base address of ADC12B Module
     * Start the conversion into memory buffer 0
     * Use the repeated sequence of channels
     */
    ADC12_B_startConversion(ADC12_B_BASE,
        ADC12_B_MEMORY_0,
        ADC12_B_REPEATED_SEQOFCHANNELS);

    // Enter LPM0, Enable interrupts
    __bis_SR_register(LPM0_bits + GIE);
    // For debugger
    __no_operation();
}

#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector=ADC12_VECTOR
__interrupt
#elif defined(__GNUC__)
__attribute__((interrupt(ADC12_VECTOR)))
#endif
void ADC12ISR (void)
{
    static uint16_t index = 0;

    switch (__even_in_range(ADC12IV,34)){
        case  0: break;   //Vector  0:  No interrupt
        case  2: break;   //Vector  2:  ADC overflow
        case  4: break;   //Vector  4:  ADC timing overflow
        case  6: break;   //Vector  6:  ADC12IFG0
        case  8: break;   //Vector  8:  ADC12IFG1
        case 10: break;   //Vector 10:  ADC12IFG2
        case 12:          //Vector 12:  ADC12IFG3
            //Move A0 results, IFG is cleared
            A0results[index] =
                ADC12_B_getResults(ADC12_B_BASE,
                    ADC12_B_MEMORY_0);
            //Move A1 results, IFG is cleared
            A1results[index] =
                ADC12_B_getResults(ADC12_B_BASE,
                    ADC12_B_MEMORY_1);
            //Move A2 results, IFG is cleared
            A2results[index] =
                ADC12_B_getResults(ADC12_B_BASE,
                    ADC12_B_MEMORY_2);
            //Move A3 results, IFG is cleared
            A3results[index] =
                ADC12_B_getResults(ADC12_B_BASE,
                    ADC12_B_MEMORY_3);
            //Move A4 results, IFG is cleared
            A4results[index] =
                ADC12_B_getResults(ADC12_B_BASE,
                    ADC12_B_MEMORY_4);
            //Move A5 results, IFG is cleared
            A5results[index] =
                ADC12_B_getResults(ADC12_B_BASE,
                    ADC12_B_MEMORY_5);
            //Move A6 results, IFG is cleared
            A6results[index] =
                ADC12_B_getResults(ADC12_B_BASE,
                    ADC12_B_MEMORY_6);
            //Move A7 results, IFG is cleared
            A7results[index] =
                ADC12_B_getResults(ADC12_B_BASE,
                    ADC12_B_MEMORY_7);
            //Move A8 results, IFG is cleared
            A8results[index] =
                ADC12_B_getResults(ADC12_B_BASE,
                    ADC12_B_MEMORY_8);
            //Move A9 results, IFG is cleared
            A9results[index] =
                ADC12_B_getResults(ADC12_B_BASE,
                    ADC12_B_MEMORY_9);
            //Move A10 results, IFG is cleared
            A10results[index] =
                ADC12_B_getResults(ADC12_B_BASE,
                    ADC12_B_MEMORY_10);
            //Move A11 results, IFG is cleared
            A11results[index] =
                ADC12_B_getResults(ADC12_B_BASE,
                    ADC12_B_MEMORY_11);
            //Move A12 results, IFG is cleared
            A12results[index] =
                ADC12_B_getResults(ADC12_B_BASE,
                    ADC12_B_MEMORY_12);
            //Move A13 results, IFG is cleared
            A13results[index] =
                ADC12_B_getResults(ADC12_B_BASE,
                    ADC12_B_MEMORY_13);
            //Move A14 results, IFG is cleared
            A14results[index] =
                ADC12_B_getResults(ADC12_B_BASE,
                    ADC12_B_MEMORY_14);
            //Move A15 results, IFG is cleared
            A15results[index] =
                ADC12_B_getResults(ADC12_B_BASE,
                    ADC12_B_MEMORY_15);
            //Increment results index, modulo; Set BREAKPOINT here
            index++;

            if (index == 8){
                (index = 0);
            }
        case 14: break;   //Vector 14:  ADC12IFG4
        case 16: break;   //Vector 16:  ADC12IFG5
        case 18: break;   //Vector 18:  ADC12IFG6
        case 20: break;   //Vector 20:  ADC12IFG7
        case 22: break;   //Vector 22:  ADC12IFG8
        case 24: break;   //Vector 24:  ADC12IFG9
        case 26: break;   //Vector 26:  ADC12IFG10
        case 28: break;   //Vector 28:  ADC12IFG11
        case 30: break;   //Vector 30:  ADC12IFG12
        case 32: break;   //Vector 32:  ADC12IFG13
        case 34: break;   //Vector 34:  ADC12IFG14
        default: break;
    }
}
