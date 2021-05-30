#pragma once

#include "cgmath.h"

// camera
struct Camera
{
	mat4	viewMatrix;
	vec3	eye;
	vec3	at;
	vec3	up;	
	vec3	n;
	vec3	u;
	vec3	v;


	mat4	projectionMatrix;
	float	fovy;			// in radian
	float	aspectRatio;
	float	dNear;
	float	dFar;
};