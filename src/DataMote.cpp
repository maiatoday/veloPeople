#include "DataMote.h"

#define MAX_FADE_COUNT (48)

DataMote::DataMote(): ofxMSAParticle()
{
    init();
}


DataMote::DataMote(ofPoint pos, float m, float d) : ofxMSAParticle(pos, m, d)
{
    init();
}

DataMote::DataMote(ofxMSAParticle &p) : ofxMSAParticle(p)
{
    init();
}

DataMote::~DataMote()
{
    //dtor
}
void DataMote::init()
{
    pMyFont = NULL;
    outsideColor.a = 255;
    outsideColor.r = 130;
    outsideColor.g = 0;
    outsideColor.b = 160;

    insideColor.a = 130;
    insideColor.r = 130;
    insideColor.g = 130;
    insideColor.b = 250;
    myAlpha = 255;
    label = 0;
    maxDistWidthSquare = MAX_DIST_SQR;

    addVelocity(ofPoint(ofRandom(-10, 10), ofRandom(-10, 10), ofRandom(-10, 10)));
}
void	DataMote::update()
{
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

    float f = 2;
    // I am drifting aimlessly or not if flipped
    float dist = getConstraintDelta()/maxDistWidthSquare;
    if (dist > 0 && dist < 1) {
        myAlpha = ofLerp(START_ALPHA, STOP_ALPHA, dist);
//            printf("a %f", dist);
    } else {
        myAlpha = insideColor.a;
    }
    ofSetColor(insideColor.r,insideColor.g,insideColor.b, myAlpha);
    ofFill();
    if (dist>=0 && dist<0.01) {
        addVelocity(ofPoint(ofRandom(-f, f), ofRandom(-f, f), ofRandom(-f, f)));
    } else {
        setVelocity(ofPoint(ofRandom(-f, f), ofRandom(-f, f), ofRandom(-f, f)));
    }
    ofCircle(getX(),getY(),_radius);

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
    label = _label;
}

void DataMote::setLabelString(const std::string& _labelString)
{
    labelString = _labelString;
}

void DataMote::setFont(ofTrueTypeFont* _pMyFont)
{
    pMyFont = _pMyFont;
}


void DataMote::setOutsideColor(ofColor _newColor)
{
    outsideColor = _newColor;
}
void DataMote::setInsideColor(ofColor _newColor)
{
    insideColor = _newColor;
}

void DataMote::setFadeDist(int _distance)
{
    maxDistWidthSquare = _distance*_distance;
}

