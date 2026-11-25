# Buzina Brasileira "CHAMAMUIÉ" (DCE nº 27)

Montagem e medição da Buzina Brasileira "CHAMAMUIÉ", publicada por Bêda Marques na revista *Divirta-se com a Eletrônica* nº 27 (p. 40–47), com a errata do esquema publicada na DCE nº 30 (p. 78).

O circuito tem dois osciladores e um estágio de potência:

- **Oscilador lento** (BC307 + BC548, ~4,2 Hz): a "respiração" da buzina, uma rampa no capacitor de 100 µF.
- **Oscilador de áudio** (BC548 + Darlington BC548/TIP31): o tom, comandado pela tensão dessa rampa.

Em toques curtos no botão, cada toque parte do capacitor quase descarregado e o tom começa agudo (~4,4 kHz) e termina grave (~2,1 kHz): é o "fiu". Ligada direto, ela soa como alarme.

📄 **[Documento completo (PDF)](chamamuie-por-dentro.pdf)**: diagrama de blocos, o fiu-fiu medido no osciloscópio, a buzina ligada direto com a conta de guardanapo, e o esquema corrigido com os blocos marcados.

## Sobre o documento

Gerado por IA (Claude) a partir de diversas iterações com Edward Schmitz, durante a montagem e a medição na bancada. Serve como referência de estudo, não como prova da verdade: explicações, contas e desenhos são interpretações e estimativas, e podem conter erros. As medidas de osciloscópio são reais, feitas num Rigol DHO804.
