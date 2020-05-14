
#include <IRremote.h>  //Biblioteca para receptor InfraVermelho
//#include <Stepper.h>   //Biblioteca para controle do motor de passo

//const int stepsPerRevolution = 800;  // change this to fit the number of steps per revolution
// for your motor

// initialize the stepper library on pins 8 through 11:
//Stepper myStepper(stepsPerRevolution, 8, 7, 6, 5);

int IR_pin = 11;   //pino de dador IR

int last_com = 0;  //auxilia memorizando ultimo comando

int steppins[] = {8, 7, 6, 5};

int i;

IRrecv irrecv(IR_pin);

decode_results results;

void turnH() {
  Serial.println("Aumentar");
  for (i=0;i<4;i++){
    digitalWrite(steppins[i], HIGH);
    delay(10);
    digitalWrite(steppins[i], LOW);
  }
  //myStepper.step(1);
}

void turnL() {
  Serial.println("Diminuir");
  for (i=4;i>=0;i--){
    digitalWrite(steppins[i], HIGH);
    delay(10);
    digitalWrite(steppins[i], LOW);
  }
  //myStepper.step(-1);
}

void setup() {
  for (i=0;i<4;i++){
    pinMode(steppins[i],OUTPUT); // Configura os pinos como saída
  }
  Serial.begin(9600);
  // In case the interrupt driver crashes on setup, give a clue
  // to the user what's going on.
  Serial.println("Enabling IRin");
  irrecv.enableIRIn(); // Start the receiver
  Serial.println("Enabled IRin");
}

void loop() {
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);

    if (results.value == 0xFF02FD) {
      turnH();
      last_com = 1;
    }

    if (results.value == 0xFF9867) {
      turnL();
      last_com = -1;
    }

    if (results.value != 0xFF02FD && results.value != 0xFF9867 && results.value != 0xFFFFFFFF) {
      last_com = 0;
      Serial.print("last com 0");
    }

    if (results.value == 0xFFFFFFFF) {
      if (last_com == 1) {
        turnH();
      }
      if (last_com == -1) {
        turnL();
      }
    }
    
    irrecv.resume(); // Receive the next value
  }
  delay(50);
}
