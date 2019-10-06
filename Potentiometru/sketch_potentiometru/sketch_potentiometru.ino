
/*
const int ledPin = 13;  //pin 9 has PWM funtion
const int potPin = A2; //pin A2 to read analog input

//Variables:
int value; //save analog value

void setup(){
  //Input or output?
  pinMode(ledPin, OUTPUT); 
  pinMode(potPin, INPUT); //Optional 
}

void loop(){
  value = analogRead(potPin);          //Read and save analog value from potentiometer
  value = map(value, 0, 1023, 0, 255); //Map value 0-1023 to 0-255 (PWM)
  analogWrite(ledPin, value);          //Send PWM value to led
  delay(100);                          //Small delay
}
*/


void setup() {
  Serial.begin(9600);
}

void loop() {
  int sensorValue = analogRead(A2);
  Serial.println(sensorValue);
  delay(100);
}
