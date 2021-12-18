#include "HAL.h"
UART *uartPanel;
ADC *adc;
PanelProtocol_t panelProtocol;
PortMapIO *adcInput_0; 
PortMapIO *adcInput_1; 
PortMapIO *adcInput_2; 
PortMapIO *adcInput_3; 

/*Пины для переключения сенсоров*/
PortMapIO *s_0; 
PortMapIO *s_1; 
PortMapIO *s_2; 
/**/
void initUart(void){
  PortMapIO rx(PORT_UART_1,
    pinUART_RX_1,
    PORT_OE_IN,
    PORT_PULL_UP_OFF,
    PORT_PULL_DOWN_OFF,
    PORT_PD_SHM_OFF,
    PORT_PD_DRIVER,
    PORT_GFEN_OFF,
    PORT_FUNC_MAIN,
    PORT_SPEED_MAXFAST,
    PORT_MODE_DIGITAL);

  PortMapIO tx(PORT_UART_1,
    pinUART_TX_1,
    PORT_OE_OUT,
    PORT_PULL_UP_OFF,
    PORT_PULL_DOWN_OFF,
    PORT_PD_SHM_OFF,
    PORT_PD_DRIVER,
    PORT_GFEN_OFF,
    PORT_FUNC_MAIN,
    PORT_SPEED_MAXFAST,
    PORT_MODE_DIGITAL);
//RST_CLK_PCLKcmd(RST_CLK_PCLK_UART1, ENABLE);
   uartPanel = new UART(MDR_UART1, 115200, UART_WordLength8b, UART_StopBits1, UART_Parity_No,UART_FIFO_OFF, UART_HardwareFlowControl_RXE | UART_HardwareFlowControl_TXE);
  uartPanel->enableIRQ();
}

/**/
/*
  Name  : CRC-16 CCITT
  Poly  : 0x1021    x^16 + x^12 + x^5 + 1
  Init  : 0xFFFF
  Revert: false
  XorOut: 0x0000
  Check : 0x29B1 ("123456789")
  MaxLen: 4095 байт (32767 бит) - обнаружение
    одинарных, двойных, тройных и всех нечетных ошибок
*/
uint16_t calckCRC(uint8_t *pcBlock, uint16_t len)
{
    uint16_t crc = 0x0000;
    uint8_t i;

    while (len--)
    {
        crc ^= *pcBlock++ << 8;

        for (i = 0; i < 8; i++)
            crc = crc & 0x8000 ? (crc << 1) ^ 0x1021 : crc << 1;
    }
    return crc;
}

/**/
void initADC(){
//7 8 9 14
//PortMapIO adcIINPUT()
adcInput_0 = new PortMapIO(ADC_PORT, pinADC_0); 
adcInput_1 = new PortMapIO(ADC_PORT, pinADC_1); 
adcInput_2 = new PortMapIO(ADC_PORT, pinADC_2); 
adcInput_3 = new PortMapIO(ADC_PORT, pinADC_3); 
adcInput_0->setPinAsAnalog();
adcInput_1->setPinAsAnalog();
adcInput_2->setPinAsAnalog();
adcInput_3->setPinAsAnalog();

adc = new ADC();
adc->init();
adc->setChannel(ADC_Canel_0);
adc->IRQ_ON();
adc->Start();
}

/**/
void initSensor(){
  s_0 = new PortMapIO(S_PORT, pinS_0); 
  s_1 = new PortMapIO(S_PORT, pinS_1); 
  s_2 = new PortMapIO(S_PORT, pinS_2); 
  s_0->setLow(); 
  s_1->setLow();  
  s_2->setLow(); 

}
/*
Выбор сенсота для чтеия
*/
void selectSensir(uint8_t sensor){
  switch(sensor){
    case 0:s_2->setLow(); s_1->setLow();s_0->setLow(); 
    break;
    case 1:s_2->setLow(); s_1->setLow();s_0->setHigh(); 
    break;
    case 2:s_2->setLow(); s_1->setHigh();s_0->setLow(); 
    break;
    case 3:s_2->setLow(); s_1->setHigh();s_0->setHigh(); 
    break;
    case 4:s_2->setHigh(); s_1->setLow();s_0->setLow(); 
    break;
    case 5:s_2->setHigh(); s_1->setLow();s_0->setHigh(); 
    break;
    case 6:s_2->setHigh(); s_1->setHigh();s_0->setLow(); 
    break;
    case 7:s_2->setHigh(); s_1->setHigh();s_0->setHigh(); 
    break;
    default : break;
  }
}
