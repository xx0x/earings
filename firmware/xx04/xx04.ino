// XX0X EARINGS SOURCE CODE
// for MCU: ATtiny85
// author: Vaclav Mach (vaclavmach@hotmail.cz, info@xx0x.cz)
// project homepage: www.xx0x.cz

#include <avr/sleep.h>
#include <avr/interrupt.h>
#include <Adafruit_NeoPixel.h>
#include "functions.h"
#include <tiny_IRremote.h>
#include <EEPROM.h>
#include "ir_codes.h"

// PORT DEFINITIONS
#define PIN_ENABLE 0
#define PIN_IR 2
#define PIN_NEOPIXELS 3
#define PIN_BTN 4

// IR STUFF
IRrecv irrecv(PIN_IR);
decode_results results;

// NEOPIXELS STUFF
#define NUMPIXELS 6
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN_NEOPIXELS, NEO_GRB + NEO_KHZ800);

// BRIGHTNESS AND COLORS STUFF
#define BRIGHTNESS_MODES 8
#define BRIGHTNESS_SETTING 3
byte baseColor[3] = {255, 0, 0};
byte brightnessSetting = BRIGHTNESS_SETTING;
byte brightnessModes[BRIGHTNESS_MODES] = {4, 12, 16, 22, 40, 64, 96, 128};
bool randomColor = true;

// LOCK ETC
bool locked = false;
#define LOCK_FLASH_ON 50
#define LOCK_FLASH_OFF 200

// BPM STUFF
#define WAIT_BETWEEN 3000
unsigned int intervalSamples = 0;
byte intervalCount = 0;
unsigned long lastTimeButtonBpm = 0;

// BUTTON STUFF
bool buttonPressed = false;
bool ignoreNextPress = false;
unsigned long lastTimeButtonPress = 0;
#define BUTTON_LONG_PRESS 1000

// ANIMATIONS
#define ANIMATION_MODES 11
#define ANIMATION_INTERVAL 400
bool forceRedraw = false;
bool animationEnabled = true;
bool animationEnded = true;
unsigned int animationBaseInterval = ANIMATION_INTERVAL;
unsigned int animationWait = 0;
unsigned long lastTime = 0;
int animationMode = 0;

#include "Animation.h"
#include "animations/AnimationBasicBlink.cpp"
#include "animations/AnimationCount.cpp"
#include "animations/AnimationFade.cpp"
#include "animations/AnimationFlashlight.cpp"
#include "animations/AnimationGlitch.cpp"
#include "animations/AnimationRandomBlink.cpp"
#include "animations/AnimationRgb.cpp"

Animation *animation;

void setup()
{
    pinMode(PIN_ENABLE, OUTPUT);
    pinMode(PIN_BTN, INPUT_PULLUP);

    digitalWrite(PIN_ENABLE, HIGH);

    irrecv.enableIRIn();
    pixels.begin();

    useDefaults();
    updateBrightness();
    reloadMode();
}

void loop()
{
    buttonCheck();
    irCheck();
    drawPixels();
}

void reloadMode()
{
    delete animation; // clears memory from previous animation

    switch (animationMode)
    {
    case 0:
        animation = new AnimationFlashlight();
        break;
    case 1:
        animation = new AnimationBasicBlink();
        break;
    case 2:
        animation = new AnimationCount();
        break;
    case 3:
        animation = new AnimationCount();
        animation->alt = true;
        break;
    case 4:
        animation = new AnimationFade();
        break;
    case 5:
        animation = new AnimationFade();
        animation->alt = true;
        break;
    case 6:
        animation = new AnimationRandomBlink();
        break;
    case 7:
        animation = new AnimationRandomBlink();
        animation->alt = true;
        break;
    case 8:
        animation = new AnimationRgb();
        break;
    case 9:
        animation = new AnimationGlitch();
        break;
    case 10:
        animation = new AnimationGlitch();
        animation->alt = true;
        break;
    }
}

void currentModeSetup()
{
    if (randomColor)
    {
        createNiceRandomColor(baseColor);
    }

    animationEnded = false;
    animation->setup();
    animationWait = animation->getInterval();
    updateBrightness();
}

void processIrCode(long code)
{
    switch (code)
    {
    case STB_CHANNEL_1:
    case STB_RED:
    case IR_RED:
    case IR2_RED:
        setBaseColor(255, 0, 0);
        break;
    case STB_CHANNEL_2:
    case STB_GREEN:
    case IR_GREEN:
    case IR2_GREEN:
        setBaseColor(0, 255, 0);
        break;
    case STB_CHANNEL_3:
    case STB_BLUE:
    case IR_BLUE:
    case IR2_BLUE:
        setBaseColor(0, 0, 255);
        break;
    case STB_CHANNEL_4:
    case IR_WHITE:
    case IR2_WHITE:
        setBaseColor(128, 128, 128);
        break;
    case STB_CHANNEL_5:
    case IR_ORANGE:
    case IR2_ORANGE:
        setBaseColor(255, 100, 0);
        break;
    case IR_GREEN2:
    case IR2_GREEN2:
        setBaseColor(0, 255, 50);
        break;
    case IR_BLUE2:
    case IR2_BLUE2:
        setBaseColor(0, 50, 255);
        break;
    case IR_ORANGE2:
    case IR2_ORANGE2:
        setBaseColor(255, 140, 0);
        break;
    case IR_BLUE3:
    case IR2_BLUE3:
        setBaseColor(40, 40, 255);
        break;
    case IR_DARKBLUE:
    case IR2_DARKBLUE:
        setBaseColor(0, 0, 50);
        break;
    case STB_YELLOW:
    case IR_YELLOW:
    case IR2_YELLOW:
    case STB_CHANNEL_6:
        setBaseColor(255, 180, 0);
        break;
    case IR_BLUE4:
    case IR2_BLUE4:
        setBaseColor(0, 128, 128);
        break;
    case STB_CHANNEL_7:
    case IR_PURPLE:
    case IR2_PURPLE:
        setBaseColor(150, 0, 150);
        break;
    case STB_CHANNEL_8:
    case IR_YELLOW2:
    case IR2_YELLOW2:
        setBaseColor(200, 200, 0);
        break;
    case IR_GREEN3:
    case IR2_GREEN3:
        setBaseColor(0, 150, 100);
        break;
    case STB_CHANNEL_9:
    case IR_PINK:
    case IR2_PINK:
        setBaseColor(180, 0, 100);
        break;
    case IR_ON:
    case IR2_ON:
        brightnessSetting = BRIGHTNESS_MODES - 1;
        updateBrightness();
        break;
    case IR_OFF:
    case IR2_OFF:
        brightnessSetting = 0;
        updateBrightness();
        break;
    case STB_VOLUME_UP:
    case IR_PLUS:
    case IR2_PLUS:
        if (brightnessSetting < BRIGHTNESS_MODES - 1)
        {
            brightnessSetting++;
            updateBrightness();
        }
        break;
    case STB_VOLUME_DOWN:
    case IR_MINUS:
    case IR2_MINUS:
        if (brightnessSetting > 0)
        {
            brightnessSetting--;
            updateBrightness();
        }
        break;

    case STB_CHANNEL_0:
    case IR_SMOOTH:
    case IR2_SMOOTH:
        randomColor = true;
        break;

    case STB_PAGE_UP:
    case STB_CHANNEL_UP:
    case IR_FLASH:
    case IR2_FLASH:
        animationMode++;
        forceRedraw = true;
        if (animationMode >= ANIMATION_MODES)
        {
            animationMode = 0;
        }
        reloadMode();
        break;

    case STB_CHANNEL_DOWN:
    case STB_PAGE_DOWN:
        if (animationMode > 0)
        {
            animationMode--;
        }
        else
        {
            animationMode = ANIMATION_MODES - 1;
        }
        forceRedraw = true;
        reloadMode();
        break;

    case STB_OK:
    case IR_STROBE:
    case IR2_STROBE:
        calculateBpm();
        break;

    case IR_FADE:
    case IR2_FADE:
        useDefaults();
        break;

    case STB_FAST_FORWARD:
        setBaseInterval(50);
        break;

    case STB_REWIND:
        setBaseInterval(400);
        break;
    case STB_PLAY:
        setBaseInterval(200);
        break;
    case STB_STOP:
        setBaseInterval(1000);
        break;
    }
}

void useDefaults()
{
    forceRedraw = true;
    randomColor = true;
    animationEnabled = true;
    animationMode = 0;
    animationBaseInterval = ANIMATION_INTERVAL;
    brightnessSetting = BRIGHTNESS_SETTING;
    updateBrightness();
    reloadMode();
}

void setBaseInterval(unsigned int i)
{
    animationEnabled = true;
    forceRedraw = true;
    animationBaseInterval = i;
}

void setBaseColor(byte r, byte g, byte b)
{
    baseColor[0] = r;
    baseColor[1] = g;
    baseColor[2] = b;
    forceRedraw = true;
    randomColor = false;
}

void updateBrightness()
{
    pixels.setBrightness(brightnessModes[brightnessSetting]);
}


/**
 * Draws the current animation.
 */
void drawPixels()
{
    if (!animationEnabled)
    {
        return;
    }

    if (forceRedraw || animationEnded)
    {
        currentModeSetup();
    }

    if (!forceRedraw && millis() - lastTime < animationWait)
    { // check whether is the time to do it
        return;
    }

    forceRedraw = false;
    lastTime = millis();
    if (!animation->draw())
    {
        animationEnded = true;
    }
}

/**
 * Checks the IR sensor for signals.
 */
void irCheck()
{
    if (irrecv.decode(&results))
    {
        if (!locked)
        {
            processIrCode(results.value);
        }
        irrecv.resume();
    }

    /* 
     * if too long analysing bpm, enable animations again without setting a new interval
     */
    if (millis() - lastTimeButtonBpm > WAIT_BETWEEN && !animationEnabled)
    {
        animationEnabled = true;
    }
}

/**
 * Sets all pixels to a specific color.
 */
void setPixelsToColor(byte r, byte g, byte b)
{
    pixels.clear();
    for (byte i = 0; i < NUMPIXELS; i++)
    {
        pixels.setPixelColor(i, r, g, b);
    }
    pixels.show();
}

/**
 * Locks the device and flashes red.
 */
void doLock()
{
    locked = true;
    setPixelsToColor(255, 0, 0);
    delay(LOCK_FLASH_ON);
    pixels.clear();
    pixels.show();
    delay(LOCK_FLASH_OFF);
    setPixelsToColor(255, 0, 0);
    delay(LOCK_FLASH_ON);
    pixels.clear();
    pixels.show();
    delay(LOCK_FLASH_OFF);
}

/**
 * Unlocks the device and flashes green.
 */
void doUnlock()
{
    locked = false;
    setPixelsToColor(0, 255, 0);
    delay(LOCK_FLASH_ON);
    pixels.clear();
    pixels.show();
    delay(LOCK_FLASH_OFF);
    setPixelsToColor(0, 255, 0);
    delay(LOCK_FLASH_ON);
    pixels.clear();
    pixels.show();
    delay(LOCK_FLASH_OFF);
}

/**
 * Checks the button for presses - if pressed for long time, sleep
 */
void buttonCheck()
{
    if (!digitalRead(PIN_BTN) && !buttonPressed)
    {
        buttonPressed = true;
        lastTimeButtonPress = millis();
    }

    if (digitalRead(PIN_BTN) && buttonPressed)
    {
        buttonPressed = false;
        if (!ignoreNextPress)
        {
            if (locked)
            {
                doUnlock();
            }
            else
            {
                doLock();
            }
        }
        ignoreNextPress = false;
    }

    if (buttonPressed && !ignoreNextPress)
    {
        if ((millis() - lastTimeButtonPress) >= BUTTON_LONG_PRESS)
        {
            buttonPressed = false;
            pixels.clear();
            pixels.show();
            digitalWrite(PIN_ENABLE, false);
            sleep();
            digitalWrite(PIN_ENABLE, true);
            locked = false;
            forceRedraw = true;
            ignoreNextPress = true;
        }
    }
}

/*
 * Reads "BPM" from the button pressings - it's not real "BPM" metering, just an interval metering
 */
void calculateBpm()
{
    long interval = millis() - lastTimeButtonBpm;

    if (interval < WAIT_BETWEEN && lastTimeButtonBpm > 0)
    { // if not first press
        intervalCount++;
        intervalSamples += interval;
        setBaseInterval(intervalSamples / intervalCount / 2); // dividing interval by 2
    }
    else
    { // first press
        intervalSamples = 0;
        intervalCount = 0;
        animationEnabled = false;
        pixels.clear();
        pixels.show();
    }
    lastTimeButtonBpm = millis();
}

ISR(PCINT0_vect)
{
}

/**
 * Falling to sleep and waking up when button pressed.
 */
void sleep()
{
    GIMSK |= _BV(PCIE);                  // Enable Pin Change Interrupts
    PCMSK |= _BV(PCINT4);                // Use PB4 as interrupt pin
    ADCSRA &= ~_BV(ADEN);                // ADC off
    set_sleep_mode(SLEEP_MODE_PWR_DOWN); // replaces above statement
    sleep_enable();                      // Sets the Sleep Enable bit in the MCUCR Register (SE BIT)
    sei();                               // Enable interrupts
    sleep_cpu();                         // sleep

    // NOW WAKING UP
    cli();                 // Disable interrupts
    PCMSK &= ~_BV(PCINT4); // Turn off PB4 as interrupt pin
    sleep_disable();       // Clear SE bit
    ADCSRA |= _BV(ADEN);   // ADC on
    sei();                 // Enable interrupts

    // CHECK WHETHER THE BUTTON IS PRESSED - if not, sleep again
    bool slp = false;
    for (byte i = 0; i < 4; i++)
    {
        delay(250);
        if (digitalRead(PIN_BTN))
        {
            slp = true;
            break;
        }
    }
    if (slp)
    {
        sleep();
    }
}
