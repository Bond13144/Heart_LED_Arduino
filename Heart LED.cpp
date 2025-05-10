// Heart LED Arduino - Pulsating LED with Button Control

#include <Arduino.h>
//#include <FastLED.h> Include FastLED library for LED control
//#include <Adafruit_NeoPixel.h> Include Adafruit NeoPixel library for LED control

const int ledPin = 3;      // LED connected to digital pin 3 (PWM)
const int buttonPin = 2;   // Button connected to digital pin 2

bool ledOn = false;
unsigned long buttonPressTime = 0;
bool buttonPressed = false;

void setup() {
    pinMode(ledPin, OUTPUT);
    //pinMode(buttonPin, INPUT_PULLUP); // Use internal pull-up resistor UPD: Changed to OUTPUT resistor
}


// Function to set up the LED pin and button pin

void loop() {
    // Button logic (active LOW)
    if (digitalRead(buttonPin) == LOW) {
        if (!buttonPressed) {
            buttonPressed = true;
            buttonPressTime = millis();
            ledOn = true;
        } else {
            // Check if button held for 5 seconds
            if (millis() - buttonPressTime >= 5000) {
                ledOn = false;
            }
        } //else {
            // Button pushed for 2 seconds
            //if (millis() - buttonPressTime >= 2000) {}
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


// Function to set color of the LED
#define ledPin 3

//void sendBit(bool bit) {
//    if (bit) {
//        // Send a 1 bit
//        digitalWrite(ledPin, HIGH);
//        delayMicroseconds(0.8);
//        digitalWrite(ledPin, LOW);
//        delayMicroseconds(0.45);
//    } else {
//        // Send a 0 bit
//        digitalWrite(ledPin, HIGH);
//        delayMicroseconds(0.4);
//        digitalWrite(ledPin, LOW);
//        delayMicroseconds(0.85);
//    }
//}

void sendByte(uint8_t byte) {
    for(uint8_t mask =0x80; mask !=0; mask >>= 1) {
        if (byte & mask) {
            // send a 1 bit
            digitalWrite(ledPin, HIGH);
            __asm__ __volatile__(
                "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n" // 0.8us
            ); 
            digitalWrite(ledPin, LOW);
            __asm__ __volatile__(
                "nop\nnop\nnop\nnop\nnop\n" // 0.45us
            ); 
        } else{
            // send a 0 bit
            digitalWrite(ledPin, HIGH);
            __asm__ __volatile__(
                "nop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\nnop\n" // 0.4us
            ); 
            digitalWrite(ledPin, LOW);
            __asm__ __volatile__(
                "nop\nnop\nnop\nnop\nnop\n" // 0.85us
            ); 
        }
    }
}


void sendColor(uint8_t g, uint8_t r, uint8_t b) {
    // Send color data in GRB format
    sendByte(g);
    sendByte(r);
    sendByte(b);
}

void setup() {
    pinMode(ledPin,OUTPUT);
    noInterrupts();
    delayMicroseconds(50); // Reset code(wait for 50 us)
    //sendColor(g, r, b); Send color data
    interrupts();
}

void loop() {
    
}