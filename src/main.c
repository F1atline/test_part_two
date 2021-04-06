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

#include "stm32f3xx_ll_usart.h"
#include "stm32f3xx_ll_utils.h"

extern PORT *PORTE;
uint16_t BUFF[32];
bool data_ready = false;

struct packet 
{
  uint8_t pre;
  uint8_t cnt;
  uint8_t type;
  uint8_t lenth;
  uint8_t data[256];
} packet_t;

struct state
{
  bool ready;
  bool transmited;
  uint8_t header_cnt;
} state_t;

packet_t message = {0};
state_t state = {0};

//Poly  : 0x31    x^8 + x^5 + x^4 + 1
uint8_t Crc8Table[256] = {
    0x00, 0x31, 0x62, 0x53, 0xC4, 0xF5, 0xA6, 0x97,
    0xB9, 0x88, 0xDB, 0xEA, 0x7D, 0x4C, 0x1F, 0x2E,
    0x43, 0x72, 0x21, 0x10, 0x87, 0xB6, 0xE5, 0xD4,
    0xFA, 0xCB, 0x98, 0xA9, 0x3E, 0x0F, 0x5C, 0x6D,
    0x86, 0xB7, 0xE4, 0xD5, 0x42, 0x73, 0x20, 0x11,
    0x3F, 0x0E, 0x5D, 0x6C, 0xFB, 0xCA, 0x99, 0xA8,
    0xC5, 0xF4, 0xA7, 0x96, 0x01, 0x30, 0x63, 0x52,
    0x7C, 0x4D, 0x1E, 0x2F, 0xB8, 0x89, 0xDA, 0xEB,
    0x3D, 0x0C, 0x5F, 0x6E, 0xF9, 0xC8, 0x9B, 0xAA,
    0x84, 0xB5, 0xE6, 0xD7, 0x40, 0x71, 0x22, 0x13,
    0x7E, 0x4F, 0x1C, 0x2D, 0xBA, 0x8B, 0xD8, 0xE9,
    0xC7, 0xF6, 0xA5, 0x94, 0x03, 0x32, 0x61, 0x50,
    0xBB, 0x8A, 0xD9, 0xE8, 0x7F, 0x4E, 0x1D, 0x2C,
    0x02, 0x33, 0x60, 0x51, 0xC6, 0xF7, 0xA4, 0x95,
    0xF8, 0xC9, 0x9A, 0xAB, 0x3C, 0x0D, 0x5E, 0x6F,
    0x41, 0x70, 0x23, 0x12, 0x85, 0xB4, 0xE7, 0xD6,
    0x7A, 0x4B, 0x18, 0x29, 0xBE, 0x8F, 0xDC, 0xED,
    0xC3, 0xF2, 0xA1, 0x90, 0x07, 0x36, 0x65, 0x54,
    0x39, 0x08, 0x5B, 0x6A, 0xFD, 0xCC, 0x9F, 0xAE,
    0x80, 0xB1, 0xE2, 0xD3, 0x44, 0x75, 0x26, 0x17,
    0xFC, 0xCD, 0x9E, 0xAF, 0x38, 0x09, 0x5A, 0x6B,
    0x45, 0x74, 0x27, 0x16, 0x81, 0xB0, 0xE3, 0xD2,
    0xBF, 0x8E, 0xDD, 0xEC, 0x7B, 0x4A, 0x19, 0x28,
    0x06, 0x37, 0x64, 0x55, 0xC2, 0xF3, 0xA0, 0x91,
    0x47, 0x76, 0x25, 0x14, 0x83, 0xB2, 0xE1, 0xD0,
    0xFE, 0xCF, 0x9C, 0xAD, 0x3A, 0x0B, 0x58, 0x69,
    0x04, 0x35, 0x66, 0x57, 0xC0, 0xF1, 0xA2, 0x93,
    0xBD, 0x8C, 0xDF, 0xEE, 0x79, 0x48, 0x1B, 0x2A,
    0xC1, 0xF0, 0xA3, 0x92, 0x05, 0x34, 0x67, 0x56,
    0x78, 0x49, 0x1A, 0x2B, 0xBC, 0x8D, 0xDE, 0xEF,
    0x82, 0xB3, 0xE0, 0xD1, 0x46, 0x77, 0x24, 0x15,
    0x3B, 0x0A, 0x59, 0x68, 0xFF, 0xCE, 0x9D, 0xAC
};

uint8_t CRC8(uint8_t *pcBlock, uint16_t len)
{
    uint8_t crc = 0xFF;

    while (len--)
        crc = Crc8Table[crc ^ *pcBlock++];

    return crc;
}

void UART_init()
{
  //enable usart clock
  RCC->APB2ENR |= RCC_APB2ENR_USART1EN;
  //enable irq dma1
  NVIC_EnableIRQ(DMA1_Channel4_IRQn); //tx
  NVIC_EnableIRQ(DMA1_Channel5_IRQn); //rx

  //DMA for TX
  LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_CHANNEL_4, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);
  LL_DMA_SetChannelPriorityLevel(DMA1, LL_DMA_CHANNEL_4, LL_DMA_PRIORITY_LOW);
  LL_DMA_SetMode(DMA1, LL_DMA_CHANNEL_4, LL_DMA_MODE_NORMAL);
  LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_CHANNEL_4, LL_DMA_PERIPH_NOINCREMENT);
  LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_CHANNEL_4, LL_DMA_MEMORY_INCREMENT);
  LL_DMA_SetPeriphSize(DMA1, LL_DMA_CHANNEL_4, LL_DMA_PDATAALIGN_BYTE);
  LL_DMA_SetMemorySize(DMA1, LL_DMA_CHANNEL_4, LL_DMA_MDATAALIGN_BYTE);

  LL_DMA_SetPeriphAddress(DMA1, LL_DMA_CHANNEL_4, (uint32_t)&USART1->DR);

  //DMA for RX
  LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_CHANNEL_5, LL_DMA_DIRECTION_PERIPH_TO_MEMORY);
  LL_DMA_SetChannelPriorityLevel(DMA1, LL_DMA_CHANNEL_5, LL_DMA_PRIORITY_LOW);
  LL_DMA_SetMode(DMA1, LL_DMA_CHANNEL_5, LL_DMA_MODE_NORMAL);
  LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_CHANNEL_5, LL_DMA_PERIPH_NOINCREMENT);
  LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_CHANNEL_5, LL_DMA_MEMORY_INCREMENT);
  LL_DMA_SetPeriphSize(DMA1, LL_DMA_CHANNEL_5, LL_DMA_PDATAALIGN_BYTE);
  LL_DMA_SetMemorySize(DMA1, LL_DMA_CHANNEL_5, LL_DMA_MDATAALIGN_BYTE);

  //init USART with requred settings
  USART_InitStruct.BaudRate = 460800;
  USART_InitStruct.DataWidth = LL_USART_DATAWIDTH_8B;
  USART_InitStruct.StopBits = LL_USART_STOPBITS_1;
  USART_InitStruct.Parity = LL_USART_PARITY_NONE;
  USART_InitStruct.TransferDirection = LL_USART_DIRECTION_TX_RX;
  USART_InitStruct.HardwareFlowControl = LL_USART_HWCONTROL_NONE;
  USART_InitStruct.OverSampling = LL_USART_OVERSAMPLING_16;
  LL_USART_Init(USART1, &USART_InitStruct);
  LL_USART_DisableIT_CTS(USART1);
  LL_USART_ConfigAsyncMode(USART1);
  LL_USART_Enable(USART1);

  NVIC_EnableIRQ(USART1_IRQn);
}


uart_process_packet(packet_t* packet)
{
  if(packet->lenth > 0)
  {
    if(CRC8(&packet->pre, (packet->lenth + 1) + 4) == 0) //if CRC calc with CRC from packet this mean that message correct
    {
      //set bit
      packet->type |= 0x80;
      //calc new CRC
      packet->data[lenth+1] = CRC8(&packet->pre, (packet->lenth + 4));
      //start dma for TX
      LL_DMA_SetMemoryAddress(DMA1, LL_DMA_CHANNEL_4, (uint32_t)&packet->pre);
      LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_4, (packet->lenth + 1) + 4));
      LL_USART_EnableDMAReq_TX(USART1);
    }
    else
    {
      //abort message and send NACK;
    }
  }
}

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

  UART_init();
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
    if(state->ready)
    {
      //set transmit false
      state->transmited = false;
      state->data_ready = false;
      //parse message
      uart_process_packet(&message);
    }
  }
}

void DMA1_CH1_IRQHandler(void)
{
  if(READ_BIT(DMA1->ISR, DMA_ISR_TCIF1))
  {
    state->data_ready = true;
  }
}

void DMA1_CH4_IRQHandler(void)
{
  //transmit cmplt msg
  if(READ_BIT(DMA1->ISR, DMA_ISR_TCIF4))
  {
    printf("MSG %d transmitted\n", message->cnt);
    //clear
    memset(&message, 0, sizeof(packet_t));
    state->transmited = true;
  }
}

void DMA1_CH5_IRQHandler(void)
{
  //transmit cmplt msg
  if(READ_BIT(DMA1->ISR, DMA_ISR_TCIF5))
  {
    printf("MSG %d recived\n", message->cnt);
    //mark for handle message;
    state->ready = true;
  }
}

void USART1_IRQHandler(void)
{
  if(READ_BIT(USART1->ISR, USART_ISR_RXNE))
  {
    switch(state->header_cnt)
    {
      case 0:
        message->pre = USART1->DR;
        state->header_cnt++;
        break;
      case 1:
        message->cnt = USART1->DR;
        state->header_cnt++;
        break;
      case 2:
        message->type = USART1->DR;
        state->header_cnt++;
        break;
      case 3:
        message->lenth = USART1->DR;
        state->header_cnt++;
        break;
      case 4:
        LL_DMA_SetMemoryAddress(DMA1, LL_DMA_CHANNEL_5, (uint32_t)&packet->pre);
        LL_DMA_SetDataLength(DMA1, LL_DMA_CHANNEL_5, (packet->lenth + 1));
        LL_USART_EnableDMAReq_RX(USART1);
        USART1->CR1 &= ~USART_CR_RXNEIE;
        state->header_cnt = 0;
        break;
    }
    
  }

  if(READ_BIT(USART1->ISR, USART_ISR_IDLE))
  {
    //abort recive
    //enable
    USART1->CR1 |= USART_CR_RXNEIE;
    state->header_cnt=0;
  }
}
/*************************** End of file ****************************/