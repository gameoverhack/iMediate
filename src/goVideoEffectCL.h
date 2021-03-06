#ifndef __GOVIDEOEFFECTCL_H
#define __GOVIDEOEFFECTCL_H

#include "MSAOpenCL.h"
#include "goThreadedVideo.h"
#include "ofxFbo.h"

class goVideoEffectCL
{
    public:
        goVideoEffectCL();
        virtual ~goVideoEffectCL();

        float				currentFPS;

        void                allocate(goThreadedVideo * _video, int _vidWidth, int _vidHeight);
        void                reallocate(goThreadedVideo * _video, int _vidWidth, int _vidHeight);

        void                setup();
        void                update();
        void                draw();

// parameters
        bool				doBlur;
        bool				doFlipX;
        bool				doFlipY;
        bool				doGreyscale;
        bool				doInvert;
        bool				doThreshold;
        bool                doHue;
        bool                doSaturation;
        bool                doContrast;
        bool                doBrightness;

        int					blurAmount;
        float   			threshLevel;
        float               rLevel, gLevel, bLevel, aLevel, contrastLevel, saturationLevel, brightnessLevel;
        ofColor             hueColour;

        bool                allocated;
        bool                muteAll;

        goThreadedVideo *     video;

        float                 videoSpeed;
        float                 fadeLevel;

    protected:

        ofxFbo              fbo;

        int                 fboTexID;

        MSA::OpenCLImage	clImage[2];             // two OpenCL images

#ifndef TARGET_OSX
        ofTexture		    texture[2];
        cl_sampler		    clSampler;				// sampler used for sampling clImages
#endif

        int					activeImageIndex;

        unsigned char		*pixels;				// temp buffer

        int					vidWidth;
        int					vidHeight;
        float               lastSpeed;

    private:


};

#endif // __GOVIDEOEFFECTCL_H
