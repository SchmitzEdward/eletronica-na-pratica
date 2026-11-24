# Buzina Brasileira "CHAMAMUIÉ" (DCE nº 27)

Montagem e medição da Buzina Brasileira "CHAMAMUIÉ", publicada por Bêda Marques na revista *Divirta-se com a Eletrônica* nº 27 (p. 40–47), com a errata do esquema publicada na DCE nº 30 (p. 78).

O circuito tem dois osciladores e um estágio de potência:

- **Oscilador lento** (BC307 + BC548, ~4,2 Hz): a "respiração" da buzina, uma rampa no capacitor de 100 µF.
- **Oscilador de áudio** (BC548 + Darlington BC548/TIP31, ~1,7 a 2,6 kHz): o tom, que sobe e desce comandado pela tensão dessa rampa.

Ligada direto, ela soa como sirene. Em toques curtos no botão, cada toque parte do capacitor descarregado e o tom faz um "V" (de ~5 kHz até ~1,8 kHz e de volta a ~2,1 kHz): é o "fiu".

📄 **[Documento completo (PDF)](chamamuie-por-dentro.pdf)**: blocos do circuito, telas do osciloscópio, os dois regimes (sirene e fiu-fiu), contas de guardanapo e o esquema corrigido com os blocos marcados.

## Sobre o documento

Gerado por IA (Claude) a partir de diversas iterações com Edward Schmitz, durante a montagem e a medição na bancada. Serve como referência de estudo, não como prova da verdade: explicações, contas e desenhos são interpretações e estimativas, e podem conter erros. As medidas de osciloscópio são reais, feitas num Rigol DHO804.
