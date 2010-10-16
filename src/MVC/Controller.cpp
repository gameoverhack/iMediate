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
            ofxSimpleGuiComboBox box = GUI.addComboBox("folderBrowser " + ofToString(i+1), FOLDERS[i], FILES.getNumberOfDirectories(),  folderArray);
            ofAddListener(GROUPS[i].groupLoaded, this, &Controller::groupLoadDone);

        }

        GUI.loadFromXML();

        GUI.show();

        EFFECTS[1].doBlur = true;
        EFFECTS[1].blurAmount = 5;

        EFFECTS[0].doInvert = true;

        DICTIONARY.loadText(ofToDataPath("Spanish.txt"));
        DICTIONARY.loadText(ofToDataPath("French.txt"));
        DICTIONARY.loadText(ofToDataPath("Latin.txt"));
        DICTIONARY.loadText(ofToDataPath("German.txt"));
        DICTIONARY.loadText(ofToDataPath("Italian.txt"));
        DICTIONARY.loadText(ofToDataPath("Portuguese.txt"));

        for (int i = 0; i < DICTIONARY.words.size(); i++)
        {
            cout << i << " = " << DICTIONARY.words[i] << endl;
        }

/*

		PLAYERS[0].loadMovie("/Volumes/GhostDriverX/Users/gameoverx/Desktop/vjMedia/trainStation/other/slow_legs-JPEG720-JPEG540.mov");
		PLAYERS[1].loadMovie("/Volumes/GhostDriverX/Users/gameoverx/Desktop/vjMedia/trains02/train2/train17-JPEG720-JPEG540.mov");
#else
		PLAYERS[0].loadMovie("C:/Users/gameoverwell/Desktop/slow_legs-JPEG720-JPEG540.mov");
		PLAYERS[1].loadMovie("C:/Users/gameoverwell/Desktop/train17-JPEG720-JPEG540.mov");
#endif

        PLAYERS[0].play();
        PLAYERS[1].play();

        EFFECTS[0].allocate(&PLAYERS[0], 720, 405);

        EFFECTS[1].allocate(&PLAYERS[1], 720, 405);

        EFFECTS[1].doBlur = true;
        EFFECTS[1].blurAmount = 5;

        EFFECTS[0].doInvert = true;
*/
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

            for (int j = 0; j < files.size(); j++)
            {
                cout << i << " :: " << j << " :: " << files[i] << endl;
            }
        }
    }


}

void Controller::groupLoadDone(int & id)
{

    EFFECTS[id].reallocate(&GROUPS[id].videoGroup[0], 720, 405);
    //GROUPS[id].videoGroup[0].forceTexture();
}
