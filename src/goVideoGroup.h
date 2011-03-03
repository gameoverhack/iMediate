#ifndef __GOVIDEOGROUP_H
#define __GOVIDEOGROUP_H

#define MAX_VIDEOS_IN_GROUP 48

#include "Model.h"

#include "goThreadedVideo.h"
#include "goVideoEffectCL.h"
#include "goVideoPreview.h"

static int instanceCount = 0;

class goVideoGroup
{
    public:

        goVideoGroup();
        virtual ~goVideoGroup();

        goThreadedVideo         *videoGroup[MAX_VIDEOS_IN_GROUP];
        goVideoPreview          videoPreviews[MAX_VIDEOS_IN_GROUP];
        //goThreadedImage         videoPreviews[MAX_VIDEOS_IN_GROUP];
        //ofxFbo         videoPreviews[MAX_VIDEOS_IN_GROUP];

        void                setup(float x, float y);
        void                update();

        void                playVideoInGroup(int index);

        void                loadVectorOfVideos(vector<string> * paths);

        ofEvent<int>        groupLoaded;

        int                 myID;

        int                 currentlyPlayingVideo, lastPlayingVideo;

        string              currentlyLoadingVideo;

        int                 numberLoaded;

        float               w_output_scale, h_output_scale;
        float               x_output_scale, y_output_scale;
        float               position_video;

        bool                scratching;

    protected:

    private:

        void                scaleVideo(int index);

        void                success(string & name);
        void                error(int & code);
        void                previewClicked(int & index);

        bool                locked, loadNext;

        bool                lastLoopVideo, lastScaleIntoMe, lastMute;

        int                 numberToLoad;

        vector<string>      filesToLoad;



};

#endif // __GOVIDEOGROUP_H
