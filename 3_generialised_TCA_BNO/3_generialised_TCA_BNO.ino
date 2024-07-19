#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>

// Define the channels
uint8_t ch = 5;
uint8_t dummy_ch = 7;

// Define the multiplexer addresses
const uint8_t TCA_addrs[] = { 0x70, 0x71, 0x72 };
const uint8_t TCA_num = sizeof(TCA_addrs) / sizeof(TCA_addrs[0]);
bool debug_flag = 0;

// Create an array of Adafruit_BNO055 objects
Adafruit_BNO055 bnoArray[TCA_num];

// select I2C channel for TCA9548A multiplexer
void tcaselect(uint8_t muxAddr, uint8_t channel) {

  // Deactivate the channel on the current multiplexer
  for (uint8_t addr : TCA_addrs) {
    Wire.beginTransmission(addr);
    Wire.write(0);
    Wire.endTransmission();
  }

  // Activate the channel on the desired multiplexer
  Wire.beginTransmission(muxAddr);
  Wire.write(1 << channel);
  Wire.endTransmission();
}

// print out the sensor details
void displaySensorDetails(Adafruit_BNO055 &bno) {
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


void initializeSensors() {
  for (uint8_t i = 0; i < TCA_num; i++) {
    bnoArray[i] = Adafruit_BNO055(i + 1);
  }
}

void setupSensor(Adafruit_BNO055 &bno, uint8_t muxAddr, uint8_t channel) {

  tcaselect(muxAddr, channel);
  if (!bno.begin()) {
    Serial.print("Ooops, no BNO055 detected ... Check your wiring or I2C ADDR!");
    while (1)
      ;
  }

  if (debug_flag) {
    displaySensorDetails(bno);
  }
  tcaselect(muxAddr, dummy_ch);
}

void performDataReadIMU(Adafruit_BNO055 &bno, uint8_t id) {

  // get calibration data
  uint8_t system, gyro, accel, mag;
  bno.getCalibration(&system, &gyro, &accel, &mag);

  // the data is reliable only if one of the calibration status is non-zero
  if (system != 0 || gyro != 0 || accel != 0 || mag != 0) {

    // get quaternion data
    imu::Quaternion read_quaternion = bno.getQuat();

    // get velocity data
    sensors_event_t read_gyro;
    bno.getEvent(&read_gyro, Adafruit_BNO055::VECTOR_GYROSCOPE);

    // get acceleration data
    sensors_event_t read_accel;
    bno.getEvent(&read_accel, Adafruit_BNO055::VECTOR_ACCELEROMETER);

    // Print quaternion data
    Serial.print("I");
    Serial.print(id);
    Serial.print("O ");
    Serial.print(read_quaternion.w(), 4);
    Serial.print(" ");
    Serial.print(read_quaternion.x(), 4);
    Serial.print(" ");
    Serial.print(read_quaternion.y(), 4);
    Serial.print(" ");
    Serial.print(read_quaternion.z(), 4);
    Serial.println();

    // Print velocity data
    Serial.print("I");
    Serial.print(id);
    Serial.print("V ");
    Serial.print(read_gyro.gyro.x, 4);
    Serial.print(" ");
    Serial.print(read_gyro.gyro.y, 4);
    Serial.print(" ");
    Serial.print(read_gyro.gyro.z, 4);
    Serial.println();

    // Print acceleration data
    Serial.print("I");
    Serial.print(id);
    Serial.print("A ");
    Serial.print(read_accel.acceleration.x, 4);
    Serial.print(" ");
    Serial.print(read_accel.acceleration.y, 4);
    Serial.print(" ");
    Serial.print(read_accel.acceleration.z, 4);
    Serial.println();

    // Print calibration data
    // Serial.print("I");
    // Serial.print(id);
    // Serial.print("C ");
    // Serial.print(system);
    // Serial.print(" ");
    // Serial.print(gyro);
    // Serial.print(" ");
    // Serial.print(accel);
    // Serial.print(" ");
    // Serial.print(mag);
    // Serial.println();
  }
}

void setup(void) {

  Serial.begin(115200);
  Serial.println("");
  Serial.println("Aquisition Starts");
  Serial.println("");

  Wire.begin();
  initializeSensors();

  for (uint8_t i = 0; i < TCA_num; i++) {
    setupSensor(bnoArray[i], TCA_addrs[i], ch);
  }
  delay(500);
}

void loop(void) {
  for (uint8_t i = 0; i < TCA_num; i++) {
    tcaselect(TCA_addrs[i], ch);
    performDataReadIMU(bnoArray[i], i);
    tcaselect(TCA_addrs[i], dummy_ch);
  }
}
