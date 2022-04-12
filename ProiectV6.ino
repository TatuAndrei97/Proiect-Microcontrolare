 
#include <Arduino_FreeRTOS.h>
void TaskBlink1( void *pvParameters );
void TaskBlink2( void *pvParameters );

const int buttonPin = 6;     // Pinul pentru button
const int ledPin =  5;      // pinul pentru ledul de advertisment

//int lastButtonState = 0;        // variabila de citire status button

TaskHandle_t TaskHandle_1;
TaskHandle_t TaskHandle_2;

void setup()
{

  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT);
  pinMode(13, OUTPUT); //Rosu R
  pinMode(3, OUTPUT); // Verde S
  pinMode(12, OUTPUT);// Galben R
  pinMode(2, OUTPUT); //Galben S
  pinMode(11, OUTPUT);//Verde R
  pinMode(4, OUTPUT); //Rosu S
 

  Serial.begin(9600);


  xTaskCreate(TaskBlink1, "task1", 128, NULL, 1, &TaskHandle_1 ); // creare task1+2
 
  xTaskCreate(TaskBlink2, "task2", 128, NULL, 1, &TaskHandle_2 );

  vTaskStartScheduler();



}

void loop() {
}


void TaskBlink1( void *pvParameters ) {

  while (1) {
    Serial.println("Task1"); //afisare in consola faptul ca se executa task1
    digitalWrite(13, HIGH);  // functionare semafoare intersectie in paralel
    digitalWrite(3, HIGH);
    
    vTaskDelay(700);
    digitalWrite(3, LOW);
    digitalWrite(2, HIGH);

    vTaskDelay(300);
    digitalWrite(13, LOW);
    digitalWrite(2, LOW);
    digitalWrite(11, HIGH);
    digitalWrite(4, HIGH);

    vTaskDelay(700);
    digitalWrite(11, LOW);
    digitalWrite(12, HIGH);
    digitalWrite(4, HIGH);
    
    vTaskDelay(300);
    digitalWrite(13, HIGH);
    digitalWrite(12, LOW);
    digitalWrite(3, HIGH);
    digitalWrite(4, LOW);



  }

}

void TaskBlink2(void *pvParameters)  {
  while (1) {
    if (digitalRead(buttonPin) == HIGH) { // conditii de executare a task-ului 2
      vTaskDelay(15); // software debounce
      if (digitalRead(buttonPin) == HIGH) {
        vTaskSuspend (TaskHandle_1);
        digitalWrite(13, HIGH);
        digitalWrite(4, HIGH);
        digitalWrite(12, LOW);
        digitalWrite(11, LOW);
        digitalWrite(3, LOW);
        digitalWrite(2, LOW);
        digitalWrite(ledPin, HIGH);
      }
      else {
        vTaskResume (TaskHandle_1);
        digitalWrite(ledPin, LOW);

      }
      Serial.println("Task2"); // afisare in consola faptul ca task2 a fost realizat
    }
  }

}
