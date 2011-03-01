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
#define NUM_SOLENOIDS 8

#include "ofxXmlSettings.h"
#include "goThreadedVideo.h"
#include "goVideoEffectCL.h"

#include "goDirList.h"
#include "goTextLoader.h"
#include "ofxSimpleGuiToo.h"
#include "ofxFbo.h"

#include "Singleton.h"
#include "Constants.h"
#include "Logger.h"

#define MODEL			ModelSingleton::Instance()
#define CONFIG			MODEL->config
#define CG				MODEL->cg_XML
#define SETAPPSTATE		MODEL->setAppState
#define GETAPPSTATE		MODEL->getAppState()

#define PLAYERS         MODEL->players
#define EFFECTS         MODEL->effects

#define FBO_OUTPUT      MODEL->outputFbo
#define FBO_PREVIEW     MODEL->previewFbo

#define DICTIONARY      MODEL->text

#define GUI             gui

#define FILES           MODEL->files

#define FOLDERARRAY     MODEL->folderArray


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

    goThreadedVideo          *  players[MAX_VIDEO_CHANNELS];
    goVideoEffectCL             effects[MAX_VIDEO_CHANNELS];

    goDirList                   files;
    string                      folderArray[1000]; // silly big number for max number of directories

    goTextLoader                text;

    ofxFbo                      outputFbo;
    ofxFbo                      previewFbo;

private:

    int							applicationState;

};

typedef Singleton<Model> ModelSingleton;   // Global declaration

#endif
