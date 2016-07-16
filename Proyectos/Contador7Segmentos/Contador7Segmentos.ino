const int firstPinDisplayOne = 2;
const int firstPinDisplayTwo = 22;
const int difPin = firstPinDisplayTwo-firstPinDisplayOne;
const int timer = 400;
const int a = 2;
const int b = 3;
const int c = 4;
const int d = 5;
const int e = 6;
const int f = 7;
const int g = 8;

int zero[] = {a,b,c,d,e,f};
int one[] = {b,c};
int two[] = {a,b,d,e,g};
int three[] = {a,b,c,d,g};
int four[] = {b,c,f,g};
int five[] = {a,c,d,f,g};
int six[] = {a,c,d,e,f,g};
int seven[] = {a,b,c};
int eight[] = {a,b,c,d,e,f,g};
int nine[] = {a,b,c,f,g};

void setup() {
  for (int pin = firstPinDisplayOne; pin < firstPinDisplayOne+8; pin++) {
    pinMode(pin, OUTPUT);
  }
  for (int pin = firstPinDisplayTwo; pin < firstPinDisplayTwo+8; pin++) {
    pinMode(pin, OUTPUT);
  }
}

void pinsOffDisplayOne() {
  for (int pin = firstPinDisplayOne; pin < firstPinDisplayOne+8; pin++) {
    digitalWrite(pin, HIGH);
  }
}

void pinsOffDisplayTwo() {
  for (int pin = firstPinDisplayTwo; pin < firstPinDisplayTwo+8; pin++) {
    digitalWrite(pin, HIGH);
  }
}

void displayOne(int *arreglo, int arrayLength) {
  pinsOffDisplayOne();
  for (int pin = 0; pin < arrayLength; pin++) {
    digitalWrite(arreglo[pin], LOW);
  }
}

void displayTwo(int *arreglo, int arrayLength) {
  pinsOffDisplayTwo();
  for (int pin = 0; pin < arrayLength; pin++) {
    digitalWrite(arreglo[pin]+difPin, LOW);
  }
}

void numberDisplayOne(int num) {
  int s = sizeof(int);
  if (num == 0) { displayOne(zero, sizeof(zero)/s); }
  if (num == 1) { displayOne(one, sizeof(one)/s); }
  if (num == 2) { displayOne(two, sizeof(two)/s); }
  if (num == 3) { displayOne(three, sizeof(three)/s); }
  if (num == 4) { displayOne(four, sizeof(four)/s); }
  if (num == 5) { displayOne(five, sizeof(five)/s); }
  if (num == 6) { displayOne(six, sizeof(six)/s); }
  if (num == 7) { displayOne(seven, sizeof(seven)/s); }
  if (num == 8) { displayOne(eight, sizeof(eight)/s); }
  if (num == 9) { displayOne(nine, sizeof(nine)/s); }
}

void numberDisplayTwo(int num) {
  int s = sizeof(int);
  if (num == 0) { displayTwo(zero, sizeof(zero)/s); }
  if (num == 1) { displayTwo(one, sizeof(one)/s); }
  if (num == 2) { displayTwo(two, sizeof(two)/s); }
  if (num == 3) { displayTwo(three, sizeof(three)/s); }
  if (num == 4) { displayTwo(four, sizeof(four)/s); }
  if (num == 5) { displayTwo(five, sizeof(five)/s); }
  if (num == 6) { displayTwo(six, sizeof(six)/s); }
  if (num == 7) { displayTwo(seven, sizeof(seven)/s); }
  if (num == 8) { displayTwo(eight, sizeof(eight)/s); }
  if (num == 9) { displayTwo(nine, sizeof(nine)/s); }
}

void loop() {
  for (int decenas = 0; decenas < 10; decenas++) {
    numberDisplayOne(decenas);
    for (int unidades = 0; unidades < 10; unidades++) {
      numberDisplayTwo(unidades);
      delay(timer);
    }
  }
}
