#ifndef __GOVIDEOEFFECTCL_H
#define __GOVIDEOEFFECTCL_H

#include "MSAOpenCL.h"
#include "goVideoPlayer.h"


class goVideoEffectCL
{
    public:
        goVideoEffectCL();
        virtual ~goVideoEffectCL();

        float				currentFPS;

        void                allocate(goVideoPlayer * _video);

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
        cl_float			threshLevel;
        cl_float            rLevel, gLevel, bLevel, aLevel, contrastLevel, saturationLevel, brightnessLevel;


    protected:
	
        MSA::OpenCLImage	clImage[2];             // two OpenCL images

#ifndef TARGET_OSX
        ofTexture		    texture[2];
        cl_sampler		    clSampler;				// sampler used for sampling clImages
#endif

        int					activeImageIndex;

        unsigned char		*pixels;				// temp buffer

        bool                allocated;

        int					vidWidth;
        int					vidHeight;

    private:

        goVideoPlayer *     video;
};

#endif // __GOVIDEOEFFECTCL_H
