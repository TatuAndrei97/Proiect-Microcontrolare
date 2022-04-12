 
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




/* Pt WIFI
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
const char* ssid     = "Tenda_3E3D48"; // nume si parola retelei wifi
const char* password = "12345678";
// Setare web server port la 80
WiFiServer server(80);

String header;
String output15State = "OPRIT";
// timp curent
unsigned long currentTime = millis();
// timp precedent
unsigned long previousTime = 0; 
// Definire timeout time in millisecunde ( 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
    Serial.begin(115200);
// Conectare  la  Wi-Fi cu SSID si password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  //afisare IP  si pornire server  
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();



//Ori in task2 ori in alt task

WiFiClient client = server.available();   // scanare clienti ce intra

  if (client) {                             // daca exista un nou client 
    Serial.println("New Client.");          // printeaza mesajul 
    String currentLine = "";                // creare sir cu date despre client
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // cat timp clientul este conectat se realizeaza
      currentTime = millis();         
      if (client.available()) {             // daca exista biti de citit 
        char c = client.read();             // cirere bit
        Serial.write(c);                    // afisare
        header += c;
        if (c == '\n') {                    // daca bitu este caracter nou
          // daca linia curenta este goala, exista 2 caractere noi la rand
          //Sfarsitul clientului HTTP,trimitere un raspuns
          if (currentLine.length() == 0) {
            // Anteturile HTTP încep întotdeauna cu un cod de răspuns (de exemplu, HTTP / 1.1 200 OK)
            // și un tip de conținut, astfel încât clientul să știe ce urmează, apoi o linie goală:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            }
            
  // Afisare pagina web
            client.println("<!DOCTYPE html><html>"); // declarere faptului ca este un document html
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS  design-ul pentru pagina web si buton
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #77878A;}</style></head>");
            
            // Titlu pagina web
            client.println("<body><h1>Tatu Andrei Proiect Intersectie</h1>");
            
            // AFisare buton de regenerare a functionarii normale a semaforului
            client.println("<p>Traficul este: " + output15State + "</p>");
            // daca traficul este oprit   
           if(output15State=="OPRIT") {
              client.println("<p><a href=\"/2/on\"><button class=\"button\">REPORNESTE</button></a></p>");
               Se executa TaskBlick2
               
                
             
           } else {
              client.println("<p><a href=\"/2/OPRIT\"><button class=\"button button2\">OPRIT</button></a></p>");
              
                 
            } 
              
            client.println();
            // iesire din bucla
            break;
          } else { // dacă ai o nouă linie, atunci șterge Linia curentă
            currentLine = "";
          }
        } else if (c != '\r') {  
          currentLine += c;      // adaugare la final de linie curenta
        }
      }
    }
    // Curatare variabile header
    header = "";
    //inchidere conexiune
  
    client.stop();
    
    Serial.println("Client disconnected.");
    Serial.println("");
  }






*/
