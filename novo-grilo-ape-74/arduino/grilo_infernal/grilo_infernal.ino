// GRILO INFERNAL - camada 1: atraso sorteado ao escurecer
//
// Apagou a luz: espera um tempo sorteado (20 s a 1 min) e só então começa a grilar.
// Acendeu a luz: cala na hora. Apagou de novo: sorteia outro atraso.
//
// LIGAÇÕES
//   D3  -> resistor de 1 kΩ -> pino 4 (reset) do 555 nº 3
//   A0  -> junção do resistor de 47 kΩ (aos 5 V do Nano) com o LDR (ao GND)
//   VIN -> +9 V      GND -> negativo da fonte

#define LDR_PIN   A0
#define GRILO_PIN 3

#define LIMIAR_ESCURO 600   // acima disto, é noite
#define LIMIAR_CLARO  450   // abaixo disto, é dia (a faixa entre os dois mantém o estado anterior)

#define MODO_DEMO    0      // 1 = atraso curto, para gravar e testar; 0 = tempos reais
#define DEBUG_SERIAL 1      // 1 = mostra leitura e estado no Monitor Serial (9600)

#if MODO_DEMO
  #define ATRASO_MIN 3000UL     // 3 a 6 segundos
  #define ATRASO_MAX 6000UL
#else
  #define ATRASO_MIN 20000UL    // 20 segundos a 1 minuto
  #define ATRASO_MAX 60000UL
#endif

enum Estado { DIA, ESPERANDO, CANTANDO };
Estado estado = DIA;

bool escuro = false;              // decisão dia/noite, já com a zona morta
unsigned long inicioEspera = 0;   // quando escureceu
unsigned long atraso = 0;         // atraso sorteado para esta noite

void setup() {
  pinMode(GRILO_PIN, OUTPUT);
  digitalWrite(GRILO_PIN, LOW);   // começa calado

#if DEBUG_SERIAL
  Serial.begin(9600);
#endif
}

void loop() {
  int leitura = analogRead(LDR_PIN);
  unsigned long agora = millis();

  // Dia ou noite, com os dois limiares
  if (!escuro && leitura > LIMIAR_ESCURO) escuro = true;
  else if (escuro && leitura < LIMIAR_CLARO) escuro = false;

  if (!escuro) {
    // Luz acesa: cala na hora, em qualquer estado
    digitalWrite(GRILO_PIN, LOW);
    estado = DIA;
  }
  else if (estado == DIA) {
    // Acabou de escurecer: sorteia o atraso
    randomSeed(micros());         // o instante em que a luz apagou serve de semente
    atraso = random(ATRASO_MIN, ATRASO_MAX + 1);
    inicioEspera = agora;
    estado = ESPERANDO;
  }
  else if (estado == ESPERANDO && agora - inicioEspera >= atraso) {
    // Passou o atraso: solta o grilo
    digitalWrite(GRILO_PIN, HIGH);
    estado = CANTANDO;
  }

#if DEBUG_SERIAL
  Serial.print("LDR = ");
  Serial.print(leitura);
  if (estado == DIA) {
    Serial.println("  dia, quieto");
  } else if (estado == ESPERANDO) {
    Serial.print("  noite, esperando: faltam ");
    Serial.print((atraso - (agora - inicioEspera)) / 1000);
    Serial.println(" s");
  } else {
    Serial.println("  noite, cantando");
  }
#endif

  delay(100);
}
