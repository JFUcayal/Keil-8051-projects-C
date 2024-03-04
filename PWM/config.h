#ifndef __CONFIG__
#define __CONFIG__
#include <REG51F380.H>


//macros para operar bits em registos não endereçáveis ao bit
#define clearbit(x,n)		x&=~(1<<n)			//bit=0;
#define setbit(x,n)			x|=(1<<n)				//bit=1;
#define isbitset(x,n) 	((x>>n)&0x1)		//bit=1?
#define isbitclear(x,n) !((x>>n)&0x1)		//bit=0?
#define togglebit (x,n) x^=(1<<n)				//bit xor 1 ->> cpl bit



sbit P2_7 = P2^7;

void init(void);




#endif