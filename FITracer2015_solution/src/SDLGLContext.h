/*
	Name: SDL_Toolbox.cpp
	Desc: Context and functions for App.
	Author: Karel Brezina (xbrezi13)
*/

#ifndef _SDL_TOOLBOX_H_
#define _SDL_TOOLBOX_H_

#include <stdio.h>
#include "OpenGL30.h"
#include "SDL.h"
#include "SDL_thread.h"
#undef main
#include "SDL_ttf.h"
#include "SDL_image.h"
#include "GLButton.h"
#include "GLText.h"
#include "image.h"

#define CPU_RENDER 0
#define GPU_RENDER 1
#define AS_BVH 2
#define AS_UNIFORM_GRID 3
#define AS_OCTREE 4
#define AS_LIST 5
#define AS_BVH_FIRST 6
#define AS_UNIFORM_GRID_FIRST 7
#define AS_OCTREE_FIRST 8
#define AS_LIST_FIRST 9
#define NO_OPTION 10

class SDLGLContext {
public:
	SDLGLContext() { 
		quit = false; 
		isPixelsChanged = false;
		loadPixelsFromTexture = false;
		wasPercentUpdated = false;
		wasSampleUpdated = false;
		indexImg = 0;
		percentDone = 0; 
		samplesCnt = 0;
		objectsCnt = 0;
		activeAS = NO_OPTION; 
		activeRenderer = NO_OPTION; 
	}
	~SDLGLContext() {}

	// Init SDL and OpenGL.
	bool Init(int width, int height, int _resultWidth, int _resultHeight);
	// Add new objects to scene.
	void AddButton(GLButton* button);
	void AddText(GLText* text);
	// Prepare all object to render.
	void PrepareScene();
	// Check user click or move over buttons.
	void CheckButtons(long xPos, long yPos, bool isClick);
	void CheckHotkeys(int pressedButton);
	// Control semaphore for pause computation.
	void SeizeSem(int i) { 
		switch (i) {
		case 0: SDL_SemWait(mainSemaphore);
			break;
		case 1: SDL_SemWait(loadPixelsSemaphore);
			break;
		case 2: SDL_SemWait(updateTextureSemaphore);
			break;
		}
	}
	void LeaveSem(int i) {
		switch (i) {
		case 0: SDL_SemPost(mainSemaphore);
			break;
		case 1: SDL_SemPost(loadPixelsSemaphore);
			break;
		case 2: SDL_SemPost(updateTextureSemaphore);
			break;
		}
	}
	// Get activated accelerate structure.
	int GetActiveAS() { return activeAS; }
	int GetActiveRenderer() { return activeRenderer; }
	//GLuint GetResultTexture() { return resultTexture; }
	void AddSample() { samplesCnt++; wasSampleUpdated = true; }
	unsigned int GetSamples() { return samplesCnt; }
	unsigned int* GetSamplesPtr() { return &samplesCnt; }
	// Get handle to result texture.
	GLuint* GetResultTextures() { return resultTexture; }
	// Get pixels from texture.
	GLfloat* GetActualImage() { return pixels; }
	// Save pixels to texture.
	void UpdateActualResult() { isPixelsChanged = true; }
	// Map pixels to right texture.
	void ReloadTexturePixels() { 
		SeizeSem(0); 
		loadPixelsFromTexture = true; 
	}
	// Get number of texture with actual results.
	unsigned int GetResultIndex() { return indexImg; }
	unsigned int GetResultResolution() { return resultHeight; }
	// Swap flag for next time.
	void Swap() { indexImg = 1 - indexImg; }
	// Is active renderer and accelerated data structure?
	bool IsActiveButtons();
	// Reset settings of active methods.
	void ResetActiveSettings() { activeAS = NO_OPTION; activeRenderer = NO_OPTION; }
	// Set percent of computation. Only for CPU renderer.
	void SetPercent(unsigned int _percent);
	// Check time of render actual sample.
	void SnapTime(Uint32 timer);
	// Draw whole window.
	void Draw();
	// Draw result of pathtracing computation.
	void DrawResult();
	// Is end of draw cycle?
	bool isQuit() { return quit; }
	// Clean before exit program.
	void Destroy();
private:
	// Initializaton of VBOs and etc.
	void InitGL();
	// Compile vertex and fragment shader.
	GLuint CompileShaders(const char * vertex_file_path, const char * fragment_file_path);
	// Create Vtx and Tex for buttons.
	void PrepareButtons();
	// Create Vtx and Tex for texts.
	void PrepareTexts();
	// Create decorations of window.
	void PrepareLines();
	// Load images ínto textures.
	void PrepareTextures();
	// Load result image pixels to buffer.
	void ActualizePixels();
	// Compute new animation Vtx.
	void UpdateAnimationVtx();
	// Check which buttons are pressed.
	void CheckButtonSettings(unsigned int ID);
	void SetActiveAS(int _activeAS) { activeAS = _activeAS; }
	void SetActiveRenderer(int _activeRenderer) { activeRenderer = _activeRenderer; }
	void DrawButtonsTexts();
	// Draw result of pathtracing.
	void DrawImage();
private:
	// Font for text rendering.
	TTF_Font* mainFont;
	//Pause button semaphore
	SDL_semaphore* mainSemaphore;
	SDL_semaphore* loadPixelsSemaphore;
	SDL_semaphore* updateTextureSemaphore;

	unsigned int screenWidth;
	unsigned int screenHeight;

	std::vector<GLButton*> listButtons;
	std::vector<GLText*> listTexts;
	unsigned int objectsCnt;

	GLuint VBO[4];
	GLuint VAO[4];
	GLuint IBO[4];

	unsigned int indexImg;
	GLfloat* pixels;
	bool isPixelsChanged;
	bool loadPixelsFromTexture;
	// Textures for texts.
	GLuint* textures;
	// Textures for animation.
	GLuint animTextures[2];
	GLfloat animVtxOrigX;
	GLfloat animVtxDiffX;
	bool wasPercentUpdated;
	// Texture for pathtracing.
	GLuint resultTexture[2];
	unsigned int resultWidth;
	unsigned int resultHeight;
	// GLSL shader.
	GLuint shaderProgram;
	//   Get texture. | Border or inside? | Object color. | Color or texture? 
	GLuint samplerUniform, isBorderUniform, colorUniform, mixTexCol;

	// Time of render one sample.
	float sampleRenderTime;
	// Time of render all samples.
	float renderTime;
	unsigned int samplesCnt;
	bool wasSampleUpdated;
	// Useable only for CPU computation.
	unsigned int percentDone;
	// Active accelerate structure.
	int activeAS;
	int activeRenderer;
	// Will be end of program?
	bool quit;

	Image output;
};

#endif // _SDL_TOOLBOX_H