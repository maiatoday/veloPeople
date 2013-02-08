#include "ofMain.h"
#include "testApp.h"
#include "ofAppGlutWindow.h"



//========================================================================
int main( ){

    ofAppGlutWindow window;
    window.setDoubleBuffering(false);
	ofSetupOpenGL(&window, 800, 600, OF_WINDOW);			// <-------- setup the GL context
//	ofSetupOpenGL(&window, 640, 480, OF_WINDOW);			// <-------- setup the GL context
//	ofSetupOpenGL(&window, 1280, 720, OF_WINDOW);			// <-------- setup the GL context
// TODO (maia#3#): Make the render bigger

	ofRunApp( new testApp() );


}
