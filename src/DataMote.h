#ifndef DATAMOTE_H
#define DATAMOTE_H

#include "ofxMSAParticle.h"
#include "ofTrueTypeFont.h"

#define MAX_DIST_SQR (400*400)
#define START_ALPHA (80)
#define STOP_ALPHA (150)
#define CHILD_ALPHA (200)

#define NODE_MIN_RADIUS			2
#define NODE_MAX_RADIUS			7

#include "MoteSatellite.h"

#include <vector>

enum drawMode {
    MODE_PAINT_BOTH = 0,
    MODE_PAINT,
    MODE_PAINT_WARM,
    MODE_ME,
    MODE_CODE
};

class DataMote : public ofxMSAParticle
{
public:
    DataMote();
    DataMote(ofPoint pos, float m = 1.0f, float d = 1.0f);
    DataMote(ofxMSAParticle &p);
    virtual ~DataMote();
    virtual void	update();
    virtual void	draw();
    void setFont(ofTrueTypeFont* _pMyFont, int i);
    void setLabel(const unsigned int _label);
    void setLabelString(const std::string& _labelString);
    void setBannerString(const std::string& _labelString);
    void setInsideColor(ofColor _newColor);
    void setOutsideColor(ofColor _newColor);
    void setChildColor(ofColor _newColor);
    void setBannerColor(ofColor _newColor);
    void setAlpha(void);
    void setFadeDist(int _distance);
    void setGlyph(ofImage* _pnewglyph);
    void setBlankGlyph(ofImage* _pnewglyph);
protected:
private:
    ofTrueTypeFont* pMyFont;
    ofTrueTypeFont* pFontSizes[4];
    unsigned int label;
    ofColor insideColor;
    ofColor outsideColor;
    ofColor childColor;
    ofColor bannerColor;
    int myAlpha;
    std::string labelString;
    std::string bannerString;
    int maxDistWidthSquare;
    ofImage* pGlyph;
    ofImage* pBlank;
    ofImage* pCurrentImage;
    std::vector<MoteSatellite*> childMotes;
    int timeToBlank;

    virtual void drawBoth();
    virtual void drawPaint();
    virtual void drawPaintWarm();
    virtual void drawText();
    virtual void drawBanner();
    virtual void drawText(int alpha);
    unsigned int fadeCount;


};

#endif // DATAMOTE_H
