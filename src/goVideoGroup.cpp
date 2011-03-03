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

    //cout << "SETTTTIIINGGG:  " << myID << endl;
}

goVideoGroup::~goVideoGroup()
{
    //dtor
}

void goVideoGroup::setup(float x, float y)
{

    int tRow = 0;

    for (int i = 0; i < MAX_VIDEOS_IN_GROUP; i++)
    {
        int tCol = i%4;
        int width = 720/10.0f;
        int height = 405/10.0f;

        if(tCol==0) tRow++;
        videoPreviews[i].setSize(width, height);
        videoPreviews[i].setPos(x + (width + 2.0f) * tCol,
                                y + (height + 2.0f) * tRow);

        videoPreviews[i].index = i;
        ofAddListener(videoPreviews[i].previewClicked, this, &goVideoGroup::previewClicked);

    }
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

    if (lastScaleIntoMe != SCALEINTOME && currentlyPlayingVideo != -1)
    {
        scaleVideo(currentlyPlayingVideo);
        lastScaleIntoMe = SCALEINTOME;
    }
    if (lastLoopVideo != LOOPVIDEO && currentlyPlayingVideo != -1) {
        if (LOOPVIDEO) videoGroup[currentlyPlayingVideo]->setLoopState(OF_LOOP_NORMAL);
        else videoGroup[currentlyPlayingVideo]->setLoopState(OF_LOOP_NONE);
        lastLoopVideo = LOOPVIDEO;
    }

    if (lastMute != MUTE[myID] && currentlyPlayingVideo != -1)
    {
        videoGroup[currentlyPlayingVideo]->setPaused(MUTE[myID]);
        lastMute = MUTE[myID];
    }
    if (currentlyPlayingVideo != -1) {
        if (scratching) {
            videoGroup[currentlyPlayingVideo]->setPaused(true);             // TODO: add isPaused() function to threaded videplayer so we don't hammer this AND cach paused state
            videoGroup[currentlyPlayingVideo]->setPosition(position_video);
        } else {
            videoGroup[currentlyPlayingVideo]->setPaused(false);
            position_video = videoGroup[currentlyPlayingVideo]->getPosition();
        }
    } else {
        position_video = 0.0f;
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
            EFFECTS[myID].reallocate(PLAYERS[myID], videoGroup[currentlyPlayingVideo]->getWidth(), videoGroup[currentlyPlayingVideo]->getHeight());
        }

        // delete all the listeners and old videoplayers
        for (int i = 0; i < MAX_VIDEOS_IN_GROUP; i++)
        {
            videoPreviews[i].disableAllEvents();
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

void goVideoGroup::previewClicked(int & index)
{
    //cout << "here" << endl;
    playVideoInGroup(index);
}

void goVideoGroup::playVideoInGroup(int index)
{
    if (REWINDONSTART) videoGroup[index]->setPosition(0);

    if (index != currentlyPlayingVideo && index <= numberLoaded)
    {
        // reset scale/aspect ratio
        scaleVideo(index);

        // start rolling the video and reallocate to Effects
        if (LOOPVIDEO) videoGroup[index]->setLoopState(OF_LOOP_NORMAL);
        else videoGroup[index]->setLoopState(OF_LOOP_NONE);

        videoGroup[index]->setPaused(false);
        EFFECTS[myID].reallocate(videoGroup[index], videoGroup[index]->getWidth(), videoGroup[index]->getHeight());

        // update variables
        if (currentlyPlayingVideo != -1)
        {
            videoGroup[currentlyPlayingVideo]->setPaused(true);
            lastPlayingVideo = currentlyPlayingVideo;
        }

        currentlyPlayingVideo = index;

    }
}

void goVideoGroup::scaleVideo(int index)
{
    float _aspect = 1.0f;
    float _width, _height;
    // for now just assume width is wider than height - TODO: scale either way properly
    if(currentlyPlayingVideo != -1)
    {
        _width = videoGroup[currentlyPlayingVideo]->getWidth();
        _height = videoGroup[currentlyPlayingVideo]->getHeight();
        //_aspect = (_width < W_OUTPUT_SCREEN || !SCALEINTOME) ? _height / _width : _width / _height;
    }

    if(SCALEINTOME)
    {

        // scale with overscan (ie no black bars if mismatched aspect ratio to output screen)

        // check if making it wider get's it high enoung
        if (_height * (W_FBODRAW_SCREEN/_width) <= H_FBODRAW_SCREEN)
        {
            // we need to scale by height
            w_output_scale = _width * (H_FBODRAW_SCREEN/_height);
            h_output_scale = H_FBODRAW_SCREEN;
        }
        else
        {
            // we need to scale by width
            w_output_scale = W_FBODRAW_SCREEN;
            h_output_scale = _height * (W_FBODRAW_SCREEN/_width);
        }

    }
    else
    {

        // no scale at all
        //w_output_scale = _width;
        //h_output_scale = _height;

        // scale to widest edge (ie., black bars if mismatched aspect ratio to output screen)
        // check if making it wider get's it too high
        if (_height * (W_FBODRAW_SCREEN/_width) > H_FBODRAW_SCREEN)
        {
            // we need to scale by height
            w_output_scale = _width * (H_FBODRAW_SCREEN/_height);
            h_output_scale = H_FBODRAW_SCREEN;
        }
        else
        {
            // we need to scale by width
            w_output_scale = W_FBODRAW_SCREEN;
            h_output_scale = _height * (W_FBODRAW_SCREEN/_width);
        }
    }

    x_output_scale = ( W_FBODRAW_SCREEN - w_output_scale ) / 2.0f;
    y_output_scale = ( H_FBODRAW_SCREEN - h_output_scale ) / 2.0f;

    //cout << i << " :: " << _width << " :: " << _height << " :: " << _aspect << " :: " << w_output_scale[i] << " :: " << h_output_scale[i] << " :: " << x_output_scale[i] << " :: " << y_output_scale[i] <<endl;

}

void goVideoGroup::success(string & name)
{

    if(numberLoaded + 1 < numberToLoad)
    {
        videoGroup[numberLoaded]->setPaused(true);
        loadNext = true;
    }
    else
    {
        currentlyLoadingVideo = "ALL LOADED";
        locked = false;

        ofNotifyEvent(groupLoaded, myID);
    }


    videoPreviews[numberLoaded].enableAllEvents();
    videoPreviews[numberLoaded].videoIcon.setFromPixels(videoGroup[numberLoaded]->getPixels(),
            videoGroup[numberLoaded]->width,
            videoGroup[numberLoaded]->height,
            OF_IMAGE_COLOR);

    numberLoaded++;
}

void goVideoGroup::error(int & code)
{
    loadNext = false;
    locked = false;
}
