#include "goVideoGroup.h"

goVideoGroup::goVideoGroup()
{
    //ctor
    myID = instanceCount;
    instanceCount++;
    locked = false;
    loadNext = false;

    currentlyPlayingVideo = -1;
    lastPlayingVideo = -1;

    for (int i = 0; i < MAX_VIDEOS_IN_GROUP; i++)
    {
        videoGroup[i] = new goThreadedVideo();
        //videoGroup[i]->verbose = true;
        ofAddListener(videoGroup[i]->loadDone, this, &goVideoGroup::success);
        ofAddListener(videoGroup[i]->error, this, &goVideoGroup::error);
    }
}

goVideoGroup::~goVideoGroup()
{
    //dtor
}

void goVideoGroup::update()
{
    if(locked)
    {
        videoGroup[numberLoaded]->psuedoUpdate();
        videoGroup[numberLoaded]->psuedoDraw();
    }

    if (loadNext)
    {
        loadNext = false;
        currentlyLoadingVideo = filesToLoad.at(numberLoaded);
        videoGroup[numberLoaded]->loadMovie(filesToLoad.at(numberLoaded));
    }

}

void goVideoGroup::loadVectorOfVideos(vector<string> * paths)
{

    if(!locked)
    {

        // swap playing video into a cache - is this leaking? NO i HOPE
        if(currentlyPlayingVideo != -1)
        {
            delete PLAYERS[myID];
            PLAYERS[myID] = new goThreadedVideo();
            std::swap(PLAYERS[myID], videoGroup[currentlyPlayingVideo]);
            EFFECTS[myID].reallocate(PLAYERS[myID], 720, 405);
        }

        // delete all the listeners and old videoplayers
        for (int i = 0; i < MAX_VIDEOS_IN_GROUP; i++)
        {
            ofRemoveListener(videoGroup[i]->loadDone, this, &goVideoGroup::success);
            ofRemoveListener(videoGroup[i]->error, this, &goVideoGroup::error);
            delete videoGroup[i];
            videoGroup[i] = NULL;
        }

        delete [] videoGroup;

        // re initialise and create all the video players
        for (int i = 0; i < MAX_VIDEOS_IN_GROUP; i++)
        {
            videoGroup[i] = new goThreadedVideo();
            //videoGroup[i]->verbose = true;
            ofAddListener(videoGroup[i]->loadDone, this, &goVideoGroup::success);
            ofAddListener(videoGroup[i]->error, this, &goVideoGroup::error);
        }

        filesToLoad.clear();
        locked = true;

        // clamp the number of files to load to a maximum
        numberToLoad = MIN(paths->size(), MAX_VIDEOS_IN_GROUP);
        for (int i = 0; i < numberToLoad; i++)
        {
            filesToLoad.push_back(paths->at(i));;
        }
        numberLoaded = 0;
        loadNext = true;
    }

}

void goVideoGroup::playVideoInGroup(int index)
{
    if (index != currentlyPlayingVideo && index <= numberLoaded)
    {
        // start rolling the video and reallocate to Effects
        videoGroup[index]->setPaused(false);
        EFFECTS[myID].reallocate(videoGroup[index], 720, 405);

        // update variables
        if (currentlyPlayingVideo != -1) {
            videoGroup[currentlyPlayingVideo]->setPaused(true);
            lastPlayingVideo = currentlyPlayingVideo;
        }

        currentlyPlayingVideo = index;

    }

}

void goVideoGroup::drawPreviews(int x, int y)
{
    for (int i = 0; i < numberLoaded; i++)
    {
        videoPreviews[i].draw(x, y + i * (videoPreviews[i].getHeight()/10.0f + 2.0f),
                                        videoPreviews[i].getWidth()/10.0f,
                                        videoPreviews[i].getHeight()/10.0f);
    }
}

void goVideoGroup::success(string & name)
{
    if(numberLoaded + 1 < numberToLoad)
    {
        videoGroup[numberLoaded]->setPaused(true);
        videoPreviews[numberLoaded].setFromPixels(videoGroup[numberLoaded]->getPixels(),
                                                  videoGroup[numberLoaded]->width,
                                                  videoGroup[numberLoaded]->height,
                                                  OF_IMAGE_COLOR);
        //videoPreviews[numberLoaded].setup(videoGroup[numberLoaded]->width, videoGroup[numberLoaded]->height);
        //videoPreviews[numberLoaded].begin();
        //videoGroup[numberLoaded]->draw(0,0);
        //videoPreviews[numberLoaded].end();

        numberLoaded++;
        loadNext = true;
    } else {
        currentlyLoadingVideo = "ALL LOADED";
        locked = false;
        ofNotifyEvent(groupLoaded, myID);
    }
}

void goVideoGroup::error(int & code)
{
    loadNext = false;
    locked = false;
}
