#include "MoteSatellite.h"

#include "ofGraphics.h"
#include "ofMath.h"

MoteSatellite::MoteSatellite()
{
    //ctor
    deltaX = ofRandom(-5,5);
    deltaY = ofRandom(-5,5);
    pulseCounter = ofRandom(2,5);
    pulseCounterStep = 0;
    inc = 1;
}

MoteSatellite::~MoteSatellite()
{
    //dtor
}


void MoteSatellite::draw(int x, int y, int radius, ofColor color)
{
    int newX = x+deltaX*pulseCounter;
    int newY = y+deltaY*pulseCounter;

    ofSetColor(200,60,60, 25*pulseCounter);
    ofLine(x,y,newX,newY);
    ofSetColor(color.r,color.g,color.b, color.a);
    ofFill();
    ofCircle(newX,newY,radius);

    pulseCounterStep++; // this slows down the pulse
    if (pulseCounterStep == 24) {
        pulseCounterStep = 0;
        pulseCounter += inc;
        if ((pulseCounter == 24) ||  (pulseCounter == 2)){
            // change directions if the pulse limit has ben reached
            inc = -inc;
        }
    }

}

int MoteSatellite::getPulseCounter()
{
    return pulseCounter;
}

