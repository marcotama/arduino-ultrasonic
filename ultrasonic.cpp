#include <Arduino.h>
#include <Keyboard.h>

int ULTRASONIC_PIN = 12;
int TRIGGER_PIN = 13;

class Ultrasonic
{
    public:
        Ultrasonic(int pin);
        void DistanceMeasure(void);
        long getDistanceInCentimeters(void);
        long getPreviousDistanceInCentimeters(void);
        long getDeltaDistance(void);
        double getRatioDeltaDistance(void);
    private:
        int _pin; //pin number of Arduino that is connected with SIG pin of Ultrasonic Ranger.
        long duration; //the Pulse time received;
        long prevDuration; //the Pulse time received at the previous reading
};
Ultrasonic::Ultrasonic(int pin)
{
    _pin = pin;
}
/*Begin the detection and get the pulse back signal*/
void Ultrasonic::DistanceMeasure(void)
{
    prevDuration = duration;
    
    pinMode(_pin, OUTPUT);
    digitalWrite(_pin, LOW);
    delayMicroseconds(2);
    digitalWrite(_pin, HIGH);
    delayMicroseconds(5);
    digitalWrite(_pin,LOW);
    pinMode(_pin,INPUT);
    duration = pulseIn(_pin,HIGH);
}
/*The measured distance from the range 0 to 400 Centimeters*/
long Ultrasonic::getDistanceInCentimeters(void)
{
    return duration/29/2;   
}
/*The measured distance at the previous reading*/
long Ultrasonic::getPreviousDistanceInCentimeters(void)
{
    return prevDuration/29/2;   
}
/*Difference between current and previous readings*/
long Ultrasonic::getDeltaDistance(void)
{
    return getPreviousDistanceInCentimeters() - getDistanceInCentimeters();   
}
/*Ratio of the difference between current and previous readings divided by previous reading*/
double Ultrasonic::getRatioDeltaDistance(void)
{
    return double(getDeltaDistance()) / getPreviousDistanceInCentimeters();
}
 
Ultrasonic ultrasonic(ULTRASONIC_PIN);
void setup()
{
    Serial.begin(9600);
    Keyboard.begin();
    // Define pin TRIGGER_PIN as input and activate the internal pull-up resistor
   pinMode(TRIGGER_PIN, INPUT_PULLUP);
}
void loop()
{
    long rangeInCentimeters;
    bool triggerButtonPushed;
    ultrasonic.DistanceMeasure();// get the current signal time;
    rangeInCentimeters = ultrasonic.getDistanceInCentimeters();//convert the time to centimeters
    Serial.println(rangeInCentimeters);//0~400cm
    triggerButtonPushed = (digitalRead(TRIGGER_PIN) == LOW);
    
    if (ultrasonic.getRatioDeltaDistance() > 0.3 || triggerButtonPushed)
        Keyboard.write(' ');
    
    
    delay(100);
}