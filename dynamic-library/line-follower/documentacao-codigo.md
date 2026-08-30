# Documentação do Código — Seguidor de Linha

## Origem

Este código é baseado no commit `255ffeead31a6e13ee9d55f82e63747157a2e0a0` de Rebeca, na branch `analog_5_sensors`.

## Objetivo

O objetivo desse código é fazer um teste no robô. Por isso, ele foi colocado na branch `test_v.1`. O intuito é apenas ter o código em mãos, conectar no Arduino, e fazer o upload.

## Bibliotecas utilizadas

Esse código utiliza duas bibliotecas (talvez nem use as duas):

- **Dynamic Motor Driver** — permite controlar os motores DC de forma mais intuitiva.
  Repositório: https://github.com/mcc-robotics/Dynamic_Motor_Driver
- **QTRSensors** — usada para calibragem mais precisa dos sensores.
  Repositório: https://github.com/pololu/qtr-sensors-arduino

## Referências de onde a ideia de testar dessa forma foi tirada

- https://robotresearchlab.com/2019/03/13/build-a-custom-pid-line-following-robot-from-scratch/
- https://github.com/pololu/qtr-sensors-arduino/tree/master/examples (biblioteca QTRSensors)
- https://github.com/mcc-robotics/Dynamic_Motor_Driver (biblioteca Dynamic Motor Driver)
- https://robotresearchlab.com/2019/02/16/pid-line-follower-tuning/ (para entender como funciona a leitura)
- https://www.youtube.com/watch?v=PP4fvBVe3rI (vídeo que me guiou para desenvolver o código)

A intenção é adaptar o que está nessas referências para o nosso caso.

## Pinagem

### Sensores

| Sensor | Pino |
|---|---|
| pinSensor1 | A4 |
| pinSensor2 | A3 |
| pinSensor3 | A1 |
| pinSensor4 | A2 |
| pinSensor5 | A0 |

### Entradas do L298N

| Sinal | Pino |
|---|---|
| ENA | 3 |
| IN1 | 5 |
| IN2 | 6 |
| ENB | 9 |
| IN3 | 7 |
| IN4 | 8 |

## Objetos e variáveis principais

- `sensors` — objeto da biblioteca QTRSensors.
- `SensorCount` — quantidade de sensores com pinagem analógica (5).
- `sensorValues[SensorCount]` — vetor onde ficam armazenados os valores lidos dos sensores.
- `driver` — instância de `L298`, um construtor que permite instanciar todos os inputs do driver.

## Controle PD

Para seguidores de linha, só é necessário PD, e não PID.

Propriedades do PD:

- `KP = 0.1275` — o valor retornado por `readLineWhite()` retorna um inteiro entre 0 e 4000 (N - 1 sensores × 1000), sendo 0 a extremidade esquerda e 4000 a extremidade direita; pela lógica, 2000 é o centro.
- `KD = 0.01` — alterar quando tiver um bom valor de P (proporcional).
- `erroAnterior = 0.0` — armazena o erro do ciclo anterior.
- `META = 2000` — posição central desejada.
- `velocidadeMaxima = 100` — porcentagem de velocidade que a biblioteca irá converter internamente em valores PWM (100% = 255).
- `velocidadeEsquerda` e `velocidadeDireita` — velocidades calculadas para cada motor.

## setup()

- `driver.init()` — semelhante àquela pilha de código que coloca o `pinMode` do driver como `output`.
- `sensors.setTypeAnalog()` — define o tipo de leitura dos sensores como analógica.
- `sensors.setSensorPins(...)` — define os pinos dos sensores, na ordem `{pinSensor5, pinSensor3, pinSensor4, pinSensor2, pinSensor1}`, totalizando `SensorCount`.
- `delay(500)` seguido de `digitalWrite(LED_BUILTIN, HIGH)` — liga o LED do Arduino para mostrar que está em modo de calibragem.
- Loop de 400 chamadas a `sensors.calibrate()` — a função chamada 400 vezes dará mais ou menos uns 10 segundos de calibragem, tempo suficiente para os sensores (caso seja muito tempo, vale diminuir).
- `digitalWrite(LED_BUILTIN, LOW)` — desliga o LED, indicando o fim da calibragem.

## loop()

- `sensors.readLineWhite(sensorValues)` — retorna um número inteiro que diz onde acha que a linha está, armazenado em `posicao`.
- `erro = posicao - META` — calcula o erro em relação ao centro.
- `ajuste = KP * erro + KD * (erro - erroAnterior)` — semelhante à variável "correção" de outro código, mas, ao invés de separar os dois termos em duas variáveis e depois somar, a soma foi feita direto.
- `erroAnterior = erro` — armazena o erro para o próximo incremento, em um loop de atribuição de valor.
- Ajuste dos motores (-100 é o valor mínimo, basicamente o 0):
  - `velocidadeEsquerda = constrain(velocidadeMaxima + ajuste, -100, velocidadeMaxima)`
  - `velocidadeDireita = constrain(velocidadeMaxima - ajuste, -100, velocidadeMaxima)`
- `driver.setMotorAPower(velocidadeEsquerda)` e `driver.setMotorBPower(velocidadeDireita)` — faz literalmente o que o método `andar()` de outro código faz.

## Necessidade de testes

Este código ainda precisa ser testado no robô físico. Ele representa uma adaptação das referências citadas acima para o caso específico deste projeto, e os valores de `KP` e `KD`, em particular, dependem de ajuste prático após a calibragem dos sensores.
