#include "config.h"
#include <REG51F380.H>

void init(void){
	
	FLSCL     = 0x90;
	CLKSEL    = 0x03;
	
	PCA0MD		= 0x00;
	
	//coloca o pino de pwm no p2.0
	P0SKIP    = 0xFF;
	P1SKIP    = 0xFF;
	XBR1      = 0x41;
	
	//quando há overflow coloca o bit PCA0CN.7 a 1
	
	
	PCA0CPH0	= 0x80; 	//128 para colocar o duty cicle a 50%  
											//(256-x)/256
	
	//colocar o low??
	
	PCA0CPM0	= 0x42;		//coloca no modo de comparador e no modo PWM de 8 bits
	
	
	
	//PCA0MD    = 0x01;		//permite interrupt por overflow do timer PCA
	//neste registo tambem podemos mudar o clk do PCA
	
	
	PCA0CN    = 0x40;		//começa o timer do PCA
	
	
	//clear bit ECON para dutty cycle de 100%
	
//	EIE1			= 0x10;
//	IE        = 0x80;	
	
}

