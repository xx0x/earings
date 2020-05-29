#include "Arduino.h"
#include "../Animation.h"



class AnimationGlitch : public Animation
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
        if(!alt || random(0,10) < 2){
            for(byte i = 0;i<NUMPIXELS;i++){
                byte color[3] = {0,0,0};
                createNiceRandomColor(color);
                pixels.setPixelColor(i, color[0],color[1],color[2]);
            }
        }   
        pixels.show();
        return false;
    }

};
