#include "StreamMote.h"

#include <stdlib.h>

#define MAX_LIFETIME (600)
#define MAX_VELOCITY (1.0)

#define MAX_STEP 8

StreamMote::StreamMote(): ofxMSAParticle()
{
    init();
}


StreamMote::StreamMote(ofPoint pos, float m, float d) : ofxMSAParticle(pos, m, d)
{
    init();
}

StreamMote::StreamMote(ofxMSAParticle &p) : ofxMSAParticle(p)
{
    init();
}

StreamMote::~StreamMote()
{
    //dtor
    childMotes.clear();
}
void StreamMote::init()
{
    mainStream=true;
    active = true;
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
//    addVelocity(ofPoint(ofRandom(-10, 10), ofRandom(-10, 10), ofRandom(-10, 10)));

    childColor = insideColor;
    childColor.a = CHILD_ALPHA;
    timeToBlank = ofRandom(300,MAX_LIFETIME);

    int childCount = ofRandom(30,60);
    for (int i = 0; i<childCount; i++) {
        MoteHistory* newchild = new MoteHistory();
        childMotes.push_back(newchild);
    }
}

void	StreamMote::update()
{
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
//        setVelocity(ofPoint(ofRandom(-f, f), ofRandom(-f, f), ofRandom(-f, f)));
//        }
    }

    if (childMotes.size() > 0) {
        for (int i = childMotes.size(); i > 1; i--) {
            childMotes[i-1]->update(childMotes[i-2]->getX(), childMotes[i-2]->getY(), _radius-2, true);
        }
        childMotes[0]->update(getX(), getY(), _radius-2, true);
    }
    if ((!mainStream) && (buildNumber==20)) {
        // harakiri
        kill();
        active = false;
    }
}


void	StreamMote::draw()
{

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
    if (label != 0) {
        for (int i = 0; i < childMotes.size(); i++) {
            childMotes[i]->draw();
        }
    }
    if (active) {
        float f = 2;
        myAlpha = ofLerp(START_ALPHA, STOP_ALPHA, _radius/NODE_MAX_RADIUS);
        ofSetColor(outsideColor.r,outsideColor.g,outsideColor.b, myAlpha);
        ofFill();
        ofCircle(getX(),getY(),_radius);
//        if (!mainStream) {
        ofSetColor(outsideColor.r,outsideColor.g,outsideColor.b, STOP_ALPHA);
//        } else {
//            ofSetColor(255,0,0, STOP_ALPHA);
//        }
        ofNoFill();
        ofCircle(getX(),getY(),_radius);

        if (label != 0) {
            labelBuildString = labelString;
            char bb[8];
            snprintf(bb, 8, "%d", buildNumber);
            labelBuildString.append(bb);
            if (pMyFont) pMyFont->drawString(labelBuildString, getX()+5,getY()+5);
        }
    }

}

void StreamMote::setLabel(const unsigned int _label)
{
    float f = 2;
    doChange = false;
    if (_label != 0) {
        if ((label != _label) || (frameStep == MAX_STEP))
            doChange = true;
    }
    label = _label;
    if (label == 0) {
        if (mainStream){
//            addVelocity(ofPoint(-10,-10,-10));
        } else {
            // ===not over someone=== harakiri
            kill();
        }
    } else {

        // adjust movement
//            setVelocity(ofPoint(ofRandom(-f, f), ofRandom(-f, f), ofRandom(-f, f)));
    }

}

void StreamMote::setLabelString(const std::string& _labelString)
{
    labelString = _labelString;
    labelString.erase(std::remove(labelString.begin(), labelString.end(), '\n'), labelString.end());
}

void StreamMote::setFont(ofTrueTypeFont* _pMyFont)
{
    pMyFont = _pMyFont;
}


void StreamMote::setInsideColor(ofColor _newColor)
{
    insideColor = _newColor;
    setChildColor(insideColor);
}
void StreamMote::setOutsideColor(ofColor _newColor)
{
    outsideColor = _newColor;
}
void StreamMote::setChildColor(ofColor _newColor)
{
    childColor = _newColor;
    childColor.a = childColor.a/2;
    for (int i = 0; i < childMotes.size(); i++) {
        childMotes[i]->setColor(childColor);
    }
}

void StreamMote::setFadeDist(int _distance)
{
    maxDistWidthSquare = _distance*_distance;
}

void StreamMote::setGlyph(ofImage* _pnewglyph)
{
    pGlyph = _pnewglyph;
    pCurrentImage = pGlyph;
}

void StreamMote::setBlankGlyph(ofImage* _pnewglyph)
{
    pBlank = _pnewglyph;
}


ofxMSAParticle* StreamMote::doForkMerge()
{
    if (doChange) {
        addVelocity(ofPoint(ofRandom(-MAX_VELOCITY, MAX_VELOCITY), ofRandom(-MAX_VELOCITY, MAX_VELOCITY), 0));
        ofPoint pos = getPosition();
        int offset = getRadius()*2;
        pos.x += offset;
        pos.y += offset;
        pos.z += offset;
        StreamMote* p = new StreamMote(getPosition(), getMass(), getDrag());
        p->setBounce(getBounce())->setRadius(getRadius())->disableCollision()->makeFree();
        p->moveTo(pos, false);
        p->setVelocity(getVelocity());

        p->setMainStream(false);
        p->setFont(pMyFont);
        p->setLabelString(labelBuildString + "_");
        p->setInsideColor(insideColor);
        p->setOutsideColor(outsideColor);
        p->setChildColor(childColor);
        p->setGlyph(pGlyph);
        p->setBlankGlyph(pBlank);
        return p;
    } else {
        return NULL;
    }
}
