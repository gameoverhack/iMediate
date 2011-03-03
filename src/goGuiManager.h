#ifndef __GOGUIMANAGER_H
#define __GOGUIMANAGER_H

#include "ofMain.h"

#include "Singleton.h"
#include "Controller.h"
#include "Model.h"
#include "Logger.h"

#define GUIMANAGER		goGuiManagerSingleton::Instance()

#define FOLDERS         GUIMANAGER->folderBox
#define LASTFOLDERS     GUIMANAGER->lastFolderBox

#define XFADE           GUIMANAGER->xfade
#define XFADETRUE       GUIMANAGER->trueXFader
#define XFADEMUTE       GUIMANAGER->xfademute
#define XFUNCMUTE       GUIMANAGER->xfuncmute
#define REVERSECHANNELS GUIMANAGER->reversechannels
#define CHANNELADIRECT  GUIMANAGER->channeladirect
#define CHANNELBDIRECT  GUIMANAGER->channelbdirect

#define XMODES          GUIMANAGER->xmodes
#define XFUNCS          GUIMANAGER->xfuncs

#define SOLENOIDCHANNEL GUIMANAGER->solenoidChannel
#define PLAYSOLENOIDS   GUIMANAGER->playSolenoids
#define CONTROLCHANNEL  GUIMANAGER->controlChannel
#define LISTENCHBEG     GUIMANAGER->listenChannelBegin
#define LISTENCHEND     GUIMANAGER->listenChannelEnd
#define REMAPCHAN       GUIMANAGER->remapChannel
#define LISTENNTBEG     GUIMANAGER->listenNoteBegin
#define LISTENNTEND     GUIMANAGER->listenNoteEnd
#define REMAPNTBEG      GUIMANAGER->remapNoteBegin
#define REMAPNTEND      GUIMANAGER->remapNoteEnd
#define REMAPMODE       GUIMANAGER->remapMode
#define CHANNELMODE     GUIMANAGER->channelMode
#define PARTICLEMODE    GUIMANAGER->particleMode
#define LEARNRANGE      GUIMANAGER->learnRange
#define REMAPOSC        GUIMANAGER->remaposc
#define OSCCHANNEL      GUIMANAGER->oscchannel
#define PROTECTCONTROL  GUIMANAGER->protect
#define SELECTIONGRP    GUIMANAGER->selectiongroup

class goGuiManager
{
public:
    goGuiManager();
    virtual ~goGuiManager();
    goGuiManager(const goGuiManager& other);
    goGuiManager& operator=(const goGuiManager& other);

    void    setup();
    void    update();
    // gui variables
    int                         folderBox[MAX_VIDEO_CHANNELS];
    int                         lastFolderBox[MAX_VIDEO_CHANNELS];

    float                       xfade;
    bool                        xfademute, xfuncmute, trueXFader, reversechannels;
    bool                        channeladirect, channelbdirect;

    int                         xmodes[2], xfuncs[2];

    int                         solenoidChannel;
    bool                        playSolenoids, protect;
    int                         controlChannel;
    int                         selectiongroup[2];

    int                         listenChannelBegin[6];
    int                         listenChannelEnd[6];
    int                         remapChannel[6];
    int                         listenNoteBegin[6];
    int                         listenNoteEnd[6];
    int                         remapNoteBegin[6];
    int                         remapNoteEnd[6];
    int                         remapMode[6];
    int                         channelMode[6];
    int                         particleMode[6];
    bool                        learnRange[6];

    bool                        remaposc[5];
    int                         oscchannel[5];

protected:

private:

    void    checkVideoFolders();

    void    groupLoadDone(int & id);
};

typedef Singleton<goGuiManager> goGuiManagerSingleton;   // Global declaration

#endif // __GOGUIMANAGER_H
