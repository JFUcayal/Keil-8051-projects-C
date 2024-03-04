#include <REG51F380.H>
#include <stdio.h>

void init_device(){

	 FLSCL     = 0x90;
   CLKSEL    = 0x03;

   P2MDOUT   = 0x01;
   P0SKIP    = 0xFF;
   P1SKIP    = 0xFF;
   XBR1      = 0x41;

	 TCON			 = 0x10; //TR0 = 1
	 TMOD			 = 0x02; //T0 8bits c/auto-reload
	 CKCON		 = 0x02; //SYSCLK/48
	
	 TL0			 = 0x02;
	 TH0			 = 0x02;
	
	 PCA0CN    = 0x40; //enable do timer PCA
	
   PCA0MD    = 0x03; //SYSCLK/4 + Eable interrupt do PCA a partir do CF 
	 //PCA0MD    = 0x04; //timer 0 overflow -> PCA clock mux
	
	
   PCA0CPM0  = 0x42; //Modo PCA ativar interrupt timer PCA
	 //PCA0CPM0  = 0x46; //Modo frequency mode
	
	
	 PCA0CPH0  = 0x80; 
	
	 EIE1      = 0x10; 
	 EA				 = 1;
}



//PCA overflow interrupt
void isr_PCA() interrupt 11 using 2 {

		static unsigned char count = 30;	

		CF = 0; 
		count--;
	
		if(count == 0){
				
			PCA0CPH0 -= 0x19;
			count = 30;
		}
}



void main(){

	init_device();
	
	while(1){
	
			
	}
}
