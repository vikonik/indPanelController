#include "HAL.h"



uint8_t p = 1;
volatile uint16_t CRC = 0;
volatile uint16_t tmpAdc = 0;

int main(void){
panelProtocol.b0 = 0x02;
panelProtocol.b1 = 00;
panelProtocol.b2 = 0xFF;
panelProtocol.k1 = 0x10;
panelProtocol.k2 = 0x31;
panelProtocol.k3 = 0x30;
panelProtocol.k4 = 0x10;

CRC = calckCRC((uint8_t*)&panelProtocol, sizeof(PanelProtocol_t));
RCC rcc(128);//Кварц 8МГц, поэтому максимально 128
pause = new DELAY;
initUart();
initADC();
buzerInit();
adc->setChannel(ADC_Canel_0);
initSensor();
selectSensor(0);

uartPanel->sendByte(0x55);
uartPanel->sendByte(0x56);
uartPanel->sendByte(0x57);




PortMapIO sA(LED_DIGIT_PORT, SEG_A);
PortMapIO sB(LED_DIGIT_PORT, SEG_B);
PortMapIO sC(LED_DIGIT_PORT, SEG_C);
PortMapIO sD(LED_DIGIT_PORT, SEG_D);
PortMapIO sE(LED_DIGIT_PORT, SEG_E);
PortMapIO sF(LED_DIGIT_PORT, SEG_F);
PortMapIO sG(LED_DIGIT_PORT, SEG_G);
PortMapIO sH(LED_DIGIT_PORT, SEG_H);
PortMapIO sI(LED_DIGIT_PORT, SEG_I);
PortMapIO sJ(LED_DIGIT_PORT, SEG_J);

PortMapIO C_1(CATODE_PORT, CATODE_1);
PortMapIO C_2(CATODE_PORT, CATODE_2);
PortMapIO C_3(CATODE_PORT, CATODE_3);
PortMapIO C_4(CATODE_PORT, CATODE_4);
PortMapIO C_5(CATODE_PORT, CATODE_5);
PortMapIO C_6(CATODE_PORT, CATODE_6);
PortMapIO C_7(CATODE_PORT, CATODE_7);
PortMapIO C_8(CATODE_PORT, CATODE_8);
PortMapIO C_9(CATODE_PORT, CATODE_9);
PortMapIO C_10(CATODE_PORT, CATODE_10);
LED_CODE ledCode(&sA, &sB, &sC, &sD, &sE, &sF, &sG, &sH, &sI, &sJ);

LED_CATODE digit[10];
digit[0].addCatode(&C_1);
digit[1].addCatode(&C_2);
digit[2].addCatode(&C_3);
digit[3].addCatode(&C_4);
digit[4].addCatode(&C_5);
digit[5].addCatode(&C_6);
digit[6].addCatode(&C_7);
digit[7].addCatode(&C_8);
digit[8].addCatode(&C_9);
digit[9].addCatode(&C_10);

LED_DIGIT ind(&ledCode, digit,7);

for(int i = 0; i < 10; i++){

//ind.ledCode->setCode(2);
(&ind.catode[i])->catodeOff();
}


digit[8].setHi();

digit[0].setLow();
digit[1].setLow();
digit[2].setLow();
digit[3].setLow();
digit[4].setLow();
digit[5].setLow();





beep();
triggerON_OFF = 0;
while(1){
checkSensorON();
if(triggerON_OFF){
ind.showDigit(0,(0<<5)|0);
pause->delay_ms(p);
ind.showDigit(1,(0<<5)|5);
pause->delay_ms(p);
ind.showDigit(2,(0<<5)|2);




selectSensor(0);
adc->setChannel(ADC_Canel_0);
adc->Start();
  while(ADC1_GetFlagStatus(ADCx_IT_END_OF_CONVERSION) != SET){}
  tmpAdc = adc->readData();
  adc->Start();
  while(ADC1_GetFlagStatus(ADCx_IT_END_OF_CONVERSION) != SET){}
  tmpAdc = adc->readData();

if(tmpAdc > 4095/3)
beep();



pause->delay_ms(p);
ind.showDigit(3,(0<<5)|3);
pause->delay_ms(p);
ind.showDigit(4,(0<<5)|4);
pause->delay_ms(p);
ind.showDigit(5,5);
pause->delay_ms(p);
ind.showDigit(6,VOLUME|0x06);
pause->delay_ms(p);




//ind.showDigit(0,6);
//ind.showDigit(0,7);
//ind.showDigit(0,8);
//ind.showDigit(0,9);
}
else{
digit[0].setHi();
digit[1].setHi();
digit[2].setHi();
digit[3].setHi();
digit[4].setHi();
digit[5].setHi();
digit[6].setHi();



while(!triggerON_OFF){
  checkSensorON();
//pause->delay_ms(250);
}
}

}




//buzer.setLow();
//C_1.setHigh();
//C_2.setHigh();
//C_3.setHigh();
//C_4.setHigh();
//C_5.setHigh();
//C_6.setHigh();
//C_7.setHigh();

while(1){
//buzer.setHigh();
//pause.delay_ms(10);
//buzer.setLow();
//pause.delay_ms(10);
//sA.setHigh();
//sB.setHigh();
//sC.setHigh();
//sD.setHigh();
//sE.setHigh();
//sF.setHigh();
//sG.setHigh();
//sH.setHigh();
//sI.setHigh();
//sJ.setHigh();


//C_1.setLow();
//pause.delay_ms(200);
//C_1.setHigh();
//pause.delay_ms(200);

//C_2.setLow();
//pause.delay_ms(200);
//C_2.setHigh();
//pause.delay_ms(200);

//C_3.setLow();
//pause.delay_ms(200);
//C_3.setHigh();
//pause.delay_ms(200);




}
}

