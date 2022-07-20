#include "motorControl.h"

#define EnMR 5
#define MR_A 16
#define MR_B 17

#define MRenc_A 22 // Amarillo
#define MRenc_B 23 // Blanco

// Configurando los parámetros de PWM
const int frecuencia = 10000;
const int canal_R = 1;
const int resolucion = 12;

volatile long R_pulses = 0;
volatile long CurrentPulsesR = 0;

double w = 0.0;
const double constValue = 11.18;
//const double constValue = 0.3913;
int cv = 0;

unsigned long lastTime = 0; // Tiempo anterior
unsigned long sampleTime = 100; // Tiempo de muestreo
unsigned int R = 562; // Resolucion del encoder R = mH*s*r

String inputString = "";
bool stringComplete = false;
const char separator = ',';
const int datoLength = 1;
double dato[datoLength];

motorControl motor(sampleTime);

//--------------setup------------
void setup() {
  motor.setCvLimits(4095,1900);
  motor.setPvLimits(19,0);
  
  pinMode(EnMR, OUTPUT);
  pinMode(MR_A, OUTPUT);
  pinMode(MR_B, OUTPUT);
  
  pinMode(MRenc_A, INPUT);    
  pinMode(MRenc_B, INPUT);

  ledcSetup(canal_R, frecuencia, resolucion);

  ledcAttachPin(EnMR, canal_R);
  
  attachInterrupt(MRenc_A, encoderR, RISING);

  Serial.begin(115200);
  
}


//--------------loop------------
void loop() {
  if (stringComplete) {
    for (int i = 0; i < datoLength; i++){
      int index = inputString.indexOf(separator);
      dato[i] = inputString.substring(0,index).toInt();
      inputString = inputString.substring(index+1);
    }
    cv = motor.scaleCv(dato[0]);
    if (cv > 1){
      clockwise (cv);
    }
    else if (cv < 1){
      anticlockwise(abs(cv));
    }
    else{
      anticlockwise (0);
    }
    inputString = ""; 
    stringComplete = false; 
  }
  
  velValue();
  
}


//--------------functions------------
void serialEvent(){
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    inputString += inChar; 
    if (inChar == '\n') {
      stringComplete = true; 
    } 
  }
}

void velValue(){
  if (millis() - lastTime >= sampleTime){ // Se actualiza cada sampleTime (milisegundos)
    
    
    w =(constValue*R_pulses)/((millis()-lastTime));// Velocidad en RPM
    lastTime = millis();            // Almacenamos el tiempo actual
    R_pulses = 0;                   // Reiniciamos los pulsos
    
    w = motor.scalePv(w);           //Escala del 0 al 100%
    Serial.println(w);
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

void clockwise(int vel){
  digitalWrite(MR_A, LOW);
  digitalWrite(MR_B, HIGH);
  ledcWrite(canal_R, vel);
  if (vel < 1) {
    digitalWrite(MR_A, LOW);
    digitalWrite(MR_B, LOW);
    ledcWrite(canal_R, 0);
  }
}

void anticlockwise(int vel){
  digitalWrite(MR_A, HIGH);
  digitalWrite(MR_B, LOW);
  ledcWrite(canal_R, vel);
  if (vel < 1) {
    digitalWrite(MR_A, LOW);
    digitalWrite(MR_B, LOW);
    ledcWrite(canal_R, 0);
  }
}
