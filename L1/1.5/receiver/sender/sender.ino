const int ledPin = 13;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial1.begin(9600);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  while(true)
  {
    Serial.write(0);
    Serial1.write(0);
    analogWrite(ledPin, 0);
    delay(500);
    Serial.write(255);
    Serial1.write((255));
    analogWrite(ledPin, 255);
    delay(500);
  }
}
