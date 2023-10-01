#include <Arduino_FreeRTOS.h>
#include <queue.h>
#include <task.h>
#include <semphr.h>

// Macrodefinições para debug
#define RTOS_SERIAL true

// PINOUTS
#define PS2CLOCK 5
#define PS2DATA 6


// Criação da task de leitura periodica do mouse
void task_read_mouse(void *pvParameters);
void task_read_sensor(void *pvParameters);


// Criação das Queues
struct mouse_struct {
  uint8_t buttons;
  uint8_t dx;
  uint8_t dy;
} Mouse_data;

struct position_struct {
  uint16_t x;
  uint16_t y;
} Position_data;

QueueHandle_t xQueue_mouse_data;
QueueHandle_t xQueue_position_data;


// Criação dos semáfaros
#if RTOS_SERIAL
SemaphoreHandle_t xSerial;
#endif

// Inicio do código
void setup(void) {

  // Instaciamento do semáfaro da serial
  Serial.begin(115200);
  xSerial = xSemaphoreCreateMutex();
  if (xSerial != NULL)
    Serial.println("Semaphore was been created");

  // Instanciamento das Queue/Filas para compartilhamento dos valores de mouse e sensores
  xQueue_mouse_data = xQueueCreate(1, sizeof(mouse_struct));
  xQueue_position_data = xQueueCreate(1, sizeof(position_struct));


  // Tarefas de leitura do mouse
  xTaskCreate(
    task_read_mouse,  // address memory pointer to the task function
    "Mouse task",     // task name
    2048,             // stack size in words (10bits)
    10,               // &red_led_gpio -> Struct that works like a pointer to parameters
    1,                // priority (1 is the higher)
    NULL              // out pointer to task handle
  );

  // Tarefas de leitura do mouse
  xTaskCreate(
    task_read_sensor,  // address memory pointer to the task function
    "Sensor task",     // task name
    2048,              // stack size in words (10bits)
    50,                // &red_led_gpio -> Struct that works like a pointer to parameters
    1,                 // priority (1 is the higher)
    NULL               // out pointer to task handle
  );
}


void loop(void) {}


void task_read_mouse(void *pvParameters) {
  TickType_t loop_time = *((TickType_t *)pvParameters);

  while (1) {
// Inicia a comunicação com o Mouse
#if RTOS_SERIAL
    if (xSemaphoreTake(xSerial, 10) == pdTRUE) {
      Serial.println(pcTaskGetName(NULL));
      Serial.println("Iniciando o mouse...");
      PS2MouseInit();
      Serial.println("Mouse Ready");
      xSemaphoreGive(xSerial);
    }
#else
    PS2MouseInit();
#endif

    // Estruturas de dados
    position_struct Position;
    mouse_struct Mouse;

    // Loop de leitura dos sensores
    while (true) {
      // Envia a solicitação de leitura dos registradores 0xEB
      PS2Write(0xEB);
      // Confirma recebimento
      PS2Read();
      // Armazena os valores lidos
      Mouse.buttons = PS2Read();
      Mouse.dx = PS2Read();
      Mouse.dy = PS2Read();

      // Use xQueueOverwrite para atualizar a fila (se a fila já contiver dados)
      if (uxQueueMessagesWaiting(xQueue_mouse_data) > 0) {
        xQueueOverwrite(xQueue_mouse_data, &Mouse);
        // Se a fila estiver vazia, adicione os dados a ela
      } else {
        xQueueSend(xQueue_mouse_data, &Mouse, portMAX_DELAY);
      }

#if RTOS_SERIAL
      xSemaphoreTake(xSerial, portMAX_DELAY);
      Serial.print(Mouse.buttons, BIN);
      Serial.print("\tdelta X=");
      Serial.print(Mouse.dx, DEC);
      Serial.print("\tdelta Y=");
      Serial.println(Mouse.dy, DEC);
      xSemaphoreGive(xSerial);
#endif

      // Delay da tasks de leitura do sensor
      vTaskDelay(pdMS_TO_TICKS(loop_time));
    }
  }
}


void task_read_sensor(void *pvParameters) {
  TickType_t loop_time = *((TickType_t *)pvParameters);
  while (true) {
#if RTOS_SERIAL
    if (xSemaphoreTake(xSerial, 10) == pdTRUE) {
      Serial.println(pcTaskGetName(NULL));
    }
#endif
    // Delay da tasks de leitura do sensor
    vTaskDelay(pdMS_TO_TICKS(loop_time));
  }
}
