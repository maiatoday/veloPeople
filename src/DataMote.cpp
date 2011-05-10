#include "DataMote.h"


DataMote::DataMote(): ofxMSAParticle() {
    pMyFont = NULL;
    insideColor.a = 255;
    insideColor.r = 130;
    insideColor.g = 0;
    insideColor.b = 160;

    outsideColor.a = 130;
    outsideColor.r = 130;
    outsideColor.g = 130;
    outsideColor.b = 250;

}


DataMote::DataMote(ofPoint pos, float m, float d) : ofxMSAParticle(pos, m, d) {
    pMyFont = NULL;
    insideColor.a = 255;
    insideColor.r = 130;
    insideColor.g = 0;
    insideColor.b = 160;

    outsideColor.a = 130;
    outsideColor.r = 130;
    outsideColor.g = 130;
    outsideColor.b = 250;
    label = 0;
    addVelocity(ofPoint(ofRandom(-10, 10), ofRandom(-10, 10), ofRandom(-10, 10)));
}

DataMote::DataMote(ofxMSAParticle &p) : ofxMSAParticle(p) {
    pMyFont = NULL;
    insideColor.a = 255;
    insideColor.r = 130;
    insideColor.g = 0;
    insideColor.b = 160;

    outsideColor.a = 130;
    outsideColor.r = 130;
    outsideColor.g = 130;
    outsideColor.b = 250;
    label = 0;
    addVelocity(ofPoint(ofRandom(-10, 10), ofRandom(-10, 10), ofRandom(-10, 10)));
}

DataMote::~DataMote() {
    //dtor
}

void	DataMote::update() {
}

void	DataMote::draw() {
    float f = 2;
    //let's draw a circle:
    if (label == 0) {
        //I am over a user or not if flipped
        addVelocity(ofPoint(ofRandom(-f, f), ofRandom(-f, f), ofRandom(-f, f)));

        ofSetColor(insideColor.r,insideColor.g,insideColor.b,insideColor.a);

        ofNoFill();		// draw "filled shapes"
        ofPoint pp = getPosition();
        string vStr = ofToString(getVelocity().x, 1);
//        ofDrawBitmapString(vStr, pp.x,pp.y);
        if (pMyFont) pMyFont->drawString(vStr, pp.x,pp.y);

        ofSetColor(insideColor.r,insideColor.g,insideColor.b,insideColor.a);
        vStr = ofToString(getVelocity().y, 1);
        if (pMyFont) pMyFont->drawString(vStr, pp.x+10,pp.y+10);
    } else {
        // I am drifting aimlessly or not if flipped
        ofSetColor(outsideColor.r,outsideColor.g,outsideColor.b,outsideColor.a);
        ofNoFill();
        setVelocity(ofPoint(ofRandom(-f, f), ofRandom(-f, f), ofRandom(-f, f)));
        ofCircle(getX(),getY(),_radius);
    }
}

void DataMote::setLabel(const unsigned int _label) {
    label = _label;
}


void DataMote::setFont(ofTrueTypeFont* _pMyFont) {
    pMyFont = _pMyFont;
}


void DataMote::setInsideColor(ofColor _newColor) {
    insideColor = _newColor;
}
void DataMote::setOutsideColor(ofColor _newColor) {
    outsideColor = _newColor;
}
