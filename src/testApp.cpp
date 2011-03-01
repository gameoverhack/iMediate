#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup()
{
    //LOG_OPEN_FILE(ofToDataPath("log.txt", false));
    LOG("Instantiating MVC");
    CONTROLLER->setup();
}

/*--------------------------------------------------------------
void testApp::update(){
}


//--------------------------------------------------------------
void testApp::draw(){
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){

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

*/
//--------------------------------------------------------------
//void testApp::resized(int w, int h){

//}

