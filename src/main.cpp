#include <Adafruit_LSM303DLH_Mag.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

// Function header
void collectSamples(int sampleCount);
void displaySensorDetails(void);
void magInit(void);

// LSM303DLHC object declaration
Adafruit_LSM303DLH_Mag_Unified mag = Adafruit_LSM303DLH_Mag_Unified(12345);

// Global variables declaration
float x, y, z = 0;

void setup(void)
{

#ifndef ESP8266
  while (!Serial)
    ; // will pause Zero, Leonardo, etc until serial console opens
#endif

  Serial.begin(9600);
  magInit();
  displaySensorDetails();
}

void loop(void)
{
  collectSamples(100);
  while (1)
    ;
}

// FUNKCIJE

void collectSamples(int sampleCount)
{
  static uint16_t cycleCounter = 0;
  sensors_event_t event;

  Serial.println("---- begin collectSamples ---------------");
  /* Get a new sensor event */
  while (1)
  {
    mag.getEvent(&event);
    x = event.magnetic.x;
    y = event.magnetic.y;
    z = event.magnetic.z;

    // skip first 15 measurements, collect "sampleCount" valid measurements
    cycleCounter++;

    if (cycleCounter > 15)
    {
      // validate
      if ((z > 0.01) | (z < -0.01))
      {
        Serial.println(z);
      }
      else
      {
        cycleCounter--;
      }
    }

    if (cycleCounter > (sampleCount + 15))
    {
      Serial.println("---- end collectSamples -----------------");
      return;
    }

    // delay - read frequency: 13 Hz
    delay(75);
  }
}

void displaySensorDetails(void)
{
  sensor_t sensor;
  mag.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print("Sensor:       ");
  Serial.println(sensor.name);
  Serial.print("Driver Ver:   ");
  Serial.println(sensor.version);
  Serial.print("Unique ID:    ");
  Serial.println(sensor.sensor_id);
  Serial.print("Max Value:    ");
  Serial.print(sensor.max_value);
  Serial.println(" uT");
  Serial.print("Min Value:    ");
  Serial.print(sensor.min_value);
  Serial.println(" uT");
  Serial.print("Resolution:   ");
  Serial.print(sensor.resolution);
  Serial.println(" uT");
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

void magInit(void)
{
  // Gain
  //mag.enableAutoRange(false);
  //mag.setMagGain(LSM303_MAGGAIN_1_9);
  mag.enableAutoRange(true);

  // Rate
  mag.setMagRate(LSM303_MAGRATE_220);

  // Initialise the sensor
  while (!mag.begin())
  {
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    delay(1000);
  }
  return;
}