#include "Model.h"
// Below ifdef required to remove warnings for unsafe version of fopen and fscanf.
// Secure version won't work cross-platform, forcing this small hack.
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

//Function provided my module CMP 203 Graphics programming
bool Model::load( char* modelFilename,  char* textureFilename)
{
	
	bool result;

	// Load in the model data,
	result = loadModel(modelFilename);
	if (!result)
	{
		MessageBox(NULL, "Model failed to load", "Error", MB_OK);
		return false;
	}

	// Load the texture for this model.
loadTexture(textureFilename);

	return true;
	
}

//Render function takes vectors and renders imported model
void Model::render()
{
	//bind 2d texture to model
	glBindTexture(GL_TEXTURE_2D, texture);

	//enable vertex array
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_NORMAL_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		
	//specify data for array
		float* vertex = &sortVer[0];
		glVertexPointer(3, GL_FLOAT, 0, vertex);

		float* normal = &sortNorm[0];
		glNormalPointer(GL_FLOAT, 0, normal);

		float* texCoord = &sortTex[0];
		glTexCoordPointer(2, GL_FLOAT, 0,texCoord);

		//dereference
	//draw triangles for number of faces	
	glBegin(GL_TRIANGLES);

	for (int i = 0; i< sortFace.size(); i++)
	{
			glArrayElement(i);
	}

	glEnd();


	//disable vertex array
	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_NORMAL_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
}


// Modified from a mulit-threaded version by Mark Ropper.
bool Model::loadModel(char* filename)
{
	vector<Vector3> verts;
	vector<Vector3> norms;
	vector<Vector3> texCs;
	vector<unsigned int> faces;
	
	FILE* file = fopen(filename, "r");
	if (file == NULL)
	{
		return false;
	}
	while (true)
	{
		char lineHeader[128];

		// Read first word of the line
		int res = fscanf(file, "%s", lineHeader);
		if (res == EOF)
		{
			break; // exit loop
		}
		else // Parse
		{
			if (strcmp(lineHeader, "v") == 0) // Vertex
			{
				Vector3 vertex;
				fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
				verts.push_back(vertex);
			}
			else if (strcmp(lineHeader, "vt") == 0) // Tex Coord
			{
				Vector3 uv;
				fscanf(file, "%f %f\n", &uv.x, &uv.y);
				texCs.push_back(uv);
			}
			else if (strcmp(lineHeader, "vn") == 0) // Normal
			{
				Vector3 normal;
				fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
				norms.push_back(normal);
			}
			else if (strcmp(lineHeader, "f") == 0) // Face
			{
				unsigned int face[9];
				int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &face[0], &face[1], &face[2],
																			&face[3], &face[4], &face[5],
																			&face[6], &face[7], &face[8]);
				if (matches != 9)
				{
					// Parser error, or not triangle faces
					return false;
				}
				
				for (int i = 0; i < 9; i++)
				{
					faces.push_back(face[i]);
				}


			}
		}
	}

	// "Unroll" the loaded obj information into a list of triangles.
	// TODO: By this point all model has been read from the file, but is not in the correct order.
	// You NEED to loop over all the data and sort it into a render ready order/format.
	
	/* IGNORE THIS
	
	Process the data pseudo-code•
	Loop for all the face data•
	Use first face value•
	Vertex index
	•Store x, y, z for vertex in another vector
	•Use second face value•Texture coordinates
	•Store u, v in texCoordvector
	•Use third face value•Normals
	•Store x, y, z in normalsvector•
	Increment loop by 3
	
	*/

	//loop for size of faces vector
	for (int i = 0; i < faces.size(); i += 3)
	{
		
		//push all unsorted data into vectors in order of faces
		temp = verts[faces[i] -1 ];
		sortVer.push_back(temp.x);
		sortVer.push_back(temp.y);
		sortVer.push_back(temp.z);

		temp = texCs[faces[i + 1]-1];
		sortTex.push_back(temp.x);
		sortTex.push_back(temp.y);

		temp = norms[faces[i + 2]-1];
		sortNorm.push_back(temp.x);
		sortNorm.push_back(temp.y);
		sortNorm.push_back(temp.z);
		
		temp = (faces[i], faces[i + 1], faces[i + 2]);
		sortFace.push_back(temp);
	
	}
	
	// Once data has been sorted clear read data (which has been copied and are not longer needed).
	verts.clear();
	norms.clear();
	texCs.clear();
	faces.clear();

	return true;
}

//function provided by module CMP203 Graphics Programming
void Model::loadTexture(char* filename)
{
	texture = SOIL_load_OGL_texture
	(
		filename,
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT | SOIL_FLAG_INVERT_Y // Depending on texture file type some need inverted others don't.
	);

	//check for an error during the load process
	if (texture == 0)
	{
		printf("SOIL loading error: '%s'\n", SOIL_last_result());
	}

}



