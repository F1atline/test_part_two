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

#include "stm32f3xx.h"

extern uint16_t BUFF[32];

void ADC_init()
{

//********************DMA***************************
  //enable clock
  RCC->AHBENR |= RCC_AHBENR_DMA1EN;
  //reg addr
  DMA1_Channel1->CPAR = (uint32_t)&ADC1->DR;
  //memory addr
  DMA1_Channel1->CMAR = (uint32_t)&BUFF[0];
  //size
  DMA1_Channel1->CNDTR = 32;
  //memory increment
  DMA1_Channel1->CCR |= DMA_CCR_MINC;
  //half word
  DMA1_Channel1->CCR |= DMA_CCR_MSIZE_0;
  //half word
  DMA1_Channel1->CCR |= DMA_CCR_PSIZE_0;
  //circular mode
  DMA1_Channel1->CCR |= DMA_CCR_CIRC;
  //perif to memory
  DMA1_Channel1->CCR &=~ DMA_CCR_DIR;
  //enable interruprt for complite
  DMA1_Channel1->CCR |= DMA_CCR_TCIE;
  //enable DMA
  DMA1_Channel1->CCR |= DMA_CCR_EN;

  //disable ADC1
  ADC1->CR &= ~ADC_CR_ADEN;
  //enable RCC for ADC1
  RCC->AHBENR |= RCC_AHBENR_ADC12EN;
  //set div to 4
  RCC->CFGR |= RCC_CFGR2_ADCPRE12_DIV4;
  //enable voltage reg
  ADC1->CR |= ADC_CR_ADVREGEN;
  //enable event triggering
  ADC1->CFGR |= ADC_CFGR_EXTEN_0;
  //set TIM15 satrt convertion
  ADC1->CFGR |= ADC_CFGR_EXTSEL_3 | ADC_CFGR_EXTSEL_2 | ADC_CFGR_EXTSEL_1;
  //select internal temperature ADC1 channel (16)
  ADC1->SQR1 |= ADC_SQR1_SQ1_4;
  //enable temperature sensor
  ADC12_COMMON->CCR |= ADC12_CCR_TSEN;
  //enable DMA
  ADC1->CFGR |= ADC_CFGR_DMAEN;
  //enable ADC
  ADC1->CR |= ADC_CR_ADEN;
}

