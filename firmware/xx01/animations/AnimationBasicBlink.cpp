#include "Arduino.h"
#include "../Animation.h"

class AnimationBasicBlink : public Animation
{

  public:
    void setup()
    {
        on = false;
    }
    int getInterval()
    {
        return animationBaseInterval;
    }

    bool draw()
    {
        on = !on;

        pixels.clear();
        if (on)
        {
            setPixelsColorToBase();
        }
        pixels.show();
        return on;
    }

  private:
    byte on;
};
