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

#include "driverlib.h"
#include "hal_LCD.h"

#define   Num_of_Results   8

volatile uint16_t A0results[Num_of_Results];
volatile uint16_t A1results[Num_of_Results];
volatile uint16_t A2results[Num_of_Results];
volatile uint16_t A3results[Num_of_Results];

void main (void)
{
    //Stop Watchdog Timer
    WDT_A_hold(WDT_A_BASE);

    //Enable A/D channel inputs - Port P1
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
        GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN2 +
        GPIO_PIN3,
        GPIO_TERNARY_MODULE_FUNCTION
        );

    /*
     * Disable the GPIO power-on default high-impedance mode to activate
     * previously configured port settings
     */
    PMM_unlockLPM5();


    //Initialize the ADC12B Module
    /*
    * Base address of ADC12B Module
    * Use internal ADC12B bit as sample/hold signal to start conversion
    * USE MODOSC 5MHZ Digital Oscillator as clock source
    * Use default clock divider/pre-divider of 1
    * Not use internal channel
    */
    ADC12_B_initParam initParam = {0};
    initParam.sampleHoldSignalSourceSelect = ADC12_B_SAMPLEHOLDSOURCE_SC;
    initParam.clockSourceSelect = ADC12_B_CLOCKSOURCE_ADC12OSC;
    initParam.clockSourceDivider = ADC12_B_CLOCKDIVIDER_1;
    initParam.clockSourcePredivider = ADC12_B_CLOCKPREDIVIDER__1;
    initParam.internalChannelMap = ADC12_B_NOINTCH;
    ADC12_B_init(ADC12_B_BASE, &initParam);

    //Enable the ADC12B module
    ADC12_B_enable(ADC12_B_BASE);

    /*
    * Base address of ADC12B Module
    * For memory buffers 0-7 sample/hold for 16 clock cycles
    * For memory buffers 8-15 sample/hold for 4 clock cycles (default)
    * Enable Multiple Sampling
    */
    ADC12_B_setupSamplingTimer(ADC12_B_BASE,
      ADC12_B_CYCLEHOLD_16_CYCLES,
      ADC12_B_CYCLEHOLD_4_CYCLES,
      ADC12_B_MULTIPLESAMPLESENABLE);

    //Configure Memory Buffers

    /*
    * Base address of the ADC12B Module
    * Configure memory buffer 0
    * Map input A0 to memory buffer 0
    * Vref+ = AVcc
    * Vref- = EXT Positive
    * Memory buffer 0 is not the end of a sequence
    */
    ADC12_B_configureMemoryParam param0 = {0};
    param0.memoryBufferControlIndex = ADC12_B_MEMORY_0;
    param0.inputSourceSelect = ADC12_B_INPUT_A0;
    param0.refVoltageSourceSelect = ADC12_B_VREFPOS_AVCC_VREFNEG_VSS;
    param0.endOfSequence = ADC12_B_NOTENDOFSEQUENCE;
    ADC12_B_configureMemory(ADC12_B_BASE, &param0);

    /*
    * Base address of the ADC12B Module
    * Configure memory buffer 1
    * Map input A1 to memory buffer 1
    * Vref+ = AVcc
    * Vref- = EXT Positive
    * Memory buffer 1 is not the end of a sequence
    */
    ADC12_B_configureMemoryParam param1 = {0};
    param1.memoryBufferControlIndex = ADC12_B_MEMORY_1;
    param1.inputSourceSelect = ADC12_B_INPUT_A1;
    param0.refVoltageSourceSelect = ADC12_B_VREFPOS_AVCC_VREFNEG_VSS;
    param1.endOfSequence = ADC12_B_NOTENDOFSEQUENCE;
    ADC12_B_configureMemory(ADC12_B_BASE, &param1);

    /*
    * Base address of the ADC12B Module
    * Configure memory buffer 2
    * Map input A2 to memory buffer 2
    * Vref+ = AVcc
    * Vref- = EXT Positive
    * Memory buffer 2 is not the end of a sequence
    */
    ADC12_B_configureMemoryParam param2 = {0};
    param2.memoryBufferControlIndex = ADC12_B_MEMORY_2;
    param2.inputSourceSelect = ADC12_B_INPUT_A2;
    param0.refVoltageSourceSelect = ADC12_B_VREFPOS_AVCC_VREFNEG_VSS;
    param2.endOfSequence = ADC12_B_NOTENDOFSEQUENCE;
    ADC12_B_configureMemory(ADC12_B_BASE, &param2);

    /*
     * Base address of the ADC12B Module
     * Configure memory buffer 3
     * Map input A3 to memory buffer 3
     * Vr+ = AVcc
     * Vr- = AVss
     * Memory buffer 3 IS the end of a sequence
     */
    ADC12_B_configureMemoryParam param3 = {0};
    param3.memoryBufferControlIndex = ADC12_B_MEMORY_3;
    param3.inputSourceSelect = ADC12_B_INPUT_A3;
    param0.refVoltageSourceSelect = ADC12_B_VREFPOS_AVCC_VREFNEG_VSS;
    param3.endOfSequence = ADC12_B_ENDOFSEQUENCE;
    ADC12_B_configureMemory(ADC12_B_BASE ,&param3);

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

    //Enable/Start first sampling and conversion cycle
    /*
     * Base address of ADC12B Module
     * Start the conversion into memory buffer 0
     * Use the repeated sequence of channels
     */
    ADC12_B_startConversion(ADC12_B_BASE,
        ADC12_B_MEMORY_0,
        ADC12_B_REPEATED_SEQOFCHANNELS);

    //Enter LPM0, Enable interrupts
    __bis_SR_register(LPM0_bits + GIE);
    //For debugger
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
