// Class for the laser sensor (time of flight)

#include "VL53L0X.h" 

class ToF_Sensor
{
  byte address;
  byte pin;
  VL53L0X dis_sensor;

 public:
 ToF_Sensor(byte sensor_address, byte SHT_pin){
    address = sensor_address;
    pin = SHT_pin;
  }

 void begin(){
   pinMode(pin,OUTPUT);
   digitalWrite(pin,LOW);
   delay(10);
  }

  void Change_Adrs(){
   digitalWrite(pin,HIGH);
   delay(10);
   dis_sensor.setAddress((uint8_t)address);
   digitalWrite(pin,LOW);
  }

  void init(){
   digitalWrite(pin,HIGH);
   if(!dis_sensor.init()){ 
     while(1); 
  }
  //continuous measurement in HIGH ACCURACY Mode  
   dis_sensor.setTimeout(500);
   dis_sensor.setMeasurementTimingBudget(200000); 
   dis_sensor.startContinuous(); 
  }

 int distance(){
   return dis_sensor.readRangeContinuousMillimeters(); 
 }
};
