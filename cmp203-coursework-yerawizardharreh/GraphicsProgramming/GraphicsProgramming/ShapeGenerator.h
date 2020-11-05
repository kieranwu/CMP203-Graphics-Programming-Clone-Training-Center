//This functions manages the procedural generation of geometry
#pragma once

// INCLUDES //
#include <glut.h>
#include <fstream>
#include <gl/gl.h>
#include <gl/glu.h>

using namespace std;

// MY CLASS INCLUDES //
#include <vector>
#include "Vector3.h"
#include "SOIL.h"

//Defining PI which will be used for calculations later
#define PI 3.1415

class ShapeGenerator
{
public:

	//Constructor and Destructor
	ShapeGenerator();
	~ShapeGenerator();

	//functions
	void generateQuad(int width, int height);
	void generateCylinder(float total, float height, float radi);
	void generatePyramid(float width, float breadth, float height);

};