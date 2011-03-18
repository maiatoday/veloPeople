#ifndef DATAMOTE_H
#define DATAMOTE_H

#include "ofxMSAParticle.h"


class DataMote : public ofxMSAParticle
{
public:
    DataMote();
	DataMote(ofPoint pos, float m = 1.0f, float d = 1.0f);
	DataMote(ofxMSAParticle &p);
    virtual ~DataMote();
    virtual void	update();
    virtual void	draw();

protected:
private:
};

#endif // DATAMOTE_H
