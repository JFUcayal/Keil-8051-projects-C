#include <REG51F380.H>      
#include "config_platform.h"

// Initialization function for device,
// Call Init_Device() from your main program
void Init_Device(void)
{
		//config wiz->UART 1
		FLSCL     = 0x90; //SYSCLK @48MHz
		CLKSEL    = 0x03; 
	
		PCA0MD    &= 0x40; //watchdog_tmr desligado
		PCA0MD    = 0x00; //watchdog_tmr desligado
	  PCA0CPM0  = 0x42;	//PWM 8-bits
	
		P0SKIP    = 0x07; //UART1-> P0.4/5
	
		XBR1      = 0x41; //CEX0 routed to port pins + xbar enable(XBARE)
		//quando há overflow coloca o bit PCA0CN.7 a 1
	
		XBR2      = 0x01; //UART1 enable
		SBRLL1    = 0x30; //UART1 @115200bps
		SBRLH1    = 0xFF; 
	
		SBCON1    = 0x43; //SYSCLK/1
		SCON1     = 0x10; //RECEIVE ENABLE

		setbit(SCON1,TI1);//Tx interrupt flag 
	
	//------------------------------------------------------------------------
	//PWM
	
	//PCA0MD    = 0x01;		//permite interrupt por overflow do timer PCA
	//neste registo tambem podemos mudar o clk do PCA	
	
		PCA0CPM0 = 0X42;	//coloca no modo de comparador e no modo PWM de 8 bits
		
		PCA0CPH0 = 0x80;  //duty-cycle
											//128 para colocar o duty cicle a 50%  
											//(256-x)/256
		      	
		PCA0CPL0 = 0x20;  //frequencia?
		     
		PCA0CN 		= 0x40;	//PCA counter/timer enable
		P0MDOUT   = 0x08;	//P0.3 is push-pull
		
}

//	EIE1			= 0x10;
//	IE        = 0x80;	