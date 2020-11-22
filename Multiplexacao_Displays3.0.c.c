

/* Includes */
#include "stm32f4xx.h"

int cont = 0, aux = 0;
int display[]={0x79, 0x76, 0x00, 0x3E, 0x7D, 0x3E, 0x31, 0x06, 0x00, 0x30, 0x3F, 0x3F, 0x73, 0x31, 0x79, 0x86}; // Números

void TIM1_TRG_COM_TIM11_IRQHandler(void)
{
	TIM11->SR&=~TIM_SR_UIF;

	GPIOC->ODR &=~0xFF;	// apagando a mensagem do display
	GPIOB->ODR |=0x17;	//desligando todos os displays

	switch(cont)
	{
		case 0:
			GPIOC->ODR|=display[aux];
			GPIOB->ODR &=~ GPIO_ODR_ODR_0;
			break;

		case 1:
			GPIOC->ODR|=display[aux];
			GPIOB->ODR &=~ GPIO_ODR_ODR_1;
			break;

		case 2:
			GPIOC->ODR|=display[aux];
			GPIOB->ODR &=~ GPIO_ODR_ODR_2;

			break;

		case 3:
			GPIOC->ODR|=display[aux];
			GPIOB->ODR &=~ GPIO_ODR_ODR_4;
			break;

		default:
			aux++;
			break;
	}


		cont++;
		if(cont>4)
		{
			cont = 0;

		}
		if(aux>15) aux = 0;
}

int main(void)
{
//Segmentos displays: A	 B	C	D	E	F	G	DP
//Porta C - Pinos:	  0  1	2   3	4	5	6	7
//Cátodos: PB0, PB1, PB2 e PB4

	int i = 0;

	RCC->AHB1ENR = (RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN);

	GPIOA->MODER = (GPIO_MODER_MODER13_1| GPIO_MODER_MODER14_1);
	GPIOB->MODER = (GPIO_MODER_MODER0_0 | GPIO_MODER_MODER1_0 | GPIO_MODER_MODER2_0 | GPIO_MODER_MODER4_0);
	GPIOC->MODER = 0x5555;

	RCC->APB2ENR = RCC_APB2ENR_TIM11EN;

	//Configurando TIM10 -> Multiplexação
	TIM11->PSC = 15999;
	TIM11->ARR = 99;
	TIM11->CR1 = TIM_CR1_CEN;
	TIM11->DIER |= TIM_DIER_UIE;


	//Habilitando a função de interrupção

	//NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);
	//NVIC_SetPriority(TIM1_UP_TIM10_IRQn,2);

	NVIC_EnableIRQ(TIM1_TRG_COM_TIM11_IRQn);
	NVIC_SetPriority(TIM1_TRG_COM_TIM11_IRQn,1);



  while (1)
  {
	  i++;


  }

}
