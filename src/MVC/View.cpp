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
        showMSG = true;
        //ofHideCursor();
    }
    SELECTIONGRP[0] = SELECTIONGRP[1] = 0;
    FBO_OUTPUT.setup(720, 405);
    FBO_PREVIEW.setup(720, 405);

    return true;
}


//--------------------------------------------------------------
void View::update(ofEventArgs &e)
{
    ofBackground(0, 0, 0);

    OSCMANAGER->update();
    MIDIMANAGER->update();
    GUIMANAGER->update();

    for (int i = 0; i < MAX_VIDEO_CHANNELS; i++)
    {
        EFFECTS[i].update();
        GROUPS[i].update();
    }

    PARTICLES->update();

    // actually do the drawing of effects
    // and layers in the update to an FBO_OUTPUT

    FBO_OUTPUT.begin();

    if (!CHANNELADIRECT && !CHANNELBDIRECT)
    {
        drawBlend();
    }
    else
    {
        glPushMatrix();
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        if (CHANNELADIRECT && !MUTE[0]) EFFECTS[REVERSECHANNELS == true ? 1 : 0].draw();
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        if (CHANNELBDIRECT && !MUTE[1]) EFFECTS[REVERSECHANNELS == true ? 0 : 1].draw();
        glPopMatrix();
    }

    FBO_OUTPUT.end();


    FBO_PREVIEW.begin();
    drawBlend();
    FBO_PREVIEW.end();

}

void View::drawBlend()
{
    float fadeLevel;

    glPushMatrix();

    if (XFADETRUE) fadeLevel = ((XFADE) + 1) / 2;
    else fadeLevel = 1.0f - (XFADE >= 0 ? XFADE : 0);

    glColor4f(fadeLevel, fadeLevel, fadeLevel, fadeLevel);         // cross fade
    if(!MUTE[0]) EFFECTS[REVERSECHANNELS == true ? 1 : 0].draw();

    PARTICLES->draw(0x000000, 0);
    PARTICLES->draw(0xFFFFFF, 1);

    glPopMatrix();

    glEnable(GL_BLEND);

    PARTICLES->draw(0x000000, 2);
    PARTICLES->draw(0xFFFFFF, 3);

    if(!XFUNCMUTE) glBlendEquation(xblendEquations[XFUNCS[0]]);
    if(!XFADEMUTE) glBlendFunc(xblendModes[XMODES[0]], xblendModes[XMODES[1]]);
    if(!XFUNCMUTE) glBlendEquation(xblendEquations[XFUNCS[1]]);

    glPushMatrix();

    PARTICLES->draw(0x000000, 4);
    PARTICLES->draw(0xFFFFFF, 5);

    if (XFADETRUE) fadeLevel = ((-XFADE) + 1) / 2;
    else fadeLevel = 1.0f - (XFADE <= 0 ? -XFADE : 0);

    glColor4f(fadeLevel, fadeLevel, fadeLevel, fadeLevel);         // cross fade
    if(!MUTE[0]) EFFECTS[REVERSECHANNELS == true ? 0 : 1].draw();

    PARTICLES->draw(0x000000, 6);
    PARTICLES->draw(0xFFFFFF, 7);

    glBlendEquation(GL_FUNC_ADD);
    glDisable(GL_BLEND);
    glPopMatrix();
    glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
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

    // draw combo effects FBO_OUTPUT for preview
    glPushMatrix();
    glTranslated(720,0,0);
    glScalef(0.5f, 0.5f, 0.0f);
    FBO_PREVIEW.draw(0,0);
    glPopMatrix();

    // draw combo effects FBO_OUTPUT for preview
    glPushMatrix();
    glTranslated(720*1.5,0,0);
    glScalef(0.5f, 0.5f, 0.0f);
    FBO_OUTPUT.draw(0,0);
    glPopMatrix();

    // draw main out
    glPushMatrix();
    glTranslated(1680,0,0);
    glScalef(1024/720.0f, 768/405.0f, 0.0f);
    FBO_OUTPUT.draw(0,0);
    glPopMatrix();

    // draw controls
    glPushMatrix();
    GUI.draw(10.0f, 405/2.0f + 10.0f);
    glPopMatrix();

    // draw rect around preview icon currently playing
    glPushMatrix();
    glColor4f(1.0f, 0.0f, 0.0f, 0.0f);
    ofNoFill();
    ofRect(GROUPS[0].videoPreviews[GROUPS[0].currentlyPlayingVideo].x,
           GROUPS[0].videoPreviews[GROUPS[0].currentlyPlayingVideo].y-1,
           GROUPS[0].videoPreviews[GROUPS[0].currentlyPlayingVideo].width+1,
           GROUPS[0].videoPreviews[GROUPS[0].currentlyPlayingVideo].height+1);
    glPopMatrix();

    glPushMatrix();
    glColor4f(1.0f, 0.0f, 0.0f, 0.0f);
    ofNoFill();
    ofRect(GROUPS[1].videoPreviews[GROUPS[1].currentlyPlayingVideo].x,
           GROUPS[1].videoPreviews[GROUPS[1].currentlyPlayingVideo].y-1,
           GROUPS[1].videoPreviews[GROUPS[1].currentlyPlayingVideo].width+1,
           GROUPS[1].videoPreviews[GROUPS[1].currentlyPlayingVideo].height+1);
    glPopMatrix();

    glPushMatrix();
    glColor4f(0.0f, 0.0f, 1.0f, 0.0f);
    ofNoFill();
    ofRect(900.0f-1.0f, (405 + 10.0f + 405/10.0f) + (405/10.0f * 3 + 4.0f) * SELECTIONGRP[0], 720/10.0f * 4.0 + 8.0f, 405/10.0f * 3 + 4.0f);
    glPopMatrix();

    glPushMatrix();
    glColor4f(0.0f, 0.0f, 1.0f, 0.0f);
    ofNoFill();
    ofRect(900.0f + 720/10.0f * 4.0 + 10.0f - 1.0f, (405 + 10.0f + 405/10.0f) + (405/10.0f * 3 + 4.0f) * SELECTIONGRP[1], 720/10.0f * 4.0 + 8.0f, 405/10.0f * 3 + 4.0f);
    glPopMatrix();

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

        stateMsg += "midi in status: channel: " + ofToString(LASTMIDIMSG.channel) +
                    ", status: " + ofToString(LASTMIDIMSG.status) +
                    ", byteOne: " + ofToString(LASTMIDIMSG.byteOne) +
                    ", byteTwo: " + ofToString(LASTMIDIMSG.byteTwo) +
                    ", timestamp: " + ofToString(LASTMIDIMSG.timestamp);


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
        CONTROLLER->fullScreen();
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
