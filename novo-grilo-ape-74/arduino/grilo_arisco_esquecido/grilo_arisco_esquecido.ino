// =====================================================================
//  GRILO ARISCO ESQUECIDO
//  Um Arduino Nano dando "comportamento de bicho" ao grilo de três 555.
//
//  O som continua sendo feito pelo circuito do NOVOGRILO, publicado na
//  revista Aprendendo & Praticando Eletrônica nº 74. O Arduino não gera
//  som nenhum: ele só decide QUANDO o grilo pode cantar.
//
//  O QUE ELE FAZ
//   1. Apagou a luz: espera um tempo sorteado antes do primeiro canto.
//   2. Canta alguns segundos, cala por um tempo sorteado, canta de novo.
//      (Som que vai e volta é muito mais difícil de localizar.)
//   3. Acendeu a luz: cala na hora e LEVA UM SUSTO, estivesse cantando ou não.
//   4. Se a luz apagar logo, ele volta ressabiado: a espera é dobrada.
//   5. Se a luz ficar acesa por 15 minutos, ele ESQUECE o susto (a essa
//      altura já deve estar vendo TV de grilo) e na próxima escuridão
//      volta tranquilo, com a espera normal.
//
//  LIGAÇÕES
//   D3  -> resistor de 1 kΩ -> pino 4 (reset) do 555 nº 3
//   A0  -> junção do resistor de 47 kΩ (aos 5 V do Nano) com o LDR (ao GND)
//   VIN -> +9 V
//   GND -> negativo da fonte (terra de verdade, não o pino 1 do 555 nº 3)
//
//  Na partida o grilo dá uns três cantos sozinho. É normal: enquanto o
//  Nano inicializa, o D3 fica solto e o reset do 555 sobe por conta própria.
// =====================================================================


// ----------------------- PINOS -----------------------
#define LDR_PIN   A0      // entrada analógica que lê o divisor do LDR
#define GRILO_PIN 3       // saída digital: HIGH = pode cantar, LOW = calado


// ---------------- DIA OU NOITE (LDR) -----------------
// A leitura vai de 0 (luz forte) a 1023 (escuro total).
// São dois limiares de propósito: entre um e outro existe uma "zona morta"
// onde o programa mantém a decisão anterior. Sem ela, na penumbra a leitura
// ficaria passeando em volta de um valor único e o grilo gaguejaria.
// É o mesmo truque dos comparadores de 1/3 e 2/3 de Vcc dentro do 555.
#define LIMIAR_ESCURO 600   // subiu acima disto: é noite
#define LIMIAR_CLARO  450   // desceu abaixo disto: é dia


// ---------------- CHAVES DE CONFIGURAÇÃO ----------------
#define MODO_DEMO    1    // 1 = tempos curtos, para testar e gravar; 0 = tempos reais
#define DEBUG_SERIAL 1    // 1 = mostra leitura e estado no Monitor Serial (9600); 0 = desliga


// ----------------------- TEMPOS -----------------------
// Todos em milissegundos. O "UL" avisa o compilador que são números grandes.
#if MODO_DEMO
  #define ATRASO_MIN    3000UL    // espera ao escurecer: 3 a 6 s
  #define ATRASO_MAX    6000UL
  #define CANTO_MIN     2000UL    // duração de cada canto: 2 a 4 s
  #define CANTO_MAX     4000UL
  #define SILENCIO_MIN  2000UL    // silêncio entre cantos: 2 a 6 s
  #define SILENCIO_MAX  6000UL
  #define TEMPO_ESQUECE 20000UL   // luz acesa por 20 s: esquece o susto
#else
  #define ATRASO_MIN    20000UL   // espera ao escurecer: 20 s a 1 min
  #define ATRASO_MAX    60000UL
  #define CANTO_MIN     4000UL    // duração de cada canto: 4 a 12 s
  #define CANTO_MAX     12000UL
  #define SILENCIO_MIN  8000UL    // silêncio entre cantos: 8 a 40 s
  #define SILENCIO_MAX  40000UL
  #define TEMPO_ESQUECE 900000UL  // luz acesa por 15 minutos: esquece o susto
#endif

#define FATOR_SUSTO 2     // assustado, a espera é multiplicada por este valor


// ----------------------- ESTADOS -----------------------
// O grilo está sempre em uma destas quatro situações:
enum Estado {
  DIA,          // luz acesa: calado
  ESPERANDO,    // escureceu: calado, aguardando o atraso sorteado
  CANTANDO,     // cantando por um tempo sorteado
  PAUSANDO      // calado entre dois cantos, por um tempo sorteado
};

Estado estado = DIA;

bool escuro    = false;           // decisão dia/noite, já passada pela zona morta
bool assustado = false;           // começa tranquilo: ninguém acendeu luz em cima dele ainda

unsigned long inicioFase  = 0;    // instante em que a fase atual começou
unsigned long duracaoFase = 0;    // quanto tempo a fase atual deve durar (sorteado)
unsigned long inicioLuz   = 0;    // instante em que a luz acendeu (para contar o esquecimento)


// ----------------- FUNÇÕES AUXILIARES -----------------

// Libera o reset do 555 nº 3. Os 5 V do Nano bastam, mesmo com o 555 em 9 V.
void canta() {
  digitalWrite(GRILO_PIN, HIGH);
}

// Segura o reset do 555 nº 3 em nível baixo: silêncio.
void cala() {
  digitalWrite(GRILO_PIN, LOW);
}

// Sorteia um tempo entre minimo e maximo (os dois incluídos).
unsigned long sorteia(unsigned long minimo, unsigned long maximo) {
  return random(minimo, maximo + 1);
}

// Começa uma nova fase: muda o estado, guarda a duração e marca a hora.
void iniciaFase(Estado novoEstado, unsigned long duracao) {
  estado      = novoEstado;
  duracaoFase = duracao;
  inicioFase  = millis();
}

// A fase atual já durou o que devia?
bool faseTerminou() {
  return millis() - inicioFase >= duracaoFase;
}


// ----------------------- SETUP -----------------------
void setup() {
  pinMode(GRILO_PIN, OUTPUT);
  cala();                         // começa calado

#if DEBUG_SERIAL
  Serial.begin(9600);
#endif
}


// ----------------------- LOOP -----------------------
void loop() {

  // 1) Lê o LDR e decide se é dia ou noite, respeitando a zona morta.
  int leitura = analogRead(LDR_PIN);

  if (!escuro && leitura > LIMIAR_ESCURO) {
    escuro = true;                // acabou de escurecer
  }
  else if (escuro && leitura < LIMIAR_CLARO) {
    escuro = false;               // acabou de clarear...
    assustado = true;             // ...e toda luz que acende é um susto,
    inicioLuz = millis();         // com hora marcada para ser esquecido
  }

  // 2) Luz acesa: calado, e contando o tempo para esquecer o susto.
  if (!escuro) {
    cala();
    estado = DIA;

    if (assustado && millis() - inicioLuz >= TEMPO_ESQUECE) {
      assustado = false;          // esqueceu: foi ver TV de grilo
    }
  }

  // 3) Está escuro: age conforme a fase em que se encontra.
  else {
    switch (estado) {

      case DIA: {
        // Acabou de escurecer. O instante exato em que a luz apagou serve de
        // semente do sorteio: como ninguém aperta o interruptor duas vezes
        // no mesmo microssegundo, a sequência nunca se repete.
        randomSeed(micros());

        unsigned long atraso = sorteia(ATRASO_MIN, ATRASO_MAX);
        if (assustado) {
          atraso = atraso * FATOR_SUSTO;    // ressabiado: demora mais para voltar
        }
        iniciaFase(ESPERANDO, atraso);
        break;
      }

      case ESPERANDO:
        // Passou o atraso: primeiro canto da noite. Voltou a confiar.
        if (faseTerminou()) {
          assustado = false;
          canta();
          iniciaFase(CANTANDO, sorteia(CANTO_MIN, CANTO_MAX));
        }
        break;

      case CANTANDO:
        // Cantou o bastante: cala e sorteia o tamanho do silêncio.
        if (faseTerminou()) {
          cala();
          iniciaFase(PAUSANDO, sorteia(SILENCIO_MIN, SILENCIO_MAX));
        }
        break;

      case PAUSANDO:
        // Acabou o silêncio: volta a cantar por outro tempo sorteado.
        if (faseTerminou()) {
          canta();
          iniciaFase(CANTANDO, sorteia(CANTO_MIN, CANTO_MAX));
        }
        break;
    }
  }

  // 4) Relatório no Monitor Serial, para acompanhar o que o bicho está pensando.
#if DEBUG_SERIAL
  Serial.print("LDR = ");
  Serial.print(leitura);

  if (estado == DIA) {
    if (assustado) {
      unsigned long passou = millis() - inicioLuz;
      unsigned long faltam = (passou < TEMPO_ESQUECE) ? (TEMPO_ESQUECE - passou) / 1000 : 0;
      Serial.print("  dia, assustado: esquece em ");
      Serial.print(faltam);
      Serial.println(" s");
    } else {
      Serial.println("  dia, tranquilo");
    }
  } else {
    unsigned long passou = millis() - inicioFase;
    unsigned long faltam = (passou < duracaoFase) ? (duracaoFase - passou) / 1000 : 0;

    if (estado == ESPERANDO) {
      Serial.print(assustado ? "  noite, esperando (assustado)" : "  noite, esperando");
    } else if (estado == CANTANDO) {
      Serial.print("  noite, cantando");
    } else {
      Serial.print("  noite, em silencio");
    }
    Serial.print(": faltam ");
    Serial.print(faltam);
    Serial.println(" s");
  }
#endif

  delay(100);   // dez leituras por segundo são mais que suficientes
}
