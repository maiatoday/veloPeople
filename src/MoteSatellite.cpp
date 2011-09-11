#include "MoteSatellite.h"

#include "ofGraphics.h"
#include "ofMath.h"

#define MIN_PULSE_COUNTER (0)
#define MAX_PULSE_COUNTER (24)
//const float easeFactor[MAX_PULSE_COUNTER] = {0,
//        0.06537002,
//        0.1304603991,
//        0.1949926928,
//        0.3212823638,
//        0.3824994973,
//        0.4420803689,
//        0.4997701026,
//        0.5553219127,
//        0.6084981586,
//        0.6590713624,
//        0.7068251811,
//        0.7515553329,
//        0.7930704704,
//        0.8311929997,
//        0.8657598395,
//        0.8966231194,
//        0.9236508119,
//        0.9467272977,
//        0.9657538598,
//        0.9806491061,
//        0.9913493175,
//        0.9978087205
//                                            };
const float easeFactor[MAX_PULSE_COUNTER] = {

    0,
    0.04,
    0.08,
    0.12,
    0.16,
    0.2,
    0.24,
    0.28,
    0.32,
    0.36,
    0.4,
    0.44,
    0.48,
    0.52,
    0.56,
    0.6,
    0.64,
    0.68,
    0.72,
    0.76,
    0.8,
    0.84,
    0.88,
    0.92
};



MoteSatellite::MoteSatellite()
{
    //ctor
    deltaX = ofRandom(-5,5);
    deltaY = ofRandom(-5,5);
    //Different Satellite motes have slightly different start points
//    pulseCounter = ofRandom(MIN_PULSE_COUNTER,MIN_PULSE_COUNTER+3);
    pulseCounter = 0;
    amplitude = ofRandom(5,15);
    inc = 1;
}

MoteSatellite::~MoteSatellite()
{
    //dtor
}


void MoteSatellite::draw(int x, int y, int radius, ofColor color)
{
    int newX = x+deltaX*easeFactor[pulseCounter]*amplitude;
    int newY = y+deltaY*easeFactor[pulseCounter]*amplitude;

//    ofSetColor(color.r,color.g,color.b, 20+10*pulseCounter);
    ofSetColor(255,255, 255, 20+10*pulseCounter);
    ofLine(x+radius,y+radius,newX,newY);
//    ofSetColor(color.r,color.g,color.b, color.a);
//    ofFill();
//    ofCircle(newX,newY,radius);

    int rectWidth = radius*2;
    int rectHeight = radius*2;
    ofNoFill();
    ofSetColor(255,255, 255, 255);
    ofRect(newX, newY, rectWidth+2, rectHeight+2);
    ofFill();
    ofSetColor(0,0, 0, 255);
    ofRect(newX+1, newY+1, rectWidth, rectHeight*2);
    ofNoFill();
    ofSetColor(255,255, 255, 255);
    ofRect(newX, newY+5, rectWidth+2, rectHeight-3);

    pulseCounter += inc;
    if ((pulseCounter == MAX_PULSE_COUNTER-1) ||  (pulseCounter == MIN_PULSE_COUNTER)) {
        // change directions if the pulse limit has been reached
        inc = -inc;
    }

}

int MoteSatellite::getPulseCounter()
{
    return pulseCounter;
}

