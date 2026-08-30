#define ENA 3
#define IN1 5
#define IN2 6
#define ENB 9
#define IN3 7
#define IN4 8

#define pinSensor1 A4 // sensor Ext esq
#define pinSensor2 A3 // sensor Meia esq
#define pinSensor3 A1 // sensor Meio
#define pinSensor4 A2 // sensor Meia dir
#define pinSensor5 A0 // sensor Ext dir
int velocidadeBase = 120;

void setup()
{
  Serial.begin(9600);

  // Motores
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // Sensores
  pinMode(pinSensor1, INPUT);
  pinMode(pinSensor2, INPUT);
  pinMode(pinSensor3, INPUT);
  pinMode(pinSensor4, INPUT);
  pinMode(pinSensor5, INPUT);
}

void frente()
{
  analogWrite(ENA, velocidadeBase);
  analogWrite(ENB, velocidadeBase);

  // Motor direito
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  // Motor esquerdo
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void meiaDir()
{
  analogWrite(ENA, velocidadeBase);
  analogWrite(ENB, 0);

  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void extDir()
{
  analogWrite(ENA, velocidadeBase);
  analogWrite(ENB, velocidadeBase);

  // Motor direito para frente
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  // Motor esquerdo para trás
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void meiaEsq()
{
  analogWrite(ENA, 0);
  analogWrite(ENB, velocidadeBase);

  // Motor direito parado
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  // Motor esquerdo para frente
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void extEsq()
{
  analogWrite(ENA, velocidadeBase);
  analogWrite(ENB, velocidadeBase);

  // Motor direito para trás
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  // Motor esquerdo para frente
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void parar()
{
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void loop()
{
  int sens1 = digitalRead(pinSensor1);
  int sens2 = digitalRead(pinSensor2);
  int sens3 = digitalRead(pinSensor3);
  int sens4 = digitalRead(pinSensor4);
  int sens5 = digitalRead(pinSensor5);

  Serial.print(sens1);
  Serial.print(" ");
  Serial.print(sens2);
  Serial.print(" ");
  Serial.print(sens3);
  Serial.print(" ");
  Serial.print(sens4);
  Serial.print(" ");
  Serial.println(sens5);

  if (sens1 == 1 && sens2 == 1 && sens3 == 0 && sens4 == 1 && sens5 == 1){
    frente();
  }

  if (sens1 == 1 && sens2 == 0 && sens3 == 1 && sens4 == 1 && sens5 == 1){
    meiaDir();
  }

  if (sens1 == 0 && sens2 == 1 && sens3 == 1 && sens4 == 1 && sens5 == 1){
    extDir();
  }

  if (sens1 == 1 && sens2 == 1 && sens3 == 1 && sens4 == 0 && sens5 == 1){
    meiaEsq();
  }

  if (sens1 == 1 && sens2 == 1 && sens3 == 1 && sens4 == 1 && sens5 == 0)
  {
    extEsq();
  }

  if (sens1 == 1 && sens2 == 1 && sens3 == 0 && sens4 == 0 && sens5 == 1){
    meiaEsq();
  }

  if (sens1 == 1 && sens2 == 0 && sens3 == 0 && sens4 == 1 && sens5 == 1){
    meiaDir();
  }

  if (sens1 == 0 && sens2 == 0 && sens3 == 0 && sens4 == 1 && sens5 == 1) {
    extDir();
  }

  if (sens1 == 1 && sens2 == 1 && sens3 == 0 && sens4 == 0 &&sens5 == 0){
    extEsq();
  }

  if (sens1 == 0 && sens2 == 0 && sens3 == 0 && sens4 == 0 && sens5 == 0){
    parar();
  }
}

