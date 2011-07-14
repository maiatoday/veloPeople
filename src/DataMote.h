#ifndef DATAMOTE_H
#define DATAMOTE_H

#include "ofxMSAParticle.h"
#include "ofTrueTypeFont.h"

#define MAX_DIST_SQR (400*400)
#define START_ALPHA (80)
#define STOP_ALPHA (255)

#define NODE_MIN_RADIUS			2
#define NODE_MAX_RADIUS			7


class DataMote : public ofxMSAParticle
{
public:
    DataMote();
    DataMote(ofPoint pos, float m = 1.0f, float d = 1.0f);
    DataMote(ofxMSAParticle &p);
    virtual ~DataMote();
    virtual void	update();
    virtual void	draw();
    void setFont(ofTrueTypeFont* _pMyFont);
    void setLabel(const unsigned int _label);
    void setLabelString(const std::string& _labelString);
    void setInsideColor(ofColor _newColor);
    void setOutsideColor(ofColor _newColor);
    void setAlpha(void);
    void setFadeDist(int _distance);
    void setGlyph(ofImage* _pnewglyph);
protected:
private:
    ofTrueTypeFont* pMyFont;
    unsigned int label;
    ofColor insideColor;
    ofColor outsideColor;
    int myAlpha;
    std::string labelString;
    int maxDistWidthSquare;
    ofImage* pGlyph;

};

#endif // DATAMOTE_H
