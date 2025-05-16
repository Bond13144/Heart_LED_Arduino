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
    {0, 0, 0}  // black, turned off 6
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

}