#include "Arduino.h"
#include "../Animation.h"

class AnimationCount : public Animation
{

public:
    void setup()
    {
        currentPixel = 0;
        if (alt)
        {
            currentPixel = 1;
        }
    }

    int getInterval()
    {
        if(alt) return animationBaseInterval / 2;
        return animationBaseInterval;
    }

    bool draw()
    {

        if (!alt)
        {
            pixels.clear();
        }
        switch (currentPixel)
        {
        case 0:
            break;
        case 1:
            setPixelColorToBase(0);
            setPixelColorToBase(2);
            setPixelColorToBase(4);
            break;
        case 2:
            setPixelColorToBase(1);
            setPixelColorToBase(3);
            setPixelColorToBase(5);
            break;
        }
        pixels.show();
        currentPixel++;

        return (currentPixel < 3);
    }

private:
    byte currentPixel;
};
