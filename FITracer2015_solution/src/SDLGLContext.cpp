/*
	Name: SDL_Toolbox.cpp
	Desc: Context and functions for App.
	Author: Karel Brezina (xbrezi13)
*/

#include "SDLGLContext.h"
#include <fstream>
#include <string>

// Returns pointer with additional offset.
#define p_OffsetInVBO(off) ((void*)(off))

// Initialize.
// width -> screen width.
// height -> screen height.
// @return -> success of init.
bool SDLGLContext::Init(int width, int height, int _resultWidth, int _resultHeight)
{	
	screenWidth = width;
	screenHeight = height;
	resultWidth = _resultWidth;
	resultHeight = _resultHeight;

	output.setWidth(resultWidth);
	output.setHeight(resultHeight);
	output.init();
	// Init VBO,VAO,IBO etc.
	InitGL();
	// Create SDL semaphore for thread sync.
	mainSemaphore = SDL_CreateSemaphore(1);
	if (mainSemaphore == NULL) {
		fprintf(stderr, "SDL error: SDL_CreateSemaphore failed\n");
		return false;
	}
	loadPixelsSemaphore = SDL_CreateSemaphore(1);
	if (mainSemaphore == NULL) {
		fprintf(stderr, "SDL error: SDL_CreateSemaphore failed\n");
		return false;
	}
	updateTextureSemaphore = SDL_CreateSemaphore(1);
	if (mainSemaphore == NULL) {
		fprintf(stderr, "SDL error: SDL_CreateSemaphore failed\n");
		return false;
	}

	// Init SDL tool for image loading.
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		fprintf(stderr, "SDL error: IMG_Init failed\n");
		return false;
	}

	//Init SDL tool for text rendering.
	if (TTF_Init() == -1) {
		fprintf(stderr, "SDL error: TTF_Init failed\n");
		return false;
	}

	// Load font.
	mainFont = TTF_OpenFont("./fonts/corbel.ttf", 20);
	if (mainFont == NULL)
	{
		fprintf(stderr, "SDL error: TTF_OpenFont failed (wrong file path?)\n");
		return false;
	}
	
	return true;
}

// Init all OpenGL stuff.
void SDLGLContext::InitGL()
{
	glGenBuffers(4, VBO);
	glGenBuffers(4, IBO);
	glGenVertexArrays(4, VAO);
	glGenTextures(2, resultTexture);
	glGenTextures(2, animTextures);

	pixels = new GLfloat[resultWidth * resultHeight * 4];

	int index = 0;
	for (unsigned int x = 0; x < resultHeight; x++)
		for (unsigned int y = 0; y < resultWidth; y++) {
			pixels[index] = 0.f;
			pixels[index + 1] = 0.f;
			pixels[index + 2] = 0.f;
			pixels[index + 3] = 1.f;
			index += 4;
		}

	glBindTexture(GL_TEXTURE_2D, resultTexture[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, resultWidth, resultHeight, 0,
		GL_RGBA, GL_FLOAT, pixels);

	glBindTexture(GL_TEXTURE_2D, resultTexture[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, resultWidth, resultHeight, 0,
		GL_RGBA, GL_FLOAT, pixels);
	glBindTexture(GL_TEXTURE_2D, 0);

	shaderProgram = CompileShaders("shaders/VShader.glsl", "shaders/FShader.glsl");

	// Locate vars in shaders.
	glBindAttribLocation(shaderProgram, 0, "in_Position");
	glBindAttribLocation(shaderProgram, 1, "in_Texcoord");
	glBindFragDataLocation(shaderProgram, 0, "out_Color");
	// Locate uniform vars.
	samplerUniform = glGetUniformLocation(shaderProgram, "uni_Sampler");
	isBorderUniform = glGetUniformLocation(shaderProgram, "uni_isBorder");
	colorUniform = glGetUniformLocation(shaderProgram, "uni_Color");
	mixTexCol = glGetUniformLocation(shaderProgram, "uni_MixTexCol");
	// Init uniform vars.
	glUniform1i(samplerUniform, 0);
	glUniform1i(isBorderUniform, false);
	glUniform4f(colorUniform, 0.f, 0.f, 0.f, 1.f);
	glUniform1f(mixTexCol, 0.f);
	// Turn on use of our shaders.
	glUseProgram(shaderProgram);
}

// Add new button.
void SDLGLContext::AddButton(GLButton* button)
{
	listButtons.push_back(button);
	objectsCnt++;
}

// Add new text.
void SDLGLContext::AddText(GLText* text) 
{
	listTexts.push_back(text);
	objectsCnt++;
}

// Create Vtx and so on of all objects.
void SDLGLContext::PrepareScene()
{
	PrepareButtons();
	PrepareTexts();
	PrepareLines();
	PrepareTextures();
}

// Load buttons Vtx to VBO.
void SDLGLContext::PrepareButtons()
{
	// For get button Vtx.
	GLVertexData* vertices;
	// 6 * number of buttons
	GLuint IBO_triangles[] = { 
		0, 1, 2, 2, 3, 0, // Button CPU render
		4, 5, 6, 6, 7, 4, // Button GPU render
		8, 9, 10, 10, 11, 8, // Button List
		12, 13, 14, 14, 15, 12, // Button Octree
		16, 17, 18, 18, 19, 16, // Button Uniform grid
		20, 21, 22, 22, 23, 20, // Button BVH
		24, 25, 26, 26, 27, 24, // Button Pause
		28, 29, 30, 30, 31, 28, // Button Save
		
		32, 33, 34, 34, 35, 32, // Samples
		36, 37, 38, 38, 39, 36, // Choose rendering mode
		40, 41, 42, 42, 43, 40, // Choose accelerate structure
		44, 45, 46, 46, 47, 44, // Rendering time 
		48, 49, 50, 50, 51, 48, // Progress of computation
		
		52, 53, 54, 54, 55, 52, // time of render
		56, 57, 58, 58, 59, 56  // samples 
	};

	GLuint IBO_lines[] = {
		0, 1, 1, 2, 2, 3, 3, 0, // Button CPU render
		4, 5, 5, 6, 6, 7, 7, 4, // Button GPU render
		8, 9, 9, 10, 10, 11, 11, 8, // Button List
		12, 13, 13, 14, 14, 15, 15, 12, // Button Octree
		16, 17, 17, 18, 18, 19, 19, 16, // Button Uniform grid
		20, 21, 21, 22, 22, 23, 23, 20, // Button BVH
		24, 25, 25, 26, 26, 27, 27, 24, // Button Pause
		28, 29, 29, 30, 30, 31, 31, 28  // Button Save
	};

	// Buttons Vtx.
	std::vector<GLVertexData> VBO_button_vtx;

	for (std::vector<GLButton*>::iterator it = listButtons.begin(); it != listButtons.end(); it++) {
		vertices = (*it)->GetVertices(screenWidth, screenHeight);
		VBO_button_vtx.push_back(vertices[0]);
		VBO_button_vtx.push_back(vertices[1]);
		VBO_button_vtx.push_back(vertices[2]);
		VBO_button_vtx.push_back(vertices[3]);
		delete [] vertices;
	}

	// Button's texts Vtx (need smaller Vtx than Button's Vtx).
	std::vector<GLVertexData> VBO_text_vtx;

	for (std::vector<GLButton*>::iterator it = listButtons.begin(); it != listButtons.end(); it++) {
		vertices = (*it)->GetTextVertices(screenWidth, screenHeight);
		VBO_text_vtx.push_back(vertices[0]);
		VBO_text_vtx.push_back(vertices[1]);
		VBO_text_vtx.push_back(vertices[2]);
		VBO_text_vtx.push_back(vertices[3]);
		delete[] vertices;
	}

	for (std::vector<GLText*>::iterator it = listTexts.begin(); it != listTexts.end(); it++) {
		vertices = (*it)->GetVertices(screenWidth, screenHeight);
		VBO_text_vtx.push_back(vertices[0]);
		VBO_text_vtx.push_back(vertices[1]);
		VBO_text_vtx.push_back(vertices[2]);
		VBO_text_vtx.push_back(vertices[3]);
		delete[] vertices;
	}

	// Button's Vtx VBO.
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, VBO_button_vtx.size() * sizeof(GLVertexData), VBO_button_vtx.data(), GL_STATIC_DRAW);
	// Button's texts Vtx VBO.
	glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ARRAY_BUFFER, VBO_text_vtx.size() * sizeof(GLVertexData), VBO_text_vtx.data(), GL_STATIC_DRAW);
	// Indices for buttons Vtx.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 8 * 8 * sizeof(GLuint), IBO_lines, GL_STATIC_DRAW);
	// Indices for button's texts Vtx.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * 15 * sizeof(GLuint), IBO_triangles, GL_STATIC_DRAW);

	// Set VAOs routine.
	glBindVertexArray(VAO[0]);
	{
		// Set data source.
		glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
		glEnableVertexAttribArray(0);
		// Load vertex data.
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLVertexData), (GLvoid*)offsetof(GLVertexData, vertex));
		glEnableVertexAttribArray(1);
		// Load texture coordinate data.
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLVertexData), (GLvoid*)offsetof(GLVertexData, texCoord));
		// Set indices source.
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO[0]);
	}
	// End of set routine.
	glBindVertexArray(0);

	glBindVertexArray(VAO[1]);
	{
		// Set data source.
		glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
		glEnableVertexAttribArray(0);
		// Load vertex data.
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLVertexData), (GLvoid*)offsetof(GLVertexData, vertex));
		glEnableVertexAttribArray(1);
		// Load texture coordinate data.
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLVertexData), (GLvoid*)offsetof(GLVertexData, texCoord));
		// Set indices source.
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO[0]);
	}
	// End of set routine.
	glBindVertexArray(0);
}

// Render text and save as OpenGL texture.
void SDLGLContext::PrepareTexts()
{
	// Amount of needed texture.
	unsigned int size = listTexts.size() + listButtons.size();
	textures = new GLuint[size];
	glGenTextures(size, textures);
	// Color of render text.
	SDL_Color black; black.r = 0x00; black.g = 0x00; black.b = 0x00; black.a = 0xFF;
	SDL_Surface* surfaceMsg;
	int textureCnt = 0;

	// Render text of all buttons.
	for (std::vector<GLButton*>::iterator it = listButtons.begin(); it != listButtons.end(); it++) {
		surfaceMsg = TTF_RenderText_Blended(mainFont, (*it)->GetText().c_str(), black);
		glBindTexture(GL_TEXTURE_2D, textures[textureCnt]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surfaceMsg->w, surfaceMsg->h, 0, 
					 GL_RGBA, GL_UNSIGNED_BYTE, surfaceMsg->pixels);
		textureCnt++;
		SDL_FreeSurface(surfaceMsg);
	}

	for (std::vector<GLText*>::iterator it = listTexts.begin(); it != listTexts.end(); it++) {
		surfaceMsg = TTF_RenderText_Blended(mainFont, (*it)->GetText().c_str(), black);
		glBindTexture(GL_TEXTURE_2D, textures[textureCnt]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surfaceMsg->w, surfaceMsg->h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, surfaceMsg->pixels);
		textureCnt++;
		SDL_FreeSurface(surfaceMsg);
	}
}

// Create all decoration as lines, points and borders.
void SDLGLContext::PrepareLines()
{
	// Easy to move all decorations.
	float wpadding = 30.f;
	float hpadding = 0.f;

	// Indices for windows border.
	GLuint indices[] = {
		16, 17, 18, 16, 18, 19, // Left down animation.
		20, 21, 22, 20, 22, 23 // Big window for pt.
	};

	// Indices for animation window.
	GLuint animIndices[] = {
		0, 1, 2, 1, 2, 3, // Cornell box image.
		2, 3, 4, 2, 4, 5 // Black image.
	};
	// Template for textures. Only for protect uninit data. 
	GLTexCoord linesTex = { 0.f, 0.f };
	// For set our Vtx.
	GLVertexData oneVertex;
	GLVertexData saveVertex;
	// Set init values.
	oneVertex.texCoord = linesTex;
	
	// All decoration.
	std::vector<GLVertexData> VBO_lines;
	// Only for animation window.
	std::vector<GLVertexData> VBO_animVtx;

	// Left upper lines. Choose rendering mode.
	GLfloat vert1[] =
	{
		80 + wpadding, screenHeight - 50 + hpadding,
		30 + wpadding, screenHeight - 50 + hpadding,
		30 + wpadding, screenHeight - 140 + hpadding,
		80 + wpadding, screenHeight - 140 + hpadding,
		100 + wpadding, screenHeight - 50 + hpadding,
		150 + wpadding, screenHeight - 50 + hpadding,
		150 + wpadding, screenHeight - 140 + hpadding,
		100 + wpadding, screenHeight - 140 + hpadding
	};

	// Left middle lines. Choose accele...
	GLfloat vert2[] =
	{
		80 + wpadding, screenHeight - 200 + hpadding,
		30 + wpadding, screenHeight - 200 + hpadding,
		30 + wpadding, screenHeight - 370 + hpadding,
		80 + wpadding, screenHeight - 370 + hpadding,
		100 + wpadding, screenHeight - 200 + hpadding,
		150 + wpadding, screenHeight - 200 + hpadding,
		150 + wpadding, screenHeight - 370 + hpadding,
		100 + wpadding, screenHeight - 370 + hpadding
	};

	// Animation window.
	GLfloat vert3[] =
	{
		20, 171,
		20, 20,
		220, 20,
		220, 171
	};

	// Result window.
	GLfloat vert4[] = 
	{ 
		screenWidth - 541.f, screenHeight - 70.f,
		screenWidth - 541.f, screenHeight - 571.f,
		screenWidth - 40.f, screenHeight - 571.f,
		screenWidth - 40.f, screenHeight - 70.f
	};

	// Points in the end of lines.
	GLfloat points[] =
	{
		80 + wpadding, screenHeight - 50 + hpadding,
		80 + wpadding, screenHeight - 140 + hpadding,
		100 + wpadding, screenHeight - 50 + hpadding,
		100 + wpadding, screenHeight - 140 + hpadding,
		80 + wpadding, screenHeight - 200 + hpadding,
		80 + wpadding, screenHeight - 370 + hpadding,
		100 + wpadding, screenHeight - 200 + hpadding,
		100 + wpadding, screenHeight - 370 + hpadding
	};

	// Map Vtx to OpenGL viewport.
	unsigned int halfWidth = screenWidth / 2;
	unsigned int halfHeight = screenHeight / 2;
	
	// Up lines.
	for (int i = 0; i < 16; i += 2) {
		oneVertex.vertex.x = (vert1[i] / halfWidth) - 1.0f;
		oneVertex.vertex.y = (vert1[i + 1] / halfHeight) - 1.0f;
		VBO_lines.push_back(oneVertex);
	}

	// Middle lines.
	for (int i = 0; i < 16; i += 2) {
		oneVertex.vertex.x = (vert2[i] / halfWidth) - 1.0f;
		oneVertex.vertex.y = (vert2[i + 1] / halfHeight) - 1.0f;
		VBO_lines.push_back(oneVertex);
	}

	// Animation window.
	// Left up corner.
	oneVertex.vertex.x = (vert3[0] / halfWidth) - 1.0f;
	oneVertex.vertex.y = (vert3[1] / halfHeight) - 1.0f;
	oneVertex.texCoord.x = 0.f;
	oneVertex.texCoord.y = 0.f;
	saveVertex = oneVertex;
	VBO_lines.push_back(oneVertex);
	VBO_animVtx.push_back(oneVertex); 

	// Left down corner.
	oneVertex.vertex.x = (vert3[2] / halfWidth) - 1.0f;
	oneVertex.vertex.y = (vert3[3] / halfHeight) - 1.0f;
	oneVertex.texCoord.x = 0.f;
	oneVertex.texCoord.y = 1.f;
	VBO_lines.push_back(oneVertex);
	VBO_animVtx.push_back(oneVertex);
	VBO_animVtx.push_back(saveVertex);
	VBO_animVtx.push_back(oneVertex);

	// Right down corner.
	oneVertex.vertex.x = (vert3[4] / halfWidth) - 1.0f;
	oneVertex.vertex.y = (vert3[5] / halfHeight) - 1.0f;
	oneVertex.texCoord.x = 1.f;
	oneVertex.texCoord.y = 1.f;
	VBO_lines.push_back(oneVertex);
	VBO_animVtx.push_back(oneVertex);

	// Right up corner.
	oneVertex.vertex.x = (vert3[6] / halfWidth) - 1.0f;
	oneVertex.vertex.y = (vert3[7] / halfHeight) - 1.0f;
	oneVertex.texCoord.x = 1.f;
	oneVertex.texCoord.y = 0.f;
	VBO_lines.push_back(oneVertex);
	VBO_animVtx.push_back(oneVertex);

	// Result window.
	// Left up corner.
	oneVertex.vertex.x = (vert4[0] / halfWidth) - 1.0f;
	oneVertex.vertex.y = (vert4[1] / halfHeight) - 1.0f;
	oneVertex.texCoord.x = 0.f;
	oneVertex.texCoord.y = 0.f;
	VBO_lines.push_back(oneVertex);

	// Left down corner.
	oneVertex.vertex.x = (vert4[2] / halfWidth) - 1.0f;
	oneVertex.vertex.y = (vert4[3] / halfHeight) - 1.0f;
	oneVertex.texCoord.x = 0.f;
	oneVertex.texCoord.y = 1.f;
	VBO_lines.push_back(oneVertex);

	// Right down corner.
	oneVertex.vertex.x = (vert4[4] / halfWidth) - 1.0f;
	oneVertex.vertex.y = (vert4[5] / halfHeight) - 1.0f;
	oneVertex.texCoord.x = 1.f;
	oneVertex.texCoord.y = 1.f;
	VBO_lines.push_back(oneVertex);

	// Right up corner.
	oneVertex.vertex.x = (vert4[6] / halfWidth) - 1.0f;
	oneVertex.vertex.y = (vert4[7] / halfHeight) - 1.0f;
	oneVertex.texCoord.x = 1.f;
	oneVertex.texCoord.y = 0.f;
	VBO_lines.push_back(oneVertex);

	// Points.
	for (int i = 0; i < 16; i += 2) {
		oneVertex.vertex.x = (points[i] / halfWidth) - 1.0f;
		oneVertex.vertex.y = (points[i + 1] / halfHeight) - 1.0f;
		VBO_lines.push_back(oneVertex);
	}

	// Set lines Vtx.
	glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
	glBufferData(GL_ARRAY_BUFFER, VBO_lines.size() * sizeof(GLVertexData), VBO_lines.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * 6 * sizeof(GLuint), indices, GL_STATIC_DRAW);

	// For compute animation state.
	animVtxOrigX = VBO_animVtx[0].vertex.x;
	animVtxDiffX = VBO_animVtx[5].vertex.x - VBO_animVtx[0].vertex.x;;

	// Set animation Vtx.
	glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
	glBufferData(GL_ARRAY_BUFFER, VBO_animVtx.size() * sizeof(GLVertexData), VBO_animVtx.data(), GL_DYNAMIC_COPY);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * 6 * sizeof(GLuint), animIndices, GL_STATIC_DRAW);

	// Set routines.
	glBindVertexArray(VAO[2]);
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO[2]);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLVertexData), (GLvoid*)offsetof(GLVertexData, vertex));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLVertexData), (GLvoid*)offsetof(GLVertexData, texCoord));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO[2]);
	}
	glBindVertexArray(0);

	glBindVertexArray(VAO[3]);
	{
		glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLVertexData), (GLvoid*)offsetof(GLVertexData, vertex));
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLVertexData), (GLvoid*)offsetof(GLVertexData, texCoord));
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO[3]);
	}
	glBindVertexArray(0);
}

// Create textures from loaded images. For animation.
void SDLGLContext::PrepareTextures()
{
	SDL_Surface* loadedImg = IMG_Load("./images/Cornell_box.png");
	glBindTexture(GL_TEXTURE_2D, animTextures[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, loadedImg->w, loadedImg->h, 0,
		GL_RGB, GL_UNSIGNED_BYTE, loadedImg->pixels);
	SDL_FreeSurface(loadedImg);

	loadedImg = IMG_Load("./images/Black.png");
	glBindTexture(GL_TEXTURE_2D, animTextures[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, loadedImg->w, loadedImg->h, 0,
		GL_RGB, GL_UNSIGNED_BYTE, loadedImg->pixels);
	SDL_FreeSurface(loadedImg);
}

// Check buttons for user interact.
void SDLGLContext::CheckButtons(long xPos, long yPos, bool isClick)
{
	// Need to convert y-axis from SDL to OpenGL coordinate.
	yPos = screenHeight - yPos;

	for (std::vector<GLButton*>::iterator it = listButtons.begin(); it != listButtons.end(); it++) {

		if ((*it)->isMouseInside(xPos, yPos)) {
			if (!isClick) {
				// Turn on highlight of button.
				(*it)->Highlight(true);
			}
			else {
				// Other buttons.
				if (((*it) != listButtons[6]) && ((*it) != listButtons[7])) {
					(*it)->Press();
					CheckButtonSettings((*it)->GetID());
				}
				// Pause button.
				else if ((*it) != listButtons[7]) {
					if ((*it)->isPressed()) {
						if (IsActiveButtons()) {
							listButtons[6]->SetColor(0.f, 1.f, 0.f, 1.f);
							listButtons[6]->SetPressed(false);
							LeaveSem(1);
						}
					}
					else {
						listButtons[6]->SetColor(1.f, 0.f, 0.f, 1.f);
						listButtons[6]->Press();
						SeizeSem(1);
					}
				}
				else {
					// Save images.
					Color c;
					int index = 0;
					for (unsigned int y = 0; y < resultHeight; y++) {
						for (unsigned int x = 0; x < resultWidth; x++) {
							c.r = pixels[index];
							c.g = pixels[index+1];
							c.b = pixels[index+2];
							output.setPixel(x, y, c);
							index += 4;
						}
					}
					output.save("FITracer2015output.png");
				}
			}
		}
		else {
			(*it)->Highlight(false);
		}

	}
}

// Update pathtracing computation state.
void SDLGLContext::CheckButtonSettings(unsigned int ID)
{
	switch (ID) {
	case 0:
		activeRenderer = CPU_RENDER;
		break;
	case 1:
		activeRenderer = GPU_RENDER;
		break;
	case 2:
		activeAS = AS_LIST;
		break;
	case 3:
		activeAS = AS_OCTREE;
		break;
	case 4:
		activeAS = AS_UNIFORM_GRID;
		break;
	case 5:
		activeAS = AS_BVH;
		break;
	}
}

// Check hotkeys for user interact.
void SDLGLContext::CheckHotkeys(int pressedButton)
{
	static Color c;
	static int index;

	switch (pressedButton) {
	// Press CPU_render.
	case 'C':
		listButtons[0]->Press();
		SetActiveRenderer(CPU_RENDER);
		break;
	// Press GPU_render.
	case 'G':
		listButtons[1]->Press();
		SetActiveRenderer(GPU_RENDER);
		break;
	// Press List.
	case 'L':
		listButtons[2]->Press();
		SetActiveAS(AS_LIST);
		break;
	// Press Octree.
	case 'O':
		listButtons[3]->Press();
		SetActiveAS(AS_OCTREE);
		break;
	// Press Uniform_grid.
	case 'U':
		listButtons[4]->Press();
		SetActiveAS(AS_UNIFORM_GRID);
		break;
	// Press Bounding_volume_hierarchy.
	case 'B':
		listButtons[5]->Press();
		SetActiveAS(AS_BVH);
		break;
	// Press Save
	case 'S':
		index = 0;
		for (unsigned int y = 0; y < resultHeight; y++) {
			for (unsigned int x = 0; x < resultWidth; x++) {
				c.r = pixels[index];
				c.g = pixels[index + 1];
				c.b = pixels[index + 2];
				output.setPixel(x, y, c);
				index += 4;
			}
		}
		output.save("FITracer2015output.png");
		break;
	// Press Pause
	case 'P':
		if (listButtons[6]->isPressed()) {
			if (IsActiveButtons()) {
				listButtons[6]->SetColor(0.f, 1.f, 0.f, 1.f);
				listButtons[6]->SetPressed(false);
				LeaveSem(1);
			}
		}
		else {
			listButtons[6]->SetColor(1.f, 0.f, 0.f, 1.f);
			listButtons[6]->Press();
			SeizeSem(1);
		}
		break;
	// Quit draw cycle.
	case VK_ESCAPE:
		quit = true;
		break;
	}
}

bool SDLGLContext::IsActiveButtons()
{
	int flag = 0;

	for (int i = 0; i < 6; i++) {
		flag += listButtons[i]->isPressed();
	}

	if (flag != 2) {
		return false;
	}

	return true;
}

// Set state of pathtracing computation. For animation indication of progress.
void SDLGLContext::SetPercent(unsigned int _percent) 
{
	if (_percent > 100) {
		percentDone = 100;
	}
	else {
		percentDone = _percent;
	}
	wasPercentUpdated = true;
}

// Compute pathtracing render time.
void SDLGLContext::SnapTime(Uint32 timer)
{
	sampleRenderTime = (SDL_GetTicks() - timer) / 1000.0f;
	renderTime += sampleRenderTime;
}

// Render window objects.
void SDLGLContext::Draw() 
{
	glClearColor(0.5f, 0.5f, 0.5f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);

	if (wasSampleUpdated) {
		std::string text;
		char buffer[15];
		if (samplesCnt < 10)
			text = "00" + std::to_string(samplesCnt);
		else if (samplesCnt < 100)
			text = "0" + std::to_string(samplesCnt);
		else
			text = std::to_string(samplesCnt);
		
		SDL_Color black; black.r = 0x00; black.g = 0x00; black.b = 0x00; black.a = 0xFF;
		SDL_Surface* surfaceMsg = TTF_RenderText_Blended(mainFont, text.c_str(), black);
		glBindTexture(GL_TEXTURE_2D, textures[14]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surfaceMsg->w, surfaceMsg->h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, surfaceMsg->pixels);
		SDL_FreeSurface(surfaceMsg);

		sprintf_s(buffer, "%.3f sec", sampleRenderTime);
		surfaceMsg = TTF_RenderText_Blended(mainFont, buffer, black);
		glBindTexture(GL_TEXTURE_2D, textures[13]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, surfaceMsg->w, surfaceMsg->h, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, surfaceMsg->pixels);
		SDL_FreeSurface(surfaceMsg);

		wasSampleUpdated = false;
	}

	DrawResult();
	DrawButtonsTexts();
}

void SDLGLContext::DrawButtonsTexts() 
{
	GLColor buttonColor;
	int i = 0;
	// For correct draw new object over previous object.
	glDisable(GL_DEPTH_TEST);

	glActiveTexture(GL_TEXTURE0);
	{
		// Draw buttons.
		{
			glBindVertexArray(VAO[0]);
			glUniform1i(isBorderUniform, false);
			// Draw color.
			glUniform1f(mixTexCol, 1.0f);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO[0]);
			for (std::vector<GLButton*>::iterator it = listButtons.begin(); it != listButtons.end(); it++) {
				// Every button can have diff color.
				buttonColor = (*it)->GetColor();
				glUniform4f(colorUniform,
					buttonColor.r,
					buttonColor.g,
					buttonColor.b,
					buttonColor.a);
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, p_OffsetInVBO(i * 6 * sizeof(GLuint)));
				i++;
			}
		}

		// Draw borders of buttons.
		{
			glBindTexture(GL_TEXTURE_2D, 0);
			glUniform1i(isBorderUniform, true);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO[1]);
			glDrawElements(GL_LINES, 64, GL_UNSIGNED_INT, 0);
		}

		glEnable(GL_BLEND);
		// For correctly render alpha channel.
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		i = 0;

		// Draw texts.
		{
			glBindVertexArray(VAO[1]);
			glUniform1i(isBorderUniform, false);
			glUniform1f(mixTexCol, 0.f);
			// rekne OpenGL odkud se berou data pro kresleni (z nasich VBO)
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO[0]);
			for (std::vector<GLButton*>::iterator it = listButtons.begin(); it != listButtons.end(); it++) {
				glBindTexture(GL_TEXTURE_2D, textures[i]);
				buttonColor = (*it)->GetColor();
				glUniform4f(colorUniform,
					buttonColor.r,
					buttonColor.g,
					buttonColor.b,
					buttonColor.a);
				// Draw quad using vertex data and index data
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, p_OffsetInVBO(i * 6 * sizeof(GLuint)));
				i++;
			}

			for (std::vector<GLText*>::iterator it = listTexts.begin(); it != listTexts.end(); it++) {
				glBindTexture(GL_TEXTURE_2D, textures[i]);
				buttonColor = (*it)->GetColor();
				glUniform4f(colorUniform,
					buttonColor.r,
					buttonColor.g,
					buttonColor.b,
					buttonColor.a);
				// Draw quad using vertex data and index data
				glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, p_OffsetInVBO(i * 6 * sizeof(GLuint)));
				i++;
			}
		}

		// Draw animation.
		{
			if (wasPercentUpdated) {
				UpdateAnimationVtx();
			}

			glBindVertexArray(VAO[3]);
			// Cornell box.
			glUniform1i(isBorderUniform, false);
			glUniform1f(mixTexCol, 0.f);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO[3]);
			// rekne OpenGL odkud se berou data pro kresleni (z nasich VBO)
			glBindTexture(GL_TEXTURE_2D, animTextures[0]);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, p_OffsetInVBO(0));
			// Black.
			glUniform1i(isBorderUniform, false);
			glUniform1f(mixTexCol, 0.f);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO[3]);
			// rekne OpenGL odkud se berou data pro kresleni (z nasich VBO)
			glBindTexture(GL_TEXTURE_2D, animTextures[1]);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, p_OffsetInVBO(6 * sizeof(GLuint)));
		}

		glDisable(GL_BLEND);

		// Draw decoration lines.
		{
			glBindVertexArray(VAO[2]);

			glUniform1i(isBorderUniform, true);
			glLineWidth(2);

			glDrawArrays(GL_LINE_STRIP, 0, 4);
			glDrawArrays(GL_LINE_STRIP, 4, 4);
			glDrawArrays(GL_LINE_STRIP, 8, 4);
			glDrawArrays(GL_LINE_STRIP, 12, 4);
			glDrawArrays(GL_LINE_LOOP, 16, 4);
			glDrawArrays(GL_LINE_LOOP, 20, 4);

			glPointSize(5);
			glDrawArrays(GL_POINTS, 24, 8);

			glPointSize(1);
			glLineWidth(1);

			glBindVertexArray(0);
		}
	}
}

void SDLGLContext::UpdateAnimationVtx()
{
	GLVertexData data[2];
	GLfloat moveX;
	GLfloat moveTex;

	moveTex = (GLfloat)percentDone / 100;
	moveX = (animVtxDiffX * moveTex) + animVtxOrigX;

	glBindBuffer(GL_ARRAY_BUFFER, VBO[3]);
	glGetBufferSubData(GL_ARRAY_BUFFER, 2 * sizeof(GLVertexData), 2 * sizeof(GLVertexData), data);

	data[0].vertex.x = moveX;
	data[0].texCoord.x = moveTex;

	data[1].vertex.x = moveX;
	data[1].texCoord.x = moveTex;

	// Set new dynamic Vtx.
	glBufferSubData(GL_ARRAY_BUFFER, 2 * sizeof(GLVertexData), 2 * sizeof(GLVertexData), data);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	wasPercentUpdated = false;
}

// Render result image of pathtracing.
void SDLGLContext::DrawResult()
{
	if (isPixelsChanged) {
		glBindTexture(GL_TEXTURE_2D, resultTexture[indexImg]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, resultWidth, resultHeight, 0,
			GL_RGBA, GL_FLOAT, pixels);
		isPixelsChanged = false;
		LeaveSem(2);
	}

	if (loadPixelsFromTexture) {
		glBindTexture(GL_TEXTURE_2D, resultTexture[indexImg]);
		glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, pixels);
		loadPixelsFromTexture = false;
		LeaveSem(0);
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_DEPTH_BUFFER);
	glActiveTexture(GL_TEXTURE0);
	{
		glBindVertexArray(VAO[2]);
		glUniform1i(isBorderUniform, false);
		glUniform1f(mixTexCol, 0.f);
		glBindTexture(GL_TEXTURE_2D, resultTexture[indexImg]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, p_OffsetInVBO(6 * sizeof(GLuint)));
		glBindVertexArray(0);
	}
	glDisable(GL_BLEND);
}

// Deallocate all stuffs.
void SDLGLContext::Destroy() 
{
	for (std::vector<GLButton*>::iterator it = listButtons.begin(); it != listButtons.end(); it++) {
		delete (*it);
	}

	for (std::vector<GLText*>::iterator it = listTexts.begin(); it != listTexts.end(); it++) {
		delete (*it);
	}

	quit = true;
	TTF_Quit();
	IMG_Quit();
}

// Build whole shader program. Error will be printed to console window.
GLuint SDLGLContext::CompileShaders(const char * vertex_file_path, const char * fragment_file_path){

	// Create the shaders.
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file.
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()){
		std::string Line = "";
		while (getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}
	else{
		fprintf(stderr, "Impossible to open %s. Are you in the right directory ?" 
					    "Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file.
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()){
		std::string Line = "";
		while (getline(FragmentShaderStream, Line))
			FragmentShaderCode += "\n" + Line;
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader.
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader.
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}

	// Compile Fragment Shader.
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader.
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}

	// Link the program.
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program.
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0){
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}