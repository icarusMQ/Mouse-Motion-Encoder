
void PS2GoHi(uint8_t pin) {
  pinMode(pin, INPUT);
  digitalWrite(pin, HIGH);
}

void PS2GoLo(uint8_t pin) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
}

void PS2Write(uint8_t data) {
  uint8_t parity = 1;
  PS2GoHi(PS2DATA);
  PS2GoHi(PS2CLOCK);
  delayMicroseconds(250);
  PS2GoLo(PS2CLOCK);
  delayMicroseconds(250);
  PS2GoLo(PS2DATA);
  delayMicroseconds(10);
  PS2GoHi(PS2CLOCK);
  while (digitalRead(PS2CLOCK) == HIGH) {};
  for (int i = 0; i < 8; i++) {
    if (data & 0x01) PS2GoHi(PS2DATA);
    else PS2GoLo(PS2DATA);
    while (digitalRead(PS2CLOCK) == LOW) {};
    while (digitalRead(PS2CLOCK) == HIGH) {};
    parity ^= (data & 0x01);
    data = data >> 1;
  }
  if (parity) PS2GoHi(PS2DATA);
  else PS2GoLo(PS2DATA);
  while (digitalRead(PS2CLOCK) == LOW) {};
  while (digitalRead(PS2CLOCK) == HIGH) {};
  PS2GoHi(PS2DATA);
  delayMicroseconds(20);
  while (digitalRead(PS2CLOCK) == HIGH) {};
  while ((digitalRead(PS2CLOCK) == LOW) || (digitalRead(PS2DATA) == LOW)) {};
  PS2GoLo(PS2CLOCK);
}

uint8_t PS2Read(void) {
  uint8_t data = 0, bit = 1;
  PS2GoHi(PS2CLOCK);
  PS2GoHi(PS2DATA);
  delayMicroseconds(20);
  while (digitalRead(PS2CLOCK) == HIGH) {};
  delayMicroseconds(5);
  while (digitalRead(PS2CLOCK) == LOW) {};
  for (int i = 0; i < 8; i++) {
    while (digitalRead(PS2CLOCK) == HIGH) {};
    if (digitalRead(PS2DATA) == HIGH) data |= bit;
    while (digitalRead(PS2CLOCK) == LOW) {};
    bit = bit << 1;
  }
  while (digitalRead(PS2CLOCK) == HIGH) {};
  while (digitalRead(PS2CLOCK) == LOW) {};
  while (digitalRead(PS2CLOCK) == HIGH) {};
  while (digitalRead(PS2CLOCK) == LOW) {};
  PS2GoLo(PS2CLOCK);
  return data;
}

void PS2MouseInit(void) {
  PS2GoHi(PS2CLOCK);
  PS2GoHi(PS2DATA);
  delayMicroseconds(250);
  PS2Write(0xFF);
  for (int i = 0; i < 3; i++)
    PS2Read();
  PS2Write(0xF0);
  PS2Read();
  delayMicroseconds(250);
}

void PS2MouseRead(uint8_t &stat, uint8_t &x, uint8_t &y) {
  PS2Write(0xEB);
  PS2Read();
  stat = PS2Read();
  x = PS2Read();
  y = PS2Read();
}