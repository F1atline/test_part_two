/**
 * @file adc.c
 * @author HES
 * @brief ADC
 * @version 0.1
 * @date 2021-04-04
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "adc.h"

void ADC_init()
{
  //disable ADC1
  ADC1->CR &= ~ADC_CR_ADEN;
  //enable RCC for ADC1
  RCC->AHBENR |= RCC_AHBENR_ADC12EN;
  //set div to 4
  RCC->CFGR |= RCC_CFGR2_ADCPRE12_DIV4;
  //set TIM1 satrt convertion
  ADC1->CR |= ADC_CFGR_EXTSEL_3 | ADC_CFGR_EXTSEL_0;

}