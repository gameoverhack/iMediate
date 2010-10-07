/*
 *  View.cpp
 *  openFrameworks
 *
 *  Created by gameoverx on 1/09/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */
static bool doEffect = false;
#include "View.h"
//--------------------------------------------------------------
bool View::setup()
{

    if(!enabled)
    {
        ofSetVerticalSync(true);
        enableViewEvents();
        showMSG = false;
        //ofHideCursor();
    }

    return true;
}


//--------------------------------------------------------------
void View::update(ofEventArgs &e)
{
    ofBackground(0, 0, 0);

    EFFECTS[0].update();
    EFFECTS[1].update();

    /*if(doEffect) {
		EFFECTS[1].update();
	} else PLAYERS[1].update();*/

}


//--------------------------------------------------------------
void View::draw(ofEventArgs &e)
{


	//glEnable(GL_BLEND);
	//	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
        EFFECTS[0].draw();
        glPushMatrix();
        glTranslatef(20,20,0);

        EFFECTS[1].draw();
        //if(doEffect) {
		//	EFFECTS[1].draw();
		//} else PLAYERS[1].draw(0,0);
        glPopMatrix();
    //glDisable(GL_BLEND);

    if(showMSG)
    {

        ofSetColor(0, 255, 0);

        string fpsStr = "appFPS: "+ofToString(ofGetFrameRate(), 2) + "\n";

        fpsStr += "effect_01_FPS: " + ofToString(EFFECTS[0].currentFPS) + "\n";
        fpsStr += "effect_02_FPS: " + ofToString(EFFECTS[1].currentFPS) + "\n";

        string stateMsg = "appstate: " + states[GETAPPSTATE] + "\n\n";

        string msg = fpsStr + " " + stateMsg;
        ofDrawBitmapString(msg, 20, ofGetHeight()-180);
        ofSetColor(255, 255, 255);

    }

}

//--------------------------------------------------------------
void View::keyPressed(ofKeyEventArgs &e)
{


    switch (e.key)
    {
		case 'f':
			showMSG = !showMSG;
			break;
		case 'l':
#ifdef TARGET_OSX
			//PLAYERS[0].loadMovie("/Volumes/GhostDriverX/Users/gameoverx/Desktop/vjMedia/trainStation/other/slow_legs-JPEG720-JPEG540.mov");
			PLAYERS[1].loadMovie("/Volumes/GhostDriverX/Users/gameoverx/Desktop/vjMedia/trains02/train2/train17-JPEG720-JPEG540.mov");
#else
			//PLAYERS[0].loadMovie("C:/Users/gameoverwell/Desktop/slow_legs-JPEG720-JPEG540.mov");
			PLAYERS[1].loadMovie("C:/Users/gameoverwell/Desktop/train17-JPEG720-JPEG540.mov");
#endif
			
			PLAYERS[0].setLoopState(OF_LOOP_NORMAL);
			PLAYERS[1].setLoopState(OF_LOOP_NORMAL);
			break;
    }


}

//--------------------------------------------------------------
void View::keyReleased(ofKeyEventArgs &e)
{
	doEffect ^= true;
}

//--------------------------------------------------------------
void View::mouseMoved(ofMouseEventArgs &e)
{

}

//--------------------------------------------------------------
void View::mouseDragged(ofMouseEventArgs &e)
{

}

//--------------------------------------------------------------
void View::mousePressed(ofMouseEventArgs &e)
{

}

//--------------------------------------------------------------
void View::mouseReleased(ofMouseEventArgs &e)
{

}

//--------------------------------------------------------------
//void View::windowResized(ofEventArgs &e)
//{

//}


//--------------------------------------------------------------
void View::enableViewEvents()
{

    LOG("Enabling keyboard, mouse, draw, update events for View");

    enabled = true;

    // turn on app events for this class
    ofAddListener(ofEvents.mousePressed, this, &View::mousePressed);
    ofAddListener(ofEvents.mouseMoved, this, &View::mouseMoved);
    ofAddListener(ofEvents.mouseDragged, this, &View::mouseDragged);
    ofAddListener(ofEvents.mouseReleased, this, &View::mouseReleased);
    ofAddListener(ofEvents.keyPressed, this, &View::keyPressed);
    ofAddListener(ofEvents.keyReleased, this, &View::keyReleased);
    ofAddListener(ofEvents.draw, this, &View::draw);
    ofAddListener(ofEvents.update, this, &View::update);
    //ofAddListener(ofEvents.windowResized, this, &View::windowResized);
}
//--------------------------------------------------------------
void View::disableViewEvents()
{

    LOG("Disabling keyboard, mouse, draw, update events for View");

    enabled = false;

    // turn off app events for this class
    ofRemoveListener(ofEvents.mousePressed, this, &View::mousePressed);
    ofRemoveListener(ofEvents.mouseMoved, this, &View::mouseMoved);
    ofRemoveListener(ofEvents.mouseDragged, this, &View::mouseDragged);
    ofRemoveListener(ofEvents.mouseReleased, this, &View::mouseReleased);
    ofRemoveListener(ofEvents.keyPressed, this, &View::keyPressed);
    ofRemoveListener(ofEvents.keyReleased, this, &View::keyReleased);
    ofRemoveListener(ofEvents.draw, this, &View::draw);
    ofRemoveListener(ofEvents.update, this, &View::update);
    //ofRemoveListener(ofEvents.windowResized, this, &View::windowResized);
}
