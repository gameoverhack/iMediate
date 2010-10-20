/*
 *  View.cpp
 *  openFrameworks
 *
 *  Created by gameoverx on 1/09/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

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
        bCustomFullscreen = false;
    }

    FBO.setup(1024, 768);

    return true;
}


//--------------------------------------------------------------
void View::update(ofEventArgs &e)
{
    ofBackground(0, 0, 0);

    CONTROLLER->checkFolders();

    for (int i = 0; i < MAX_VIDEO_CHANNELS; i++)
    {
        EFFECTS[i].update();
        GROUPS[i].update();
    }


    // actually do the drawing of effects
    // and layers in the update to an FBO

    FBO.begin();

    EFFECTS[0].draw();
    glEnable(GL_BLEND);
    glBlendFunc(GL_ZERO, GL_SRC_COLOR);
    EFFECTS[1].draw();
    glDisable(GL_BLEND);

    FBO.end();

}


//--------------------------------------------------------------
void View::draw(ofEventArgs &e)
{

    // draw video layer 1 & 2 separately
    glPushMatrix();
    glTranslated(0,0,0);
    glScalef(0.5f, 0.5f, 0.0f);
    EFFECTS[0].draw();
    glPopMatrix();

    glPushMatrix();
    glTranslated(720/2.0f,0,0);
    glScalef(0.5f, 0.5f, 0.0f);
    EFFECTS[1].draw();
    glPopMatrix();

    // draw combo effects FBO for preview
    glPushMatrix();
    glTranslated(720,0,0);
    glScalef(0.5f, 0.5f, 0.0f);
    FBO.draw(0,0);
    glPopMatrix();

    // draw main out
    glPushMatrix();
    glTranslated(1680,0,0);
    glScalef(1024/720.0f, 768/405.0f, 0.0f);
    FBO.draw(0,0);
    glPopMatrix();

    // draw controls
    glPushMatrix();
    GUI.draw(10.0f, 405/2.0f + 10.0f);
    glPopMatrix();

    GROUPS[0].drawPreviews(720/2.0f * 3.0f, 0);
    GROUPS[1].drawPreviews(720/2.0f * 3.0f + 720/10.0f + 10.0f, 0);

    if(showMSG)
    {

        ofSetColor(0, 255, 0);

        string fpsStr = "appFPS: "+ofToString(ofGetFrameRate(), 2) + "\n";

        fpsStr += "effect_01_FPS: " + ofToString(EFFECTS[0].currentFPS) + "\n";
        fpsStr += "effect_02_FPS: " + ofToString(EFFECTS[1].currentFPS) + "\n";

        string stateMsg = "appstate: " + states[GETAPPSTATE] + "\n\n";

        for (int i = 0; i < MAX_VIDEO_CHANNELS; i++)
        {
            stateMsg += "Group " + ofToString(i) + " loading: " + GROUPS[i].currentlyLoadingVideo + "\n";
        }


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
    case OF_KEY_F2:
        showMSG = !showMSG;
        break;
    case OF_KEY_F12:
        exit(0);
        break;
    case OF_KEY_F1:
#ifdef TARGET_WIN32
        if (!bCustomFullscreen)
        {
            LOG("Trying to force fullscreen on Windows 7" + ofToString(ofGetWidth()));
            windowTitle = "imMediate";
            ofSetWindowTitle(windowTitle);
            int x = 0;
            int y = 0;
            int width = 1680 + 1024;
            int height = 1050;
            int storedWindowX, storedWindowY, storedWindowH, storedWindowW;
            HWND vWnd  = FindWindow(NULL,  "imMediate");
            long windowStyle = GetWindowLong(vWnd, GWL_STYLE);
            windowStyle &= ~WS_OVERLAPPEDWINDOW;
            windowStyle |= WS_POPUP;
            SetWindowLong(vWnd, GWL_STYLE, windowStyle);
            SetWindowPos(vWnd, HWND_TOP, x, y, width, height, SWP_FRAMECHANGED);
            bCustomFullscreen = true;
        }
        else
        {
            int x = 0;
            int y = 0;
            int width = 1680;
            int height = 1050;
            HWND vWnd  = FindWindow(NULL,  "imMediate");
            long windowStyle = GetWindowLong(vWnd, GWL_STYLE);
            windowStyle |= WS_TILEDWINDOW;
            SetWindowLong(vWnd, GWL_STYLE, windowStyle);
            SetWindowPos(vWnd, HWND_TOP, x, y, width, height, SWP_FRAMECHANGED);
            bCustomFullscreen = false;
        }

#else
        ofToggleFullscreen();
#endif
        break;
    }

    if (e.key - 'a' >= 0 && e.key - 'a' < 27) GROUPS[0].playVideoInGroup(e.key - 'a');
    if (e.key - 'A' >= 0 && e.key - 'A' < 27) GROUPS[1].playVideoInGroup(e.key - 'A');

}

//--------------------------------------------------------------
void View::keyReleased(ofKeyEventArgs &e)
{

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
