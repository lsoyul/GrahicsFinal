GLuint	vertexPositionBuffer = 0;		// ID holder for vertex position buffer
GLuint	vertexPositionBuffer2 = 0;

GLuint	textureObject = 0;
GLuint	textureObject2 = 0;
unsigned char* image_1 = nullptr;
unsigned char* image_2 = nullptr;
int width, height, comp;
int width2, height2, comp2;
vertex VBA[4], VBA2[4];
extern "C" unsigned char*	stbi_load(const char* filename, int* x, int* y, int* comp, int req_comp);
extern "C" void				stbi_image_free(void* retval_from_stbi_load);

void imageInit(){
	unsigned char* image0 = stbi_load("../bin/images/MAIN.png", &width, &height, &comp, 0);
	image_1 = (unsigned char*)malloc(sizeof(unsigned char)*width*height*comp);
	for (int y = 0, stride = width*comp; y < height; y++) memcpy(image_1 + (height - 1 - y)*stride, image0 + y*stride, stride); // vertical flip
	stbi_image_free(image0); // release the original image

	unsigned  char * image_0 = stbi_load("../bin/images/keyboard.png", &width2, &height2, &comp2, 0);
	image_2 = (unsigned char*)malloc(sizeof(unsigned char)*width2*height2*comp2);
	for (int y = 0, stride = width2*comp2; y < height2; y++) memcpy(image_2 + (height2 - 1 - y)*stride, image_0 + y*stride, stride); // vertical flip
	stbi_image_free(image_0);
	VBA[0].pos = vec3(-1.0f, -1.0f, 0.0f); 	VBA[0].norm = vec3(0.0f, 0.0f, 1.0f);	VBA[0].tex = vec2(0.0f, 0.0f);
	VBA[1].pos = vec3(1.0f, -1.0f, 0.0f);		VBA[1].norm = vec3(0.0f, 0.0f, 1.0f);	VBA[1].tex = vec2(1.0f, 0.0f);
	VBA[2].pos = vec3(1.0f, 1.0f, 0.0f);		VBA[2].norm = vec3(0.0f, 0.0f, 1.0f);	VBA[2].tex = vec2(1.0f, 1.0f);
	VBA[3].pos = vec3(-1.0f, 1.0f, 0.0f);		VBA[3].norm = vec3(0.0f, 0.0f, 1.0f);	VBA[3].tex = vec2(0.0f, 1.0f);


	glGenBuffers(1, &vertexPositionBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexPositionBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VBA), VBA, GL_STATIC_DRAW);


	// create textures
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);

	glGenTextures(1, &textureObject);
	glBindTexture(GL_TEXTURE_2D, textureObject);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8 /* GL_RGB for legacy GL */, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image_1);


	// allocate and create mipmap
	for (int k = 1, w = width >> 1, h = height >> 1; k < 9; k++, w = w >> 1, h = h >> 1)
		glTexImage2D(GL_TEXTURE_2D, k, GL_RGB8 /* GL_RGB for legacy GL */, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	free(image_1);

	VBA2[0].pos = vec3(-1.0f, -1.0f, 0.0f); 	VBA2[0].norm = vec3(0.0f, 0.0f, 1.0f);	VBA2[0].tex = vec2(0.0f, 0.0f);
	VBA2[1].pos = vec3(1.0f, -1.0f, 0.0f);		VBA2[1].norm = vec3(0.0f, 0.0f, 1.0f);	VBA2[1].tex = vec2(1.0f, 0.0f);
	VBA2[2].pos = vec3(1.0f, 1.0f, 0.0f);		VBA2[2].norm = vec3(0.0f, 0.0f, 1.0f);	VBA2[2].tex = vec2(1.0f, 1.0f);
	VBA2[3].pos = vec3(-1.0f, 1.0f, 0.0f);		VBA2[3].norm = vec3(0.0f, 0.0f, 1.0f);	VBA2[3].tex = vec2(0.0f, 1.0f);

	glGenBuffers(1, &vertexPositionBuffer2);
	glBindBuffer(GL_ARRAY_BUFFER, vertexPositionBuffer2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(VBA2), VBA2, GL_STATIC_DRAW);


	// create textures
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);

	glGenTextures(1, &textureObject2);
	glBindTexture(GL_TEXTURE_2D, textureObject2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8 /* GL_RGB for legacy GL */, width2, height2, 0, GL_RGB, GL_UNSIGNED_BYTE, image_2);


	// allocate and create mipmap
	for (int k = 1, w = width2 >> 1, h = height2 >> 1; k < 9; k++, w = w >> 1, h = h >> 1)
		glTexImage2D(GL_TEXTURE_2D, k, GL_RGB8 /* GL_RGB for legacy GL */, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	//free(image_2);
}

void render_menu()
{
	glUniform1i(glGetUniformLocation(program, "isMenu"), 1);
	// clear screen (with background color) and clear depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// notify to GL that we like to use our program now
	glUseProgram(program);

	// bind vertex position buffer
	glBindBuffer(GL_ARRAY_BUFFER, vertexPositionBuffer);

	// bind vertex position buffer
	GLuint vertexPositionLoc = glGetAttribLocation(program, "position");
	glEnableVertexAttribArray(vertexPositionLoc);
	glVertexAttribPointer(vertexPositionLoc, sizeof(vertex().pos) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(vertex), 0);

	// bind vertex normal buffer
	GLuint vertexNormalLoc = glGetAttribLocation(program, "normal");
	glEnableVertexAttribArray(vertexNormalLoc);
	glVertexAttribPointer(vertexNormalLoc, sizeof(vertex().norm) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)(sizeof(vertex().pos)));

	// bind vertex texture buffer
	GLuint vertexTexlLoc = glGetAttribLocation(program, "texcoord");
	glEnableVertexAttribArray(vertexTexlLoc);
	glVertexAttribPointer(vertexTexlLoc, sizeof(vertex().tex) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)(sizeof(vertex().pos) + sizeof(vertex().norm)));

	// render vertices: trigger shader programs to process vertex data
	glDrawArrays(GL_QUADS, 0, 4);
	glBindTexture(GL_TEXTURE_2D, textureObject);

	glUniform1i(glGetUniformLocation(program, "TEX"), 0);



}
void render_keyboard(){
	glUseProgram(program);
	GLint uloc = glGetUniformLocation(program, "isMenu");

	if (uloc > -1) glUniform1i(uloc, 1);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindBuffer(GL_ARRAY_BUFFER, vertexPositionBuffer2);

	// bind vertex position buffer
	GLuint vertexPositionLoc = glGetAttribLocation(program, "position");
	glEnableVertexAttribArray(vertexPositionLoc);
	glVertexAttribPointer(vertexPositionLoc, sizeof(vertex().pos) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(vertex), 0);

	// bind vertex normal buffer
	GLuint vertexNormalLoc = glGetAttribLocation(program, "normal");
	glEnableVertexAttribArray(vertexNormalLoc);
	glVertexAttribPointer(vertexNormalLoc, sizeof(vertex().norm) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)(sizeof(vertex().pos)));

	// bind vertex texture buffer
	GLuint vertexTexlLoc = glGetAttribLocation(program, "texcoord");
	glEnableVertexAttribArray(vertexTexlLoc);
	glVertexAttribPointer(vertexTexlLoc, sizeof(vertex().tex) / sizeof(GLfloat), GL_FLOAT, GL_FALSE, sizeof(vertex), (GLvoid*)(sizeof(vertex().pos) + sizeof(vertex().norm)));

	// render vertices: trigger shader programs to process vertex data
	glDrawArrays(GL_QUADS, 0, 4);
	glBindTexture(GL_TEXTURE_2D, textureObject2);
	glUniform1i(glGetUniformLocation(program, "TEX"), 0);

}