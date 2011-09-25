#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"



//========================================================================
int main( ){

    ofAppGlutWindow window;
//	ofSetupOpenGL(&window, 640, 480, OF_WINDOW);			// <-------- setup the GL context
//	ofSetupOpenGL(&window, 800, 600, OF_WINDOW);			// <-------- setup the GL context
	ofSetupOpenGL(&window, 1280, 720, OF_WINDOW);			// <-------- setup the GL context

	ofRunApp( new testApp() );


}
