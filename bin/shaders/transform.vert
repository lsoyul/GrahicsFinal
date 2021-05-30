#version 120

// vertex attributes
attribute vec3 position;
attribute vec2 texcoord;

// matrix
uniform vec3 tileposition;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;
uniform float aspectRatio;
uniform float screenheight;

//Select HUD, or other sprite
uniform int isHUD;
uniform vec3 color;
uniform int isMenu;

//VARYING
varying vec3 norm;
varying float HUD;
varying vec2 tc;


void main()
{
	if(isMenu==1){
		gl_Position = vec4( position, 1 );
		tc = texcoord;
		return;
	}
	if(isHUD == 1)
	{
		gl_Position = vec4( position.x,position.y - screenheight, position.z, 1.0 );
		norm = color;
		HUD = 1.0;
	}
	else
	{
		vec4 wpos = modelMatrix * vec4(position + tileposition, 1.0);
		vec4 epos = viewMatrix * wpos;
		gl_Position = projectionMatrix * epos;

		// pass normal to fragment shader
		norm = color;
		HUD = 0.0;
	}
}
