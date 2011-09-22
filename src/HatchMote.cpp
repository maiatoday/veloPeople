#include "HatchMote.h"

#include <stdlib.h>

#define MAX_LIFETIME (600)
#define MAX_VELOCITY (2.0)

#define MAX_STEP 8
#define MAX_D (10)

HatchMote::HatchMote(): ofxMSAParticle() {
    init();
}


HatchMote::HatchMote(ofPoint pos, float m, float d) : ofxMSAParticle(pos, m, d) {
    init();
}

HatchMote::HatchMote(ofxMSAParticle &p) : ofxMSAParticle(p) {
    init();
}

HatchMote::~HatchMote() {
    //dtor
}
void HatchMote::init() {
    mainStream=true;
    pMyFont = NULL;
    buildNumber = 0;
    frameStep = ofRandom(0, MAX_STEP);
    insideColor.a = 255;
    insideColor.r = 130;
    insideColor.g = 0;
    insideColor.b = 160;

    outsideColor.a = 130;
    outsideColor.r = 130;
    outsideColor.g = 130;
    outsideColor.b = 250;
    myAlpha = 255;
    addVelocity(ofPoint(0, ofRandom(-MAX_VELOCITY, MAX_VELOCITY), 0));

    pGlyph = NULL;
    label = 0;
    childColor = insideColor;
    childColor.a = CHILD_ALPHA;
    timeToBlank = ofRandom(300,MAX_LIFETIME);
    fadeLife = 0;
    dx = ofRandom(-MAX_D, MAX_D);
    dy = ofRandom(-MAX_D, MAX_D);
}

void	HatchMote::update() {
    float f = 5;
    if (frameStep == MAX_STEP) {
        buildNumber++;
        frameStep = 0;

        if (label == 0) {
            addVelocity(ofPoint(0, ofRandom(-f, f), 0));
        }

    } else {
        frameStep ++;
    }

}


void	HatchMote::draw() {

    myAlpha = ofLerp(START_ALPHA, STOP_ALPHA, _radius/NODE_MAX_RADIUS);

    if (label == 0) {
        // only draw if we are not over a player
        if (fadeLife > 0) {
            fadeLife--;
            ofSetColor(childColor.r,childColor.g,childColor.b, myAlpha);
        } else {
            ofSetColor(outsideColor.r,outsideColor.g,outsideColor.b, myAlpha);
        }
        ofFill();
        ofLine(getX()+dx,getY()+dy,getX()-dx,getY()-dy);
    } else {
        ofSetColor(insideColor.r,insideColor.g,insideColor.b, myAlpha);
        ofFill();
        ofRect(getX(),getY(),dx,dy);
    }


}

void HatchMote::setLabel(const unsigned int _label) {
    if (_label == 0) {
        if (label != _label) {
            fadeLife = ofRandom(10,30);
        }
    }
    label = _label;
}

void HatchMote::setLabelString(const std::string& _labelString) {
    labelString = _labelString;
    labelString.erase(std::remove(labelString.begin(), labelString.end(), '\n'), labelString.end());
}

void HatchMote::setFont(ofTrueTypeFont* _pMyFont) {
    pMyFont = _pMyFont;
}


void HatchMote::setInsideColor(ofColor _newColor) {
    insideColor = _newColor;
}
void HatchMote::setOutsideColor(ofColor _newColor) {
    outsideColor = _newColor;
}
void HatchMote::setChildColor(ofColor _newColor) {
    childColor = _newColor;
    childColor.a = childColor.a/2;
}

void HatchMote::setFadeDist(int _distance) {
    maxDistWidthSquare = _distance*_distance;
}

void HatchMote::setGlyph(ofImage* _pnewglyph) {
    pGlyph = _pnewglyph;
    pCurrentImage = pGlyph;
}

void HatchMote::setBlankGlyph(ofImage* _pnewglyph) {
    pBlank = _pnewglyph;
}
