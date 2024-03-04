#include <REG51F380.H>
#include "config.h"





void main(void){
	init();
	
	while (1){
		if(!P0_6){
			while (!P0_6);
			
			PCA0CPH0	+= 0x10;
		}
		if(!P0_7){
			while (!P0_7);
			
			PCA0CPH0	-= 0x10;
		}
	}
}