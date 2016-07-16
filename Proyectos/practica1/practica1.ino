const int pinAnalog = A0;

int lectura = 0;
int voltaje = 0;
int i = 1;
float tiempo = 0.00;

void setup() {
  Serial.begin(9600);
  Serial.println("i \t Tiempo \t Voltaje");
}

void loop() {
  if(i<=10) {
    principal();
  }
  /*else{
    Serial.println("¿Desea continuar? S= Si - N = No");
    char flat = Serial.read();
    Serial.println(flat);
  }*/
}

int principal() {
  //float times = micros();
  lectura = analogRead(pinAnalog);
  tiempo = micros();// - times;
  voltaje = map(lectura, 0, 1023, 0, 5);
  myprint();
  i++;
  //delay(1000);
}

void myprint() {
  Serial.print(i);
  Serial.print(" \t ");
  if(tiempo>1000) {
    float timeMili = tiempo/1000;
    Serial.print(timeMili);
    Serial.print(" milis.");
    Serial.print(" \t ");
  }
  else {
    Serial.print(tiempo);
    Serial.print(" micros.");
    Serial.print("  ");
  }
  Serial.println((float) voltaje);
}
