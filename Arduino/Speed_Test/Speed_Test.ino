// Mouse pinouts
#define PS2CLOCK 5
#define PS2DATA 6

// Mouse definitions
uint32_t d_time = 0;
uint8_t buttons;
uint8_t dx;
uint8_t dy;

// Sensors definitions
#define SENSOR1 A0
#define SENSOR2 A1
#define SENSOR3 A2
#define SENSOR4 A3

uint16_t S1;
uint16_t S2;
uint16_t S3;
uint16_t S4;


// Inicio do código
void setup(void) {
  Serial.begin(115200);

  PS2MouseInit();
  Serial.println("Mouse Ready");

  pinMode(SENSOR1, INPUT);
  pinMode(SENSOR2, INPUT);
  pinMode(SENSOR3, INPUT);
  pinMode(SENSOR4, INPUT);
  Serial.println("Sensores configurados");
}

uint32_t calc_time = 0;

void loop(void) {

  d_time = micros();
  PS2MouseRead(buttons, dx, dy);
  calc_time = micros() - d_time;
  Serial.print("Tempo de leitura MOUSE (us):\t");
  Serial.println(calc_time);

  d_time = micros();
  S1 = map( analogRead(SENSOR1), 0, 1024, 0, 100);
  S2 = map( analogRead(SENSOR2), 0, 1024, 0, 100);
  S3 = map( analogRead(SENSOR3), 0, 1024, 0, 100);
  S4 = map( analogRead(SENSOR4), 0, 1024, 0, 100);
  calc_time = micros() - d_time;
  Serial.print("Tempo de leitura SENSORES (us):\t");
  Serial.println(calc_time);
  delay(250);
}
