/*
 *  Controller.h
 *  openFrameworks
 *
 *  Created by gameoverx on 1/09/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __CONTROLLER_H
#define __CONTROLLER_H

#define CONTROLLER		ControllerSingleton::Instance()

#include "Singleton.h"
#include "Model.h"
#include "View.h"
#include "Logger.h"
#include "ApplicationLoader.h"

#include "goVideoGroup.h"
#include "goGuiManager.h"
#include "goMidiManager.h"
#include "goOSCManager.h"

#define GROUPS          CONTROLLER->groups

class Controller
{

public:

    Controller()
    {
        LOG("Controller instantiated");
    };
    ~Controller()
    {
        LOG("Controller destroyed");
    };

    goVideoGroup                groups[MAX_VIDEO_CHANNELS];

    void						setup();
    void                        fullScreen();

    string                      folderArray[1000]; // silly big number for max number of directories

private:
    string                      windowTitle;
    bool                        bCustomFullscreen;
};

typedef Singleton<Controller> ControllerSingleton;   // Global declaration

#endif
