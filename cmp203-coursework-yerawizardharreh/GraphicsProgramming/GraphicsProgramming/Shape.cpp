#include "shape.h"

//Vertex for barricade
extern float vertsBarricade[] = {	

							//Left
							0,0,0,
							1,0,0,
							1,1,0,

							//Right
							0,0,1,
							1,0,1,
							1,1,1,

							//Front left
							1,1,1,
							0,0,0,
							1,1,0,

							//front right
							1,1,1,
							0,0,0,
							0,0,1,

							//Back left
							1,1,1,
							1,1,0,
							1,0,0,

							//back right
							1,1,1,
							1,0,0,
							1,0,1,
							
};

//Normals for barricade
extern float norms[] = {
						//LEFT
						0,0,1,
						0,0,1,
						0,0,1,

						//Right
						0,0,-1,
						0,0,-1,
						0,0,-1,

						//Front

						1,0,0,
						1,0,0,
						1,0,0,
						1,0,0,
						1,0,0,
						1,0,0,

						//Back
						-1,0,0,
						-1,0,0,
						-1,0,0,
						-1,0,0,
						-1,0,0,
						-1,0,0,

						};		

//UV texture coordinates for barricade
extern float texcoords[]= {

							//Left
							0,1,
							1,1,
							1,0,

							//Right
							0,1,
							1,1,
							0,1,

							//Front 
							0,0,
							1,1,
							0,1,

							0,0,
							1,1,
							0,1,

							//back
							1,0,
							0,0,
							0,1,

							1,0,
							0,1,
							1,1,



							};

//Vertex for Cube
extern float vertsCube[] = {

						//fr0nt

						-1,1,1,
						-1,-1,1,
						1,-1,1,
						1,1,1,

						//back
						-1,1,-1,
						-1,-1,-1,
						1,-1,-1,
						1,1,-1,

						//Top

						-1,1,1,
						-1,1,-1,
						1,1,-1,
						1,1,1,

						//bottom

						-1,-1,1,
						-1,-1,-1,
						1,-1,-1,
						1,-1,1,

						//left

						-1,1,-1,
						-1,-1,-1,
						-1,-1,1,
						-1,1,1,

						//right
						1,1,-1,
						1,-1,-1,
						1,-1,1,
						1,1,1,
					
};

//Normals for Cube
extern float normsCube[] = {

					//front
					0,0,1,
					0,0,1,
					0,0,1,
					0,0,1,

					//back
					0,0,-1,
					0,0,-1,
					0,0,-1,
					0,0,-1,

					//Top
					0,1,0,
					0,1,0,
					0,1,0,
					0,1,0,

					//bottom
					0,1,0,
					0,1,0,
					0,1,0,
					0,1,0,
					
					//left
					-1,0,0,
					-1,0,0,
					-1,0,0,
					-1,0,0,

					//right
					1,0,0,
					1,0,0,
					1,0,0,
					1,0,0,

};

//UV texture coordinates for cube
extern float texCube[] = {
						
					//front
					0,0,
					0,1,
					1,1,
					1,0,

					//back
						0,0,
					0,1,
					1,1,
					1,0,

					//top
						0,0,
					0,1,
					1,1,
					1,0,
					
					//bottom
						0,0,
					0,1,
					1,1,
					1,0,

					//left
						0,0,
					0,1,
					1,1,
					1,0,

					//right
						0,0,
					0,1,
					1,1,
					1,0,

					
};

//UV texture coordinates for skybox
extern float texSky[] = {

	//front
	0.5,0.25,
	0.5,0.5,
	0.25,0.5,
	0.25,0.25,

	//back
	0.75,0.25,
	0.75,0.5,
	1,0.5,
	1,0.25,

	//top
	0.25,0,
	0.25,0.25,
	0.5,0.25 ,
	0.5,0,


	//bottom
	0.5,0.5,
	0.5,0.75,
	0.25,0.75,
	0.25,0.5,

	//left
	0.75,0.25,
	0.75,0.5,
	0.5,0.5,
	0.5,0.25,

	//right
	0,0.25,
	0,0.5,
	0.25,0.5,
	0.25,0.25,
};

//indices for cube
extern GLubyte cubeIndices[] = {
					//front
					0,1,2,
					2,3,0,

					//back
					4,5,6,
					6,7,4,

					//top
					8,9,10,
					10,11,8,

					//bottom
					12,13,14,
					14,15,12,

					//left
					16,17,18,
					18,19,16,

					//right
					20,21,22,
					22,23,20,
};

//render barricade using Vertex array
void Shape::renderBarricade()
{
	//Enable vertex arrays
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	//specify data for openGL to use
	glVertexPointer(3, GL_FLOAT, 0, vertsBarricade);
	glNormalPointer(GL_FLOAT, 0, norms);
	glTexCoordPointer(2, GL_FLOAT, 0, texcoords);

	//Render array element 1 to 18
	glBegin(GL_TRIANGLES);

		for (int i = 0; i < 18; i++)
		{
			glArrayElement(i);
		}

	glEnd();

	//disable vertex array
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

//render a cube using vertex arrays
void Shape::renderCube()
{
	//enable vertex array
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	//Specify data for openGL to use
	glVertexPointer(3, GL_FLOAT, 0, vertsCube);
	glNormalPointer(GL_FLOAT, 0, normsCube);
	glTexCoordPointer(2, GL_FLOAT, 0, texCube);

	//dereference
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, cubeIndices);

	//disable vertex array
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}

void Shape::renderSkybox()
{
	//enable vertex array
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	//specify data for openGL to use
	glVertexPointer(3, GL_FLOAT, 0, vertsCube);
	glNormalPointer(GL_FLOAT, 0, normsCube);
	glTexCoordPointer(2, GL_FLOAT, 0, texSky);

	//dereference
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, cubeIndices);

	//disable vertex array
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);


}
