#include "DataMote.h"

DataMote::DataMote()
{
    //ctor
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
