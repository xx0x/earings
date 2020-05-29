#include "Arduino.h"
#include "../Animation.h"

class AnimationFlashlight : public Animation
{

  public:
    void setup()
    {
    }
    int getInterval()
    {
        return animationBaseInterval;
    }

    bool draw()
    {
        pixels.clear();
        setPixelsColorToBase();
        pixels.show();
        return false;
    }
};
