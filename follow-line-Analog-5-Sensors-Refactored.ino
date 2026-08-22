//mudei os sensores para pinagem analógica
#define pinSensor1 A1 
#define pinSensor2 A2
#define pinSensor3 A3
#define pinSensor4 A4
#define pinSensor5 A5

/* mantive a configuração dos motores, igual nas versões anteriores */
#define ENA 5
#define IN1 4
#define IN2 3
#define ENB 9
#define IN3 8
#define IN4 10

// cria variáveis como double por que recebe agora valor analógico.
float sensorExtremaEsquerda = 0;
float sensorMeiaEsquerda = 0;
float sensorMeiaDireita = 0;
float sensorExtremaDireita = 0;
float sensorFrente = 0;
//cria constantes com valores iniciais para tentativa e erro
int Kp = 15;
int Ki = 0;
int Kd = 5;
//define variáveis para cálculo
float mediaPonderada;
//float proporcional;
float erroAnterior = 0;
//float derivada;
//float integral;
//float correcao;
//define variáveis de velocidade
float velocidadeEsquerda;
float velocidadeDireita;
int velocidadeBase=200;
int contador = 0;
float guardaErros [5] = {0,0,0,0,0}; 
void setup()
{
    // definir todos os sensores como input para leitura
    pinMode(pinSensor1, INPUT);
    pinMode(pinSensor2, INPUT);
    pinMode(pinSensor3, INPUT);
    pinMode(pinSensor4, INPUT);
    pinMode(pinSensor5, INPUT);

    pinMode(ENA, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);

    pinMode(ENB, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);

    Serial.begin(9600);
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

void loop()
{
    //Chama a função para calcular os erros dos sensores
    float erro = calcularErrosSensor();
    //multiplica a media ponderada pela constante proporcional
    float proporcional = Kp * erro;
    //chama uma função para somar todos os erros acumulados no vetor
    float somaDosErros = calcularIntegral(proporcional);
    //calcula a integral multiplicado o seu valor com a soma de todos os erros
    float integral = Ki * somaDosErros;
    //correção é de fato o PID mas sem a devidada, nesse momento sem esse refino, talvez adicionar posteriormente
    correcao = proporcional + integral;
    /*aqui é onde de fato a correção é imposta, essa cálculo não faz com que sempre a direita vá mais rápido que a esquerda, por que não necessariamente correção será positivo
    o valor pode vim a ser negativo (temos pesos negativos) o que vai fazer o contrário o esquerdo ir mais rápido que o direito, e o fato de que uma sempre vá mais rápido que a outra
    não é um problema, pois de fato o único momento onde não deve ser feitas correções é quando correção é 0 (não há erro)*/
    velocidadeDireita = velocidadeBase + correcao;
    velocidadeEsquerda = velocidadeBase - correcao;
    
    //necessário colocar constrain para limitar a velocidade
    andar(constrain(velocidadeDireita, 0, 255), constrain(velocidadeEsquerda,0,255));
    
    //calculo da derivada caso seja necessário
    float derivada = Kd * (erro - erroAnterior);

    // correcao = proporcional  + integral + derivada
    //atualiza erroAnterior
    erroAnterior = erro;
}

float calcularErrosSensor()
{
    //definição de todos os sensores 
    sensorExtremaEsquerda = analogRead(pinSensor1); //peso -2
    sensorMeiaEsquerda = analogRead(pinSensor2); //peso -1
    sensorMeiaDireita = analogRead(pinSensor3); //peso 1
    sensorExtremaDireita = analogRead(pinSensor4); //peso 2
    sensorFrente = analogRead(pinSensor5); //peso 0
    
    float somaLeituras = (sensorExtremaEsquerda + sensorMeiaEsquerda + sensorFrente + sensorExtremaDireita + sensorMeiaDireita);
    //teste para uma situação em que todos os sensores dessem muito baixo está todo no preto(para não quebrar a divisão e impedir compilação)
    if(somaLeituras >= 15){
      // a média ponderada aqui é o erro, o cálculo do erro
      mediaPonderada = (sensorExtremaEsquerda * -2 + sensorMeiaEsquerda * -1 + sensorMeiaDireita * 1 + sensorExtremaDireita * 2)/somaLeituras;
    }else{
      parar();
      return;
    }

    return mediaPonderada;
}
float calcularIntegral(float erro)
{
  guardaErros[contador] = erro;

  contador = contador++;

  if(contador > guardaErros.length()-1){
      contador = 0;
    }

  float somaErros = 0;

  for(int i =0; i<guardaErros.length();i++){
      somaErros += guardaErros[i];
    }
  return somaErros;
}


    