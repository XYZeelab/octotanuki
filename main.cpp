#include "SPI.h"
#include "Ethernet.h"
#include "DHT.h"

#include "Thread.h"
#include "ThreadController.h"

#include "pb_encode.h"
#include "pb_decode.h"
#include "sensordata.pb.h"

#define DHTPIN 2
#define DHTTYPE DHT11   // DHT 11

DHT dht(DHTPIN, DHTTYPE);

float GetTemperature(bool isFahrenheit);

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 11, 16);

// Initialize the Ethernet server library
// with the IP address and port you want to use
// (port 80 is default for HTTP):
EthernetServer server(80);

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }


  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}


void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 5");  // refresh the page automatically every 5 sec
          client.println();
          float temp = GetTemperature(false);

          client.print("<Temperature>") + client.print(temp) + client.println("</Temperature>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}

float GetHumidity()
{
	float h = dht.readHumidity();

	if (isnan(h)) {
		  Serial.println("Failed to read from DHT sensor!");
		  return 0.0f;
	}
	delay(500);

	return h;
}

float GetTemperature(bool isFahrenheit)
{
	float t;

	if (isFahrenheit)
	{
		// Read temperature as Fahrenheit (isFahrenheit = true)
		t = dht.readTemperature(isFahrenheit);
	} else {
		// Read temperature as Celsius (the default)
		t = dht.readTemperature(isFahrenheit);
	}

	// Check if any reads failed and exit early (to try again).
	if (isnan(t)) {
		Serial.println("Failed to read from DHT sensor!");
		return 0.0f;
	}

	return t;
}

float GetHeatIndex(bool isFahrenheit)
{
	  // Reading temperature or humidity takes about 250 milliseconds!
	  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
	  float h = dht.readHumidity();
	  // Read temperature as Celsius (the default)
	  float t = dht.readTemperature();
	  // Read temperature as Fahrenheit (isFahrenheit = true)
	  float f = dht.readTemperature(true);

	  // Check if any reads failed and exit early (to try again).
	  if (isnan(h) || isnan(t) || isnan(f)) {
	    Serial.println("Failed to read from DHT sensor!");
	    return 0.0f;
	  }

	  // Compute heat index in Fahrenheit (the default)
	  float hif = dht.computeHeatIndex(f, h);
	  // Compute heat index in Celsius (isFahreheit = false)
	  float hic = dht.computeHeatIndex(t, h, false);
	delay(500);

	return t;
}
