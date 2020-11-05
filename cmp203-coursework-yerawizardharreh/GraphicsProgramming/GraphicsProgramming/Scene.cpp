#include "Scene.h"

// Scene constructor, initilises OpenGL
// You should add further variables to need initilised.
Scene::Scene(Input *in, Camera &camera)
{
	// Store pointer for input class
	input = in;
	cam = &camera;

	//OpenGL settings
	glShadeModel(GL_SMOOTH);							// Enable Smooth Shading
	glClearColor(0,0,0, 1.0f);										// Black Background
	glClearDepth(1.0f);													// Depth Buffer Setup
	glClearStencil(0);													// Clear stencil buffer
	glEnable(GL_DEPTH_TEST);								// Enables Depth Testing
	glDepthFunc(GL_LEQUAL);								// The Type Of Depth Testing To Do
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	// Really Nice Perspective Calculations
	glLightModelf(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);

	// Other OpenGL / render setting should be applied here.

	//Hide cursor
	glutSetCursor(GLUT_CURSOR_NONE);
	
	// Initialise scene variables
	
	//Lighting
	glEnable(GL_LIGHTING);
	glEnable(GL_COLOR_MATERIAL);
	
	//Set up blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Enable textures
	glEnable(GL_TEXTURE_2D);


	//Camera set up

	//Free Camera
	rotationCam = (0, 0, 0);

	cam->setPosition(Vector3(0,0,-5));
	cam->setUp(Vector3(0, 0, 0));
	cam->setYaw(rotationCam.x);
	cam->setPitch(rotationCam.y);
	cam->setRoll(rotationCam.z);

	//Spotlight Camera
	spotHor = 90;
	spotVer = -45;
	
	camSpot.setPosition(Vector3(-9, 6.5, -12));
	camSpot.setUp(Vector3(0, 0, 0));
	camSpot.setYaw(spotHor);
	camSpot.setPitch(spotVer);
	camSpot.setRoll(rotationCam.z);

	//Elevator Camera
	elevatorY = -4;

	camElevator.setUp(Vector3(0, 0, 0));
	camElevator.setYaw(0);
	camElevator.setPitch(0);
	camElevator.setRoll(0);

	//Set Camera to start on
	camNum = 1;

	//Load textures
	tempTex = SOIL_load_OGL_texture("gfx/other/temp.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	WallCTex = SOIL_load_OGL_texture("gfx/other/WallC.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	barrTex = SOIL_load_OGL_texture("gfx/other/barricade.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);
	skybox = SOIL_load_OGL_texture("gfx/other/skybox.png", SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT);

	//import models to be rendered later
	lightStand.load("models/other/LightStand.obj","gfx/other/lightStand.png");
	turrets.load("models/other/Turret.obj", "gfx/other/turret.png");
	lightTop.load("models/other/lightTop.obj", "gfx/other/light.png");

	//Other variable initialisaton
	rotation = 0;
	zPosition = -2.1;
	lightDim = 0;
	lightSwitch = false;
	elevatorMove = false;
	transition = false;

	//update cameras for first frame
	cam->update();
	camSpot.update();
	camElevator.update();
}

//Function used to handle inputs
void Scene::handleInput(float dt)
{
	// Handle user input

	//Change camNum variable to key pressed this will change the camera being viewed from
	if (input->isKeyDown('1'))
	{
		camNum = 1;
	}
	if (input->isKeyDown('2'))
	{
		camNum = 2;
	}
	if (input->isKeyDown('3'))
	{
		camNum = 3;
	}

	//Transition controls the moving blocks in the scene 
	if (input->isKeyDown('l'))
	{
		transition = true;
	}
	if (input->isKeyDown('k'))
	{
		transition = false;
	}

	//wireframe
	//detect keypress of x key to turn on or off wireframe mode
	if (input->isKeyDown('x'))
	{
		if (!WireFrame)
		{
			glPolygonMode(GL_FRONT, GL_FILL);
			glPolygonMode(GL_BACK, GL_FILL);
			WireFrame = true;
		}
		else
		{
			glPolygonMode(GL_FRONT, GL_LINE);
			glPolygonMode(GL_BACK, GL_LINE);
			WireFrame = false;
		}
	}

	//switch statement used to run the correct controls functions based on camera 
	switch (camNum)
	{
	case 1: cameraControl(dt); break;
	case 2:spotCam(dt); break;
	case 3:elevatorCam(dt); break;
	}
}

//Update function used to update variables , runs every frame
void Scene::update(float dt)
{
	// update scene related variables.
	// Calculate FPS for output
	calculateFPS();

	//increment rotation by 5 multiplied by delta time
	rotation += 5 * dt;

	//check if transition is true and Zpostion is less than 0 , if so then increment zposition until it is equal to 0
	if (transition && zPosition < 0)
	{
		zPosition += 1 * dt;
		//set zposition to 0 if it is greater than 0
		if (zPosition > 0)
		{
			zPosition = 0;
		}
	}
	//check if transition is false and zposition is greater than -2.1 
	if (!(transition) && zPosition >-2.1)
	{
		zPosition -= 1 * dt;
		if (zPosition < -2.1)
		{
			zPosition = -2.1;
		}
	}

	//Checks the state of elevatorMove and the value of elevatorY and will increase elevatorY if true and less than 0
	if (elevatorMove && elevatorY < 0)
	{
		elevatorY += 1 * dt;
		if (elevatorY > 0)
		{
			elevatorY = 0;
		}
	}
	//otherwise if false and greater than -4 it will decrease to -4
	if (!(elevatorMove) && elevatorY > -4)
	{
		elevatorY -= 1 * dt;
		if (elevatorY < -4)
		{
			elevatorY = -4;
		}
	}

	//check state of lightSwitch  and increase lightDim if true and decrease if false
	//This will also change the state of light switch when lightDim hits 1 or 0;
	if (lightSwitch)
	{
		lightDim += 0.5 * dt;
		if (lightDim >= 1)
		{
			lightSwitch = false;
		}
	}
	else
	{
		lightDim -= 0.5 * dt;
		if (lightDim <= 0)
		{
			lightSwitch = true;
		}
	}
}

//This function will manage what OpenGL will render
void Scene::render() {

	// Clear Color and Depth Buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	// Reset transformations
	glLoadIdentity();

	// Set the openGL glulookat to the right camera properties
	switch (camNum)
	{
	case 1:gluLookAt(cam->getPosX(), cam->getPosY(), cam->getPosZ(), cam->getLookat().x, cam->getLookat().y, cam->getLookat().z, cam->getUpx(), cam->getUpy(), cam->getUpz()); break;
	case 2:gluLookAt(camSpot.getPosX(), camSpot.getPosY(), camSpot.getPosZ(), camSpot.getLookat().x, camSpot.getLookat().y, camSpot.getLookat().z, camSpot.getUpx(), camSpot.getUpy(), camSpot.getUpz()); break;
	case 3:gluLookAt(camElevator.getPosX(), camElevator.getPosY(), camElevator.getPosZ(), camElevator.getLookat().x, camElevator.getLookat().y, camElevator.getLookat().z, camElevator.getUpx(), camElevator.getUpy(), camElevator.getUpz()); break;
	}
	

	// Render geometry/scene here -------------------------------------
	
	//skybox
	//Bind texture to skybox
	glBindTexture(GL_TEXTURE_2D, skybox);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);


	glPushMatrix();		
		//disable depth test  - this will allow us to see other geometry through the skybox
		glDisable(GL_DEPTH_TEST);
		//move skybox to always bewhere the camera is
		glTranslatef(cam->getPosX(), cam->getPosY(), cam->getPosZ());
		//render the skybox using vertex array function
		shapeVA.renderSkybox();
		//enable depth testing
		glEnable(GL_DEPTH_TEST);
	glPopMatrix();

	//render lighting
	drawlight();	

	//render light and shadow 
	drawShadow();	

	//render center pyramid reflection
	reflection();

	//render the overall level
	drawLevel();

	//render objects throughout the level
	drawObjects();

	//render transparent ceiling
	transparentObjects();

	// End render geometry --------------------------------------

	// Render text, should be last object rendered.
	renderTextOutput();
	
	// Swap buffers, after all objects are rendered.
	glutSwapBuffers();
}

// Handles the resize of the window. If the window changes size the perspective matrix requires re-calculation to match new window size.
void Scene::resize(int w, int h) 
{
	width = w;
	height = h;
	// Prevent a divide by zero, when window is too short
	// (you cant make a window of zero width).
	if (h == 0)
		h = 1;

	float ratio = (float)w / (float)h;
	fov = 45.0f;
	nearPlane = 0.1f;
	farPlane = 100.0f;

	// Use the Projection Matrix
	glMatrixMode(GL_PROJECTION);

	// Reset Matrix
	glLoadIdentity();

	// Set the viewport to be the entire window
	glViewport(0, 0, w, h);

	// Set the correct perspective.
	gluPerspective(fov, ratio, nearPlane, farPlane);

	// Get Back to the Modelview
	glMatrixMode(GL_MODELVIEW);

	
}

// Calculates FPS
void Scene::calculateFPS()
{

	frame++;
	time = glutGet(GLUT_ELAPSED_TIME);

	if (time - timebase > 1000) {
		sprintf_s(fps, "FPS: %4.2f", frame*1000.0 / (time - timebase));
		timebase = time;
		frame = 0;
	}
}

//Function to manage controlling camera with mouse movements
void Scene::mouseMove(float dt)
{

	//The follow will check the mouse position and compare it with whether is greater or less than half the width or the height and update the rotation of the camera accordingly
	if (input->getMouseX() > width / 2)
	{
		cam->rotation(Vector3(1, 0, 0), dt);
		cam->update();
	}
	else if (input->getMouseX() < width / 2)
	{
		cam->rotation(Vector3(-1, 0, 0), dt);
		cam->update();
	}

	if (input->getMouseY() > height / 2)
	{
		cam->rotation(Vector3(0, -1, 0), dt);
		cam->update();
	}
	else if (input->getMouseY() < height / 2)
	{
		cam->rotation(Vector3(0, 1, 0), dt);
		cam->update();
	}

	//move mouse back to center
	glutWarpPointer(width / 2, height / 2);
}

//Function to quickly calculate the magnitude of a given vector
float Scene::findMag(float X, float Y, float Z)
{
	//square values
	float Xs = X * X;
	float Ys = Y * Y;
	float Zs = Z * Z;

	//total the square values of X Y and Z and return the square root of the total
	float mag = Xs + Ys + Zs;
	return sqrt(mag);
}

//Function to manage control of the Free camera
void Scene::cameraControl(float dt)
{
		//check W key press
		if (input->isKeyDown('w'))
		{
			//set X Y Z vector to lookat - position of camera
			float X = cam->getLookat().x - cam->getPosX();
			float Y = cam->getLookat().y - cam->getPosY();
			float Z = cam->getLookat().z - cam->getPosZ();

			//pass new vector to find the magnitude
			float ans = findMag(X, Y, Z);

			//move the camera in the unit vector using the new vector divided by its magnitude
			cam->move(Vector3(X / ans, Y / ans, Z / ans), dt);
		}
		//Check S key press
		if (input->isKeyDown('s'))
		{
			//Set X Y Z vector to lookat - position of camera
			float X = cam->getLookat().x - cam->getPosX();
			float Y = cam->getLookat().y - cam->getPosY();
			float Z = cam->getLookat().z - cam->getPosZ();

			//pass new vector to find the magnitude 
			float ans = findMag(X, Y, Z);

			//move the camera in the unit vector using the new vector divided by its magnitude
			cam->move(Vector3(-X / ans, -Y / ans, -Z / ans), dt);
		}
		//check A key press
		if (input->isKeyDown('a'))
		{
			//get new look at 
			float x, y, z;

			//temp values for sin/cos from
			float cosP, cosY;
			float  sinP, sinY;

			//Roll, pitch and yall are variables soted by the camera
			//handle rotation
			//only want to calculate these values once, when rotation changes, not every frame
			//Use current camera yaw + 90 and pitch (Right of current direction)

			cosY = cosf((cam->getYaw() + 90) * PI / 180);
			cosP = cosf(cam->getPitch() * PI / 180);
			sinY = sinf((cam->getYaw() + 90) * PI / 180);
			sinP = sinf(cam->getPitch() * PI / 180);


			//this using the parametric equation of a sphere

			//calculate forward vector
			float forwardx = sinY * cosP;
			float forwardy = sinP;
			float forwardz = cosP * -cosY;

			//calculate new lookat vector by adding foward vector to position
			x = cam->getPosX() + forwardx;
			y = cam->getPosY() + forwardy;
			z = cam->getPosZ() + forwardz;

			//Calculate new vector from subtracting camera position from local lookat vector above 
			float X = x - cam->getPosX();
			float Y = y - cam->getPosY();
			float Z = z - cam->getPosZ();

			//find magnitude of new vector
			float ans = findMag(X, Y, Z);

			//move camera in opposite direction of unit vector
			cam->move(Vector3(-X / ans, -Y / ans, -Z / ans), dt);

		}
		if (input->isKeyDown('d'))
		{
			//get new look at 
			float x, y, z;

			//temp values for sin/cos from
			float cosP, cosY;
			float  sinP, sinY;

			//Roll, pitch and yall are variables soted by the camera
			//handle rotation
			//only want to calculate these values once, when rotation changes, not every frame
			//Use current camera yaw + 90 and pitch (Right of current direction)

			cosY = cosf((cam->getYaw() + 90) * PI / 180);
			cosP = cosf(cam->getPitch() * PI / 180);
			sinY = sinf((cam->getYaw() + 90) * PI / 180);
			sinP = sinf(cam->getPitch() * PI / 180);


			//this using the parametric equation of a sphere

			//calculate forward vector
			float forwardx = sinY * cosP;
			float forwardy = sinP;
			float forwardz = cosP * -cosY;

			//calculate new lookat vector by adding foward vector to position
			x = cam->getPosX() + forwardx;
			y = cam->getPosY() + forwardy;
			z = cam->getPosZ() + forwardz;

			//Calculate new vector from subtracting camera position from local lookat vector above 
			float X = x - cam->getPosX();
			float Y = y - cam->getPosY();
			float Z = z - cam->getPosZ();

			//find magnitude of new vector
			float ans = findMag(X, Y, Z);

			//move camera in direction of unit vector
			cam->move(Vector3(X / ans, Y / ans, Z / ans), dt);
		}
		//Check Space key press
		if (input->isKeyDown(VK_SPACE))
		{
			//move camera up
			cam->move(Vector3(0, 1, 0), dt);
		}
		//Check Z key press
		if (input->isKeyDown('z'))
		{
			//move camera down
			cam->move(Vector3(0, -1, 0), dt);
		}
		//Check Q key press
		if (input->isKeyDown('q'))
		{
			//add to camera roll and update
			cam->rotation(Vector3(0, 0, 1), dt);
			cam->update();
		}
		//Check E key press
		if (input->isKeyDown('e'))
		{
			//minus  to camera roll and update
			cam->rotation(Vector3(0, 0, -1), dt);
			cam->update();
		}
		//Check R key press
		if (input->isKeyDown('r'))
		{
			//minus to camera Yaw and update
			cam->rotation(Vector3(-1, 0, 0), dt);
			cam->update();
		}
		//Check T key press
		if (input->isKeyDown('t'))
		{
			//add to camera Yaw and update
			cam->rotation(Vector3(1, 0, 0), dt);
			cam->update();
		}
		//Check C key press
		if (input->isKeyDown('c'))
		{
			//Add to camera Pitch and update
			cam->rotation(Vector3(0, 1, 0), dt);
			cam->update();
		}
		//Check V key press 
		if (input->isKeyDown('v'))
		{
			//minus to camera Pitch and update
			cam->rotation(Vector3(0, -1, 0), dt);
			cam->update();
		}

		//run mouse movement function
		mouseMove(dt);
}

//Function used to manage controls of the spotlight camera
void Scene::spotCam(float dt)
{
	//Check A key press
	if(input->isKeyDown('a'))
	{
		//Check if spotHor (horizontal value ) is greater than 45
		if (spotHor > 45)
		{
			//decrease spotHor by 10 multiplied by delta time
			spotHor -= 10 * dt;
			//set camera Yaw to spotHor and update
			camSpot.setYaw(spotHor);
			camSpot.update();
		}
	}
	//Check D key press
	if (input->isKeyDown('d'))
	{
		//Check if spotHor is less than 135 
		if (spotHor < 135)
		{
			//increase spotHor by 10 mulitplied by delta time
			spotHor += 10 * dt;
			//set camera Yaw to spotHor and update
			camSpot.setYaw(spotHor);
			camSpot.update();
		}
	}
	//Check W key press
	if (input->isKeyDown('w'))
	{
		//Check if spotVer is less than -20(vertical value)
		if (spotVer < -20)
		{
			//increase spotVer by 10 multiplied by delta time
			spotVer += 10 * dt;
			//set camera Pitch and update
			camSpot.setPitch(spotVer);
			camSpot.update();
		}
	}
	//Check S key press
	if (input->isKeyDown('s'))
	{
		//Check if spotVer is greater than or equal to -45
		if (spotVer >= -45)
		{
			//decrease spotVer by 10 multiplied by delta time
			spotVer -= 10 * dt;
			//set camera Pitch and update
			camSpot.setPitch(spotVer);
			camSpot.update();
		}
	}
}

//Function to manage elevator camera controls
void Scene::elevatorCam(float dt)
{
	//update position of the camera to follow elevator
	camElevator.setPosition(Vector3(4, elevatorY+0.5, 2));

	//Check space key press
	if (input->isKeyDown(VK_SPACE))
	{
		//check if elevatormove is false
		if (!elevatorMove)
		{
			//set elevator move to true
			elevatorMove = true;
		}
	}
	//Check Z key press
	if (input->isKeyDown('z'))
	{
		//check if elevatormove is true
		if (elevatorMove)
		{
			//set elevatormove to false
			elevatorMove = false;
		}
	}
}

//Function used to manage shadow at the front of the scene
void Scene::drawShadow()
{
	//bind texture to nothing
	glBindTexture(GL_TEXTURE_2D, NULL);

	//set up spotlight variables
	GLfloat Light_Diffuse[] = { 0,0.75,0,1 };
	GLfloat Light_Position[] = { 4, 3,2 , 1 };
	GLfloat Light_Ambient[] = { 0,0.4,0,1 };
	GLfloat Spot_Direction[] = { 0,-1,-0.7 };

	//give light5 properties using just defined
	glLightfv(GL_LIGHT5, GL_DIFFUSE, Light_Diffuse);
	glLightfv(GL_LIGHT5, GL_AMBIENT, Light_Ambient);
	glLightfv(GL_LIGHT5, GL_POSITION, Light_Position);
	glLightf(GL_LIGHT5, GL_SPOT_CUTOFF, 40.0f);
	glLightfv(GL_LIGHT5, GL_SPOT_DIRECTION, Spot_Direction);
	glLightf(GL_LIGHT5, GL_SPOT_EXPONENT, 2);
	glLightf(GL_LIGHT5, GL_CONSTANT_ATTENUATION, 0.5);
	glLightf(GL_LIGHT5, GL_LINEAR_ATTENUATION, 0.1);
	glLightf(GL_LIGHT5, GL_QUADRATIC_ATTENUATION, 0.01);

	//enable light
	glEnable(GL_LIGHT5);

	//define floor for shadow to be projected on
	GLfloat floorVerts[] =
	{
		0,-0.55,0,
		10,-0.55,0,
		10,-0.55,-10,
		0,-0.55,-10,
	};

	//generate shadow matrix using function
	shadow.generateShadowMatrix(shadowMatrix, Light_Position, floorVerts);

	//bind tempTex to geometry
	glBindTexture(GL_TEXTURE_2D, tempTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	
	//main entrance
	glPushMatrix();	
		//scale to half size
		glScalef(0.5, 0.5, 0.5);
		//translate quad to entrance of scene
		glTranslatef(0, -1.149, -26);
		//generate Quad with width 15 height 26
		shapeGen.generateQuad(15, 26);
	glPopMatrix();

	//disable depth test lighting and 2d texture
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);

	//set color to dark grey - black
	glColor3f(0.1, 0.1, 0.1);

	glPushMatrix();
		//apply shadow matrix transformation
		glMultMatrixf((GLfloat*)shadowMatrix);

		//translate rotate and scale the same as object
		glTranslatef(5,-0.55,-6);
		glRotatef(270, 0, 1, 0);
		glScalef(1, 1, 2);

		//render barricade object using vertex array
		shapeVA.renderBarricade();
	glPopMatrix();

	//reset colour
	glColor3f(1, 1, 1);

	//re-enable depth test lights and 2d textures
 	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);

	//set colour to green
	glColor3f(0, 1, 0);

	glPushMatrix();
		//translate and scale 	
		glTranslatef(4, 2.7, 2.2);
		glScalef(0.2, 0.2, 0.2);
		//render Cylinder using geometry generator 
		shapeGen.generateCylinder(10,1,1);
	glPopMatrix();

	//bind texture to barrTex
	glBindTexture(GL_TEXTURE_2D, barrTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);


	glPushMatrix();
		
		//set colour
		glColor3f(0.5, 0.5, 0.5);
		//translate rotate and scale the same as the shadow
		glTranslatef(5, -0.55, -6);
		glRotatef(270, 0, 1, 0);
		glScalef(1, 1, 2);
		//render barricade using vertex array
		shapeVA.renderBarricade();
	glPopMatrix();

	//reset colour
	glColor3f(1, 1, 1);
}

//Function used to manage lights
void Scene::drawlight()
{

	//create light properties
	GLfloat Light_Diffuse[] = { lightDim,0,0,06 };
	GLfloat Light_Position[] = { 3.75, 20, -46.75, 1 };
	GLfloat Light_AmbientR[] = { lightDim,0,0,0.6 };
	GLfloat Spot_Direction[] = { 0,-1,0 };

	GLfloat Light_Ambient[] = { 0.3,0.3,0.3,1 };
	GLfloat Light_Diffuse1[] = { 0.2,0.2,0.2,1 };

	//define Castle light stand spotlight as light0
	glLightfv(GL_LIGHT0, GL_DIFFUSE, Light_Diffuse);
	glLightfv(GL_LIGHT0, GL_AMBIENT, Light_AmbientR);
	glLightfv(GL_LIGHT0, GL_POSITION, Light_Position);
	glLightf(GL_LIGHT0, GL_SPOT_CUTOFF, 2.0f);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, Spot_Direction);
	glLightf(GL_LIGHT0, GL_SPOT_EXPONENT, 2);
	glLightf(GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.5);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.1);
	glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.01);

	//enable light
	glEnable(GL_LIGHT0);

	//define point light as light 1 (general ambient lighting)
	glLightfv(GL_LIGHT1, GL_DIFFUSE, Light_Diffuse1);
	glLightfv(GL_LIGHT1, GL_AMBIENT, Light_Ambient);

	//enable light
	glEnable(GL_LIGHT1);


	//SPOTLIGHT
	//initialise X Y Z 
	float X, Y, Z;

	//set X Y Z to spotlight camera lookat minus spotlight camera position
	X = camSpot.getLookat().x - camSpot.getPosX();
	Y = camSpot.getLookat().y - camSpot.getPosY();
	Z = camSpot.getLookat().z - camSpot.getPosZ();

	//find magnitude of vector
	float mag = findMag(X, Y, Z);

	//define properties of spotlight (spotlight should point direction of camera)
	GLfloat Light_Diffuse_SPOT[] = { 1,1,1,1 };
	GLfloat Light_Position_SPOT[] = { camSpot.getPosX(),camSpot.getPosY(),camSpot.getPosZ(), 1 };
	GLfloat Light_Ambient_SPOT[] = { 0.3,0.3,0.3,0.7 };
	GLfloat Spot_Direction_SPOT[] = { X/mag,Y/mag ,Z/mag };

	//define spotlight using properties defined above
	glLightfv(GL_LIGHT2, GL_DIFFUSE, Light_Diffuse_SPOT);
	glLightfv(GL_LIGHT2, GL_AMBIENT, Light_Ambient_SPOT);
	glLightfv(GL_LIGHT2, GL_POSITION, Light_Position_SPOT);
	glLightf(GL_LIGHT2, GL_SPOT_CUTOFF, 12.0f);
	glLightfv(GL_LIGHT2, GL_SPOT_DIRECTION, Spot_Direction_SPOT);
	glLightf(GL_LIGHT2, GL_SPOT_EXPONENT, 6);
	glLightf(GL_LIGHT2, GL_CONSTANT_ATTENUATION, 0.5);
	glLightf(GL_LIGHT2, GL_LINEAR_ATTENUATION, 0.1);
	glLightf(GL_LIGHT2, GL_QUADRATIC_ATTENUATION, 0.01);

	//enable light
	glEnable(GL_LIGHT2);

}

//Function used to manage the geometry of the general scene (walls and floor)
void Scene::drawLevel()
{
	
	//Bind geomerty to temptex

	glBindTexture(GL_TEXTURE_2D, tempTex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	//FLOORS

	glPushMatrix();
		//scale geometry to half the size
		glScalef(0.5, 0.5, 0.5);

		//main entrance
		glPushMatrix();
			glTranslatef(0, -1.15, -26);
			shapeGen.generateQuad(15,26);
		glPopMatrix();

		//middle lower area

		//First Wide part
		glPushMatrix();
			glTranslatef(0, -4,-66);
			shapeGen.generateQuad(15, 40);
		glPopMatrix();

		//middle end part
		glPushMatrix();
			glTranslatef(-20, -2, -101);
			shapeGen.generateQuad(55, 35);
		glPopMatrix();

		//right side
		glPushMatrix();
			glTranslatef(15, -2, -66);
			shapeGen.generateQuad(20, 66);
		glPopMatrix();

		//left side
		glPushMatrix();
			glTranslatef(-20, -2, -66);
			shapeGen.generateQuad(20, 66);
		glPopMatrix();


		//CASTLE(Part at the end of the scene)

		glPushMatrix();
			//middle platform
			glPushMatrix();
				glTranslatef(-10, 2, -95);
				shapeGen.generateQuad(35, 9);
			glPopMatrix();

			//left platform
			glPushMatrix();
				glTranslatef(-20, 6, -101);
				shapeGen.generateQuad(16, 11);
			glPopMatrix();

			//right platform
			glPushMatrix();
				glTranslatef(19, 6, -101);
				shapeGen.generateQuad(16, 11);
			glPopMatrix();
	
			//Towers part	

			//back platform
			glPushMatrix();
				glTranslatef(-10, 10, -101);
				shapeGen.generateQuad(35, 6);
			glPopMatrix();

			//middle platform
			glPushMatrix();
				glTranslatef(3, 16, -101);
				shapeGen.generateQuad(9, 8);
			glPopMatrix();

			//front platform
			glPushMatrix();
				glTranslatef(5, 19, -95);
				shapeGen.generateQuad(5, 3);
			glPopMatrix();

		glPopMatrix();

	//RAMPS 

	//ENTRANCE PLATFORM

	//middle ramp entrance
		glPushMatrix();
			glTranslatef(0, -4, -32.4);
			glRotatef(336, 1, 0, 0);
			shapeGen.generateQuad(15, 7);
		glPopMatrix();

		//left ramp entrance
		glPushMatrix();
			glTranslatef(-1.8, -2, -26);
			glRotatef(25, 0, 0, 1);
			shapeGen.generateQuad(2, 26);
		glPopMatrix();

		//right ramp entrance
		glPushMatrix();
			glTranslatef(14.9, -1.2, -26);
			glRotatef(336, 0, 0, 1);
			shapeGen.generateQuad(2, 26);
		glPopMatrix();

		//LOWER AREA

		//middle ramp end
		glPushMatrix();
			glTranslatef(0, -2, -66);
			glRotatef(45, 1, 0, 0);
			shapeGen.generateQuad(15,3);
		glPopMatrix();


		//middle ramp left
		glPushMatrix();
			glTranslatef(0, -2, -66);
			glRotatef(330, 0, 0, 1);
			shapeGen.generateQuad(4, 40);
		glPopMatrix();

		//middle ramp right
		glPushMatrix();
			glTranslatef(11.6, -4, -66);
			glRotatef(30, 0, 0, 1);
			shapeGen.generateQuad(4, 40);
		glPopMatrix();

	glPopMatrix();

	//Walls
	glPushMatrix();

		//Scale and translate
		glScalef(0.5, 0.5, 0.5);
		glTranslatef(0, -2, 0);

		//elevator Wall Center back
		glPushMatrix();
			glTranslatef(5, -9, 5);
			glRotatef(270, 1, 0, 0);
			shapeGen.generateQuad(5, 41);
		glPopMatrix();

		//elevator Wall center front
		glPushMatrix();
			glTranslatef(5, -9, 0);
			glRotatef(270, 1, 0, 0);
			shapeGen.generateQuad(5, 10);
		glPopMatrix();

		//elevator Wall left
		glPushMatrix();
			glTranslatef(5, -9, 0);
			glRotatef(90, 0, 0, 1);
			shapeGen.generateQuad(41, 5);
		glPopMatrix();

		//elevator Wall right
		glPushMatrix();
			glTranslatef(10, -9, 0);
			glRotatef(90, 0, 0, 1);
			shapeGen.generateQuad(41, 5);
		glPopMatrix();


		//Front wall Left
		glPushMatrix();
			glTranslatef(-20, 0, 0);
			glRotatef(270, 1, 0, 0);
			shapeGen.generateQuad(25, 32);
		glPopMatrix();

		//Front Wall Right
		glPushMatrix();
			glTranslatef(10, 0, 0);
			glRotatef(270, 1, 0, 0);
			shapeGen.generateQuad(25, 32);
		glPopMatrix();
		
		//Left Wall
		glPushMatrix();
			glTranslatef(-20,0, -101);
		     glRotatef(90, 0, 0, 1);
			shapeGen.generateQuad(32,101);
		glPopMatrix();

		//Right Wall
		glPushMatrix();
			glTranslatef(35, 0, -101);
			glRotatef(90, 0, 0, 1);
			shapeGen.generateQuad(32, 101);
		glPopMatrix();

		//Back Wall
		glPushMatrix();
			glTranslatef(-20, 0, -101);
			glRotatef(270, 1, 0, 0);
			shapeGen.generateQuad(55, 32);
		glPopMatrix();

		//Castle

		//Front Section

		//bind texture to Wall texture
		glBindTexture(GL_TEXTURE_2D, WallCTex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		//Front Wall
		glPushMatrix();
			glTranslatef(-10, 0, -86);
			glRotatef(270, 1, 0, 0);
			shapeGen.generateQuad(35, 4);
		glPopMatrix();

		//Front Wall Left Inner
		glPushMatrix();
			glTranslatef(-10, 0, -90);
			glRotatef(90, 0, 0, 1);
			shapeGen.generateQuad(4, 4);
		glPopMatrix();

		//Front Wall right Inner
		glPushMatrix();
			glTranslatef(25, 0, -90);
			glRotatef(90, 0, 0, 1);
			shapeGen.generateQuad(4, 4);
		glPopMatrix();

		//Left Wall Front
		glPushMatrix();
			glTranslatef(-20, 0, -90);
			glRotatef(270, 1, 0, 0);
			shapeGen.generateQuad(16, 8);
		glPopMatrix();

		//Left Wall Inner
		glPushMatrix();
			glTranslatef(-4, 0, -95);
			glRotatef(90, 0, 0, 1);
			shapeGen.generateQuad(8, 5);
		glPopMatrix();

		//Right Wall Front
		glPushMatrix();
			glTranslatef(19, 0, -90);
			glRotatef(270, 1, 0, 0);
			shapeGen.generateQuad(16, 8);
		glPopMatrix();

		//Right Wall Inner
		glPushMatrix();
			glTranslatef(19, 0, -95);
			glRotatef(90, 0, 0, 1);
			shapeGen.generateQuad(8, 5);
		glPopMatrix();

		//Inner Back Front
		glPushMatrix();
			glTranslatef(-10, 0, -95);
			glRotatef(270, 1, 0, 0);
			shapeGen.generateQuad(35, 12);
		glPopMatrix();

		//Inner Wall Left 
		glPushMatrix();
			glTranslatef(-10, 0, -101);
			glRotatef(90, 0, 0, 1);
			shapeGen.generateQuad(12, 6);
		glPopMatrix();

		//Inner Wall Right 
		glPushMatrix();
			glTranslatef(25, 0, -101);
			glRotatef(90, 0, 0, 1);
			shapeGen.generateQuad(12, 6);
		glPopMatrix();

		//Inner Tower Section

		//Inner Front face
		glPushMatrix();
			glTranslatef(3, 0, -93);
			glRotatef(270, 1, 0, 0);
			shapeGen.generateQuad(9, 18);
		glPopMatrix();

		//Inner Back
		glPushMatrix();
			glTranslatef(5, 17, -95);
			glRotatef(270, 1, 0, 0);
			shapeGen.generateQuad(5, 4);
		glPopMatrix();

		//Inner Left 
		glPushMatrix();
			glTranslatef(3, 0, -95);
			glRotatef(90, 0, 0, 1);
			shapeGen.generateQuad(18, 2);
		glPopMatrix();

	//Inner Right 
		glPushMatrix();
			glTranslatef(12, 0, -95);
			glRotatef(90, 0, 0, 1);
			shapeGen.generateQuad(18, 2);
		glPopMatrix();

		//Left wall 
		glPushMatrix();
			glTranslatef(3, 12, -101);
			glRotatef(90, 0, 0, 1);
			shapeGen.generateQuad(6, 6);
		glPopMatrix();

		//Right wall
		glPushMatrix();
			glTranslatef(12, 12, -101);
			glRotatef(90, 0, 0, 1);
			shapeGen.generateQuad(6, 6);
		glPopMatrix();

		//Outer Front face
		glPushMatrix();
			glTranslatef(5, 0, -92);
			glRotatef(270, 1, 0, 0);
			shapeGen.generateQuad(5, 21);
		glPopMatrix();

		//Outer Front Left
		glPushMatrix();
			glTranslatef(5, 0, -93);
			glRotatef(90, 0, 0, 1);
			shapeGen.generateQuad(21, 1);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(5, 11, -95);
			glRotatef(90, 0, 0, 1);
			shapeGen.generateQuad(10, 2);
		glPopMatrix();

		//Outer Front Right
		glPushMatrix();
			glTranslatef(10, 0, -93);
			glRotatef(90, 0, 0, 1);
			shapeGen.generateQuad(21, 1);
		glPopMatrix();

		glPushMatrix();
			glTranslatef(10, 11, -95);
			glRotatef(90, 0, 0, 1);
			shapeGen.generateQuad(10, 2);
		glPopMatrix();

	glPopMatrix();

}

//Function used to manage the objects to be rendered
void Scene::drawObjects()
{

	glPushMatrix();

		//bind geometry to barricade texture 
		glBindTexture(GL_TEXTURE_2D, barrTex);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

		//Barricades 	

		//render left side of barricades
		glPushMatrix();
			drawBarricade();
		glPopMatrix();

		//draw right side
		glPushMatrix();
			//flip in X axis
			glScalef(-1, 1, 1);
			//translate 8 unites to right (-8 as geometry was flipped)
			glTranslatef(-8, 0, 0);
			//render Barricades function
			drawBarricade();
		glPopMatrix();

		//middle barricades

		//front
		glPushMatrix();
			glTranslatef(4.8, -2, -20);
			glRotatef(270, 0, 1, 0);
			glScalef(1, 1, 2);
			shapeVA.renderBarricade();
		glPopMatrix();

		//middle
		glPushMatrix();
			glTranslatef(4.8, -2, -25);
			glRotatef(270, 0, 1, 0);
			glScalef(1, 1, 2);
			shapeVA.renderBarricade();
		glPopMatrix();

		//end
		glPushMatrix();
			glTranslatef(4.8, -1, -34);
			glRotatef(270, 0, 1, 0);
			glScalef(1, 1, 2);
			shapeVA.renderBarricade();
		glPopMatrix();

		//Crenellations

		//bottom middle
		glPushMatrix();
			glTranslatef(-5, -1, -42);
			glRotatef(90, 0, 1, 0);
			glScalef(1, 1.5, 0.5);
			drawCrenellation(36);
		glPopMatrix();

		//top middle left side
		glPushMatrix();
			glTranslatef(-2, 1, -43);
			glRotatef(90, 0, 1, 0);
			glScalef(0.5, 1, 0.5);
			drawCrenellation(10);
		glPopMatrix();

		//top middle right side
		glPushMatrix();
			glTranslatef(5, 1, -43);
			glRotatef(90, 0, 1, 0);
			glScalef(0.5, 1, 0.5);
			drawCrenellation(10);
		glPopMatrix();

		//Left Section 
		glPushMatrix();
			glTranslatef(-9.5, 3, -45);
			glRotatef(90, 0, 1, 0);
			glScalef(0.5, 1, 0.5);
			drawCrenellation(15);
		glPopMatrix();

		//right Section 
		glPushMatrix();
			glTranslatef(9.5, 3, -45);
			glRotatef(90, 0, 1, 0);
			glScalef(0.5, 1, 0.5);
			drawCrenellation(15);
		glPopMatrix();

		//upper left 
		glPushMatrix();
			glTranslatef(-5, 5, -47.5);
			glRotatef(90, 0, 1, 0);
			glScalef(0.5, 1, 0.5);
			drawCrenellation(13);
		glPopMatrix();

		//upper right
		glPushMatrix();
			glTranslatef(6, 5, -47.5);
			glRotatef(90, 0, 1, 0);
			glScalef(0.5, 1, 0.5);
			drawCrenellation(13);
		glPopMatrix();


		//Towers
		glPushMatrix();
			
			//render spotlight tower 	
			spotTower();
				
			//right side front
			glPushMatrix();
				glTranslatef(7.5, 0, 0);
				glScalef(-1, 1, 1);
				drawTower();
			glPopMatrix();

			//right side back
			glPushMatrix();
				glTranslatef(7.5, 0, -22);
				glScalef(-1, 1, 1);
				drawTower();
			glPopMatrix();

			//leftside back
			glPushMatrix();
				glTranslatef(0, 0, -22);
				drawTower();
			glPopMatrix();

		glPopMatrix();

		//floor cube left 
		glPushMatrix();
			glScalef(2, 1, 0.25);
			glTranslatef(-2.25, zPosition, -50);
			shapeVA.renderCube();
		glPopMatrix();


		//floor cube right
		glPushMatrix();
			glScalef(2, 1, 0.25);
			glTranslatef(6, zPosition, -50);
			shapeVA.renderCube();
		glPopMatrix();

		//floor cube middle first
		glPushMatrix();
			glScalef(1, 1, 0.25);
			glTranslatef(0, -2, -20);
			glTranslatef(3.75, zPosition, -50);
			shapeVA.renderCube();
		glPopMatrix();

		//floor cube middle end
		glPushMatrix();
		glScalef(1, 1, 0.25);
			glTranslatef(0, -2, -60);
			glTranslatef(3.75, zPosition, -50);
			shapeVA.renderCube();
		glPopMatrix();

		//Elevator
		glPushMatrix();
			glScalef(1.3, 1, 1.3);
			glTranslatef(2.9, elevatorY - 1.5, 0.95);
			shapeVA.renderCube();
		glPopMatrix();


		//Flag/LightStand

		glPushMatrix();
			glTranslatef(3.75, 9.55, -46.75);
			glScalef(0.05, 0.05, 0.05);
			lightStand.render();
		glPopMatrix();

		//light top
		glPushMatrix();
			glTranslatef(3.75, 9.55, -46.75);
			glScalef(0.05, 0.05, 0.05);
			lightTop.render();
		glPopMatrix();

		//turrets
		glPushMatrix();
			glTranslatef(3.75, 8, -45.9);
			glRotatef(90, 0, 1, 0);
			glScalef(0.1, 0.1, 0.1);

			//middle each turret translated 10 down from the previous
			glPushMatrix();
				turrets.render();
			glPopMatrix();

			glPushMatrix();
				glTranslatef(0, -10, 0);
				turrets.render();
			glPopMatrix();

			glPushMatrix();
				glTranslatef(0, -20, 0);
				turrets.render();
			glPopMatrix();

			glPushMatrix();
				glTranslatef(0, -30, 0);
				turrets.render();
			glPopMatrix();

			//left each turret translated 10 down from the previous
			glPushMatrix();
				glTranslatef(5.5, -25, -17.5);

				glPushMatrix();
					turrets.render();
				glPopMatrix();

				glPushMatrix();
					glTranslatef(0, -10, 0);
					turrets.render();
				glPopMatrix();

				glPushMatrix();
					glTranslatef(0, -20, 0);
					turrets.render();
				glPopMatrix();

				glPushMatrix();
					glTranslatef(0, -30, 0);
					turrets.render();
				glPopMatrix();
				
			glPopMatrix();

			//Right each turret translated 10 down from the previous

			glPushMatrix();
				glTranslatef(5.5, -25, 17.5);

				glPushMatrix();
					turrets.render();
				glPopMatrix();

				glPushMatrix();
					glTranslatef(0, -10, 0);
					turrets.render();
				glPopMatrix();

				glPushMatrix();
					glTranslatef(0, -20, 0);
					turrets.render();
				glPopMatrix();

				glPushMatrix();
					glTranslatef(0, -30, 0);
					turrets.render();
					glPopMatrix();
			glPopMatrix();
		glPopMatrix();
		glPopMatrix();
}

//Function used to manage the barricades to be rendered
void Scene::drawBarricade()
{

	//entrance
	glPushMatrix();
		glTranslatef(2, -0.6, -12.3);
		glRotatef(310, 0, 1, 0);
		glScalef(1, 1, 2);
		shapeVA.renderBarricade();
	glPopMatrix();


	//The Following renders a barricade object using vertex arrays translated to the correct position
	//rotated to face the correct direction and scaled 
	glPushMatrix();
		glTranslatef(-3, -1, -8);
		glRotatef(270, 0, 1, 0);
		glScalef(1, 1, 2);
		shapeVA.renderBarricade();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-6, -1, -4);
		glRotatef(270, 0, 1, 0);
		glScalef(1, 1, 2);
		shapeVA.renderBarricade();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-6, -1, -18);
		glRotatef(270, 0, 1, 0);
		glScalef(1, 1, 2);
		shapeVA.renderBarricade();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-3, -1, -22);
		glRotatef(270, 0, 1, 0);
		glScalef(1, 1, 2);
		shapeVA.renderBarricade();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-6, -1, -24);
		glRotatef(270, 0, 1, 0);
		glScalef(1, 1, 2);
		shapeVA.renderBarricade();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-3, -1, -26);
		glRotatef(270, 0, 1, 0);
		glScalef(1, 1, 2);
		shapeVA.renderBarricade();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(-1, -1, -30);
		glRotatef(270, 0, 1, 0);
		glScalef(1, 1, 2);
		shapeVA.renderBarricade();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(0, -1, -36);
		glRotatef(310, 0, 1, 0);
		glScalef(1, 1, 2);
		shapeVA.renderBarricade();
	glPopMatrix();

	glPushMatrix();
		glTranslatef(2, -1, -40);
		glRotatef(270, 0, 1, 0);
		glScalef(1, 1, 2);
		shapeVA.renderBarricade();
	glPopMatrix();
}

//Function used to manage how many crenellations to render
void Scene::drawCrenellation(int num)
{
	glPushMatrix();
		//loop for the value of num which is passed in increment by two
		for (int i = 0; i < num; i += 2)
		{
			glPushMatrix();
				//translate in z axis by i
				glTranslatef(0, 0,i);
				//render barricade using vertex array as it is the same shape
				shapeVA.renderBarricade();
			glPopMatrix();
		}
	glPopMatrix();
}

//Function used to handle rendering the geometry to make towers
void Scene::drawTower()
{
	glPushMatrix();

	//scale geometry to half the size
	glScalef(0.5, 0.5, 0.5);

	//tower floor
		glPushMatrix();
			glTranslatef(32, 8.6, -26);
			shapeGen.generateQuad(3, 4);
		glPopMatrix();

		//walls center
		glPushMatrix();
			glTranslatef(32, 8.6, -26);
			glRotatef(90, 0, 0, 1);
			shapeGen.generateQuad(1,4);
		glPopMatrix();

		//right
		glPushMatrix();
			glTranslatef(32, 8.6, -26);
			glRotatef(90, 0, 0, 1);
			glRotatef(90, 1, 0, 0);
			shapeGen.generateQuad(1, 3);
		glPopMatrix();

		//left
		glPushMatrix();
			glTranslatef(32, 8.6, -22);
			glRotatef(90, 0, 0, 1);
			glRotatef(90, 1, 0, 0);
			shapeGen.generateQuad(1, 3);
		glPopMatrix();

	glPopMatrix();

	//top pyramid
	glPushMatrix();
		glTranslatef(16.7, 5.5, -12);
		glRotatef(270, 1, 0, 0);
		shapeGen.generatePyramid(1.5,2,1);
	glPopMatrix();

	//tower base
	glPushMatrix();
		glTranslatef(17, -1, -12);
		glRotatef(270, 1, 0, 0);
		glRotatef(270, 0, 0, 1);
		glScalef(0.75,0.75,0.75);
		shapeGen.generateCylinder(40, 7,1);
	glPopMatrix();
}

//Function used to handle rendering the geometry to make spotlight tower
void Scene::spotTower()
{
	glPushMatrix();

		//scale geometry to half size
		glScalef(0.5, 0.5, 0.5);

		//tower floor
		glPushMatrix();
			glTranslatef(32, 8.6, -26);
			shapeGen.generateQuad(3, 4);
		glPopMatrix();

		//walls
		glPushMatrix();
			glTranslatef(32, 8.6, -26);
			glRotatef(90, 0, 0, 1);
			shapeGen.generateQuad(1, 4);
		glPopMatrix();

		//right
		glPushMatrix();
			glTranslatef(32, 8.6, -26);
			glRotatef(90, 0, 0, 1);
			glRotatef(90, 1, 0, 0);
			shapeGen.generateQuad(1, 3);
		glPopMatrix();

		//left
		glPushMatrix();
			glTranslatef(32, 8.6, -22);
			glRotatef(90, 0, 0, 1);
			glRotatef(90, 1, 0, 0);
			shapeGen.generateQuad(1, 3);
		glPopMatrix();
	glPopMatrix();

	//top pyramid
	glPushMatrix();
		glTranslatef(16.7, 5.5, -12);
		glRotatef(270, 1, 0, 0);
		shapeGen.generatePyramid(1.5, 2, 1);
	glPopMatrix();

	//light
	glPushMatrix();
		glTranslatef(-8.5, 5.5, -12);
		//rotate cylinder to face direction of camera
		glRotatef(-spotHor+180, 0, 1, 0);
		glRotatef(-spotVer+25, 1, 0, 0);
		glScalef(0.1, 0.1, 0.1);
		shapeGen.generateCylinder(10, 2, 1);
	glPopMatrix();

	//tower base
	glPushMatrix();
		glTranslatef(17, -1, -12);
		glRotatef(270, 1, 0, 0);
		glRotatef(270, 0, 0, 1);
		glScalef(0.75, 0.75, 0.75);
		shapeGen.generateCylinder(40, 7, 1);
	glPopMatrix();
}

//Functiom to handle rendering of transparent object
void Scene::transparentObjects()
{
	//ceiling
	glPushMatrix();
		//enable blending
		glEnable(GL_BLEND);
		//set color to transparent blue
		glColor4f(0, 0, 1, 0.2);
		//draw quad at vertexes
		glBegin(GL_QUADS);
			glVertex3f(-10, 15, -50.5);
			glVertex3f(-10, 15, 0);
			glVertex3f(17.5, 15, 0);
			glVertex3f(17.5, 15, -50.5);
		glEnd();	
		//disable blend
		glDisable(GL_BLEND);
	glPopMatrix();
	
	//reset color
	glColor3f(1, 1, 1);
}

//Function to handle reflections
void Scene::reflection()
{
	glPushMatrix();

	//unbind textures
	glBindTexture(GL_TEXTURE_2D,NULL);

	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
	//turn off writing frame buffer

	glEnable(GL_STENCIL_TEST);
	//enable stencil test

	glStencilFunc(GL_ALWAYS, 1, 1);
	//set stencil to pass

	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
	//set stencil operation

	glDisable(GL_DEPTH_TEST);
	//disable depth tyest

	//draw window
	glTranslatef(2.8, -1.99, -22);
	shapeGen.generateQuad(2, 2);

	glEnable(GL_DEPTH_TEST);
	//enable depth test

	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
	//turn on rendering to frame buffer

	glStencilFunc(GL_EQUAL, 1, 1);
	//set stencil funtion to test if value is 1

	glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
	//set stencil operation

	glDisable(GL_DEPTH_TEST);
	//disable depth test

		//draw reflected object
		glPushMatrix();
			//scale in opposite Y axis
			glScalef(1, -1, 1);

			//apply same rotation and translation as object
			glRotatef(270, 1, 0, 0);
			glTranslatef(1, -1, 1);
			glRotatef(rotation, 0, 0, 1);

			//render pyramid using shape generator
			shapeGen.generatePyramid(1,1,1);
		glPopMatrix();

	glEnable(GL_DEPTH_TEST);
	//enable depth testing

	glDisable(GL_STENCIL_TEST);
	//disable stencil test

	glEnable(GL_BLEND);
	//enable alpha blending

	glDisable(GL_LIGHTING);
	//disable lighting

	glColor4f(0.8, 0.8, 1, 0.8);
	//set colour

	shapeGen.generateQuad(2, 2);
	//render 2x2 quad

	glEnable(GL_LIGHTING);
	//enable lights

	glDisable(GL_BLEND);
	//disable blend

	//render spinning pyramid
		glPushMatrix();
			glRotatef(270, 1, 0, 0);
			glTranslatef(1, -1, 1);
			glRotatef(rotation, 0, 0, 1);
			shapeGen.generatePyramid(1, 1, 1);
		glPopMatrix();

	glPopMatrix();
}

// Compiles standard output text including FPS and current mouse position.
void Scene::renderTextOutput()
{
	char camnumber[40];
	// Render current mouse position and frames per second.
	sprintf_s(mouseText, "Mouse: %i, %i", input->getMouseX(), input->getMouseY());
	sprintf_s(camnumber, "Cam Number: %i", camNum);
	displayText(-1.f, 0.96f, 1.f, 0.f, 0.f, mouseText);
	displayText(-1.f, 0.90f, 1.f, 0.f, 0.f, fps);
	displayText(-1.f, 0.84, 0, 0, 1.f, "Free Cam: 1 \n SpotLight Cam: 2 \n Elevator Cam: 3");
	displayText(-1.f, 0.78, 0, 0, 1.f, camnumber);
	displayText(-1.f, 0.72, 0, 0, 1.f, "Controls - ");
	displayText(-1.f, 0.66, 0, 0, 1.f, "K , L - Raise and lower walls");
	switch (camNum)
	{
	case 1: displayText(-1.f, 0.60, 0, 0, 1.f, "WASD - Move"); displayText(-1.f, 0.54, 0, 0, 1.f, "Space - Go Up"); displayText(-1.f, 0.48, 0, 0, 1.f, "Z - Go down"); displayText(-1.f, 0.42, 0, 0, 1.f, "Q E  - Roll "); displayText(-1.f, 0.36, 0, 0, 1.f, "R T - Turn"); displayText(-1.f, 0.30, 0, 0, 1.f, "C V - Tilt "); break;
	case 2:displayText(-1.f, 0.60, 0, 0, 1.f, "WASD - Move Spotlight"); break;
	case 3:displayText(-1.f, 0.60, 0, 0, 1.f, "Space - Go Up"); displayText(-1.f, 0.54, 0, 0, 1.f, "Z - Go down"); break;
	}
	

}

// Renders text to screen. Must be called last in render function (before swap buffers)
void Scene::displayText(float x, float y, float r, float g, float b, char* string) {
	// Get Lenth of string
	int j = strlen(string);

	// Swap to 2D rendering
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1.0, 1.0, -1.0, 1.0, 5, 100);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	// Orthographic lookAt (along the z-axis).
	gluLookAt(0.0f, 0.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

	// Set text colour and position.
	glColor3f(r, g, b);
	glRasterPos2f(x, y);
	// Render text.
	for (int i = 0; i < j; i++) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, string[i]);
	}
	// Reset colour to white.
	glColor3f(1.f, 1.f, 1.f);

	// Swap back to 3D rendering.
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, ((float)width/(float)height), nearPlane, farPlane);
	glMatrixMode(GL_MODELVIEW);
}
