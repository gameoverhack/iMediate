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

#include "goParticleMask.h"

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

    bool				showMSG;

    bool				enabled;

    void				enableViewEvents();
    void				disableViewEvents();

    void				update(ofEventArgs &e);
    void				draw(ofEventArgs &e);
    void                drawBlend();

    //void				windowResized(ofEventArgs &e);

    void				keyPressed(ofKeyEventArgs &e);
    void				keyReleased(ofKeyEventArgs &e);
    void				mouseMoved(ofMouseEventArgs &e);
    void				mouseDragged(ofMouseEventArgs &e);
    void				mousePressed(ofMouseEventArgs &e);
    void				mouseReleased(ofMouseEventArgs &e);

    float               w_output_scale[2], h_output_scale[2];
    float               w_preview_scale[2], h_preview_scale[2];
    float               x_output_scale[2], y_output_scale[2];
};

typedef Singleton<View> ViewSingleton;   // Global declaration

#endif
