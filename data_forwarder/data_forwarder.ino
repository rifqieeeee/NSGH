#include <Wire.h>
#include <MPU6050.h>

MPU6050 mpu;

void setup()
{
  Serial.begin(115200);
  
  Serial.println("Initialize MPU6050");
  
  while(!mpu.begin(MPU6050_SCALE_2000DPS, MPU6050_RANGE_2G))
  {
    Serial.println("MPU6050 not found!");
    delay(500);
  }
 
  // Calibrate gyrometer
  mpu.calibrateGyro();
  mpu.setThreshold(3);
  
  Serial.println("MPU6050 ready.");
  delay(2000);
}
 
void loop()
{
  // Read gyrometer data
  Vector rawGyro = mpu.readRawGyro();

  Serial.print(rawGyro.XAxis);
  Serial.print(", ");
  Serial.print(rawGyro.YAxis);
  Serial.print(", ");
  Serial.print(rawGyro.ZAxis);
  Serial.print(", ");

  Vector rawAccel = mpu.readRawAccel();
  Serial.print(rawAccel.XAxis);
  Serial.print(", ");
  Serial.print(rawAccel.YAxis);
  Serial.print(", ");
  Serial.println(rawAccel.ZAxis);

  delay(500);
}
