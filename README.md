# test
target stm32f303vct6

## part two description
 * using ADC1 ch16 with DMA
 * ADC run after triggering TIM15
 * TIM2 300Hz, TIM3 1KHz, TIM4 10KHz
 * using bit band for work with GPIO
 * PORTE13 toggle with freq 300Hz, PORT12 toggle with freq 1KHz, PORT11 toggle with freq 10KHz

 ## part one description
 * requred LL driver :(
 * using USART1 with baud 460800
 * using DMA1 CH4 for TX and CH5 for RX
 * need handling recieving DMA1 CH5 timeout (handling disconnect in resieve time)
 * need test and refactoring

### release notes
* tag 0.2.0 complite part two
* tag 0.1.0 complite part one (need testing)


