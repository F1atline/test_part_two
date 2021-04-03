/**
 * @file gpio.c
 * @author HES
 * @brief easy GPIO driver
 * @version 0.1
 * @date 2021-04-03
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "gpio.h"

volatile PORT *PORTE = (volatile PORT *) GPIOE;

void gpio_init()
{
  //enable RCC for PORTE
  SET_BIT(RCC->AHBENR, RCC_AHBENR_GPIOEEN);
  //reset out
  PORTE->BS13 = 1;
  //medium speed
  PORTE->OSPEED13 = 1;
  //no pull down
  PORTE->PUPD13 = 0;
  //out mode
  PORTE->MOD13 = 1;
}
