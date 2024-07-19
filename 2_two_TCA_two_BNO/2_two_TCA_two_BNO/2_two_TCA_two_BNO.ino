#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

uint8_t ch = 0;
uint8_t dummy_ch = 7;  // assign dummy channel that is not connected to anything

Adafruit_BNO055 bno1 = Adafruit_BNO055(1);  // assign id 1
Adafruit_BNO055 bno2 = Adafruit_BNO055(2);  // assign id 2

// Select I2C channel on first TCA9548A multiplexer(0x70)
void tcaselect1(uint8_t channel) {

  // deactivate the second multiplexer
  Wire.beginTransmission(0x71);
  Wire.write(0);
  Wire.endTransmission();

  // activate the channel on the first multiplexer
  Wire.beginTransmission(0x70);
  Wire.write(1 << channel);
  Wire.endTransmission();
}

// Select I2C channel on second TCA9548A multiplexer(0x71)
void tcaselect2(uint8_t channel) {

  // deactivate the first multiplexer
  Wire.beginTransmission(0x70);
  Wire.write(0);
  Wire.endTransmission();

  // activate the channel on the second multiplexer
  Wire.beginTransmission(0x71);
  Wire.write(1 << channel);
  Wire.endTransmission();
}

// Print sensor information (bno1)
void displaySensorDetails1(void) {
  sensor_t sensor;
  bno1.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print("Sensor:       ");
  Serial.println(sensor.name);
  Serial.print("Driver Ver:   ");
  Serial.println(sensor.version);
  Serial.print("Unique ID:    ");
  Serial.println(sensor.sensor_id);
  Serial.print("Max Value:    ");
  Serial.print(sensor.max_value);
  Serial.println(" xxx");
  Serial.print("Min Value:    ");
  Serial.print(sensor.min_value);
  Serial.println(" xxx");
  Serial.print("Resolution:   ");
  Serial.print(sensor.resolution);
  Serial.println(" xxx");
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

// Print sensor information (bno2)
void displaySensorDetails2(void) {
  sensor_t sensor;
  bno2.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print("Sensor:       ");
  Serial.println(sensor.name);
  Serial.print("Driver Ver:   ");
  Serial.println(sensor.version);
  Serial.print("Unique ID:    ");
  Serial.println(sensor.sensor_id);
  Serial.print("Max Value:    ");
  Serial.print(sensor.max_value);
  Serial.println(" xxx");
  Serial.print("Min Value:    ");
  Serial.print(sensor.min_value);
  Serial.println(" xxx");
  Serial.print("Resolution:   ");
  Serial.print(sensor.resolution);
  Serial.println(" xxx");
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}



void setup(void) {

  Wire.begin();

  Serial.begin(115200);
  Serial.println("");
  Serial.println("Orientation Sensor Test");
  Serial.println("");

  // select the channel on the first multiplexer and try to initialise bno1
  tcaselect1(ch);
  if (!bno1.begin()) {
    /* There was a problem detecting the BNO055 ... check your connections */
    Serial.print("no BNO055-1 detected ... Check your wiring or I2C ADDR!");
    while (1)
      ;
  } else {
    displaySensorDetails1();
  }
  // deselect the channel by selecting dummy channel
  tcaselect1(dummy_ch);

  // select the channel on the second multiplexer and try to initialise bno2
  tcaselect2(ch);
  if (!bno2.begin()) {
    Serial.print("no BNO055-2 detected ... Check your wiring or I2C ADDR!");
    while (1)
      ;
  } else {
    displaySensorDetails2();
  }
  // deselect the channel by selecting dummy channel
  tcaselect2(dummy_ch);

  delay(1000);
}

void loop(void) {

  // select the channel on the first multiplexer
  tcaselect1(ch);

  // Get quaternion data from the BNO055-1
  imu::Quaternion q1 = bno1.getQuat();
  Serial.print(F("Quaternion 1: "));
  Serial.print(q1.w(), 4);
  Serial.print(F(" "));
  Serial.print(q1.x(), 4);
  Serial.print(F(" "));
  Serial.print(q1.y(), 4);
  Serial.print(F(" "));
  Serial.print(q1.z(), 4);
  Serial.println(F(""));
  delay(100);

  // deselect the channel by selecting dummy channel
  tcaselect1(dummy_ch);

  // select the channel on the second multiplexer
  tcaselect2(ch);

  // Get quaternion data from the BNO055-2
  imu::Quaternion q2 = bno2.getQuat();
  Serial.print(F("Quaternion 2: "));
  Serial.print(q2.w(), 4);
  Serial.print(F(" "));
  Serial.print(q2.x(), 4);
  Serial.print(F(" "));
  Serial.print(q2.y(), 4);
  Serial.print(F(" "));
  Serial.print(q2.z(), 4);
  Serial.println(F(""));
  delay(100);

  // deselect the channel by selecting dummy channel
  tcaselect2(dummy_ch);
}
