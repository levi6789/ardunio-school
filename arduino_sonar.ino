long duration;
int distance;

const int trigPin = 10;
const int echoPin = 11;

void setup(){
// put setup code here to run once
pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT);
Serial.begin(115200);


}

void loop (){
// put main code here to run repeatedly
digitalWrite(trigPin, LOW);
delayMicroseconds(2);

digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW); 

duration = pulseIn(echoPin, HIGH);

distance = duration * 0.034 / 2;

Serial.print("Distance: ");
Serial.println(distance);

}
