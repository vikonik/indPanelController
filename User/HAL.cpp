#include "HAL.h"
UART *uartPanel;
ADC *adc;
DELAY *pause;
TIMER *beepTimer;
TIMER *ledTimer;
TIMER *delayMessure;
LED_DIGIT *ind;

PanelProtocol_t panelProtocol;
PortMapIO *adcInput_0; 
PortMapIO *adcInput_1; 
PortMapIO *adcInput_2; 
PortMapIO *adcInput_3; 

/*Пины для переключения сенсоров*/
PortMapIO *s_0; 
PortMapIO *s_1; 
PortMapIO *s_2; 

//Пищалка
PortMapIO *buzer;
uint16_t adcData = 0;
uint8_t triggerON_OFF = 0;//Триггер включеиня выключения
uint8_t triggerSensorLoc = 0;
uint8_t beepEnable = 0;

uint8_t triggerMessureTimer = 0;//для измерения отступа для начала измерения сенсора
uint8_t ledCounter = 0;//Счетчик для переключеиня индикаторов и сенсоров
uint8_t sensorCounter = 0;
uint8_t heatVolume[10];//Мощность нагрева
uint16_t ticPauseSendCMD = 0;//Команды в силовую часть надо слать с интервалом 500мс

uint16_t calibrationData[9];//Калибровка сенсоров для определения порога срабатывания
uint16_t calibrationOnOff = 0;


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
   uartPanel = new UART(MDR_UART1, 9600, UART_WordLength8b, UART_StopBits1, UART_Parity_No,UART_FIFO_OFF, UART_HardwareFlowControl_RXE | UART_HardwareFlowControl_TXE);
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
adc->switchToSinglConversion();
adc->setChannel(ADC_Canel_0);
//adc->IRQ_ON();
//adc->Start();
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
void selectSensor(uint8_t sensor){
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

/*
Сенсор включения
реагирует на свет, будем калхозить...
*/
void checkSensorON(){
uint16_t array[8];
uint16_t tmp = 0;
adc->Stop();
adc->setChannel(ADC_Canel_3);

for(int i = 0; i < 5; i++){
adc->Start();
  while(ADC1_GetFlagStatus(ADCx_IT_END_OF_CONVERSION) != SET){}
  adc->readData();
}

//Заполняем массив для фильтра
for(int i = 0; i < 8; i++){
  adc->Start();
  while(ADC1_GetFlagStatus(ADCx_IT_END_OF_CONVERSION) != SET){}
  array[i] = adc->readData();
}



for(int i = 0; i < 8; i++){

  if(array[i] > POROG_ON_OFF)tmp = array[i];
  else{
    tmp = 0;
    break;
  }
}

if(tmp > (POROG_ON_OFF)){

if(triggerON_OFF) 
  triggerON_OFF = 0;
else 
  triggerON_OFF = 1;

beep();
while(tmp > (POROG_ON_OFF)){
  adc->Start();
  while(ADC1_GetFlagStatus(ADCx_IT_END_OF_CONVERSION) != SET){}
  tmp = adc->readData();
}
pause->delay_ms(250);

}
}

/*
Выключение индикаторов
*/
void indicatorOFF(){

}

/**/
void buzerInit(){
buzer = new PortMapIO (BUZER_PORT,BUZER_PIN);
  beepTimer = new TIMER(MDR_TIMER1, 500);
}
/**/
void beep(){
//  for(int i = 0; i < 50; i++){
//    buzer->setHigh();
//    pause->delay_ms(1);
//    buzer->setLow();
//    pause->delay_ms(1);
//  }

beepEnable = 1;
}

/*
Детектор сенсора для канфорки
*/
volatile uint16_t tmpAdc = 0;
uint8_t readSensor(uint8_t sensorNumber){
uint16_t porog = 0;

//selectSensor(7);
//adc->setChannel(ADC_Canel_0);
//adc->Start();
//  while(ADC1_GetFlagStatus(ADCx_IT_END_OF_CONVERSION) != SET){}
//  porog = adc->readData();
//  adc->Start();
//  while(ADC1_GetFlagStatus(ADCx_IT_END_OF_CONVERSION) != SET){}
//  porog = adc->readData();

selectSensor(sensorNumber);
adc->setChannel(ADC_Canel_0);
//delayMessure->timerStop();
//triggerMessureTimer = 0;
//delayMessure->timerStart();
//while(triggerMessureTimer == 0){}

for(int i = 0; i < 10; i++){
  adc->Start();
  while(ADC1_GetFlagStatus(ADCx_IT_END_OF_CONVERSION) != SET){}
  tmpAdc = adc->readData();
}

  adc->Start();
  while(ADC1_GetFlagStatus(ADCx_IT_END_OF_CONVERSION) != SET){}
  tmpAdc = adc->readData();

//if(sensorNumber == 1)
//sensorNumber = 1;

//  if(tmpAdc > (4096-porog)){
//    beep();
//    adc->Start();
//    while(ADC1_GetFlagStatus(ADCx_IT_END_OF_CONVERSION) != SET){}
//      tmpAdc = adc->readData();
//    while(tmpAdc > (4096-porog)){
//      adc->Start();
//      while(ADC1_GetFlagStatus(ADCx_IT_END_OF_CONVERSION) != SET){}
//      tmpAdc = adc->readData();
//    }
//  }

porog = (calibrationData[sensorNumber] + (calibrationData[sensorNumber] / 4));
  if(tmpAdc > porog){//(4096-porog)
    return 1;
  }

return 0;
}

/*
Инициализация таймера для переключения индикаторов
*/
void ledTimerInit(){
ledTimer = new  TIMER(MDR_TIMER2, 1000);
}
/**/
void delayMessureTimerInit(){
delayMessure = new  TIMER(MDR_TIMER3, 100);
}
/**/
void setHeat(void){

uint16_t crc = calckCRC((uint8_t*)&panelProtocol, sizeof(PanelProtocol_t));
uartPanel->sendByte(panelProtocol.b0);
uartPanel->sendByte(panelProtocol.b1);
uartPanel->sendByte(panelProtocol.b2);
uartPanel->sendByte(panelProtocol.k1);
uartPanel->sendByte(panelProtocol.k2);
uartPanel->sendByte(panelProtocol.k3);
uartPanel->sendByte(panelProtocol.k4);
uartPanel->sendByte((crc >> 8));
uartPanel->sendByte((crc & 0xFF));

}

/*

*/
uint8_t checkSensorLock(void){
uint16_t porog = 0;

selectSensor(7);
adc->setChannel(ADC_Canel_0);
adc->Start();
  while(ADC1_GetFlagStatus(ADCx_IT_END_OF_CONVERSION) != SET){}
  porog = adc->readData();
  adc->Start();
  while(ADC1_GetFlagStatus(ADCx_IT_END_OF_CONVERSION) != SET){}
  porog = adc->readData();

selectSensor(2);
adc->setChannel(ADC_Canel_1);
adc->Start();
  while(ADC1_GetFlagStatus(ADCx_IT_END_OF_CONVERSION) != SET){}
  tmpAdc = adc->readData();
  adc->Start();
  while(ADC1_GetFlagStatus(ADCx_IT_END_OF_CONVERSION) != SET){}
  tmpAdc = adc->readData();


//if(sensorNumber == 1)
//sensorNumber = 1;

//  if(tmpAdc > (4096-porog)){
//    beep();
//    adc->Start();
//    while(ADC1_GetFlagStatus(ADCx_IT_END_OF_CONVERSION) != SET){}
//      tmpAdc = adc->readData();
//    while(tmpAdc > (4096-porog)){
//      adc->Start();
//      while(ADC1_GetFlagStatus(ADCx_IT_END_OF_CONVERSION) != SET){}
//      tmpAdc = adc->readData();
//    }
//  }

  if(tmpAdc > (4096-porog)){
    return 1;
  }
return 0;
}

/*
чтение калибровочных данных
*/
uint16_t readCalibration(uint8_t sensorNumber){
uint16_t porog = 0;
selectSensor(sensorNumber);//Установили канал мультиплексора
adc->setChannel(ADC_Canel_0);
pause->delay_ms(1);
  
  adc->Start();
  while(ADC1_GetFlagStatus(ADCx_IT_END_OF_CONVERSION) != SET){}
  porog = adc->readData();
  adc->Start();
  while(ADC1_GetFlagStatus(ADCx_IT_END_OF_CONVERSION) != SET){}
  porog += adc->readData();
porog /= 2;
return  porog;
}

/*
Калибровка сенсоров
*/
void sensorCalib(void){
for(int i = 0; i< 9; i++){
calibrationData[i] = 0;
}

adc->setChannel(ADC_Canel_3);
pause->delay_ms(1);
  adc->Start();
  while(ADC1_GetFlagStatus(ADCx_IT_END_OF_CONVERSION) != SET){}
  calibrationOnOff = adc->readData();
  adc->Start();
  while(ADC1_GetFlagStatus(ADCx_IT_END_OF_CONVERSION) != SET){}
  calibrationOnOff = adc->readData();

ind->showDigit(2,(0<<5)|heatVolume[0]); sensorCounter = 0;
calibrationData[sensorCounter]  = readCalibration( sensorCounter);

ind->showDigit(3,(0<<5)|heatVolume[1]); sensorCounter = 1;
calibrationData[sensorCounter]  = readCalibration( sensorCounter);

ind->showDigit(4,0|heatVolume[3]); sensorCounter = 3;
calibrationData[sensorCounter]  = readCalibration( sensorCounter);

ind->showDigit(5,heatVolume[4]); sensorCounter = 4;
calibrationData[sensorCounter]  = readCalibration( sensorCounter);

}

