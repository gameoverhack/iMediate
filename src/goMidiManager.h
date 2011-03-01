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
#define ARD             MIDIMANAGER->ard

enum remapModes
{
    NONE,
    SOLENOID_CONTROL,
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
        int                 ardTimer[8];
        bool				bSetupArduino;			// flag variable for setting up arduino once

        ofxMidiIn			midiIn;
        ofxMidiOut          midiOut;
        void				newMidiMessage(ofxMidiEventArgs& eventArgs);

        bool                newMSG, newMSGCH[3], newMSGNT[3];
        midiMSG             newMidiMsg;
        midiMSG				lastMidiMsg;

        int                 midiFXChannel;

    protected:
    private:
        void                remap(int & index, bool * var0, bool * var1);
        void                remap(int & index, int * var0, int * var1, float rBegin0, float rEnd0, float rBegin1, float rEnd1);
        void                remap(int & index, int rBegin0, int rEnd0, int rBegin1, int rEnd1);
        void                remap(int & index, int maxR0, int maxR1);
        //void                remap(cl_float * var0, cl_float * var1, float rBegin0, float rEnd0, float rBegin1, float rEnd1);
        void                remap(int & index, float * var0, float * var1, float rBegin0, float rEnd0, float rBegin1, float rEnd1);
};

typedef Singleton<goMidiManager> goMidiManagerSingleton;   // Global declaration

#endif // __GOMIDIMANAGER_H
