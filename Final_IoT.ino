#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <DHT.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

// LED Pins
const int redLED = 3;
const int yellowLED = 4;
const int greenLED = 5;

// Gas Sensors
const int mq2Pin = A0;
const int mq135Pin = A1;

void setup() {
  Serial.begin(9600);
  pinMode(redLED, OUTPUT);
  pinMode(yellowLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  
  // Initialize OLED
  if(!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.display();
  delay(2000);
  display.clearDisplay();
  
  dht.begin();
}

void loop() {
  delay(2000);
  
  // Read sensors
  float humidity = dht.readHumidity();
  float temp = dht.readTemperature();
  int mq2Value = analogRead(mq2Pin);
  int mq135Value = analogRead(mq135Pin);
  
  // Air Quality Calculation
  int airQuality = (mq2Value + mq135Value) / 2;
  
  // Determine air quality status
  String airStatus = "";
  if(airQuality < 200) {
    airStatus = "Good";
    digitalWrite(greenLED, HIGH);
    digitalWrite(yellowLED, LOW);
    digitalWrite(redLED, LOW);
  }
  else if(airQuality >= 200 && airQuality < 500) {
    airStatus = "Moderate";
    digitalWrite(yellowLED, HIGH);
    digitalWrite(greenLED, LOW);
    digitalWrite(redLED, LOW);
  }
  else {
    airStatus = "Poor";
    digitalWrite(redLED, HIGH);
    digitalWrite(yellowLED, LOW);
    digitalWrite(greenLED, LOW);
  }

  // Display data on OLED
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  
  display.setCursor(0,0);
  display.print("Temp: "); display.print(temp); display.println(" C");
  display.print("Humidity: "); display.print(humidity); display.println(" %");
  display.print("MQ2: "); display.println(mq2Value);
  display.print("MQ135: "); display.println(mq135Value);
  display.setCursor(0,56);
  display.print("Air: "); display.print(airStatus);
  
  display.display();

  // Serial monitor output
  Serial.print("Temperature: "); Serial.print(temp);
  Serial.print("°C\tHumidity: "); Serial.print(humidity);
  Serial.print("%\tMQ2: "); Serial.print(mq2Value);
  Serial.print("\tMQ135: "); Serial.print(mq135Value);
  Serial.print("\tAir Quality: "); Serial.println(airStatus);
}

///////////////////////////////////////////////////////////////
PINOUT
//////////////////////////////////////////////////////////////

// Arduino Uno Connections:
// - OLED SDA -> A4
// - OLED SCL -> A5
// - DHT22 -> Digital Pin 2
// - MQ2 AOUT -> A0
// - MQ135 AOUT -> A1
// - Red LED -> Digital Pin 3
// - Yellow LED -> Digital Pin 4
// - Green LED -> Digital Pin 5
// - All sensors VCC -> 5V
// - All sensors GND -> GND
// - LEDs GND through resistors -> GND