/*
Maze Solver Robot: Right Wall Follow
Mechatronic Project - HFU - BME - WS25/26
By: Mehdi Farzaneh
*/

#include "VL53L0X.h" 
#include "Wire.h"

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

class Us_Sensor
{
  byte Trig;
  byte Echo;

 public:
 Us_Sensor(byte Trig_pin, byte Echo_pin){
    Trig = Trig_pin;
    Echo = Echo_pin;
  }

 void begin(){
    pinMode(Echo, INPUT);
    pinMode(Trig, OUTPUT);
  }

 long distance(){
  digitalWrite(Trig, LOW);
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig, LOW);
  long duration = pulseIn(Echo, HIGH); // timeout 20ms
  return duration * 0.34 / 2; // Convert to mm
 }
};

class DC_Motor
{
  byte RM_1;
  byte RM_2;
  byte RM_enable;
  byte LM_1;
  byte LM_2;
  byte LM_enable;
  byte PWM;

 public:
 DC_Motor(byte rm_1, byte rm_2, byte rm_en, byte lm_1, byte lm_2, byte lm_en){
  RM_1 = rm_1;
  RM_2 = rm_2;
  RM_enable = rm_en;
  LM_1 = lm_1;
  LM_2 = lm_2;
  LM_enable = lm_en;
 }

 void begin(){
  pinMode(RM_1, OUTPUT);
  pinMode(RM_2, OUTPUT);
  pinMode(RM_enable, OUTPUT);
  pinMode(LM_1, OUTPUT);
  pinMode(LM_2, OUTPUT);
  pinMode(LM_enable, OUTPUT);
 }

 void turn_right(byte PWM) {// Turn right 90 deg if no wall on the right
    analogWrite(RM_enable, PWM);
    digitalWrite(RM_1, LOW);
    digitalWrite(RM_2, HIGH);
    analogWrite(LM_enable, PWM);
    digitalWrite(LM_1, HIGH);
    digitalWrite(LM_2, LOW);
 }

 void turn_around(byte PWM) {// Turn 180 deg if wall Ahead and on the Right
    analogWrite(RM_enable, PWM);
    digitalWrite(RM_1, LOW);
    digitalWrite(RM_2, HIGH);
    analogWrite(LM_enable, PWM);
    digitalWrite(LM_1, HIGH);
    digitalWrite(LM_2, LOW);
 }

 void move_forward(byte PWM){// move forward 
    analogWrite(RM_enable, PWM);
    digitalWrite(RM_1, HIGH);
    digitalWrite(RM_2, LOW);
    analogWrite(LM_enable, PWM);
    digitalWrite(LM_1, HIGH);
    digitalWrite(LM_2, LOW);
 }

  void tilt_right(byte PWM) {// Turn right a few deg 
    analogWrite(RM_enable, PWM);
    digitalWrite(RM_1, LOW);
    digitalWrite(RM_2, HIGH);
    analogWrite(LM_enable, PWM);
    digitalWrite(LM_1, HIGH);
    digitalWrite(LM_2, LOW);
 }

  void tilt_left(byte PWM) {// Turn left a few deg 
    analogWrite(RM_enable, PWM);
    digitalWrite(RM_1, HIGH);
    digitalWrite(RM_2, LOW);
    analogWrite(LM_enable, PWM);
    digitalWrite(LM_1, LOW);
    digitalWrite(LM_2, HIGH);
 }

 void Stop() {
  analogWrite(RM_enable, 0);
  analogWrite(LM_enable, 0);
  delay(500);
 }
};

/*   // Uncomments this if ToF sensor is used
ToF_Sensor Left_sen(21,2);  // right ToF sensor
ToF_Sensor Mid_sen(22,4);  // mid ToF sensor
ToF_Sensor Right_sen(23,3);  // left ToF sensor
*/

Us_Sensor Right_sen(2,3);  // right ultrasound sensor
Us_Sensor Mid_sen(4,5);  // mid ultrasound sensor
Us_Sensor Left_sen(6,7);  // left ultrasound sensor

DC_Motor Motor(9,8,10,7,12,11); // pins 8-9 for right_motor and 13-12 for left_motor
                                // pins 10 and 11 for PWM write
#define Photo_Resistor A0

// Initial values
#define Side_Wall 75 // Distance threshold in mm - Needs callibration 
#define Front_Wall 75 // Distance threshold in mm - Needs callibration 
bool Black_Spot = false;
#define Low_Resistance 350 // resistance of the Photo_Resistor sensor when reaches a black surface
#define move_speed 65 // motor speed in PWM

void setup() {
 Wire.begin();
 // configuring the distance sensors
 Right_sen.begin();
 Mid_sen.begin();
 Left_sen.begin();

 /*    // Uncomments this if ToF sensor is used
 Left_sen.Change_Adrs();
 Mid_sen.Change_Adrs();
 Right_sen.Change_Adrs();
 Left_sen.init();
 Mid_sen.init();
 Right_sen.init();
 */
 // configuring the 2 DC motors
 Motor.begin();
 Motor.Stop();

 pinMode(Photo_Resistor,INPUT);
}

void loop() {
  // check if the robot is at the end point
  if (analogRead(Photo_Resistor) > Low_Resistance){
   Black_Spot = true;
  }
  else {
    Black_Spot = false;
  }

  while(!Black_Spot){ // if not at the end point
  // again check if the robot is at the end point
  if (analogRead(Photo_Resistor) > Low_Resistance){
   Black_Spot = true;
   Motor.Stop();
   break;
  }

  int right_dis = Right_sen.distance();
  int front_dis = Mid_sen.distance();
  int left_dis = Left_sen.distance();

  if (front_dis < Front_Wall){
    Motor.Stop();
   }
  // Adjustment to stay in the middle if there is more than 10 mm difference between right & left
  if (right_dis - left_dis > 10){
    Motor.tilt_right(move_speed);
    delay(250); // wait for tilting
    break;
  }
  else if (left_dis - right_dis > 10){
    Motor.tilt_left(move_speed);
    delay(250); // wait for tilting
    break;
  }

  /// Right-hand Wall Following Logic
  if (right_dis > Side_Wall) {
    // Turn right 90 deg if no wall on the Right
    delay(500); // wait to pass the edge
    Motor.turn_right(move_speed);
    delay(1000); // wait for the 90deg turn
    Motor.move_forward(move_speed);
    delay(3000); // wait to move forward one block
  } 
  else if (front_dis < Front_Wall && right_dis < Side_Wall) {
    // Turn 180 deg if wall Ahead and on the Right
    delay(500); // wait to pass the edge
    Motor.turn_around(move_speed);
    delay(2000); // wait for the 180deg turn
  } 
  else {
    // Move forward 
    Motor.move_forward(move_speed);
  }

  delay(200);
  }
}
