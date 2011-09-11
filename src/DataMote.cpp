#include "DataMote.h"

#define MAX_LIFETIME (600)

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
    childMotes.clear();
}


void DataMote::init()
{
    float coinFlip = ofRandom(0,8);
    if (coinFlip > 1) {
        iAmAnExitCode = false;
    } else {
        iAmAnExitCode = true;
    }


    pMyFont = NULL;
    insideColor.a = 255;
    insideColor.r = 130;
    insideColor.g = 0;
    insideColor.b = 160;

    outsideColor.a = 130;
    outsideColor.r = 130;
    outsideColor.g = 130;
    outsideColor.b = 250;

    bsodColor.r = 0;
    bsodColor.g = 10;
    bsodColor.b = 145;
    bsodColor.a = 255;

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
    if (label == 0) {
        // ===no-one there===
        if (iAmAnExitCode) {
            drawText();
        } else {
            for (int i = 0; i < childMotes.size(); i++) {
                childMotes[i]->draw(getX(), getY(), _radius-1, childColor);
            }
            int rectWidth = _radius*3;
            int rectHeight = _radius*3;
            ofNoFill();
            ofSetColor(255,255, 255, 255);
            ofRect(getX(), getY(), rectWidth+2, rectHeight+2);
            ofFill();
            ofSetColor(0,0, 0, 255);
            ofRect(getX()+1, getY()+1, rectWidth, rectHeight*2);
            ofNoFill();
            ofSetColor(255,255, 255, 255);
            ofRect(getX(), getY()+5, rectWidth+2, rectHeight-3);
        }

    } else {
        // ===someone there===
        //draw mote
        ofPoint pp = getPosition();
        myAlpha = 255;
//        if (pGlyph) pCurrentImage->draw(pp.x, pp.y, _radius*3, _radius*3);
//        if (pCurrentImage) pCurrentImage->draw(pp.x, pp.y, pCurrentImage->getWidth()/2, pCurrentImage->getHeight()/2);
        if (pCurrentImage) pCurrentImage->draw(pp.x, pp.y);
//        if (pGlyph) pGlyph->draw(pp.x, pp.y);
//        if (timeToBlank == MAX_LIFETIME)        drawText();

    }
}
void DataMote::drawText()
{
    ofPoint pp = getPosition();
    int newX = pp.x+ofRandom(-2,2);
    int newY = pp.y+ofRandom(-2,2);
    ofSetColor(bsodColor.r,bsodColor.g,bsodColor.b, bsodColor.a);
    ofFill();
    ofRect(newX-2, newY-10, labelString.length()*8, 20);
//    if (pMyFont) pMyFont->drawString(labelString, newX-2, newY-1);
//    if (pMyFont) pMyFont->drawString(labelString, newX+2, newY+2);
    ofSetColor(255, 255, 255, 255);
    if (pMyFont) pMyFont->drawString(labelString, newX, newY);
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
            setVelocity(ofPoint(0, ofRandom(-f, f), 0));
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
    if (iAmAnExitCode) {
        labelString = _labelString;
    }
}

void DataMote::setFont(ofTrueTypeFont* _pMyFont)
{
    pMyFont = _pMyFont;
}


void DataMote::setInsideColor(ofColor _newColor)
{
    insideColor = _newColor;
    setChildColor(insideColor);
}
void DataMote::setOutsideColor(ofColor _newColor)
{
    outsideColor = _newColor;
}
void DataMote::setChildColor(ofColor _newColor)
{
    childColor = _newColor;
    childColor.a = childColor.a/2;
//    for (int i = 0; i < childMotes.size(); i++) {
//        childMotes[i]->setColor(childColor);
//    }
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
