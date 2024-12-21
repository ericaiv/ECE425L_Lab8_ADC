/**
 * @file ADC.c
 *
 * @brief Source code for the ADC driver.
 *
 * @author Aaron Nanas
 *
 */

#include "ADC.h"

void ADC_Init(void)
{
	// Enable the clock to ADC Module 0 by setting the
	// R0 bit (Bit 0) in the RCGCADC register and provide
	// a short delay of 1 ms after enabling the clock
	SYSCTL->RCGCADC |= 0x01;
	SysTick_Delay1ms(1);
	
	// Enable the clock to Port E by setting the
	// R4 bit (Bit 4) in the RCGCGPIO register
	SYSCTL->RCGCGPIO |= 0x10;
	
	// Configure the PE2 pin as an input by clearing Bit 2 in the DIR register
	GPIOE->DIR &= ~0x04;
	
	// Disable the digital functionality for the PE2 pin
	// by clearing Bit 2 in the DEN register
	GPIOE->DEN &= ~0x04;
	
	// Enable the analog functionality for the PE2 pin
	// by setting Bit 2 in the AMSEL register
	GPIOE->AMSEL |= 0x04;
	
	// Select the alternate function for the PE2 pin (AIN1)
	// by setting Bit 2 in the AFSEL register
	// Note: Refer to Table 23-5 from the TM4C123GH6PM Microcontroller Datasheet
	// for a list of GPIO Pins and Alternate Functions
	GPIOE->AFSEL |= 0x04;
	
	// Disable the Sample Sequencer 0 by clearing the ASEN0 bit (Bit 0) in the ACTSS register
	ADC0->ACTSS &= ~0x1;
		
	// Select the event (trigger) that initiates sampling for the
	// Sample Sequence 0 by clearing all of the bits in the EM0 field (Bits 3 to 0).
	// This enables Sample Sequencer 0 to sample whenever the SSn bit in the PSSI register is set
	ADC0->EMUX &= ~0x000F;
	
	// Clear all of the bits (Bits 31 to 0) in the SSMUX0 register before configuration
	ADC0->SSMUX0 &= ~0xFFFFFFFF;
	
	// Choose Channel 1 (PE2) for the first sample by writing a value of 0x01
	// in the MUX0 field (Bits 3 to 0) in the SSMUX0 register
	ADC0->SSMUX0 |= 0x00000001;
	
	// Assign Channel 1 to be the end of the sampling sequence
	// by setting the END0 bit (Bit 1) in the SSCTL0 register
	ADC0->SSCTL0 |= 0x00000002;
	
	// Enable the raw interrupt signal (INR0) for the first sample by setting the IE0 bit (Bit 2)
	// in the SSCTL0 register. The INR0 interrupt signal will be triggered
	// at the end of the first sample's conversion
	ADC0->SSCTL0 |= 0x00000004;
	
	// Enable Sample Sequencer 0 by setting the ASEN0 bit (Bit 0) in the ACTSS register
	ADC0->ACTSS |= 0x1;
}

void ADC_Sample(double analog_value_buffer[])
{
	// Begin sampling on the Sample Sequencer 0 module by setting the
	// SS0 bit (Bit 0) in the PSSI register. Note that the SS0 bit 
	// is a write-only bit, so it will be automatically cleared after setting it
	ADC0->PSSI |= 0x01;
	
	// Wait until an interrupt has occurred by checking the
	// SS0 raw interrupt status bit (INR0, Bit 0) in the RIS register
	// If INR0 is set to 1, then that indicates the end of the sampling sequence
	while ((ADC0->RIS & 0x01) == 0);
	
	// Read the conversion result for the sample collected by the Sample Sequencer 0 module
	// by reading the SSFIFO0 (Sample Sequence Result FIFO 0) register
	uint32_t conversion_result = ADC0->SSFIFO0;
	
	// Clear the interrupt signal by setting the IN0 bit (Bit 0) in the ISC register.
	ADC0->ISC |= 0x01;
	
	// Calculate the measured analog voltage and store the results in a double array
	analog_value_buffer[0] = (conversion_result * 3.3) / 4096;
}
