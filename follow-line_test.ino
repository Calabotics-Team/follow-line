/* ========= SEGUIDOR DE LINHA CALABOTICS ========= */
/* 
  A intenção desse código é para testar possíveis situações, dado a reunião de terça-feira (11/06) vimos que estamos com problemas no infravermelho e no PID
  também, ou seja, em tudo. Mas já conseguimos ter outputs de códigos que fizemos, esse comentário é como uma documentação do que eu vou fazer aqui.

  Primeiramente, para resolver o problema do infravermelho, que estava com a leitura invertida, basta inverter a lógical. Ok. Porém estudei mais profundamente
  e talvez a pista lá na competição tenha diferentes formas de junções, junções são as formas de como a pista é elaborada. 

    Exemplos: durante o percurso, acredito que não terá apenas linhas retas ou curvas, possa ser, por exemplo, que tenha uma cruz " + " em que o carrinho
    terá que tomar uma decisão. Sobre a decisão em uma leitura de sensor que seja de cruz especificadamente, precisamos de uma reunião para ver como o
    robô vai tomar a decisão certa (qual dos quatro caminhos ir) e não cair em falso.

  Um outro assunto que devemos tratar o quanto antes é alterar o hardware: o sensor que usamos na terça-feira é limitado, apenas dois não é o suficiente para
  fazer uma leitura com maior precisão, tendo em mente que, quanto mais sensores, maior será a acurácia de decisão do carrinho.

  O que apresento aqui não é uma solução do problema que tivemos na terça-feira, mas sim uma reformulação: considerando que podemos trocar o sensor que estamos 
  usando atualmente por um outro, o código desse arquivo é uma versão hipotética em que o carrinho tenha 5 sensores.

    **para melhor explicação, irei fazer uma documentação em markdown só para isso**

  **
  -> ATENÇÃO: é de extrema importância saber a largura da linha escura, pois pode influenciar no i/o dos sensores. Por convenção, vou fazer o código como se a lagura
  da fita fosse menor que a distância entre o sensor da meia esqueda, e o sensor da meia direita (vou colocar representações na documentação markdown).

  -> Para este código, irei desconsiderar qualquer lógica de PID. Vamos por partes, com o infravermelho funcionando perfeitamente, a lógica do PID será mais fácil de
     implementar, tendo em mente que é apenas a lógica de controle de velocidade aplicada em cada caso que estará neste código.
  ** 
*/


/* ========== CÓDIGO ========== */

/* alteração do i/o para o carrinho fazer o digitalRead() em 5 inputs (peguei os inputs restantes dos que não foram utilizados entre os números de 2 a 13)*/
#define pinSensor1 2 
#define pinSensor2 6
#define pinSensor3 7
#define pinSensor4 11
#define pinSensor5 12

/* mantive a configuração dos motores, igual nas versões anteriores */
#define ENA 5
#define IN1 4
#define IN2 3
#define ENB 9
#define IN3 8
#define IN4 10

/* (PID desconsiderado)
int velocidadeBase = 150;
float Kp = 25;
float Kd = 10;
int erro = 0;
int erroAnterior = 0;
*/

// inicialização dos leitores como falso (zero) por padrão
bool sensorExtremaEsquerda = 0;
bool sensorMeiaEsquerda = 0;
bool sensorMeiaDireita = 0;
bool sensorExtremaDireita = 0;
bool sensorAtras = 0;

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

// métodos (preferi deixar acima do loop por motivos de costume, levando em consideração que o C++ compila de cima para baixo)
void andar(int IN1E, int IN2E, int IN1D, int IN2D) 
{    
       digitalWrite(IN1, IN1E);
       digitalWrite(IN2, IN2E);
       digitalWrite(IN3, IN1D);
       digitalWrite(IN4, IN2D);
}

// este método irá testar algumas condições de leitura, e terá retorno um número inteiro para cada situação. Reaproveitarei o switch feito por Leonardo, achei uma boa ideia
int decisao(bool sensorAtras, bool sensorExtremaEsquerda, bool sensorMeiaEsquerda, bool sensorMeiaDireita, bool sensorExtremaDireita)
{
  
  if (sensorAtras && !sensorExtremaEsquerda && !sensorMeiaEsquerda && !sensorMeiaDireita && !sensorExtremaDireita) // primeira lógica: seguir para frente 
  {
    return 0;
  } else if (sensorExtremaEsquerda && sensorMeiaDireita && sensorAtras && !sensorMeiaDireita && !sensorExtremaDireita) // segunda lógica: ir para a esquerda
  {
    return 1;
  } else if (sensorExtremaDireita && sensorMeiaDireita && sensorAtras && !sensorMeiaEsquerda && !sensorExtremaEsquerda) // terceira lógica: ir para a direita
  {
    return -1;
  } else if (sensorAtras && sensorExtremaEsquerda && sensorMeiaEsquerda && sensorMeiaDireita && sensorExtremaDireita) // lógica quando o carrinho encontra um "T" na linha 
  {  
    return 2; // precisamos ver como faremos a lógica de decisão nesse sentido
  } else 
  {
    return 3; // pensar em mais lógicas possíveis (exemplo: se nenhum dos sensores estiverem indentificando nada, parar)
  }
}

void loop()
{
    // instância dos 5 sensores
    sensorExtremaEsquerda = digitalRead(pinSensor1);
    sensorMeiaEsquerda = digitalRead(pinSensor2);
    sensorMeiaDireita = digitalRead(pinSensor3);
    sensorExtremaDireita = digitalRead(pinSensor4);
    sensorAtras = digitalRead(pinSensor5);

    int retornoDecisao = decisao(sensorAtras, sensorExtremaEsquerda, sensorMeiaEsquerda, sensorMeiaDireita, sensorExtremaDireita);

    switch(retornoDecisao) 
    {
      case 1:
        andar(1,0,0,1);
        break;
      case -1:
        andar(0,1,1,0);
        break;
      case 0:
        andar(0,1,0,1);
        break;
      case 2:
        // decidir lógica
        break;
      case 3: 
        // decidir lógica
      default:
        andar(0,0,0,0);
    }
}


    /* 
    
    // lógica desconsiderada para este escopo:

       if (sensorE && !sensorD)
    {
      //carrinho move para a direita
        erro = -1;
    }
    else if (!sensorE && sensorD)
    {
        //carrinho move para a esquerda
        erro = 1;
    }
    else if (!sensorE && !sensorD)
    {
    //carrinho continua indo para a frente
            erro = 0;
    }
    else
    {
   
    erro = erroAnterior;
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);

    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    }

    int derivada = erro - erroAnterior;

    int correcao = (Kp * erro) + (Kd * derivada);

    int velocidadeMotorEsq = velocidadeBase + correcao;
    int velocidadeMotorDir = velocidadeBase - correcao;

    velocidadeMotorEsq = constrain(velocidadeMotorEsq, 0, 255);
    velocidadeMotorDir = constrain(velocidadeMotorDir, 0, 255);

    analogWrite(ENB, velocidadeMotorEsq);
    analogWrite(ENA, velocidadeMotorDir);

    erroAnterior = erro;

    switch(erro) {
      case 1:
        andar(1,0,0,1);
        break;
      case -1:
        andar(0,1,1,0);
        break;
      case 0:
        andar(0,1,0,1);
        break;
      default:
        andar(0,0,0,0);
    }
}

void andar(int IN1E, int IN2E, int IN1D, int IN2D) 
{    
       digitalWrite(IN1, IN1E);
       digitalWrite(IN2, IN2E);
       digitalWrite(IN3, IN1D);
       digitalWrite(IN4, IN2D);
}
*/