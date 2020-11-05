#pragma once
//Camera Class used to help the glulookat function easier to control


//Includes used
#include <cmath>
#include "Vector3.h"


class Camera
{
	//defining Pi for readability
#define PI (3.1415)

public:

	//functions

	//Update function calculates where it is looking at
	void update();

	//move function uses vector3 passed in to update its position in 3d space
	void move(Vector3 temp,float dt)
	{
		//adds the vector3 temp to the position multiplied by 5 and delta time 
		position.x += temp.x * 5 * dt;
		position.y += temp.y * 5 * dt;
		position.z += temp.z * 5* dt;
	};
	
	//rotation function takes vector3 temp and updates the yaw pitch and roll of the camera
	void rotation(Vector3 temp,float dt)
	{
		//add vector3 temp to the yaw pitch and roll multiplied by 5 and delta time
		Yaw += temp.x *40* dt;
		Pitch += temp.y * 40*dt;
		Roll += temp.z * 40*dt;
	};


	//getters
	float getUpx() { return up.x; }
	float getUpy() { return up.y; }
	float getUpz() { return up.z; }

	float getPosX() { return position.x; }
	float getPosY() { return position.y; }
	float getPosZ() { return position.z; }

	Vector3 getLookat() { return position + forward; }

	float getYaw() { return Yaw; }
	float getPitch() { return Pitch; }
	float getRoll() { return Roll; }

//setters
	void setPosition(Vector3 temp) {position = temp; }
	void setUp(Vector3 temp) { up = temp; }
	void setYaw(float temp) { Yaw = temp; }
	void setPitch(float temp) { Pitch = temp; }
	void setRoll(float temp) { Roll = temp; }

	//private variables
private:
	float Yaw, Pitch, Roll;
	Vector3 up;
	Vector3 position;
	Vector3 forward;
};

