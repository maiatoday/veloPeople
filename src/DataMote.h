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
protected:
private:
    ofTrueTypeFont* pMyFont;
    unsigned int label;
};

#endif // DATAMOTE_H
