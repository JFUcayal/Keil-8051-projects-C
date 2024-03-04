#ifndef __DRIVER_UART_1_H__
#define __DRIVER_UART_1_H__

#define EXIT_SUCESS 0 //Return without errors
#define ENODATA 61		//No data available on FIFO
#define ENOBUFS 105		//No buffer space available on FIFO



void init_UART1(void); //ativa a interrupt e rececao da UART 1

unsigned char get_rxbuffsize(void);

unsigned char get_txbuffsize(void);

int putchar_UART1(unsigned char c); //escreve um char para o buffer de transmissao

int getchar_UART1(void); //lê um char para o buffer de rececao

char getKey_block(void); //getchar com bloqueio->bloqueia ate receber um char da uart 1

char getKey_non_block(void); //getchar sem bloqueio-> retorna ENODATA em caso de nao receber nada

char putChar_block(char c); //putchar com bloqueio->bloqueia se nao houver mais espaço no buffer

void UART1_isr(void); //interrupt para enviar e reveber a partir do buffer da UART 1

#endif