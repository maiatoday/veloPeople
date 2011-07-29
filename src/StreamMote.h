#ifndef DATAMOTE_H
#define DATAMOTE_H

#include "ofxMSAParticle.h"
#include "ofTrueTypeFont.h"

#define MAX_DIST_SQR (400*400)
#define START_ALPHA (80)
#define STOP_ALPHA (200)
#define CHILD_ALPHA (200)

#define NODE_MIN_RADIUS			2
#define NODE_MAX_RADIUS			7

#include "MoteHistory.h"

#include <vector>

class StreamMote : public ofxMSAParticle
{
public:
    StreamMote();
    StreamMote(ofPoint pos, float m = 1.0f, float d = 1.0f);
    StreamMote(ofxMSAParticle &p);
    virtual ~StreamMote();
    virtual void	update();
    virtual void	draw();
    void setFont(ofTrueTypeFont* _pMyFont);
    void setLabel(const unsigned int _label);
    void setLabelString(const std::string& _labelString);
    void setInsideColor(ofColor _newColor);
    void setOutsideColor(ofColor _newColor);
    void setChildColor(ofColor _newColor);
    void setAlpha(void);
    void setFadeDist(int _distance);
    void setGlyph(ofImage* _pnewglyph);
    void setBlankGlyph(ofImage* _pnewglyph);
protected:
private:
    ofTrueTypeFont* pMyFont;
    unsigned int label;
    ofColor insideColor;
    ofColor outsideColor;
    ofColor childColor;
    int myAlpha;
    std::string labelString;
    int maxDistWidthSquare;
    ofImage* pGlyph;
    ofImage* pBlank;
    ofImage* pCurrentImage;
    std::vector<MoteHistory*> childMotes;
    int timeToBlank;


};

#endif // DATAMOTE_H
