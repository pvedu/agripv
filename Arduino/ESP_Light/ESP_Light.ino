//Light Sensor is connected to GPIO 35 (Analog ADC1_CH6)
const int lightPin = 35;
int lightValue = 0;
int ledPin = 2;//internal blue led

void setup() {
  flash_blue();
  Serial.begin(115200);
  delay(1000);
}

void loop() {
  Serial.print("Light = ");
  digitalWrite(ledPin, HIGH); //turn on the blue led while taking measurements
  lightValue = avgMeasure(lightPin, 1000);
  Serial.println(lightValue);
  digitalWrite(ledPin, LOW); // turn off blue led
  delay(500);
}

float avgMeasure(int pin, int num)
{
  analogRead(pin); //discard first value
  delay(2);
  float x = 0;
  for (int count = 0; count < num; count++) {
    x = x + analogRead(pin);
  }
  x = x / num;
  return x;
}

void flash_blue() {
  //flashes the blue light for a couble of seconds
  pinMode(ledPin, OUTPUT);
  for (int count = 0; count < 10; count++) {
    digitalWrite(ledPin, LOW);
    delay(100);
    digitalWrite(ledPin, HIGH);
    delay(100);
  }
}
