/**
 ******************************************************************************
 * @file           : main.c
 * @author         : NL
 * @brief          : Main program body,
 * 					 simple application to test and debug
 ******************************************************************************
 */

#include <stdint.h>
#include "stm32f4xx.h"
#define SECOND 1600000
#define AFRL AFR[0]
#define AFRH AFR[1]

#define STRSIZE 50

USART_TypeDef* us2;

    //Adds one char in Putty terminal
	int __io_putchartest(int ch){
		//bit 7 (TXE) of Status Register  barrier until the transmit data register is empty
		while(!(us2->SR&(1<<7)));
		us2->DR = ch;// transmit char
		return (int)ch;
	}

   //Adds a sequence of chars in Putty Terminal
	void USART2_Transmittest(uint8_t * data, uint32_t len){
		int i=0;
		while(i<len){
			__io_putchartest(data[i++]);
		}
	}


	int __io_getchartest(void){
		//copy char in data register
		auto c=us2->DR;
		//return -1 if EOF else ASCII code of char
		return c=='\0' ? -1 : (int)c;
	}

	void wait(int nbSec){
		int cpt=0;
		while((++cpt)/SECOND<nbSec);
	}

	int32_t USART2_Receivetest(uint8_t * data, uint32_t len, uint32_t timeout){
		int i=0;
		timeout/=4;//Due to the latence in the counting process it takes about 4 times longer to reach one second.
		while(i<len){
			int cpt=0;
			//bit 5 of SR(RXNE) Read data register not empty
			while((us2->SR & (1<<5))==0){
				//wait till timeout then exit
				++cpt;
				if(i==0){
					if(cpt/SECOND>timeout ? 1 : 0)return -1;
				}
				else if((float)cpt/(float)SECOND>0.5f ? 1 : 0) return i;
			}
			//get char
			int c=__io_getchar();
			//reached EOF
			if(c==-1)break;
			//COPY CHAR
			data[i]=c;
			++i;//Next char
		}
		return i;
	}

void resetStringtest(char * data,int N){
	for(int i=0;i<N;++i){
		data[i]='\0';
	}
}

/**
  * @brief main function
  * @param  none
  * @retval 0 if success
 */

int func(void)
{
	//Activez l’horloge du périphérique GPIOA (RCC->AHB1ENR |= 1) pour pouvoir le configurer
	RCC->AHB1ENR |= 1;
	GPIO_TypeDef * PA = GPIOA;
	//• Configurez les PIN PA2 et PA3 en mode alternative sur la fonction N°7 AFRL
	PA->MODER&=~(0xF<<4);//SETS MODER2-3 TO 0;
	PA->MODER|=(0xA<<4);//sets MODER2-3 to 10 each
	PA->AFRL&=~0xFF00;//Resets AFRL2-3 To 0;
	PA->AFRL|=0x77<<8;//Sets AFRL2-3 To 0111 each;
	//• Activez l’horloge du périphérique USART2 qui est par défaut désactivée : registre RCC (RCC→APB1ENR |=
	//0x20000).
	RCC->APB1ENR |= 0x20000;//enable CLOCK
	//• Configurez si nécessaire les registres CR2 et CR3 de l’USART pour 1 bit de STOP, pas de parité
	us2= USART2;

	//1 stop bit
	us2->CR2&=~(0x3<<12);//sets bit 12-13 to 00
	//Disable parity control
	us2->CR1&=~(1<<10);//sets bit 10 of CR1 to 0

	//• Configurez la vitesse à 115200 b/s (registre BRR de l’USART) avec l’oversampling réglé à 16 la formule est :
	//Tx/Rx baud = F_HPB1 / (16 * USARTDIV) où USARTDIV est un nombre non signé en virgule fixe avec
	//la partie fractionnel codée sur 4 bits (Cf p 519 de la doc)
	us2->BRR=SystemCoreClock/115200 +1;


	//• Activez l’émission, la réception et le périphérique avec 8 bits de données et un oversampling de 16 (registre
	//CR1 de l’SART)
	us2->CR1&=~(1<<15);//oversampling 16
	us2->CR1 |= USART_CR1_TE | USART_CR1_RE;// emission et réception
	us2->CR1 |= USART_CR1_UE; // activate usart
	us2->CR1 &=~(1<<12);//word length 8 bits

	//2) Utilisation

	uint8_t result[STRSIZE];
	int timeout=5;
	while(1){
		//will wait for the first character until timeout seconds has passed. then will stop collecting
		//either when no character has been typed for 0.5s or we have reach STRSIZE character
		if(USART2_Receivetest(result,STRSIZE,timeout)==-1)continue;//if there's no new character we don't print anything
		USART2_Transmittest(result,STRSIZE);//prints in Putty what was typed in putty
		resetStringtest(&result,STRSIZE);//sets all chars to '\0'
	}
	int debugPoint=0;
}



