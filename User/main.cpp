#include "HAL.h"

//02 00 FF 10 30 30 10 F3 EA  

uint8_t p = 1;
volatile uint16_t CRC = 0;
uint8_t ledCounterTmp = 0;

uint8_t sensorStatus[10];

int main(void){
panelProtocol.b0 = 0x02;
panelProtocol.b1 = 00;
panelProtocol.b2 = 0xFF;
panelProtocol.k1 = 0x10;
panelProtocol.k2 = 0x30;
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
ledTimerInit();

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

//LED_DIGIT ind(&ledCode, digit,7);
ind = new LED_DIGIT(&ledCode, digit,6);
for(int i = 0; i < 10; i++){

//ind.ledCode->setCode(2);
ind->catode[i].catodeOff();
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

if(sensorCounter == 0 | sensorCounter == 1 | sensorCounter == 3 | sensorCounter == 4  ){//Попытка убрать задержку
ledCounterTmp = sensorCounter;
if(readSensor(sensorCounter)){
  if(sensorStatus[sensorCounter] < 31)
    sensorStatus[sensorCounter]++;
}
else sensorStatus[sensorCounter] = 0;

if(sensorStatus[sensorCounter] == 30){
  beep();
heatVolume[sensorCounter]++;
heatVolume[sensorCounter] %= 10;
}

  while(ledCounterTmp == sensorCounter);
}
panelProtocol.k1 = 0x10|heatVolume[0];
panelProtocol.k2 = 0x30|heatVolume[1];
panelProtocol.k3 = 0x30|heatVolume[3];
panelProtocol.k4 = 0x10|heatVolume[4];
if(!(ticPauseSendCMD % 300))setHeat();
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

for(int i = 0; i < 10; i++){
  heatVolume[i] = 0;
}

panelProtocol.k1 = 0x10;
panelProtocol.k2 = 0x30;
panelProtocol.k3 = 0x30;
panelProtocol.k4 = 0x10;
for(int i = 0; i < 20; )
  if(!(ticPauseSendCMD % 500)){i++;setHeat();}

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

