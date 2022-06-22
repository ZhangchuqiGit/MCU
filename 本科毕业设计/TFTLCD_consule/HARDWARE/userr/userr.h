#ifndef __USERR_H
#define __USERR_H		  

#include "sys.h"
#include "usart.h"
#include "delay.h"

#include "led.h"
#include "key.h"
#include "adc.h"
#include "lcd.h"
#include "picture.h"
#include "font.h" 
#include "touch.h"
#include "24l01.h" 	 

#define ooioio   1

void display_sensor(void);
void display_main(void);
void display_col(void);
void display_sensor_data(void);
void display_main_data(void);
void display_col_data(void);
void display_led_data(void);
void RX_(void);
void TX_(void);
void num_char(u8 *buf_1,u8 *buf_2,u8 i);
void NRF_check(void);
void NRF_01(u8 num_nrf); //  1:RX模式      ;    0:TX模式  
void cmp_sensor(u16 x,u16 y,u8 *p,u16 set_numL,u16 set_numH);
	
/************** RXģʽ *****************/
void RX_(void);
/************** TXģʽ *****************/
void TX_(void);


extern u8 tx_buf[32];  

extern u8 flag_func;
extern u8 NRF_Check;
extern u8 TX_RX;
extern u8 ledf;
extern u8 nrf_gg;
extern u8 nrf_d;
extern u8 bnb[2];


#endif  	


