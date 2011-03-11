#include "testApp.h"

//--------------------------------------------------------------
testApp::testApp()
{

}

testApp::~testApp()
{

}

//--------------------------------------------------------------
void testApp::setup()
{
	ofEnableAlphaBlending();
	ofSetWindowPosition(ofGetScreenWidth() - ofGetWidth() - 20, 20);

	oni.setup();
	
	// players
	for (int i = 0; i < MAX_PLAYERS; i++) players[i].allocate(oni.width, oni.height);		
}

//--------------------------------------------------------------
void testApp::update()
{
	ofSetWindowTitle(ofToString(ofGetFrameRate()));
	oni.update();
}

//--------------------------------------------------------------
void testApp::draw()
{	
	ofBackground(0, 0, 0);
	glPushMatrix();

		glScalef(ofGetWidth() / (float)oni.width, ofGetHeight() / (float)oni.height, 1);
		
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
		ofSetColor(255, 255, 255);
		if (oni.bDrawCam)
			oni.drawCam(0, 0);
		else
			oni.drawDepth(0, 0);
		
		ofSetColor(255, 255, 255, 200);
		if (oni.bDrawPlayers)
			oni.drawPlayers(0, 0);	
	glPopMatrix();

		oni.skeletonTracking();
}



//--------------------------------------------------------------
void testApp::keyPressed  (int key)
{
	if(key == '1') oni.bDrawPlayers = !oni.bDrawPlayers;
	if(key == '2') oni.bDrawCam = !oni.bDrawCam;

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::resized(int w, int h){

}