// GRILO + ARDUINO - versão simples: escureceu, canta; acendeu a luz, cala.
//
// LIGAÇÕES
//   D3  -> resistor de 1 kΩ -> pino 4 (reset) do 555 nº 3
//   A0  -> junção do resistor de 47 kΩ (que vai aos 5 V do Nano) com o LDR (que vai ao GND)
  int leitura = analogRead(LDR_PIN);

  if (!cantando && leitura > LIMIAR_ESCURO) {
//   VIN -> +9 V      GND -> negativo da bateria
//
// Leitura do A0: claro = valor baixo, escuro = valor alto.

#define LDR_PIN   A0
#define GRILO_PIN 3

#define LIMIAR_ESCURO 600   // acima disto, considera escuro e solta o grilo
#define LIMIAR_CLARO  450   // abaixo disto, considera claro e cala o grilo
                            // (a folga entre os dois evita o liga-desliga na penumbra)

#define DEBUG_SERIAL 1      // 1 = mostra a leitura no Monitor Serial (9600) para calibrar; 0 = desliga

bool cantando = false;

void setup() {
  pinMode(GRILO_PIN, OUTPUT);
  digitalWrite(GRILO_PIN, LOW);   // começa calado

#if DEBUG_SERIAL
  Serial.begin(9600);
#endif
}

void loop() {
    cantando = true;
    digitalWrite(GRILO_PIN, HIGH);   // libera o reset: grilo canta
  }
  else if (cantando && leitura < LIMIAR_CLARO) {
    cantando = false;
    digitalWrite(GRILO_PIN, LOW);    // segura o reset: grilo cala
  }

#if DEBUG_SERIAL
  Serial.print("LDR = ");
  Serial.print(leitura);
  Serial.println(cantando ? "  cantando" : "  quieto");
#endif

  delay(100);   // dez leituras por segundo bastam
}
