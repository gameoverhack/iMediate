/*
 *  View.h
 *  openFrameworks
 *
 *  Created by gameoverx on 1/09/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __VIEW_H
#define __VIEW_H

#define VIEW ViewSingleton::Instance()

#include "Singleton.h"
#include "Controller.h"
#include "Logger.h"

#include "ofEvents.h"

#define VIEW ViewSingleton::Instance()

class View
{

public:

    View()
    {
        LOG("View instantiated");
        enabled = false;
    };
    ~View()
    {
        LOG("View destroyed");
    };

    bool				setup();

private:

    string              windowTitle;
    bool                bCustomFullscreen;

    bool				showMSG;

    bool				enabled;

    void				enableViewEvents();
    void				disableViewEvents();

    void				update(ofEventArgs &e);
    void				draw(ofEventArgs &e);
    //void				windowResized(ofEventArgs &e);

    void				keyPressed(ofKeyEventArgs &e);
    void				keyReleased(ofKeyEventArgs &e);
    void				mouseMoved(ofMouseEventArgs &e);
    void				mouseDragged(ofMouseEventArgs &e);
    void				mousePressed(ofMouseEventArgs &e);
    void				mouseReleased(ofMouseEventArgs &e);

};

typedef Singleton<View> ViewSingleton;   // Global declaration

#endif
