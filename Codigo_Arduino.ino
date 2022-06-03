
int ledv = 10;
int ledvd = 12;
int sensorluz = A0;
int valorluz = 0;
int motor1 = 3;
int motor2 = 2;
int vel = 180;
#define tempoAbre  5000
#define tempoFecha 5000
#define velocidadeRotacao 6
String valorluz_str;

bool abre  = true;
bool fecha = true;

void setup() {
  pinMode (ledv, OUTPUT);
  pinMode (ledvd, OUTPUT);
  pinMode (motor1, OUTPUT);
  pinMode (motor2, OUTPUT);
  pinMode (velocidadeRotacao, OUTPUT);
  pinMode (sensorluz, INPUT);
  Serial.begin(9600);
}

void loop() {
   valorluz = analogRead (sensorluz);
   Serial.println(valorluz);
   delay(500);
   
   if((valorluz) < 800){
     if (fecha == true) {
       analogWrite(velocidadeRotacao, vel);
       digitalWrite(motor1, HIGH);
       digitalWrite(motor2, LOW);
       digitalWrite(ledv, HIGH);
       digitalWrite(ledvd, LOW);
       delay(tempoFecha);
       digitalWrite(motor1, LOW);
       digitalWrite(motor2, LOW);
       analogWrite(velocidadeRotacao, 0);
       
       fecha = false;
       abre = true;
            
     }
  }
  
  else{
    if (abre == true){
      analogWrite(velocidadeRotacao, vel);
      digitalWrite(motor1, LOW);
      digitalWrite(motor2, HIGH);
      digitalWrite(ledv, LOW);
      digitalWrite(ledvd, HIGH);
      delay(tempoAbre);
      digitalWrite(motor1, LOW);
      digitalWrite(motor2, LOW);
      analogWrite(velocidadeRotacao, 0);
      
      abre = false;
      fecha = true;
     
    }
   }
  }
