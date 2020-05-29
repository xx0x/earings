#include "Arduino.h"
#include "../Animation.h"



class AnimationRandomBlink : public Animation
{

  public:
    void setup()
    {
    }

    int getInterval()
    {
        return animationBaseInterval / 2;
    }

    bool draw()
    {
        pixels.clear();    
        if(random(0,10) < 2){
            if(!alt){
                setPixelsColorToBase();
            }else{
                setPixelColorToBase(random(0,NUMPIXELS));
            }
        }   
        pixels.show();
        return false;
    }

};
