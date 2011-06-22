#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	context.setupUsingXMLFile(ofToDataPath("openni/config/ofxopenni_config.xml", true));
	depth.setup(&context);
	user.setup(&context, &depth);
}

//--------------------------------------------------------------
void testApp::update(){
	context.update();
	user.update();

}

//--------------------------------------------------------------
void testApp::draw(){
	depth.draw(0,0,640,480);
	user.draw();
	ofxTrackedUser* tracked = user.getTrackedUser(0);
	if(tracked != NULL) {
		tracked->debugDraw();
	}
    
    
    //draw 3D coords on top
    cursorWorld = depth.getWorldXYZ(cursorScreen);
    //
    stringstream strCoords;
    strCoords << ofToString(cursorWorld);
    //
    ofDrawBitmapString(strCoords.str(), cursorScreen.x, cursorScreen.y);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){

}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){
    
    cursorScreen.x = x;
    cursorScreen.y = y;
    
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

    cursorScreen.x = x;
    cursorScreen.y = y;
    
}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}