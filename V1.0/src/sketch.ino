#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <PubSubClient.h>
#include <WiFi.h>

const char* ssid = "Wokwi-GUEST";
const char* password = "";

const char* mqtt_server = "mqtt-dashboard.com";
const int mqtt_port = 1883;  // Adjust the port as needed
const char* clientId = "mqttx_84ea7c5f";
const char* mqtt_username = "";
const char* mqtt_password = "";

WiFiClient espClient;
PubSubClient client(espClient);
// MPU6050 instance
Adafruit_MPU6050 mpu;

unsigned long lastMsg = 0; 
#define MSG_BUFFER_SIZE (50) 
char msg [MSG_BUFFER_SIZE];

void setup_wifi() {
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println("\"" + String(ssid) + "\"");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(clientId)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("check", "Start sending");
      // ... and resubscribe
      client.subscribe("test data");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void publishMessage(const char* topic, String payload, boolean retained) { 
    if (client.publish (topic, payload.c_str(), true)) 
    Serial.println("Message Published ["+String(topic)+"]: "+payload);
}
// Core 0 Task
void core0Task(void * parameter) {
 for (;;) {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

    // Calculate the total acceleration value
    float totalAcceleration = sqrt(a.acceleration.x * a.acceleration.x + a.acceleration.y * a.acceleration.y );
    float totalGyro=g.gyro.y;
    // Check if the patient's orientation exceeds the threshold
    if ((totalAcceleration > 90) || (totalGyro > 40)) {
      Serial.print(String(totalAcceleration) + " " + String(totalGyro));
      publishMessage("Motions", "Abnormal Movement Detected " + String(totalAcceleration)+ " " + String(totalGyro), true);
    }

    delay(1000);
 }
}

// Core 1 Task
void core1Task(void * parameter) {
 for (;;) {

 // Read MPU6050 sensor data
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  // Create a JSON payload with sensor data
  String payloadD = "{";
  payloadD += "\"accel_x\":" + String(a.acceleration.x) + ",";
  payloadD += "\"accel_y\":" + String(a.acceleration.y) + ",";
  payloadD += "\"accel_z\":" + String(a.acceleration.z) + ",";
  payloadD += "\"gyro_x\":" + String(g.gyro.x) + ",";
  payloadD += "\"gyro_y\":" + String(g.gyro.y) + ",";
  payloadD += "\"gyro_z\":" + String(g.gyro.z) + "}";

  // Publish sensor data to the MQTT topic "sensor_data"
  publishMessage("sensor_data", payloadD, true);
  delay(1000);
 }
}

void setup() {
  Serial.begin(9600); 
  while (!Serial) delay(1); 
  setup_wifi(); 
  client.setServer (mqtt_server, mqtt_port);
  if (!client.connected()) reconnect();
  client.loop();
  // Start the two tasks
  xTaskCreatePinnedToCore(core0Task, "Core0Task", 10000, NULL, 1, NULL, 0);
  xTaskCreatePinnedToCore(core1Task, "Core1Task", 10000, NULL, 1, NULL, 1);
}

void loop() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

    // Calculate the total acceleration value
    float totalAcceleration = sqrt(a.acceleration.x * a.acceleration.x + a.acceleration.y * a.acceleration.y );
    float totalGyro=g.gyro.y;
    // Check if the patient's orientation exceeds the threshold
    if ((totalAcceleration > 90) || (totalGyro > 40)) {
      Serial.print(String(totalAcceleration) + " " + String(totalGyro));
      publishMessage("Motions", "Abnormal Movement Detected " + String(totalAcceleration)+ " " + String(totalGyro), true);
    }


}

