#ifndef PARTICLE_H
#define PARTICLE_H

#include "ofMain.h"
#include "ofxVectorMath.h"


class particle
{
    public:
        particle();
        virtual ~particle();

		ofxVec2f    getPosition(){ return pos; }
		float       getX(){ return pos.x; }
		float       getY(){ return pos.y; }

		void setup( float x_, float y_, float damp_, ofxVec2f vel_, int _pWidth, int _pHeight);
        void update();
        void draw();

        void addForce( ofxVec2f frc );

        ofxVec2f pos;
        ofxVec2f vel;

        float damp;
	
		int		pWidth;
		int		pHeight;

		void	setWidth(int _pWidth);
	
	protected:
    private:
};

#endif // PARTICLE_H
