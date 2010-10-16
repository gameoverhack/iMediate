#include "goVideoGroup.h"

goVideoGroup::goVideoGroup()
{
    //ctor
    myID = instanceCount;
    instanceCount++;
    locked = false;
    loadNext = false;

    for (int i = 0; i < MAX_VIDEOS_IN_GROUP; i++)
    {
        videoGroup[i].verbose = true;
        ofAddListener(videoGroup[i].loadDone, this, &goVideoGroup::success);
        ofAddListener(videoGroup[i].error, this, &goVideoGroup::error);
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
        videoGroup[numberLoaded].psuedoUpdate();
        videoGroup[numberLoaded].psuedoDraw();
    }

    if (loadNext)
    {
        loadNext = false;
        videoGroup[numberLoaded].loadMovie(filesToLoad.at(numberLoaded));
    }

}

void goVideoGroup::loadVectorOfVideos(vector<string> * paths)
{

    if(!locked)
    {
        filesToLoad.clear();
        locked = true;
        numberToLoad = MIN(paths->size(), MAX_VIDEOS_IN_GROUP);
        for (int i = 0; i < numberToLoad; i++)
        {
            filesToLoad.push_back(paths->at(i));;
        }
        numberLoaded = 0;
        loadNext = true;
    }

}

void goVideoGroup::success(string & name)
{
    if(numberLoaded + 1 < numberToLoad)
    {
        numberLoaded++;
        loadNext = true;
    } else {
        locked = false;
        ofNotifyEvent(groupLoaded, myID);
    }
}


void goVideoGroup::error(int & code)
{
    loadNext = false;
    locked = false;
}
