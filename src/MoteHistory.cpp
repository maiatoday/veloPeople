#include "MoteHistory.h"

#include "ofGraphics.h"
#include "ofMath.h"


MoteHistory::MoteHistory()
{
    //ctor
    color.r = 155;
    color.g = 0;
    color.b = 0;
    color.a = 128;
    active = false;
    skipCount = 0;
}

MoteHistory::~MoteHistory()
{
    //dtor
}


void MoteHistory::update(int _x, int _y, int _radius, bool _active)
{
    active = _active;
    if (active) {
        if  (skipCount >0) {
            skipCount--;
        } else {
            x= _x;
            y = _y;
        }
    }
    radius = _radius;
}

void MoteHistory::draw()
{
    if (active && (skipCount ==0)) {
        ofSetColor(color.r,color.g,color.b, color.a);
        ofFill();
        ofCircle(x,y,radius);
    }

}



