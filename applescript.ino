#include "SparkButton/SparkButton.h"
SparkButton b = SparkButton();

#define ON_DIM 40
#define OFF 0

int state[] = {0, 0, 0, 0, 0};
int peak[] = {0, 0, 0, 0, 0};

void setup()
{
    for (int i = 1; i < 5; i++) {
      pinMode(i, INPUT_PULLUP);
    }
    b.begin();
}

void led(int n, int bright) {
    if (n == 1) {
        b.ledOn(1, bright, bright, bright);
        b.ledOn(11, bright, bright, bright);
    } else {
        b.ledOn((n - 1) * 3, bright, bright, bright);
    }
}

void loop()
{
    for (int i = 1; i < 5; i++) {
        int value = digitalRead(i) ? 0 : 1; // Pullup -> invert
        if (value != state[i]) {
            state[i] = value;
            if (value) { // pressed button
                peak[i] = value;
                led(i, ON_DIM);
            } else { // released button
                int pressedCount = 0;
                for (int j = 1; j < 5; j++) {
                    // Anything still pressed?
                    if (state[j]) pressedCount++;
                }
                if (pressedCount) continue;
                
                int peakCount = 0;
                for (int j = 1; j < 5; j++) {
                    // How many were pressed?
                    if (peak[j]) peakCount++;
                    peak[j] = 0; // Reset
                    led(j, OFF);
                }
                if (peakCount == 1) {
                    char buffer[10];
                    sprintf(buffer, "D%i", i);
                    Spark.publish("release", buffer);
                } else if (peakCount == 4) {
                    Spark.publish("release", "ALL");
                }
            }
        }
    }
}
