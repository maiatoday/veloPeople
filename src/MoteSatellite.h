#ifndef MOTESATELLITE_H
#define MOTESATELLITE_H
#include "ofTypes.h"


class MoteSatellite
{
    public:
        MoteSatellite();
        virtual ~MoteSatellite();
        void draw(int x, int y, int radius, ofColor color);
        int getPulseCounter();
    protected:
    private:
    int deltaX;
    int deltaY;
    int pulseCounter;
    int inc;
    int amplitude;

};

#endif // MOTESATELLITE_H
