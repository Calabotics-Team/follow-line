// Mudei os sensores para pinagem analógica
#define pinSensor1 A4
#define pinSensor2 A3
#define pinSensor3 A2
#define pinSensor4 A1
#define pinSensor5 A0

/* Mantive a configuração dos motores, igual nas versões anteriores */
#define ENA 3 // Alterado para porta PMW
#define IN1 5
#define IN2 6


#define ENB 8
#define IN3 9
#define IN4 7


void setup()
{
    // Configuração dos sensores como entrada
    pinMode(pinSensor1, INPUT);
    pinMode(pinSensor2, INPUT);
    pinMode(pinSensor3, INPUT);
    pinMode(pinSensor4, INPUT);
    pinMode(pinSensor5, INPUT);

    // Configuração dos motores
    pinMode(ENA, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);

    pinMode(ENB, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);

    Serial.begin(9600);
}

/*
 * Mantém os dois motores andando para frente.
 * A velocidade de cada motor determina a correção da trajetória.
 */
void andar(int velocidadeDireita, int velocidadeEsquerda)
{
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);

    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);

    analogWrite(ENA, velocidadeDireita);
    analogWrite(ENB, velocidadeEsquerda);
}

void parar()
{
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);

    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);

    analogWrite(ENA, 0);
    analogWrite(ENB, 0);
}

void loop()
{
  
  double leitura1 = analogRead(pinSensor1);
  double leitura2 = analogRead(pinSensor2);

  double leitura3 = analogRead(pinSensor3);

  double leitura4 = analogRead(pinSensor4);
  double leitura5 = analogRead(pinSensor5);
    


  
  double analogBaixo = 500;
  double analogAlto = 1023;

  if (leitura1 >= analogBaixo && leitura2 >= analogBaixo && leitura3 >= analogBaixo && leitura4 >= analogBaixo && leitura5 >= analogBaixo) {
    for(int i = 0; i <= 255; i += 10){
        Serial.println(i);
        andar(i, i);
        delay(2000);
      }
  } else {
    parar();
  }
    
    
}



