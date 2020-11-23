/**
**===========================================================================
**  @file    main.c
**  @author's:
**  Felipe Silveira Carvalho - Número 6 - Turma 4324
** 	Nathan Mattes - Número: 17 - Turma 4324
**  @date    27/11/2020
**  @version 1.0
**
**===========================================================================
*/
//Objetivo: Projetar e construir um painel capaz de exibir mensagens de texto.

/* Includes */
#include "stm32f4xx.h"


//Variável contador, auxiliar, das posições (representando cada disp), auxiliar, do tamanho da frase
int cont = 0, aux = 0, pos[4]={0,0,0,0}, aux2, tamfrase = 22;
// declarando as posições do display, onde cada uma representa uma letra da mensagem, em hexadecima
int display[]={0x00, 0x00, 0x00, 0x00, 0x79, 0x76, 0x00, 0x3E, 0x6d, 0x00, 0x7D, 0x3E, 0x31, 0x06, 0x00,
				0x30, 0x3F, 0x3F, 0x73, 0x31, 0x79, 0x86, 0x00};

void TIM1_UP_TIM10_IRQHandler (void) // Declarando a função de interrupção do tempo da mensagem
{
	TIM10->SR &=~ TIM_SR_UIF; // zera a contagem

	pos[0] = aux+4; //atribuindo valores para cada um dos displays
	pos[1] = aux+3;
	pos[2] = aux+2;
	pos[3] = aux+1;

	aux++;//acrescenta um a var auxiliar
	if(aux>tamfrase) aux = 0; // se aux for maior do que o total de caracteres da frase, é zerado
}

void TIM1_TRG_COM_TIM11_IRQHandler(void) // Função de interrupção do efeito da retina
{
	TIM11->SR&=~TIM_SR_UIF; // zera a contagem

	GPIOC->ODR &=~0xFF;	// apagando a mensagem do display

	switch(cont) // switch com o contador, cada caso representa um display (unidade, dezena, centena, milhar)
	{
		case 1: // cada caso liga um display e desliga os outros // Unidade
			GPIOB->ODR &=~ (GPIO_ODR_ODR_4|GPIO_ODR_ODR_1|GPIO_ODR_ODR_2);
			GPIOC->ODR|=display[pos[0]]; // Imprime o valor da pos 0
			GPIOB->ODR ^= GPIO_ODR_ODR_0; // Liga o display
			break;

		case 2: // Repete o processo para dezena
			GPIOB->ODR &=~ (GPIO_ODR_ODR_4|GPIO_ODR_ODR_0|GPIO_ODR_ODR_2);
			GPIOC->ODR|=display[pos[1]];
			GPIOB->ODR ^= GPIO_ODR_ODR_1;
			break;

		case 3:
			GPIOB->ODR &=~ (GPIO_ODR_ODR_4|GPIO_ODR_ODR_1|GPIO_ODR_ODR_0);
			GPIOC->ODR|=display[pos[2]];
			GPIOB->ODR ^= GPIO_ODR_ODR_2;
			break;

		case 4:
			GPIOB->ODR &=~ (GPIO_ODR_ODR_0|GPIO_ODR_ODR_1|GPIO_ODR_ODR_2);
			GPIOC->ODR|=display[pos[3]];
			GPIOB->ODR ^= GPIO_ODR_ODR_4;
			break;
	}

		cont++; // acrescenta um ao contador

		if(cont>4) // se for maior do que quatro, volta pra um
		{
			cont = 1;
		}
}

int main(void)
{
//Segmentos displays: A	 B	C	D	E	F	G	DP
//Porta C - Pinos:	  0  1	2   3	4	5	6	7
//Cátodos: PB0, PB1, PB2 e PB4

	int i = 0;//Variavel que movimenta o main

	RCC->AHB1ENR = (RCC_AHB1ENR_GPIOAEN | RCC_AHB1ENR_GPIOBEN | RCC_AHB1ENR_GPIOCEN);//Ativando o clock das portas GPIO

	GPIOA->MODER = (GPIO_MODER_MODER13_1| GPIO_MODER_MODER14_1);//Configurando PA13 e PA14 como função digital alternativa
	GPIOB->MODER = (GPIO_MODER_MODER0_0 | GPIO_MODER_MODER1_0 | GPIO_MODER_MODER2_0 | GPIO_MODER_MODER4_0);//Configurando PB0, PB1, PB2 e PB4 como saída
	GPIOC->MODER = 0x5555;//Configurando PC0 até PC7 como saída

	RCC->APB2ENR = (RCC_APB2ENR_TIM10EN | RCC_APB2ENR_TIM11EN);//Habilitando o clock dos Timers Timer10 e Timer11

	//Configurando TIM11 -> Multiplexação
	TIM11->PSC = 15999;// 16MHz/16kHz=1kHz
	TIM11->ARR = 1;//1kHz/2=500Hz
	TIM11->CR1 = TIM_CR1_CEN;//Habilita a contagem do timer11
	TIM11->DIER |= TIM_DIER_UIE;//Habilita a interrupção do timer11

	//Configurando TIM10 -> Lógica da mensagem
	TIM10->PSC = 15999;// 16MHz/16kHz=1kHz
	TIM10->ARR = 299;//1kHz/300=3,33Hz
	TIM10->CR1 = TIM_CR1_CEN;//Habilita a contagem do timer10
	TIM10->DIER |= TIM_DIER_UIE;//Habilita a interrupção do timer10


	//Habilitando a função de interrupção

	NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);//Função do Timer10
	NVIC_SetPriority(TIM1_UP_TIM10_IRQn,1);//Prioridade 1

	NVIC_EnableIRQ(TIM1_TRG_COM_TIM11_IRQn);//Função do Timer11
	NVIC_SetPriority(TIM1_TRG_COM_TIM11_IRQn,2);//Prioridade 2

  while (1)
  {
	  i++;//Movimento do main
  }
}
