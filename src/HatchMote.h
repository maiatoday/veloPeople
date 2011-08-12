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

class HatchMote : public ofxMSAParticle
{
public:
    HatchMote();
    HatchMote(ofPoint pos, float m = 1.0f, float d = 1.0f);
    HatchMote(ofxMSAParticle &p);
    virtual ~HatchMote();
    virtual void	update();
    virtual void	draw();
    void init();
    void setFont(ofTrueTypeFont* _pMyFont);
    void setLabel(const unsigned int _label);
    void setLabelString(const std::string& _labelString);
    std::string getLabelBuildString(){return labelBuildString;};
    void setInsideColor(ofColor _newColor);
    void setOutsideColor(ofColor _newColor);
    void setChildColor(ofColor _newColor);
    void setAlpha(void);
    void setFadeDist(int _distance);
    void setGlyph(ofImage* _pnewglyph);
    void setBlankGlyph(ofImage* _pnewglyph);
    void setMainStream(bool _isMain) {mainStream = _isMain;};
protected:
private:
    ofTrueTypeFont* pMyFont;
    unsigned int label;
    ofColor insideColor;
    ofColor outsideColor;
    ofColor childColor;
    int myAlpha;
    std::string labelString;
    unsigned char buildNumber;
    std::string labelBuildString;
    unsigned int frameStep;
    int maxDistWidthSquare;
    ofImage* pGlyph;
    ofImage* pBlank;
    ofImage* pCurrentImage;
//    std::vector<MoteHistory*> childMotes;
    int timeToBlank;
    bool mainStream;
    bool doChange;
    int dx;
    int dy;

};

#endif // DATAMOTE_H
