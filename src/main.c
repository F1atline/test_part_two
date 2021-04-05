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

#include "timer.h"

#include "adc.h"

extern PORT *PORTE;
uint16_t BUFF[32];
bool data_ready = false;

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
}

/*********************************************************************
*
*       main()
*
*  Function description
*   Application entry point.
*/
 int main(void) {
  float temperature = 0;
  printf("reset\n");
  //call RCC configuration
  clock_config();
  printf("complite RCC initialization\n");
  gpio_init();
  printf("complite GPIO initialization\n");
  timer_init();
  printf("complite timers initialization\n");
  ADC_init();
  printf("complite ADC initialization\n");
  NVIC_EnableIRQ(DMA1_Channel1_IRQn);
  for(;;) {
    if(data_ready)
    {
      uint32_t tmp_val;
      for(uint32_t i = 0; i<32; i++)
      {
        tmp_val += (uint32_t)BUFF[i];
      }
      temperature = ((V25 - (tmp_val/32)) / AVG_SLOPE) + 25;
      printf("%f\n", temperature);
      data_ready = false;
    }
  }
}

/*************************** End of file ****************************/
void DMA1_CH1_IRQHandler(void)
{
  if(READ_BIT(DMA1->ISR, DMA_ISR_TCIF1))
  {
    data_ready = true;
  }
}