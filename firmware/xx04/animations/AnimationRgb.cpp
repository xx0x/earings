#include "Arduino.h"
#include "../Animation.h"

class AnimationRgb : public Animation
{

  public:
    void setup()
    {
        currentPixel = 0;
    }

    int getInterval()
    {
        return animationBaseInterval;
    }

    bool draw()
    {
        pixels.clear();
        pixels.setPixelColor(currentPixel % NUMPIXELS, 255, 0, 0);
        pixels.setPixelColor((currentPixel + 1) % NUMPIXELS, 255, 0, 0);
        pixels.setPixelColor((currentPixel + 2) % NUMPIXELS, 0, 255, 0);
        pixels.setPixelColor((currentPixel + 3) % NUMPIXELS, 0, 255, 0);
        pixels.setPixelColor((currentPixel + 4) % NUMPIXELS, 0, 0, 255);
        pixels.setPixelColor((currentPixel + 5) % NUMPIXELS, 0, 0, 255);
        pixels.show();
        currentPixel+=2;

        return (currentPixel < NUMPIXELS);
    }

  private:
    byte currentPixel;
};
