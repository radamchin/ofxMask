#include "ofMain.h"
#include "ofApp.h"
#include "ofAppGLFWWindow.h"

//========================================================================
int main( ){
    
    ofGLFWWindowSettings settings;
    settings.setGLVersion(3, 2); // Use programmable renderer
    settings.setSize(1024,768);
    
    ofCreateWindow(settings);
	// ofSetupOpenGL(1024,1024,OF_WINDOW);			// <-------- setup the GL context

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
