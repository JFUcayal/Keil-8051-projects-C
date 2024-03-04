#include <REG51F380.H>
#include "driver_uart_1.h"
#include "config_platform.h"

#define ES1 1 //ativar as interrupts da UART1

//buffer tem de ter um tamanho de 2^n
#define TXBUFF_SIZE 16
#define RXBUFF_SIZE 16

unsigned char pdata txbuff[TXBUFF_SIZE];
unsigned char pdata rxbuff[RXBUFF_SIZE];

//buffer de transmissao
volatile unsigned char tx_write_index = 0;
volatile unsigned char tx_read_index = 0;

//buffer de receçao 
volatile unsigned char rx_write_index = 0;
volatile unsigned char rx_read_index = 0;

bit tx_reset = 1;

void init_UART1(void){
		SCON1 = 0x10;			//UART 1 control 
		setbit(EIE2,ES1);	//ativar a interrupt da uart1
}


unsigned char get_rxbuffsize(void){
		return rx_write_index - rx_read_index; //verificar se a FIFO esta cheia ou vazio consoante o resultado
}


unsigned char get_txbuffsize(void){
		 return tx_write_index - tx_read_index; //verificar se a FIFO esta cheia ou vazio consoante o resultado
}


int putchar_UART1(unsigned char c){
		if(get_txbuffsize() >= TXBUFF_SIZE){ //se o buffer estiver cheio da return de erro->ENOBUFS == 105
				return ENOBUFS;
			
			txbuff[tx_read_index & (TXBUFF_SIZE - 1)] = c; 
			tx_write_index ++; 
			
			if(tx_reset){		
				tx_reset=0;
				setbit(SCON1,TI1);
			}			
		}
		
		return EXIT_SUCESS; // == return 0-> sem erros		
}


int getchar_UART1(void){
		if(get_rxbuffsize() == 0){ //se nao tiver nada para mandar da return de erro->ENODATA == 61
				return ENODATA;
		}
		
		return rxbuff[rx_read_index++ & (RXBUFF_SIZE - 1)]; //return da info recebida do buffer da UART 1
}


char getKey_block(void){
    int key;
    
    do{
        key = getchar_UART1();
    }
    while(key == ENODATA); //return 61-> no data available in FIFO
    
    return(key);  
}


char getKey_non_block(void){
    return getchar_UART1();
}


char putChar_block(char c){
    unsigned char loop_val = 0;
    
    if(c == '\n'){ 
        while(putchar_UART1(0xd) != 0)       //wait until '\r' is accepted
            for(loop_val = 0; loop_val < 50; loop_val++);   //wait loop
    }
    
    while(putchar_UART1(c) != 0)    //wait until character is accepted
        for(loop_val = 0; loop_val < 50; loop_val++); //wait loop
    
    return c;
}


void UART1_isr(void) interrupt 16 {
	
		//RX
	  if(isbitset(SCON1,RI1)) //entrar na interrupt se a flag de rececao estiver a 1
    {
            clearbit(SCON1,RI1); //limpar a flag da interrupt
       
            if(((rx_write_index - rx_read_index)&~(RXBUFF_SIZE-1))==0) //
            {
                    rxbuff [rx_write_index & (RXBUFF_SIZE-1)] = SBUF1; //mandar para o buffer a info que esta na UART
                    rx_write_index++; //incrementar o index de leitura 
            }
    }
     
		//TX	
    if(isbitset(SCON1,TI1)) //entrar na interrupt se a flag de transmissao estiver a 1
    {
            clearbit(SCON1,TI1); //entrar na interrupt se a flag de transmissao estiver a 1

            if(tx_write_index != tx_read_index) //verificar se ambos os index's estao na mesma posição
            {
                    SBUF1 = txbuff [tx_read_index & (TXBUFF_SIZE-1)]; //enviar para a UART a info atual
                    tx_write_index++; //incrementar o index de escrita 
                    tx_reset = 0; 
            }
            else //se os index's estiverem na mesma posição ent ha um reset no buffer
                    tx_reset = 1;
    }
}