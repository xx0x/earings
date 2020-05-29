#ifndef Animation_h
#define Animation_h


#include "Arduino.h"

class Animation
{
  public:
    virtual void setup();
    virtual int getInterval();
    virtual bool draw();
    bool alt = false;

    
    void setPixelsColor(byte r, byte g, byte b)
    {
        for (byte i = 0; i < NUMPIXELS; i++)
        {
            pixels.setPixelColor(i, r, g, b);
        }
    }

    void setPixelColorToBase(byte pixel)
    {
        pixels.setPixelColor(pixel, baseColor[0], baseColor[1], baseColor[2]);
    }

    void setPixelColorToBase(byte pixel, byte divider)
    {
        pixels.setPixelColor(pixel, baseColor[0] / divider, baseColor[1] / divider, baseColor[2] / divider);
    }

    void setPixelsColorToBase()
    {
        setPixelsColor(baseColor[0], baseColor[1], baseColor[2]);
    }

    void setPixelsColorToBase(byte divider)
    {
        setPixelsColor(baseColor[0] / divider, baseColor[1] / divider, baseColor[2] / divider);
    }

};

#endif
