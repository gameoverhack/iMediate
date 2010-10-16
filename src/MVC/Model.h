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

#define GROUPS          MODEL->groups
#define PLAYERS         MODEL->players
#define EFFECTS         MODEL->effects


#define GUI             gui

#define FILES           MODEL->files

#define FOLDERS         MODEL->folderBox
#define LASTFOLDERS     MODEL->lastFolderBox

#include "ofxXmlSettings.h"
#include "goVideoGroup.h"
#include "goThreadedVideo.h"
#include "goVideoEffectCL.h"
#include "goDirList.h"
#include "ofxSimpleGuiToo.h"

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

    goVideoGroup                groups[MAX_VIDEO_CHANNELS];
    goThreadedVideo            players[MAX_VIDEO_CHANNELS];
    goVideoEffectCL             effects[MAX_VIDEO_CHANNELS];

    goDirList                   files;

    // gui elements
    int                         folderBox[MAX_VIDEO_CHANNELS];
    int                         lastFolderBox[MAX_VIDEO_CHANNELS];

private:

    int							applicationState;

};

typedef Singleton<Model> ModelSingleton;   // Global declaration

#endif
