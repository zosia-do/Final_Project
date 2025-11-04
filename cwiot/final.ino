#include "config.h"

#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_NeoPixel.h>

//humidity/temp sensor declaration
#define DHTPIN 21
#define DHTTYPE DHT11
DHT dht (DHTPIN, DHTTYPE);

//setting up the 'digital' feed
AdafruitIO_Feed *temperatureFeed = io.feed("temperature");
AdafruitIO_Feed *humidityFeed = io.feed("humidity");

// RGB LED setup
#define RED_PIN 33
#define GREEN_PIN 27
#define BLUE_PIN 12

void setup() {
  //start serial connection
  Serial.begin(115200);
  dht.begin();
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  //wait for it to open
  while(! Serial);

  // connect to io.adafruit.com
  Serial.print("Connecting to Adafruit IO");
  io.connect();


  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  // we are connected
  Serial.println();
  Serial.println(io.statusText());

}

void loop() {
  io.run();

//read temp and humidity
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Check if readings are valid
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    delay(3000);
    return;
  }

  // round to 1 decimal place
  temperature = round(temperature * 10) / 10.0;
  humidity    = round(humidity * 10) / 10.0;

  // Serial print the data
  Serial.print(" Temperature is : ");
  Serial.print(temperature, 1);
  Serial.print("°C and humidity is : ");
  Serial.print(humidity, 1);
  Serial.println("%");

  // Send data to Adafruit IO
  Serial.print("Sending temperature -> ");
  Serial.println(temperature, 1);
  temperatureFeed->save(temperature);

  Serial.print("Sending humidity -> ");
  Serial.println(humidity, 1);
  humidityFeed->save(humidity);

//determining the LED status
  bool isBad = false;

  // Good range: temp 20-25°C, humidity 40-60%
  // Average range: temp 15-30°C, humidity 30-70%
  // Anything else is Bad
  if (temperature >= 20 && temperature <= 25 && humidity >= 40 && humidity <= 60) {
    // Good - green
    isBad = false;
    analogWrite(RED_PIN, 0);
    analogWrite(GREEN_PIN, 255);
    analogWrite(BLUE_PIN, 0);
    Serial.println("Status: Good (GREEN)");
  } else if (temperature >= 15 && temperature <= 30 && humidity >= 30 && humidity <= 70) {
    // Average - yellow
    isBad = false;
    analogWrite(RED_PIN, 255);
    analogWrite(GREEN_PIN, 255);
    analogWrite(BLUE_PIN, 0);
    Serial.println("Status: Average (YELLOW)");
  } else {
    // Bad - red
    isBad = true;
    analogWrite(RED_PIN, 255);
    analogWrite(GREEN_PIN, 0);
    analogWrite(BLUE_PIN, 0);
    Serial.println("Status: Bad (RED)");
  }
  delay(10000);

}

// this function is called whenever an 'digital' feed message
// is received from Adafruit IO. it was attached to
// the 'digital' feed in the setup() function above.
void handleMessage(AdafruitIO_Data *data) {
  Serial.print("received <- ");
  Serial.println(data->toString());
}

