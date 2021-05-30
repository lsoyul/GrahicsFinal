#pragma once

#include "cgmath.h"
#include "GL/glew.h"

// vertex/index/mesh definitions
struct vertex
{
	vec3 pos;
	vec3 norm;
	vec2 tex;
};

struct Mesh
{
	GLuint				vertexBuffer;
	GLuint				indexBuffer;
	std::vector<vertex>	vertexList;
	std::vector<uint>	indexList;
};

Mesh* loadMesh( const char* vertexFilePath, const char* indexFilePath );