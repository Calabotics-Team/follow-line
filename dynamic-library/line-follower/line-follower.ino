
/* ============ CÓDIGO DE TESTE ========== */ 
// Em teoria, é o código PD completo, não é necessário o integral.

/*Este código é baseado no commit 255ffeead31a6e13ee9d55f82e63747157a2e0a0 de Rebeca, na branch analog_5_sensors*/

/*O objetivo desse código é para fazer um teste no robô, por isso, jogarei este na branch test_v.1, basicamente, o intuito é 
apenas ter o código em mãos, conectar no arduino, e fazer upload.
*/

/*Esse código utiliza duas bibliotecas (talvez nem use as duas), que são: 

  - Dynamic Motor Driver (permite controlar os motores DC de forma mais intuitiva)
  - QTRSensors (teste para calibragem mais precisa dos sensores)
*/

/*Referências da onde eu tirei a ideia de testar dessa forma: 

  https://robotresearchlab.com/2019/03/13/build-a-custom-pid-line-following-robot-from-scratch/
  https://github.com/pololu/qtr-sensors-arduino/tree/master/examples (biblioteca QTRSensors)
  https://github.com/mcc-robotics/Dynamic_Motor_Driver (biblioteca Dynamic Motor Driver)
  https://robotresearchlab.com/2019/02/16/pid-line-follower-tuning/ (para entender como funciona a leitura)

*/

// A intenção é adaptar o que está nessas referências para o nosso caso.


#include <QTRSensors.h>
#include <L298.h>

// sensores
#define pinSensor1 A4 
#define pinSensor2 A3
#define pinSensor3 A1
#define pinSensor4 A2
#define pinSensor5 A0

// L298N inputs
#define ENA 3
#define IN1 5
#define IN2 6
#define ENB 9
#define IN3 7
#define IN4 8

QTRSensors sensors; // objeto da biblioteca

const uint8_t SensorCount = 5; // quantidade de sensores com pinagem analógica
uint16_t sensorValues[SensorCount]; 

L298 driver(IN1, IN2, ENA, IN3, IN4, ENB); // um construtor que permite instanciar todos os inputs do driver

// Para seguidores de linha, só é necessário PD, e não PID
//Propriedades do PD
const double KP = 0.1275; // o valor retornado por readLineWhite() retorna um inteiro entre 0 - 4000 (N - 1 sensores * 1000), sendo 0 a extremidade esquerda, e 4000 a extremidade direita, pela lógica, 2000 é o centro
const double KD = 0.01; // alterar quando tiver um bom valor de P (proporcional)
double erroAnterior = 0.0;
const int META = 2000;
const int velocidadeMaxima = 100; // essa é a porcentagem de velocidade que a biblioteca irá converter internamente em valores PWM (100% = 255)
double velocidadeEsquerda = 0.0;
double velocidadeDireita = 0.0;

void setup() {

  driver.init(); // semelhate à aquela pilha de código que coloca o pinMode do driver como output
  sensors.setTypeAnalog();
  sensors.setSensorPins((const uint8_t[]){pinSensor5, pinSensor3, pinSensor4, pinSensor2, pinSensor1}, SensorCount);
  
  delay(500);
  digitalWrite(LED_BUILTIN, HIGH); // ligando o LED do arduino para mostrar que está em modo de calibragem

  for (uint16_t i = 0; i < 400; i++) { // a função chamada 400 vezes dará mais ou menos uns 10 segundos de calibragem, tempo suficiente para os sensores (caso seja muito tempo, vale diminuir)
    sensors.calibrate();
  }
  digitalWrite(LED_BUILTIN, LOW); // desliga o led -> fim da calibragem
}

void loop() {

  uint16_t posicao = sensors.readLineWhite(sensorValues); // retorna um número inteiro que diz onde acha que a linha está
  int erro = posicao - META;
  int ajuste = KP * erro + KD * (erro - erroAnterior); // semelhante a variável "correcao" outro código, mas ao invés de separar os dois em duas variáveis e depois somar, eu somei direto 
  
  erroAnterior = erro; // armazena o erro para o próximo incremento, loop de atribuir valor

  // Ajustar motores (-100 é o valor mínimo, basicamente o 0)
  velocidadeEsquerda = constrain(velocidadeMaxima + ajuste, -100, velocidadeMaxima);
  velocidadeDireita = constrain(velocidadeMaxima - ajuste, -100, velocidadeMaxima);

  // faz literalmente o que o método andar() do outro código faz
  driver.setMotorAPower(velocidadeEsquerda);
  driver.setMotorBPower(velocidadeDireita);
  
}
