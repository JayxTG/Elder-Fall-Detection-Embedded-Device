#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <PubSubClient.h>
#include <WiFi.h>


const int mpuAddress = 0x68;          // I2C address of the MPU-6050

float xByGyro, yByGyro, zByGyro; 

const char* ssid = "Wokwi-GUEST";
const char* password = "";

const char* mqtt_server = "mqtt-dashboard.com";
const int mqtt_port = 1883;  // Adjust the port as needed
const char* clientId = "clientId-3GieYXySOm";
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


void setup() {
  Serial.begin(9600); 
  Wire.begin();

  
  while (!Serial) delay(1); 
  setup_wifi(); 
  client.setServer (mqtt_server, mqtt_port);


  Wire.beginTransmission( mpuAddress);
  Wire.write( 0x6B);                           // PWR_MGMT_1 register
  Wire.write( 0);                              // set to zero (wakes up the MPU-6050)
  auto error = Wire.endTransmission();
  if( error != 0)
  {
    Serial.println(F( "Error, MPU-6050 not found"));
    for(;;);                                   // halt the sketch if error encountered
  }

}

void loop() {
  if (!client.connected()) reconnect();
  client.loop();
 
  Wire.beginTransmission( mpuAddress);
  Wire.write( 0x3B);                   // Starting with register 0x3B (ACCEL_XOUT_H)
  Wire.endTransmission( false);        // No stop condition for a repeated start

  // The MPU-6050 has the values as signed 16-bit integers.
  // There are 7 values in 14 registers.
  int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;

  Wire.requestFrom( mpuAddress, 14);   // request a total of 14 bytes
  AcX = Wire.read()<<8 | Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
  AcY = Wire.read()<<8 | Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
  AcZ = Wire.read()<<8 | Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
  Tmp = Wire.read()<<8 | Wire.read();  // 0x41 (TEMP_OUT_H)   & 0x42 (TEMP_OUT_L)
  GyX = Wire.read()<<8 | Wire.read();  // 0x43 (GYRO_XOUT_H)  & 0x44 (GYRO_XOUT_L)
  GyY = Wire.read()<<8 | Wire.read();  // 0x45 (GYRO_YOUT_H)  & 0x46 (GYRO_YOUT_L)
  GyZ = Wire.read()<<8 | Wire.read();  // 0x47 (GYRO_ZOUT_H)  & 0x48 (GYRO_ZOUT_L)

  // The acceleration is directly mapped into the angles.
  // That is rather artificial.
  // The combined gravity could be used for an angle, while ignoring the strength.
  //
  // The gyro sets the rotation speed.
  // The angle created by the rotation speed is added to angle by the accelerometer.
  //
  // The conversion from the sensor values to the rotation is just a value
  // that makes it look good on the display.

  float xByAccel = (float) AcX * 0.0001;      // static angle by accelerometer
  float yByAccel = (float) AcY * 0.0001;
  float zByAccel = (float) AcZ * 0.0001;

  xByGyro += (float) GyX * 0.00001;           // moving angle by gyro
  yByGyro += (float) GyY * 0.00001;
  zByGyro += (float) GyZ * 0.00001;

  float x = xByAccel + xByGyro;               // combine both angles
  float y = yByAccel + yByGyro;
  float z = zByAccel + zByGyro;

    Serial.println(String(x) + " " +String(y)+ " " +String(z));
    if ((float(x) > 1.00) || (float(y)> 1.00)|| (float(z)> 1.00)) {

      publishMessage("Motions", "Abnormal Movement Detected " , true);
    }
  delay(1000);

}

