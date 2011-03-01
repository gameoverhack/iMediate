#include "goVideoEffectCL.h"

static MSA::OpenCL openCL;
static bool isSetup = false;
static int effectInstanceCount = 0;
goVideoEffectCL::goVideoEffectCL()
{
    doBlur          = false;
    doFlipX         = false;
    doFlipY         = false;
    doGreyscale     = false;
    doInvert        = false;
    doThreshold     = false;
    doHue           = false;
    doSaturation    = false;
    doContrast      = false;
    doBrightness    = false;

    activeImageIndex = 0;
    allocated = false;
}

goVideoEffectCL::~goVideoEffectCL()
{
    //dtor
    openCL.finish();
}

void goVideoEffectCL::reallocate(goThreadedVideo * _video, int _vidWidth, int _vidHeight)
{
    if (!allocated)
    {
        allocate(_video, _vidWidth, _vidHeight);
    }
    else
    {
        video = _video;
        vidWidth = _vidWidth;
        vidHeight = _vidHeight;
    }
}

void goVideoEffectCL::setup()
{
    if(!isSetup)
    {
        // init openCL from OpenGL context to enable GL-CL data sharing
        openCL.setupFromOpenGL();
        // load and compile openCL program
        openCL.loadProgramFromFile("MSAopenCL/ImageProcessing.cl");

        // load kernels
        openCL.loadKernel("msa_boxblur");
        openCL.loadKernel("msa_flipx");
        openCL.loadKernel("msa_flipy");
        openCL.loadKernel("msa_greyscale");
        openCL.loadKernel("msa_invert");
        openCL.loadKernel("msa_threshold");
        openCL.loadKernel("go_hue");
        openCL.loadKernel("go_saturation");
        openCL.loadKernel("go_contrast");
        openCL.loadKernel("go_brightness");

        isSetup = true;

    }
}

void goVideoEffectCL::allocate(goThreadedVideo * _video, int _vidWidth, int _vidHeight)
{

    video = _video;
    vidWidth = _vidWidth;
    vidHeight = _vidHeight;

    fboTexID = effectInstanceCount;
    effectInstanceCount++;

    fbo.setup(_vidWidth, _vidHeight);

    // allocate temp buffer
    //pixels		= new unsigned char[vidWidth * vidHeight * 4];



#ifdef TARGET_OSX
    // create openCL textures and related OpenGL textures
    clImage[0].initWithTexture(vidWidth, vidHeight, GL_RGBA);
    clImage[1].initWithTexture(vidWidth, vidHeight, GL_RGBA);
#else
    // allocate textures
    texture[0].allocate(vidWidth, vidHeight, GL_RGBA);
    texture[1].allocate(vidWidth, vidHeight, GL_RGBA);
    // create openCL textures mapped to OpenGL textures
    clImage[0] = *(openCL.createImageFromTexture(texture[0]));
    clImage[1] = *(openCL.createImageFromTexture(texture[1]));
#endif


    // create image sampler
    // gameover: has been hard coded into the kernels - maybe should be kept as an argument??
    //clSampler = clCreateSampler(openCL.getContext(), CL_FALSE, CL_ADDRESS_CLAMP_TO_EDGE, CL_FILTER_NEAREST, NULL);

    allocated = true;
}

void goVideoEffectCL::update()
{

    if(allocated)
    {

        if(lastSpeed != videoSpeed)
        {
            lastSpeed = videoSpeed;
            video->setSpeed(videoSpeed);
        }

        video->update();

        // if there is a new frame....
        if(video->isFrameNew())
        {

#ifdef TARGET_OSX
            fbo.attach(clImage[activeImageIndex].getTexture(), 0);
#else
            fbo.attach(texture[activeImageIndex], 0);
#endif

            fbo.begin();
            glPushMatrix();
            glColor4f(fadeLevel, fadeLevel, fadeLevel, fadeLevel);
            video->draw(0, 0, vidWidth, vidHeight);
            glPopMatrix();
            fbo.end();
            //glFinish();
            if(!muteAll)
            {
                clEnqueueAcquireGLObjects(openCL.getQueue(), 1, &clImage[activeImageIndex].getCLMem(), 0,0,0); // added by gameover (matt gingold)
                clEnqueueAcquireGLObjects(openCL.getQueue(), 1, &clImage[1-activeImageIndex].getCLMem(), 0,0,0); // added by gameover (matt gingold)

                if(doBlur)
                {
                    MSA::OpenCLKernel *kernel = openCL.kernel("msa_boxblur");
                    for(int i=0; i<blurAmount; i++)
                    {
                        cl_int offset = i * i / 2 + 1;
                        kernel->setArg(0, clImage[activeImageIndex].getCLMem());
                        kernel->setArg(1, clImage[1-activeImageIndex].getCLMem());
                        kernel->setArg(2, offset);
                        kernel->run2D(vidWidth, vidHeight);
                        activeImageIndex = 1 - activeImageIndex;
                    }
                }

                if(doFlipX)
                {
                    MSA::OpenCLKernel *kernel = openCL.kernel("msa_flipx");
                    kernel->setArg(0, clImage[activeImageIndex].getCLMem());
                    kernel->setArg(1, clImage[1-activeImageIndex].getCLMem());
                    kernel->run2D(vidWidth, vidHeight);
                    activeImageIndex = 1 - activeImageIndex;
                }


                if(doFlipY)
                {
                    MSA::OpenCLKernel *kernel = openCL.kernel("msa_flipy");
                    kernel->setArg(0, clImage[activeImageIndex].getCLMem());
                    kernel->setArg(1, clImage[1-activeImageIndex].getCLMem());
                    kernel->run2D(vidWidth, vidHeight);
                    activeImageIndex = 1 - activeImageIndex;
                }

                if(doGreyscale)
                {
                    MSA::OpenCLKernel *kernel = openCL.kernel("msa_greyscale");
                    kernel->setArg(0, clImage[activeImageIndex].getCLMem());
                    kernel->setArg(1, clImage[1-activeImageIndex].getCLMem());
                    kernel->run2D(vidWidth, vidHeight);
                    activeImageIndex = 1 - activeImageIndex;
                }

                if(doInvert)
                {
                    MSA::OpenCLKernel *kernel = openCL.kernel("msa_invert");
                    kernel->setArg(0, clImage[activeImageIndex].getCLMem());
                    kernel->setArg(1, clImage[1-activeImageIndex].getCLMem());
                    kernel->run2D(vidWidth, vidHeight);
                    activeImageIndex = 1 - activeImageIndex;
                }

                if(doThreshold)
                {
                    MSA::OpenCLKernel *kernel = openCL.kernel("msa_threshold");
                    kernel->setArg(0, clImage[activeImageIndex].getCLMem());
                    kernel->setArg(1, clImage[1-activeImageIndex].getCLMem());
                    kernel->setArg(2, threshLevel);
                    kernel->run2D(vidWidth, vidHeight);
                    activeImageIndex = 1 - activeImageIndex;
                }

                if(doHue)
                {
                    MSA::OpenCLKernel *kernel = openCL.kernel("go_hue");
                    kernel->setArg(0, clImage[activeImageIndex].getCLMem());
                    kernel->setArg(1, clImage[1-activeImageIndex].getCLMem());
                    rLevel = hueColour.r;
                    gLevel = hueColour.g;
                    bLevel = hueColour.b;
                    aLevel = hueColour.a;
                    kernel->setArg(2, rLevel);
                    kernel->setArg(3, gLevel);
                    kernel->setArg(4, bLevel);
                    kernel->setArg(5, aLevel);
                    kernel->run2D(vidWidth, vidHeight);
                    activeImageIndex = 1 - activeImageIndex;
                }

                if(doSaturation)
                {
                    MSA::OpenCLKernel *kernel = openCL.kernel("go_saturation");
                    kernel->setArg(0, clImage[activeImageIndex].getCLMem());
                    kernel->setArg(1, clImage[1-activeImageIndex].getCLMem());
                    kernel->setArg(2, saturationLevel);
                    kernel->run2D(vidWidth, vidHeight);
                    activeImageIndex = 1 - activeImageIndex;
                }

                if(doContrast)
                {
                    MSA::OpenCLKernel *kernel = openCL.kernel("go_contrast");
                    kernel->setArg(0, clImage[activeImageIndex].getCLMem());
                    kernel->setArg(1, clImage[1-activeImageIndex].getCLMem());
                    kernel->setArg(2, contrastLevel);
                    kernel->run2D(vidWidth, vidHeight);
                    activeImageIndex = 1 - activeImageIndex;
                }

                if(doBrightness)
                {
                    MSA::OpenCLKernel *kernel = openCL.kernel("go_brightness");
                    kernel->setArg(0, clImage[activeImageIndex].getCLMem());
                    kernel->setArg(1, clImage[1-activeImageIndex].getCLMem());
                    kernel->setArg(2, brightnessLevel);
                    kernel->run2D(vidWidth, vidHeight);
                    activeImageIndex = 1 - activeImageIndex;
                }

                clEnqueueReleaseGLObjects(openCL.getQueue(), 1, &clImage[activeImageIndex].getCLMem(), 0,0,0); // added by gameover (matt gingold)
                clEnqueueReleaseGLObjects(openCL.getQueue(), 1, &clImage[1-activeImageIndex].getCLMem(), 0,0,0); // added by gameover (matt gingold)
            }
            // calculate capture fps
            static float lastTime = 0;
            float nowTime = ofGetElapsedTimef();
            float timeDiff = nowTime - lastTime;
            if(timeDiff > 0 ) currentFPS = 0.9f * currentFPS + 0.1f / timeDiff;
            lastTime = nowTime;

        }
    }

}

void goVideoEffectCL::draw()
{
    if (allocated)
    {
        // make sure all openCL kernels have finished executing before drawing
        openCL.finish();

#ifdef TARGET_OSX
        // draw the OpenGL texture (which was mapped to the openCL image)
        clImage[activeImageIndex].getTexture().draw(0, 0);
#else
        texture[activeImageIndex].draw(0, 0);
#endif
    }


}

