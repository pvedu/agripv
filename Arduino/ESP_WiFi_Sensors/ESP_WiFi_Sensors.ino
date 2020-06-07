/*********
   Simple server to display sensors on the Agrivoltaics Project.
   Gets the sensor values when the webpage is viewed

  Rui Santos
  Complete project details at https://randomnerdtutorials.com
*********/

// Load Wi-Fi library
#include <WiFi.h>
//Load DHT22 library. Why is this in quotes.
#include "DHT.h" //This is from adafruit
#define DHTPIN 23     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22   // DHT 22  

// Replace with your network credentials
const char* ssid     = "EAT";
const char* password = "ginger1249";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

//Light Sensot
const int lightPin = 35;
int lightValue = 0;

//Soil Moisture
int val = 0; //value for storing moisture value
int soilPin = 34;//Declare a variable for the soil moisture sensor
int soilValue = 0;

//blue led
int LED1 = 2;

// Initialize DHT sensor.
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  flash_blue();
  Serial.begin(115200);
  Serial.println("ESP_WIFI");
  pinMode(LED1, OUTPUT);
  digitalWrite(LED1, LOW);


  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin(); //start webserver
  dht.begin();
}

void loop() {
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client

    while (client.connected()) {            // loop while the client's connected
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();
            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;color: #B4243F}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");

            // Web Page Heading
            client.println("<body><h1>Sensors</h1>");
            digitalWrite(LED1, HIGH);
            soilValue = readSoil();
            Serial.print("Soil Moisture: ");
            Serial.println(soilValue);
            client.print("<p>Soil Moisture: ");
            client.print(soilValue);
            client.println("</p>");

            lightValue = avgMeasure(lightPin, 1000);
            Serial.print("Light: ");
            Serial.println(lightValue);
            client.print("<p>Light: ");
            client.print(lightValue);
            client.println(" </p>");

            //Humidity Sensor
            float h = dht.readHumidity();
            // Read temperature as Celsius (the default)
            float t = dht.readTemperature();
            // Read temperature as Fahrenheit (isFahrenheit = true)
            float f = dht.readTemperature(true);
            if (isnan(h) || isnan(t) || isnan(f)) {
              Serial.println(F("Failed to read from DHT sensor!"));
              client.println("<p>Failed to read from DHT sensor </p>");

            } else {
              //if we have a valid DHT response then print the data
              // Compute heat index in Fahrenheit (the default)
              float hif = dht.computeHeatIndex(f, h);
              // Compute heat index in Celsius (isFahreheit = false)
              float hic = dht.computeHeatIndex(t, h, false);
              Serial.print(F("Humidity: "));
              Serial.print(h);
              Serial.print(F(" %  Temperature: "));
              Serial.print(t);
              Serial.print(F("°C "));
              Serial.print(f);
              Serial.print(F("°F  Heat index: "));
              Serial.print(hic);
              Serial.print(F("°C "));
              Serial.print(hif);
              Serial.println(F("°F"));

              //also print to the web page
              client.print("<p>");
              client.print(F("Humidity: "));
              client.print(h);
              client.print(F(" % </p><p> Temperature: "));

              client.print(t);
              client.print(F("&deg;C, "));

              client.print(f);
              client.print(F("&deg;F</p><p>  Heat index: "));
              client.print(hic);
              client.print(F("&deg;C, "));
              client.print(hif);
              client.print(F("&deg;F"));
              client.println("</p>");
            }
            digitalWrite(LED1, LOW); // sensors are done so turn off the blue LED.
            client.println("</body></html>");

            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println("Client disconnected.");
    Serial.println("");
  }
}

int readSoil() {
  delay(10);//wait 10 milliseconds
  val = analogRead(soilPin);//Read the SIG value form sensor
  return val;//send current moisture value
}

float avgMeasure(int pin, int num)
{
  analogRead(pin); //discard first value
  delay(2);
  float x = 0;
  for (int count = 0; count < num; count++) {
    x = x + analogRead(pin);
    //delay(5);
  }
  x = x / num;
  return x;

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
