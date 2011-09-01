#include "StreamMote.h"
#include "LSystemStrings.h"
#include <stdlib.h>

#define MAX_LIFETIME (100)
#define MAX_VELOCITY (1.0)
#define MIN_L_LENGTH (1)
#define MAX_L_LENGTH (20)

#define MAX_STEP 100

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

    int childCount = ofRandom(30,60);
    for (int i = 0; i<childCount; i++) {
        MoteHistory* newchild = new MoteHistory();
        childMotes.push_back(newchild);
    }
    turtle = Turtle("F", "-", "+");
    turtle.angle = ofRandom(10.0, 40.0);
    turtleLength = MIN_L_LENGTH;
    generationCounter = 0;
    fadefactor = 0;
    startAngle = 10;
}

void	StreamMote::update()
{
    if (frameStep == MAX_STEP) {
        buildNumber++;
        frameStep = 0;
        if (generationCounter < 2) {
            generationCounter++;
            turtleLength = MIN_L_LENGTH;
        }
    } else {
        frameStep ++;
    }
    float f = 2;
    if (label == 0) {
        // ===no-one there===
        // adjust movement
//        addVelocity(ofPoint(0, ofRandom(-f, f), 0));

        turtleLength = MIN_L_LENGTH;
    } else {
        // ===someone there===       // adjust movement
//        float dist = getConstraintDelta()/maxDistWidthSquare;
//        if (dist>=0 && dist<0.01) {
//            addVelocity(ofPoint(ofRandom(-f, f), ofRandom(-f, f), ofRandom(-f, f)));
//        } else {
//        setVelocity(ofPoint(ofRandom(-f, f), ofRandom(-f, f), ofRandom(-f, f)));
//        }

        if (turtleLength < MAX_L_LENGTH) {
            turtleLength++;
        }
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
    }
}


void	StreamMote::draw()
{
    if (label != 0) {
        for (int i = 0; i < childMotes.size(); i++) {
            childMotes[i]->draw();
        }
    }
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

    if (fadefactor != 0) {
        turtle.length = turtleLength;
        switch(generationCounter) {
        default:
        case 0:
            turtle.draw(L1,getX(),getY(), startAngle);
            break;
        case 1:
            turtle.draw(L2,getX(),getY(), startAngle);
            break;
        case 2:
            turtle.draw(L3,getX(),getY(), startAngle);
            break;
        }
    }

    if (label != 0) {
        labelBuildString = labelString;
        char bb[8];
        snprintf(bb, 8, "%d", buildNumber);
        labelBuildString.append(bb);
        if (pMyFont) pMyFont->drawString(labelBuildString, getX()+5,getY()+5);
    }

}

void StreamMote::setLabel(const unsigned int _label)
{
    float f = 2;
    if (label != _label) {
        if (_label == 0) {
            //leaving player
            generationCounter = 2;
            fadefactor = 0; //TODO this should decrement eventually
        } else {
            // moving over player or different player
            startAngle = ofRandom(0,270);
            generationCounter = 0;
            fadefactor = 1;
            turtleLength = MIN_L_LENGTH;
        }
    }
    label = _label;
    if (label == 0) {
        if (mainStream) {
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
    turtle.setMyColor(_newColor);
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


