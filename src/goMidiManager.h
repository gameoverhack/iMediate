#ifndef __GOMIDIMANAGER_H
#define __GOMIDIMANAGER_H

#include "ofMain.h"

#include "Singleton.h"
#include "Controller.h"
#include "Model.h"
#include "Logger.h"
#include "goGuiManager.h"
#include "goParticleMask.h"

#include "ofxMidi.h"

#define MIDIMANAGER		goMidiManagerSingleton::Instance()
#define LASTMIDIMSG     MIDIMANAGER->lastMidiMsg

enum remapModes
{
    NONE,
    PARTICLE_GENERATE,
    RANDOM_VIDEO,
    NOTE_TO_VIDEOS,
    FX_BLUR,
    FX_FLIP_X,
    FX_FLIP_Y,
    FX_GREYSCALE,
    FX_INVERT,
    FX_THRESHOLD,
    FX_SATURATION,
    FX_CONTRAST,
    FX_BRIGHTNESS,
    X_FADER,
    CH_FADER,
    REVERSE_CHANNELS
};

enum channelModes
{
    CHANNEL_BOTH,
    CHANNEL_SPLIT,
    CHANNEL_A,
    CHANNEL_B
};

enum particleModes
{
    ALL_LAYERS,
    LAYER_01,
    LAYER_02,
    LAYER_03,
    LAYER_04,
    LAYER_05,
    LAYER_06,
    LAYER_07,
    LAYER_08
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

        bool                newMSG, newMSGCH[3], newMSGNT[3];
        midiMSG             newMidiMsg;
        midiMSG				lastMidiMsg;

    protected:
    private:
};

typedef Singleton<goMidiManager> goMidiManagerSingleton;   // Global declaration

#endif // __GOMIDIMANAGER_H
