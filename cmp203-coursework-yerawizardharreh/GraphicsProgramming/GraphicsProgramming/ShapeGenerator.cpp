#include "ShapeGenerator.h"

//constructor
ShapeGenerator::ShapeGenerator()
{
}

//destructor
ShapeGenerator::~ShapeGenerator()
{
}

//Function handles generating a Quad using width and height passed in
void ShapeGenerator::generateQuad(int width, int height)
{
	//begin drawing quad
	glBegin(GL_QUADS);
	//loop for width
	for (int i = 0; i < width; i++)
	{
		//loop for height
		for (int j = 0; j < height; j++)
		{
			//define normal
			glNormal3f(0, 1, 0);

			//define texture coord and Vertex
			glTexCoord2f(0,0);
			glVertex3f(i, 0, j );

			glTexCoord2f(0, 1);
			glVertex3f(i, 0, j+1);

			glTexCoord2f(1, 1);
			glVertex3f(i + 1, 0, j+1);

			glTexCoord2f(1, 0);
			glVertex3f(i + 1, 0, j );
		}
	}
	glEnd();
}

//Function handles generating a cylinder
void ShapeGenerator::generateCylinder(float total, float height,float radi)
{
	//initialise variables 
	float x, y, U, V, theta, X,Y;
	float inter = ((2 * 180) / total);
	float radius = radi;
	float angle = 0;
	float mag = 0;

	//Vector used to store points of the cylinder
	vector <Vector3> points, tex,tubeTex,norms;

	//SAVE POINTS ON CYLINDER

	//loop for total(number of segments)
	for (int i = 0; i < total; i++)
	{
		//calculate X and Y points for vertex and U V points for textures
		theta = (angle * PI) / 180;
		x = radius * cosf(theta);
		y = radius * sinf(theta);
		U = (cosf(theta) / (radius * 2)) + 0.5;
		V = (sinf(theta) / (radius * 2)) + 0.5;

		//square X and Y to find magnitude
		X = x * x;
		Y = y * y;
		mag = sqrt(X + Y);

		//push back vertex texture coords and normals
		points.push_back(Vector3(x, y, 0));
		tex.push_back(Vector3(U, V, 0));
		norms.push_back(Vector3(X / mag, Y / mag, 0));

		//increment angle
		angle += inter;
		//check if angle is between 0 - 360
		if (angle >= 360)
		{
			angle = 0;
		}

		//calculate X and Y points for vertex and U V points for textures
		theta = (angle * PI) / 180;
		x = radius * cosf(theta);
		y = radius * sinf(theta);
		U = (cosf(theta) / (radius * 2)) + 0.5;
		V = (sinf(theta) / (radius * 2)) + 0.5;

		//square X and Y to find magnitude
		X = x * x;
		Y = y * y;
		mag = sqrt(X + Y);

		//push back vertex texture coords and normals
		points.push_back(Vector3(x, y, 0));
		tex.push_back(Vector3(U, V, 0));
		norms.push_back(Vector3(X / mag, Y / mag, 0));

	}


	//Bottom Disc

	//This will draw a disc by looping triangles where once point is always at the center and the others simply follow the order in the vector
	for (int i = 0; i < (total * 2) - 1; i++)
	{

		glBegin(GL_TRIANGLES);

		glNormal3f(0, 0, -1);
		glTexCoord2f(0.5, 0.5);
		glVertex3f(0, 0, 0);

		glNormal3f(0, 0, -1);
		glTexCoord2f(tex[i].x, tex[i].y);
		glVertex3f(points[i].x, points[i].y, 0);

		glNormal3f(0, 0, -1);
		glTexCoord2f(tex[i + 1].x, tex[i + 1].y);
		glVertex3f(points[i + 1].x, points[i + 1].y, 0);

		glEnd();
	}

	//This evenly spreads the UV textures onto the tube
	for (float i = 0; i <= 1; i += 1/height) 
	{
		for (float j = 0; j <= 1; j += 1/total)
		{
			tubeTex.push_back(Vector3(j, i, 0));
		}
	}


	//draw tube
	//This tube is rendered by looping the same way the disc is drawn but
	//rather than always having a center point triangles are drawn in the z axis too
	for (int i = 0; i < height; i++)
	{
		glBegin(GL_TRIANGLES);

		for (int j = 0; j < (total * 2) - 1; j++)
		{
			glNormal3f(norms[i].x, norms[i].y, 0);
			glTexCoord2f(tubeTex[j].x, tubeTex[j].y);
			glVertex3f(points[j].x, points[j].y, i + 1);

			glNormal3f(norms[i].x, norms[i].y, 0);
			glTexCoord2f(tubeTex[j].x, tubeTex[j].y);
			glVertex3f(points[j].x, points[j].y, i);

			glNormal3f(norms[i+1].x, norms[i+1].y, 0);
			glTexCoord2f(tubeTex[j + 1].x, tubeTex[j + 1].y);
			glVertex3f(points[j + 1].x, points[j + 1].y, i + 1);

			glNormal3f(norms[i].x, norms[i].y, 0);
			glTexCoord2f(tubeTex[j].x, tubeTex[j].y);
			glVertex3f(points[j].x, points[j].y, i);

			glNormal3f(norms[i+1].x, norms[i+1].y, 0);
			glTexCoord2f(tubeTex[j + 1].x, tubeTex[j + 1].y);
			glVertex3f(points[j + 1].x, points[j + 1].y, i + 1);

			glNormal3f(norms[i+1].x, norms[i+1].y, 0);
			glTexCoord2f(tubeTex[j + 1].x, tubeTex[j + 1].y);
			glVertex3f(points[j + 1].x, points[j + 1].y, i);
		}
		glEnd();

	}


	//draw disc top
	//This will draw a disc by looping triangles where once point is always at the center and the others simply follow the order in the vector
	for (int i = 0; i < (total * 2) - 1; i++)
	{
		glBegin(GL_TRIANGLES);

		glNormal3f(0, 0, 1);
		glTexCoord2f(0.5, 0.5);
		glVertex3f(0, 0, height);

		glNormal3f(0, 0, 1);
		glTexCoord2f(tex[i].x, tex[i].y);
		glVertex3f(points[i].x, points[i].y, height);

		glNormal3f(0, 0, 1);
		glTexCoord2f(tex[i + 1].x, tex[i + 1].y);
		glVertex3f(points[i + 1].x, points[i + 1].y, height);
		glEnd();
	}
}

//Function handles rendering a pyramid 
void ShapeGenerator::generatePyramid(float width, float breadth, float height)
{
	//initialise variables 
	//X and Y are halved as the pyramid should be rendered around 0,0,0
	float X = width / 2;
	float Y = breadth / 2;

	//draw square base
	glBegin(GL_QUADS);

	glNormal3f(0, 0, -1);
	glTexCoord2f(0, 0);
	glVertex3f(-X, Y, 0);

	glNormal3f(0, 0, -1);
	glTexCoord2f(0, 1);
	glVertex3f(-X, -Y, 0);

	glNormal3f(0, 0, -1);
	glTexCoord2f(1, 1);
	glVertex3f(X, -Y, 0);

	glNormal3f(0, 0, -1);
	glTexCoord2f(1, 0);
	glVertex3f(X, Y, 0);

	glEnd();

	//draw triangles which link square base to center point in z axis
	glBegin(GL_TRIANGLES);

	glNormal3f(-1, 0, 0);
	glTexCoord2f(0.5, 0);
	glVertex3f(0, 0, height);

	glNormal3f(-1, 0, 0);
	glTexCoord2f(0, 1);
	glVertex3f(-X, Y, 0);

	glNormal3f(-1, 0, 0);
	glTexCoord2f(1, 1);
	glVertex3f(-X, -Y, 0);

	glNormal3f(0, -1, 0);
	glTexCoord2f(0.5, 0);
	glVertex3f(0, 0, height);

	glNormal3f(0, -1, 0);
	glTexCoord2f(0, 1);
	glVertex3f(-X, -Y, 0);

	glNormal3f(0, -1, 0);
	glTexCoord2f(1, 1);
	glVertex3f(X, -Y, 0);

	glNormal3f(1, 0, 0);
	glTexCoord2f(0.5, 0);
	glVertex3f(0, 0, height);

	glNormal3f(1, 0, 0);
	glTexCoord2f(0, 1);
	glVertex3f(X, -Y, 0);

	glNormal3f(1, 0, 0);
	glTexCoord2f(1, 1);
	glVertex3f(X, Y, 0);

	glNormal3f(0, 1, 0);
	glTexCoord2f(0.5, 0);
	glVertex3f(0, 0, height);

	glNormal3f(0, 1, 0);
	glTexCoord2f(0, 1);
	glVertex3f(X, Y, 0);

	glNormal3f(0, 1, 0);
	glTexCoord2f(1, 1);
	glVertex3f(-X, Y, 0);

	glEnd();
	//draw sides

}
