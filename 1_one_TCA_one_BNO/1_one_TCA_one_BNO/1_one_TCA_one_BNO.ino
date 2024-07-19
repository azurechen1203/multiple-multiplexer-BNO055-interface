#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

uint8_t ch = 0;
Adafruit_BNO055 bno = Adafruit_BNO055();


// Select I2C channel on TCA9548A multiplexer
void tcaselect(uint8_t channel) {
  Wire.beginTransmission(0x70);
  Wire.write(1 << channel);
  Wire.endTransmission();
}

// Print sensor information
void displaySensorDetails(void) {
  sensor_t sensor;
  bno.getSensor(&sensor);
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

  // select the channel
  tcaselect(ch);

  // initialise BNO055
  if (!bno.begin()) {
    Serial.print("no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1)
      ;
  } else {
    displaySensorDetails();
  }

  delay(1000);
}


void loop(void) {

  // select the channel
  tcaselect(ch);

  // Get quaternion data
  imu::Quaternion q = bno.getQuat();
  Serial.print(F("Quaternion: "));
  Serial.print(q.w(), 4);
  Serial.print(F(" "));
  Serial.print(q.x(), 4);
  Serial.print(F(" "));
  Serial.print(q.y(), 4);
  Serial.print(F(" "));
  Serial.print(q.z(), 4);
  Serial.println(F(""));

  // Get calibration status
  uint8_t sys, gyro, accel, mag = 0;
  bno.getCalibration(&sys, &gyro, &accel, &mag);
  Serial.print(F("Calibration: "));
  Serial.print(sys, DEC);
  Serial.print(F(" "));
  Serial.print(gyro, DEC);
  Serial.print(F(" "));
  Serial.print(accel, DEC);
  Serial.print(F(" "));
  Serial.println(mag, DEC);

  delay(100);
}
