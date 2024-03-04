#ifndef __TIMER_MODULE__
#define __TIMER_MODULE__

#define start_timer0() TR0=1
#define stop_timer0() TR0=0

extern volatile bit output_PB1;
extern volatile bit output_PB2;

void Timer0_Init(int reload); //config timer 0 em 8 bits c/ auto_reload

void Timer0_isr(void); //debounce utilizando o sliding window

void Timer0_Interrupt_Init(void(*handler)(void)); //handler aponta para a rotina do debounce

void sliding_window(void); //algoritmo de debounce

#endif