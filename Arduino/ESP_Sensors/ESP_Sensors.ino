
// REQUIRES the following Arduino libraries:
// - DHT Sensor Library: https://github.com/adafruit/DHT-sensor-library
// - Adafruit Unified Sensor Lib: https://github.com/adafruit/Adafruit_Sensor

#include "DHT.h"
#define DHTPIN 23     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321

DHT dht(DHTPIN, DHTTYPE);

//Light Sensor
const int lightPin = 35;
int lightValue = 0;

int val = 0; //value for storing moisture value
int soilPin = 34;//Declare a variable for the soil moisture sensor
int ledPin = 2;//Variable for Soil moisture Power

void setup() {
  flash_blue();
  Serial.begin(115200);
  Serial.println(F("Agrivoltaics"));
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  dht.begin();
}

void loop() {
  // Wait a few seconds between measurements.

  digitalWrite(ledPin, HIGH); //turn on the blue led while taking measurements
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
  } else {
    // Compute heat index in Fahrenheit (the default)
    float hif = dht.computeHeatIndex(f, h);
    // Compute heat index in Celsius (isFahreheit = false)
    float hic = dht.computeHeatIndex(t, h, false);

    Serial.print(F("Humidity: "));
    Serial.print(h);
    Serial.print(F("%  Temperature: "));
    Serial.print(t);
    Serial.print(F("°C "));
    Serial.print(f);
    Serial.print(F("°F  Heat index: "));
    Serial.print(hic);
    Serial.print(F("°C "));
    Serial.print(hif);
    Serial.println(F("°F"));
  }
  Serial.print("Soil Moisture = ");
  //get soil moisture value from the function below and print it
  Serial.println(readSoil());

  lightValue = avgMeasure(lightPin, 1000);
  Serial.print("Light = ");
  Serial.println(lightValue);
  Serial.println();
  digitalWrite(ledPin, LOW); // turn off blue led
  delay(2000);
}

void flash_blue() {
  //flashes the blue light for a couble of seconds
  pinMode(2, OUTPUT);
  for (int count = 0; count < 10; count++) {
    digitalWrite(2, LOW);
    delay(100);
    digitalWrite(2, HIGH);
    delay(100);
  }
}

float avgMeasure(int pin, int num) {
  analogRead(pin); //discard first value
  delay(2);
  float x = 0;
  for (int count = 0; count < num; count++) {
    x = x + analogRead(pin);
  }
  x = x / num;
  return x;
}

int readSoil() {
  delay(10);//wait 10 milliseconds
  val = analogRead(soilPin);//Read the SIG value form sensor
  return val;//send current moisture value
}
