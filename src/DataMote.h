#ifndef DATAMOTE_H
#define DATAMOTE_H

#include "ofxMSAParticle.h"


class DataMote : public ofxMSAParticle
{
public:
    DataMote();
    virtual ~DataMote();
    virtual void	update();
    virtual void	draw();

protected:
private:
};

#endif // DATAMOTE_H
