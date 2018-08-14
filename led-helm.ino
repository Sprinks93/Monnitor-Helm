/*
 * This is the code for the LED Mask showcased through: https://www.youtube.com/watch?v=KEPldnI0fgk
 */

#include "application.h"
#include "neopixel/neopixel.h"

// SYSTEM_MODE(AUTOMATIC);
SYSTEM_MODE(SEMI_AUTOMATIC); // This stops the board from automatically searching for WiFi connectivity

#define PATTERN_INDEX_MAX 9 // The maximum index allowed for the patterns
#define DEBOUNCE_DELAY 300 // The delay for the debouncing of the button to stop false positives

#define LED_PIN D3
#define TOGGLE_PIN D2
#define WIFI_PIN D1

// IMPORTANT: Set Pixel PIN, COUNT, and TYPE
#define PIXEL_PIN A5
#define PIXEL_COUNT 120
#define PIXEL_TYPE WS2812B

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);

// LED Matrix for Mask
/*
000---001---002---003---004---005---006---007---008---009---010---011---012---013---014 // 15
---028---027---026---025---024---023---022---021---020---019---018---017---016---015--- // 14
------029---030---031---032---033---034---035---036---037---038---039---040---041------ // 13
---------053---052---051---050---049---048---047---046---045---044---043---042--------- // 12
------054---055---056---057---058---059---060---061---062---063---064---065---066------ // 13
---------078---077---076---075---074---073---072---071---070---069---068---067--------- // 12
------------079---080---081---082---083---084---085---086---087---088---089------------ // 11
---------------099---098---097---096---095---094---093---092---091---090--------------- // 10
------------------100---101---102---103---104---105---106---107---108------------------ // 9
---------------------------114---113---112---111---110---109--------------------------- // 6
------------------------------115---116---117---118---119------------------------------ // 5
*/

int patternIndex = 0; // This is the index to select which pattern to play
bool patternToggled = true; // Every function must check this between frames in order to allow for a quick response to the button
unsigned long lastDebounceTime; // The last time when the button was triggered

void setup()
{
    attachInterrupt(TOGGLE_PIN, togglePattern, FALLING); // This will allow you to create a button that triggers the patternChange
    attachInterrupt(WIFI_PIN, connect, CHANGE); // This will allow you to create switch that triggers the WiFi connection call

    strip.begin(); // Sends the start protocol for the LEDs.
    strip.show(); // Initialize all pixels to 'off'
    strip.setBrightness(10); // Stops the LEDs from being blinding
}

void loop()
{
    // Unfortunately, particle's build doesn't currently allow for function pointers,
    // so we'll have to do a simple switch case
    switch(patternIndex)
    {
        case 0:
            smile(); // generic rainbow pattern
            break;
        case 1:
            wink(); // an orange jack-o-lantern pattern
            break;
        case 2:
            heartAnimation(200); // an animating red heart
            break;
        case 3:
            hi(); // shows 'hi'
            break;
        case 4:
            bye(); // shows 'bye'
            break;
        case 5:
            music(200); // an animating red heart
            break;
        case 6:
            rainbow(20); // a mask in white with a smile
            break;
        default:
            smile();
            break;
    }

    if(patternToggled){
        patternToggled = false;
    }
}

// Signals the stop of the pattern and increments the pattern index
void togglePattern(){
    if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) //if current time minus the last trigger time is greater than
    {                                                  //the delay (debounce) time, button is completley closed.
        lastDebounceTime = millis();

        //switch was pressed, do whatever you need to here
        patternIndex++;
        if(patternIndex > PATTERN_INDEX_MAX)
        {
            patternIndex = 0;
        }
        patternToggled = true;
    }
}

// Attempts to connect using the normal WiFi connectivity capabilities
void connect() {
    strip.setBrightness(30);
    if (Spark.connected() == false) {
        Spark.connect();
    }
}

// Fills from the start pixel, to the finish pixel with the color
void fillFrom(uint16_t start, uint16_t finish, uint32_t color)
{
    uint16_t i;
    for(i = start; i<=finish; i++)
    {
        strip.setPixelColor(i, color);
    }
}

void rainbow(uint8_t wait) {
    uint16_t i, j;

    for(j=0; j<256 && !patternToggled; j++) {
        for(i=0; i<strip.numPixels(); i++) {
            strip.setPixelColor(i, Wheel((i+j) & 255));
        }
        strip.show();
        delay(wait);
    }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
    if(WheelPos < 85) {
        return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
    } else if(WheelPos < 170) {
        WheelPos -= 85;
        return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    } else {
        WheelPos -= 170;
        return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
}

void heartAnimation(uint8_t wait){
    uint16_t i;
    for(i=0; i<20 && !patternToggled; i++) {
        xsmallHeart();
        strip.show();
        delay(wait*2);

        smallHeart();
        strip.show();
        delay(wait);

        bigHeart();
        strip.show();
        delay(wait*2);
    }
}

void xsmallHeart()
{
    strip.clear();

    fillFrom(48, 49, strip.Color(255, 0, 0));
    fillFrom(46, 47, strip.Color(255, 0, 0));

    strip.setPixelColor(104, strip.Color(255, 0, 0));

    fillFrom(94, 95, strip.Color(255, 0, 0));

    fillFrom(83, 85, strip.Color(255, 0, 0));

    fillFrom(71, 74, strip.Color(255, 0, 0));

    fillFrom(58, 62, strip.Color(255, 0, 0));
}

void smallHeart()
{
    strip.clear();

    strip.setPixelColor(117, strip.Color(255, 0, 0));

    strip.setPixelColor(33, strip.Color(255, 0, 0));
    strip.setPixelColor(34, strip.Color(255, 0, 0));

    fillFrom(48, 50, strip.Color(255, 0, 0));

    strip.setPixelColor(60, strip.Color(255, 0, 0));

    strip.setPixelColor(111, strip.Color(255, 0, 0));
    strip.setPixelColor(112, strip.Color(255, 0, 0));

    fillFrom(103, 105, strip.Color(255, 0, 0));

    fillFrom(93, 96, strip.Color(255, 0, 0));

    fillFrom(82, 86, strip.Color(255, 0, 0));

    fillFrom(70, 75, strip.Color(255, 0, 0));

    strip.setPixelColor(37, strip.Color(255, 0, 0));
    strip.setPixelColor(36, strip.Color(255, 0, 0));

    fillFrom(45, 47, strip.Color(255, 0, 0));

    fillFrom(58, 62, strip.Color(255, 0, 0));
}

void bigHeart()
{
    strip.clear();

    fillFrom(4, 5, strip.Color(255, 0, 0));
    fillFrom(9, 10, strip.Color(255, 0, 0));
    fillFrom(18, 20, strip.Color(255, 0, 0));
    fillFrom(23, 25, strip.Color(255, 0, 0));
    fillFrom(31, 39, strip.Color(255, 0, 0));
    fillFrom(44, 51, strip.Color(255, 0, 0));
    fillFrom(56, 64, strip.Color(255, 0, 0));
    fillFrom(69, 76, strip.Color(255, 0, 0));
    fillFrom(81, 87, strip.Color(255, 0, 0));
    fillFrom(92, 97, strip.Color(255, 0, 0));
    fillFrom(102, 106, strip.Color(255, 0, 0));
    fillFrom(110, 113, strip.Color(255, 0, 0));
    fillFrom(116, 118, strip.Color(255, 0, 0));
}

void hi(){
    strip.clear();
    strip.setPixelColor(customMappingFunction(9, 1), strip.Color(255, 108, 3));
    strip.setPixelColor(customMappingFunction(13, 1), strip.Color(255, 108, 3));
    strip.setPixelColor(customMappingFunction(17, 1), strip.Color(255, 108, 3));
    strip.setPixelColor(customMappingFunction(16, 2), strip.Color(255, 108, 3));
    strip.setPixelColor(customMappingFunction(18, 2), strip.Color(255, 108, 3));
    strip.setPixelColor(customMappingFunction(9, 3), strip.Color(255, 108, 3));
    strip.setPixelColor(customMappingFunction(13, 3), strip.Color(255, 108, 3));
    strip.setPixelColor(customMappingFunction(17, 3), strip.Color(255, 108, 3));
    strip.setPixelColor(customMappingFunction(10, 4), strip.Color(255, 108, 3));
    strip.setPixelColor(customMappingFunction(12, 4), strip.Color(255, 108, 3));
    strip.setPixelColor(customMappingFunction(9, 5), strip.Color(255, 108, 3));
    strip.setPixelColor(customMappingFunction(13, 5), strip.Color(255, 108, 3));
    strip.setPixelColor(customMappingFunction(17, 5), strip.Color(255, 108, 3));
    strip.setPixelColor(customMappingFunction(9, 7), strip.Color(255, 108, 3));
    strip.setPixelColor(customMappingFunction(13, 7), strip.Color(255, 108, 3));
    strip.setPixelColor(customMappingFunction(17, 7), strip.Color(255, 108, 3));
    strip.show();
}

void bye(){
    strip.clear();
    strip.setPixelColor(customMappingFunction(6, 2), strip.Color(112, 2, 255));
    strip.setPixelColor(customMappingFunction(8, 2), strip.Color(112, 2, 255));
    strip.setPixelColor(customMappingFunction(12, 2), strip.Color(112, 2, 255));
    strip.setPixelColor(customMappingFunction(16, 2), strip.Color(112, 2, 255));
    strip.setPixelColor(customMappingFunction(18, 2), strip.Color(112, 2, 255));
    strip.setPixelColor(customMappingFunction(20, 2), strip.Color(112, 2, 255));
    strip.setPixelColor(customMappingFunction(22, 2), strip.Color(112, 2, 255));
    strip.setPixelColor(customMappingFunction(9, 3), strip.Color(112, 2, 255));
    strip.setPixelColor(customMappingFunction(13, 3), strip.Color(112, 2, 255));
    strip.setPixelColor(customMappingFunction(15, 3), strip.Color(112, 2, 255));
    strip.setPixelColor(customMappingFunction(6, 4), strip.Color(112, 2, 255));
    strip.setPixelColor(customMappingFunction(10, 4), strip.Color(112, 2, 255));
    strip.setPixelColor(customMappingFunction(14, 4), strip.Color(112, 2, 255));
    strip.setPixelColor(customMappingFunction(18, 4), strip.Color(112, 2, 255));
    strip.setPixelColor(customMappingFunction(7, 5), strip.Color(112, 2, 255));
    strip.setPixelColor(customMappingFunction(9, 5), strip.Color(112, 2, 255));
    strip.setPixelColor(customMappingFunction(19, 5), strip.Color(112, 2, 255));
    strip.setPixelColor(customMappingFunction(21, 5), strip.Color(112, 2, 255));
    strip.setPixelColor(customMappingFunction(6, 6), strip.Color(112, 2, 255));
    strip.setPixelColor(customMappingFunction(10, 6), strip.Color(112, 2, 255));
    strip.setPixelColor(customMappingFunction(14, 6), strip.Color(112, 2, 255));
    strip.setPixelColor(customMappingFunction(18, 6), strip.Color(112, 2, 255));
    strip.setPixelColor(customMappingFunction(9, 7), strip.Color(112, 2, 255));
    strip.setPixelColor(customMappingFunction(6, 8), strip.Color(112, 2, 255));
    strip.setPixelColor(customMappingFunction(8, 8), strip.Color(112, 2, 255));
    strip.setPixelColor(customMappingFunction(14, 8), strip.Color(112, 2, 255));
    strip.setPixelColor(customMappingFunction(18, 8), strip.Color(112, 2, 255));
    strip.setPixelColor(customMappingFunction(20, 8), strip.Color(112, 2, 255));
    strip.setPixelColor(customMappingFunction(22, 8), strip.Color(112, 2, 255));
    strip.show();
}

void music(uint8_t wait){
    uint16_t i;
    for(i=0; i<20 && !patternToggled; i++) {
        musica();
        strip.show();
        delay(wait*2);

        musicb();
        strip.show();
        delay(wait);

        musicc();
        strip.show();
        delay(wait*2);
    }
}

void musica
