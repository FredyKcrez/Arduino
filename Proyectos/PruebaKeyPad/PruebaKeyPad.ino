/* @file HelloKeypad.pde
|| @version 1.0
|| @author Alexander Brevig
|| @contact alexanderbrevig@gmail.com
||
|| @description
|| | Demonstrates the simplest use of the matrix Keypad library.
|| #
*/
#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {9, 8, A1, 7}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {6, 5, 3, 2}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

String cad = "000";
char letra;

void setup(){
  Serial.begin(9600);
}
  
void loop(){
  leerKeypad();
  //validarValor();
}

void leerKeypad() {
  char key = keypad.getKey();
  if(key) {
    switch(key) {
      case '*':
        cad = "000";
        letra = 'Z';
        break;
      case '#':
        imprimir();
        break;
      case 'A':
        letra = 'R';
        break;
      case 'B':
        letra = 'V';
        break;
      case 'C':
        letra = 'A';
        break;
      case 'D':
        letra = 'Z';
        break;
      default:
        ingresarlo(key);
        break;
    }
  }
}

void ingresarlo(char k) {
  cad[0] = cad[1];
  cad[1] = cad[2];
  cad[2] = k;
}

void imprimir() {
  int val = cad.toInt();
  if(val>=0 && val<= 255) {
    Serial.println(val);
    Serial.println(letra);
  }
  else {
    Serial.println("Valor no permitido");
  }
}

/*void validarValor() {
  
}*/
