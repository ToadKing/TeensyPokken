byte SNES_CLOCK, SNES_LATCH, SNES_DATA;

void snes_setup(byte clock, byte latch, byte data) {
  SNES_CLOCK = clock;
  SNES_LATCH = latch;
  SNES_DATA = data;

  pinMode(SNES_CLOCK, OUTPUT);
  pinMode(SNES_LATCH, OUTPUT);
  pinMode(SNES_DATA, INPUT);

  digitalWrite(SNES_CLOCK, LOW);
  digitalWrite(SNES_LATCH, LOW);
}

uint16_t snes_read(void) {
  uint16_t btns = 0;

  digitalWrite(SNES_LATCH, HIGH);
  delayMicroseconds(12);
  digitalWrite(SNES_LATCH, LOW);
  delayMicroseconds(6);

  // read first button

  btns |= (digitalRead(SNES_DATA) << 0);
  delayMicroseconds(6);

  for (byte i = 1; i < 16; i++) {
    digitalWrite(SNES_CLOCK, HIGH);
    delayMicroseconds(6);
    btns |= (digitalRead(SNES_DATA) << i);
    digitalWrite(SNES_CLOCK, LOW);
    delayMicroseconds(6);
  }

  // last clock, doesn't correspond to a button
  digitalWrite(SNES_CLOCK, HIGH);
  delayMicroseconds(6);
  digitalWrite(SNES_CLOCK, LOW);

  return ~btns;
}
