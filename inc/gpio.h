#ifndef _GPIO_H_
#define _GPIO_H_

#include <stdio.h>
#include <stdlib.h>
#include "stm32f3xx.h"

typedef struct {
  //MODER
  __IO uint32_t MOD0    : 2;
  __IO uint32_t MOD1    : 2;
  __IO uint32_t MOD2    : 2;
  __IO uint32_t MOD3    : 2;
  __IO uint32_t MOD4    : 2;
  __IO uint32_t MOD5    : 2;
  __IO uint32_t MOD6    : 2;
  __IO uint32_t MOD7    : 2;
  __IO uint32_t MOD8    : 2;
  __IO uint32_t MOD9    : 2;
  __IO uint32_t MOD10   : 2;
  __IO uint32_t MOD11   : 2;
  __IO uint32_t MOD12   : 2;
  __IO uint32_t MOD13   : 2;
  __IO uint32_t MOD14   : 2;
  __IO uint32_t MOD15   : 2;
  //OTYPER
  __IO uint32_t OTYP0   : 1;
  __IO uint32_t OTYP1   : 1;
  __IO uint32_t OTYP2   : 1;
  __IO uint32_t OTYP3   : 1;
  __IO uint32_t OTYP4   : 1;
  __IO uint32_t OTYP5   : 1;
  __IO uint32_t OTYP6   : 1;
  __IO uint32_t OTYP7   : 1;
  __IO uint32_t OTYP8   : 1;
  __IO uint32_t OTYP9   : 1;
  __IO uint32_t OTYP10  : 1;
  __IO uint32_t OTYP11  : 1;
  __IO uint32_t OTYP12  : 1;
  __IO uint32_t OTYP13  : 1;
  __IO uint32_t OTYP14  : 1;
  __IO uint32_t OTYP15  : 1;
  __IO uint32_t OTYPX   : 16;
  //OSPEEDR
  __IO uint32_t OSPEED0 : 2;
  __IO uint32_t OSPEED1 : 2;
  __IO uint32_t OSPEED2 : 2;
  __IO uint32_t OSPEED3 : 2;
  __IO uint32_t OSPEED4 : 2;
  __IO uint32_t OSPEED5 : 2;
  __IO uint32_t OSPEED6 : 2;
  __IO uint32_t OSPEED7 : 2;
  __IO uint32_t OSPEED8 : 2;
  __IO uint32_t OSPEED9 : 2;
  __IO uint32_t OSPEED10: 2;
  __IO uint32_t OSPEED11: 2;
  __IO uint32_t OSPEED12: 2;
  __IO uint32_t OSPEED13: 2;
  __IO uint32_t OSPEED14: 2;
  __IO uint32_t OSPEED15: 2;
  //PUPDR
  __IO uint32_t PUPD0   : 2;
  __IO uint32_t PUPD1   : 2;
  __IO uint32_t PUPD2   : 2;
  __IO uint32_t PUPD3   : 2;
  __IO uint32_t PUPD4   : 2;
  __IO uint32_t PUPD5   : 2;
  __IO uint32_t PUPD6   : 2;
  __IO uint32_t PUPD7   : 2;
  __IO uint32_t PUPD8   : 2;
  __IO uint32_t PUPD9   : 2;
  __IO uint32_t PUPD10  : 2;
  __IO uint32_t PUPD11  : 2;
  __IO uint32_t PUPD12  : 2;
  __IO uint32_t PUPD13  : 2;
  __IO uint32_t PUPD14  : 2;
  __IO uint32_t PUPD15  : 2;
  //IDR
  __IO uint32_t IDR0    : 1;
  __IO uint32_t IDR1    : 1;
  __IO uint32_t IDR2    : 1;
  __IO uint32_t IDR3    : 1;
  __IO uint32_t IDR4    : 1;
  __IO uint32_t IDR5    : 1;
  __IO uint32_t IDR6    : 1;
  __IO uint32_t IDR7    : 1;
  __IO uint32_t IDR8    : 1;
  __IO uint32_t IDR9    : 1;
  __IO uint32_t IDR10   : 1;
  __IO uint32_t IDR11   : 1;
  __IO uint32_t IDR12   : 1;
  __IO uint32_t IDR13   : 1;
  __IO uint32_t IDR14   : 1;
  __IO uint32_t IDR15   : 1;
  __IO uint32_t IDRX    : 16;
  //ODR
  __IO uint32_t B0      : 1;
  __IO uint32_t B1      : 1;
  __IO uint32_t B2      : 1;
  __IO uint32_t B3      : 1;
  __IO uint32_t B4      : 1;
  __IO uint32_t B5      : 1;
  __IO uint32_t B6      : 1;
  __IO uint32_t B7      : 1;
  __IO uint32_t B8      : 1;
  __IO uint32_t B9      : 1;
  __IO uint32_t B10     : 1;
  __IO uint32_t B11     : 1;
  __IO uint32_t B12     : 1;
  __IO uint32_t B13     : 1;
  __IO uint32_t B14     : 1;
  __IO uint32_t B15     : 1;
  __IO uint32_t BX      : 16;
  //BSRR
    //BS
  __IO uint32_t BS0     : 1;
  __IO uint32_t BS1     : 1;
  __IO uint32_t BS2     : 1;
  __IO uint32_t BS3     : 1;
  __IO uint32_t BS4     : 1;
  __IO uint32_t BS5     : 1;
  __IO uint32_t BS6     : 1;
  __IO uint32_t BS7     : 1;
  __IO uint32_t BS8     : 1;
  __IO uint32_t BS9     : 1;
  __IO uint32_t BS10    : 1;
  __IO uint32_t BS11    : 1;
  __IO uint32_t BS12    : 1;
  __IO uint32_t BS13    : 1;
  __IO uint32_t BS14    : 1;
  __IO uint32_t BS15    : 1;
    //BR
  __IO uint32_t BR0     : 1;
  __IO uint32_t BR1     : 1;
  __IO uint32_t BR2     : 1;
  __IO uint32_t BR3     : 1;
  __IO uint32_t BR4     : 1;
  __IO uint32_t BR5     : 1;
  __IO uint32_t BR6     : 1;
  __IO uint32_t BR7     : 1;
  __IO uint32_t BR8     : 1;
  __IO uint32_t BR9     : 1;
  __IO uint32_t BR10    : 1;
  __IO uint32_t BR11    : 1;
  __IO uint32_t BR12    : 1;
  __IO uint32_t BR13    : 1;
  __IO uint32_t BR14    : 1;
  __IO uint32_t BR15    : 1;
  //LCKR
  __IO uint32_t LCK0    : 1;
  __IO uint32_t LCK1    : 1;
  __IO uint32_t LCK2    : 1;
  __IO uint32_t LCK3    : 1;
  __IO uint32_t LCK4    : 1;
  __IO uint32_t LCK5    : 1;
  __IO uint32_t LCK6    : 1;
  __IO uint32_t LCK7    : 1;
  __IO uint32_t LCK8    : 1;
  __IO uint32_t LCK9    : 1;
  __IO uint32_t LCK10   : 1;
  __IO uint32_t LCK11   : 1;
  __IO uint32_t LCK12   : 1;
  __IO uint32_t LCK13   : 1;
  __IO uint32_t LCK14   : 1;
  __IO uint32_t LCK15   : 1;
  __IO uint32_t LCKK16  : 1;
  __IO uint32_t LCKKx   : 16;
  //AFRL
  __IO uint32_t AFR0    : 4;
  __IO uint32_t AFR1    : 4;
  __IO uint32_t AFR2    : 4;
  __IO uint32_t AFR3    : 4;
  __IO uint32_t AFR4    : 4;
  __IO uint32_t AFR5    : 4;
  __IO uint32_t AFR6    : 4;
  __IO uint32_t AFR7    : 4;
  //AFRH
  __IO uint32_t AFR8    : 4;
  __IO uint32_t AFR9    : 4;
  __IO uint32_t AFR10   : 4;
  __IO uint32_t AFR11   : 4;
  __IO uint32_t AFR12   : 4;
  __IO uint32_t AFR13   : 4;
  __IO uint32_t AFR14   : 4;
  __IO uint32_t AFR15   : 4;
} PORT;

void gpio_init();

#endif