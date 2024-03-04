#include <REG51F380.H>
#include <stdio.h>

enum states{one,two,three};
enum states N_STATE = one;

sbit button   = P0^7;
sbit LED		  = P2^3;
bit flag_30s = 0;


void init_device(){

	//SYSCL
	FLSCL  = 0x90;
	CLKSEL = 0x03;

	P2MDOUT = 0x01;
	P0SKIP  = 0xFF;
	P1SKIP  = 0xFF;
	XBR1	  = 0x41;

	//PCA
	PCA0CN   = 0x40;
	PCA0MD   = 0x03;
	PCA0CPM0 = 0x42;
	
	PCA0CPH0 = 0x4D; //77 -> 30% duty-cycle inicial
	
	
	//timer
	TCON			 = 0x10; //TR0 = 1
	TMOD			 = 0x02; //T0 8bits c/auto-reload
	CKCON		   = 0x02; //SYSCLK/48
	
  TL0			   = -0xFF;
	TH0			   = -0xFF;
	
	//interrupts
	EIE1 = 0x12;
	EA 	 = 1;

}

//----------------------------------------------------

void isr_PCA(void) interrupt 11 using 2 {

		static unsigned char counter = 30;
	
		CF = 0;
		counter--;
	
		if(counter == 0){
				PCA0CPH0 -= 0x19; //aumentar 10% duty-cycle
				counter = 30;
		}
}

void isr_TMR0(void) interrupt 1 using 3 {

		static unsigned char tmr_count = 6;
		
		TF0 = 0;
		tmr_count--;

		if(tmr_count == 0){
				flag_30s = 1;
				tmr_count = 6;
		}	
}

//----------------------------------------------------

void s_wave(){
	
	if(!button){
		
		//Desligar a onda -> ECOM0 = 1
		
		PCA0CPM0 = 0x00;
		N_STATE = two;
	}
}


void s_led(){
	
		LED = 0;
		TR0 = 1;
	
		//if(flag_30s){
		if(!button){
			
		//flag_30s = 0;
		N_STATE = three;
	}
}


void s_off(){

		LED = 1;
	
	  if(!button){
	
		N_STATE = one;
	}
}

//----------------------------------------------------

void main(){
	
	enum states STATE = one;
	
	init_device();

	while(1){
	
		STATE = N_STATE;
	
		switch(STATE){
			case one:
				s_wave();	
					 break;
			
			case two:
				s_led();
				  break;
			
			case three:
				s_off();
				  break;	
		}
	}
}