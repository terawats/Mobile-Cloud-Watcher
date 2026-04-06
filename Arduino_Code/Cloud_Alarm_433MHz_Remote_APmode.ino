#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#include <Adafruit_MLX90614.h>
#include <EEPROM.h>
#include "index.h"

// WiFi AP credentials
const char* ssid = "Cloud-Watcher-Panel";
const char* password = "12345678";

// Static IP address setup
IPAddress local_IP(192, 168, 0, 1); 
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress primaryDNS(192,168, 0, 1);

ESP8266WebServer server(80);

// Create an instance of the MLX90614 sensor
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

// IO Pin
int LEDPin = D0; 
int RF433_Relay_pin = D5; 

// Battery monitoring
const int BATTERY_PIN = A0;
const float VOLTAGE_DIVIDER_RATIO = 5.0; // 1/5 voltage divider
const float MAX_BATTERY_VOLTAGE = 4.2; // Li-ion max voltage

// SkyTemperature data storage (720 readings for 12 hours at 1 minute intervals)
const int MAX_READINGS = 720;
float SkyTemperatures[MAX_READINGS];
unsigned long timestamps[MAX_READINGS];
int currentIndex = 0;
bool arrayFull = false;

// SkyTemperature limit settings
float SkyTemperatureLimit = 30.0;  // Default SkyTemperature limit
float sky_temperature = 15;
int sky_unsafe_status = 0;

const int EEPROM_TEMP_LIMIT_ADDR = 0;  // EEPROM address for SkyTemperature limit
int LED_toggle = 0;

// Timing variables
unsigned long previousSensorReadTime;
unsigned long previousChartIndexTime;
unsigned long sky_previous_unsafe_time;
unsigned long currentmillis;
const unsigned long READING_INTERVAL = 1000; // 1 sec in milliseconds
const unsigned long intervalUnsafeTime = 10000; // 10 sec to reset alarm for re-triger

void takeSkyTemperatureReading() {
    float temp;  
    float objectTemperature;
    float ambientTemperature;

    currentmillis = millis();

    temp = mlx.readObjectTempC();
    temp = mlx.readObjectTempC();
    if(isnan(temp)){
      Serial.println("Failed to read SkyTemperature !!!");
    }else{
      objectTemperature = temp;
    }
    ambientTemperature = mlx.readAmbientTempC();
    sky_temperature = objectTemperature - ( (33.0/100)*(ambientTemperature) + (8.0/100)*pow((exp(100.0/1000*ambientTemperature)) , (100.0/100)) ) + 10;

    // Shift data index every 60 sec.
    if(currentmillis - previousChartIndexTime > 60000){ // 60,000 millisec
      previousChartIndexTime = currentmillis ;
      if(currentIndex < MAX_READINGS - 1){
          currentIndex++;
      }
      if (currentIndex == MAX_READINGS - 1) {
        arrayFull = true;
        //shift SkyTemperature array
        for (int i = 0; i < MAX_READINGS - 2 ; i++) {
          SkyTemperatures[i] = SkyTemperatures[i + 1];
          timestamps[i] = timestamps[i + 1];
        }
        SkyTemperatures[MAX_READINGS - 1] = 0;
        timestamps[MAX_READINGS - 1] = currentmillis;
      }
    }

    if(sky_temperature > SkyTemperatures[currentIndex]){
        SkyTemperatures[currentIndex] = sky_temperature;
        timestamps[currentIndex] = currentmillis;
    }
    Serial.print("SkyTemperature reading ");
    Serial.print(currentIndex);
    Serial.print(": ");
    Serial.print(sky_temperature);
    Serial.print("°C, ");
    Serial.print("Ambient Temperature : ");
    Serial.print(ambientTemperature);
    Serial.print("°C, ");
    Serial.print("sky_unsafe_status : ");
    Serial.print(sky_unsafe_status);
    Serial.print(", Battery % : ");
    Serial.println(getBatteryPercentage());
}

void handleRoot() {
  server.send(200, "text/html", html);
}

void handleData() {
  String json = "{";
  json += "\"SkyTemperatures\":[";
  
  int count = arrayFull ? MAX_READINGS : currentIndex;
  int startIdx = arrayFull ? currentIndex : 0;
  
  for (int i = 0; i <= currentIndex; i++) {
    if (i > 0) json += ",";
    json += String(SkyTemperatures[i], 2);
  }
  
  json += "],\"timestamps\":[";
  
  for (int i = 0; i <= currentIndex; i++) {
    if (i > 0) json += ",";
    json += String(timestamps[i]);
  }

  json += "],\"battery\":[";
  json += String(getBatteryPercentage());

  json += "]}";
  
  server.send(200, "application/json", json);
}

void handleCurrent() {
  float temp = 25.1; //mlx.readObjectTempC();
  String json = "{\"SkyTemperature\":" + String(temp, 2) + "}";
  server.send(200, "application/json", json);
}

void handleGetLimit() {
  String json = "{\"limit\":" + String(SkyTemperatureLimit, 1) + "}";
  server.send(200, "application/json", json);
}

void handleSetLimit() {
  if (server.hasArg("limit")) {
    float newLimit = server.arg("limit").toFloat();
    if (newLimit >= 0 && newLimit <= 100) {  // Reasonable SkyTemperature range
      SkyTemperatureLimit = newLimit;
      saveSkyTemperatureLimitToEEPROM();
      Serial.print("SkyTemperature limit updated to: ");
      Serial.print(SkyTemperatureLimit);
      Serial.println("°C");
      server.send(200, "application/json", "{\"status\":\"success\",\"limit\":" + String(SkyTemperatureLimit, 1) + "}");
    } else {
      server.send(400, "application/json", "{\"status\":\"error\",\"message\":\"Invalid limit range (0-100)\"}");
    }
  } else {
    server.send(400, "application/json", "{\"status\":\"error\",\"message\":\"Missing limit parameter\"}");
  }
}

void handleGetBattery() {
  String json = "{\"battery\":" + String(getBatteryPercentage(), 1) + "}";
  server.send(200, "application/json", json);
}

void loadSkyTemperatureLimitFromEEPROM() {
  // Read SkyTemperature limit from EEPROM
  EEPROM.begin(sizeof(SkyTemperatureLimit)*2);
  EEPROM.get(EEPROM_TEMP_LIMIT_ADDR, SkyTemperatureLimit);
  EEPROM.end();
  
  // Check if EEPROM value is valid (not NaN or out of reasonable range)
  if (isnan(SkyTemperatureLimit) || SkyTemperatureLimit < 0 || SkyTemperatureLimit > 100) {
    SkyTemperatureLimit = 30.0;  // Default value
    saveSkyTemperatureLimitToEEPROM();  // Save default to EEPROM
  }
}

void saveSkyTemperatureLimitToEEPROM() {
  EEPROM.begin(sizeof(SkyTemperatureLimit)*2);
  EEPROM.put(EEPROM_TEMP_LIMIT_ADDR, SkyTemperatureLimit);
  EEPROM.commit();
  EEPROM.end();
}

void Send_RF433MHz_alarm()
{
  digitalWrite(RF433_Relay_pin, LOW); 
  delay(500);
  digitalWrite(RF433_Relay_pin, HIGH); 
}

float getBatteryPercentage() {
  int rawValue = analogRead(BATTERY_PIN);
  float voltage = (rawValue / 1024.0) * VOLTAGE_DIVIDER_RATIO;
  float percentage = ((voltage - 3.0) / (MAX_BATTERY_VOLTAGE - 3.0)) * 100.0;
  return constrain(percentage, 0.0, 100.0);
}

void LowBatteryCheck(){
  if(getBatteryPercentage() < 5.0){
    Serial.println("Battery critical level --> Shutdown in 10 seconds");
    for(int i=0;i<33;i++){
      digitalWrite(LEDPin, HIGH); 
      delay(150);
      digitalWrite(LEDPin, LOW); 
      delay(150);
    }
    ESP.deepSleep(0);
  }
}

void setup() {
  // Initial IO pin
  pinMode(LEDPin, OUTPUT);
  pinMode(RF433_Relay_pin, OUTPUT);
  pinMode(BATTERY_PIN, INPUT);
  digitalWrite(LEDPin, HIGH); 
  digitalWrite(RF433_Relay_pin, HIGH); 

  Serial.begin(115200);
  delay(1000);
  
  // Low battery check
  LowBatteryCheck();

  // Initialize I2C and MLX90614
  if (!mlx.begin()) {
    Serial.println("Error connecting to MLX sensor. Check wiring.");
    while (1);
  }
  
  // Initialize EEPROM
  //********** Un-comment for 1st use EEPROM *******************
  // saveSkyTemperatureLimitToEEPROM()
  //************************************************************

  // Load SkyTemperature limit from EEPROM
  loadSkyTemperatureLimitFromEEPROM();
  
  Serial.println("MLX90614 initialized successfully");
  Serial.print("SkyTemperature limit loaded: ");
  Serial.print(SkyTemperatureLimit);
  Serial.println("°C");
  
  // Initialize SkyTemperature array
  for (int i = 0; i < MAX_READINGS; i++) {
    SkyTemperatures[i] = 0.0;
    timestamps[i] = 0;
  }
  
  // Configure the soft AP with custom IP settings
  if (!WiFi.softAPConfig(local_IP, gateway, subnet)) {
    Serial.println("AP Config Failed");
  }

  // Setup WiFi AP
  WiFi.softAP(ssid, password);
  Serial.println();
  Serial.print("Access Point created: ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());
  
  // Setup web server routes
  server.on("/", handleRoot);
  server.on("/data", handleData);
  server.on("/current", handleCurrent);
  server.on("/battery", handleGetBattery);
  server.on("/setlimit", HTTP_POST, handleSetLimit);
  server.on("/getlimit", handleGetLimit);
  
  server.begin();
  Serial.println("HTTP server started");
  
  // Take initial reading
  currentmillis = millis();
  previousSensorReadTime = currentmillis;
  previousChartIndexTime = currentmillis;
  sky_previous_unsafe_time = currentmillis;
  takeSkyTemperatureReading();
}

void loop() {
  server.handleClient();
  
  // Take SkyTemperature reading every xx sec
  currentmillis = millis();
  if (currentmillis - previousSensorReadTime >= READING_INTERVAL) {
    if(LED_toggle == 1){
      digitalWrite(LEDPin, HIGH);
      LED_toggle = 0;
    } else {
      digitalWrite(LEDPin, LOW);
      LED_toggle = 1;
    }
    takeSkyTemperatureReading();
    previousSensorReadTime = currentmillis;
  }
  

  // cheeck for cloud, sky temperature over limit
  if(sky_temperature >= SkyTemperatureLimit){
    // Triger for remote alarm
    if(sky_unsafe_status == 0 ){
      sky_unsafe_status = 1; 
      Serial.println("Alarm !!!: Sky temperature over limit");
      Serial.print("Sky temperature: ");
      Serial.print(sky_temperature);
      Serial.print(", limit: ");
      Serial.println(SkyTemperatureLimit);
      Send_RF433MHz_alarm();
      sky_previous_unsafe_time = currentmillis;
    }else{
      if(sky_unsafe_status == 1 && (currentmillis - sky_previous_unsafe_time >= intervalUnsafeTime)){
        sky_unsafe_status = 0;
        Serial.println("Sky unsafe status are reseted by timmer");      
      }
    }
  }else{
    if(sky_unsafe_status == 1 && (currentmillis - sky_previous_unsafe_time >= intervalUnsafeTime)){
      sky_unsafe_status = 0;
      Serial.println("Sky unsafe status are cleared");
    }
  }

  LowBatteryCheck();
  delay(100);
}

