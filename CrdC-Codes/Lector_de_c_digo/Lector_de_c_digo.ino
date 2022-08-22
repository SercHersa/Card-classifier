#define robotAlert 2    // Pin donde el robot le dirá a Arduino que ya se alineo con la caja
#define par 32
#define impar 33

#define code_A 25
#define code_B 26
#define code_C 27
#define code_D 14

bool A, B, C, D;

void setup() {
  pinMode(robotAlert, OUTPUT);
  pinMode(impar, OUTPUT);
  pinMode(par, OUTPUT);

  pinMode(code_A, INPUT);
  pinMode(code_B, INPUT);
  pinMode(code_C, INPUT);
  pinMode(code_D, INPUT);

  Serial.begin(115200);
}


void loop() {
  A = digitalRead(code_A);
  B = digitalRead(code_B);
  C = digitalRead(code_C);
  D = digitalRead(code_D);
  if (D == 0){
    digitalWrite(par, HIGH);
  }
  else digitalWrite(par, LOW);

  //if (A == 0 && B == 0 && C == 0 && D == 0){

}
