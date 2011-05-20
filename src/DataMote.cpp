#include "DataMote.h"


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
    maxDistWidthSquare = MAX_DIST_SQR;

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
    label = 0;
    addVelocity(ofPoint(ofRandom(-10, 10), ofRandom(-10, 10), ofRandom(-10, 10)));
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
    if (label == 0) {
        //I am over a user or not if flipped
        addVelocity(ofPoint(ofRandom(-f, f), ofRandom(-f, f), ofRandom(-f, f)));

        ofSetColor(insideColor.r,insideColor.g,insideColor.b,insideColor.a);

        ofNoFill();		// draw "filled shapes"
        ofPoint pp = getPosition();
        if (pMyFont) pMyFont->drawString(labelString, pp.x,pp.y);
//        string vStr = ofToString(getVelocity().x, 1);
//        if (pMyFont) pMyFont->drawString(vStr, pp.x,pp.y);
//
//        ofSetColor(insideColor.r,insideColor.g,insideColor.b,insideColor.a);
//        vStr = ofToString(getVelocity().y, 1);
//        if (pMyFont) pMyFont->drawString(vStr, pp.x+10,pp.y+10);
    } else {
        // I am drifting aimlessly or not if flipped
        float dist = getConstraintDelta()/maxDistWidthSquare;
        if (dist > 0 && dist < 1) {
            myAlpha = ofLerp(START_ALPHA, STOP_ALPHA, dist);
//            printf("a %f", dist);
        } else {
            myAlpha = outsideColor.a;
        }
        ofSetColor(outsideColor.r,outsideColor.g,outsideColor.b, myAlpha);
        ofFill();
        if (dist>=0 && dist<0.01) {
            addVelocity(ofPoint(ofRandom(-f, f), ofRandom(-f, f), ofRandom(-f, f)));
        } else {
            setVelocity(ofPoint(ofRandom(-f, f), ofRandom(-f, f), ofRandom(-f, f)));
        }
        ofCircle(getX(),getY(),_radius);
    }
}

void DataMote::setLabel(const unsigned int _label)
{
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


void DataMote::setInsideColor(ofColor _newColor)
{
    insideColor = _newColor;
}
void DataMote::setOutsideColor(ofColor _newColor)
{
    outsideColor = _newColor;
}

void DataMote::setFadeDist(int _distance)
{
    maxDistWidthSquare = _distance*_distance;
}
