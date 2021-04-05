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
        RCC->AHBENR |= RCC_AHBENR_DMA1EN;

        DMA1_Channel1->CPAR = (uint32_t)&ADC1->DR;     //
        DMA1_Channel1->CMAR = (uint32_t)&BUFF[0];        // Массив
        DMA1_Channel1->CNDTR = 32;                        // Длина буфера
        DMA1_Channel1->CCR |= DMA_CCR_MINC;            // Инкремент адреса
        DMA1_Channel1->CCR |= DMA_CCR_PSIZE_0;        // 16 бит
        DMA1_Channel1->CCR |= DMA_CCR_PSIZE_0;        // 16 бит
        DMA1_Channel1->CCR |= DMA_CCR_CIRC;            // Непрерывный режим
        DMA1_Channel1->CCR &=~ DMA_CCR_DIR;            // 01: peripheral-to-Memory

        DMA1_Channel1->CCR |= DMA_CCR_TCIE;            // Transfer complete interrupt enable
        DMA1_Channel1->CCR |= DMA_CCR_EN;                 // Вкл. передачу




  //disable ADC1
  ADC1->CR &= ~ADC_CR_ADEN;
  //enable RCC for ADC1
  RCC->AHBENR |= RCC_AHBENR_ADC12EN;
  //set div to 4
  RCC->CFGR |= RCC_CFGR2_ADCPRE12_DIV4;
  //set TIM1 satrt convertion
  ADC1->CR |= ADC_CFGR_EXTSEL_3 | ADC_CFGR_EXTSEL_0;
  //select internal temperature ADC1 channel (16)
  ADC1->SQR1 |= ADC_SQR1_SQ1_4;
  //enable DMA
  ADC1->CFGR |= ADC_CFGR_DMAEN;
  ADC1->CR |= ADC12_CCR_TSEN;
  ADC1->CFGR |= ADC_CFGR_CONT;
  ADC1->CFGR |= ADC_CFGR_EXTEN_0;
  ADC1->IER |= ADC_IER_EOSIE;
  ADC1->CR |= ADC_CR_ADEN;
}

