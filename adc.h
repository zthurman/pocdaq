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

#ifndef AI_SCANNER_ADC_H_
#define AI_SCANNER_ADC_H_

extern volatile unsigned char conf;

void Init_GPIO_For_ADC12_B_All_AI(void);
void Init_Enable_ADC12_B(void);
void Config_Mem_Buffers(void);

#endif /* AI_SCANNER_ADC_H_ */
