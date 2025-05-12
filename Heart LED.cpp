// Heart LED Arduino - Pulsating LED with Button Control

#include <Arduino.h>
//#include <FastLED.h> Include FastLED library for LED control
//#include <Adafruit_NeoPixel.h> Include Adafruit NeoPixel library for LED control

const int ledPin = 3;      // LED connected to digital pin 3 (PWM)
const int buttonPin = 2;   // Button connected to digital pin 2



uint8_t brightness = 255;
bool fadeIn = true;

#define ledPin 3
#define buttonPin 2

int colorIndex=0;
unsigned long buttonPressTime=0;
bool isPressed=false;
int currentColorIndex = 0;
int nextColorIndex = 0;
bool buttonPressed = false;
bool transitioning = false;

const uint8_t colors[][3] = {  // Color index define
    {255, 0, 0}, // Red 0
    {0,255, 0}, // Green 1 
    {0, 0, 255}, // Blue 2
    {255, 255, 0}, // Yellow 3
    {255, 0, 255}, // Magenta 4
    {0, 255, 255}, // Cyan 5
    {0, 0, 0} // off 6
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

// Function to set up color of the LED

void loop() {
    bool buttonState = digitalRead(buttonPin) == LOW; 
    static unsigned long lastDebounce = 0;
    // debounce check
        if (buttonState && !isPressed && millis() - lastDebounce > 200) {
        isPressed = true;
        lastDebounce = millis();
        nextColorIndex = (currentColorIndex + 1) % 6;
        transitioning = true;
        fadeIn = false; 
    }
    if (!buttonState) {
        isPressed = false;
    }

    delay(10);

    if (transitioning) {
        if (!fadeIn && brightness > 0) {
            brightness --;
        } else if (!fadeIn && brightness == 0) {
            currentColorIndex = nextColorIndex;
            fadeIn = true;
        } else if (fadeIn && brightness < 255) {
            brightness ++;
        } else if (fadeIn && brightness == 255) {
            transitioning = false;
        }
    } else {
        if (fadeIn) {
            if (brightness < 255) brightness++;
            else fadeIn = false; 
        } else {
            if (brightness > 0) brightness --;
            else fadeIn = true;
        }
    }

    if (!buttonState && isPressed) {
        unsigned long pressDuration = millis() - buttonPressTime;
        isPressed = false;
        
        if (pressDuration >= 5000) { // 5 sec+
            colorIndex = 6; // set black/off
            fadeIn = false;
        } else {
            colorIndex = (colorIndex + 1) % 6; // set color
            brightness = 255;
            fadeIn = true;
        }

        noInterrupts();
        delayMicroseconds(60);
        sendColor(
            colors[colorIndex][0] * brightness / 255, 
            colors[colorIndex][1] * brightness / 255, 
            colors[colorIndex][2] * brightness / 255);
        interrupts();
    }
    if (fadeIn) {
        delay(20);
        if (brightness > 0) {
            brightness --;
            noInterrupts();
            sendColor(
                colors[colorIndex][0] * brightness / 255, 
                colors[colorIndex][1] * brightness / 255, 
                colors[colorIndex][2] * brightness / 255
            );
            interrupts();
        } else {
            fadeIn = false;
        }
    } 
}

void sendByte(uint8_t byte) { // Function to send a byte to the LED
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

void sendColor(uint8_t g, uint8_t r, uint8_t b) {   // Send color data in GRB format
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
