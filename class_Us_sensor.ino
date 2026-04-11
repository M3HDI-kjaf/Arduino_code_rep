// Class for the Ultrasound sensor

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
