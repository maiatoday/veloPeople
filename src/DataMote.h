#ifndef DATAMOTE_H
#define DATAMOTE_H

#include "ofxMSAParticle.h"
#include "ofTrueTypeFont.h"

#define MAX_DIST_SQR (400*400)
#define START_ALPHA (80)
#define STOP_ALPHA (255)

class DataMote : public ofxMSAParticle
{
public:
    DataMote();
    DataMote(ofPoint pos, float m = 1.0f, float d = 1.0f);
    DataMote(ofxMSAParticle &p);
    virtual ~DataMote();
    virtual void	update();
    virtual void	draw();
    virtual void init();
    void setFont(ofTrueTypeFont* _pMyFont);
    void setLabel(const unsigned int _label);
    void setLabelString(const std::string& _labelString);
    void setInsideColor(ofColor _newColor);
    void setOutsideColor(ofColor _newColor);
    void setAlpha(void);
    void setFadeDist(int _distance);
protected:
    unsigned int label;
    ofColor insideColor;
    ofTrueTypeFont* pMyFont;
    std::string labelString;
    int maxDistWidthSquare;
    int myAlpha;
    ofColor outsideColor;
    virtual void drawInside();
    virtual void drawOutside();
    virtual void drawOutside(int alpha);
    unsigned int fadeCount;
private:

};

#endif // DATAMOTE_H
