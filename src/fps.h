#include "support.h"
#include "cgmath.h"		// slee's simple math library
#include "trackball.h"
#include "mesh.h"
#include "inc/fmod.hpp"
#include <time.h>

#pragma comment (lib, "fmodex_vc.lib")

int curStage = 0;


#define MAIN 0
#define KEYBOARD 1
#define GAME 2
int state = MAIN;
int isMenu = 0;

//*******************************************************************
// index variables for OpenGL objects
GLuint	program = 0;
GLuint	screenBuffer = 0;// ID holder for GPU program
GLuint	vertexCircleBuffer = 0;		// ID holder for vertex position buffer
GLuint	vectexSquareBuffer = 0;
GLuint	spikebuffer = 0;
GLuint	indexBuffer = 0;
GLuint	ObjectBuffer = 0;
GLuint	screenWordsbuffer = 0;
//*******************************************************************
// global variables
int		frame = 0;	// index of rendering frames
Camera	camera;
int windowWidth = 1280;//1366
int windowHeight = 720;//768


//Variables for changing visible with mouse
int programbegin = 0;
int prex = 0;
int prey = 0;
float radx = 0;
float rady = 0;


//variable for changing eye of camera by keyboard
float falling_speed = 0.0f;
float gravity = 0.25f;
vec4 acc = vec4(0, 0, 0, 0);
bool keystate[256];
bool ablejump = true;
bool up = true;
int walk = 0;

float screenheight = 1.0f;
int c = 1;
vec4 colors = vec4{ 0.0f, 0.0f, 0.0f, 1.0f };
vec3 rotat1 = vec3(0, 0, 1);
float speed = 0.001f;

bool iisHUD = 0;
bool isStart = 1;

//variable for time
int dt = 0;
//int second;

struct object{
	vec3 pos1;
	vec3 pos2;
	vec3 pos3;
	vec3 mov;
};

struct spik
{
	int index;
	vec3 mov;
	vec3 position;
	vec4 color;
	bool visible;
};

struct squ
{
	int index;
	vec3 position;
	vec4 color;
	bool bloom;
	bool visible;
	bool boom;
	//for boom animation
	bool boomAni;
};
struct squ box[100];
struct spik spikes[100];
struct object inv[3];

int currentbgm = 0;
bool screenbit = 0;
float rotat = 0;
bool boombit = false;

FMOD::System *g_System;
FMOD::Sound *g_Sound[17];
FMOD::Channel *g_Channel[5];

bool game = true;

//*******************************************************************
void accerlate()
{
	if (keystate['w'] == true)
	{
		if (acc.x < 2.5f)
		{
			acc.x += 0.1f;
		}
	}
	else if (keystate['w'] == false)
	{
		if (acc.x > 0.0f)
		{
			acc.x -= 0.2f;
		}
		else if (acc.x < 0.0f)
		{
			acc.x = 0.0f;
		}
	}
	if (keystate['a'] == true)
	{
		if (acc.y < 2.5f)
		{
			acc.y += 0.1f;
		}
	}
	else if (keystate['a'] == false)
	{
		if (acc.y > 0.0f)
		{
			acc.y -= 0.2f;
		}
		else if (acc.y < 0.0f)
		{
			acc.y = 0.0f;
		}
	}
	if (keystate['s'] == true)
	{
		if (acc.z < 2.5f)
		{
			acc.z += 0.1f;
		}
	}
	else if (keystate['s'] == false)
	{
		if (acc.z > 0.0f)
		{
			acc.z -= 0.2f;
		}
		else if (acc.z < 0.0f)
		{
			acc.z = 0.0f;
		}
	}
	if (keystate['d'] == true)
	{
		if (acc.w < 2.5f)
		{
			acc.w += 0.1f;
		}
	}
	else if (keystate['d'] == false)
	{
		if (acc.w > 0.0f)
		{
			acc.w -= 0.2f;
		}
		else if (acc.w < 0.0f)
		{
			acc.w = 0.0f;
		}
	}
	
}
void setfootstomp()
{
	if (keystate['w'] == true || keystate['a'] == true || keystate['s'] == true || keystate['d'] == true)
		walk++;
	if ((walk > 18) && (ablejump == true))
	{
		walk = 0;
		g_System->playSound(FMOD_CHANNEL_FREE, g_Sound[10], 0, &g_Channel[1]);
	}
	vec3 front = (camera.u ^ camera.up).normalize();
	vec3 left = camera.u.normalize();

	camera.eye.z -= 5 * front.z * acc.x;
	camera.eye.x -= 5 * front.x * acc.x;
	
	camera.eye.z += 5 * front.z * acc.z;
	camera.eye.x += 5 * front.x * acc.z;
	
	camera.eye.z -= 5 * left.z * acc.y;
	camera.eye.x -= 5 * left.x * acc.y;
	
	camera.eye.z += 5 * left.z * acc.w;
	camera.eye.x += 5 * left.x * acc.w;
	
	
}

void setgravity_jump()
{
	falling_speed -= gravity;

	camera.eye.y += falling_speed;

	int i = (int)camera.eye.z / 200;
	int j = (int)camera.eye.x / 200;

	
	if (camera.eye.y <= 100)
	{
		if ((camera.eye.x >= 0.0f) && (camera.eye.z >= 0.0f) && (box[10 * i + j].visible == true) && (up == true))
		{
			ablejump = true;
			camera.eye.y = 100.0f;
			falling_speed = 0;
		}
		else
		{
			up = false;
			ablejump = false;
		}
	}
	
	
}