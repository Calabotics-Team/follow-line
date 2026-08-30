# Robô Seguidor de Linha - Guia de Configuração

## Requisitos

- Arduino Uno (ou compatível)
- Arduino IDE instalado
- Cabo USB para Arduino
- Bibliotecas incluídas neste arquivo

---

## Instalação Passo a Passo

### Passo 1: Clonar ou Baixar o Repositório

1. Clique em **Code** (botão verde) no repositório do GitHub
2. Selecione **Download ZIP**
3. Extraia o arquivo em uma pasta de sua preferência

Ou use Git:

```bash
git clone <URL-do-repositório>
cd line-follower-robot
```

### Passo 2: Instalar as Bibliotecas

#### Opção A: Instalação Automática (Recomendado)

1. Abra o Arduino IDE
2. Vá para **Sketch → Include Library → Add .ZIP Library**
3. Navegue até a pasta `libraries/DynamicMotorDriver`
4. Clique em **Selecionar Pasta**
5. Repita o processo para `libraries/QTRSensors`

#### Opção B: Instalação Manual

1. Localize a pasta de bibliotecas do Arduino:
   - **Windows:** `C:\Users\SeuUsuário\Documents\Arduino\libraries\`
   - **Mac:** `~/Documents/Arduino/libraries/`
   - **Linux:** `~/Arduino/libraries/`
2. Copie as pastas:
   - `DynamicMotorDriver`
   - `QTRSensors`

   Para a pasta `libraries` do Arduino
3. Reinicie o Arduino IDE

### Passo 3: Abrir o Código

1. Abra o Arduino IDE
2. Vá para **File → Open**
3. Navegue até `line-follower/line-follower.ino`
4. Clique em **Abrir**

### Passo 4: Configurar a Porta e Placa

1. Vá para **Tools → Board** e selecione **Arduino Uno** (ou sua placa)
2. Vá para **Tools → Port** e selecione a porta COM do seu Arduino
   - **Windows:** `COM3`, `COM4`, etc.
   - **Mac:** `/dev/cu.usbserial-*`
   - **Linux:** `/dev/ttyUSB0`

### Passo 5: Upload do Código

1. Conecte o Arduino ao computador via USB
2. Clique no botão **Upload** (seta para a direita)
3. Aguarde a mensagem: **"Upload complete"**

---

## Configuração de Hardware

### Ligação dos Sensores QTR

| Sensor | Pino Arduino |
|---|---|
| Sensor 1 | A4 |
| Sensor 2 | A3 |
| Sensor 3 | A1 |
| Sensor 4 | A2 |
| Sensor 5 | A0 |

> **Observação:** Os sensores devem ser dispostos horizontalmente na frente do robô.

### Ligação do Driver L298N

| Driver | Pino Arduino |
|---|---|
| IN1 (Motor A - Direção) | 5 |
| IN2 (Motor A - Direção) | 6 |
| ENA (Motor A - PWM) | 3 |
| IN3 (Motor B - Direção) | 7 |
| IN4 (Motor B - Direção) | 8 |
| ENB (Motor B - PWM) | 9 |

**Motores:**

- **Motor A (Esquerda):** IN1, IN2, ENA
- **Motor B (Direita):** IN3, IN4, ENB

---

## Calibração dos Sensores

1. Após o upload, o LED interno do Arduino acenderá
2. Isto indica que o modo de calibração foi iniciado
3. Passe o robô sobre a linha várias vezes durante 10 segundos
4. Certifique-se de expor cada sensor tanto à linha (preto) quanto ao fundo (branco)
5. Quando o LED apagar, a calibração terminou
6. O robô está pronto para começar a seguir a linha!

---

## Ajuste dos Parâmetros PD

Se o robô não estiver se comportando corretamente, você pode ajustar os valores em `line-follower.ino`:

**Se o robô não segue a linha:**

- Aumente `KP` (ex: 0.1275 → 0.15)

**Se o robô oscila demais (zig-zag):**

- Diminua `KP` (ex: 0.1275 → 0.10)
- Aumente `KD` (ex: 0.01 → 0.02)

**Se o robô é muito lento em responder:**

- Aumente `KD` para mais amortecimento

**Exemplo de ajuste:**

```cpp
const double KP = 0.15;  // Aumentado de 0.1275
const double KD = 0.02;  // Aumentado de 0.01
```

---

## Solução de Problemas

### Erro: "L298.h: No such file or directory"

- Verifique se as bibliotecas foram instaladas corretamente
- Reinicie o Arduino IDE
- Tente instalar as bibliotecas novamente usando a opção "Add .ZIP Library"

### Erro: "QTRSensors.h: No such file or directory"

- Mesmo procedimento acima para a biblioteca QTRSensors

### O Arduino não é detectado

- Verifique se o cabo USB está funcionando
- Tente outro cabo USB
- Verifique a porta correta em **Tools → Port**
- Instale os drivers CH340 se necessário (comum em Arduino clones)

### O robô não segue a linha após calibração

- Verifique se todos os sensores estão ligados corretamente
- Teste a calibração novamente
- Ajuste os parâmetros `KP` e `KD`
- Verifique se a linha tem contraste suficiente com o fundo

### Um motor não funciona

- Verifique as conexões do motor
- Teste com o código de exemplo `DRV8835_Basic` ou `TB67H420FTG_Basic`
- Verifique se o driver L298N está recebendo energia

---

## Documentação Completa

Para entender melhor o código e as decisões de design, consulte:

- `documentacao-codigo.md` - Documentação detalhada do código
- `libraries/DynamicMotorDriver/README.md` - Documentação da biblioteca Motor
- `libraries/QTRSensors/README.md` - Documentação da biblioteca QTR

---

## Referências

- **Dynamic Motor Driver:** https://github.com/mcc-robotics/Dynamic_Motor_Driver
- **QTR Sensors Arduino:** https://github.com/pololu/qtr-sensors-arduino
- **Build a Custom PID Line Following Robot:** https://robotresearchlab.com/2019/03/13/build-a-custom-pid-line-following-robot-from-scratch/
