#include "adc.h"
#include <stdio.h>
#include <stdint.h>
#include <math.h>

char read_temp = 0;

uint16_t temp;
uint16_t pot;
#define TEMP_CHANNEL 18
#define POT_CHANNEL 8
char tempWord[] = "Temperature: ";
char potWord[] = "Potentiometer: ";
char lumWord[] = "Luminosity: ";
char degrees[] = "°C";
char volts[] = " Volts";
char percent[] = " %";
char newLine[] = "\r\n";
char tempStr[6];
char potStr[6];
char lumStr[6];

int cnt=0;
// Initialize temperature sensor, sampling precision, external trigger, and resolution
void initADC() {
    adc->SQR3 = POT_CHANNEL;            // Channel 8 (PB0)
    adc->SMPR2 |= (7 << 24);            // 480 cycles to sample for channel 8
    adc->SMPR1 |= (7 << 24);            // 480 cycles to sample for channel 18

    adcCom->CCR |= ADC_CCR_TSVREFE;     // Enable TSVREFE
    adc->CR1 |= ADC_CR1_RES_0;          // Configure ADC resolution to 10 bits (1024 possible values)
    adc->CR1 |= ADC_CR1_EOCIE;          // Enable EOC (End Of Conversion) interrupt
    adc->CR2 |= ADC_CR2_ADON;           // Enable ADC

    // Configure TIM2 as the trigger source for ADC
    adc->CR2 &= ~ADC_CR2_EXTEN;         // Clear EXTEN
    adc->CR2 |= ADC_CR2_EXTEN_0;        // Trigger on rising edge
    adc->CR2 &= ~ADC_CR2_EXTSEL;        // Clear EXTSEL
    adc->CR2 |= 3 << 25;                // Set TIM2_TRGO as the trigger source
}

// Calculate luminosity L* from luminance Y
float calculate_L(float Y, float Yn) {
    float ratio = Y / Yn;
    float threshold = pow(6.0 / 29.0, 3);

    if (ratio > threshold) {
        return 116 * pow(ratio, 1.0 / 3.0) - 16;
    } else {
        float factor = pow(29.0 / 6.0, 2);
        return 116 * (1.0 / 3.0) * factor * ratio;
    }
}

// Convert ADC value to temperature
float DR_To_Temp(uint16_t adc_value) {
    return ((adc_value / 1024.0) * 3.3 - 0.76) / 0.0025 + 25;
}

// Convert ADC value to voltage
float DR_To_Volts(uint16_t adc_value) {
    return (adc_value / 1024.0) * 3.3;
}

// Read ADC value
uint16_t ADC_read() {
    while (!(adc->SR & ADC_SR_EOC));
    return (uint16_t)adc->DR;
}

// Show temperature data via USART
void showTemp(uint16_t adc_value) {
    float temperature = DR_To_Temp(adc_value);
    sprintf(tempStr, "%.1f", temperature);
    USART2_Transmit(tempWord, sizeof(tempWord));
    USART2_Transmit(tempStr, sizeof(tempStr));
    USART2_Transmit(degrees, sizeof(degrees));
    USART2_Transmit(newLine, sizeof(newLine));
}

// Show voltage data via USART
void showVoltage(uint16_t adc_value) {
    float voltage = DR_To_Volts(adc_value);
    sprintf(potStr, "%.2f", voltage);
    USART2_Transmit(potWord, sizeof(potWord));
    USART2_Transmit(potStr, sizeof(potStr));
    USART2_Transmit(volts, sizeof(volts));
    USART2_Transmit(newLine, sizeof(newLine));
}

// Show luminance data via USART
void showLuminance(float luminance) {
    sprintf(lumStr, "%.0f", luminance);
    USART2_Transmit(lumWord, sizeof(lumWord));
    USART2_Transmit(lumStr, sizeof(lumStr));
    USART2_Transmit(percent, sizeof(percent));
    USART2_Transmit(newLine, sizeof(newLine));
}

void ADC_IRQHandler(void) {
    if (ADC1->SR & ADC_SR_EOC) { // End of conversion flag
        switch (ADC1->SQR3) {
        case POT_CHANNEL: // Convert channel 8
            pot = ADC_read(); // Read ADC result for channel 8
            float potVoltage = DR_To_Volts(pot);
            float luminance = calculate_L(pot, 1024);

            // Calculate PWM duty cycle
            uint16_t newDutyCycle = (luminance / 100.0) * TIM2->ARR;
            TIM2_set_pwm(newDutyCycle); // Set PWM based on ADC value

            adc->SQR3 = TEMP_CHANNEL; // Set ADC to temperature channel
            break;

        case TEMP_CHANNEL: // Convert channel 18
            temp = ADC_read(); // Read ADC result for channel 18
            if(cnt>100){
				showVoltage(pot);
				showLuminance(luminance);
				showTemp(temp);
				cnt=0;
            }

            adc->SQR3 = POT_CHANNEL; // Set ADC to POT channel
            break;
        }
        cnt++;
    }

}
