#include <HCSR04.h>
#include <Simple5641AS.h>

HCSR04 hc(52, 53); // initialisation class HCSR04 (trig pin , echo pin)
// include serial
// #include <Serial.h>

// segmentPins[] = { a, b, c, d, e, f, g, dot };
const uint8_t segmentPins[] = {4, 6, 3, 10, 9, 2, 5, 13};
// digitSelectionPins[] = { D1, D2, D3, D4 };
const uint8_t digitSelectionPins[] = {12, 11, 8, 7};
Simple5641AS component(segmentPins, digitSelectionPins);

// const uint8_t segmentCodes[] = {
//     B11111100,
//     B01100000,
//     B11011010,
//     B11110010,
//     B01100110,
//     B10110110,
//     B10111110,
//     B11100000,
//     B11111110,
//     B11110110};

uint8_t mappedNumbers[][8] = {
    {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, LOW, LOW},  // 0
    {LOW, HIGH, HIGH, LOW, LOW, LOW, LOW, LOW},      // 1
    {HIGH, HIGH, LOW, HIGH, HIGH, LOW, HIGH, LOW},   // 2
    {HIGH, HIGH, HIGH, HIGH, LOW, LOW, HIGH, LOW},   // 3
    {LOW, HIGH, HIGH, LOW, LOW, HIGH, HIGH, LOW},    // 4
    {HIGH, LOW, HIGH, HIGH, LOW, HIGH, HIGH, LOW},   // 5
    {HIGH, LOW, HIGH, HIGH, HIGH, HIGH, HIGH, LOW},  // 6
    {HIGH, HIGH, HIGH, LOW, LOW, LOW, LOW, LOW},     // 7
    {HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, HIGH, LOW}, // 8
    {HIGH, HIGH, HIGH, HIGH, LOW, HIGH, HIGH, LOW}   // 9
};

float distance;
float buffer = 0.8; // buffer distance
bool isDistanceSet = false;
int lastObjectDetectionMillis = 0;
int lastDistanceSetMillis = 0;
int lastDistanceUpdateMillis = 0;

int detectionCount[] = {0, 0, 0, 0};
int lastCount = 0;

void distanceLoop()
{
    float dist = hc.dist();
    int millis = ::millis();
    if (!isDistanceSet)
    {
        setDistance(dist);
    }
    Serial.println("===");
    // Serial.println(detectionCount);
    Serial.println(millis);
    Serial.println(dist);
    Serial.println(distance);
    Serial.println(distance - buffer);

    if (dist < (distance - buffer) && millis - lastObjectDetectionMillis > 800)
    {
        detectObject();
    }
    else if (millis - lastObjectDetectionMillis > 2000 && millis - lastDistanceSetMillis > 1000)
    {
        setDistance(dist);
    }

    // set distance every 2 seconds if no object was detected in the last 2 seconds
}

void selectDigit(int digit)
{
    for (int i = 0; i < 4; i++)
    {
        // digitalWrite(digitSelectionPins[i], LOW);
        if (i == digit)
        {
            digitalWrite(digitSelectionPins[i], LOW);
        }
        else
        {
            digitalWrite(digitSelectionPins[i], HIGH);
        }
    }
}

void resetSegments()
{
    for (int i = 0; i < 8; i++)
    {
        digitalWrite(segmentPins[i], LOW);
    }
}

void resetNumbers()
{
    for (int i = 0; i < 4; i++)
    {
        selectDigit(i);
        for (int j = 0; j < 8; j++)
        {
            digitalWrite(segmentPins[j], LOW);
        }
    }
}

void writeNumber(int decimal, int number)
{
    selectDigit(decimal);
    for (int i = 0; i < 8; i++)
    {
        digitalWrite(segmentPins[i], mappedNumbers[number][i]);
    }
    delay(1);
}

void writeFourDigitNumber(int numbers[])
{
    for (int i = 0; i < 4; i++)
    {
        writeNumber(i, numbers[i]);
    }
}

void setDistance(float dist)
{
    distance = dist;
    Serial.println("setting distance");
    isDistanceSet = true;
    lastDistanceSetMillis = millis();
}

void incrementDetectionCount()
{
    for (int i = 3; i >= 0; i--)
    {
        if (detectionCount[i] < 9)
        {
            detectionCount[i]++;
            break;
        }
        else
        {
            detectionCount[i] = 0;
        }
    }
}

void detectObject()
{
    Serial.println("object detected");
    incrementDetectionCount();
    lastObjectDetectionMillis = millis();
}

void setup()
{
    Serial.begin(115200);
}
int test[] = {1, 2, 3, 4};
void loop()
{
    // writeFourDigitNumber(test);
    writeFourDigitNumber(detectionCount);
    // resetSegments();

    if (millis() - lastDistanceUpdateMillis > 60)
    {
        distanceLoop();
        lastDistanceUpdateMillis = millis();
    }

    // if (lastCount < detectionCount)
    // {
    // component.shortDisplayNumber(detectionCount);
    // delay(2);
    // delay(60);
    // }
    // else
    // {

    // }
}