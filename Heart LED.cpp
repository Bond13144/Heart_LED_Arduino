// Heart LED Arduino - Pulsating LED with Button Control

#include <Arduino.h>
//#include <FastLED.h> Include FastLED library for LED control
//#include <Adafruit_NeoPixel.h> Include Adafruit NeoPixel library for LED control

const int ledPin = 3;      // LED connected to digital pin 3 (PWM)
const int buttonPin = 2;   // Button connected to digital pin 2

bool ledOn = false;
unsigned long buttonPressTime = 0;
bool buttonPressed = false;

#define ledPin 3
#define buttonPin 2

int colorIndex=0;
unsigned long buttonPressTime=0;
bool isPressed=false;

const uint8_t colors[][3] = {
    {255, 0, 0}, // Red
    {0,255, 0}, // Green
    {0, 0, 255}, // Blue
    {255, 255, 0}, // Yellow
    {255, 0, 255}, // Magenta
    {0, 255, 255}, // Cyan
    {0, 0, 0} // off
};

//void setup() {
//    pinMode(ledPin, OUTPUT);
//    //pinMode(buttonPin, INPUT_PULLUP); // Use internal pull-up resistor UPD: Changed to OUTPUT resistor
//}

void setup() {
    pinMode(ledPin, OUTPUT);
    pinMode(buttonPin, INPUT); // set button external pull-up resistor 
    noInterrupts();
    delayMicroseconds(50); // Reset code(wait for 50 us)
    sendColor(colors[colorIndex][0], colors[colorIndex][1], colors[colorIndex][2]); //Send color data
    interrupts();
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
        } 
        } else {
            buttonPressed = false;
        }
 bool buttonState = digitalRead(buttonPin) == LOW; 

    if (buttonState && !isPressed) {
        isPressed = true;
        buttonPressTime = millis();
    }

    if (!buttonState && isPressed) {
        unsigned long pressDuration = millis() - buttonPressTime;
        isPressed = false;
        
        if (pressDuration >= 1000) { // 1 sec+
            colorIndex = 6; // set black
        } else {
            colorIndex = (colorIndex + 1) % 6; 
        }

        noInterrupts();
        delayMicroseconds(60);
        sendColor(colors[colorIndex][0], colors[colorIndex][1], colors[colorIndex][2]);
        interrupts();
    }
}

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


// Function to set color of the LED


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








