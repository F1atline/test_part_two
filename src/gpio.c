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
volatile PORT *PORTE = (volatile PORT *) GPIOC;

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

  //reset out
  PORTE->BS12 = 1;
  //medium speed
  PORTE->OSPEED12 = 1;
  //no pull down
  PORTE->PUPD12 = 0;
  //out mode
  PORTE->MOD12 = 1;

  //reset out
  PORTE->BS11 = 1;
  //medium speed
  PORTE->OSPEED11 = 1;
  //no pull down
  PORTE->PUPD11 = 0;
  //out mode
  PORTE->MOD11 = 1;

  //enable clock for PORTC
  SET_BIT(RCC->AHBENR, RCC_AHBENR_GPIOCEN);

  //TX
  //fast speed
  PORTC->OSPEED4 = 3; 
  //no pull down
  PORTC->PUPD4 = 0;
  //AF
  PORTC->MOD4 = 2;
  //
  PORTC->AFR4 = 7;

 
  //RX
  //fast speed
  PORTC->OSPEED5 = 3; 
  //no pull down
  PORTC->PUPD5 = 0;
  //AF
  PORTC->MOD5 = 2;
  //
  PORTC->AFR5 = 7;
}
