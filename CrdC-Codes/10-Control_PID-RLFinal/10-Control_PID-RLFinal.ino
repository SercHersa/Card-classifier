#include "motorControl.h"

#define EnML 15
#define ML_A 2
#define ML_B 4

#define MLenc_A 21 // Amarillo
#define MLenc_B 19 // Blanco

// Configurando los parámetros de PWM
const int frecuencia = 10000;
const int canal_L = 0;
const int resolucion = 12;

volatile long L_pulses = 0;
volatile long CurrentPulsesL = 0;

double wRef = 0.0;        // Velocidad angular en rad/s
double w = 0.0;           // Velocidad angular de referencia en rad/s

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
  pinMode(EnML, OUTPUT);
  pinMode(ML_A, OUTPUT);
  pinMode(ML_B, OUTPUT);
  
  pinMode(MLenc_A, INPUT);    
  pinMode(MLenc_B, INPUT);

  ledcSetup(canal_L, frecuencia, resolucion);

  ledcAttachPin(EnML, canal_L);
  
  attachInterrupt(MLenc_A, encoderL, RISING);

  Serial.begin(115200);

  //motor.lambdaTunning(1.1408,0.1032,0.0896);   // const de porporcionalidad, const de tiempo en lazo abierto,retardo
  motor.setGains(0.5, 0.04, 0.04);
  
  motor.setCvLimits(4095,1900);               // valores límites de pwm, para vel máxima y minima
  motor.setPvLimits(19,0);                    // valores límites en rad/s
  
//  Serial.print(motor.getK());
//  Serial.print(", ");
//  Serial.print(motor.getTi());
//  Serial.print(", ");
// Serial.println(motor.getTd());
  lastTime = millis();
}


//--------------loop------------
void loop() {
  if (stringComplete) {
    for (int i = 0; i < datoLength; i++){
      int index = inputString.indexOf(separator);
      dato[i] = inputString.substring(0,index).toFloat();
      inputString = inputString.substring(index+1);
    }
    wRef = dato[0];
    
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
    
    
    w =(constValue*L_pulses)/((millis()-lastTime));// Velocidad en RPM
    lastTime = millis();            // Almacenamos el tiempo actual
    L_pulses = 0;                   // Reiniciamos los pulsos
    
    cv = motor.compute(wRef,w);           // Control PID
    
    if (cv > 1){
      clockwise (cv);
    }
    else if (cv < 1){
      anticlockwise(abs(cv));
    }
    else{
      anticlockwise (0);
    }
    
    Serial.println(w);
    Serial.println(cv);
  }
}

void encoderL(){
  if (digitalRead(MLenc_B) == HIGH){     // si B es HIGH, sentido horario
    L_pulses++ ;        // incrementa POSICION en 1
  }
  else {          // si B es LOW, sentido anti horario
    L_pulses-- ;        // decrementa POSICION en 1
  }

  CurrentPulsesL = L_pulses;
}

void clockwise(int vel){
  digitalWrite(ML_A, LOW);
  digitalWrite(ML_B, HIGH);
  ledcWrite(canal_L, vel);
  if (vel < 1) {
    digitalWrite(ML_A, LOW);
    digitalWrite(ML_B, LOW);
    ledcWrite(canal_L, 0);
  }
}

void anticlockwise(int vel){
  digitalWrite(ML_A, HIGH);
  digitalWrite(ML_B, LOW);
  ledcWrite(canal_L, vel);
  if (vel < 1) {
    digitalWrite(ML_A, LOW);
    digitalWrite(ML_B, LOW);
    ledcWrite(canal_L, 0);
  }
}
