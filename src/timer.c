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
  if(READ_BIT(RCC->APB1ENR, RCC_APB1ENR_TIM2EN) == RCC_APB1ENR_TIM2EN)
  {
    printf("TIM2 Enabled");
  }
  //set prescaller
  TIM2->PSC = 4800;
  //ser reload value
  TIM2->ARR  = 10000;
  //enable update interrupt
  TIM2->DIER |= TIM_DIER_UIE;
  //start timer
  TIM2->CR1 |= TIM_CR1_CEN;
  //enable timer interrupt
  NVIC_EnableIRQ(TIM2_IRQn);
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





