#ifndef __GOMIDIMANAGER_H
#define __GOMIDIMANAGER_H

#include "ofMain.h"

#include "Singleton.h"
#include "Controller.h"
#include "Model.h"
#include "Logger.h"
#include "goGuiManager.h"

#include "ofxMidi.h"

#define MIDIMANAGER		goMidiManagerSingleton::Instance()
#define LASTMIDIMSG     MIDIMANAGER->lastMidiMsg
#define REMAPMODE       MIDIMANAGER->remapMode

enum remapModes
{
    CHANGE_VIDEOS,
    CHANGE_BLUR
};

struct midiMSG {
	int 	port;
	int		channel;
	int 	status;
	int 	byteOne;
	int 	byteTwo;
	double 	timestamp;
};

class goMidiManager
{
    public:

        goMidiManager();
        virtual ~goMidiManager();
        goMidiManager(const goMidiManager& other);
        goMidiManager& operator=(const goMidiManager& other);

        void setup();
        void update();

        void setupArduino();
        void updateArduino();

        ofArduino			ard;
        bool				bSetupArduino;			// flag variable for setting up arduino once

        ofxMidiIn			midiIn;
        void				newMidiMessage(ofxMidiEventArgs& eventArgs);

        bool                newMSG, newMSGCH, newMSGNT;
        midiMSG             newMidiMsg;
        midiMSG				lastMidiMsg;

        int                 remapMode;
    protected:
    private:
};

typedef Singleton<goMidiManager> goMidiManagerSingleton;   // Global declaration

#endif // __GOMIDIMANAGER_H
