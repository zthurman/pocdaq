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

#define     RED             0x0001
#define     RED_OFF         0x00FE
#define     ENABLE_RED      0xFFFE

unsigned short ADCValue0=0;
unsigned short ADCValue1=0;
unsigned short ADCValue2=0;

int usefulGwarbage(void)
{
    // Disable Watchdog
    // Example code in Section 8.6.5 of Reference 3
    WDTCTL = WDTPW | WDTHOLD;

    // Clock Configuration:
    // Default

    // LED for blinking
    P1DIR = RED;

    // Configure ADC A0-A15 pins for ADC12.
    // Table 6-20 in Reference 1.
    // P1.0 - A0
    P1SEL1 |= BIT0;
    P1SEL0 |= BIT0;
    // P1.1 - A1
    P1SEL1 |= BIT1;
    P1SEL0 |= BIT1;
    // P1.2 - A2
    P1SEL1 |= BIT2;
    P1SEL0 |= BIT2;
    // P1.3 - A3
    P1SEL1 |= BIT3;
    P1SEL0 |= BIT3;
    // Table 6-36 in Reference 1.
    // P8.7 - A4
    P8SEL1 |= BIT7;
    P8SEL0 |= BIT7;
    // P8.6 - A5
    P8SEL1 |= BIT6;
    P8SEL0 |= BIT6;
    // P8.5 - A6
    P8SEL1 |= BIT5;
    P8SEL0 |= BIT5;
    // P8.4 - A7
    P8SEL1 |= BIT4;
    P8SEL0 |= BIT4;
    // Table 6-37 in Reference 1.
    // P9.0 - A8
    P9SEL1 |= BIT0;
    P9SEL0 |= BIT0;
    // P9.1 - A9
    P9SEL1 |= BIT1;
    P9SEL0 |= BIT1;
    // P9.2 - A10
    P9SEL1 |= BIT2;
    P9SEL0 |= BIT2;
    // P9.3 - A11
    P9SEL1 |= BIT3;
    P9SEL0 |= BIT3;
    // Table 6-38 in Reference 1.
    // P9.4 - A12
    P9SEL1 |= BIT4;
    P9SEL0 |= BIT4;
    // P9.5 - A13
    P9SEL1 |= BIT5;
    P9SEL0 |= BIT5;
    // P9.6 - A14
    P9SEL1 |= BIT6;
    P9SEL0 |= BIT6;
    // P9.7 - A15
    P9SEL1 |= BIT7;
    P9SEL0 |= BIT7;

    /*
     * Comes from the Note in Section 6.11.1 in Reference 1.
     */
    PM5CTL0 &= ~LOCKLPM5;

    /* This controls the reference system.
     * It makes a little bit more sense after you read
     * Section 26.3.1 of Reference 3. I can't say that
     * I'm 100% confident but it appears to set the internal
     * reference to 2.5V.
     */
    while(REFCTL0 & REFGENBUSY);
    REFCTL0 |= REFVSEL_3 | REFON;

    // Configure ADC 12
    ADC12CTL0 = ADC12SHT0_2 | ADC12ON;
    ADC12CTL1 = ADC12SHP | ADC12CONSEQ_2;
    ADC12CTL2 = ADC12RES_2;

    // Enable ADC conversion complete interrupt
    // Section 2.2 Reference 2
    ADC12IER0 |= ADC12IE0|ADC12IE1|ADC12IE2|ADC12IE3|ADC12IE4|ADC12IE5|ADC12IE6|ADC12IE7|ADC12IE8|ADC12IE9|ADC12IE10|ADC12IE11|ADC12IE12|ADC12IE13|ADC12IE14|ADC12IE15;

    // Read the input channels
    // Table 28-8 from Reference 3
    ADC12MCTL0 = ADC12VRSEL_1 | ADC12INCH_0;
    ADC12MCTL1 = ADC12VRSEL_1 | ADC12INCH_1;
    ADC12MCTL2 = ADC12VRSEL_1 | ADC12INCH_2;
    ADC12MCTL3 = ADC12VRSEL_1 | ADC12INCH_3;
    ADC12MCTL4 = ADC12VRSEL_1 | ADC12INCH_4;
    ADC12MCTL5 = ADC12VRSEL_1 | ADC12INCH_5;
    ADC12MCTL6 = ADC12VRSEL_1 | ADC12INCH_6;
    ADC12MCTL7 = ADC12VRSEL_1 | ADC12INCH_7;
    ADC12MCTL8 = ADC12VRSEL_1 | ADC12INCH_8;
    ADC12MCTL9 = ADC12VRSEL_1 | ADC12INCH_9;
    ADC12MCTL10 = ADC12VRSEL_1 | ADC12INCH_10;
    ADC12MCTL11 = ADC12VRSEL_1 | ADC12INCH_11;
    ADC12MCTL12 = ADC12VRSEL_1 | ADC12INCH_12;
    ADC12MCTL13 = ADC12VRSEL_1 | ADC12INCH_13;
    ADC12MCTL14 = ADC12VRSEL_1 | ADC12INCH_14;
    ADC12MCTL15 = ADC12VRSEL_1 | ADC12INCH_15 + ADC12EOS; // last channel, end of sequence

    while(1)
    {
        P1OUT = RED;
        __delay_cycles(5000);

        ADC12CTL0 |= ADC12ENC | ADC12SC;
        __bis_SR_register(LPM0_bits + GIE);
        P1OUT = RED_OFF;
        __no_operation();
    }

}

