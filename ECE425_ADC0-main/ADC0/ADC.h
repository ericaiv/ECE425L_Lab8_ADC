/**
 * @file ADC.h
 *
 * @brief Header file for the ADC driver.
 *
 * @author Aaron Nanas
 *
 */


#include "TM4C123GH6PM.h"
#include "SysTick_Delay.h"

/**
 * @brief
 *
 * @param
 *
 * @return
 */
void ADC_Init(void);

/**
 * @brief
 *
 * @param
 *
 * @return
*/
void ADC_Sample(double analog_value_buffer[]);
