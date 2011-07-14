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
    label = 0;
    maxDistWidthSquare = MAX_DIST_SQR;

    pGlyph = NULL;

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

    pGlyph = NULL;
    label = 0;
    addVelocity(ofPoint(ofRandom(-10, 10), ofRandom(-10, 10), ofRandom(-10, 10)));
}

DataMote::~DataMote()
{
    //dtor
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

    float f = 2;
    if (label == 0) {
        // ===no-one there===
//        float dist = getConstraintDelta()/maxDistWidthSquare;
        // draw mote
//        if (dist > 0 && dist < 1) {
//            myAlpha = ofLerp(START_ALPHA, STOP_ALPHA, dist);
//        } else {
//            myAlpha = outsideColor.a;
//        }
        myAlpha = ofLerp(START_ALPHA, STOP_ALPHA, _radius/NODE_MAX_RADIUS);
        ofSetColor(outsideColor.r,outsideColor.g,outsideColor.b, myAlpha);
        ofFill();
        ofCircle(getX(),getY(),_radius);
        ofNoFill();
        ofCircle(getX(),getY(),_radius);

    } else {
        // ===someone there===
        //draw mote
        ofPoint pp = getPosition();
        myAlpha = 255;
        if (pGlyph) pGlyph->draw(pp.x, pp.y, _radius*3, _radius*3);
//        if (pGlyph) pGlyph->draw(pp.x, pp.y);

    }
}

void DataMote::setLabel(const unsigned int _label)
{
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
            setVelocity(ofPoint(ofRandom(-f, f), ofRandom(-f, f), ofRandom(-f, f)));
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

void DataMote::setFadeDist(int _distance)
{
    maxDistWidthSquare = _distance*_distance;
}


void DataMote::setGlyph(ofImage* _pnewglyph)
{
    pGlyph = _pnewglyph;
}
