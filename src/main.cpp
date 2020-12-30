#include <Adafruit_LSM303DLH_Mag.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <math.h>

// Function header
double collectSamplesAndMean(int sampleCount);
void collectSamples(int sampleCount);
void displaySensorDetails(void);
void magInit(void);

// LSM303DLHC object declaration
Adafruit_LSM303DLH_Mag_Unified mag = Adafruit_LSM303DLH_Mag_Unified(12345);

// Global variables declaration
double x, y, z, finalWeight, finalMagVal;
char ledPin = 14;
char buttonPin = 12;

void setup(void)
{
  Serial.begin(9600);
  magInit();
  displaySensorDetails();

  // IO config
  pinMode(ledPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
}

void loop(void)
{
  static double tare = 0; // tara

  finalMagVal = collectSamplesAndMean(400);
  finalWeight = -154785.2 - 1288.846 * pow(finalMagVal, 1) - 4.17052 * pow(finalMagVal, 2) - 0.006647351 * pow(finalMagVal, 3) - 0.000005248848 * pow(finalMagVal, 4) - 1.645575e-9 * pow(finalMagVal, 5);

  Serial.println("Masa: " + ((String)(finalWeight - tare)) + "g   B_z: " + (String)finalMagVal + "uT    Tara: " + (String)tare);

  if (digitalRead(buttonPin) == 0) // tariranje
  {
    tare = finalWeight;
  }

  delay(200);
}

//////////////////////////// FUNKCIJE /////////////////////////

double collectSamplesAndMean(int sampleCount)
{
  uint16_t cycleCounter = 0;
  double mean = 0;
  sensors_event_t event;

  //Serial.println("---- begin collectSamplesAndMean ---------------");
  while (1)
  {
    /* Get a new sensor event */
    mag.getEvent(&event);
    z = event.magnetic.z;

    // skip first 15 measurements, collect "sampleCount" valid measurements
    cycleCounter++;

    if (cycleCounter > 15)
    {
      // validate
      if ((z > 0.01) | (z < -0.01))
      {
        mean += z;
      }
      else
      {
        // if invalid, repeat the measurement
        cycleCounter--;
      }
    }

    if (cycleCounter >= (sampleCount + 15))
    {
      // divide sum of values by number of values => calculate mean
      mean = mean / sampleCount;
      //Serial.println("Mean: " + String(mean));
      //Serial.println("---- end collectSamples -----------------");
      return mean;
    }

    // delay - read frequency: 13 Hz - 75 ms
    //                         200 Hz - 5 ms
    delay(5);
  }
}

void collectSamples(int sampleCount)
{
  uint16_t cycleCounter = 0;
  sensors_event_t event;

  Serial.println("---- begin collectSamples ---------------");
  /* Get a new sensor event */
  while (1)
  {
    mag.getEvent(&event);
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

    // delay - read frequency: 13 Hz - 75 ms
    //                         200 Hz - 5 ms
    delay(5);
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

  // Initialise the sensor
  while (!mag.begin())
  {
    Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
    delay(1000);
  }
  // Gain
  //mag.enableAutoRange(false);
  //mag.setMagGain(LSM303_MAGGAIN_1_9);
  mag.enableAutoRange(true);

  // Rate
  mag.setMagRate(LSM303_MAGRATE_220);

  return;
}