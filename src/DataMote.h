#ifndef DATAMOTE_H
#define DATAMOTE_H

#include "ofxMSAParticle.h"
#include "ofTrueTypeFont.h"

#define MAX_DIST_SQR (50.0*50.0)
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
    void setLabel(const unsigned int _label, const unsigned int _frontUser);
    void setLabelString(const std::string& _labelString);
    void setInsideColor(ofColor _newColor);
    void setOutsideColor(ofColor _newColor);
    void setAlpha(void);
    void setFadeDist(float _distance);
    void setGlyph(ofImage* _pnewglyph);
    void setOutsideGlyph(ofImage* _pnewglyph);
protected:
    unsigned int label;
    unsigned int frontUser;
    ofColor insideColor;
    ofTrueTypeFont* pMyFont;
    std::string labelString;
    float maxDistWidthSquare;
    int myAlpha;
    ofColor outsideColor;
    ofColor outsideTextColor;
    virtual void drawInside();
    virtual void drawOutside();
    virtual void drawOutside(ofColor _newColor, int alpha);
    virtual void drawInsideNoFront();
    unsigned int fadeCount;
private:

    ofImage* pGlyph;
    ofImage* pOutsideGlyph;

};

#endif // DATAMOTE_H
