#include "goVideoEffectCL.h"


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
}

void goVideoEffectCL::allocate(goVideoPlayer * _video)
{

    video = _video;
    vidWidth = video->width;
    vidHeight = video->height;

    cout << vidWidth << endl;
    cout << vidHeight << endl;

	// allocate temp buffer
	pixels		= new unsigned char[vidWidth * vidHeight * 4];



#ifdef TARGET_OSX
	// create openCL textures and related OpenGL textures
	clImage[0].initWithTexture(vidWidth, vidHeight, GL_RGBA);
	clImage[1].initWithTexture(vidWidth, vidHeight, GL_RGBA);
#else
    // allocate textures
	texture[0].allocate(vidWidth, vidHeight, GL_RGBA);
	texture[1].allocate(vidWidth, vidHeight, GL_RGBA);
	// create openCL textures mapped to OpenGL textures
	clImage[0] = *(OPENCL.createImageFromTexture(texture[0]));
	clImage[1] = *(OPENCL.createImageFromTexture(texture[1]));
#endif

    //if (!OPENCL.isSetup)
    //{



        // create image sampler
        // gameover: has been hard coded into the kernels - maybe should be kept as an argument??
        //clSampler = clCreateSampler(OPENCL.getContext(), CL_FALSE, CL_ADDRESS_CLAMP_TO_EDGE, CL_FILTER_NEAREST, NULL);
    //}


    allocated = true;
}

void goVideoEffectCL::update()
{

    video->update();

    // if there is a new frame....
	if(video->isFrameNew()) {

		// RGB textures don't seem to work well. so need to copy the vidgrabber data into a RGBA texture
		int pixelIndex = 0;
		for(int i=0; i<vidWidth; i++) {
			for(int j=0; j<vidHeight; j++) {
				int indexRGB	= pixelIndex * 3;
				int indexRGBA	= pixelIndex * 4;
				pixels[indexRGBA  ] = video->getPixels()[indexRGB  ];
				pixels[indexRGBA+1] = video->getPixels()[indexRGB+1];
				pixels[indexRGBA+2] = video->getPixels()[indexRGB+2];
				pixels[indexRGBA+3] = 255;
				pixelIndex++;
			}
		}

#ifdef TARGET_OSX
		// write the new pixel data into the openCL Image (and thus the OpenGL texture)
		clImage[activeImageIndex].write(pixels);
#else
        //texture[activeImageIndex].loadData(video->getPixels(), vidWidth, vidHeight, GL_RGB);
        texture[activeImageIndex].loadData(pixels, vidWidth, vidHeight, GL_RGBA);
        glFinish();
#endif

        clEnqueueAcquireGLObjects(OPENCL.getQueue(), 1, &clImage[activeImageIndex].getCLMem(), 0,0,0); // added by gameover (matt gingold)
        clEnqueueAcquireGLObjects(OPENCL.getQueue(), 1, &clImage[1-activeImageIndex].getCLMem(), 0,0,0); // added by gameover (matt gingold)

		if(doBlur) {
			MSA::OpenCLKernel *kernel = OPENCL.kernel("msa_boxblur");
			for(int i=0; i<blurAmount; i++) {
				cl_int offset = i * i / 2 + 1;
				kernel->setArg(0, clImage[activeImageIndex].getCLMem());
				kernel->setArg(1, clImage[1-activeImageIndex].getCLMem());
				kernel->setArg(2, offset);
                kernel->run2D(vidWidth, vidHeight);
				activeImageIndex = 1 - activeImageIndex;
			}
		}

		if(doFlipX) {
			MSA::OpenCLKernel *kernel = OPENCL.kernel("msa_flipx");
			kernel->setArg(0, clImage[activeImageIndex].getCLMem());
			kernel->setArg(1, clImage[1-activeImageIndex].getCLMem());
            kernel->run2D(vidWidth, vidHeight);
			activeImageIndex = 1 - activeImageIndex;
		}


		if(doFlipY) {
			MSA::OpenCLKernel *kernel = OPENCL.kernel("msa_flipy");
			kernel->setArg(0, clImage[activeImageIndex].getCLMem());
			kernel->setArg(1, clImage[1-activeImageIndex].getCLMem());
            kernel->run2D(vidWidth, vidHeight);
			activeImageIndex = 1 - activeImageIndex;
		}

		if(doGreyscale) {
			MSA::OpenCLKernel *kernel = OPENCL.kernel("msa_greyscale");
			kernel->setArg(0, clImage[activeImageIndex].getCLMem());
			kernel->setArg(1, clImage[1-activeImageIndex].getCLMem());
            kernel->run2D(vidWidth, vidHeight);
			activeImageIndex = 1 - activeImageIndex;
		}

		if(doInvert) {
			MSA::OpenCLKernel *kernel = OPENCL.kernel("msa_invert");
			kernel->setArg(0, clImage[activeImageIndex].getCLMem());
			kernel->setArg(1, clImage[1-activeImageIndex].getCLMem());
            kernel->run2D(vidWidth, vidHeight);
			activeImageIndex = 1 - activeImageIndex;
		}

		if(doThreshold) {
			MSA::OpenCLKernel *kernel = OPENCL.kernel("msa_threshold");
			kernel->setArg(0, clImage[activeImageIndex].getCLMem());
			kernel->setArg(1, clImage[1-activeImageIndex].getCLMem());
			kernel->setArg(2, threshLevel);
            kernel->run2D(vidWidth, vidHeight);
			activeImageIndex = 1 - activeImageIndex;
		}

		if(doHue) {
			MSA::OpenCLKernel *kernel = OPENCL.kernel("go_hue");
			kernel->setArg(0, clImage[activeImageIndex].getCLMem());
			kernel->setArg(1, clImage[1-activeImageIndex].getCLMem());
			kernel->setArg(2, rLevel);
			kernel->setArg(3, gLevel);
			kernel->setArg(4, bLevel);
			kernel->setArg(5, aLevel);
            kernel->run2D(vidWidth, vidHeight);
			activeImageIndex = 1 - activeImageIndex;
		}

		if(doSaturation) {
			MSA::OpenCLKernel *kernel = OPENCL.kernel("go_saturation");
			kernel->setArg(0, clImage[activeImageIndex].getCLMem());
			kernel->setArg(1, clImage[1-activeImageIndex].getCLMem());
			kernel->setArg(2, saturationLevel);
            kernel->run2D(vidWidth, vidHeight);
			activeImageIndex = 1 - activeImageIndex;
		}

		if(doContrast) {
			MSA::OpenCLKernel *kernel = OPENCL.kernel("go_contrast");
			kernel->setArg(0, clImage[activeImageIndex].getCLMem());
			kernel->setArg(1, clImage[1-activeImageIndex].getCLMem());
			kernel->setArg(2, contrastLevel);
            kernel->run2D(vidWidth, vidHeight);
			activeImageIndex = 1 - activeImageIndex;
		}

		if(doBrightness) {
			MSA::OpenCLKernel *kernel = OPENCL.kernel("go_brightness");
			kernel->setArg(0, clImage[activeImageIndex].getCLMem());
			kernel->setArg(1, clImage[1-activeImageIndex].getCLMem());
			kernel->setArg(2, brightnessLevel);
            kernel->run2D(vidWidth, vidHeight);
			activeImageIndex = 1 - activeImageIndex;
		}

        clEnqueueReleaseGLObjects(OPENCL.getQueue(), 1, &clImage[activeImageIndex].getCLMem(), 0,0,0); // added by gameover (matt gingold)
        clEnqueueReleaseGLObjects(OPENCL.getQueue(), 1, &clImage[1-activeImageIndex].getCLMem(), 0,0,0); // added by gameover (matt gingold)
    }

    // calculate capture fps
    static float lastTime = 0;
    float nowTime = ofGetElapsedTimef();
    float timeDiff = nowTime - lastTime;
    if(timeDiff > 0 ) currentFPS = 0.9f * currentFPS + 0.1f / timeDiff;
    lastTime = nowTime;

}

void goVideoEffectCL::draw()
{
    // make sure all openCL kernels have finished executing before drawing
	OPENCL.finish();

#ifdef TARGET_OSX
	// draw the OpenGL texture (which was mapped to the openCL image)
	clImage[activeImageIndex].getTexture().draw(0, 0);
#else
	texture[activeImageIndex].draw(0, 0);
#endif

}

