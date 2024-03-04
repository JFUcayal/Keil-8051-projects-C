#pragma small								//compact->warning
#include <REG51F380.H>
#include "config_platform.h"


// Initialization function for device,
// Call Init_Device() from your main program
void Init_Device(void)
{
		//config wiz->UART 1
		FLSCL     = 0x90;
		CLKSEL    = 0x03;
		PCA0MD    = 0x00;
		P0SKIP    = 0x0F;
		XBR1      = 0x40;
		XBR2      = 0x01;
		SBRLL1    = 0x30;
		SBRLH1    = 0xFF;
		SBCON1    = 0x43;
		SCON1     = 0x10;
	
		setbit(SCON1,TI1);
}
