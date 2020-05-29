#include "Arduino.h"
#include "../Animation.h"



class AnimationFade : public Animation
{

  public:
    void setup()
    {
        currentBrightness = 0;
        brightnessDirection = 1;
        if(alt){
            currentPixel = random(0,NUMPIXELS);
        }
    }

    int getInterval()
    {
        return animationBaseInterval / brightnessModes[brightnessSetting];
    }

    bool draw()
    {
        if(currentBrightness + brightnessDirection > brightnessModes[brightnessSetting]){
            brightnessDirection = -brightnessDirection;
        }
        currentBrightness += brightnessDirection;

        pixels.clear();        

        pixels.setBrightness(currentBrightness);

        if(!alt){
            setPixelsColorToBase();
        }else{
            setPixelColorToBase(currentPixel);
        }

        pixels.show();

              
        return (!(currentBrightness == 0 && brightnessDirection < 0));
    }

  private:
    byte currentBrightness;
    byte currentPixel;
    short brightnessDirection;
};
