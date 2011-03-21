#include "DataMote.h"


DataMote::DataMote(): ofxMSAParticle() {

}


DataMote::DataMote(ofPoint pos, float m, float d) : ofxMSAParticle(pos, m, d) {
    label = 0;

}

DataMote::DataMote(ofxMSAParticle &p) : ofxMSAParticle(p) {
//	init(p.getPosition(), p.getMass, p.getDrag());
//	_isFixed = p.isFixed();
//	setBounce(p.getBounce());
//	setRadius(p.getRadius());
}

DataMote::~DataMote() {
    //dtor
}

void	DataMote::update() {
}

void	DataMote::draw() {
    //let's draw a circle:
    if (label > 0) {
// TODO (maia#1#): draw differently if over user        //I am over a user
        ofSetColor(255,130,0);

    } else {
        // I am drifting aimlessly
        ofSetColor(130,130, 130);
    }
    ofFill();		// draw "filled shapes"
    ofCircle(getX(),getY(),_radius);
}

void DataMote::setLabel(const unsigned int _label) {
    label = _label;
}
