#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>
#include <DHT_U.h>

// OLED Configuration
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// DHT Configuration
#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
// Moisture sensor configuration
#define MOISTURE_PIN A0 // Analog pin connected to the sensor

void setup()
{
  // Initialize Serial Monitor
  Serial.begin(9600);
  Serial.println("Starting setup...");

  // Initialize DHT Sensor
  Serial.println("Initializing DHT sensor...");
  dht.begin();
  Serial.println("DHT sensor initialized.");

  // Initialize OLED
  Serial.println("Initializing OLED...");
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  { // 0x3C is common for most OLEDs
    Serial.println("SSD1306 allocation failed");
    for (;;)
      ;
  }
  display.clearDisplay();
  display.setTextSize(1);              // Text size
  display.setTextColor(SSD1306_WHITE); // Text color
  display.display();
  Serial.println("OLED initialized successfully.");
}

void loop()
{
  // Read Temperature and Humidity
  Serial.println("Reading DHT sensor...");
  float temp = dht.readTemperature();
  float humidity = dht.readHumidity();
  // Read soil moisture level
  int moistureLevel = analogRead(MOISTURE_PIN);
  // Convert moisture level to percentage (adjust mapping as needed)
  int moisturePercent = map(moistureLevel, 1023, 300, 0, 100);
  moisturePercent = constrain(moisturePercent, 0, 100); // Ensure value is between 0 and 100
  SerialPrintMoistureLevel(moisturePercent);
  // Check if any reads failed
  if (isnan(temp) || isnan(humidity))
  {
    DisplayDHTError();
    return;
  }

  SerialPrintTempandHumidity(temp, humidity);

  DisplayTempHumidityMoisture(temp, humidity, moisturePercent);

  delay(2000);
}

void SerialPrintTempandHumidity(float temp, float humidity)
{
  // Display on Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(temp);
  Serial.println(" *C");
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");
}
// Function to check if soil moisture is below threshold
bool isWaterNeeded(float soilMoisture) {
    return soilMoisture < 59;
}
void DisplayTempHumidityMoisture(float temp, float humidity, int moisturePercent)
{
  // Update OLED Display

  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("Temp & Hum & Moist");
  display.setCursor(0, 20);
  display.print("Temp: ");
  display.print(temp);
  display.println(" *C");
  //display.setCursor(0, 40);
  display.print("Humidity: ");
  display.print(humidity);
  display.println(" %");
if (isWaterNeeded(moisturePercent))
{
  //  display.setCursor(0, 10);
  display.println("Water Plants");
}
else{
   //display.setCursor(0, 10);
  display.println(":-)");
}
  display.print("Moisture: ");
  display.print(moisturePercent);
  display.println(" %");

  display.display();
}
void DisplayDHTError()
{
  Serial.println("Failed to read from DHT sensor!");
  display.clearDisplay();
  display.setCursor(0, 0);
  display.println("DHT Sensor Error!");
  display.display();
  delay(2000);
}
void SerialPrintMoistureLevel(int moisturePercent)
{
  Serial.print("Moisture Level: ");
  Serial.print(moisturePercent);
  Serial.println(" %");
}
