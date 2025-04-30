#include <Wire.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// ------------------ CONFIG ------------------
const char* ssid = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

const char* mqtt_server = "YOUR_MQTT_BROKER_IP_OR_HOST";
const int mqtt_port = 1883;
const char* mqtt_user = "YOUR_MQTT_USERNAME";
const char* mqtt_pass = "YOUR_MQTT_PASSWORD";

#define SENSOR_ADDR 0x08
#define DEVICE_ID "honeywell_ipm_sensor" // change if using multiple devices
// --------------------------------------------

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected!");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Connecting to MQTT...");
    if (client.connect(DEVICE_ID, mqtt_user, mqtt_pass)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" trying again in 5s");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  Wire.begin();  // SDA = D2, SCL = D1 on NodeMCU
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  readAndPublishParticulateData();
  delay(5000);  // Publish every 5 seconds
}

void readAndPublishParticulateData() {
  Wire.beginTransmission(SENSOR_ADDR);
  Wire.write(0x10);  // Write command
  Wire.write(0x03);  // Function code
  Wire.write(0x00);  // Address high
  Wire.write(0x11);  // Address low
  Wire.endTransmission();

  delay(100);

  Wire.requestFrom(SENSOR_ADDR, 30);
  if (Wire.available() < 12) {
    Serial.println("Insufficient sensor data.");
    return;
  }

  byte data[30];
  for (int i = 0; i < 30; i++) {
    data[i] = Wire.read();
  }

  uint16_t pm1  = (data[0] << 8) | data[1];
  uint8_t  crc1 = data[2];
  uint16_t pm25 = (data[3] << 8) | data[4];
  uint8_t  crc2 = data[5];
  uint16_t pm4  = (data[6] << 8) | data[7];
  uint8_t  crc3 = data[8];
  uint16_t pm10 = (data[9] << 8) | data[10];
  uint8_t  crc4 = data[11];

  // Simple CRC validation
  if (crc1 != ((data[0] + data[1]) & 0xFF) ||
      crc2 != ((data[3] + data[4]) & 0xFF) ||
      crc3 != ((data[6] + data[7]) & 0xFF) ||
      crc4 != ((data[9] + data[10]) & 0xFF)) {
    Serial.println("CRC error on one or more data points");
    return;
  }

  // Create a JSON-like payload
  char payload[128];
  snprintf(payload, sizeof(payload),
           "{\"pm1\":%u,\"pm2_5\":%u,\"pm4\":%u,\"pm10\":%u}",
           pm1, pm25, pm4, pm10);

  Serial.print("Publishing: ");
  Serial.println(payload);

  client.publish("sensors/pm", payload);  // Change topic as needed
}
