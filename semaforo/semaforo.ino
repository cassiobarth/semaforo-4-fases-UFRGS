/*
 * Semáforo de 4 Fases — Arduino Mega 2560
 * Disciplina: PGIE024 – Seminário Avançado: Projeto de Robótica Educacional
 * Aplicado à Educação Especial na Perspectiva Inclusiva (turma RE)
 * Prof. Dr. Renato Ventura Bayan Henriques
 *
 * Controle de semáforo com 4 vias (N/S/L/O), LEDs de pedestre,
 * botões com interrupção e debounce, fase amarela e rotina de travessia.
 *
 * Fases do ciclo:
 *   0 – NS verde  / LO vermelho
 *   1 – NS amarelo / LO vermelho
 *   2 – LO verde  / NS vermelho
 *   3 – LO amarelo / NS vermelho
 */

// ─── Pinos dos semáforos de carros (cátodo comum) ───────────────────────────
// Norte
#define CAR_N_RED    22
#define CAR_N_YEL    23
#define CAR_N_GRN    24
// Sul
#define CAR_S_RED    25
#define CAR_S_YEL    26
#define CAR_S_GRN    27
// Leste
#define CAR_L_RED    28
#define CAR_L_YEL    29
#define CAR_L_GRN    30
// Oeste
#define CAR_O_RED    31
#define CAR_O_YEL    32
#define CAR_O_GRN    33

// ─── Pinos dos semáforos de pedestres (ânodo comum) ─────────────────────────
// Norte
#define PED_N_RED    34
#define PED_N_GRN    35
// Sul
#define PED_S_RED    36
#define PED_S_GRN    37
// Leste
#define PED_L_RED    38
#define PED_L_GRN    39
// Oeste
#define PED_O_RED    40
#define PED_O_GRN    41

// ─── Pinos dos botões de pedestres ──────────────────────────────────────────
#define BTN_N  2   // INT4 (Mega)
#define BTN_S  3   // INT5 (Mega)
#define BTN_L  18  // INT3 (Mega)
#define BTN_O  19  // INT2 (Mega)

// ─── Tempos (ms) ────────────────────────────────────────────────────────────
#define T_GREEN      5000
#define T_YELLOW     2000
#define T_CROSSING   4000
#define DEBOUNCE_MS  200

// ─── Índices dos pedestres ──────────────────────────────────────────────────
#define IDX_N 0
#define IDX_S 1
#define IDX_L 2
#define IDX_O 3

// ─── Variáveis voláteis para interrupções ────────────────────────────────────
volatile bool          pedRequest[4] = {false, false, false, false};
volatile unsigned long lastPress[4]  = {0, 0, 0, 0};

// ─── Fase atual ──────────────────────────────────────────────────────────────
int fase = 0;

// ─── ISRs dos botões ─────────────────────────────────────────────────────────
void isr_N() {
  unsigned long now = millis();
  if (now - lastPress[IDX_N] > DEBOUNCE_MS) {
    pedRequest[IDX_N] = true;
    lastPress[IDX_N]  = now;
    Serial.println(F("Botão Norte pressionado"));
  }
}

void isr_S() {
  unsigned long now = millis();
  if (now - lastPress[IDX_S] > DEBOUNCE_MS) {
    pedRequest[IDX_S] = true;
    lastPress[IDX_S]  = now;
    Serial.println(F("Botão Sul pressionado"));
  }
}

void isr_L() {
  unsigned long now = millis();
  if (now - lastPress[IDX_L] > DEBOUNCE_MS) {
    pedRequest[IDX_L] = true;
    lastPress[IDX_L]  = now;
    Serial.println(F("Botão Leste pressionado"));
  }
}

void isr_O() {
  unsigned long now = millis();
  if (now - lastPress[IDX_O] > DEBOUNCE_MS) {
    pedRequest[IDX_O] = true;
    lastPress[IDX_O]  = now;
    Serial.println(F("Botão Oeste pressionado"));
  }
}

// ─── Helpers para semáforo de carro (cátodo comum: HIGH = aceso) ─────────────
void carRed(int r, int y, int g)    { digitalWrite(r, HIGH); digitalWrite(y, LOW);  digitalWrite(g, LOW);  }
void carYellow(int r, int y, int g) { digitalWrite(r, LOW);  digitalWrite(y, HIGH); digitalWrite(g, LOW);  }
void carGreen(int r, int y, int g)  { digitalWrite(r, LOW);  digitalWrite(y, LOW);  digitalWrite(g, HIGH); }

// ─── Helpers para semáforo de pedestre (ânodo comum: LOW = aceso) ─────────────
void pedRed(int r, int g)   { digitalWrite(r, LOW);  digitalWrite(g, HIGH); }
void pedGreen(int r, int g) { digitalWrite(r, HIGH); digitalWrite(g, LOW);  }

void pedBlink(int r, int g, int times) {
  for (int i = 0; i < times; i++) {
    pedGreen(r, g); delay(300);
    pedRed(r, g);   delay(300);
  }
}

// ─── Aplicar estado completo de todos os semáforos ───────────────────────────
void aplicarFase(int f) {
  switch (f) {
    case 0: // NS verde / LO vermelho
      carGreen(CAR_N_RED, CAR_N_YEL, CAR_N_GRN);
      carGreen(CAR_S_RED, CAR_S_YEL, CAR_S_GRN);
      carRed  (CAR_L_RED, CAR_L_YEL, CAR_L_GRN);
      carRed  (CAR_O_RED, CAR_O_YEL, CAR_O_GRN);
      pedRed  (PED_N_RED, PED_N_GRN);
      pedRed  (PED_S_RED, PED_S_GRN);
      pedGreen(PED_L_RED, PED_L_GRN);
      pedGreen(PED_O_RED, PED_O_GRN);
      Serial.println(F("Fase 0: NS VERDE | LO VERMELHO"));
      break;

    case 1: // NS amarelo / LO vermelho
      carYellow(CAR_N_RED, CAR_N_YEL, CAR_N_GRN);
      carYellow(CAR_S_RED, CAR_S_YEL, CAR_S_GRN);
      carRed   (CAR_L_RED, CAR_L_YEL, CAR_L_GRN);
      carRed   (CAR_O_RED, CAR_O_YEL, CAR_O_GRN);
      pedRed(PED_N_RED, PED_N_GRN);
      pedRed(PED_S_RED, PED_S_GRN);
      pedRed(PED_L_RED, PED_L_GRN);
      pedRed(PED_O_RED, PED_O_GRN);
      Serial.println(F("Fase 1: NS AMARELO | LO VERMELHO"));
      break;

    case 2: // LO verde / NS vermelho
      carRed  (CAR_N_RED, CAR_N_YEL, CAR_N_GRN);
      carRed  (CAR_S_RED, CAR_S_YEL, CAR_S_GRN);
      carGreen(CAR_L_RED, CAR_L_YEL, CAR_L_GRN);
      carGreen(CAR_O_RED, CAR_O_YEL, CAR_O_GRN);
      pedGreen(PED_N_RED, PED_N_GRN);
      pedGreen(PED_S_RED, PED_S_GRN);
      pedRed  (PED_L_RED, PED_L_GRN);
      pedRed  (PED_O_RED, PED_O_GRN);
      Serial.println(F("Fase 2: LO VERDE | NS VERMELHO"));
      break;

    case 3: // LO amarelo / NS vermelho
      carRed   (CAR_N_RED, CAR_N_YEL, CAR_N_GRN);
      carRed   (CAR_S_RED, CAR_S_YEL, CAR_S_GRN);
      carYellow(CAR_L_RED, CAR_L_YEL, CAR_L_GRN);
      carYellow(CAR_O_RED, CAR_O_YEL, CAR_O_GRN);
      pedRed(PED_N_RED, PED_N_GRN);
      pedRed(PED_S_RED, PED_S_GRN);
      pedRed(PED_L_RED, PED_L_GRN);
      pedRed(PED_O_RED, PED_O_GRN);
      Serial.println(F("Fase 3: LO AMARELO | NS VERMELHO"));
      break;
  }
}

// ─── Rotina de travessia de pedestres ────────────────────────────────────────
void rotinaTravessia(int idx) {
  bool isNS = (idx == IDX_N || idx == IDX_S);

  // Só executa se o semáforo de carros correspondente está vermelho
  bool carVermelho = isNS ? (fase == 2 || fase == 3)
                          : (fase == 0 || fase == 1);
  if (!carVermelho) {
    Serial.println(F("Travessia ignorada: carros com verde nessa via"));
    return;
  }

  int rPin, gPin;
  switch (idx) {
    case IDX_N: rPin = PED_N_RED; gPin = PED_N_GRN; break;
    case IDX_S: rPin = PED_S_RED; gPin = PED_S_GRN; break;
    case IDX_L: rPin = PED_L_RED; gPin = PED_L_GRN; break;
    default:    rPin = PED_O_RED; gPin = PED_O_GRN; break;
  }

  const __FlashStringHelper* nome;
  switch (idx) {
    case IDX_N: nome = F("Norte");  break;
    case IDX_S: nome = F("Sul");    break;
    case IDX_L: nome = F("Leste");  break;
    default:    nome = F("Oeste");  break;
  }

  Serial.print(F("Travessia de pedestre: "));
  Serial.println(nome);

  pedGreen(rPin, gPin);
  delay(T_CROSSING);
  pedBlink(rPin, gPin, 5);
  pedRed(rPin, gPin);
}

// ─── Processamento de comando serial ─────────────────────────────────────────
void processarSerial() {
  if (!Serial.available()) return;
  char c = (char)Serial.read();
  while (Serial.available()) Serial.read(); // limpa buffer

  int novaFase = -1;
  switch (c) {
    // N ou S: ativa NS verde (fase 0)
    case 'N': case 'n':
    case 'S': case 's':
      novaFase = 0;
      break;
    // L ou O: ativa LO verde (fase 2)
    case 'L': case 'l':
    case 'O': case 'o':
      novaFase = 2;
      break;
    default:
      return;
  }

  if (novaFase != fase) {
    fase = novaFase;
    aplicarFase(fase);
  }
}

// ─── setup ────────────────────────────────────────────────────────────────────
void setup() {
  Serial.begin(9600);

  // Pinos de saída — semáforos de carros
  const int carPins[] = {
    CAR_N_RED, CAR_N_YEL, CAR_N_GRN,
    CAR_S_RED, CAR_S_YEL, CAR_S_GRN,
    CAR_L_RED, CAR_L_YEL, CAR_L_GRN,
    CAR_O_RED, CAR_O_YEL, CAR_O_GRN
  };
  for (int i = 0; i < 12; i++) {
    pinMode(carPins[i], OUTPUT);
    digitalWrite(carPins[i], LOW);
  }

  // Pinos de saída — semáforos de pedestres (ânodo comum: HIGH = apagado)
  const int pedPins[] = {
    PED_N_RED, PED_N_GRN,
    PED_S_RED, PED_S_GRN,
    PED_L_RED, PED_L_GRN,
    PED_O_RED, PED_O_GRN
  };
  for (int i = 0; i < 8; i++) {
    pinMode(pedPins[i], OUTPUT);
    digitalWrite(pedPins[i], HIGH);
  }

  // Botões com pull-up interno e interrupções externas
  pinMode(BTN_N, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BTN_N), isr_N, FALLING);

  pinMode(BTN_S, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BTN_S), isr_S, FALLING);

  pinMode(BTN_L, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BTN_L), isr_L, FALLING);

  pinMode(BTN_O, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(BTN_O), isr_O, FALLING);

  // Estado inicial
  fase = 0;
  aplicarFase(fase);

  Serial.println(F("=== Semáforo 4 Fases — UFRGS ==="));
  Serial.println(F("Digite N, S, L ou O para controlar as fases."));
  Serial.println(F("Pressione o botão para acionar a travessia de pedestres."));
}

// ─── loop ─────────────────────────────────────────────────────────────────────
void loop() {
  // Duração da fase: verde = T_GREEN, amarelo = T_YELLOW
  unsigned long duracao = (fase % 2 == 0) ? T_GREEN : T_YELLOW;
  unsigned long inicio  = millis();

  while (millis() - inicio < duracao) {
    processarSerial();

    // Verifica e executa requisições de travessia de pedestres
    for (int i = 0; i < 4; i++) {
      if (pedRequest[i]) {
        pedRequest[i] = false;
        rotinaTravessia(i);
      }
    }

    delay(10);
  }

  // Avança automaticamente para a próxima fase
  fase = (fase + 1) % 4;
  aplicarFase(fase);
}
