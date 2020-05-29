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
        pixels.setPixelColor(currentPixel % 3, 255, 0, 0);
        pixels.setPixelColor((currentPixel + 1) % 3, 0, 255, 0);
        pixels.setPixelColor((currentPixel + 2) % 3, 0, 0, 255);
        pixels.show();
        currentPixel++;

        return (currentPixel < NUMPIXELS);
    }

  private:
    byte currentPixel;
};
