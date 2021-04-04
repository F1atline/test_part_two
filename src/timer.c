/**
 * @file timer.c
 * @author HES
 * @brief stm32 timers functions
 * @version 0.1
 * @date 2021-04-04
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "timer.h"

#include "gpio.h"

extern PORT *PORTE;

void timer_init()
{
    //enable RCC for TIM2
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
  if(READ_BIT(RCC->APB2ENR, RCC_APB2ENR_TIM1EN) == RCC_APB2ENR_TIM1EN)
  {
    printf("TIM1 Enabled\n");
  }
  //set prescaller
  TIM1->PSC = 48-1;
  //ser reload value
  TIM1->ARR  = 10000 - 1;
  //TRGO for ADC
  TIM1->CR2 |= TIM_CR2_MMS_1;
  //enable update interrupt
  TIM1->DIER |= TIM_DIER_UDE;
  //start timer
  TIM1->CR1 |= TIM_CR1_CEN;

  //enable RCC for TIM2
  RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
  if(READ_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM2EN) == RCC_APB1ENR_TIM2EN)
  {
    printf("TIM2 Enabled\n");
  }
  //set prescaller
  TIM2->PSC = 48-1;
  //ser reload value
  TIM2->ARR  = 3333/2 - 1;
  //enable update interrupt
  TIM2->DIER |= TIM_DIER_UIE;
  //start timer
  TIM2->CR1 |= TIM_CR1_CEN;
  //enable timer interrupt
  NVIC_EnableIRQ(TIM2_IRQn);

  //enable RCC for TIM3
  RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
  if(READ_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM3EN) == RCC_APB1ENR_TIM3EN)
  {
    printf("TIM3 Enabled\n");
  }
  //set prescaller
  TIM3->PSC = 48-1;
  //ser reload value
  TIM3->ARR  = 1000/2-1;
  //enable update interrupt
  TIM3->DIER |= TIM_DIER_UIE;
  //start timer
  TIM3->CR1 |= TIM_CR1_CEN;
  //enable timer interrupt
  NVIC_EnableIRQ(TIM3_IRQn);

    //enable RCC for TIM4
  RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;
  if(READ_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM4EN) == RCC_APB1ENR_TIM4EN)
  {
    printf("TIM4 Enabled\n");
  }
  //set prescaller
  TIM4->PSC = 48-1;
  //ser reload value
  TIM4->ARR  = 100/2-1;
  //enable update interrupt
  TIM4->DIER |= TIM_DIER_UIE;
  //start timer
  TIM4->CR1 |= TIM_CR1_CEN;
  //enable timer interrupt
  NVIC_EnableIRQ(TIM4_IRQn);

}

void TIM2_IRQHandler(void)
{
  //update interrupt flag
  if(READ_BIT(TIM2->SR, TIM_SR_UIF))
  {
    //clear flag
    CLEAR_BIT(TIM2->SR, TIM_SR_UIF);
    //toggle pin
    PORTE->B13 = ~PORTE->B13;
  }
}

void TIM3_IRQHandler(void)
{
  //update interrupt flag
  if(READ_BIT(TIM3->SR, TIM_SR_UIF))
  {
    //clear flag
    CLEAR_BIT(TIM3->SR, TIM_SR_UIF);
    //toggle pin
    PORTE->B12 = ~PORTE->B12;
  }
}

void TIM4_IRQHandler(void)
{
  //update interrupt flag
  if(READ_BIT(TIM4->SR, TIM_SR_UIF))
  {
    //clear flag
    CLEAR_BIT(TIM4->SR, TIM_SR_UIF);
    //toggle pin
    PORTE->B11 = ~PORTE->B11;
  }
}