#include <REG51F380.H>
#include <stdio.h>
#include "config_platform.h"
#include "driver_uart_1.h"
#include "timer_module.h"

#define uint8_t unsigned short
	
enum states{one,two,three};
enum states N_STATE = one;

unsigned char duty_vals[64] = {127, 139, 152, 164, 176, 187, 198, 208, 217, 225, 233, 239, 244, 249, 252, 253, 254, 253, 252, 249, 244, 239, 233, 225, 217,
208, 198, 187, 176, 164, 152, 139, 127, 115, 102, 90, 78, 67, 56, 46, 37, 29, 21, 15, 10, 5, 2, 1, 0, 1, 2, 5, 10, 15, 21, 29, 37, 46, 56, 67, 78, 90, 102,
115};

bit flag_PB0 = 0;			
bit last_PB0 = 1;

sbit PB1  = P0^6;
sbit PB2  = P0^7;

const int DELTA = 0x19; //variação do duty 10%

//---------------------------------------------------------------
void plus_bright(void){

	//verificar que nao passa do valor maximo de brilho
		
	if(PCA0CPH0 == 0){ 
	
		PCA0CPH0 = 0x00; //duty cycle = 100%		
	}		
	PCA0CPH0	-= DELTA; //aumentar brilho		
}


void minus_bright(void){
	
	//verificar que nao passa do valor minimo de brilho
	
	if(PCA0CPH0 < 0xFF){ //FF-> maximo 
	
		//0% duty cycle clearbit ECOM0 = 0  clearbit(ECOM0,PCA0CPM0);
		PCA0CPH0 = 0xFF;
		clearbit(PCA0CPM0, 6);
		
		PCA0CPH0	+= DELTA; //diminuir brilho
	}		
}

//---------------------------------------------------------------
//verifica o que vem da UART1
void uart_check(char c){
	
			if(c == 'f'||c == 'F'){
					//sinewave
					//enable interrupt PCA
				
				  N_STATE = three;							
				
			}else if(c == 's' || c == 'S') {
					//speed controller
					PCA0CN = 0x40; //PCA timer enable
					setbit(PCA0CPM0, 6); //ECOM0 = 1;
								
						N_STATE = two;
									
			}else if(c == 'p' || c == 'P') {
					//stopped
				
					N_STATE = one;	
				
			} else if(N_STATE == two){
					if(c == 'i' || c == 'I') {
									//incrementar duty cycle
									plus_bright();
									
					} else if(c == 'd' || c == 'D') {
									//decrementar duty cycle 
									minus_bright();					
					} 
		 }
}

//---------------------------------------------------------------
//Funcao para detetar a ativadade nos botoes ->  sem debounce
void check_buttons(){

		if(!PB1){
			while (!PB1);		
				plus_bright();			
		}
		
		if(!PB2){
			while (!PB2);		
			  minus_bright();
		}
}
//---------------------------------------------------------------

/*
void buttons_check(bit former_PB1_output ,bit former_PB2_output){
	
		//PB1
		if(former_PB1_output != output_PB1){
			if(!output_PB1){ //se estiver a 0 -> foi pressionado logo incrementa
				//aumentar brilho
				PCA0CPH0 -= DELTA;	 //aumenta o duty cycle
			}
		}

		//PB2
		if(former_PB2_output != output_PB2){
			if(!output_PB1){ //se estiver a 0 -> foi pressionado logo decrementa
				//diminuir brilho
				PCA0CPH0 += DELTA;	//diminui o duty cycle
			}
		}		
}
*/

//---------------------------------------------------------------
//funcao debounce que é apontada pelo handler

void timer0_callback(){
		sliding_window();
}

//---------------------------------------------------------------

void isr_PCA(void) interrupt 11 using 1{

	static unsigned char counter = 5;
	static unsigned char index = 0;
	
	CF = 0;
	counter--;
	
	if(counter == 0){
		
		index++;
		PCA0CPH0 = (duty_vals[index & 64]); //mascara com 64 para dar a volta aos valores do array
		
		counter = 5;		
	}
}

//---------------------------------------------------------------

void stopped(){
	
		//0% duty cycle clearbit ECOM0 = 0 
		PCA0CPH0 = 0xFF; //diminuir duty cycle
		clearbit(PCA0CPM0, 6); //ECOM0 = 0;			  
}

void speed_controller(){
		PCA0CN = 0x40;
		setbit(PCA0CPM0, 6);
		//enable interrupt PCA
		
		check_buttons(); //incrementar e decrementar a partir dos botoes
}

void sinewave(){
		PCA0CN = 0x40;
		setbit(PCA0MD, 1); //enable interrupt PCA
		setbit(PCA0CPM0, 6); //ECOM0 = 1;			  
		setbit(PCA0CPM0, 1); //enable flag interrupt 	
}

//---------------------------------------------------------------
void main(void){
	
	enum states STATE = one;
	char c;
	
	Init_Device();
	init_UART1();

	Timer0_Init(06); //contar de 6 a 256 de forma a obter 250x -> isr de 250 em 250us
	Timer0_Interrupt_Init(timer0_callback); //inicializaçao dos parametros da interrupt -> timer e variaveis
		
	//PCA0CN = 0; //começar com o output de PWM desligado

	EA = 1;
	start_timer0();
	
	while (1){
		
		c = getKey_non_block();
		uart_check(c);	
		
		STATE = N_STATE;
		
		switch(STATE){
			
			//STOPPED
			case one:
					stopped();
					break;
			//SPEED CONTROLLER
			case two:
					speed_controller();
				  break;
			//SINEWAVE
			case three:
					sinewave();
				  break;	
		}		
	}
}
