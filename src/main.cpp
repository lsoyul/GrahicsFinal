#include "game.h"
#include "menu.h"
#include "OBJloader.h"

#define numOfL 40

Obj inv1;
Obj inv2;
Obj inv3;



void update()
{
	// Moving player function
	SetCursorPos(windowWidth / 2, windowHeight / 2);

	if (game == true)
	{
		accerlate();
		setfootstomp();
		setgravity_jump();

		bloomfoottile();			// set another color from camera eye
		hit();						//if hit,
		finish();
		setcolors();
		falling();
	}
	g_System->update();

	if ((screenbit == 1) && (c == 30))
	{
		clear();
	}

	// init starting time clear()

	if (isStart == 1) {
		//boom(pat[curStage].start);
		game = false;
		screenbit = 1;
		isStart = 0;
	}
	//function for boom pattern
	for (int i = 0; i < 2*pat[curStage].length; i++)
	{
		if (dt == pat[curStage].gaps[i])
		{
			for (uint j = 0; j < pat[curStage].sequence[i].size(); j++)
			{
				boom(pat[curStage].sequence[i][j]);
			}
		}
	}

	
	//function for hit point

	for (int i = 0; i < 100; i++){
		spikes[i].position += spikes[i].mov;
	}


	// configure projection matrix
	camera.fovy = PI/3.0f;
	//camera.aspectRatio; // udpate in reshape()
	camera.dNear = 1.0f;
	camera.dFar = 20000.0f;
	camera.projectionMatrix = mat4::perspective( camera.fovy, camera.aspectRatio, camera.dNear, camera.dFar );


	// send  Matrixes to vertex shader
	mat4 modelMatrix;
	GLint uloc;
	uloc = glGetUniformLocation( program, "modelMatrix" );
	if(uloc>-1) glUniformMatrix4fv( uloc, 1, GL_TRUE, modelMatrix );
	uloc = glGetUniformLocation( program, "viewMatrix" );
	if(uloc>-1) glUniformMatrix4fv( uloc, 1, GL_TRUE, camera.viewMatrix );
	uloc = glGetUniformLocation( program, "projectionMatrix" );
	if(uloc>-1) glUniformMatrix4fv( uloc, 1, GL_TRUE, camera.projectionMatrix );
	uloc = glGetUniformLocation(program, "aspectRatio");
	if (uloc > -1) glUniform1f(uloc, camera.aspectRatio);


	ShowCursor(false);
	
}

void render()
{

	if (state == MAIN) render_menu();

	else if (state == KEYBOARD) render_keyboard();

	else {
		// clear screen (with background color) and clear depth buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// notify to GL that we like to use our program now
		glUseProgram(program);

		glUniform1i(glGetUniformLocation(program, "isMenu"), 0);

		for (uint i = 0; i < pat[curStage].visibles.size(); i++){
			box[pat[curStage].visibles[(int)i]].visible = true;
			spikes[pat[curStage].visibles[(int)i]].visible = true;
		}
		iisHUD = 0;

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

		//tile**************************************************************************************

		glLineWidth(2.0f);

		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				if (box[10 * i + j].visible == true)
				{
					GLint uloc = glGetUniformLocation(program, "isHUD");
					if (uloc > -1) glUniform1i(uloc, 0);

					uloc = glGetUniformLocation(program, "tileposition");
					if (uloc > -1) glUniform3fv(uloc, 1, box[10 * i + j].position);

					GLuint vertexPositionLoc = glGetAttribLocation(program, "position");
					glEnableVertexAttribArray(vertexPositionLoc);
					glBindBuffer(GL_ARRAY_BUFFER, vectexSquareBuffer);
					glVertexAttribPointer(vertexPositionLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

					glUniform4fv(glGetUniformLocation(program, "tilecolor"), 1, vec4(0.0f, 0.0f, 0.0f, 0.0f));
					glDrawElements(GL_LINE_STRIP, 24, GL_UNSIGNED_INT, NULL);

					if (box[10 * i + j].boom == true)
					{
						uloc = glGetUniformLocation(program, "tilecolor");
						if (uloc > -1) glUniform4fv(uloc, 1, vec4(1.0f, 0.0f, 0.0f, 1.0f));

					}
					else
					{
						box[10 * i + j].color.x = 1.0f - ((float)curStage*0.03f);
						uloc = glGetUniformLocation(program, "tilecolor");
						if (uloc > -1) glUniform4fv(uloc, 1, (box[10 * i + j].bloom == true) ? vec4(1.0f, 1.0f, 0.0f, 1.0f) : box[10 * i + j].color);
					}
					if (box[10 * i + j].index == pat[curStage].start)
						glUniform4fv(glGetUniformLocation(program, "tilecolor"), 1, vec4(1.0f, 0.5f, 0.0f, 1.0f));

					if (box[10 * i + j].index == pat[curStage].finish)
						glUniform4fv(glGetUniformLocation(program, "tilecolor"), 1, vec4(0.3f, 1.0f, 0.0f, 1.0f));

					glDrawElements(GL_QUADS, 24, GL_UNSIGNED_INT, NULL);
				}
			}
		}

		// SPIKES****************************************************************


		for (int i = 0; i < 10; i++)
		{
			for (int j = 0; j < 10; j++)
			{
				if (spikes[10 * i + j].visible == true)
				{
					//printf("%d\n", 10*i+j);
					GLint uloc = glGetUniformLocation(program, "isHUD");
					if (uloc > -1) glUniform1i(uloc, 0);

					uloc = glGetUniformLocation(program, "tileposition");
					if (uloc > -1) glUniform3fv(uloc, 1, spikes[10 * i + j].position);

					GLuint vertexPositionLoc = glGetAttribLocation(program, "position");
					glEnableVertexAttribArray(vertexPositionLoc);
					glBindBuffer(GL_ARRAY_BUFFER, spikebuffer);
					glVertexAttribPointer(vertexPositionLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

					glUniform4fv(glGetUniformLocation(program, "tilecolor"), 1, vec4(0.0f, 0.0f, 0.0f, 0.0f));
					glDrawArrays(GL_LINE_STRIP, 0, 18);
					glUniform4fv(glGetUniformLocation(program, "tilecolor"), 1, colors);
					//glUniform4fv(glGetUniformLocation(program, "tilecolor"), 1, spikes[10*i+j].color);

					glDrawArrays(GL_TRIANGLES, 0, 18);

					if (box[10 * i + j].boom == true)
					{
						spikes[10 * i + j].mov = vec3(0.0f, 20.0f, 0.0f);
						if (spikes[10 * i + j].position.y > 120.0f) spikes[10 * i + j].mov = vec3(0.0f, 0.0f, 0.0f);
						if (spikes[10 * i + j].position.y > 200.0f) spikes[10 * i + j].position.y = -300.0f;
					}
					if (box[10 * i + j].boom == false)
					{
						spikes[10 * i + j].mov = vec3(0.0f, -20.0f, 0.0f);
						if (spikes[10 * i + j].position.y < 0.0f) {
							spikes[10 * i + j].mov = vec3(0.0f, 0.0f, 0.0f);
							spikes[10 * i + j].position.y = 0.0f;
						}
					}
				}
			}
		}

		////////////////////// background invaders ////////////////////////////////

		glLineWidth(1.0f);

		mat4 modelMatrix;
		modelMatrix = mat4::translate(vec3(2000.0f, 0.0f, 0.0f)) * modelMatrix;
		glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_TRUE, modelMatrix);

		glBindBuffer(GL_ARRAY_BUFFER, inv1.vertexbuffer);

		GLuint zomvertexPositionLoc = glGetAttribLocation(program, "position");
		glEnableVertexAttribArray(zomvertexPositionLoc);
		glVertexAttribPointer(zomvertexPositionLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glUniform3fv(glGetUniformLocation(program, "tileposition"), 1, vec3(0.0f, 0.0f, 0.0f));

		glUniform4fv(glGetUniformLocation(program, "tilecolor"), 1, vec4(0.0f, 0.0f, 0.0f, 0.0f));
		glDrawArrays(GL_LINES, 0, inv1.vertices.size());
		glUniform4fv(glGetUniformLocation(program, "tilecolor"), 1, colors);
		glDrawArrays(GL_QUADS, 0, inv1.vertices.size());


		//2
		glBindBuffer(GL_ARRAY_BUFFER, inv2.vertexbuffer);

		zomvertexPositionLoc = glGetAttribLocation(program, "position");
		glEnableVertexAttribArray(zomvertexPositionLoc);
		glVertexAttribPointer(zomvertexPositionLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glUniform4fv(glGetUniformLocation(program, "tilecolor"), 1, vec4(0.0f, 0.0f, 0.0f, 0.0f));
		glDrawArrays(GL_LINES, 0, inv2.vertices.size());
		glUniform4fv(glGetUniformLocation(program, "tilecolor"), 1, colors);
		glDrawArrays(GL_QUADS, 0, inv2.vertices.size());

		//3
		glBindBuffer(GL_ARRAY_BUFFER, inv3.vertexbuffer);

		zomvertexPositionLoc = glGetAttribLocation(program, "position");
		glEnableVertexAttribArray(zomvertexPositionLoc);
		glVertexAttribPointer(zomvertexPositionLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glUniform4fv(glGetUniformLocation(program, "tilecolor"), 1, vec4(0.0f, 0.0f, 0.0f, 0.0f));
		glDrawArrays(GL_LINES, 0, inv3.vertices.size());
		glUniform4fv(glGetUniformLocation(program, "tilecolor"), 1, colors);
		glDrawArrays(GL_QUADS, 0, inv3.vertices.size());

		modelMatrix = mat4::rotate(vec3(0.0f, 1.0f, 0.0f), 180.0f*PI / 180.0f) * modelMatrix;
		modelMatrix = mat4::translate(vec3(2200.0f, 0.0f, 1200.0f)) * modelMatrix;
		glUniformMatrix4fv(glGetUniformLocation(program, "modelMatrix"), 1, GL_TRUE, modelMatrix);


		glBindBuffer(GL_ARRAY_BUFFER, inv1.vertexbuffer);

		zomvertexPositionLoc = glGetAttribLocation(program, "position");
		glEnableVertexAttribArray(zomvertexPositionLoc);
		glVertexAttribPointer(zomvertexPositionLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glUniform4fv(glGetUniformLocation(program, "tilecolor"), 1, vec4(0.0f, 0.0f, 0.0f, 0.0f));
		glDrawArrays(GL_LINES, 0, inv1.vertices.size());
		glUniform4fv(glGetUniformLocation(program, "tilecolor"), 1, colors);
		glDrawArrays(GL_QUADS, 0, inv1.vertices.size());

		//2
		glBindBuffer(GL_ARRAY_BUFFER, inv2.vertexbuffer);

		zomvertexPositionLoc = glGetAttribLocation(program, "position");
		glEnableVertexAttribArray(zomvertexPositionLoc);
		glVertexAttribPointer(zomvertexPositionLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glUniform4fv(glGetUniformLocation(program, "tilecolor"), 1, vec4(0.0f, 0.0f, 0.0f, 0.0f));
		glDrawArrays(GL_LINES, 0, inv2.vertices.size());
		glUniform4fv(glGetUniformLocation(program, "tilecolor"), 1, colors);
		glDrawArrays(GL_QUADS, 0, inv2.vertices.size());

		//3
		glBindBuffer(GL_ARRAY_BUFFER, inv3.vertexbuffer);

		zomvertexPositionLoc = glGetAttribLocation(program, "position");
		glEnableVertexAttribArray(zomvertexPositionLoc);
		glVertexAttribPointer(zomvertexPositionLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glUniform4fv(glGetUniformLocation(program, "tilecolor"), 1, vec4(0.0f, 0.0f, 0.0f, 0.0f));
		glDrawArrays(GL_LINES, 0, inv3.vertices.size());
		glUniform4fv(glGetUniformLocation(program, "tilecolor"), 1, colors);
		glDrawArrays(GL_QUADS, 0, inv3.vertices.size());

		////////////////////////////////////////////////////////////////////////

		modelMatrix;
		modelMatrix = mat4::rotate(rotat1, rotat) * modelMatrix;
		rotat += speed;
		modelMatrix = mat4::translate(vec3(1000.0f, -100.0f, 1000.0f)) * modelMatrix;


		GLint ulocc = glGetUniformLocation(program, "tileposition");
		if (ulocc > -1) glUniform3fv(ulocc, 1, vec3(0, 0, 0));

		ulocc = glGetUniformLocation(program, "modelMatrix");
		if (ulocc > -1) glUniformMatrix4fv(ulocc, 1, GL_TRUE, modelMatrix);

		ulocc = glGetUniformLocation(program, "isHUD");
		if (ulocc > -1) glUniform1i(ulocc, 0);

		ulocc = glGetUniformLocation(program, "tilecolor");
		if (ulocc > -1) glUniform4fv(ulocc, 1, colors);

		GLuint vertexPositionLoc = glGetAttribLocation(program, "position");
		glEnableVertexAttribArray(vertexPositionLoc);
		glBindBuffer(GL_ARRAY_BUFFER, vertexCircleBuffer);
		glVertexAttribPointer(vertexPositionLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

		glPointSize(25.0f);
		glDrawArrays(GL_POINTS, 0, 73 * 37 + 2);




		//HUD*****************************************************************************

		GLint uloc = glGetUniformLocation(program, "isHUD");
		if (uloc > -1) glUniform1i(uloc, 1);

		iisHUD = 1;

		if (iisHUD == 1){
			glBindBuffer(GL_ARRAY_BUFFER, screenWordsbuffer);
			vertexPositionLoc = glGetAttribLocation(program, "position");
			glEnableVertexAttribArray(vertexPositionLoc);
			glVertexAttribPointer(vertexPositionLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
			vec4 Wcolor = vec4(colors.x*2.0f, colors.y*2.0f, colors.z, 1.0f);
			glUniform4fv(glGetUniformLocation(program, "HUDcolor"), 1, Wcolor);

			glPointSize(45.0f);
			glDrawArrays(GL_POINTS, 0, 62);
			glUniform4fv(glGetUniformLocation(program, "HUDcolor"), 1, colors);
		}

		if (screenbit == 1)
		{
			c++;
			if (c > 30)
				c = 30;
			setHeight();
			uloc = glGetUniformLocation(program, "screenheight");
			if (uloc > -1)glUniform1f(uloc, screenheight);

			GLuint vertexPositionLoc = glGetAttribLocation(program, "position");
			glEnableVertexAttribArray(vertexPositionLoc);
			glBindBuffer(GL_ARRAY_BUFFER, screenBuffer);
			glVertexAttribPointer(vertexPositionLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

			vec4 Hcolor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
			glUniform4fv(glGetUniformLocation(program, "HUDcolor"), 1, Hcolor);

			glDrawArrays(GL_QUADS, 0, 4);



		}
		else if (screenbit == 0)
		{
			c--;
			if (c < 1)
				c = 1;
			setHeight();
			uloc = glGetUniformLocation(program, "screenheight");
			if (uloc > -1)glUniform1f(uloc, screenheight);

			GLuint vertexPositionLoc = glGetAttribLocation(program, "position");
			glEnableVertexAttribArray(vertexPositionLoc);
			glBindBuffer(GL_ARRAY_BUFFER, screenBuffer);
			glVertexAttribPointer(vertexPositionLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);

			glDrawArrays(GL_QUADS, 0, 4);
		}

	}
	// now swap backbuffer with front buffer, and display it
	glutSwapBuffers();

}

void display()
{
	update();
	render();

	//setting limit of frame per second  ,  in this case, it is set 60 fps.
	int curtime = glutGet(GLUT_ELAPSED_TIME);
	int time = 0;
	while (time - curtime < 12)
	{
		time = glutGet(GLUT_ELAPSED_TIME);
	}
	dt++;
	if (dt >= (pat[curStage].gaps[pat[curStage].gaps.size()-1]))
	{
		dt = 0;
		for (int i = 0; i < 100; i++)
		{
			afterboom(i);
		}
	}
}

void reshape( int width, int height )
{
	// set current viewport in pixels
	// viewport: the window area that are affected by rendering
	// (win_x, win_y, win_width, win_height)
	glViewport( 0, 0, width, height );

	camera.aspectRatio = float(width)/float(height);

	// post signal to call display
	// this causes GL to call display() soon
	// but, we do not know exactly when dipslay() is called
	glutPostRedisplay();				
}

void mouse( int button, int state, int x, int y )
{
	
}

void motion( int x, int y )
{
	
	if (programbegin == 0)
	{
		printf("%d %d\n", x, y);
		prex = x;
		prey = y;
		programbegin = 1;
		return;
	}
	int x_ = prex-x;
	int y_ = prey-y;

	

	float radd = PI / 3600.0f;
	radx = radd * x_;

	rady = radd * y_;
	
	mat4 view = camera.viewMatrix;

	vec3 n = vec3(view._31, view._32, view._33);
	vec3 u = vec3(view._11, view._12, view._13);
	vec3 v = vec3(view._21, view._22, view._23);

	mat4 matn;
	matn._14 = n.x;
	matn._24 = n.y;
	matn._34 = n.z;
	mat4 matu;
	matu._14 = u.x;
	matu._24 = u.y;
	matu._34 = u.z;
	mat4 matv;
	matv._14 = v.x;
	matv._24 = v.y;
	matv._34 = v.z;

	if ((mat4::rotate(u, rady) * matv)._24 > 0)
	{
		matn = mat4::rotate(u, rady) * matn;
		matv = mat4::rotate(u, rady) * matv;
	}
	matn = mat4::rotate(camera.up, radx) * matn;
	matu = mat4::rotate(camera.up, radx) * matu;
	matv = mat4::rotate(camera.up, radx) * matv;


	n.x = matn._14;
	n.y = matn._24;
	n.z = matn._34;

	view._31 = n.x;
	view._32 = n.y;
	view._33 = n.z;
	view._34 = -n.dot(camera.eye);

	u.x = matu._14;
	u.y = matu._24;
	u.z = matu._34;

	view._11 = u.x;
	view._12 = u.y;
	view._13 = u.z;
	view._14 = -u.dot(camera.eye);

	v.x = matv._14;
	v.y = matv._24;
	v.z = matv._34;

	view._21 = v.x;
	view._22 = v.y;
	view._23 = v.z;
	view._24 = -v.dot(camera.eye);

	camera.n = n;
	camera.u = u;
	camera.v = v;

	camera.viewMatrix = view;
}

void idle()
{
	glutPostRedisplay();	// signal to call display soon
}

void keyboard( unsigned char key, int x, int y )
{
	if (state == MAIN){
		if (key == ' ')
		{
			state = KEYBOARD;
			g_Channel[0]->stop();
			g_System->playSound(FMOD_CHANNEL_FREE, g_Sound[0], 0, &g_Channel[0]);
		}
	}
	else if (state == KEYBOARD){
		if (key == ' ') state = GAME;
	}
	else if (state == GAME){
		if (key == 27 || key == 'q' || key == 'Q')		// ESCAPE
		{
			exit(0);
		}
	}
	//if (key == 27 || key == 'q' || key == 'Q')		// ESCAPE
	//{
	//	exit(0);
	//}
	if (key == ' ')
	{
		if (ablejump == true)
		{
			g_System->playSound(FMOD_CHANNEL_FREE, g_Sound[10], 0, &g_Channel[1]);
			ablejump = false;
			falling_speed = 8.0f;
		}
	}
	if (key == 'w')
	{
		keystate['w'] = true;
	}
	if (key == 's')
	{
		keystate['s'] = true;
	}
	if (key == 'a')
	{
		keystate['a'] = true;
	}
	if (key == 'd')
	{
		keystate['d'] = true;
	}
	if (key == 'r')
	{
		if (game == false)
		{
			game = true;
			screenbit = 0;
			g_System->playSound(FMOD_CHANNEL_FREE, g_Sound[12], 0, &g_Channel[1]);
		}
	}
}

void upkeyboard(unsigned char key, int x, int y)
{
	if (key == 'w')
	{
		keystate['w'] = false;
	}
	if (key == 's')
	{
		keystate['s'] = false;
	}
	if (key == 'a')
	{
		keystate['a'] = false;
	}
	if (key == 'd')
	{
		keystate['d'] = false;
	}
	if (key == 'r')
	{
		keystate['r'] = true;
	}
}

bool initShaders( const char* vertShaderPath, const char* fragShaderPath )
{
	// create a program before linking shaders
	program = glCreateProgram();
	glUseProgram( program );

	// compile shader sources
	GLuint vertexShader = glCreateShader( GL_VERTEX_SHADER );
	const char* vertexShaderSource = readShader( vertShaderPath ); if(vertexShaderSource==NULL) return false;
	GLint vertexShaderLength = strlen(vertexShaderSource);
	glShaderSource( vertexShader, 1, &vertexShaderSource, &vertexShaderLength );
	glCompileShader( vertexShader );
	if(!checkShader( vertexShader, "vertexShader" )){ printf( "Unable to compile vertex shader\n" ); return false; }
	
	GLuint fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );
	const char* fragmentShaderSource = readShader( fragShaderPath ); if(fragmentShaderSource==NULL) return false;
	GLint fragmentShaderLength = strlen(fragmentShaderSource);
	glShaderSource( fragmentShader, 1, &fragmentShaderSource, &fragmentShaderLength );
	glCompileShader( fragmentShader );
	if(!checkShader( fragmentShader, "fragmentShader" )){ printf( "Unable to compile fragment shader\n" ); return false; }
	
	// attach vertex/fragments shaders and link program
	glAttachShader( program, vertexShader );
	glAttachShader( program, fragmentShader );
	glLinkProgram( program );
	if(!checkProgram( program, "program" )){ printf( "Unable to link program\n" ); return false; }



	// deallocate string
	free((void*)vertexShaderSource);
	free((void*)fragmentShaderSource);

	return true;
}

bool userInit()
{
	// init GL states
	glClearColor(1.0f,1.0f,1.0f,1.0f);		// clear color for glClear()
	glEnable( GL_CULL_FACE );					// turn on backface culling
	glEnable( GL_DEPTH_TEST );					// turn on depth tests
	glShadeModel( GL_SMOOTH );
	imageInit();
	
	vec3 square[] = {
		vec3(200.0f, -100.0f, 0.0f),
		vec3(0.0f, -100.0f, 0.0f),
		vec3(0.0f, -100.0f, 200.0f),
		vec3(200.0f, -100.0f, 200.0f),
		vec3(200.0f, -400.0f, 0.0f),
		vec3(0.0f, -400.0f, 0.0f),
		vec3(0.0f, -400.0f, 200.0f),
		vec3(200.0f, -400.0f, 200.0f)
	};

	vec3 spike[] = {	// 18 vertices
		vec3(200.0f, -300.0f, 0.0f), vec3(200.0f, -300.0f, 200.0f), vec3(0.0f, -300.0f, 200.0f),
		vec3(200.0f, -300.0f, 0.0f), vec3(0.0f, -300.0f, 200.0f), vec3(0.0f, -300.0f, 0.0f),
		vec3(100.0f, -120.0f, 100.0f), vec3(0.0f, -300.0f, 200.0f), vec3(200.0f, -300.0f, 200.0f),
		vec3(100.0f, -120.0f, 100.0f), vec3(200.0f, -300.0f, 200.0f), vec3(200.0f, -300.0f, 0.0f),
		vec3(100.0f, -120.0f, 100.0f), vec3(200.0f, -300.0f, 0.0f), vec3(0.0f, -300.0f, 0.0f),
		vec3(100.0f, -120.0f, 100.0f), vec3(0.0f, -300.0f, 0.0f), vec3(0.0f, -300.0f, 200.0f)
	};

	glGenBuffers(1, &spikebuffer);
	glBindBuffer(GL_ARRAY_BUFFER, spikebuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(spike), spike, GL_STATIC_DRAW);

	glGenBuffers(1, &vectexSquareBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vectexSquareBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(square), square, GL_STATIC_DRAW);

	GLuint IBS[] = { 0, 1, 2, 3, 0, 4, 5, 1, 1, 5, 6, 2, 3, 2, 6, 7, 0, 3, 7, 4, 5, 4, 7, 6 };

	glGenBuffers(1, &indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(IBS), IBS, GL_STATIC_DRAW);

	pat[0] = loadPat("./patterns/stage1.pattern");
	pat[1] = loadPat("./patterns/stage2.pattern");
	pat[2] = loadPat("./patterns/stage3.pattern");
	pat[3] = loadPat("./patterns/stage4.pattern");
	pat[4] = loadPat("./patterns/stage5.pattern");
	pat[5] = loadPat("./patterns/stage6.pattern");
	pat[6] = loadPat("./patterns/stage7.pattern");
	pat[7] = loadPat("./patterns/stage8.pattern");
	pat[8] = loadPat("./patterns/stage9.pattern");
	pat[9] = loadPat("./patterns/stage10.pattern");

	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10; j++)
		{
			box[10 * i + j].index = 10 * i + j;
			box[10 * i + j].position = vec3((float)(j * 200), 0.0f, (float)(i * 200));
			box[10 * i + j].visible = false;
			box[10 * i + j].boom = false;

			box[10 * i + j].color = vec4(1.0f, i * 0.1f, j * 0.1f, 1.0f);
			box[10 * i + j].boomAni = false;
			box[10 * i + j].bloom = false;
			spikes[10 * i + j].position = box[10 * i + j].position;
			spikes[10 * i + j].color = vec4(0.0f, j*0.1f, i*0.1f, 1.0f);
			spikes[10 * i + j].mov = 0.0f;
			spikes[10 * i + j].visible = false;
		}
		
	}
	float rad = 10000.0f;
	vec3 circleVertices[73 * 37 + 2];
	circleVertices[0] = vec3(0, 0, rad);

	float num;
	for (int j = 0; j < 37; j++)
	{
		float z = rad * cos((PI / 180) * 9 * (j + 1));
		float radd = sqrt(rad*rad - pow(z, 2));
		for (int i = 0; i < 73; i++)
		{
			num = (float)(5 * i * PI) / 180;
			circleVertices[(73 * j) + i + 1] = vec3((radd * cos(num)), (radd * sin(num)), z);
		}
	}
	circleVertices[73 * 37 + 1] = vec3(0, 0, -rad);

	glGenBuffers(1, &vertexCircleBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexCircleBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(circleVertices), circleVertices, GL_STATIC_DRAW);


	vec3 screenVertices[] = {
		vec3(1.0f, 1.0f, 0.0f), vec3(-1.0f, 1.0f, 0.0f),
		vec3(-1.0f, -1.0f, 0.0f), vec3(1.0f, -1.0f, 0.0f)
	};
	glGenBuffers(1, &screenBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, screenBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(screenVertices), screenVertices, GL_STATIC_DRAW);

	vec3 screenWords[] = {
		//P
		vec3(-0.75f, 0.75f, 0.0f), vec3(-0.75f, 0.65f, 0.0f), vec3(-0.75f, 0.55f, 0.0f), vec3(-0.75f, 0.45f, 0.0f), vec3(-0.75f, 0.35f, 0.0f), vec3(-0.75f, 0.25f, 0.0f), vec3(-0.75f, 0.15f, 0.0f),
		vec3(-0.70f, 0.75f, 0.0f), vec3(-0.65f, 0.70f, 0.0f), vec3(-0.60f, 0.60f, 0.0f), vec3(-0.65f, 0.50f, 0.0f), vec3(-0.70f, 0.45f, 0.0f),
		//r
		vec3(-0.55f, 0.15f, 0.0f), vec3(-0.55f, 0.25f, 0.0f), vec3(-0.55f, 0.35f, 0.0f), vec3(-0.55f, 0.45f, 0.0f), vec3(-0.50f, 0.35f, 0.0f), vec3(-0.45f, 0.40f, 0.0f),
		//e
		vec3(-0.35f, 0.30f, 0.0f), vec3(-0.30f, 0.30f, 0.0f), vec3(-0.25f, 0.30f, 0.0f), vec3(-0.25f, 0.40f, 0.0f), vec3(-0.30f, 0.48f, 0.0f), vec3(-0.35f, 0.40f, 0.0f),
		vec3(-0.35f, 0.20f, 0.0f), vec3(-0.30f, 0.14f, 0.0f), vec3(-0.25f, 0.15f, 0.0f),
		//s
		vec3(-0.15f, 0.45f, 0.0f), vec3(-0.10f, 0.47f, 0.0f), vec3(-0.05f, 0.45f, 0.0f),
		vec3(-0.13f, 0.35f, 0.0f), vec3(-0.07f, 0.25f, 0.0f),
		vec3(-0.15f, 0.15f, 0.0f), vec3(-0.10f, 0.13f, 0.0f), vec3(-0.05f, 0.15f, 0.0f),
		//s
		vec3(0.05f, 0.45f, 0.0f), vec3(0.10f, 0.47f, 0.0f), vec3(0.15f, 0.45f, 0.0f),
		vec3(0.07f, 0.35f, 0.0f), vec3(0.12f, 0.25f, 0.0f),
		vec3(0.05f, 0.15f, 0.0f), vec3(0.10f, 0.13f, 0.0f), vec3(0.15f, 0.15f, 0.0f),
		//'R'
		vec3(0.35f, 0.75f, 0.0f), vec3(0.85f, 0.75f, 0.0f),
		vec3(0.50f, 0.65f, 0.0f), vec3(0.50f, 0.55f, 0.0f), vec3(0.50f, 0.45f, 0.0f), vec3(0.50f, 0.35f, 0.0f), vec3(0.50f, 0.25f, 0.0f), vec3(0.50f, 0.15f, 0.0f),
		vec3(0.55f, 0.65f, 0.0f), vec3(0.60f, 0.65f, 0.0f), vec3(0.65f, 0.65f, 0.0f), vec3(0.70f, 0.60f, 0.0f), vec3(0.70f, 0.50f, 0.0f),
		vec3(0.55f, 0.45f, 0.0f), vec3(0.60f, 0.45f, 0.0f), vec3(0.65f, 0.45f, 0.0f),
		vec3(0.69f, 0.35f, 0.0f), vec3(0.70f, 0.25f, 0.0f), vec3(0.70f, 0.15f,0.0f)

	};
	glGenBuffers(1, &screenWordsbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, screenWordsbuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(screenWords), screenWords, GL_STATIC_DRAW);

	// init camera
	
	camera.eye = vec3( 50, 200, 50);
	camera.at = vec3( 1000, 200, 1000);
	camera.up = vec3( 0, 1, 0 );
	camera.viewMatrix = mat4::lookAt( camera.eye, camera.at, camera.up );

	vec3 n = vec3(camera.viewMatrix._31, camera.viewMatrix._32, camera.viewMatrix._33);
	vec3 u = vec3(camera.viewMatrix._11, camera.viewMatrix._12, camera.viewMatrix._13);
	vec3 v = vec3(camera.viewMatrix._21, camera.viewMatrix._22, camera.viewMatrix._23);
	camera.n = n;
	camera.u = u;
	camera.v = v;

	camera.aspectRatio = (float)windowWidth / (float)windowHeight;


	for (int i = 0; i < 256; i++)
	{
		keystate[i] = false;
	}


	inv1 = loadObj("./mesh/Space_Invader2.obj", "./mesh/Space_Invader.obj");
	inv2 = loadObj("./mesh/Space_Invader.obj", "./mesh/Space_Invader.obj");
	inv3 = loadObj("./mesh/Space_Invader3.obj", "./mesh/Space_Invader.obj");

	for (unsigned int i = 0; i < inv1.vertices.size(); i++){		// 크기가 너무 작아서 우선 보려고 100배 해놨음
		inv1.vertices[i].z *= -(1.0f);
		inv1.vertices[i].z += 400.0f;
		inv1.vertices[i] *= 5.0f;

		inv[0].pos1 = vec3(0.0f,0.0f,-1500.0f);
	}
	for (unsigned int i = 0; i < inv2.vertices.size(); i++){
		inv2.vertices[i].z *= -(1.0f);
		inv2.vertices[i].z += 400.0f;
		inv2.vertices[i].x -= 200.0f;
		inv2.vertices[i] *= 5.0f;

		inv[1].pos1 = vec3(-200.0f, 0.0f, 400.0f);
	}
	for (unsigned int i = 0; i < inv3.vertices.size(); i++){
		inv3.vertices[i].z *= -(1.0f);
		inv3.vertices[i].z += 400.0f;
		inv3.vertices[i].x -= 400.0f;
		inv3.vertices[i] *= 5.0f;

		inv[2].pos1 = vec3(-400.0f, 0.0f, 400.0f);
	}

	glGenBuffers(1, &inv1.vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, inv1.vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, inv1.vertices.size() * sizeof(vec3), inv1.vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &inv2.vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, inv2.vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, inv2.vertices.size() * sizeof(vec3), inv2.vertices[0], GL_STATIC_DRAW);

	glGenBuffers(1, &inv3.vertexbuffer);
	glBindBuffer(GL_ARRAY_BUFFER, inv3.vertexbuffer);
	glBufferData(GL_ARRAY_BUFFER, inv3.vertices.size() * sizeof(vec3), inv3.vertices[0], GL_STATIC_DRAW);

	return true;
}

int main( int argc, char* argv[] )
{
	// GLUT initialization
	glutInit( &argc, argv );
	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGBA );

	int screenWidth		= glutGet(GLUT_SCREEN_WIDTH);
	int screenHeight	= glutGet(GLUT_SCREEN_HEIGHT);
	
	FMOD::System_Create(&g_System);
	g_System->init(5, FMOD_INIT_NORMAL,0);

	g_System->createStream("../bin/sounds/1.wav", FMOD_SOFTWARE | FMOD_CREATESAMPLE | FMOD_LOOP_NORMAL, 0, &g_Sound[0]);
	g_System->createStream("../bin/sounds/2.wav", FMOD_SOFTWARE | FMOD_CREATESAMPLE | FMOD_LOOP_NORMAL, 0, &g_Sound[1]);
	g_System->createStream("../bin/sounds/3.wav", FMOD_SOFTWARE | FMOD_CREATESAMPLE | FMOD_LOOP_NORMAL, 0, &g_Sound[2]);
	g_System->createStream("../bin/sounds/4.wav", FMOD_SOFTWARE | FMOD_CREATESAMPLE | FMOD_LOOP_NORMAL, 0, &g_Sound[3]);
	g_System->createStream("../bin/sounds/5.wav", FMOD_SOFTWARE | FMOD_CREATESAMPLE | FMOD_LOOP_NORMAL, 0, &g_Sound[4]);
	g_System->createStream("../bin/sounds/6.wav", FMOD_SOFTWARE | FMOD_CREATESAMPLE | FMOD_LOOP_NORMAL, 0, &g_Sound[5]);
	g_System->createStream("../bin/sounds/7.wav", FMOD_SOFTWARE | FMOD_CREATESAMPLE | FMOD_LOOP_NORMAL, 0, &g_Sound[6]);
	g_System->createStream("../bin/sounds/8.wav", FMOD_SOFTWARE | FMOD_CREATESAMPLE | FMOD_LOOP_NORMAL, 0, &g_Sound[7]);
	g_System->createStream("../bin/sounds/9.wav", FMOD_SOFTWARE | FMOD_CREATESAMPLE | FMOD_LOOP_NORMAL, 0, &g_Sound[8]);
	g_System->createStream("../bin/sounds/10.wav", FMOD_SOFTWARE | FMOD_CREATESAMPLE | FMOD_LOOP_NORMAL, 0, &g_Sound[9]);
	g_System->createSound("../bin/sounds/walk.mp3", FMOD_SOFTWARE | FMOD_CREATESAMPLE, 0, &g_Sound[10]);
	g_System->createSound("../bin/sounds/scream.mp3", FMOD_SOFTWARE | FMOD_CREATESAMPLE, 0, &g_Sound[11]);
	g_System->createSound("../bin/sounds/spawn.mp3", FMOD_SOFTWARE | FMOD_CREATESAMPLE, 0, &g_Sound[12]);
	g_System->createSound("../bin/sounds/iwaaaa.mp3", FMOD_SOFTWARE | FMOD_CREATESAMPLE, 0, &g_Sound[13]);
	g_System->createSound("../bin/sounds/spike.mp3", FMOD_SOFTWARE | FMOD_CREATESAMPLE, 0, &g_Sound[14]);
	g_System->createSound("../bin/sounds/clear.wav", FMOD_SOFTWARE | FMOD_CREATESAMPLE, 0, &g_Sound[15]);
	g_System->createStream("../bin/sounds/opening.wav", FMOD_SOFTWARE | FMOD_CREATESAMPLE | FMOD_LOOP_NORMAL, 0, &g_Sound[16]);

	g_System->playSound(FMOD_CHANNEL_FREE, g_Sound[16], 0, &g_Channel[0]);
	glutInitWindowSize( windowWidth, windowHeight );
	glutInitWindowPosition( 0, 0 );
	glutCreateWindow( "ESCAPER" );


	// Register callbacks
	glutDisplayFunc( display );
	glutReshapeFunc( reshape );		// callback when the window is resized
	glutKeyboardFunc( keyboard );
	glutKeyboardUpFunc(upkeyboard);
	glutMouseFunc( mouse );
	glutMotionFunc(motion);
	glutPassiveMotionFunc( motion );		// callback when the mouse is moving
	glutIdleFunc( idle );			// idle-time callback

	// init and check GLEW, version, extensions
	if(!initExtensions()){ printf( "Failed to init extensions.\n" ); return 0; }
	
	// create and compile shaders/program
	if(!initShaders("shaders/transform.vert","shaders/transform.frag")){ printf( "Failed to init program and shaders\n" ); return 0; }

	// user initialization
	if(!userInit()){ printf( "Failed to userInit()\n" ); return 0; }

	SetCursorPos(windowWidth / 2, windowHeight / 2);
	// Start rendering loop
	glutMainLoop();
	return 0;
}
