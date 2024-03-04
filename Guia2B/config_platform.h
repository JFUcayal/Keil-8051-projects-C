#ifndef __CONFIG_PLATFORM__
#define __CONFIG_PLATFORM__

//macros
#define setbit(x,n)			x |= (1<<n)
#define clearbit(x,n)		x &=~ (1<<n)
#define togglebit(x,n)	x ^= (1<<n)
#define isbitset(x,n)		((x >> n) & 0x1)	
#define isbitclear(x,n)	!((x >> n) & 0x1) 

#define RI1 0
#define TI1 1

sbit P0_0 = P0^0;
sbit P0_1 = P0^1;
sbit P0_2 = P0^2;
sbit P0_3 = P0^3;
sbit P0_4 = P0^4;
sbit P0_5 = P0^5;
sbit P0_6 = P0^6;
sbit P0_7 = P0^7;

sbit P2_0 = P2^0;
sbit P2_1 = P2^1;
sbit P2_2 = P2^2;
sbit P2_3 = P2^3;
sbit P2_4 = P2^4;
sbit P2_5 = P2^5;
sbit P2_6 = P2^6;
sbit P2_7 = P2^7;

void Init_Device(void);

#endif