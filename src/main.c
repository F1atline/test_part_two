/*********************************************************************
*                    SEGGER Microcontroller GmbH                     *
*                        The Embedded Experts                        *
**********************************************************************

-------------------------- END-OF-HEADER -----------------------------

File    : main.c
Purpose : Generic application start

*/

#include <stdio.h>
#include <stdlib.h>

#include "stm32f3xx.h"
#include "main.h"

#include "gpio.h"

extern PORT *PORTE;

void clock_config()
{
  //set FLASH latency
  MODIFY_REG(FLASH->ACR, FLASH_ACR_LATENCY, FLASH_ACR_LATENCY_0);
  //wait set FLASH latency
  while((uint32_t)(READ_BIT(FLASH->ACR, FLASH_ACR_LATENCY)) != FLASH_ACR_LATENCY_0);
  //enable HSE external oscillator (HSE Bypass)
  SET_BIT(RCC->CR, RCC_CR_HSEBYP);
  //enable HSE crystal oscillator (HSE ON)
  SET_BIT(RCC->CR, RCC_CR_HSEON);
  //wait HSE
  while(READ_BIT(RCC->CR, RCC_CR_HSERDY) != RCC_CR_HSERDY); 
  //configure PLL
  //select source and multiplier
  MODIFY_REG(RCC->CFGR, RCC_CFGR_PLLSRC | RCC_CFGR_PLLMUL, ((RCC_CFGR_PLLSRC_HSE_PREDIV | RCC_CFGR2_PREDIV_DIV1) & RCC_CFGR_PLLSRC) | RCC_CFGR_PLLMUL6);
  MODIFY_REG(RCC->CFGR2, RCC_CFGR2_PREDIV, ((RCC_CFGR_PLLSRC_HSE_PREDIV | RCC_CFGR2_PREDIV_DIV1) & RCC_CFGR2_PREDIV));
  //enable PLL
  SET_BIT(RCC->CR, RCC_CR_PLLON);
  //white PLL
  while(READ_BIT(RCC->CR, RCC_CR_PLLRDY) != (RCC_CR_PLLRDY));
  //set AHB div
  MODIFY_REG(RCC->CFGR, RCC_CFGR_HPRE, RCC_CFGR_HPRE_DIV1);
  //set APB1 div
  MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE1, RCC_CFGR_PPRE1_DIV2);
  //set APB2 div
  MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE2, RCC_CFGR_PPRE1_DIV1);
  //set clock source
  MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SW_PLL);
  //white system clock
  while(((uint32_t)(READ_BIT(RCC->CFGR, RCC_CFGR_SWS))) != RCC_CFGR_SWS_PLL);
  printf("complite RCC initialization\n");
}

/*********************************************************************
*
*       main()
*
*  Function description
*   Application entry point.
*/
int main(void) {

  printf("reset\n");
  //call RCC configuration
  clock_config();

  gpio_init();

  for(;;) {
  PORTE->B13 = 1;
  for(int i = 0; i < 1000000; i++);
  PORTE->B13 = 0;
  for(int i = 0; i < 1000000; i++);
  }
}

/*************************** End of file ****************************/
