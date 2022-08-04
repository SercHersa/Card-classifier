#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

#include "motorControl.h"

#define EnML 15
#define ML_A 2
#define ML_B 4

#define EnMR 5
#define MR_A 16
#define MR_B 17

#define MLenc_A 21 // Amarillo
#define MLenc_B 19 // Blanco

#define MRenc_A 22 // Amarillo
#define MRenc_B 23 // Blanco

// Configurando los parámetros de PWM
const int frecuencia = 10000;
const int canal_L = 0;
const int canal_R = 1;
const int resolucion = 12;

volatile long L_pulses = 0;

volatile long R_pulses = 0;

double wLRef = 0.0;        // Velocidad angular en rad/s
double wL = 0.0;           // Velocidad angular de referencia en rad/s

double wRRef = 0.0;        // Velocidad angular en rad/s
double wR = 0.0;           // Velocidad angular de referencia en rad/s

const double constValue = 11.18;      
const double radio = 0.035;         // radio en metros
const double L = 0.175;             // Distancia entre llanta en metros
double uRobot = 0;                  // Velocidad lineal del robot
double wRobot = 0;                  // Velocidad angular del robot
double phi = 0;                     // Angulo del robot respecto a la referencia

double dist = 0;
double x = 0;
double y = 0;

//const double constValue = 0.3913;

int cvL = 0;
int cvR = 0;

unsigned long lastTime = 0; // Tiempo anterior
unsigned long sampleTime = 100; // Tiempo de muestreo
unsigned int R = 562; // Resolucion del encoder R = mH*s*r

String inputString = "";
bool stringComplete = false;
const char separator = ',';
const int datoLength = 2;
double dato[datoLength];

motorControl motorL(sampleTime);
motorControl motorR(sampleTime);

//-------variables para el control

float phid = 0.0;

float ePhi = 0;
float ex = 0;
float ey = 0;

float kp = 2;

//--------------setup------------
void setup() {
  pinMode(EnML, OUTPUT);
  pinMode(ML_A, OUTPUT);
  pinMode(ML_B, OUTPUT);

  pinMode(EnMR, OUTPUT);
  pinMode(MR_A, OUTPUT);
  pinMode(MR_B, OUTPUT);
  
  pinMode(MLenc_A, INPUT);    
  pinMode(MLenc_B, INPUT);

  pinMode(MRenc_A, INPUT);    
  pinMode(MRenc_B, INPUT);

  ledcSetup(canal_L, frecuencia, resolucion);
  ledcSetup(canal_R, frecuencia, resolucion);

  ledcAttachPin(EnML, canal_L);
  ledcAttachPin(EnMR, canal_R);
  
  attachInterrupt(MLenc_A, encoderL, RISING);
  attachInterrupt(MRenc_A, encoderR, RISING);

  Serial.begin(115200);
  SerialBT.begin("ColmiBot"); //Bluetooth device name

  motorL.setGains(0.45, 0.04, 0.04);           //K, Ti, Td
  motorR.setGains(0.45, 0.04, 0.04);           //K, Ti, Td
  
  motorL.setCvLimits(4095,1900);               // valores límites de pwm, para vel máxima y minima
  motorL.setPvLimits(19,0);                    // valores límites en rad/s

  motorR.setCvLimits(4095,1900);               // valores límites de pwm, para vel máxima y minima
  motorR.setPvLimits(19,0);                    // valores límites en rad/s

  lastTime = millis();
}


//--------------loop------------
void loop() {
  /*if (stringComplete) {
    for (int i = 0; i < datoLength; i++){
      int index = inputString.indexOf(separator);
      dato[i] = inputString.substring(0,index).toFloat();
      inputString = inputString.substring(index + 1);
    }

    velocityMotor(dato[0],dato[1]);     // Velocidad lineal del robot y angular
    
    inputString = ""; 
    stringComplete = false; 
  }*/

  //velocityMotor(10,0);
  posControl(2,0);
  printValues();
  
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

void serialEventBT(){
  while (SerialBT.available()) {
    char inChar = (char)SerialBT.read();
    inputString += inChar; 
    if (inChar == '\n') {
      stringComplete = true; 
    } 
  }
}

void velValue(){
  if (millis() - lastTime >= sampleTime){ // Se actualiza cada sampleTime (milisegundos)
    
    wL =(constValue*L_pulses)/((millis()-lastTime));// Velocidad en RPM
    wR =(constValue*R_pulses)/((millis()-lastTime));// Velocidad en RPM
    
    lastTime = millis();            // Almacenamos el tiempo actual
    
    L_pulses = 0;                   // Reiniciamos los pulsos
    R_pulses = 0;                   // Reiniciamos los pulsos
    
    cvL = motorL.compute(wLRef,wL);           // Control PID
    cvR = motorR.compute(wRRef,wR);           // Control PID
    
    if (cvL > 1) clockwiseL (cvL);
    else if (cvL < 1) anticlockwiseL(abs(cvL));
    else anticlockwiseL (0);

    if (cvR > 1) clockwiseR (cvR);
    else if (cvR < 1) anticlockwiseR(abs(cvR));
    else anticlockwiseR (0);

    velocityRobot(wL,wR);

    printValues();
    
    
  }
}

void encoderL(){
  if (digitalRead(MLenc_B) == HIGH){     // si B es HIGH, sentido horario
    L_pulses++ ;        // incrementa POSICION en 1
  }
  else {          // si B es LOW, sentido anti horario
    L_pulses-- ;        // decrementa POSICION en 1
  }

}

void encoderR(){
  if (digitalRead(MRenc_B) == HIGH){     // si B es HIGH, sentido horario
    R_pulses-- ;        // incrementa POSICION en 1
  }
  else {          // si B es LOW, sentido anti horario
    R_pulses++ ;        // decrementa POSICION en 1
  }

}

void clockwiseL(int vel){
  digitalWrite(ML_A, LOW);
  digitalWrite(ML_B, HIGH);
  ledcWrite(canal_L, vel);
  if (vel < 1) {
    digitalWrite(ML_A, LOW);
    digitalWrite(ML_B, LOW);
    ledcWrite(canal_L, 0);
  }
}

void anticlockwiseL(int vel){
  digitalWrite(ML_A, HIGH);
  digitalWrite(ML_B, LOW);
  ledcWrite(canal_L, vel);
  if (vel < 1) {
    digitalWrite(ML_A, LOW);
    digitalWrite(ML_B, LOW);
    ledcWrite(canal_L, 0);
  }
}

void clockwiseR(int vel){
  digitalWrite(MR_A, LOW);
  digitalWrite(MR_B, HIGH);
  ledcWrite(canal_R, vel);
  if (vel < 1) {
    digitalWrite(MR_A, LOW);
    digitalWrite(MR_B, LOW);
    ledcWrite(canal_R, 0);
  }
}

void anticlockwiseR(int vel){
  digitalWrite(MR_A, HIGH);
  digitalWrite(MR_B, LOW);
  ledcWrite(canal_R, vel);
  if (vel < 1) {
    digitalWrite(MR_A, LOW);
    digitalWrite(MR_B, LOW);
    ledcWrite(canal_R, 0);
  }
}

void velocityMotor(double u, double w){
  wLRef = (u - (L*w/2))/radio;
  wRRef = (u + (L*w/2))/radio;
  velValue();
}

void velocityRobot(double wL, double wR){
  uRobot = (radio *(wR + wL)) / 2;     //Promedio de las velocidades angulares por el radio de la llanta
  wRobot = (radio *(wR - wL)) / L;     //Diferencia entre las velocidades angulares por el radio de la llanta entre la distancia entre ellas
  
  phi = phi + 0.1 * wRobot;            //Angulo en radianes
  phi = atan2(sin(phi),cos(phi));
  //thetha = phi/PI *180;
  
  dist = 0.1 * uRobot;
  //dist = dist + 0.1 * uRobot;

  x = x + dist * cos(phi);
  y = y + dist * sin(phi);
}

void posControl(float xdes, float ydes){
  //speeds();
  //V = (VL + VR)/2;
  //V = 3000;
  int U = 1;
  ex = xdes - x;
  ey = ydes - y;
  phid = atan2(ey,ex);
  ePhi = phid-phi;
  
  int W = wRobot + kp *ePhi;
  
  

  if (abs(ex) < 0.5 && abs(ey) < 0.5){
    velocityMotor(0,0);
  }
  else {
    velocityMotor(U,W);
  }
  
  
}


void printValues(){
  //Serial.println(uRobot);
    //Serial.println(wRobot);
  Serial.print("ex: ");
  Serial.print(ex);

  Serial.print("  x: ");
  Serial.print(x);

  Serial.print("  ey: ");
  Serial.print(ey);
  
  Serial.print("  y: ");
  Serial.print(y);
  
  Serial.print("  uRobot: ");
  Serial.print(uRobot);
  
  Serial.print("  wRobot: ");
  Serial.print(wRobot);

  Serial.print("  phi: ");
  Serial.println(phi);
}

void rute(){
  //if (startf){
    
  
}
