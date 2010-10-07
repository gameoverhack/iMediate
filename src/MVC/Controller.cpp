/*
 *  Controller.cpp
 *  openFrameworks
 *
 *  Created by gameoverx on 1/09/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "Controller.h"

void Controller::setup()
{

    SETAPPSTATE(APP_INIT);

    LOG("Application Initializing");

    // Load in the config XML and setup the Model
    bool abort = true;

    if (APPLOADER->setup("config.xml"))
    {
        abort = false;
    }
    else
    {
        LOG("...config error. Exiting!");
        abort = true;
    }

    if(VIEW->setup())
    {
        abort = false;
    }
    else
    {
        LOG("...view failed to setup. Exiting!");
        abort = true;
    }

    if(abort)
    {
        // exit!
        exit(0);
    }
    else
    {

        ofSetLogLevel(OF_LOG_VERBOSE);
		
        PLAYERS[0].loadMovie("/Volumes/GhostDriverX/Users/gameoverx/Desktop/vjMedia/trainStation/other/slow_legs-JPEG720-JPEG540.mov");
        PLAYERS[0].play();

        PLAYERS[1].loadMovie("/Volumes/GhostDriverX/Users/gameoverx/Desktop/vjMedia/trains02/train2/train17-JPEG720-JPEG540.mov");
        PLAYERS[1].play();

        EFFECTS[0].allocate(&PLAYERS[0]);

        EFFECTS[1].allocate(&PLAYERS[1]);

        EFFECTS[1].doBlur = true;
        EFFECTS[1].blurAmount = 5;

        EFFECTS[0].doInvert = true;

        // start application
        SETAPPSTATE(APP_READY);
    }


}

