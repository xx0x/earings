#include "Arduino.h"
#include "../Animation.h"

class AnimationCount : public Animation
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
        if(!alt){
            pixels.clear();
        }
        setPixelColorToBase(currentPixel);
        pixels.show();
        currentPixel++;

        return (currentPixel < NUMPIXELS);
    }

  private:
    byte currentPixel;
};
