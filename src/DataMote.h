#ifndef DATAMOTE_H
#define DATAMOTE_H

#include "ofxMSAParticle.h"
#include "ofTrueTypeFont.h"


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
    void setInsideColor(ofColor _newColor);
    void setOutsideColor(ofColor _newColor);
protected:
private:
    ofTrueTypeFont* pMyFont;
    unsigned int label;
    ofColor insideColor;
    ofColor outsideColor;

};

#endif // DATAMOTE_H
