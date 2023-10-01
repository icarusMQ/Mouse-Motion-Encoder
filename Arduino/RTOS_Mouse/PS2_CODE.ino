
void PS2GoHi(int pin) {
  pinMode(pin, INPUT);
  digitalWrite(pin, HIGH);
}

void PS2GoLo(int pin) {
  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
}

void PS2Write(unsigned char data) {
  unsigned char parity = 1;
  PS2GoHi(PS2DATA);
  PS2GoHi(PS2CLOCK);
  delayMicroseconds(300);
  PS2GoLo(PS2CLOCK);
  delayMicroseconds(300);
  PS2GoLo(PS2DATA);
  delayMicroseconds(10);
  PS2GoHi(PS2CLOCK);
  while (digitalRead(PS2CLOCK) == HIGH)
    ;
  for (int i = 0; i < 8; i++) {
    if (data & 0x01) PS2GoHi(PS2DATA);
    else PS2GoLo(PS2DATA);
    while (digitalRead(PS2CLOCK) == LOW)
      ;
    while (digitalRead(PS2CLOCK) == HIGH)
      ;
    parity ^= (data & 0x01);
    data = data >> 1;
  }
  if (parity) PS2GoHi(PS2DATA);
  else PS2GoLo(PS2DATA);
  while (digitalRead(PS2CLOCK) == LOW)
    ;
  while (digitalRead(PS2CLOCK) == HIGH)
    ;
  PS2GoHi(PS2DATA);
  delayMicroseconds(50);
  while (digitalRead(PS2CLOCK) == HIGH)
    ;
  while ((digitalRead(PS2CLOCK) == LOW) || (digitalRead(PS2DATA) == LOW))
    ;
  PS2GoLo(PS2CLOCK);
}

unsigned char PS2Read(void) {
  unsigned char data = 0, bit = 1;
  PS2GoHi(PS2CLOCK);
  PS2GoHi(PS2DATA);
  delayMicroseconds(50);
  while (digitalRead(PS2CLOCK) == HIGH)
    ;
  delayMicroseconds(5);
  while (digitalRead(PS2CLOCK) == LOW)
    ;
  for (int i = 0; i < 8; i++) {
    while (digitalRead(PS2CLOCK) == HIGH)
      ;
    if (digitalRead(PS2DATA) == HIGH) data |= bit;
    while (digitalRead(PS2CLOCK) == LOW)
      ;
    bit = bit << 1;
  }
  while (digitalRead(PS2CLOCK) == HIGH)
    ;
  while (digitalRead(PS2CLOCK) == LOW)
    ;
  while (digitalRead(PS2CLOCK) == HIGH)
    ;
  while (digitalRead(PS2CLOCK) == LOW)
    ;
  PS2GoLo(PS2CLOCK);
  return data;
}

void PS2MouseInit(void) {
  PS2GoHi(PS2CLOCK);
  PS2GoHi(PS2DATA);
  delayMicroseconds(100);
  PS2Write(0xFF);
  for (int i = 0; i < 3; i++) PS2Read();
  PS2Write(0xF0);
  PS2Read();
  delayMicroseconds(100);
}

void PS2MousePos(char &stat, char &x, char &y) {
  PS2Write(0xEB);
  PS2Read();
  stat = PS2Read();
  x = PS2Read();
  y = PS2Read();
}