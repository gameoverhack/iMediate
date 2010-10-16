#ifndef __GOVIDEOGROUP_H
#define __GOVIDEOGROUP_H

#define MAX_VIDEOS_IN_GROUP 100

#include "goThreadedVideo.h"
#include "goThreadedImage.h"

static int instanceCount = 0;

class goVideoGroup
{
    public:

        goVideoGroup();
        virtual ~goVideoGroup();

        goThreadedVideo videoGroup[MAX_VIDEOS_IN_GROUP];
        goThreadedImage videoPreviews[MAX_VIDEOS_IN_GROUP];

        void update();
        void loadVectorOfVideos(vector<string> * paths);

        ofEvent<int>   groupLoaded;

        int myID;

    protected:



    private:

        void    success(string & name);
        void    error(int & code);

        bool                locked, loadNext;
        int                 numberToLoad;
        int                 numberLoaded;

        vector<string>     filesToLoad;



};

#endif // __GOVIDEOGROUP_H
