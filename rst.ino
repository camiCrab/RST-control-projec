#include <TimerOne.h>
// Variables para la interrupción
volatile float Time = 0;
const int trigPin = 7;
const int echoPin = 6;
float distance_setpoint = 10; // Distancia deseada
// Define variables
volatile double duration;
volatile double distance1 = 0;
volatile int voltajebits = 0;
int in3 = 2; // Pin que controla el sentido de giro Motor B
int in4 = 4; // Pin que controla el sentido de giro Motor B
int EnB = 3; // Pin que controla la velocidad del Motor A PWM
float ultrasonicRead(); // Declaración de la función para leer la distancia
void setup() {
  Serial.begin(115200);  // Iniciamos la comunicación serial a 9600 baudios
  Timer1.initialize(20000); // Configura el TIMER en 20 milisegundos = 20000 microsegundos
  Timer1.attachInterrupt(Temporizador); // Configura la interrupción del Timer 1
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(2, OUTPUT); // Salidas del motor
  pinMode(4, OUTPUT);
  pinMode(3, OUTPUT); 
  analogWrite(EnB, 0); // Inicializa la velocidad del motor a cero
  delay(5000);
}
const float sa = 1, s1 = -0.579382135730782, r0 = 2.079050275705499, r1 = -1.925512816401701;
float motorSpeed = 0, u_1 = 0,  distance1_1=0;
float e = 0, e_1 = 0,  distance1_1_1 = 0;
float t = 0;
void loop() {
  if(Serial.available()>0){
  char seleccion = Serial.read();
  if(seleccion=='A'){
  Serial.print(Time);
  Serial.print(" ");
  Serial.print(motorSpeed);
  Serial.print(" ");
  Serial.print(distance1);
  Serial.print(" ");
  Serial.print(distance_setpoint);
  Serial.print(" ");
  Serial.println(e);  
  };
    if(seleccion == 'D'){
      while (Serial.available()==0);
      int data = Serial.read();
      distance_setpoint = data;
      }
    }
}
void Temporizador(void) {
  Time = Time + 0.02; // Incrementa el temporizador
  t = distance_setpoint*0.153537459303763;
  distance1 = ultrasonicRead();
  distance1_1 = r0*distance1 + r1*distance1_1_1; 
  e = t - distance1_1;
  motorSpeed = e - s1*u_1;
  //motorSpeed = (b0*e) - (b1*e_1) + (a1*u_1);
  u_1 = motorSpeed;
  e_1 = e;
  distance1_1_1 = distance1;
  if (e > 0) {    // Cambia la dirección del motor según el signo del error
    digitalWrite(in3, HIGH); // Giro ADELANTE
    digitalWrite(in4, LOW);
  } else {
    digitalWrite(in3, LOW); // Giro ATRÁS
    digitalWrite(in4, HIGH);
  }
  voltajebits = (abs(motorSpeed)*255)/5.7; // Ajusta la velocidad del motor dentro de los límites
  if (voltajebits < 50) voltajebits = 50;
  if (voltajebits > 255) voltajebits = 255;
  analogWrite(EnB, voltajebits);
}
float ultrasonicRead() { // Función para leer la distancia
  digitalWrite(trigPin, LOW);// Clears the trigPin
  digitalWrite(trigPin, HIGH);   // Sets the trigPin on HIGH state for 10 micro seconds
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);   // Reads the echoPin, returns the sound wave travel time in microseconds
  double distance = duration * 0.034 / 2.0;   // Calcula la distancia
  return distance;
  }
