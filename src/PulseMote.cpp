#include "PulseMote.h"

PulseMote::PulseMote()
{
    //ctor
}

PulseMote::~PulseMote()
{
    //dtor
}

void	PulseMote::draw()
{

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
    float dist = getConstraintDelta()/maxDistWidthSquare;
    if (dist > 0 && dist < 1) {
        myAlpha = ofLerp(START_ALPHA, STOP_ALPHA, dist);
//            printf("a %f", dist);
    } else {
        myAlpha = outsideColor.a;
    }
    ofSetColor(255,0,0, myAlpha);
    ofFill();
    if (dist>=0 && dist<0.01) {
        addVelocity(ofPoint(ofRandom(-f, f), ofRandom(-f, f), ofRandom(-f, f)));
    } else {
        setVelocity(ofPoint(ofRandom(-f, f), ofRandom(-f, f), ofRandom(-f, f)));
    }
    ofCircle(getX(),getY(),_radius);
}
