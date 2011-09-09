#include "DataMote.h"

#define MAX_LIFETIME (600)

#define MAX_FADE_COUNT (48)
DataMote::DataMote(): ofxMSAParticle()
{
    pMyFont = NULL;
    insideColor.a = 255;
    insideColor.r = 130;
    insideColor.g = 0;
    insideColor.b = 160;

    outsideColor.a = 130;
    outsideColor.r = 130;
    outsideColor.g = 130;
    outsideColor.b = 250;
    myAlpha = 255;
    label = 0;
    maxDistWidthSquare = MAX_DIST_SQR;

    pGlyph = NULL;


    childColor = insideColor;
    childColor.a = CHILD_ALPHA;
    timeToBlank = ofRandom(300,MAX_LIFETIME);
    int childCount = ofRandom(2,7);
    for (int i = 0; i<childCount; i++) {
        MoteSatellite* newchild = new MoteSatellite();
        childMotes.push_back(newchild);
    }


}


DataMote::DataMote(ofPoint pos, float m, float d) : ofxMSAParticle(pos, m, d)
{
    pMyFont = NULL;
    insideColor.a = 255;
    insideColor.r = 130;
    insideColor.g = 0;
    insideColor.b = 160;

    outsideColor.a = 130;
    outsideColor.r = 130;
    outsideColor.g = 130;
    outsideColor.b = 250;
    myAlpha = 255;
    maxDistWidthSquare = MAX_DIST_SQR;

    pGlyph = NULL;
    pCurrentImage = NULL;
    label = 0;
    addVelocity(ofPoint(ofRandom(-10, 10), ofRandom(-10, 10), ofRandom(-10, 10)));

    childColor = insideColor;
    childColor.a = CHILD_ALPHA;
    timeToBlank = ofRandom(300,MAX_LIFETIME);
    int childCount = ofRandom(2,7);
    for (int i = 0; i<childCount; i++) {
        MoteSatellite* newchild = new MoteSatellite();
        childMotes.push_back(newchild);
    }
}

DataMote::DataMote(ofxMSAParticle &p) : ofxMSAParticle(p)
{
    pMyFont = NULL;
    insideColor.a = 255;
    insideColor.r = 130;
    insideColor.g = 0;
    insideColor.b = 160;

    outsideColor.a = 130;
    outsideColor.r = 130;
    outsideColor.g = 130;
    outsideColor.b = 250;
    myAlpha = 255;
    maxDistWidthSquare = MAX_DIST_SQR;

    pGlyph = NULL;
    label = 0;
    addVelocity(ofPoint(ofRandom(-10, 10), ofRandom(-10, 10), ofRandom(-10, 10)));

    childColor = insideColor;
    childColor.a = CHILD_ALPHA;
    timeToBlank = ofRandom(300,MAX_LIFETIME);

    int childCount = ofRandom(2,7);
    for (int i = 0; i<childCount; i++) {
        MoteSatellite* newchild = new MoteSatellite();
        childMotes.push_back(newchild);
    }
}

DataMote::~DataMote()
{
    //dtor
    childMotes.clear();
}

void	DataMote::update()
{
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
}
void	DataMote::draw()
{

    if (label == 0) {
        drawOutside();
    } else {
        drawInside();
    }
}

void DataMote::drawInside()
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
    // ===no-one there===
    myAlpha = ofLerp(START_ALPHA, STOP_ALPHA, _radius/NODE_MAX_RADIUS);
    ofSetColor(insideColor.r,insideColor.g,insideColor.b, myAlpha);
    ofFill();
    ofCircle(getX(),getY(),_radius);
    ofSetColor(insideColor.r,insideColor.g,insideColor.b, STOP_ALPHA);
    ofNoFill();
    ofCircle(getX(),getY(),_radius);
    for (int i = 0; i < childMotes.size(); i++) {
        childMotes[i]->draw(getX(), getY(), _radius-1, childColor);
    }
        if ((fadeCount <= MAX_FADE_COUNT) && (fadeCount > 0)) {
        fadeCount--;
    }
    if (fadeCount > 0) {
        drawOutside(outsideColor.a*fadeCount/MAX_FADE_COUNT);
    }
}

void DataMote::drawOutside()
{
    float f = 2;
    //I am over a user or not if flipped
    addVelocity(ofPoint(ofRandom(-f, f), ofRandom(-f, f), ofRandom(-f, f)));

    drawOutside(outsideColor.a);
}

void DataMote::drawOutside(int _alpha)
{
    ofSetColor(outsideColor.r,outsideColor.g,outsideColor.b,_alpha);

    ofNoFill();		// draw "filled shapes"
    ofPoint pp = getPosition();
    if (pMyFont) pMyFont->drawString(labelString, pp.x,pp.y);
}

void DataMote::setLabel(const unsigned int _label)
{
    if (label != _label) {
        if (_label == 0) {
            fadeCount = MAX_FADE_COUNT+1;
        } else {
            fadeCount = MAX_FADE_COUNT;
        }
    }
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
            // ===someone there===       // adjust movement
//        float dist = getConstraintDelta()/maxDistWidthSquare;
//        if (dist>=0 && dist<0.01) {
//            addVelocity(ofPoint(ofRandom(-f, f), ofRandom(-f, f), ofRandom(-f, f)));
//        } else {
//            setVelocity(ofPoint(ofRandom(-f, f), ofRandom(-f, f), ofRandom(-f, f)));

            addVelocity(ofPoint(0, ofRandom(-f, f), 0));
//        }
        }
    }
}

void DataMote::setLabelString(const std::string& _labelString)
{
    labelString = _labelString;
}

void DataMote::setFont(ofTrueTypeFont* _pMyFont)
{
    pMyFont = _pMyFont;
}


void DataMote::setInsideColor(ofColor _newColor)
{
    insideColor = _newColor;
}
void DataMote::setOutsideColor(ofColor _newColor)
{
    outsideColor = _newColor;
}
void DataMote::setChildColor(ofColor _newColor)
{
    childColor = _newColor;
    childColor.a = childColor.a/2;
}

void DataMote::setFadeDist(int _distance)
{
    maxDistWidthSquare = _distance*_distance;
}


void DataMote::setGlyph(ofImage* _pnewglyph)
{
    pGlyph = _pnewglyph;
    pCurrentImage = pGlyph;
}
void DataMote::setBlankGlyph(ofImage* _pnewglyph)
{
    pBlank = _pnewglyph;
}
