// SOCRATES

/* ATENÇÃO: OS PINOS DOS SENSORES ESTÃO DIFERENTES, EM ORDEM */
#define pinSensor1 A4
#define pinSensor2 A3
#define pinSensor3 A2
#define pinSensor4 A1
#define pinSensor5 A0

/* Configuração dos motores */
#define ENA 3
#define IN1 5
#define IN2 6

#define ENB 9
#define IN3 7
#define IN4 8


int leituras[5];

const int minimo[5] = {
  9, 9, 9, 9, 9
};

const int maximo[5] = {
  1023, 1023, 1023, 1023, 1023
};

float normalizado[5];


int contador = 0;

const float Kp = 50.0;
const float Kd = 0.0;

float somaNormalizados = 0;
float mediaPonderada = 0;

float erroAnterior = 0;

const float linhaLimiar = 0.2;


const int PWM_Minimo = 62;
const int PWM_Maximo = 255;

int velocidadeEsquerda;
int velocidadeDireita;

const int velocidadeBase = 80;


unsigned long ultimaExec = 0;

const unsigned long intervalo = 4;


unsigned long tempoInicioRecuperacao = 0;

bool recuperando = false;

const unsigned long TEMPO_MAX_RECUPERACAO = 350;


const unsigned long TEMPO_MAX_ROBO = 110000;



void setup() {

  attachInterrupt(
    digitalPinToInterrupt(2),
    contadorParada,
    RISING
  );

  // Sensores
  pinMode(pinSensor1, INPUT);
  pinMode(pinSensor2, INPUT);
  pinMode(pinSensor3, INPUT);
  pinMode(pinSensor4, INPUT);
  pinMode(pinSensor5, INPUT);

  // Motor direito
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  // Motor esquerdo
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  Serial.begin(9600);
}


// ======================================================
// ANDAR PARA FRENTE
// ======================================================

void andar(int velocidadeDireita, int velocidadeEsquerda) {

  // Não permite velocidade negativa
  if (velocidadeDireita <= 0) {
    velocidadeDireita = 0;
  }

  if (velocidadeEsquerda <= 0) {
    velocidadeEsquerda = 0;
  }

  // PWM mínimo
  if (velocidadeDireita > 0 &&
      velocidadeDireita < PWM_Minimo) {

    velocidadeDireita = PWM_Minimo;
  }

  if (velocidadeEsquerda > 0 &&
      velocidadeEsquerda < PWM_Minimo) {

    velocidadeEsquerda = PWM_Minimo;
  }

  // PWM máximo
  if (velocidadeDireita > PWM_Maximo) {
    velocidadeDireita = PWM_Maximo;
  }

  if (velocidadeEsquerda > PWM_Maximo) {
    velocidadeEsquerda = PWM_Maximo;
  }

  // Sentido para frente
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);

  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);

  // Aplicação do PWM
  analogWrite(ENA, velocidadeDireita);
  analogWrite(ENB, velocidadeEsquerda);
}


// ======================================================
// INTERRUPÇÃO DO CONTADOR
// ======================================================

void contadorParada() {

  contador++;
}


// ======================================================
// PARAR
// ======================================================

void parar() {

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
}


// ======================================================
// RECUPERAÇÃO
// ======================================================

void recuperar() {

  if (!recuperando) {

    tempoInicioRecuperacao = millis();

    recuperando = true;
  }

  while (
    millis() - tempoInicioRecuperacao
    < TEMPO_MAX_RECUPERACAO
  ) {

    // Se o erro anterior era positivo,
    // a linha estava para a direita.
    if (erroAnterior > 0) {

      reDireita(80);

    } else {

      reEsquerda(80);
    }
  }

  parar();
}


// ======================================================
// LEITURA DOS SENSORES
// ======================================================

void leitura() {

  leituras[0] = analogRead(pinSensor1);  // peso -2
  leituras[1] = analogRead(pinSensor2);  // peso -1
  leituras[2] = analogRead(pinSensor3);  // peso  0
  leituras[3] = analogRead(pinSensor4);  // peso +1
  leituras[4] = analogRead(pinSensor5);  // peso +2
}


// ======================================================
// NORMALIZAÇÃO
// ======================================================

void normalizar() {

  for (int i = 0; i < 5; i++) {

    float faixa = maximo[i] - minimo[i];

    if (faixa <= 0) {

      normalizado[i] = 0;

    } else {

      normalizado[i] =
        (leituras[i] - minimo[i]) / faixa;

      // Limita entre 0 e 1
      if (normalizado[i] < 0) {

        normalizado[i] = 0;

      } else if (normalizado[i] > 1) {

        normalizado[i] = 1;
      }
    }
  }
}


// ======================================================
// SOMA DOS SENSORES NORMALIZADOS
// ======================================================

void calcularSomaNormalizados() {

  somaNormalizados = 0;

  for (int i = 0; i < 5; i++) {

    somaNormalizados += normalizado[i];
  }
}


// ======================================================
// RECUPERAÇÃO PARA A ESQUERDA
// ======================================================

void reEsquerda(int velocidadeRe) {

  // Carrinho não dá ré.
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  analogWrite(ENA, velocidadeRe);
  analogWrite(ENB, 0);
}


// ======================================================
// RECUPERAÇÃO PARA A DIREITA
// ======================================================

void reDireita(int velocidadeRe) {

  // Carrinho não dá ré.
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);

  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);

  analogWrite(ENA, 0);
  analogWrite(ENB, velocidadeRe);
}


// ======================================================
// LOOP PRINCIPAL
// ======================================================

void loop() {

  unsigned long tempoAtual = millis();


  // ====================================================
  // PARADA AUTOMÁTICA APÓS 110 SEGUNDOS
  // ====================================================

  if (tempoAtual >= TEMPO_MAX_ROBO) {

    parar();

    // Mantém o robô parado indefinidamente
    while (true) {

      parar();
    }
  }


  // ====================================================
  // CONTROLE DO INTERVALO DE EXECUÇÃO
  // ====================================================

  if (tempoAtual - ultimaExec < intervalo) {

    return;
  }

  ultimaExec = tempoAtual;


  // ====================================================
  // LEITURA DOS SENSORES
  // ====================================================

  leitura();

  normalizar();

  calcularSomaNormalizados();


  // ====================================================
  // VERIFICA SE ESTÁ SAINDO DE UMA RECUPERAÇÃO
  // ====================================================

  recuperando = false;


  // ====================================================
  // LINHA DETECTADA
  // ====================================================

  if (somaNormalizados >= linhaLimiar) {


    // --------------------------------------------------
    // CÁLCULO DA POSIÇÃO DA LINHA
    // --------------------------------------------------

    mediaPonderada =
      (
        normalizado[0] * -2.0 +
        normalizado[1] * -1.0 +
        normalizado[3] *  1.0 +
        normalizado[4] *  2.0
      )
      / somaNormalizados;


    // --------------------------------------------------
    // ERRO ATUAL
    // --------------------------------------------------

    float erroAtual = mediaPonderada;


    // --------------------------------------------------
    // TERMO PROPORCIONAL
    // --------------------------------------------------

    float parcelaProporcional =
      Kp * erroAtual;


    // --------------------------------------------------
    // VARIAÇÃO DO ERRO
    // --------------------------------------------------

    float variacaoErro =
      erroAtual - erroAnterior;


    // --------------------------------------------------
    // TERMO DERIVATIVO
    // --------------------------------------------------

    float parcelaDerivativa =
      Kd * variacaoErro;


    // --------------------------------------------------
    // CORREÇÃO TOTAL
    // --------------------------------------------------

    float correcao =
      parcelaProporcional +
      parcelaDerivativa;


    // --------------------------------------------------
    // VELOCIDADE DOS MOTORES
    // --------------------------------------------------

    velocidadeDireita =
      velocidadeBase + correcao;

    velocidadeEsquerda =
      velocidadeBase - correcao;


    // --------------------------------------------------
    // MOVIMENTA O ROBÔ
    // --------------------------------------------------

    andar(
      velocidadeDireita,
      velocidadeEsquerda
    );


    // --------------------------------------------------
    // SALVA O ERRO PARA A PRÓXIMA ITERAÇÃO
    // --------------------------------------------------

    erroAnterior = erroAtual;

  }


  // ====================================================
  // LINHA NÃO DETECTADA
  // ====================================================

  else {

    recuperar();
  }
}
