#include "StreamMote.h"

#include <stdlib.h>

#define MAX_LIFETIME (600)
#define MAX_VELOCITY (2.0)

#define MAX_STEP 35

StreamMote::StreamMote(): ofxMSAParticle() {
    init();
}


StreamMote::StreamMote(ofPoint pos, float m, float d) : ofxMSAParticle(pos, m, d) {
    init();
}

StreamMote::StreamMote(ofxMSAParticle &p) : ofxMSAParticle(p) {
    init();
}

StreamMote::~StreamMote() {
    //dtor
    childMotes.clear();
}
void StreamMote::init() {
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
    addVelocity(ofPoint(ofRandom(-10, 10), ofRandom(-10, 10), ofRandom(-10, 10)));

    childColor = insideColor;
    childColor.a = CHILD_ALPHA;
    timeToBlank = ofRandom(300,MAX_LIFETIME);

    int childCount = ofRandom(30,60);
    for (int i = 0; i<childCount; i++) {
        MoteHistory* newchild = new MoteHistory();
        childMotes.push_back(newchild);
    }
}

void	StreamMote::update() {
    if (frameStep == MAX_STEP) {
        buildNumber++;
        frameStep = 0;
    } else {
        frameStep ++;
    }
    float f = 2;
    if (label == 0) {
        // ===no-one there===
        // adjust movement
//        addVelocity(ofPoint(0, ofRandom(-f, f), 0));
    } else {
        // ===someone there===       // adjust movement
//        float dist = getConstraintDelta()/maxDistWidthSquare;
//        if (dist>=0 && dist<0.01) {
//            addVelocity(ofPoint(ofRandom(-f, f), ofRandom(-f, f), ofRandom(-f, f)));
//        } else {
        setVelocity(ofPoint(ofRandom(-f, f), ofRandom(-f, f), ofRandom(-f, f)));
//        }
    }

    if (childMotes.size() > 0) {
        for (int i = childMotes.size(); i > 1; i--) {
            childMotes[i-1]->update(childMotes[i-2]->getX(), childMotes[i-2]->getY(), _radius-1, true);
        }
        childMotes[0]->update(getX(), getY(), _radius-1, true);
    }
}


void	StreamMote::draw() {

    if (timeToBlank == 0) {
        timeToBlank = MAX_LIFETIME;
        if (pCurrentImage == pGlyph) {
            pCurrentImage = pBlank;
        } else {
            pCurrentImage = pGlyph;
        }
    } else {

        timeToBlank--;
    }

    float f = 2;
    myAlpha = ofLerp(START_ALPHA, STOP_ALPHA, _radius/NODE_MAX_RADIUS);
    ofSetColor(outsideColor.r,outsideColor.g,outsideColor.b, myAlpha);
    ofFill();
    ofCircle(getX(),getY(),_radius);
    ofSetColor(outsideColor.r,outsideColor.g,outsideColor.b, STOP_ALPHA);
    ofNoFill();
    ofCircle(getX(),getY(),_radius);

    string tempString = labelString;
    char bb[8];
    snprintf(bb, 8, "%d", buildNumber);
    tempString.append(bb);
////    itoa(buildNumber, bb, 10);
//    tempString.append(bb);
    if (pMyFont) pMyFont->drawString(tempString, getX()+5,getY()+5);
    for (int i = 0; i < childMotes.size(); i++) {
        childMotes[i]->draw();
    }
}

void StreamMote::setLabel(const unsigned int _label) {
    bool doChange = false;
    if (label != _label)
        doChange = true;
    label = _label;
    if (doChange) {
        float f = 2;
        if (label == 0) {
            // ===no-one there===
            // adjust movement
            addVelocity(ofPoint(0, ofRandom(-f, f), 0));
        } else {
            // ===someone there===
            // adjust movement
            setVelocity(ofPoint(ofRandom(-f, f), ofRandom(-f, f), ofRandom(-f, f)));
        }
    }
}

void StreamMote::setLabelString(const std::string& _labelString) {
    labelString = _labelString;
}

void StreamMote::setFont(ofTrueTypeFont* _pMyFont) {
    pMyFont = _pMyFont;
}


void StreamMote::setInsideColor(ofColor _newColor) {
    insideColor = _newColor;
}
void StreamMote::setOutsideColor(ofColor _newColor) {
    outsideColor = _newColor;
}
void StreamMote::setChildColor(ofColor _newColor) {
    childColor = _newColor;
    childColor.a = childColor.a/2;
    for (int i = 0; i < childMotes.size(); i++) {
        childMotes[i]->setColor(childColor);
    }
}

void StreamMote::setFadeDist(int _distance) {
    maxDistWidthSquare = _distance*_distance;
}


void StreamMote::setGlyph(ofImage* _pnewglyph) {
    pGlyph = _pnewglyph;
    pCurrentImage = pGlyph;
}
void StreamMote::setBlankGlyph(ofImage* _pnewglyph) {
    pBlank = _pnewglyph;
}
