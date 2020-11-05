#include "Camera.h"

//Update function calculates where the camera is looking at
//The following code is adapted from Lecture slides from CMP 203 Graphics Programming
void Camera::update()
{
	//temp values for sin/cos from
	float cosR, cosP, cosY;
	float sinR, sinP, sinY;
	
	//Roll, pitch and yall are variables sorted by the camera
	//handle rotation
	//only want to calculate these values once, when rotation changes, not every frame

	cosY = cosf(Yaw * PI/ 180);
	cosP = cosf(Pitch * PI / 180);
	cosR = cosf(Roll * PI / 180);
	sinY = sinf(Yaw * PI / 180);
	sinP = sinf(Pitch * PI / 180);
	sinR = sinf(Roll * PI / 180);


	//this using the parametric equation of a sphere

	//calculate the three vectors to put into glu lookat
	//look direction, position and the up vector
	//This ficmtion could also calculate the right vecotr

	
	
		forward.x = sinY * cosP;
		forward.y = sinP;
		forward.z = cosP * -cosY;
	
	//look at point
	//To calculate add forward vector to camera point

	//up vector
	up.x = -cosY * sinR - sinY * sinP * cosR;
	up.y = cosP * cosR;
	up.z = -sinY * sinR - sinP* cosR * -cosY;

	//slide Vector (right)
	//This is a cross product between the foward and up vector
	// if you dont need to calculate this, dont do it


}


