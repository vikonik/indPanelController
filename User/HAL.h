#include "MDR32F9Qx_uart.h"             // Keil::Drivers:UART
#include "MDR32F9Qx_timer.h"            // Keil::Drivers:TIMER
#include "MDR32F9Qx_adc.h"              // Keil::Drivers:ADC
//#include "MDR32F9Qx_uart.h"             // Keil::Drivers:UART

#include "MDR32F9Qx_rst_clk.h"          // Keil::Drivers:RST_CLK
#include "MDR32F9Qx_port.h"             // Keil::Drivers:PORT

#include "MDR32F9Qx_config.h"           // Keil::Device:Startup




#include "portMaping.h"
#include "RCC.h"
#include "port.h"
#include "delay.h"
#include "MDR32F9Qx_it.h"
#include "ledind.h"
#include "ADC.h"
#include "uart.h"
#include "timer.h"

#define ADC_Canel_0 0
#define ADC_Canel_1 1
#define ADC_Canel_2 2
#define ADC_Canel_3 7
 
#define VOLUME 0x80//Маркер для отображения уровня нагрузки

#define POROG_ON_OFF 4095/2

extern UART *uartPanel;
extern ADC  *adc;
extern DELAY *pause;
extern TIMER *beepTimer;


extern uint32_t tickDelay;
extern uint16_t adcData;
extern uint8_t triggerON_OFF;//Триггер включеиня выключения
extern uint8_t beepEnable;//пищим в прерывании
extern PortMapIO *adcInput_0; 
extern PortMapIO *adcInput_1; 
extern PortMapIO *adcInput_2; 
extern PortMapIO *adcInput_3; 
extern PortMapIO *buzer;


typedef struct {
  uint8_t b0;
  uint8_t b1;
  uint8_t b2;
  uint8_t k1;
  uint8_t k2;
  uint8_t k3;
  uint8_t k4;
}PanelProtocol_t;

extern PanelProtocol_t panelProtocol;
void initUart(void);
uint16_t calckCRC(uint8_t *pcBlock, uint16_t len);
void initADC();
void initSensor();
void selectSensor(uint8_t sensor);
void checkSensorON();//Проверка сенсора включеинч/выключеия
void buzerInit();
void beep();
