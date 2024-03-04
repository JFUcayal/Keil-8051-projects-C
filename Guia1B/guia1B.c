#include <REG51F380.H>
#include <stdio.h>
#include "timer_module.h"
#include "config_platform.h"
#include "driver_uart_1.h"

//#include "compiler_defs.h"
//#include "C8051F380_defs.h"

#define ARRAY_SIZE 16
#define uint8_t unsigned short

bit flag_PB0 = 0;			
bit last_PB0 = 1;

sbit P2_7 = P2^7;
sbit P2_0 = P2^0;
sbit PB1  = P0^6;
sbit PB2  = P0^7;

int count = 0;
bit flag_update = 1;

unsigned char uart_nums[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
unsigned char numeros[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x88,0x83,0xC6,0xA1,0x86,0x8E};

//-------------------------------------------------------------------------------
//funcao debounce que é apontada pelo handler
void timer0_callback(){
		sliding_window();
}

//-------------------------------------------------------------------------------
//delays de leitura do teclado e botoes
void simple_delay(void)
{
		int i;
		for(i=0;i<32000;i++);
}

void delay_loop(void)
{
	int i;
	for(i=0;i<10;i++){simple_delay();}
}

//-------------------------------------------------------------------------------
//funcoes de verificacao
//-------------------------------------------------------------------------------
//verifica os botoes pb1 e pb2
void buttons_check(bit former_PB1_output ,bit former_PB2_output){
			
	//PB1
	if(former_PB1_output != output_PB1){
		if(!output_PB1){ //se estiver a 0 -> foi pressionado logo incrementa
			count++;
			flag_update=1;
		}
	}

	//PB2
	if(former_PB2_output != output_PB2){
		if(!output_PB1){ //se estiver a 0 -> foi pressionado logo decrementa
			count--;
			flag_update=1;
		}
	}		
} 

//verifica o que vem da UART1
void uart_check(char c){
	
			if(c == 'i'||c == 'I'){
					//incrementar
					count++;
					flag_update = 1;			
				
			} else if(c == 'd'||c == 'D') {
					//decrementar
					count--;
					flag_update = 1;
			} 
}

//-------------------------------------------------------------------------------

void main(void)
{
	bit former_PB1_output = output_PB1;
	bit former_PB2_output = output_PB2;
	
	char c;	
	
	//inicializações
	Init_Device();
	init_UART1();
	Timer0_Init(06); //contar de 6 a 256 de forma a obter 250x -> isr de 250 em 250us
	Timer0_Interrupt_Init(timer0_callback); //inicializaçao dos parametros da interrupt -> timer e variaveis
	
	EA = 1;	//ENABLE ALL INTERRUPTS
	//setbit(SCON1,1); 
	
	start_timer0(); //ativar o timer 0 para controlar as interrupts
	P2=numeros[count & (ARRAY_SIZE-1)];
	
	putChar_block(uart_nums[count]); 
	putChar_block('\n');
	
	while(1){
		
		if(flag_update){
				count = count & (ARRAY_SIZE-1);
				P2=numeros[count & (ARRAY_SIZE-1)];
			  
				putChar_block(uart_nums[count]);
				putChar_block('\n');
        			
				flag_update=0;
		}
		
		//receive UART					
		c = getKey_non_block();
		buttons_check(former_PB1_output,former_PB2_output);
		uart_check(c);
		
		//former_PB1_output = output_PB1;
		//former_PB2_output = output_PB2;	
		
		delay_loop();
		
	}
}	