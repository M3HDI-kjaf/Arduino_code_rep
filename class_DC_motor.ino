// Class to run and control DC motors in the Maze

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
