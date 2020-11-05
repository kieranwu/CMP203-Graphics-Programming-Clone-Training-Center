// Scene class. Configures a basic 3D scene.
// Interfaces with the Input class to handle user input
// Calculates and outputs Frames Per Second (FPS) rendered.
// Important functions are the constructor (initialising the scene), 
// update (for process user input and updating scene objects) and render (renders scene).


//This Class was adapted and changed from the orginal provided by the module CMP203 Graphics Programming
#ifndef _SCENE_H
#define _SCENE_H

// Include GLUT, openGL, input.
#include "glut.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "Input.h"
#include <stdio.h>

// Further includes should go here:
#include "SOIL.h"
#include <vector>
#include "Camera.h"
#include "Shape.h"
#include "Model.h"
#include "Shadow.h"
#include "ShapeGenerator.h"

class Scene{

public:
	Scene(Input *in, Camera& cam);
	// Main render function
	void render();
	// Handle input function that receives delta time from parent.
	void handleInput(float dt);
	// Update function receives delta time from parent (used for frame independent updating).
	void update(float dt);
	// Resizes the OpenGL output based on new window size.
	void resize(int w, int h);

protected:
	// Renders text (x, y positions, RGB colour of text, string of text to be rendered)
	void displayText(float x, float y, float r, float g, float b, char* string);
	// A function to collate all text output in a single location
	void renderTextOutput();
	//A function rto calculate the frame rate
	void calculateFPS();

	// draw primitive functions
	
	// For access to user input.
	Input* input;
		
	// For Window and frustum calculation.
	int width, height;
	float fov, nearPlane, farPlane;

	// For FPS counter and mouse coordinate output.
	int frame = 0, time, timebase = 0;
	char fps[40];
	char mouseText[40];

	//initialise Variables and objects

	//Camera object
	Camera* cam;
	Camera camSpot;
	Camera camElevator;
	Vector3 rotationCam;
	int camNum;
	float spotHor;
	float spotVer;

	//Shadows 
	Shadow shadow;
	Shape shapeVA;
	float shadowMatrix[16];

	//Lights
	float lightDim;
	bool lightSwitch;

	//WireFrame
	bool WireFrame = false;

	//Imported Models
	Model turrets;
	Model lightStand;
	Model lightTop;

	//Textures
	GLuint tempTex;
	GLuint WallCTex;
	GLuint barrTex;
	GLuint skybox;

	//drawing objects
	ShapeGenerator shapeGen;

	//Other
	float elevatorY;
	float elevatorMove;
	float rotation;

	//moving terrain
	bool transition;
	float zPosition;
	float test;


	//Functions
	void cameraControl(float dt);
	void spotCam(float dt);
	void elevatorCam(float dt);
	void mouseMove(float dt);
	
	//Lighting
	void drawlight();


	//shadows
	void drawShadow();

	//Geometry
	void drawLevel();
	void drawObjects();
	void drawBarricade();
	void drawCrenellation(int num);
	void drawTower();
	void spotTower();
	void transparentObjects();

	//reflection
	void reflection();

	//Other 
	float findMag(float X,float Y,float Z);
};

#endif}