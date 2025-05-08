// Heart LED Arduino - Pulsating LED with Button Control

#include <Arduino.h>

const int ledPin = 6;      // LED connected to digital pin 6 (PWM)
const int buttonPin = 2;   // Button connected to digital pin 2

bool ledOn = false;
unsigned long buttonPressTime = 0;
bool buttonPressed = false;

void setup() {
    pinMode(ledPin, OUTPUT);
    pinMode(buttonPin, INPUT_PULLUP); // Use internal pull-up resistor
}

void loop() {
    // Button logic (active LOW)
    if (digitalRead(buttonPin) == LOW) {
        if (!buttonPressed) {
            buttonPressed = true;
            buttonPressTime = millis();
            ledOn = true;
        } else {
            // Check if button held for 2 seconds
            if (millis() - buttonPressTime >= 2000) {
                ledOn = false;
            }
        }
    } else {
        buttonPressed = false;
    }

    // LED control with attenuation (pulsating effect)
    if (ledOn) {
        // Pulsate LED using sine wave
        static unsigned long lastMillis = 0;
        static float angle = 0;
        if (millis() - lastMillis > 15) { // Adjust speed here
            lastMillis = millis();
            angle += 0.08; // Adjust smoothness here
            if (angle > 2 * PI) angle -= 2 * PI;
            int brightness = (sin(angle) * 127.5) + 127.5; // 0-255
            analogWrite(ledPin, brightness);
        }
    } else {
        analogWrite(ledPin, 0); // LED off
    }
}