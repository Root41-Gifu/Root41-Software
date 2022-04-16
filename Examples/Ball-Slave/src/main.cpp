#include <Arduino.h>
#include <SPI.h>
#include <def.h>

#define BALL_NUM 16
#define PULSE_TIME 2000

#define BALL_RST PD0

//SPI関連
#define SS_PIN 10
#define SCK_PIN 13
#define MISO_PIN 12
#define MOSI_PIN 11

const int ball[16]={A0,A1,A2,A3,A4,A5,A6,A7,PD2,PD3,PD4,PD5,PD6,PD7,PB0,PB1};
volatile byte receiveData;

volatile unsigned long startTimer[16];
volatile unsigned long pulsewidth[16];
volatile uint8_t pulseByte;
unsigned long pulseTimer[16];
bool pulseFlag[16];
int numCounter[16];
volatile int pulseCounter;
volatile bool NUMflag;

bool getSensorPin(int8_t);

unsigned long timer;

bool getSensorPin(int8_t pin){
  switch(pin){
    case 0: return PINC&_BV(PC0);
    case 1: return PINC&_BV(PC1);
    case 2: return PINC&_BV(PC2);
    case 3: return PINC&_BV(PC3);
    case 4: return PINC&_BV(PC4);
    case 5: return PINC&_BV(PC5);
    case 6: return PIND&_BV(PD1);
    case 7: return PIND&_BV(PD1);
    case 8: return PIND&_BV(PD2);
    case 9: return PIND&_BV(PD3);
    case 10: return PIND&_BV(PD4);
    case 11: return PIND&_BV(PD5);
    case 12: return PIND&_BV(PD6);
    case 13: return PIND&_BV(PD7);
    case 14: return PINB&_BV(PB0);
    case 15: return PINB&_BV(PB1);
  }
}

void setup() {
  SPCR |= bit(SPE);
  pinMode(MISO, OUTPUT);
  pinMode(PD0, OUTPUT);
  pinMode(PD2,INPUT);
  digitalWrite(PD0, LOW);

  pinMode(A0, INPUT);

  ADCSRA = ADCSRA & 0xf8;   // 分周比を決めるビット(ADPS2:0)を000へ
  ADCSRA = ADCSRA | 0x02;
  SPI.attachInterrupt();
}

void loop() {
  // const unsigned long startTime_us = micros();
  // while(getSensorPin(0)==true){
  //   if(getSensorPin(0)==false){
  //     pulsewidth[0]=micros()-startTime_us;
  //     break;
  //   }
  // }
  
  for(int i=0; i<16; i++){
    if(!pulseFlag[i]){
      PORTD |=~_BV(PD0);
      if(getSensorPin(i)==true){
        pulseFlag[i]=true;
        startTimer[i]=micros();
      }
      PORTD |= _BV(PD5);
    }else{
      PORTD |=~_BV(PD0);
      if(getSensorPin(i)==false){
        pulsewidth[i]=micros()-startTimer[i];
        pulseFlag[i]=false;
      }
      if(micros()-startTimer[i]>500){
        pulsewidth[i]=0;
      }
      PORTD |= _BV(PD5);
    }
  }
  // uint8_t IRsensor=B00000000;
  // IRsensor=PINC;
  // if(!pulseFlag[0]){
  //   if(PINC&_BV(PC0)==true){
  //     pulseFlag[0]=true;
  //     startTimer[0]=micros();
  //   }
  // }else{
  //   if(getSensorPin(0)==false){
  //     pulsewidth[0]=micros()-startTimer[0];
  //     pulseFlag[0]=0;
  //   }
  // }
  // for(int i=5; i<10; i++){
  //   if(!pulseFlag[i]){
  //     if(getSensorPin(i)==true){
  //       pulseFlag[i]=true;
  //       startTimer[i]=micros();
  //     }
  //   }else{
  //     if(getSensorPin(i)==false){
  //       pulsewidth[i]=micros()-startTimer[i];
  //       pulseFlag[i]=0;
  //     }
  //   }
  // }
  // for(int i=10; i<16; i++){
  //   if(!pulseFlag[i]){
  //     if(getSensorPin(i)==true){
  //       pulseFlag[i]=true;
  //       startTimer[i]=micros();
  //     }
  //   }else{
  //     if(getSensorPin(i)==false){
  //       pulsewidth[i]=micros()-startTimer[i];
  //       pulseFlag[i]=0;
  //     }
  //   }
  // }
  
  
  //   while(getSensorPin(0)==true){
  //     if(micros()-startTime_us>3000){
  //       pulsewidth[0]=0;
  //     }
  //   }
  // }
  // do {
  //     for (int i = 0; i < 1; i++) {
  //         PORTD |=~_BV(PD0);
  //         if(getSensorPin(i)==false) {
  //             pulsewidth[i] += 1;
  //         }
  //         PORTD |= _BV(PD5);
  //     }
  // } while((micros() - startTime_us) < PULSE_TIME);
  // for(int i=0; i<BALL_NUM; i++){
  //   pulsewidth[i]=0;
  // }
  //6,14,15ポートは読み取れない
}

ISR(SPI_STC_vect) {  //頼むから　うまく動けよ SPI
  receiveData = SPDR;
  if(!NUMflag){
    NUMflag=true;
    SPDR=pulseCounter;
  }else{
    SPDR=pulsewidth[pulseCounter]+16;
    pulseCounter++;
    NUMflag=false;
    if(pulseCounter>15){
      pulseCounter=0;
    }
  }
}

