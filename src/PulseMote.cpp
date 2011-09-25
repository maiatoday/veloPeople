#include "PulseMote.h"

#define MIN_PULSE_COUNTER (0)
#define MAX_PULSE_COUNTER (76)

#define MIN_PULSE_ALPHA (0)
#define MAX_PULSE_ALPHA (190)

#define ALPHA_PULSE_FACTOR ((MAX_PULSE_ALPHA-MIN_PULSE_ALPHA)/(MAX_PULSE_COUNTER-MIN_PULSE_COUNTER))
PulseMote::PulseMote(): DataMote()
{
    init();
}

PulseMote::~PulseMote()
{
    //dtor
}

void PulseMote::init()
{
    DataMote::init();
    inc = 1;
    pulseCounter = 0;
    printf("init pulse mote");
}

void	PulseMote::draw()
{

    pulseCounter += inc;
    if ((pulseCounter >= MAX_PULSE_COUNTER-1) ||  (pulseCounter <= MIN_PULSE_COUNTER)) {
        // change directions if the pulse limit has been reached
        inc = -inc;
    }
    if (label == 0) {
        drawOutside();
    } else {
        drawInside();
    }
}

void PulseMote::drawInside()
{

    float f = 2;
    // I am drifting aimlessly or not if flipped
    ofSetColor(0,0,0, MAX_PULSE_ALPHA-(ALPHA_PULSE_FACTOR*pulseCounter));
    ofNoFill();
    setVelocity(ofPoint(ofRandom(-f, f), ofRandom(-f, f), ofRandom(-f, f)));
    ofCircle(getX(),getY(),_radius+pulseCounter);
}
