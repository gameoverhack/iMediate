#include "goGuiManager.h"

goGuiManager::goGuiManager()
{
    //ctor
}

goGuiManager::~goGuiManager()
{
    //dtor
}

void goGuiManager::setup()
{
    string xblendEquationNamesAsString[] =
    {
        "FUNC_ADD",
        "MIN",
        "MAX",
        "FUNC_SUBTRACT",
        "FUNC_REVERSE_SUBTRACT",
    };
    string xblendModeNamesAsString[] =
    {
        "ZERO",
        "ONE",
        "SRC_COLOR",
        "ONE_MINUS_SRC_COLOR",
        "SRC_ALPHA",
        "ONE_MINUS_SRC_ALPHA",
        "DST_ALPHA",
        "ONE_MINUS_DST_ALPHA"
    };
    string remapModeNames[] = {"NONE",
                               "PARTICLE_GENERATE",
                               "RANDOM_VIDEO",
                               "NOTE_TO_VIDEOS",
                               "FX_BLUR",
                               "FX_FLIP_X",
                               "FX_FLIP_Y",
                               "FX_GREYSCALE",
                               "FX_INVERT",
                               "FX_THRESHOLD",
                               "FX_SATURATION",
                               "FX_CONTRAST",
                               "FX_BRIGHTNESS",
                               "X_FADER",
                               "CH_FADER",
                               "REVERSE_CHANNELS"
                              };
    string channelModeNames[] = {"CHANNEL_BOTH",
                                 "CHANNEL_SPLIT",
                                 "CHANNEL_A",
                                 "CHANNEL_B"};
    string particleModeNames[] = {"ALL_LAYERS",
                                  "LAYER_01",
                                  "LAYER_02",
                                  "LAYER_03",
                                  "LAYER_04",
                                  "LAYER_05",
                                  "LAYER_06",
                                  "LAYER_07",
                                  "LAYER_08"
                                 };
    // create drop down video folder groups
    GUI.addTitle("Video");
    for (int i = 0; i < MAX_VIDEO_CHANNELS; i++)
    {
        GUI.addComboBox("folderBrowser " + ofToString(i+1), FOLDERS[i], FILES.getNumberOfDirectories(),  FOLDERARRAY);
        //ofAddListener(GROUPS[i].groupLoaded, this, &goGuiManager::groupLoadDone);
    }

    GUI.addSlider("Channel A Speed", EFFECTS[0].videoSpeed, -5.0f, 5.0f);
    GUI.addSlider("Channel B Speed", EFFECTS[1].videoSpeed, -5.0f, 5.0f);
    GUI.addToggle("Particle Color", PARTICLES->particleColors);
    GUI.addToggle("Particle Size", PARTICLES->sizeParticle);
    GUI.addToggle("Particle Speed", PARTICLES->speedParticle);
    GUI.addToggle("Particle Link", PARTICLES->linkParticle);
    GUI.addToggle("Particle Erase", PARTICLES->eraseParticle);
    GUI.addSlider("Particle Size", PARTICLES->pWidth, 0, 720.0f);
    GUI.addSlider("Particle Type", PARTICLES->particlePattern, 0, 16);

    // create effects interface for video groups
    for (int i = 0; i < MAX_VIDEO_CHANNELS; i++)
    {
        GUI.addTitle("Effects Group" + i).setNewColumn(true);
        GUI.addToggle("Blur", EFFECTS[i].doBlur);
        GUI.addSlider("Blur Level", EFFECTS[i].blurAmount, 0.0, 20);
        GUI.addToggle("FlipX", EFFECTS[i].doFlipX);
        GUI.addToggle("FlipY", EFFECTS[i].doFlipY);
        GUI.addToggle("Greyscale", EFFECTS[i].doGreyscale);
        GUI.addToggle("Invert", EFFECTS[i].doInvert);
        GUI.addToggle("Threshold", EFFECTS[i].doThreshold);
        GUI.addSlider("Threshold Level", EFFECTS[i].threshLevel, 0.0, 1);
        GUI.addToggle("Hue", EFFECTS[i].doHue);
        GUI.addColorPicker("Hue Colour", &(EFFECTS[i].hueColour.r));
        GUI.addToggle("Saturation", EFFECTS[i].doSaturation);
        GUI.addSlider("Saturation Level", EFFECTS[i].saturationLevel, 0.0, 10);
        GUI.addToggle("Contrast", EFFECTS[i].doContrast);
        GUI.addSlider("Contrast Level", EFFECTS[i].contrastLevel, 0.0, 10);
        GUI.addToggle("Brightness", EFFECTS[i].doBrightness);
        GUI.addSlider("Brightness Level", EFFECTS[i].brightnessLevel, 0.0, 10);
    }

    GUI.addTitle("X-Faders & Blends").setNewColumn(true);
    GUI.addToggle("Reverse Channels", REVERSECHANNELS);
    GUI.addToggle("Channel A Direct", CHANNELADIRECT);
    GUI.addToggle("Channel A FXMute", EFFECTS[0].muteAll);
    GUI.addToggle("Channel B Direct", CHANNELBDIRECT);
    GUI.addToggle("Channel B FXMute", EFFECTS[1].muteAll);
    GUI.addSlider("Channel A Fade", EFFECTS[0].fadeLevel, 0, 1.0);
    GUI.addSlider("Channel B Fade", EFFECTS[1].fadeLevel, 0, 1.0);

    GUI.addSlider("X-Fade", XFADE, -1.0f, 1.0f);

    GUI.addComboBox("X-Fade Func Top", xfuncs[0], 5,  xblendEquationNamesAsString);
    for (int i = 0; i < 2; i++)
    {
        GUI.addComboBox("X-Fade Mode " + ofToString(i+1), xmodes[i], 8,  xblendModeNamesAsString);
    }
    GUI.addComboBox("X-Fade Func Bot", xfuncs[1], 5,  xblendEquationNamesAsString);

    GUI.addToggle("Use True Fader", XFADETRUE);
    GUI.addToggle("Use Fade Funcs", XFUNCMUTE);
    GUI.addToggle("Use Fade Modes", XFADEMUTE);

    GUI.addPage("MIDI");
    GUI.addTitle("Controller Settings");
    GUI.addToggle("MIDI Recieve", MIDIMANAGER->newMSG);
    GUI.addSlider("Solenoid Channel", solenoidChannel, 1, 16);
    GUI.addToggle("Play Solenoids", playSolenoids);
    GUI.addSlider("Controller Channel", controlChannel, 1, 16);

    for (int i = 0; i < 3; i++)
    {
        GUI.addTitle("MIDI Remapper" + i).setNewColumn(true);
        GUI.addSlider("Listen Channel Begin", LISTENCHBEG[i], 1, 16);
        GUI.addSlider("Listen Channel End", LISTENCHEND[i], 1, 16);
        GUI.addToggle("RX in Channel Range", MIDIMANAGER->newMSGCH[i]);
        GUI.addSlider("Map Listen To Channel", REMAPCHAN[i], 1, 16);
        GUI.addSlider("Listen Note Begin", LISTENNTBEG[i], 1, 127);
        GUI.addSlider("Listen Note End", LISTENNTEND[i], 1, 127);
        GUI.addToggle("RX in Note Range", MIDIMANAGER->newMSGNT[i]);

        GUI.addComboBox("Remap Mode", REMAPMODE[i], 16, remapModeNames);
        GUI.addComboBox("Channel Mode", CHANNELMODE[i], 4, channelModeNames);
        GUI.addComboBox("Particle Mode", PARTICLEMODE[i], 9, particleModeNames);
    }

    //GUI.addSlider("Remap Note Begin", remapNoteBegin, 1, 127);
    //GUI.addSlider("Remap Note End", remapNoteEnd, 1, 127);

    GUI.addPage("OSC");
    GUI.addToggle("OSC Recieve", OSCMANAGER->newMSG);
    GUI.addSlider("Coefficient 1", OSCMANAGER->co1, 0, 1.0);
    GUI.addSlider("Coefficient 2", OSCMANAGER->co2, 0, 1.0);
    GUI.addSlider("Coefficient 3", OSCMANAGER->co3, 0, 1.0);
    GUI.addSlider("Amplitude", OSCMANAGER->amp, 0, 1.0);
    GUI.addSlider("Note", OSCMANAGER->note, 0, 127);

    GUI.loadFromXML();

    GUI.show();
}

void goGuiManager::update()
{
    checkVideoFolders();

}

void goGuiManager::checkVideoFolders()
{
    for (int i = 0; i < MAX_VIDEO_CHANNELS; i++)
    {
        if (FOLDERS[i] != LASTFOLDERS[i])
        {
            LASTFOLDERS[i] = FOLDERS[i];

            vector<string> files;
            FILES.getFilesByDirectory(FOLDERS[i], &files);

            GROUPS[i].loadVectorOfVideos(&files);

            //break;

        }
    }

}

void goGuiManager::groupLoadDone(int & id)
{

    // nothing for now

}

goGuiManager::goGuiManager(const goGuiManager& other)
{
    //copy ctor
}

goGuiManager& goGuiManager::operator=(const goGuiManager& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}
