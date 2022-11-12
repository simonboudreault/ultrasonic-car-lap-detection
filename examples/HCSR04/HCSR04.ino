#include <HCSR04.h>

HCSR04 hc(2, 3); // initialisation class HCSR04 (trig pin , echo pin)
// include serial
// #include <Serial.h>

float distance;
float buffer = 0.5; // buffer distance
bool isDistanceSet = false;
int lastObjectDetectionMillis = 0;
int lastDistanceSetMillis = 0;

int detectionCount = 0;

void setDistance(float dist)
{
    distance = dist;
    Serial.println("setting distance");
    isDistanceSet = true;
    lastDistanceSetMillis = millis();
}

void detectObject()
{
    Serial.println("object detected");
    detectionCount++;
    lastObjectDetectionMillis = millis();
}

void setup()
{
    Serial.begin(115200);
}

void loop()
{

    float dist = hc.dist();
    int millis = ::millis();
    if (!isDistanceSet)
    {
        setDistance(dist);
    }
    Serial.println("===");
    Serial.println(detectionCount);
    Serial.println(millis);
    Serial.println(dist);
    Serial.println(distance);
    Serial.println(distance - buffer);

    if (dist < (distance - buffer) && millis - lastObjectDetectionMillis > 2000)
    {
        detectObject();
    }

    // set distance every 2 seconds if no object was detected in the last 2 seconds
    if (millis - lastObjectDetectionMillis > 2000 && millis - lastDistanceSetMillis > 2000)
    {
        setDistance(dist);
    }

    delay(60);
}