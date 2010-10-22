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
    bool                        playSolenoids;
    int                         controlChannel;

    int                         listenChannelBegin[3];
    int                         listenChannelEnd[3];
    int                         remapChannel[3];
    int                         listenNoteBegin[3];
    int                         listenNoteEnd[3];
    int                         remapNoteBegin[3];
    int                         remapNoteEnd[3];
    int                         remapMode[3];
    int                         channelMode[3];
    int                         particleMode[3];

protected:

private:

    void    checkVideoFolders();

    void    groupLoadDone(int & id);
};

typedef Singleton<goGuiManager> goGuiManagerSingleton;   // Global declaration

#endif // __GOGUIMANAGER_H
