#include <Arduino.h>
#include "SPI.h"

//Register address
#define WAVEFORM   0x01
#define AENERGY    0x02
#define RAENERGY   0x03
#define LAENERGY   0x04
#define VAENERGY   0x05
#define RVAENERGY  0x06
#define LVAENERGY  0x07
#define LVARENERGY 0x08
#define MODE       0x09
#define IRQEN      0x0A   // interrupts enable register
#define STATUS     0x0B   // interrupts status register
#define RSTSTATUS  0x0C   // interrupts status register but read will reset all interrupt flags
#define CH1OS      0x0D
#define CH2OS      0x0E
#define GAIN       0x0F
#define PHCAL      0x10
#define APOS       0x11
#define WGAIN      0x12
#define WDIV       0x13
#define CFNUM      0x14
#define CFDEN      0x15
#define IRMS       0x16
#define VRMS       0x17
#define IRMSOS     0x18
#define VRMSOS     0x19
#define VAGAIN     0x1A
#define VADIV      0x1B
#define LINECYC    0x1C
#define ZXTOUT     0x1D
#define SAGCYC     0x1E
#define SAGLVL     0x1F
#define IPKLVL     0x20
#define VPKLVL     0x21
#define IPEAK      0x22
#define RSTIPEAK   0x23
#define VPEAK      0x24
#define RSTVPEAK   0x25
#define TEMP       0x26
#define PERIOD     0x27
#define TMODE      0x3D
#define CHKSUM     0x3E
#define DIEREV     0X3F

//#define DATAOUT 11//MOSI
//#define DATAIN  12//MISO 
//#define SPICLOCK  13//sck
//#define SLAVESELECT 10//ss

// Class Atributes
//#define RESET 5
#define CS 5                // Chip Select ADE7753 - Digital output pin nbr on Olimex Energy Shield  
#define WRITE 0x80           // WRITE bit BT7 to write to registers
#define CLKIN 4000000        // ADE7753 frec, 4.000000MHz

void setup()
{
  Serial.begin(9600);
//  digitalWrite(RESET,LOW);
//  digitalWrite(RESET,HIGH);  RESET pin hardwired to 5V on arduino uno
  //pinMode(DATAOUT, OUTPUT);
  //pinMode(DATAIN, INPUT);
  //pinMode(SPICLOCK,OUTPUT);
  pinMode(CS,OUTPUT);
  digitalWrite(CS,HIGH); //Setting to High means disable it
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE2);
  SPI.setClockDivider(SPI_CLOCK_DIV8);
  SPI.begin();
  
  //Serial.println(read8(GAIN),BIN);
  //write8(GAIN,01100001);
  //Serial.println(read8(GAIN),BIN);
  //digitalWrite(SLAVESELECT,HIGH);
  SPI.end();
}

unsigned char write8(char reg, unsigned char data){
  
        digitalWrite(CS,LOW);
  unsigned char data0 = 0;

  // 8th bit (DB7) of the register address controls the Read/Write mode (Refer to spec page 55 table 13)
  // For Write -> DB7 = 1  / For Read -> DB7 = 0
  reg |= WRITE;
  data0 = (unsigned char)data;
  delayMicroseconds(50);
  SPI.transfer((unsigned char)reg);          //register selection
  delayMicroseconds(50);
  SPI.transfer((unsigned char)data0);
  delayMicroseconds(50);
        digitalWrite(CS,HIGH);
}

unsigned char read8(char reg){
  // enableChip();

        digitalWrite(CS,LOW);

        
        float b0;
        delayMicroseconds(50);
  SPI.transfer(reg);
  delayMicroseconds(50);
  b0=SPI.transfer(0x00);
        Serial.print("b0 value: ");
        Serial.println(b0);
  delayMicroseconds(50);
        delay(10);
        digitalWrite(CS,HIGH);
  // disableChip();
  //    return (unsigned long)SPI.transfer(0x00);

  return b0;
}



void loop(){
  
        //pinMode(CS,OUTPUT);  // Chip select by digital output on pin nbs CS
        //digitalWrite(CS,LOW);//is disabled by default, so need to set
        // SPI Init
        SPI.setDataMode(SPI_MODE2);
        SPI.setClockDivider(SPI_CLOCK_DIV8);
        SPI.setBitOrder(MSBFIRST);
        SPI.begin();
        
        delay(1000);
        Serial.print("Read register value: ");
        Serial.println(read8(GAIN),HEX); //Default value of GAIN = 0x0
        //Serial.println(read8(SAGCYC),HEX); //Default value of GAIN = 0x0
        //Serial.println(read8(IPKLVL),HEX); //Default value of GAIN = 0x0
        //write8(0x0F,0x00);
        //Serial.print("Read register value again after writing: ");
        //Serial.println(read8(GAIN),BIN);
        Serial.println("  ");
        delay(1000);
         
        //write8(GAIN,01100001);
        //Serial.println(read8(GAIN),BIN);
        //digitalWrite(SLAVESELECT,HIGH);
        
        SPI.end();
        delay(10);
       // digitalWrite(CS,HIGH);

  
}


/*
void setREG16(int adress, int value){
        digitalWrite(CS,LOW);
        adress |= WRITE;
        delayMicroseconds(5);
        SPI.transfer(adress);
        delayMicroseconds(5);
        SPI.transfer(value>>8);
        delayMicroseconds(5);
        SPI.transfer(value);
        digitalWrite(CS,HIGH);
}

long readREG16(int adress){
    
        digitalWrite(CS,LOW);
         long reg, a;
        delayMicroseconds(5);
        SPI.transfer(adress);
        delayMicroseconds(5);
        reg = SPI.transfer(0x00);
        a = SPI.transfer(0x00);
        delayMicroseconds(5);
        digitalWrite(CS,HIGH);
        reg  = reg <<  8;
        reg = reg + a;
        
        return reg;
  
}
*/
