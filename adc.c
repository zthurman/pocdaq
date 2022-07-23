/**
 * LICENSE: Apache 2.0
 * Reference: github.com/zthurman/pocdaq
 *
 * Sets up ADC12_B on MSP430FR6989.
 *
 * References:
 * 1 - MSP430FR698x(1), MSP430FR598x(1) Mixed-Signal Microcontrollers datasheet (Rev. D)
 * 2 - Designing With MSP430FR58xx/FR59xx/68xx/69xx ADC (Rev. A)
 * 3 - MSP430x5xx and MSP430x6xx Family User's Guide (Rev. Q)
 * 4 - msp430_driverlib_2_91_13_01
 *
 */

#include <driverlib.h>
#include "adc.h"

void Init_GPIO_For_ADC12_B_All_AI()
{
    /*
     * Can't use Port 1 pins 0, 1, or 2 on the LaunchPad MSP430FR6989,
     * similarly Port 9 pin 7 isn't landed on a pin for use that I'm
     * seeing.
     *
     * For a custom PCB mounted case change the MSP6989_CONF to 0.
     */
    if (conf == 1)
    {
        /* Enable A/D channel inputs - Port P1
         * Table 6-20 in Reference 1.
         */
        GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
            GPIO_PIN3,
            GPIO_TERNARY_MODULE_FUNCTION
            );
        /* Enable A/D channel inputs - Port P9
         * Tables 6-37 & 6-38 in Reference 1.
         */
        GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P9,
            GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN2 +
            GPIO_PIN3 + GPIO_PIN4 + GPIO_PIN5 +
            GPIO_PIN6,
            GPIO_TERNARY_MODULE_FUNCTION
            );
    }
    else
    {
        /* Enable A/D channel inputs - Port P1
         * Table 6-20 in Reference 1.
         */
        GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
            GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN2 +
            GPIO_PIN3,
            GPIO_TERNARY_MODULE_FUNCTION
            );

        /* Enable A/D channel inputs - Port P9
         * Tables 6-37 & 6-38 in Reference 1.
         */
        GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P9,
            GPIO_PIN0 + GPIO_PIN1 + GPIO_PIN2 +
            GPIO_PIN3 + GPIO_PIN4 + GPIO_PIN5 +
            GPIO_PIN6 + GPIO_PIN7,
            GPIO_TERNARY_MODULE_FUNCTION
            );
    }

    /* Enable A/D channel inputs - Port P8
     * Table 6-36 in Reference 1.
     */
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P8,
        GPIO_PIN7 + GPIO_PIN6 + GPIO_PIN5 +
        GPIO_PIN4,
        GPIO_TERNARY_MODULE_FUNCTION
        );
}

void Init_Enable_ADC12_B()
{
    // Initialize the ADC12B Module
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

    // Enable the ADC12B module
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
}

void Config_Mem_Buffers()
{

    // Configure Memory Buffers
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
    param1.refVoltageSourceSelect = ADC12_B_VREFPOS_AVCC_VREFNEG_VSS;
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
    param2.refVoltageSourceSelect = ADC12_B_VREFPOS_AVCC_VREFNEG_VSS;
    param2.endOfSequence = ADC12_B_NOTENDOFSEQUENCE;
    ADC12_B_configureMemory(ADC12_B_BASE, &param2);

    /*
     * Base address of the ADC12B Module
     * Configure memory buffer 3
     * Map input A3 to memory buffer 3
     * Vr+ = AVcc
     * Vr- = AVss
     * Memory buffer 3 is not the end of a sequence
     */
    ADC12_B_configureMemoryParam param3 = {0};
    param3.memoryBufferControlIndex = ADC12_B_MEMORY_3;
    param3.inputSourceSelect = ADC12_B_INPUT_A3;
    param3.refVoltageSourceSelect = ADC12_B_VREFPOS_AVCC_VREFNEG_VSS;
    param3.endOfSequence = ADC12_B_NOTENDOFSEQUENCE;
    ADC12_B_configureMemory(ADC12_B_BASE ,&param3);

    /*
     * Base address of the ADC12B Module
     * Configure memory buffer 4
     * Map input A4 to memory buffer 4
     * Vr+ = AVcc
     * Vr- = AVss
     * Memory buffer 4 is not the end of a sequence
     */
    ADC12_B_configureMemoryParam param4 = {0};
    param4.memoryBufferControlIndex = ADC12_B_MEMORY_4;
    param4.inputSourceSelect = ADC12_B_INPUT_A4;
    param4.refVoltageSourceSelect = ADC12_B_VREFPOS_AVCC_VREFNEG_VSS;
    param4.endOfSequence = ADC12_B_NOTENDOFSEQUENCE;
    ADC12_B_configureMemory(ADC12_B_BASE ,&param4);

    /*
     * Base address of the ADC12B Module
     * Configure memory buffer 5
     * Map input A5 to memory buffer 5
     * Vr+ = AVcc
     * Vr- = AVss
     * Memory buffer 5 is not the end of a sequence
     */
    ADC12_B_configureMemoryParam param5 = {0};
    param5.memoryBufferControlIndex = ADC12_B_MEMORY_5;
    param5.inputSourceSelect = ADC12_B_INPUT_A5;
    param5.refVoltageSourceSelect = ADC12_B_VREFPOS_AVCC_VREFNEG_VSS;
    param5.endOfSequence = ADC12_B_NOTENDOFSEQUENCE;
    ADC12_B_configureMemory(ADC12_B_BASE ,&param5);

    /*
     * Base address of the ADC12B Module
     * Configure memory buffer 6
     * Map input A6 to memory buffer 6
     * Vr+ = AVcc
     * Vr- = AVss
     * Memory buffer 6 is not the end of a sequence
     */
    ADC12_B_configureMemoryParam param6 = {0};
    param6.memoryBufferControlIndex = ADC12_B_MEMORY_6;
    param6.inputSourceSelect = ADC12_B_INPUT_A6;
    param6.refVoltageSourceSelect = ADC12_B_VREFPOS_AVCC_VREFNEG_VSS;
    param6.endOfSequence = ADC12_B_NOTENDOFSEQUENCE;
    ADC12_B_configureMemory(ADC12_B_BASE ,&param6);

    /*
     * Base address of the ADC12B Module
     * Configure memory buffer 7
     * Map input A7 to memory buffer 7
     * Vr+ = AVcc
     * Vr- = AVss
     * Memory buffer 7 is not the end of a sequence
     */
    ADC12_B_configureMemoryParam param7 = {0};
    param7.memoryBufferControlIndex = ADC12_B_MEMORY_7;
    param7.inputSourceSelect = ADC12_B_INPUT_A7;
    param7.refVoltageSourceSelect = ADC12_B_VREFPOS_AVCC_VREFNEG_VSS;
    param7.endOfSequence = ADC12_B_NOTENDOFSEQUENCE;
    ADC12_B_configureMemory(ADC12_B_BASE ,&param7);

    /*
     * Base address of the ADC12B Module
     * Configure memory buffer 8
     * Map input A8 to memory buffer 8
     * Vr+ = AVcc
     * Vr- = AVss
     * Memory buffer 8 is not the end of a sequence
     */
    ADC12_B_configureMemoryParam param8 = {0};
    param8.memoryBufferControlIndex = ADC12_B_MEMORY_8;
    param8.inputSourceSelect = ADC12_B_INPUT_A8;
    param8.refVoltageSourceSelect = ADC12_B_VREFPOS_AVCC_VREFNEG_VSS;
    param8.endOfSequence = ADC12_B_NOTENDOFSEQUENCE;
    ADC12_B_configureMemory(ADC12_B_BASE ,&param8);

    /*
     * Base address of the ADC12B Module
     * Configure memory buffer 9
     * Map input A9 to memory buffer 9
     * Vr+ = AVcc
     * Vr- = AVss
     * Memory buffer 9 is not the end of a sequence
     */
    ADC12_B_configureMemoryParam param9 = {0};
    param9.memoryBufferControlIndex = ADC12_B_MEMORY_9;
    param9.inputSourceSelect = ADC12_B_INPUT_A9;
    param9.refVoltageSourceSelect = ADC12_B_VREFPOS_AVCC_VREFNEG_VSS;
    param9.endOfSequence = ADC12_B_NOTENDOFSEQUENCE;
    ADC12_B_configureMemory(ADC12_B_BASE ,&param9);

    /*
     * Base address of the ADC12B Module
     * Configure memory buffer 10
     * Map input A10 to memory buffer 10
     * Vr+ = AVcc
     * Vr- = AVss
     * Memory buffer 10 is not the end of a sequence
     */
    ADC12_B_configureMemoryParam param10 = {0};
    param10.memoryBufferControlIndex = ADC12_B_MEMORY_10;
    param10.inputSourceSelect = ADC12_B_INPUT_A10;
    param10.refVoltageSourceSelect = ADC12_B_VREFPOS_AVCC_VREFNEG_VSS;
    param10.endOfSequence = ADC12_B_NOTENDOFSEQUENCE;
    ADC12_B_configureMemory(ADC12_B_BASE ,&param10);

    /*
     * Base address of the ADC12B Module
     * Configure memory buffer 11
     * Map input A11 to memory buffer 11
     * Vr+ = AVcc
     * Vr- = AVss
     * Memory buffer 11 is not the end of a sequence
     */
    ADC12_B_configureMemoryParam param11 = {0};
    param11.memoryBufferControlIndex = ADC12_B_MEMORY_11;
    param11.inputSourceSelect = ADC12_B_INPUT_A11;
    param11.refVoltageSourceSelect = ADC12_B_VREFPOS_AVCC_VREFNEG_VSS;
    param11.endOfSequence = ADC12_B_NOTENDOFSEQUENCE;
    ADC12_B_configureMemory(ADC12_B_BASE ,&param11);

    /*
     * Base address of the ADC12B Module
     * Configure memory buffer 12
     * Map input A12 to memory buffer 12
     * Vr+ = AVcc
     * Vr- = AVss
     * Memory buffer 12 is not the end of a sequence
     */
    ADC12_B_configureMemoryParam param12 = {0};
    param12.memoryBufferControlIndex = ADC12_B_MEMORY_12;
    param12.inputSourceSelect = ADC12_B_INPUT_A12;
    param12.refVoltageSourceSelect = ADC12_B_VREFPOS_AVCC_VREFNEG_VSS;
    param12.endOfSequence = ADC12_B_NOTENDOFSEQUENCE;
    ADC12_B_configureMemory(ADC12_B_BASE ,&param12);

    /*
     * Base address of the ADC12B Module
     * Configure memory buffer 13
     * Map input A13 to memory buffer 13
     * Vr+ = AVcc
     * Vr- = AVss
     * Memory buffer 13 is not the end of a sequence
     */
    ADC12_B_configureMemoryParam param13 = {0};
    param13.memoryBufferControlIndex = ADC12_B_MEMORY_13;
    param13.inputSourceSelect = ADC12_B_INPUT_A13;
    param13.refVoltageSourceSelect = ADC12_B_VREFPOS_AVCC_VREFNEG_VSS;
    param13.endOfSequence = ADC12_B_NOTENDOFSEQUENCE;
    ADC12_B_configureMemory(ADC12_B_BASE ,&param13);

    /*
     * Base address of the ADC12B Module
     * Configure memory buffer 14
     * Map input A14 to memory buffer 14
     * Vr+ = AVcc
     * Vr- = AVss
     * Memory buffer 14 is not the end of a sequence
     */
    ADC12_B_configureMemoryParam param14 = {0};
    param14.memoryBufferControlIndex = ADC12_B_MEMORY_14;
    param14.inputSourceSelect = ADC12_B_INPUT_A14;
    param14.refVoltageSourceSelect = ADC12_B_VREFPOS_AVCC_VREFNEG_VSS;
    param14.endOfSequence = ADC12_B_NOTENDOFSEQUENCE;
    ADC12_B_configureMemory(ADC12_B_BASE ,&param14);

    /*
     * Base address of the ADC12B Module
     * Configure memory buffer 15
     * Map input A15 to memory buffer 15
     * Vr+ = AVcc
     * Vr- = AVss
     * Memory buffer 15 IS the end of a sequence
     */
    ADC12_B_configureMemoryParam param15 = {0};
    param15.memoryBufferControlIndex = ADC12_B_MEMORY_15;
    param15.inputSourceSelect = ADC12_B_INPUT_A15;
    param15.refVoltageSourceSelect = ADC12_B_VREFPOS_AVCC_VREFNEG_VSS;
    param15.endOfSequence = ADC12_B_ENDOFSEQUENCE;
    ADC12_B_configureMemory(ADC12_B_BASE ,&param15);
}
