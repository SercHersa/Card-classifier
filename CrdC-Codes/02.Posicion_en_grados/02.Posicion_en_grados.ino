#define MRenc_A 22 // Amarillo
#define MRenc_B 23 // Blanco

volatile long R_pulses = 0;
volatile long CurrentPulsesR = 0;

double P = 0.0;
unsigned long lastTime = 0; // Tiempo anterior
unsigned long sampleTime = 100; // Tiempo de muestreo
unsigned int R = 562; // Resolucion del encoder R = mH*s*r

void setup() {
  pinMode(MRenc_A, INPUT);    
  pinMode(MRenc_B, INPUT);

  attachInterrupt(MRenc_A, encoderR, RISING);

  Serial.begin(115200);
  Serial.println("Posicion en grados");
}

void loop() {
  posValue();
  
}

void posValue(){
  if (millis() - lastTime >= sampleTime){ // Se actualiza cada sampleTime (milisegundos)
    lastTime = millis();
    P = (R_pulses*360.0)/R;
    Serial.print("Posicion: ");
    Serial.println(P);
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
