/*
 *  Model.h
 *  openFrameworks
 *
 *  Created by gameoverx on 1/09/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef __MODEL_H
#define __MODEL_H

#define MAX_VIDEO_CHANNELS 2

#define MODEL			ModelSingleton::Instance()
#define CONFIG			MODEL->config
#define CG				MODEL->cg_XML
#define SETAPPSTATE		MODEL->setAppState
#define GETAPPSTATE		MODEL->getAppState()

#define PLAYERS         MODEL->players
#define EFFECTS         MODEL->effects

#include "ofxXmlSettings.h"
#include "goVideoPlayer.h"
#include "goVideoEffectCL.h"

#include "Singleton.h"
#include "Constants.h"
#include "Logger.h"

class Model
{

public:

    Model()
    {
        LOG("Model instantiated");
    };
    ~Model()
    {
        LOG("Model destroyed");
    };

    ofxXmlSettings				cg_XML;

    configStructure				config;

    void						setAppState(int _state);
    int							getAppState();

    goVideoPlayer               players[MAX_VIDEO_CHANNELS];
    goVideoEffectCL             effects[MAX_VIDEO_CHANNELS];

private:

    int							applicationState;

};

typedef Singleton<Model> ModelSingleton;   // Global declaration

#endif
