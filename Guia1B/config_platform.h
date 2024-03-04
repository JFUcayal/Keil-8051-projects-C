#ifndef __CONFIG_PLATFORM__
#define __CONFIG_PLATFORM__

//macros
#define setbit(x,n)			x|=(1<<n)
#define clearbit(x,n)		x&=~(1<<n)
#define togglebit(x,n)	x^=(1<<n)
#define isbitset(x,n)		((x>>n)& 0x1)	
#define isbitclear(x,n)	!((x>>n)&0x1) 

#define RI1 0
#define TI1 1

void Init_Device(void);

#endif