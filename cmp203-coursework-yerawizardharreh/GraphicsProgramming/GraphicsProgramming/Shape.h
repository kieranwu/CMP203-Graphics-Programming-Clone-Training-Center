//This Class handles rendering of shapes with use of vertex arrays

//This class was heavily changed from the one provided by the module CMP203 Graphics Programming
#ifndef _SHAPE_H
#define _SHAPE_H

//includes used
#include "glut.h"
#include <gl/gl.h>
#include <gl/glu.h>

class Shape
{

	public:
		//functions
		void renderBarricade();
		void renderCube();
		void renderSkybox();
};
#endif 
