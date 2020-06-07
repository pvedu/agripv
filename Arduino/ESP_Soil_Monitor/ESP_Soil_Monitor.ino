int val = 0; //value for storing moisture value
int soilPin = 34;//Declare a variable for the soil moisture sensor
int ledPin = 2;//internal blue led

void setup() {
  flash_blue();
  Serial.begin(115200);   // open serial over USB
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
}

void loop()
{
  Serial.print("Soil Moisture = ");
  digitalWrite(ledPin, HIGH); //turn on the blue led while taking measurements
  Serial.println(readSoil());
  digitalWrite(ledPin, LOW); // turn off blue led
  delay(1000);//take a reading every second
}

int readSoil() {
  delay(10);//wait 10 milliseconds
  val = analogRead(soilPin);//Read the SIG value form sensor
  return val;//send current moisture value
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
