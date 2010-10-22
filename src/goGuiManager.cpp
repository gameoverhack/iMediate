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
    // create drop down video folder groups
    GUI.addTitle("Video");
    for (int i = 0; i < MAX_VIDEO_CHANNELS; i++)
    {
        GUI.addComboBox("folderBrowser " + ofToString(i+1), FOLDERS[i], FILES.getNumberOfDirectories(),  FOLDERARRAY);
        //ofAddListener(GROUPS[i].groupLoaded, this, &goGuiManager::groupLoadDone);
    }

    GUI.addSlider("Channel A Speed", EFFECTS[0].videoSpeed, -5.0f, 5.0f);
    GUI.addSlider("Channel B Speed", EFFECTS[1].videoSpeed, -5.0f, 5.0f);

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
        EFFECTS[i].threshLevel = 0.2;
        GUI.addToggle("Hue", EFFECTS[i].doHue);
        GUI.addColorPicker("Hue Colour", &(EFFECTS[i].hueColour.r));
        GUI.addToggle("Saturation", EFFECTS[i].doSaturation);
        GUI.addSlider("Saturation Level", EFFECTS[i].saturationLevel, 0.0, 10);
        EFFECTS[i].saturationLevel = 1.0;
        GUI.addToggle("Contrast", EFFECTS[i].doContrast);
        GUI.addSlider("Contrast Level", EFFECTS[i].contrastLevel, 0.0, 10);
        EFFECTS[i].contrastLevel = 1.0;
        GUI.addToggle("Brightness", EFFECTS[i].doBrightness);
        GUI.addSlider("Brightness Level", EFFECTS[i].brightnessLevel, 0.0, 10);
        EFFECTS[i].brightnessLevel = 1.0;
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

    for (int i = 0; i < 2; i++)
    {
        GUI.addComboBox("X-Fade Mode " + ofToString(i+1), xmodes[i], 8,  xblendModeNamesAsString);
    }

    GUI.addPage("MIDI");
    GUI.addToggle("MIDI Recieve", MIDIMANAGER->newMSG);
    GUI.addSlider("Solenoid Channel", solenoidChannel, 1, 16);
    GUI.addToggle("Play Solenoids", playSolenoids);
    GUI.addSlider("Controller Channel", controlChannel, 1, 16);
    GUI.addSlider("Listen Channel Begin", listenChannelBegin, 1, 16);
    GUI.addSlider("Listen Channel End", listenChannelEnd, 1, 16);
    GUI.addToggle("RX in Channel Range", MIDIMANAGER->newMSGCH);
    GUI.addSlider("Map Listen To Channel", remapChannel, 1, 16);
    GUI.addSlider("Listen Note Begin", listenNoteBegin, 1, 127);
    GUI.addSlider("Listen Note End", listenNoteEnd, 1, 127);
    GUI.addToggle("RX in Note Range", MIDIMANAGER->newMSGNT);
    string remapModeNames[] = {"CHANGE_VIDEOS", "CHANGE_BLUR"};
    GUI.addComboBox("Remap Mode", REMAPMODE, 2, remapModeNames);
    //GUI.addSlider("Remap Note Begin", remapNoteBegin, 1, 127);
    //GUI.addSlider("Remap Note End", remapNoteEnd, 1, 127);

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
