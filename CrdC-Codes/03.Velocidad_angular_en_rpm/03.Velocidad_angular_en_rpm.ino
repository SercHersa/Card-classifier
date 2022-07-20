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

double N = 0.0;
int cv = 0;

unsigned long lastTime = 0; // Tiempo anterior
unsigned long sampleTime = 100; // Tiempo de muestreo
unsigned int R = 562; // Resolucion del encoder R = mH*s*r

String inputString = "";
bool stringComplete = false;

void setup() {
  pinMode(EnMR, OUTPUT);
  pinMode(MR_A, OUTPUT);
  pinMode(MR_B, OUTPUT);
  
  pinMode(MRenc_A, INPUT);    
  pinMode(MRenc_B, INPUT);

  ledcSetup(canal_R, frecuencia, resolucion);

  ledcAttachPin(EnMR, canal_R);
  
  attachInterrupt(MRenc_A, encoderR, RISING);

  Serial.begin(115200);
  Serial.println("Velocidad en rpm");
}

void loop() {
  velValue();
  
}

void velValue(){
  if (millis() - lastTime >= sampleTime){ // Se actualiza cada sampleTime (milisegundos)
    cv = -4095;
    if (cv > 0){
      clockwise (cv);
    }
    else{
      anticlockwise(abs(cv));
    }
    N =(R_pulses*60.0*1000.0)/((millis()-lastTime)*R);// Velocidad en RPM
    lastTime = millis();
    R_pulses = 0;
    Serial.print("Velocidad: ");
    Serial.println(N);
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
}

void anticlockwise(int vel){
  digitalWrite(MR_A, HIGH);
  digitalWrite(MR_B, LOW);
  ledcWrite(canal_R, vel);
}
