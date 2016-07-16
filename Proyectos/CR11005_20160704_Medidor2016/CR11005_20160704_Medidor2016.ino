/*
 * Referencias:
 * https://github.com/FriedCircuits/ADE7753/blob/master/ADE7753/ADE7753.ino
 * http://www.avrfreaks.net/forum/problem-reading-ade7753-registers
 * http://arduino.stackexchange.com/questions/16348/how-do-you-use-spi-on-an-arduino
 */

 #include <Ethernet.h>
 #include <SPI.h>
 #include <SD.h>

//-----------------------------------------------------------------------------------------------------------------------------------------------------
//Chip Select del medidor y del arduino ethernet
const int csmedidor = 5;
const int csethernet = 10;
const int pinsdcard = 4;
//-----------------------------------------------------------------------------------------------------------------------------------------------------

//SPCR = (1<<SPE)|(1<<MSTR)|(1<<CPOL)|(1<<CPHA)|(1<<SPR1)|(1<<SPR0); //set clock rate to 1/16th system

byte eeprom_output_data;
byte multi_byte_data[3];
byte eeprom_input_data=0;
long long_eeprom_data = 0;
byte clr;
int address=0;
//data buffer
char buffer [128];

void fill_buffer()
{
  for (int I=0;I<128;I++)
  {
    buffer[I]=I;
  }
}

char spi_transfer(volatile char data)
{
  SPDR = data;                    // Start the transmission
  while (!(SPSR & (1<<SPIF)))     // Wait the end of the transmission
  {
  };
  return SPDR;                    // return the received byte
}

void setup()
{
  /**** Inicializar el Serial ****/
  Serial.begin(9600);

  pinMode(csmedidor, OUTPUT);
  registrosADE7553(); //inicializar los registros
  digitalWrite(csmedidor, HIGH);

  /******* Configuración SPI del medidor *******/
  /*
   * Modos con los que trabaja el SPI
   * SPI_MODE0
   * SPI_MODE1
   * SPI_MODE2
   * SPI_MODE3
   */
  SPI.setDataMode(SPI_MODE1);
  /*
   * Divisores del reloj
   * SPI_CLOCK_DIV2
   * SPI_CLOCK_DIV4
   * SPI_CLOCK_DIV8
   * SPI_CLOCK_DIV16
   * SPI_CLOCK_DIV32
   * SPI_CLOCK_DIV64
   * SPI_CLOCK_DIV128
   */
  SPI.setClockDivider(SPI_CLOCK_DIV2);
  /*
   * Orden del bit
   * LSBFIRST (Menos significativo primero)
   * MSBFIRST (Más significativo primero)
   */
  SPI.setBitOrder(MSBFIRST);
  /**** Inicializar el SPI ****/
  
  /*
   * Configuración en una sola transacción
   * SPI.beginTransaction(SPISettings (2000000, MSBFIRST, SPI_MODE1));  // 2 MHz clock
   */

  SPI.begin();
  /******* Fin Configuración SPI del medidor *******/

  //pinMode(SLAVESELECT,OUTPUT);
  //digitalWrite(SLAVESELECT,HIGH); //disable device
  SPCR = (1<<SPE)|(1<<MSTR)|(1<<CPHA)|(1<<SPR1)|(1<<SPR0);
  SPSR = (0<<SPI2X);
  clr=SPSR;
  clr=SPDR;
  delay(10);
  Serial.println("init complete");
  delay(1000);

  //testrun starts here

  //utils
  //read_eeprom(address value, how many bytes)
  //write_to_eeprom(target, values, bytes to write)

  //read what is there right now
  address = LINECYC;
 Serial.print(address,HEX);
 eeprom_output_data = read_eeprom(STATUS,2);

  long TestWrite;
  TestWrite = 0xABCD;
  Serial.println(TestWrite, BIN);
  write_to_eeprom(address, TestWrite, 2);
Serial.println(eeprom_output_data, BIN);
  eeprom_output_data = read_eeprom(address, 2);
  Serial.println("Completed basic read write test");
  
  delay(10000);

}

void write_to_eeprom(int EEPROM_address, long write_buffer, int bytes_to_write)
{
  //Serial.print("Multiwrite ops to addr>");
  //Serial.println(EEPROM_address, HEX);
  //set write mode
  byte make_write_cmd = B10000000;
  byte this_write = B00000000;
  EEPROM_address = EEPROM_address|make_write_cmd;
  //digitalWrite(SLAVESELECT,LOW);
  spi_transfer((char)(EEPROM_address));      //send address

  //here there should be a t7 delay, however long that is
  for (int i = 0; i<bytes_to_write; i++){
  //Serial.println(i);
  this_write = byte(write_buffer>>(8*((bytes_to_write-1)-i)));
  //Serial.println(this_write, HEX);
  spi_transfer((char)(this_write));      //send data byte
  }
  //digitalWrite(SLAVESELECT,HIGH); //release chip, signal end transfer
}

long read_eeprom(int EEPROM_address, int bytes_to_read)
{
  //Serial.print("Multi-read to addr>");
  //Serial.print(EEPROM_address, HEX);
  Serial.println(" Data starts:");
  long data = 0;
  byte reader_buf = 0;
  //digitalWrite(SLAVESELECT,LOW);
  spi_transfer((char)(EEPROM_address));      //send LSByte address
  for (int i = 1; i <= bytes_to_read; i++){
    reader_buf = spi_transfer(0xFF); //get data byte
    Serial.println(i);
    Serial.println(reader_buf, BIN);

    data = data|reader_buf;
    if (i< bytes_to_read) {
      data = data<<8;
    }
    }
  Serial.print("completed. data was>");
  Serial.println(data, BIN);
  //digitalWrite(SLAVESELECT,HIGH); //release chip, signal end transfer
  return data;
}

void loop() {
  eeprom_output_data = read_eeprom(STATUS,2);
  Serial.println("STATUS CHECK");
  Serial.println(eeprom_output_data, BIN);
  Serial.println(eeprom_output_data, HEX);
  delay(1000);
  eeprom_output_data = read_eeprom(LINECYC,2);
  Serial.println("LINECYC CHECK");
  Serial.println(eeprom_output_data, BIN);
  Serial.println(eeprom_output_data, HEX);   

  delay(1000);
  eeprom_output_data = read_eeprom(TEMP,1);
  Serial.println("TEMP CHECK");
  Serial.println(eeprom_output_data, BIN);
  Serial.println(eeprom_output_data, HEX);
  delay(5000);
}
