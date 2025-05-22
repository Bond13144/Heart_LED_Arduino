// Heart LED Arduino - Pulsating LED with Button Control

#include <Arduino.h>
//#include <FastLED.h> Include FastLED library for LED control
//#include <Adafruit_NeoPixel.h> Include Adafruit NeoPixel library for LED control


uint8_t brightness = 255;
bool fadeIn = true;

#define ledPin 3 // LED connected to digital pin 3 (PWM)
#define buttonPin 2 // Button connected to digital pin 2

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
    {0, 191, 205}, // Violet 6
    {50, 138, 51}, // emerald 7
    {0, 0, 0}  // black, turned off 8
};


void setup() {
    pinMode(ledPin, OUTPUT);
    pinMode(buttonPin, INPUT); // set button external pull-up resistor 
    noInterrupts();
    delayMicroseconds(50); // Reset code(wait for 50 us)
    sendColor(colors[currentColorIndex][0], colors[currentColorIndex][1], colors[currentColorIndex][2]); //Send color data
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
        buttonPressTime = millis();
        nextColorIndex = (currentColorIndex + 1) % 8;
        transitioning = true;
        fadeIn = false; 
    } if (!buttonState && isPressed) {
        isPressed = false;
        unsigned long pressDuration = millis() - buttonPressTime;
        if (pressDuration >= 5000) { // 5 sec+
            currentColorIndex = 8; // set black/off
            fadeIn = false;
            transitioning = false;
            brightness = 0;
        }
    }
    if (!buttonState) {
        isPressed = false;
        unsigned long pressDuration = millis() - buttonPressTime;
        
        if (pressDuration >= 5000) { // long press to turn off
            currentColorIndex = 8;
            fadeIn = false;
            transitioning = false;
            brightness = 0; 
        } else {
            nextColorIndex = (currentColorIndex + 1) % 8;
            transitioning = true;
            fadeIn = false;
        }
    }

    delay(10);

    if (transitioning) { // Slow color change
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
        if (fadeIn) { // Attenuation
            if (brightness < 255) brightness++;
            else fadeIn = false; 
        } else {
            if (brightness > 0) brightness --;
            else fadeIn = true;
        }

    } 

    noInterrupts();
        delayMicroseconds(60);
        sendColor(
            colors[currentColorIndex][0] * brightness / 255, 
            colors[currentColorIndex][1] * brightness / 255, 
            colors[currentColorIndex][2] * brightness / 255);
        interrupts();
        delay(20);

    //if (!buttonState && isPressed) {
    //    unsigned long pressDuration = millis() - buttonPressTime;
    //    isPressed = false;
    //    
    //     else {
    //        currentColorIndex = (currentColorIndex + 1) % 6; // set color
    //        brightness = 255;
    //        fadeIn = true;
    //    }
    //}
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
