#include "DataMote.h"

#define MAX_LIFETIME (600)

#define MAX_FADE_COUNT (48)
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

DataMote::~DataMote() {
    //dtor
    // bleargh vector of pointers so must delete objects
    for (int i = 0; i<childMotes.size(); i++) delete childMotes[i];
    childMotes.clear();
}

void	DataMote::update() {
    float f = 2;
    switch (label) {
        case MODE_PAINT_BOTH:
        case MODE_ME:
        setVelocity(ofPoint(ofRandom(-f, f), ofRandom(-f, f), ofRandom(-f, f)));
        break;
        case MODE_CODE:
        case MODE_PAINT:
        case MODE_PAINT_WARM:
//        setVelocity(ofPoint(ofRandom(-f, f), ofRandom(-f, f), ofRandom(-f, f)));
//        addVelocity(ofPoint(0, ofRandom(-f, f), 0));
        break;
    }
}
void	DataMote::draw() {
    switch (label) {
    case MODE_PAINT_BOTH:
        drawBoth();
        break;
    case MODE_PAINT:
        drawPaint();
        break;
    case MODE_PAINT_WARM:
        drawPaintWarm();
        break;
    case MODE_CODE:
        drawText();
        break;
    case MODE_ME:
        drawBanner();
        break;

    default:
        break;
    }
}

void DataMote::drawBoth() {

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
//    myAlpha = ofLerp(START_ALPHA, STOP_ALPHA, _radius/NODE_MAX_RADIUS);
    myAlpha = ofRandom(START_ALPHA, STOP_ALPHA);
    ofSetColor(insideColor.r,insideColor.g,insideColor.b, myAlpha);
    ofFill();
    ofCircle(getX(),getY(),_radius);
//    ofSetColor(insideColor.r,insideColor.g,insideColor.b, STOP_ALPHA);
//    ofNoFill();
//    ofCircle(getX(),getY(),_radius);
    ofColor lineColor(255,255,255,255);
    for (int i = 0; i < childMotes.size(); i++) {
        childMotes[i]->draw(getX(), getY(), _radius-1, childColor, lineColor);
    }
    if ((fadeCount <= MAX_FADE_COUNT) && (fadeCount > 0)) {
        fadeCount--;
    }
    if (fadeCount > 0) {
        drawText(outsideColor.a*fadeCount/MAX_FADE_COUNT);
    }
}
void DataMote::drawPaint() {

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
//    myAlpha = ofLerp(START_ALPHA, STOP_ALPHA, _radius/NODE_MAX_RADIUS);
    myAlpha = ofRandom(START_ALPHA-10, STOP_ALPHA-10);
    ofSetColor(insideColor.r,insideColor.g,insideColor.b, myAlpha);
    ofFill();
    ofCircle(getX(),getY(),_radius);
//    ofSetColor(insideColor.r,insideColor.g,insideColor.b, STOP_ALPHA);
//    ofNoFill();
//    ofCircle(getX(),getY(),_radius);
//    ofColor lineColor(0,0,0,255);
    for (int i = 0; i < childMotes.size(); i++) {
        childMotes[i]->draw(getX(), getY(), _radius/2, childColor, childColor);
    }
}

void DataMote::drawPaintWarm() {

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
//    myAlpha = ofLerp(START_ALPHA, STOP_ALPHA, _radius/NODE_MAX_RADIUS);
    myAlpha = ofRandom(START_ALPHA-10, STOP_ALPHA-10);
    ofSetColor(outsideColor.r,outsideColor.g,outsideColor.b, myAlpha);
    ofFill();
    ofCircle(getX(),getY(),_radius);
//    ofSetColor(insideColor.r,insideColor.g,insideColor.b, STOP_ALPHA);
//    ofNoFill();
//    ofCircle(getX(),getY(),_radius);
    ofColor lineColor(255,255,255,255);
    for (int i = 0; i < childMotes.size(); i++) {
        childMotes[i]->draw(getX(), getY(), _radius/2, outsideColor, lineColor);
    }
}

void DataMote::drawText() {
    float f = 2;
    //I am over a user or not if flipped
//    addVelocity(ofPoint(ofRandom(-f, f), ofRandom(-f, f), ofRandom(-f, f)));

    drawText(outsideColor.a);
}

void DataMote::drawText(int _alpha) {
    ofSetColor(outsideColor.r,outsideColor.g,outsideColor.b,_alpha);

    ofNoFill();		// draw "filled shapes"
    ofPoint pp = getPosition();
    if (pMyFont) pMyFont->drawString(labelString, pp.x,pp.y);
}

void DataMote::drawBanner() {

    ofSetColor(bannerColor.r,bannerColor.g,bannerColor.b,bannerColor.a);

    ofNoFill();		// draw "filled shapes"
    ofPoint pp = getPosition();
    if (pMyFont) pMyFont->drawString(bannerString, pp.x,pp.y);

}

void DataMote::setLabel(const unsigned int _label) {
    if (label != _label) {
        if (_label != MODE_PAINT_BOTH) {
            fadeCount = MAX_FADE_COUNT+1;
            int i = ofRandom(0,3);
            pMyFont = pFontSizes[i];
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
            addVelocity(ofPoint(0, ofRandom(-f, f), 0));
    }
}

void DataMote::setLabelString(const std::string& _labelString) {
    labelString = _labelString;
}

void DataMote::setBannerString(const std::string& _labelString) {
    bannerString = _labelString;
}

void DataMote::setFont(ofTrueTypeFont* _pMyFont, int i) {
    pMyFont = _pMyFont;
    pFontSizes[i] = _pMyFont;
}


void DataMote::setInsideColor(ofColor _newColor) {
    insideColor = _newColor;
}
void DataMote::setOutsideColor(ofColor _newColor) {
    outsideColor = _newColor;
}
void DataMote::setChildColor(ofColor _newColor) {
    childColor = _newColor;
    childColor.a = childColor.a/2;
}
void DataMote::setBannerColor(ofColor _newColor) {
    bannerColor = _newColor;
}

void DataMote::setFadeDist(int _distance) {
    maxDistWidthSquare = _distance*_distance;
}


void DataMote::setGlyph(ofImage* _pnewglyph) {
    pGlyph = _pnewglyph;
    pCurrentImage = pGlyph;
}
void DataMote::setBlankGlyph(ofImage* _pnewglyph) {
    pBlank = _pnewglyph;
}
