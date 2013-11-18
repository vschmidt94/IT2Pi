/* IT2PI Rocket Telemetry Project   */
/* 2013.10.27                       */
/* Goal: Make Arduino Payload for   */
/*  model rocket.  Payload should   */
/*  measure and record acceleration */
/*  altitude and temperature        */
/*                                  */
/* Some code is taken from portions */
/* Adafruit sample code for BMP085  */
/* and ADXL345 sensors              */
/*                                  */
/* logging handled by Sparkfun      */
/* OpenLog module                   */


/* Include the appropriate support libraries */
#include <Wire.h>                   // adds I2C support
#include <Adafruit_Sensor.h>        // Adafruit universal sensor library
#include <Adafruit_ADXL345.h>       // Adafruit Accelerometer library
#include <Adafruit_BMP085.h>        // Adafruit Altimeter library


/* Assign a unique ID to this sensor at the same time */
Adafruit_ADXL345 accel = Adafruit_ADXL345(12345);  // makes new accelerometer object named accel
Adafruit_BMP085 bmp = Adafruit_BMP085(10085);      // makes new altimeter object named bmp

void displaySensorDetails(void)   
// This is a function that will display the details for the accelerometer in the Serial Monitor
// Useful to see what the sensor resultion is at and min/max values the sensor can report
{
  sensor_t sensor;
  accel.getSensor(&sensor);
  Serial.println("---Accelerometer Detail-------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  // Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" m/s^2");
  // Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" m/s^2");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" m/s^2");  
  Serial.println("------------------------------------");
  
  bmp.getSensor(&sensor);
  Serial.println("---Altimeter (Pressure Sensor) Detail-");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  // Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" m/s^2");
  // Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" m/s^2");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" m/s^2");  
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}

void displayDataRate(void)
// This is a function that is used to display the accelerometer data rate in the Serial Monitor
// Data rate is how fast the sensor is sampling.  60 HZ = 1/sec
// For rocket project, ideally at least 10 samples per second if not more
// Target will be 600 HZ = 10/second, not a valid rate for the sensor so closest
// valid rate is 800 HZ = 13.3 samples per second.
// We will experiment with 1600 HZ, but might be too fast for rest of the hardware
// ** may not be an issue now using OpenLog **

{
  Serial.print  ("ADXL345 Data Rate:    "); 
  
  switch(accel.getDataRate())
  {
    case ADXL345_DATARATE_3200_HZ:
      Serial.print  ("3200 "); 
      break;
    case ADXL345_DATARATE_1600_HZ:
      Serial.print  ("1600 "); 
      break;
    case ADXL345_DATARATE_800_HZ:
      Serial.print  ("800 "); 
      break;
    case ADXL345_DATARATE_400_HZ:
      Serial.print  ("400 "); 
      break;
    case ADXL345_DATARATE_200_HZ:
      Serial.print  ("200 "); 
      break;
    case ADXL345_DATARATE_100_HZ:
      Serial.print  ("100 "); 
      break;
    case ADXL345_DATARATE_50_HZ:
      Serial.print  ("50 "); 
      break;
    case ADXL345_DATARATE_25_HZ:
      Serial.print  ("25 "); 
      break;
    case ADXL345_DATARATE_12_5_HZ:
      Serial.print  ("12.5 "); 
      break;
    case ADXL345_DATARATE_6_25HZ:
      Serial.print  ("6.25 "); 
      break;
    case ADXL345_DATARATE_3_13_HZ:
      Serial.print  ("3.13 "); 
      break;
    case ADXL345_DATARATE_1_56_HZ:
      Serial.print  ("1.56 "); 
      break;
    case ADXL345_DATARATE_0_78_HZ:
      Serial.print  ("0.78 "); 
      break;
    case ADXL345_DATARATE_0_39_HZ:
      Serial.print  ("0.39 "); 
      break;
    case ADXL345_DATARATE_0_20_HZ:
      Serial.print  ("0.20 "); 
      break;
    case ADXL345_DATARATE_0_10_HZ:
      Serial.print  ("0.10 "); 
      break;
    default:
      Serial.print  ("???? "); 
      break;
  }  
  Serial.println(" Hz");  
}

void displayRange(void)
{
  Serial.print  ("Range:         +/- "); 
  
  switch(accel.getRange())
  {
    case ADXL345_RANGE_16_G:
      Serial.print  ("16 "); 
      break;
    case ADXL345_RANGE_8_G:
      Serial.print  ("8 "); 
      break;
    case ADXL345_RANGE_4_G:
      Serial.print  ("4 "); 
      break;
    case ADXL345_RANGE_2_G:
      Serial.print  ("2 "); 
      break;
    default:
      Serial.print  ("?? "); 
      break;
  }  
  Serial.println(" g");  
}

void setup(void) 
{
  delay(3000) // Give openlog time to start
  Serial.begin(9600);
  
  while (!Serial) {
    ; // wait for serial port to connect
  }
  
  
  /* Initialise the sensors */
  Serial.println("Accelerometer Test"); Serial.println("");
  
  // Checks to make sure ADXL is online
  if(!accel.begin())  // This if checks for the ADXL345
    {
      /* There was a problem detecting the ADXL345 ... check your connections */
      Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
      while(1);
    }
    
  Serial.println("Altimeter Test"); Serial.println("");
  
  // Checks to make sure BMP085 is online
  if(!bmp.begin())  // This IF checks for the BMP085
    {
      /* There was a problem detecting the BMP085 ... check your connections */
      Serial.print("Ooops, no BMP085 detected ... Check your wiring or I2C ADDR!");
      while(1);
    }
 // Finished testing if sensors are online

  /* Set the ADXL345 range to whatever is appropriate for your project */
  accel.setRange(ADXL345_RANGE_16_G);
  // displaySetRange(ADXL345_RANGE_8_G);
  // displaySetRange(ADXL345_RANGE_4_G);
  // displaySetRange(ADXL345_RANGE_2_G);
  
  /* Set the ADXL345 data rate */
  accel.setDataRate(ADXL345_DATARATE_800_HZ);
  
  /* Display some basic information on the sensors */
  displaySensorDetails();
  
  /* Display additional ADXL345 settings (outside the scope of sensor_t) */
  displayDataRate();
  displayRange();
  Serial.println("");
}

void loop(void) 
{
  
     /* Get a new sensor event */ 
    sensors_event_t event; 
    accel.getEvent(&event); // get the ADXL345 data for the event
    
    Serial.print(millis());
    Serial.print(",");
    Serial.print(event.acceleration.x);
    Serial.print(",");
    Serial.print(event.acceleration.y);
    Serial.print(",");
    Serial.print(event.acceleration.z);
    Serial.print(",");
    
    bmp.getEvent(&event); // get the BMP085 data for the event
    
  /* First we get the current temperature from the BMP085 */
    float temperature;
    bmp.getTemperature(&temperature);
    
    Serial.print(temperature);
    Serial.print(",");
    
    /* Then convert the atmospheric pressure, SLP and temp to altitude    */
    /* Update this next line with the current SLP for better results      */
    float seaLevelPressure = SENSORS_PRESSURE_SEALEVELHPA;
  
    Serial.println(bmp.pressureToAltitude(seaLevelPressure, event.pressure, temperature)); 
    
  delay(5);
  
  
}
