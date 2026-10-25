# Alterando o Grilo com IA

Este arquivo serve para você continuar mexendo nos programas do grilo conversando com uma inteligência artificial (Claude, ChatGPT, Gemini ou outra), mesmo sem ser programador. Foi assim que os programas originais nasceram: as ideias, os pinos e os testes de bancada vieram de quem montou o circuito, e a IA ajudou a transformar tudo em código comentado.

## Como usar

1. Abra uma conversa nova na IA de sua preferência.
2. Envie este arquivo (`PROMPT-IA.md`) e o `.ino` que você quer alterar. Se quiser que a IA compare versões, envie todos.
3. Copie o prompt abaixo, complete o final com o que você quer mudar e envie.
4. Grave o programa novo, teste na bancada e conte para a IA o que aconteceu. Peça um ajuste por vez: é mais fácil descobrir o que funcionou.
5. Antes de gravar, clique em **Verificar** na IDE do Arduino. Se aparecer erro, copie a mensagem inteira e cole na conversa.

## O prompt

Copie tudo o que está dentro do quadro:

```
Você vai me ajudar a modificar um programa para Arduino Nano que controla um
grilo eletrônico. Leia com atenção o contexto antes de propor qualquer mudança.

O CIRCUITO
- O som é gerado pelo "Novogrilo", circuito de Bêda Marques publicado na revista
  Aprendendo e Praticando Eletrônica nº 74: três CI 555 em cascata, alimentados
  com 9 V. O primeiro (lento) liga e desliga o segundo, o segundo liga e desliga
  o terceiro, e o terceiro gera um agudo de uns 5 kHz que vai para o transdutor.
- O Arduino NÃO gera som. Ele só controla o pino 4 (reset) do terceiro 555:
  HIGH = o grilo canta, LOW = o grilo cala.

A LIGAÇÃO (não mude sem me avisar)
- D3 -> resistor de 1 kΩ -> pino 4 do terceiro 555.
- A0 -> divisor resistivo: resistor de 47 kΩ ligado aos 5 V do Nano e LDR ligado
  ao GND. Leitura alta (perto de 1023) = escuro; leitura baixa = claro.
- VIN -> +9 V da bateria. GND -> negativo da bateria (nunca o pino 1 de um 555).

REGRAS QUE O PROGRAMA DEVE RESPEITAR
- Detecção de dia e noite com histerese: dois limiares (hoje LIMIAR_ESCURO 600
  e LIMIAR_CLARO 450), com zona morta entre eles para o grilo não gaguejar na
  penumbra.
- Nada de delay() longo: use millis() para os tempos, para o grilo reagir na
  hora quando a luz acender.
- Tempos em #define no topo do arquivo, em milissegundos.
- Manter as chaves MODO_DEMO (tempos curtos para teste) e DEBUG_SERIAL
  (mensagens no Monitor Serial a 9600 baud).
- Código comentado em português, explicando o porquê de cada parte, para
  alguém que entende de eletrônica mas não é programador.
- Ao ligar, o grilo dá duas ou três griladinhas enquanto o Nano inicializa.
  Isso é esperado e não precisa ser corrigido.

COMO EU QUERO QUE VOCÊ TRABALHE
- Antes de escrever código, explique em poucas frases o que vai mudar e por quê.
- Entregue o programa inteiro, pronto para colar na IDE do Arduino.
- Se a mudança exigir alterar a ligação ou acrescentar componentes, diga
  claramente quais, onde ligar e por quê.
- Se a minha ideia tiver algum problema elétrico (risco para o Arduino ou para
  os 555), me avise antes.
- Ao final, sugira um roteiro curto de teste na bancada.

Em anexo estão os programas atuais. O que eu quero mudar agora é:
[DESCREVA AQUI A SUA IDEIA]
```

## Ideias para começar

Algumas brincadeiras que cabem no mesmo circuito, só mexendo no programa:

- **Susto acumulativo:** cada vez que a luz acende, a espera aumenta (dobro, triplo, quádruplo...) até um teto. Quanto mais a vítima procura, mais o grilo demora, e ela começa a achar que ganhou.
- **Grilo madrugador:** ele só começa a cantar depois de um tempo longo de escuridão contínua, quando a casa já está dormindo.
- **Grilo de horário:** canta mais no começo da noite e vai sossegando com o passar das horas.
- **Grilo econômico:** colocar o Nano para dormir entre as leituras, para a bateria durar mais. (Peça à IA para explicar as limitações da placa Nano nesse modo.)
- **Botão de pânico:** um botão em outro pino que cala o grilo por uma hora, para quando a pegadinha já foi longe demais.

## Um aviso honesto

A IA erra. Às vezes inventa uma função que não existe, troca um pino ou esquece uma regra. Por isso o roteiro é sempre o mesmo: verificar na IDE, testar na bancada com o `MODO_DEMO` ligado e o Monitor Serial aberto, e só então partir para os tempos reais. O ferro de solda continua sendo o juiz final.
