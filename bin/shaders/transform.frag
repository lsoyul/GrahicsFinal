// the second input from vertex shader
varying vec3 norm;
varying float HUD;

uniform vec4 tilecolor;
uniform vec4 HUDcolor;

uniform sampler2D TEX;	//texture object from application
varying vec2 tc;
uniform int isMenu;

void main()
{
	if(isMenu==1){
		gl_FragColor = texture2D( TEX, tc );
		return;
	}
	if(HUD == 1.0)
	{
		gl_FragColor = HUDcolor;
		//gl_FragColor = vec4(0.0,1.0,0.0,1.0);
	}
	else
	{
		gl_FragColor = tilecolor;
	}
}
