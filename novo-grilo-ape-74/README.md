# Novo Grilo — APE nº 74

Material complementar dos vídeos publicados no canal Edward Schmitz.

O **Novogrilo** é o grilo eletrônico de Bêda Marques publicado na revista *Aprendendo e Praticando Eletrônica* nº 74, de 1995. São três 555 empilhados: o primeiro, bem lento, liga e desliga o segundo; o segundo liga e desliga o terceiro; e o terceiro gera o agudo de uns 5 kHz que vai para o transdutor. Do encadeamento sai o cri-cri.

Esta pasta reúne o material dos dois episódios da série:

1. **O grilo original**, montado em breadboard e medido no osciloscópio, estágio por estágio.
2. **O grilo com Arduino**, em que um Arduino Nano passa a decidir *quando* o bicho canta: só no escuro, em intervalos sorteados, e com direito a susto quando alguém acende a luz.

## Arquivos

```
novo-grilo-ape-74/
├── README.md                          ← este arquivo
├── novogrilo-relatorio-do-video.pdf   ← medições do grilo original (episódio 1)
├── PROMPT-IA.md                       ← prompt para continuar alterando os programas com IA
└── arduino/
    ├── grilo_simples/
    │   └── grilo_simples.ino
    ├── grilo_infernal/
    │   └── grilo_infernal.ino
    ├── grilo_arisco/
    │   └── grilo_arisco.ino
    └── grilo_arisco_esquecido/
        └── grilo_arisco_esquecido.ino ← a versão mostrada no vídeo
```

Cada programa fica numa pasta com o mesmo nome porque a IDE do Arduino exige isso para abrir o arquivo.

## Vídeos

- Episódio 1, o grilo original: https://youtu.be/Arfrq9QXHxc
- Episódio 2, o grilo com Arduino: https://youtu.be/ggpLcA16KN4

---

# Grilo + Arduino

O som continua sendo todo do circuito do Bêda. O Arduino não gera som nenhum: ele só segura ou solta o reset do último 555. É a pergunta que guiou o projeto: o que Bêda faria com esse grilo se tivesse um Arduino na gaveta? A resposta abaixo é uma brincadeira nossa, não algo que esteja na revista.

## Os quatro programas

Eles formam uma escada. Cada um acrescenta uma camada ao anterior, então dá para estudar na ordem e parar no degrau que interessar.

| Programa                 | O que faz                                                                                                                   |
| ------------------------ | --------------------------------------------------------------------------------------------------------------------------- |
| `grilo_simples`          | Escureceu, canta. Clareou, cala. Serve para testar a ligação e calibrar o LDR.                                              |
| `grilo_infernal`         | Ao escurecer, espera um tempo sorteado antes de começar a cantar.                                                           |
| `grilo_arisco`           | Canta e cala em intervalos sorteados; leva susto se for pego cantando.                                                      |
| `grilo_arisco_esquecido` | Toda luz que acende assusta; se a luz ficar acesa por 15 minutos, ele esquece o susto. **É o programa explicado no vídeo.** |

## A ligação

É a mesma para os quatro programas.

| Pino do Nano | Vai para                                              |
| ------------ | ----------------------------------------------------- |
| D3           | resistor de 1 kΩ, e daí ao pino 4 (reset) do 555 nº 3 |
| A0           | junção do resistor de 47 kΩ com o LDR                 |
| 5V           | a outra ponta do resistor de 47 kΩ                    |
| GND          | a outra ponta do LDR e o negativo da fonte            |
| VIN          | +9 V                                                  |

```
   5 V (do Nano)                         D3 ──[1 kΩ]── pino 4 do 555 nº 3
        │
     [47 kΩ]
        │
        ├────── A0
        │
      [LDR]
        │
       GND
```

O 555 nº 3 é o último da cascata, o que aciona o transdutor. A ligação que antes levava o pino 4 dele ao positivo sai do circuito: agora quem manda nesse pino é o Arduino.

**Peças além do grilo já montado:** um Arduino Nano, um resistor de 1 kΩ, um resistor de 47 kΩ e um LDR.

O mesmo circuito foi montado também em placa padrão (aquela placa perfurada com trilhas, que funciona como uma breadboard soldada), com uma barra de pinos para encaixar o Nano.

### Como o Arduino manda no grilo

O pino 4 do 555 é o reset. Em nível alto o CI oscila; em nível baixo, cala.

- `digitalWrite(3, HIGH)`: o grilo canta. Os 5 V do Nano bastam para liberar o reset, mesmo com o 555 alimentado em 9 V.
- `digitalWrite(3, LOW)`: o grilo cala.

### Cuidados que a bancada ensinou

1. **O pull-up de 47 kΩ vai nos 5 V do Nano, não nos 9 V.** A entrada analógica não foi feita para passar de 5 V. Com os 9 V o Nano até sobrevive, porque os 47 kΩ limitam a corrente no diodo de proteção do pino, mas a leitura bate no teto e passa a depender da carga da bateria.
2. **O GND do Nano vai ao negativo da fonte**, nunca ao pino 1 do 555 nº 3. Nesse circuito o pino 1 dos estágios seguintes é comandado pelo estágio anterior e não é terra de verdade.
3. **O resistor de 1 kΩ em série é obrigatório.** Ligar o pino 4 direto, sem resistor, já custou dois CIs na bancada.
4. **O circuito precisa da fonte de 9 V.** Só com o USB do computador o grilo fica mal alimentado e canta sem parar. USB e bateria podem ficar ligados juntos, o que é útil para acompanhar o Monitor Serial.

## Dia ou noite: os dois limiares

O A0 transforma a tensão do divisor num número de 0 a 1023. Com o LDR do lado do terra, luz forte dá número baixo e escuro dá número alto.

```
1023 ┤ escuro total
     │
     │   zona de CANTAR
 600 ┤─────────────── LIMIAR_ESCURO: passou para cima, é noite
     │   zona morta (mantém a decisão anterior)
 450 ┤─────────────── LIMIAR_CLARO: passou para baixo, é dia
     │   zona de CALAR
     │
   0 ┤ luz forte
```

São dois limiares, e não um, por causa da penumbra. Com um valor único, a leitura ficaria tremendo em volta dele e o grilo gaguejaria, ligando e desligando várias vezes por segundo. Com a zona morta no meio, a leitura precisa atravessar 150 pontos para mudar a decisão. O nome técnico é histerese, e é o mesmo truque que o próprio 555 usa por dentro, com seus comparadores em 1/3 e 2/3 da alimentação.

**Para calibrar:** grave o `grilo_simples`, abra o Monitor Serial em 9600, anote a leitura com a luz acesa e com o LDR tampado, e ajuste os dois limiares para ficarem entre esses valores, com folga de cada lado. Cada LDR e cada ambiente pedem números um pouco diferentes.

## O programa do vídeo: `grilo_arisco_esquecido`

A ideia é um grilo que dá trabalho para ser achado:

- **Apagou a luz:** ele espera um tempo sorteado antes do primeiro canto. Ninguém apaga a luz e já ouve grilo; ele espera a vítima deitar e se ajeitar.
- **Canta alguns segundos e cala**, depois volta, sempre com tempos sorteados. Som que vai e volta é muito mais difícil de localizar.
- **Acendeu a luz:** ele cala na hora e leva um susto, estivesse cantando ou não.
- **Apagou logo em seguida:** ele volta ressabiado e demora o dobro para cantar. A pessoa acha que acabou, relaxa... e aí ele canta.
- **Luz acesa por 15 minutos:** ele conclui que a pessoa foi fazer outra coisa e esquece o susto. Na próxima escuridão volta tranquilo, com a espera normal.
- **Ao ligar o circuito**, começa tranquilo, porque ninguém acendeu luz em cima dele ainda.

O grilo está sempre em um de quatro estados:

```
            escureceu                 passou o atraso
   DIA ───────────────► ESPERANDO ───────────────────► CANTANDO ◄──┐
    ▲                                                     │        │
    │                                        cantou o     │        │ acabou o
    │   acendeu a luz (em qualquer estado)   bastante     ▼        │ silêncio
    └─────────────────────────────────────────────────  PAUSANDO ──┘
```

O Monitor Serial mostra o que o bicho está "pensando": a leitura do LDR, o estado e quanto falta para a próxima mudança, inclusive a contagem regressiva do susto (`dia, assustado: esquece em 742 s`, depois `dia, tranquilo`).

### Os outros degraus da escada

- **`grilo_infernal`:** só o atraso sorteado. Escureceu, espera de 20 s a 1 min e começa a cantar sem parar. Acendeu a luz, cala.
- **`grilo_arisco`:** acrescenta as pausas e o susto, mas só se assusta se for pego **depois** de já ter cantado. O susto passa quando ele volta a cantar.

## Tempos

| Parâmetro                 | Infernal     | Arisco             | Arisco Esquecido    |
| ------------------------- | ------------ | ------------------ | ------------------- |
| Espera ao escurecer       | 20 s a 1 min | 20 s a 1 min       | 20 s a 1 min        |
| Duração do canto          | contínuo     | 4 a 12 s           | 4 a 12 s            |
| Silêncio entre cantos     | não tem      | 8 a 40 s           | 8 a 40 s            |
| Espera depois de um susto | não tem      | o dobro            | o dobro             |
| Para esquecer o susto     | não tem      | até cantar de novo | 15 min de luz acesa |

Todos os tempos estão em `#define` no começo de cada arquivo, em milissegundos. Mude à vontade.

## As chaves de configuração

| Chave          | Valor 0                     | Valor 1                                          |
| -------------- | --------------------------- | ------------------------------------------------ |
| `MODO_DEMO`    | tempos reais (tabela acima) | tempos de poucos segundos, para testar e gravar  |
| `DEBUG_SERIAL` | sem mensagens               | mostra leitura e estado no Monitor Serial (9600) |

No modo demo do Arisco Esquecido a espera cai para 3 a 6 s, o canto para 2 a 4 s e o esquecimento para 20 s. Foi assim que ele rodou no vídeo. O `grilo_simples` só tem o `DEBUG_SERIAL`.

A escolha é feita na hora de compilar: muda-se o número e grava-se de novo.

## Coisas que parecem defeito e não são

**O grilo dá duas ou três griladinhas ao ligar.** Enquanto o Nano inicializa, o D3 fica solto, e o reset do 555 sobe sozinho. Assim que o programa assume, ele percebe a luz e cala o bicho. O mesmo acontece ao apertar o reset do Nano, ao conectar o cabo USB e ao abrir o Monitor Serial, porque em todos esses casos o Nano reinicia. Dá para acompanhar pelos LEDs da placa e serve de autoteste.

**O LED de alimentação do Nano fica aceso no escuro.** Um pedaço de fita isolante preta resolve. Para uma pegadinha, ajuda.

## Dicas de montagem

- **Use três 555 iguais**, do mesmo fabricante e com o mesmo código. Misturar CIs diferentes pode deixar o comportamento estranho.
- **Trocar os 555 de posição pode mudar o timbre.** Cada CI tem suas tolerâncias, e as frequências de cada estágio mudam um pouco. Vale brincar de trocar e escolher a voz que mais agradar.
- **Transdutor:** o alto-falantinho de placa-mãe de PC (aquele do conector "SPEAKER", com uns 16 Ω) funcionou melhor que a cápsula piezo nos testes.
- **Resistor em série com o alto-falantinho:** de 27 a 47 Ω. Baixa um pouco o volume (à noite qualquer cri-cri parece um alarme), limita a corrente do 555 e alivia a bateria.

## Limitações

O Nano gasta perto de 20 mA mesmo com o grilo calado. Com uma bateria de 9 V comum, a autonomia fica na casa de um dia. Para deixar o grilo de plantão por mais tempo, use fonte ou um pack maior. Baixar esse consumo (modo de sono, placa sem LED e sem regulador) fica como desafio para a próxima versão.

## Mexendo nos programas com IA

Os programas foram escritos com a ajuda do Claude, a inteligência artificial da Anthropic, a partir das instruções e dos testes de bancada de Edward Schmitz, e são todos comentados linha a linha. O arquivo [`PROMPT-IA.md`](PROMPT-IA.md) traz um prompt pronto para você continuar alterando o grilo com o Claude ou com outra IA: basta enviar o arquivo e os programas e começar a conversar.

---

## Créditos

- **Circuito do grilo:** Novogrilo, de Bêda Marques, revista *Aprendendo e Praticando Eletrônica* nº 74 (1995).
- **Controle com Arduino, testes e documentação:** Edward Schmitz, com programas desenvolvidos com auxílio do Claude (Anthropic).

Os nomes "Grilo Infernal", "Grilo Arisco" e "Grilo Arisco Esquecido" são destes programas, em homenagem ao jeito como Bêda batizava seus projetos, e não constam da publicação original.

## Observação

Este material foi preparado por Edward Schmitz para fins educativos. A publicação original pertence aos respectivos detentores de seus direitos.
