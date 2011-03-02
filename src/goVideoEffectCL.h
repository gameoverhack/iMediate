#ifndef __GOVIDEOEFFECTCL_H
#define __GOVIDEOEFFECTCL_H

#include "MSAOpenCL.h"
#include "Constants.h"
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
        void                draw(float x, float y, float w, float h);

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

        float                 speed_video;
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

        int					w_video;
        int					h_video;
        float               lastSpeed;

    private:


};

#endif // __GOVIDEOEFFECTCL_H
