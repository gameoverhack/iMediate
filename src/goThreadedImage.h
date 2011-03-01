#ifndef __GOTHREADEDIMAGE_H
#define __GOTHREADEDIMAGE_H

#include "ofMain.h"
#include "ofxThread.h"

enum threadedImageState
{
    LOAD,
    SAVE
};

const string threadedImageState[] =
{
    "LOAD",
    "SAVE"
};

class goThreadedImage : public ofxThread, public ofImage
{
    public:

        goThreadedImage();
        virtual ~goThreadedImage();
        goThreadedImage(const goThreadedImage& other);
        goThreadedImage& operator=(const goThreadedImage& other);

        string fileName;

        void saveThreaded(string fileName);
        void loadThreaded(string fileName);

    protected:

    private:

        int     state;

        void threadedFunction();
};

#endif // __GOTHREADEDIMAGE_H
