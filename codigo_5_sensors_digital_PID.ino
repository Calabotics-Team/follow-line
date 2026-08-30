///mudei os sensores para pinagem analógica
#define pinSensor1 A4 //sensor Ext esq
#define pinSensor2 A3 //sensor Meia esq
#define pinSensor3 A1 //sensor Meio
#define pinSensor4 A2//sensor Meia dir
#define pinSensor5 A0 //sensor Ext dir
//*************************************************//
/* mantive a configuração dos motores, igual nas versões anteriores */
#define ENA 3
#define IN1 5
#define IN2 6
#define ENB 9
#define IN3 7
#define IN4 8
//pesos para cada sensor
int pesos[5] = {-2, -1, 0, 1, 2};
//cria constantes com valores iniciais para tentativa e erro
float Kp = 30;
float Kd = 0;
float Ki = 0;
//define variáveis para cálculo
int proporcional;
int erroAnterior = 0;
int derivada;
int correcao;
//define variáveis de velocidade
double velocidadeEsquerda;
double velocidadeDireita;
int velocidadeBase=120;

void setup() {

  Serial.begin(9600);

  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

}

/*andar: definir os IN1, IN2, IN3, IN4 com constantes, por que os motores devem sempre andar para frente, a velocidade que dita a direção, chutei se essa definição é que manda para frente
necessário confirmar */
void andar(double velocidadeDireita, double velocidadeEsquerda) 
{    
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, velocidadeDireita);
  analogWrite (ENB, velocidadeEsquerda);
}

void parar(){
  //TODO: implementar um delay que aguarda e verifica novamente se ainda todos os sensores estão muito baixos, se sim, para, se não, dá return
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  analogWrite(ENA, LOW);
  analogWrite (ENB, LOW);
}

void loop() {
  int sensor1 = digitalRead(pinSensor1);  
  int sensor2 = digitalRead(pinSensor2);  
  int sensor3 = digitalRead(pinSensor3);  
  int sensor4 = digitalRead(pinSensor4);  
  int sensor5 = digitalRead(pinSensor5);  
  
  int erro = sensor1 * pesos[0] + sensor2 * pesos[1] + sensor3 * pesos[2] + sensor4 * pesos[3] + sensor5 * pesos[4];

  //a proporcional nada mais é do que a constante x erro
  proporcional = Kp * erro;
    
  //a derivada sendo a constante x o erro atual - o erro anterior
  derivada = Kd * (erro - erroAnterior);

  //correção é de fato o PID mas sem a integral, nesse momento sem esse refino, talvez adicionar posteriormente
  correcao = proporcional + derivada;
  Serial.print("correcao: ");
  Serial.println(correcao);

  // Velocidade dos motores
  velocidadeDireita = velocidadeBase + correcao;
  velocidadeEsquerda = velocidadeBase - correcao;

  //limitar a velocidade 
  velocidadeDireita = constrain(velocidadeDireita, 0, 255);
  velocidadeEsquerda = constrain(velocidadeEsquerda, 0, 255);

  Serial.print("Velocidade Direita: ");
  Serial.println(velocidadeDireita);
  Serial.print("Velocidade Esquerda: ");
  Serial.println(velocidadeEsquerda);
  

  andar(velocidadeDireita, velocidadeEsquerda);
  //atualiza erroAnterior
  erroAnterior = erro;
}