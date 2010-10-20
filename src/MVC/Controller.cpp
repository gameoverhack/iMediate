/*
 *  Controller.cpp
 *  openFrameworks
 *
 *  Created by gameoverx on 1/09/09.
 *  Copyright 2009 __MyCompanyName__. All rights reserved.
 *
 */

#include "Controller.h"

void Controller::setup()
{

    SETAPPSTATE(APP_INIT);

    LOG("Application Initializing");

    // Load in the config XML and setup the Model
    bool abort = true;

    if (APPLOADER->setup("config.xml"))
    {
        abort = false;
    }
    else
    {
        LOG("...config error. Exiting!");
        abort = true;
    }

    if(VIEW->setup())
    {
        abort = false;
    }
    else
    {
        LOG("...view failed to setup. Exiting!");
        abort = true;
    }

    if(abort)
    {
        // exit!
        exit(0);
    }
    else
    {

        ofSetLogLevel(OF_LOG_VERBOSE);

        string rootVideoFolder;

#ifdef TARGET_OSX
        rootVideoFolder = "/Volumes/GhostDriverX/Users/gameoverx/Desktop/vjMedia/";
#else
        rootVideoFolder = "C:/Users/gameoverwell/Desktop/vjMedia";
#endif

        FILES.setVerbose(true);
        FILES.allowExt("mov");
        FILES.listDir(rootVideoFolder, true);

        string folderArray[1000]; // silly big number for max number of directories

        for (int i = 0; i < FILES.getNumberOfDirectories(); i++)
        {
            folderArray[i] = FILES.directoriesWithFiles.at(i).substr(rootVideoFolder.size());
        }

        // start a new group
        GUI.addTitle("Video Folders");
        for (int i = 0; i < MAX_VIDEO_CHANNELS; i++)
        {
            GUI.addComboBox("folderBrowser " + ofToString(i+1), FOLDERS[i], FILES.getNumberOfDirectories(),  folderArray);
            ofAddListener(GROUPS[i].groupLoaded, this, &Controller::groupLoadDone);

        }

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
for (int i = 0; i < MAX_VIDEO_CHANNELS; i++)
    {
        gui.addTitle("Previews" + i).setNewColumn(true);
        for (int j = 0; j < MAX_VIDEOS_IN_GROUP; j++)
        {
            GUI.addContent("file", GROUPS[i].videoPreviews[j], 40);
        }
    }
        GUI.loadFromXML();

        GUI.show();

        DICTIONARY.loadText(ofToDataPath("Spanish.txt"));
        DICTIONARY.loadText(ofToDataPath("French.txt"));
        DICTIONARY.loadText(ofToDataPath("Latin.txt"));
        DICTIONARY.loadText(ofToDataPath("German.txt"));
        DICTIONARY.loadText(ofToDataPath("Italian.txt"));
        DICTIONARY.loadText(ofToDataPath("Portuguese.txt"));

        for (int i = 0; i < DICTIONARY.words.size(); i++)
        {
            //cout << i << " = " << DICTIONARY.words[i] << endl;
        }


        // start application
        SETAPPSTATE(APP_READY);
    }


}

void Controller::checkFolders()
{
    for (int i = 0; i < MAX_VIDEO_CHANNELS; i++)
    {
        if (FOLDERS[i] != LASTFOLDERS[i])
        {
            LASTFOLDERS[i] = FOLDERS[i];

            vector<string> files;
            FILES.getFilesByDirectory(FOLDERS[i], &files);

            GROUPS[i].loadVectorOfVideos(&files);

            /*for (int j = 0; j < files.size(); j++)
            {
                cout << i << " :: " << j << " :: " << files[i] << endl;
            }
            */
        }
    }

}

void Controller::groupLoadDone(int & id)
{



}
