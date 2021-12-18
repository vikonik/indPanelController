#include "MDR1986VE1T.h"                // Device header
#include "MDR32F9Qx_port.h"             // Keil::Drivers:PORT

//PORT A

//#define _NAME_ PORT_Pin_0
//#define _NAME_ PORT_Pin_1
//#define _NAME_ PORT_Pin_2
//#define _NAME_ PORT_Pin_3  //Шина дисплея
//#define _NAME_ PORT_Pin_4  //
//#define _NAME_ PORT_Pin_5  //  
//#define _NAME_ PORT_Pin_6  //  
//#define _NAME_ PORT_Pin_7  // 
//#define _NAME_ PORT_Pin_8  //  
//#define _NAME_ PORT_Pin_9  //  
//#define _NAME_ PORT_Pin_10 //
//#define _NAME_ PORT_Pin_11 
//#define _NAME_ PORT_Pin_12
//#define _NAME_ PORT_Pin_13
//#define _NAME_ PORT_Pin_14
//#define _NAME_ PORT_Pin_15

//PORT B
#define LED_DIGIT_PORT 	MDR_PORTB
#define SEG_A	 PORT_Pin_0
#define SEG_B	 PORT_Pin_1
#define SEG_C	 PORT_Pin_2
#define SEG_D	 PORT_Pin_3  //
#define SEG_E	 PORT_Pin_4  //
#define SEG_F	 PORT_Pin_5  //  
#define SEG_G	 PORT_Pin_6  //  
#define SEG_H  PORT_Pin_7  // 
#define SEG_I  PORT_Pin_8  //  
#define SEG_J  PORT_Pin_9  //  
//#define _NAME_ PORT_Pin_10 //
//#define _NAME_ PORT_Pin_11 
//#define _NAME_ PORT_Pin_12
//#define _NAME_ PORT_Pin_13
//#define _NAME_ PORT_Pin_14
//#define _NAME_ PORT_Pin_15

//PORT C
#define PORT_UART_1 MDR_PORTC
//#define _NAME_ PORT_Pin_0
//#define _NAME_ PORT_Pin_1
//#define _NAME_ PORT_Pin_2
#define pinUART_TX_1 PORT_Pin_3  //Шина дисплея
#define pinUART_RX_1 PORT_Pin_4  //
//#define _NAME_ PORT_Pin_5  //  
//#define _NAME_ PORT_Pin_6  //  
//#define _NAME_ PORT_Pin_7  // 
//#define _NAME_ PORT_Pin_8  //  
//#define _NAME_ PORT_Pin_9  //  
//#define _NAME_ PORT_Pin_10 //
//#define _NAME_ PORT_Pin_11 
//#define _NAME_ PORT_Pin_12
//#define _NAME_ PORT_Pin_13
//#define _NAME_ PORT_Pin_14
//#define _NAME_ PORT_Pin_15

//PORT D
#define ADC_PORT MDR_PORTD
#define S_PORT MDR_PORTD
#define pinS_0 PORT_Pin_0 //Пины управлеия мультиплексором
#define pinS_1 PORT_Pin_1 //Пины управлеия мультиплексором
#define pinS_2 PORT_Pin_2 //Пины управлеия мультиплексором
//#define _NAME_ PORT_Pin_3  //Шина дисплея
//#define _NAME_ PORT_Pin_4  //
//#define _NAME_ PORT_Pin_5  //  
//#define _NAME_ PORT_Pin_6  //  
#define pinADC_0 PORT_Pin_7  // 
#define pinADC_1 PORT_Pin_8  //  
#define pinADC_2 PORT_Pin_9  //  
//#define _NAME_ PORT_Pin_10 //
//#define _NAME_ PORT_Pin_11 
//#define _NAME_ PORT_Pin_12
//#define _NAME_ PORT_Pin_13
#define pinADC_3 PORT_Pin_14
//#define _NAME_ PORT_Pin_15

//PORT E
#define BUZER_PORT MDR_PORTE

#define CATODE_PORT MDR_PORTE

#define CATODE_1 PORT_Pin_0
#define CATODE_2 PORT_Pin_1
#define CATODE_3 PORT_Pin_2
#define CATODE_4 PORT_Pin_3  //
#define CATODE_5 PORT_Pin_4  //
#define CATODE_6 PORT_Pin_5  //  
#define CATODE_7 PORT_Pin_6  //  
#define CATODE_8 PORT_Pin_7  // 
#define CATODE_9 PORT_Pin_8  //  
#define CATODE_10 PORT_Pin_9  //  
#define BUZER_PIN PORT_Pin_10 //
//#define _NAME_ PORT_Pin_11 
//#define _NAME_ PORT_Pin_12
//#define _NAME_ PORT_Pin_13
//#define _NAME_ PORT_Pin_14
//#define _NAME_ PORT_Pin_15

