/*
 * Lista de archivos de los registros del chip ADE7553 y funciones con los que trabaja
 * sacado de la librería del ADE7753 que se encuentra en la siguiente dirección:
 * https://code.google.com/archive/p/ardugrid7753/source/default/source
 * Para descargar el archivo, pegar en la barra de direcciones el siguiente link:
 * https://storage.googleapis.com/google-code-archive-source/v2/code.google.com/ardugrid7753/source-archive.zip
 * Una vez descargado, se descomprime el archivo .zip y los archivos de la clase ADE7753 estan
 * en la ruta ardugrid7753/trunk
 */

//registers on ADE7753
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
  
// Pines del SPI
#define DATAOUT MOSI
#define DATAIN MISO
#define SPICLOCK SCK
//#define SLAVESELECT 8

// registro MODE - localización del bit
#define DISHPF   0x0001 // bit 0 - HPF (high-pass filter) in Channel 1 is disabled when this bit is set.
#define DISLPF2  0x0002 // bit 1 - LPF (low-pass filter) after the multiplier (LPF2) is disabled when this bit is set.
#define DISCF    0x0004 // bit 2 - Frequency output CF is disabled when this bit is set.
#define DISSAG   0x0008 // bit 3 - Line voltage sag detection is disabled when this bit is set.
#define ASUSPEND 0x0010 // bit 4 - By setting this bit to Logic 1, both ADE7753 A/D converters can be turned off. In normal operation, this bit should be left at Logic 0. All digital functionality can be stopped by suspending the clock signal at CLKIN pin.
#define TEMPSEL  0x0020 // bit 5 - Temperature conversion starts when this bit is set to 1. This bit is automatically reset to 0 when the temperature conversion is finished.
#define SWRST    0x0040 // bit 6 - Software Chip Reset. A data transfer should not take place to the ADE7753 for at least 18 us after a software reset.
#define CYCMODE  0x0080 // bit 7 - Setting this bit to Logic 1 places the chip into line cycle energy accumulation mode.
#define DISCH1   0x0100 // bit 8 - ADC 1 (Channel 1) inputs are internally shorted together.
#define DISCH2   0x0200 // bit 9 - ADC 2 (Channel 2) inputs are internally shorted together.
#define SWAP     0x0400 // bit 10 - By setting this bit to Logic 1 the analog inputs V2P and V2N are connected to ADC 1 and the analog inputs V1P and V1N are connected to ADC 2.
#define DTRT1    0x0800 // bit 11 - These bits are used to select the waveform register update rate.
#define DTRT0    0x1000 // bit 12 - These bits are used to select the waveform register update rate.
#define WAVSEL1  0x2000 // bit 13 - These bits are used to select the source of the sampled data for the waveform register.
#define WAVSEL0  0x4000 // bit 14 - These bits are used to select the source of the sampled data for the waveform register.
#define POAM     0x8000 // bit 15 - Writing Logic 1 to this bit allows only positive active power to be accumulated in the ADE7753.

/** A continuación se listan la función de cada bit para el Interrupt Status Register, el Reset Interrupt Status Register, 
  * y el Interrupt Enable Register. 
  * Se listan en el orden siguiente:
  *            Mascara BIT / Localización Bit / Descripción
  */
#define AEHF      0x0001      // bit 0 - Indicates that an interrupt occurred because the active energy register, AENERGY, is more than half full.
#define SAG       0x0002      // bit 1 - Indicates that an interrupt was caused by a SAG on the line voltage.
#define CYCEND    0x0004      // bit 2 - Indicates the end of energy accumulation over an integer number of half line cycles as defined by the content of the LINECYC register�see the Line Cycle Energy Accumulation Mode section.
#define WSMP      0x0008      // bit 3 - Indicates that new data is present in the waveform register.
#define ZX        0x0010      // bit 4 - This status bit is set to Logic 0 on the rising and falling edge of the the voltage waveform. See the Zero-Crossing Detection section.
#define TEMPREADY 0x0020      // bit 5 - Indicates that a temperature conversion result is available in the temperature register.
#define RESET     0x0040      // bit 6 - Indicates the end of a reset (for both software or hardware reset). The corresponding enable bit has no function in the interrupt enable register, i.e., this status bit is set at the end of a reset, but it cannot be enabled to cause an interrupt.
#define AEOF      0x0080      // bit 7 - Indicates that the active energy register has overflowed.
#define PKV       0x0100      // bit 8 - Indicates that waveform sample from Channel 2 has exceeded the VPKLVL value.
#define PKI       0x0200      // bit 9 - Indicates that waveform sample from Channel 1 has exceeded the IPKLVL value.
#define VAEHF     0x0400      // bit 10 - Indicates that an interrupt occurred because the active energy register, VAENERGY, is more than half full.
#define VAEOF     0x0800      // bit 11 - Indicates that the apparent energy register has overflowed.
#define ZXTO      0x1000      // bit 12 - Indicates that an interrupt was caused by a missing zero crossing on the line voltage for the specified number of line cycles�see the Zero-Crossing Timeout section.
#define PPOS      0x2000      // bit 13 - Indicates that the power has gone from negative to positive.
#define PNEG      0x4000      // bit 14 - Indicates that the power has gone from positive to negative.
#define RESERVED  0x8000      // bit 15 - Reserved.

//Constantes
#define GAIN_1    0x0
#define GAIN_2    0x1
#define GAIN_4    0x2
#define GAIN_8    0x3
#define GAIN_16   0x4
#define INTEGRATOR_ON  1
#define INTEGRATOR_OFF 0
#define FULLSCALESELECT_0_5V    0x00
#define FULLSCALESELECT_0_25V   0x01
#define FULLSCALESELECT_0_125V  0x02

//Atributos de clase
#define READ  0x00           // WRITE bit BT7 to write to registers
#define WRITE 0x80           // WRITE bit BT7 to write to registers
#define CLKIN 4000000        // ADE7753 frec, 4.000000MHz

#define csMedidor 5

/*
 * A continuación se listan una serie de funciones útiles con el chip ADE7753
 */
void activarADE7753() {
  // Activamos el CS del medidor
  digitalWrite(csMedidor, LOW);
}

void desactivarADE7753() {
  // Desactivamos el CS del medidor
  digitalWrite(csMedidor, HIGH);
}

/* 
 * Función utilizada para leer 8 bits de un registro especifico (reg) del ADE7753
 * reg contiene la dirección del registro 
 */
unsigned char read8(char reg) {
  activarADE7753();
  unsigned char b0;
  reg = reg | READ;
  delayMicroseconds(50);
  SPI.transfer(reg);
  delayMicroseconds(50);
  b0 = SPI.transfer(0x00);
  delayMicroseconds(50);
  desactivarADE7753();
  // retornamos el contenido del registro
  return b0;
}

/* 
 * Lectura de 16 bits; 2 de 8 bits, por eso el uso de 2 unsigned char
 * reg contiene la dirección del registro 
 */
unsigned int read16(char reg) {
  activarADE7753();
  unsigned char b1,b0;
  reg = reg | READ;
  delayMicroseconds(50);
  SPI.transfer(reg);
  delayMicroseconds(50);
  b1 = SPI.transfer(0x00); // primeros 8 bits
  delayMicroseconds(50);
  b0 = SPI.transfer(0x00); // últimos 8 bits
  delayMicroseconds(50);
  desactivarADE7753();
  // retornamos dos registros de 8 bits (16 bits)
  return (unsigned int)b1<<8 | (unsigned int)b0;
}

/* 
 * Lectura de 24 bits; 3 de 8 bits, por eso el uso de 3 unsigned char
 * reg contiene la dirección del registro 
 */
unsigned long read24(char reg) {
  activarADE7753();
  unsigned char b2,b1,b0;
  reg = reg | READ;
  delayMicroseconds(50);
  SPI.transfer(reg);
  delayMicroseconds(50);
  b2 = SPI.transfer(0x00); // primeros 8 bits
  delayMicroseconds(50);
  b1 = SPI.transfer(0x00); // siguientes 8 bits
  delayMicroseconds(50);
  b0 = SPI.transfer(0x00); // últimos 8 bits
  desactivarADE7753();
  // retornamos tres registros de 8 bits (24 bits)
  return (unsigned long)b2<<16 | (unsigned long)b1<<8 | (unsigned long)b0;
}

/* 
 * Escribimos 8 bits (data) en el ADE7753 en el registro epecificado con reg
 * reg contiene la dirección del registro 
 */
void write8(char reg, unsigned char data) {
  activarADE7753();
  unsigned char data0 = 0;
  reg |= WRITE;
  data0 = (unsigned char) data;
  delayMicroseconds(50);
  SPI.transfer((unsigned char) reg); //register selection
  delayMicroseconds(50);
  SPI.transfer((unsigned char) data0);
  delayMicroseconds(50);
  desactivarADE7753();
}

/* 
 * Escribimos 16 bits (data) en el ADE7753 en el registro epecificado con reg
 * reg contiene la dirección del registro 
 */
void write16(char reg, unsigned int data) {
  activarADE7753();
  unsigned char data0=0, data1=0;
  reg |= WRITE;
  data0 = (unsigned char)data;
  data1 = (unsigned char)(data>>8);
  delayMicroseconds(50);
  SPI.transfer((unsigned char)reg);    
  delayMicroseconds(50);    
  //envio de dato, MSB primero según la configuración del SPI
  SPI.transfer((unsigned char)data1);
  delayMicroseconds(50);
  SPI.transfer((unsigned char)data0);  
  delayMicroseconds(50);
  desactivarADE7753();
}

/*void setMode(int m) {
  write16(MODE, m);
}

int getMode() {
  return read16(MODE);
}*/

int getEnabledInterrupts() {
  return read16(IRQEN);
}

int getInterruptStatus() {
  return read16(STATUS);
}

int getresetInterruptStatus() {
  return read16(RSTSTATUS);
}

long getActiveEnergyLineSync() {
  return read24(LAENERGY);
}

long getApparentEnergyLineSync() {
  return read24(LVAENERGY);
}

long getReactiveEnergyLineSync() {
  return read24(LVARENERGY);
}

long getIRMS() {
  long lastupdate = 0;
  getresetInterruptStatus(); // Clear all interrupts
  lastupdate = millis();
  while(!( getInterruptStatus() & ZX )) {
    // wait Zero-Crossing
    // wait for the selected interrupt to occur
    if ( ( millis() - lastupdate ) > 100) { 
      break;  
    }
  }    
  return read24(IRMS);
}

long getVRMS(){
  long lastupdate = 0;
  getresetInterruptStatus(); // Clear all interrupts
  lastupdate = millis();
  while(!( getInterruptStatus() & ZX )) {
    // wait Zero-Crossing
    // wait for the selected interrupt to occur
    if ( ( millis() - lastupdate ) > 100) {
      break;
    }
  }          
  return read24(VRMS);
}

long vrms() {
  char i=0;
  long v=0;
  getVRMS(); //Ignore first reading to avoid garbage
  for(i=0; i<100; ++i){
    v += getVRMS();
  }
  return v/100;
}

long irms(){
  char n=0;
  long i=0;
  getIRMS(); //Ignore first reading to avoid garbage
  for(n=0; n<100; ++n){
    i += getIRMS();
  }
  return i/100;
}

/*char chkSum() {
  return read8(CHKSUM);
}*/

long getActiveEnergy() {
  return read24(AENERGY);
}

long getActiveEnergyReset() {
  return read24(RAENERGY);
}

long getApparentEnergyReset() {
  return read24(RVAENERGY);
}

void setInterruptsMask(int Mask16){
  write16(IRQEN, Mask16);
}
