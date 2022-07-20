#define MRenc_A 22 // Amarillo
#define MRenc_B 23 // Blanco

volatile int R_pulses = 0;
volatile int CurrentPulsesR = 0;
volatile byte ant = 0;
volatile byte act = 0;

unsigned long lastTime = 0; // Tiempo anterior
unsigned long sampleTime = 100; // Tiempo de muestreo

void setup() {
  pinMode(MRenc_A, INPUT);    
  pinMode(MRenc_B, INPUT);

  attachInterrupt(MRenc_A, encoderR, RISING);

  Serial.begin(115200);
}

void loop() {
  if (millis() - lastTime >= sampleTime){ // Se actualiza cada sampleTime (milisegundos)
    lastTime = millis();
    Serial.print("Count: ");
    Serial.println(R_pulses);
}

}

void encoderR(){
  if (digitalRead(MRenc_B) == HIGH){     // si B es HIGH, sentido horario
    R_pulses-- ;        // incrementa POSICION en 1
  }
  else {          // si B es LOW, sentido anti horario
    R_pulses++ ;        // decrementa POSICION en 1
  }

  CurrentPulsesR = R_pulses;
}
