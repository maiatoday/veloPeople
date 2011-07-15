#ifndef MOTESATELLITE_H
#define MOTESATELLITE_H
#include "ofTypes.h"


class MoteSatellite
{
    public:
        MoteSatellite();
        virtual ~MoteSatellite();
        void draw(int x, int y, int radius, ofColor color);
    protected:
    private:
    int deltaX;
    int deltaY;
    int pulseCounter;
    int pulseCounterStep;
    int inc;

};

#endif // MOTESATELLITE_H
