#include <REG51F380.H>
#include "timer_module.h"

//VARIAVEIS
volatile bit output_PB1 = 1;
volatile bit output_PB2 = 1;
static bit callback_ready = 0; //bit que indica se a ISR esta pronta para executar ou nao


//apontador para funcao->escolher em tempo de exec qual função executar
static void (*isr_callback)(void);


void Timer0_Init(int reload){ 											//reload == 0x06 ???
	
		TMOD      = 0x02;		//8 bits c/ auto_reload
    CKCON     = 0x02;		//f=1MHz -> T=1us
    TL0       = (reload);		//contar 250x de forma a obter interrupts de 250us
    TH0       = (reload);
}


void Timer0_isr(void) interrupt 1 using 2{				//interrupt 1 -> lista de prioridades a 1 == timer 0 ?
	if(callback_ready)
	{
		callback_ready=0;
		isr_callback();
		
		//reset à flag automatico com auto_reload	
	}
}

void Timer0_Interrupt_Init(void(*handler)(void)){
	
	//apontador de funcao toma o nome do argumento
	isr_callback = handler;
	callback_ready = 1;
	
	//TF0=0; //? -> NECESSARIO COM AUTO_RELOAD?
	ET0=1; //ativa as interrupçoes do timer 0
}

//-------------------------------------------------------------------------------
//função debounce
void sliding_window(void){

	static unsigned char window_PB1 = 0xff; //byte preenchido com 1's ->1 == não pressionado
	static unsigned char window_PB2 = 0xff;
	static unsigned char count1s_PB1 = 8; //contador de 1's começa no valor max
	static unsigned char count1s_PB2 = 8; 
	unsigned char pin_sample;
	
	
	
	//if para pin sample ser 0
	//break point dentro do if
	
	
	//----------------------debounce PB1 - P0_6 -----------------------------------------
	
	//output_PB1 = 1; //output do botao começa a 1->não pressionado
	
	//TF0=0; //limpar as flags de interrupçao do timer 0
	
	pin_sample=(unsigned char)P0^6; //retirar uma amostra do botao por interrupção
	count1s_PB1 -= (window_PB1>>7); //apenas fica o MSB na window e subtrai ao contador de 1's
	count1s_PB1 += pin_sample; //somar a amostra retirada nesta interrupção
	window_PB1<<=1; //deslizar a janela 1 bit para a esq -> retirar o valor mais antigo da janela	
	window_PB1|=pin_sample; //OR logico->colocar a amostra no bit0->novo valor no LSB

	output_PB1=((4-count1s_PB1)>>7);
	//A saida é 1 se count1s > 4
	//A saida é 0 se count1s <= 4
	
	//----------------------debounce PB2 - P0_7 -----------------------------------------
	
	//output_PB2 = 1; //output do botao começa a 1->não pressionado
		
	//TF0=0; //limpar as flags de interrupçao do timer 0
	
	pin_sample=(unsigned char)P0^7; //retirar uma amostra do botao por interrupção
	count1s_PB2-=(window_PB2>>7); //apenas fica o MSB na window e subtrai ao contador de 1's
	count1s_PB2+=pin_sample; //somar a amostra retirada nesta interrupção
	window_PB2<<=1; //deslizar a janela 1 bit para a esq -> retirar o valor mais antigo da janela
	window_PB2|=pin_sample; //OR logico->colocar a amostra no bit0->novo valor no LSB
	
	output_PB2=((4-count1s_PB2)>>7);
	//A saida é 1 se count1s > 4
	//A saida é 0 se count1s <= 4
	
	
	/*
	if(output_PB != last_PB)
		flag_PB = 1;	//flag_PB==1->nao esta pressionado

	last_PB = output_PB; //update do PB0
	*/
	
}
