#include "goThreadedImage.h"

goThreadedImage::goThreadedImage()
{
    //ctor
}

goThreadedImage::~goThreadedImage()
{
    //dtor
}

void goThreadedImage::saveThreaded(string fileName) {
	this->fileName = fileName;
	state = SAVE;
	startThread(false, false);   // blocking, verbose
}

void goThreadedImage::loadThreaded(string fileName) {
	this->fileName = fileName;
	state = LOAD;
	startThread(false, false);   // blocking, verbose
}

void goThreadedImage::threadedFunction()
{
    while( isThreadRunning() != 0 ){
        stopThread();
        if(state == SAVE) {
            if( lock() ){
                saveImage(fileName);
                unlock();
            } else ofLog(OF_LOG_VERBOSE, "Cannot save image because I'm locked");
        }
        if(state == LOAD) {
            if( lock() ){
                loadImage(fileName);
                unlock();
            } else ofLog(OF_LOG_VERBOSE, "Cannot load image because I'm locked");
        }
    }
}

goThreadedImage::goThreadedImage(const goThreadedImage& other)
{
    //copy ctor
}

goThreadedImage& goThreadedImage::operator=(const goThreadedImage& rhs)
{
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}
