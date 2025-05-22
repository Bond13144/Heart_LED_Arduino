// Heart LED project with Adafruit extension for CPP.

#include <Arduino.h>
//#include <Adafruit_NeoPixel.h> Decomment after setup extension!!!!

#define LED_PIN 3
#define NUM_LEDS 1
#define BUTTON_PIN 2

//Adafruit_NeoPixel strip(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);   Decomment after setup extension!!!!

//colors
const uint8_t colors[][3] = {
    {255, 0, 0}, // Green 0 
    {0, 255, 0}, // Red 1
    {0, 0, 255}, // Blue 2
    {255, 255, 0},   // Yellow 3
    {255, 0, 255},   // Magenta 4
    {0, 255, 255},   // Cyan 5
    {0, 191, 205}, // Violet 6
    {50, 138, 51}, // emerald 7
    {0, 0, 0}  // black, turned off 8
};

int currenColorIndex = 0;
int nextColorIndex = 1;

uint8_t brightness = 0;
bool fadeIn = true;
bool transitioning = false;

bool isPressed = false;
unsigned long buttonPressStart = 0;
unsigned long lastDebounce = 0;

void setup() {
    pinMode(BUTTON_PIN, INPUT); // Using external pull-up resistor
    //strip.begin();
    //strip.setBrightness();
    //strip.show();
}


void loop() {
    bool buttonState = digitalRead(BUTTON_PIN) == LOW;
    unsigned long now = millis();

    if (buttonState && !isPressed && now - lastDebounce > 200) {
        isPressed = true;
        buttonPressStart = now;
        lastDebounce = now;
    }

    if (!buttonState && isPressed) {
        unsigned long pressDuration = now - buttonPressStart;
        isPressed = false;
        
        if (pressDuration >= 5000) {
            currenColorIndex = 8;
            transitioning = false;
            fadeIn = false;
        } else {
            nextColorIndex = (currenColorIndex + 1) & 8;
            transitioning = true;
            fadeIn = false;
        } 
    }
    if (transitioning) {
        if (!fadeIn && brightness > 0) {
            brightness --;
        } else if (!fadeIn && brightness == 0) {
            currenColorIndex = nextColorIndex;
            fadeIn = true;
        } else if (!fadeIn && brightness < 255) {
            brightness++;
        } else if (!fadeIn && brightness == 255) {
            transitioning = false;
        }
    } else {
        if (fadeIn) {
            if (brightness < 255) brightness++;
            else fadeIn = false;
        } else {
            if (brightness > 0) brightness--;
            else fadeIn = true;
        }
    }

    setColor(colors[currenColorIndex][0], colors[currenColorIndex][1], colors[currenColorIndex][2], brightness);
    delay(20);
}


void setColor(uint8_t r, uint8_t g, uint8_t b, uint8_t brightness) {
    uint8_t scaledR = r * brightness / 255;
    uint8_t scaledG = g * brightness / 255;
    uint8_t scaledB = b * brightness / 255;
    //strip.setpixelcolor(0, strip.color(scaledR, scaledG, scaledB));
    //strip.show();
}