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
                               "SOLENOID_CONTROL",
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

    GUI.addToggle("Particle Color", PARTICLES->particleColors);
    GUI.addToggle("Particle Size", PARTICLES->sizeParticle);
    GUI.addToggle("Particle Speed", PARTICLES->speedParticle);
    GUI.addToggle("Particle Link", PARTICLES->linkParticle);
    GUI.addToggle("Particle Erase", PARTICLES->eraseParticle);
    GUI.addSlider("Particle Size", PARTICLES->pWidth, 0, 720.0f);
    GUI.addSlider("Particle Speed", PARTICLES->pDamp, 0, 40.0f);
    GUI.addSlider("Particle Type", PARTICLES->particlePattern, 0, 16);
    GUI.addToggle("Scale Into Me", SCALEINTOME);
    GUI.addToggle("Rewind On Start", REWINDONSTART);
    GUI.addToggle("Loop Video", LOOPVIDEO);


    // create effects interface for video groups
    for (int i = 0; i < MAX_VIDEO_CHANNELS; i++)
    {
        GUI.addTitle("Effects Group" + ofToString(i)).setNewColumn(true);
        GUI.addToggle("Blur" + ofToString(i), EFFECTS[i].doBlur);
        GUI.addSlider("Blur Level" + ofToString(i), EFFECTS[i].blurAmount, 0, 20);
        GUI.addToggle("FlipX" + ofToString(i), EFFECTS[i].doFlipX);
        GUI.addToggle("FlipY" + ofToString(i), EFFECTS[i].doFlipY);
        GUI.addToggle("Greyscale" + ofToString(i), EFFECTS[i].doGreyscale);
        GUI.addToggle("Invert" + ofToString(i), EFFECTS[i].doInvert);
        GUI.addToggle("Threshold" + ofToString(i), EFFECTS[i].doThreshold);
        GUI.addSlider("Threshold Level" + ofToString(i), EFFECTS[i].threshLevel, 0.0, 1.0);
        GUI.addToggle("Hue" + ofToString(i), EFFECTS[i].doHue);
        GUI.addColorPicker("Hue Colour" + ofToString(i), &(EFFECTS[i].hueColour.r));
        GUI.addToggle("Saturation" + ofToString(i), EFFECTS[i].doSaturation);
        GUI.addSlider("Saturation Level" + ofToString(i), EFFECTS[i].saturationLevel, 0.0, 10.0);
        GUI.addToggle("Contrast" + ofToString(i), EFFECTS[i].doContrast);
        GUI.addSlider("Contrast Level" + ofToString(i), EFFECTS[i].contrastLevel, 0.0, 10.0);
        GUI.addToggle("Brightness" + ofToString(i), EFFECTS[i].doBrightness);
        GUI.addSlider("Brightness Level" + ofToString(i), EFFECTS[i].brightnessLevel, 0.0, 10.0);
    }

    GUI.addTitle("X-Faders & Blends").setNewColumn(true);
    GUI.addToggle("Reverse Channels", REVERSECHANNELS);
    GUI.addSlider("Channel A Speed", EFFECTS[0].speed_video, -5.0f, 5.0f);
    GUI.addSlider("Channel B Speed", EFFECTS[1].speed_video, -5.0f, 5.0f);
    GUI.addSlider("Channel A Position", GROUPS[0].position_video, 0.0f, 1.0f);
    GUI.addSlider("Channel B Position", GROUPS[1].position_video, 0.0f, 1.0f);
    GUI.addToggle("Channel A Direct", CHANNELADIRECT);
    GUI.addToggle("Channel A FXMute", EFFECTS[0].muteAll);
    GUI.addToggle("Channel A MUTE", MUTE[0]);
    GUI.addToggle("Channel B Direct", CHANNELBDIRECT);
    GUI.addToggle("Channel B FXMute", EFFECTS[1].muteAll);
    GUI.addToggle("Channel B MUTE", MUTE[1]);
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

    //GUI.addTitle("Scratch").setNewColumn(true);
    //GUI.addMovieSlider("Movie 1", (GROUPS[0].videoGroup[GROUPS[0].currentlyPlayingVideo]));
    //GUI.addMovieSlider("Movie 2", (GROUPS[1].videoGroup[GROUPS[1].currentlyPlayingVideo]));

    GUI.addPage("MIDI");
    GUI.addTitle("Controller Settings");
    GUI.addToggle("MIDI Recieve", MIDIMANAGER->newMSG);
    GUI.addSlider("Solenoid Channel", solenoidChannel, 1, 16);
    GUI.addToggle("Play Solenoids", playSolenoids);
    GUI.addSlider("Controller Channel", controlChannel, 1, 16);
    GUI.addToggle("Protect Control CH", PROTECTCONTROL);

    for (int i = 0; i < 3; i++)
    {
        GUI.addTitle("MIDI Remapper" + i).setNewColumn(true);
        GUI.addSlider("Listen Channel Begin" + ofToString(i), LISTENCHBEG[i], 1, 16);
        GUI.addSlider("Listen Channel End" + ofToString(i), LISTENCHEND[i], 1, 16);
        GUI.addToggle("RX in Channel Range" + ofToString(i), MIDIMANAGER->newMSGCH[i]);
        GUI.addSlider("Map Listen To Channel" + ofToString(i), REMAPCHAN[i], 1, 16);
        GUI.addSlider("Listen Note Begin" + ofToString(i), LISTENNTBEG[i], 1, 127);
        GUI.addSlider("Listen Note End" + ofToString(i), LISTENNTEND[i], 1, 127);
        GUI.addToggle("RX in Note Range" + ofToString(i), MIDIMANAGER->newMSGNT[i]);
        GUI.addToggle("Learn Note Range" + ofToString(i), LEARNRANGE[i]);

        GUI.addComboBox("Remap Mode" + ofToString(i), REMAPMODE[i], 16, remapModeNames);
        GUI.addComboBox("Channel Mode" + ofToString(i), CHANNELMODE[i], 4, channelModeNames);
        GUI.addComboBox("Particle Mode" + ofToString(i), PARTICLEMODE[i], 9, particleModeNames);
    }

    //GUI.addSlider("Remap Note Begin", remapNoteBegin, 1, 127);
    //GUI.addSlider("Remap Note End", remapNoteEnd, 1, 127);

    GUI.addPage("OSC");
    GUI.addToggle("OSC Recieve", OSCMANAGER->newMSG);
    GUI.addSlider("Coefficient 1", OSCDATA[0], 0, 1.0);
    GUI.addToggle("CO1 Remap to Midi", REMAPOSC[0]);
    GUI.addSlider("Remap Channel 1", OSCCHANNEL[0], 1, 19);
    GUI.addSlider("Coefficient 2", OSCDATA[1], 0, 1.0);
    GUI.addToggle("CO2 Remap to Midi", REMAPOSC[1]);
    GUI.addSlider("Remap Channel 2", OSCCHANNEL[1], 1, 19);
    GUI.addSlider("Coefficient 3", OSCDATA[2], 0, 1.0);
    GUI.addToggle("CO3 Remap to Midi", REMAPOSC[2]);
    GUI.addSlider("Remap Channel 3", OSCCHANNEL[2], 1, 19);
    GUI.addSlider("Amplitude", OSCDATA[3], 0, 1.0);
    GUI.addToggle("AMP Remap to Midi", REMAPOSC[3]);
    GUI.addSlider("Remap Channel 4", OSCCHANNEL[3], 1, 19);
    GUI.addSlider("Note", OSCDATA[4], 0, 127);
    GUI.addToggle("NTA Remap to Midi", REMAPOSC[4]);
    GUI.addSlider("Remap Channel 5", OSCCHANNEL[4], 1, 19);

    for (int i = 3; i < 6; i++)
    {
        GUI.addTitle("OSC->MIDI Remapper" + i).setNewColumn(true);
        GUI.addSlider("Listen Channel Begin" + ofToString(i), LISTENCHBEG[i], 1, 19);
        GUI.addSlider("Listen Channel End" + ofToString(i), LISTENCHEND[i], 1, 19);
        GUI.addToggle("RX in Channel Range" + ofToString(i), OSCMANAGER->newMSGCH[i-3]);
        GUI.addSlider("Map Listen To Channel" + ofToString(i), REMAPCHAN[i], 1, 19);
        GUI.addSlider("Listen Note Begin" + ofToString(i), LISTENNTBEG[i], 1, 127);
        GUI.addSlider("Listen Note End" + ofToString(i), LISTENNTEND[i], 1, 127);
        GUI.addToggle("RX in Note Range" + ofToString(i), OSCMANAGER->newMSGNT[i-3]);
        GUI.addToggle("Learn Note Range" + ofToString(i), LEARNRANGE[i]);

        GUI.addComboBox("Remap Mode" + ofToString(i), REMAPMODE[i], 16, remapModeNames);
        GUI.addComboBox("Channel Mode" + ofToString(i), CHANNELMODE[i], 4, channelModeNames);
        GUI.addComboBox("Particle Mode" + ofToString(i), PARTICLEMODE[i], 9, particleModeNames);
    }

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
