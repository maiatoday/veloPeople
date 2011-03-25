#include "DataMote.h"


DataMote::DataMote(): ofxMSAParticle()
{

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
    if (label > 0) {
        //I am over a user
        ofSetColor(255,130,0, 128);
        ofFill();		// draw "filled shapes"
        addVelocity(ofPoint(ofRandom(-f, f), ofRandom(-f, f), ofRandom(-f, f)));
        ofPoint pp = getPosition();
//        string xStr = " " + pp.x + " " + pp.y;
//        string xStr = " 1 2";
//        string vStr = " " + getVelocity().x;
//        string vStr = " 1.3";
        string vStr = ofToString(getVelocity().x, 1);
//        ofDrawBitmapString(xStr, pp.x,pp.y);
//        ofDrawBitmapString(vStr, pp.x,pp.y);
        pMyFont->drawString(vStr, pp.x,pp.y);
    } else {
        // I am drifting aimlessly
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
