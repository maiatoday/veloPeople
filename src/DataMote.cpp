#include "DataMote.h"


DataMote::DataMote(): ofxMSAParticle()
{
    pMyFont = NULL;

}


DataMote::DataMote(ofPoint pos, float m, float d) : ofxMSAParticle(pos, m, d)
{
    label = 0;
    addVelocity(ofPoint(ofRandom(-10, 10), ofRandom(-10, 10), ofRandom(-10, 10)));
}

DataMote::DataMote(ofxMSAParticle &p) : ofxMSAParticle(p)
{
    label = 0;
    addVelocity(ofPoint(ofRandom(-10, 10), ofRandom(-10, 10), ofRandom(-10, 10)));


}

DataMote::~DataMote()
{
    //dtor
}

void	DataMote::update()
{
}

void	DataMote::draw()
{
    float f = 2;
    //let's draw a circle:
    if (label == 0) {
        //I am over a user or not if flipped
        addVelocity(ofPoint(ofRandom(-f, f), ofRandom(-f, f), ofRandom(-f, f)));

        ofSetColor(255,130,0, 160);
        ofNoFill();		// draw "filled shapes"
        ofPoint pp = getPosition();
        string vStr = ofToString(getVelocity().x, 1);
//        ofDrawBitmapString(vStr, pp.x,pp.y);
        if (pMyFont) pMyFont->drawString(vStr, pp.x,pp.y);

        ofSetColor(160,130,100, 160);
        vStr = ofToString(getVelocity().y, 1);
        if (pMyFont) pMyFont->drawString(vStr, pp.x+10,pp.y+10);
    } else {
        // I am drifting aimlessly or not if flipped
        ofSetColor(130,130, 130, 250);
        ofNoFill();
        setVelocity(ofPoint(ofRandom(-f, f), ofRandom(-f, f), ofRandom(-f, f)));
        ofCircle(getX(),getY(),_radius);
    }
}

void DataMote::setLabel(const unsigned int _label)
{
    label = _label;
}


void DataMote::setFont(ofTrueTypeFont* _pMyFont)
{
    pMyFont = _pMyFont;
}
