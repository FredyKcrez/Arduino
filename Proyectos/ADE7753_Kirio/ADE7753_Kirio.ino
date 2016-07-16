// the sensor communicates using SPI, so include the library:
#include <SPI.h>

#define WAVEFORM 0x01
#define AENERGY 0x02
#define RAENERGY 0x03
#define LAENERGY 0x04
#define VAENERGY 0x05
#define LVAENERGY 0x06
#define LVARENERGY 0x07
#define MODE 0x09
#define IRQEN 0x0A
#define STATUS 0x0B
#define RSTSTATUS 0x0C
#define CH1OS 0x0D
#define CH2OS 0x0E
#define GAIN 0x0F
#define PHCAL 0x10
#define APOS 0x11
#define WGAIN 0x12
#define WDIV 0x12
#define CFNUM 0x14
#define CFDEN 0x15
#define IRMS 0x16
#define VRMS 0x17
#define IRMSOS 0x18
#define VRMSOS 0x19
#define VAGAIN 0x1A
#define VADIV 0x1B
#define LINECYC 0x1C
#define ZXTOUT 0x1D
#define SAGCYC 0x1E
#define SAGLVL 0x1F
#define IPKLVL 0x20
#define VPKLVL 0x21
#define IPEAK 0x22
#define RSTIPEAK 0x23
#define VPEAK 0x24
#define RSTVPEAK 0x25
#define TEMP 0x26
#define PERIOD 0x27
#define TMODE 0x3D
#define CHKSUM 0x3E
#define DIEREV 0x3F

#define DATAOUT       51    //MOSI
#define DATAIN        50    //MISO 
#define SPICLOCK      52    //SCK
#define SLAVESELECT   53    //SS

const byte READ = 0x00;     // ADE7753 read command
const byte WRITE = 0x80;    // ADE7753 write command


void setup() {
  Serial.begin(115200);

  // start the SPI library:
  SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE1));

  // Initalize the  data ready and chip select pins:
  pinMode(DATAIN, INPUT);
  pinMode(DATAOUT, OUTPUT);
  pinMode(SPICLOCK, OUTPUT);
  pinMode(SLAVESELECT, OUTPUT);

  //Configure ADE7753 
  //writeRegister(CH1OS, 0x80);
  //writeRegister(GAIN, 0x03);
  //writeRegister(MODE, 0x0008);
  // give the sensor time to set up:
  delay(200);
  Serial.println("Initilization Completed");
}

void loop() {
  writeRegister(CH1OS, 0x80);
  readRegister(MODE, 2);
  //delay(2000);
}

//Read from or write to register from the ADE7753:
unsigned int readRegister(byte thisRegister, int bytesToRead) {
  byte inByte = 0;           // incoming byte from the SPI
  unsigned int result = 0;   // result to return
  Serial.println(thisRegister, HEX);
  byte dataToSend = thisRegister | READ;
  //Serial.println(thisRegister, BIN);
  Serial.println(dataToSend, BIN);
  // take the chip select low to select the device:
  digitalWrite(SLAVESELECT, LOW);
  // send the device the register you want to read:
  SPI.transfer(dataToSend);
  // send a value of 0 to read the first byte returned:
  result = SPI.transfer(0x00);
  // decrement the number of bytes left to read:
  bytesToRead--;
  // if you still have another byte to read:
  if (bytesToRead > 0) {
    // shift the first byte left, then get the second byte:
    result = result << 8;
    inByte = SPI.transfer(0x00);
    // combine the byte you just got with the previous one:
    result = result | inByte;
    // decrement the number of bytes left to read:
    bytesToRead--;
  }
  // take the chip select high to de-select:
  digitalWrite(SLAVESELECT, HIGH);
  return (result);
}


//Sends a write command to ADE7753

void writeRegister(byte thisRegister, byte thisValue) {

  byte dataToSend = thisRegister | WRITE;

  // take the chip select low to select the device:
  digitalWrite(SLAVESELECT, LOW);

  SPI.transfer(dataToSend); //Send register location
  SPI.transfer(thisValue);  //Send value to record into register

  // take the chip select high to de-select:
  digitalWrite(SLAVESELECT, HIGH);
  Serial.print("RegisterAdddress: ");
  Serial.println(dataToSend, BIN);
  Serial.print("Sent Data is: ");
  Serial.println(thisValue, BIN);
  delay(2000);
}

