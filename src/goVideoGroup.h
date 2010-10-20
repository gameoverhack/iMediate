#ifndef __GOVIDEOGROUP_H
#define __GOVIDEOGROUP_H

#define MAX_VIDEOS_IN_GROUP 100

#include "Model.h"
#include "goThreadedVideo.h"
#include "goThreadedImage.h"

static int instanceCount = 0;

class goVideoGroup
{
    public:

        goVideoGroup();
        virtual ~goVideoGroup();

        goThreadedVideo *videoGroup[MAX_VIDEOS_IN_GROUP];
        goThreadedImage         videoPreviews[MAX_VIDEOS_IN_GROUP];
        //ofxFbo         videoPreviews[MAX_VIDEOS_IN_GROUP];

        void update();

        void playVideoInGroup(int index);
        void drawPreviews(int x, int y);

        void loadVectorOfVideos(vector<string> * paths);

        ofEvent<int>   groupLoaded;

        int             myID;

        int             currentlyPlayingVideo, lastPlayingVideo;

        string          currentlyLoadingVideo;

        int                 numberLoaded;

    protected:

    private:

        void    success(string & name);
        void    error(int & code);

        bool                locked, loadNext;
        int                 numberToLoad;

        vector<string>     filesToLoad;



};

#endif // __GOVIDEOGROUP_H
