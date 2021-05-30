#pragma once

#include "cgmath.h"
#include "GL/glew.h"
#include <vector>


struct Obj{

	GLuint vertexbuffer;
	GLuint indexbuffer;

	std::vector< vec3 > vertices;
	std::vector< vec2 > uvs;
	std::vector< vec3 > normals;

	std::vector<uint> vertexIndices, uvIndices, normalIndices;

};

Obj loadObj(char *fname, char *mtlname);