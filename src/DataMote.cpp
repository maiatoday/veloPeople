#include "DataMote.h"


DataMote::DataMote(): ofxMSAParticle() {

}


DataMote::DataMote(ofPoint pos, float m, float d) : ofxMSAParticle(pos, m, d){

}

DataMote::DataMote(ofxMSAParticle &p) : ofxMSAParticle(p){
//	init(p.getPosition(), p.getMass, p.getDrag());
//	_isFixed = p.isFixed();
//	setBounce(p.getBounce());
//	setRadius(p.getRadius());
}

DataMote::~DataMote()
{
    //dtor
}

void	DataMote::update()
{
}
void	DataMote::draw() {
	//let's draw a circle:
	ofSetColor(255,130,0);
	ofFill();		// draw "filled shapes"
	ofCircle(getX(),getY(),_radius);}
