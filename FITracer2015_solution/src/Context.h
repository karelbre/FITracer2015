#ifndef _CONTEXT_H_
#define _CONTEXT_H_

#include "Button.h"
#include <SDL_ttf.h>
#include <list>

#define BUTTONS_CNT 7
#define TEXTS_CNT 5
#define IMG_CNT 2

#define CPU_RENDER 0
#define GPU_RENDER 1
#define BVH 2
#define UNIFORM_GRID 3
#define OCTREE 4
#define LIST 5
#define PAUSE 6

class Context {
public:
	Context() { 
		sdlWindow = NULL; 
		sdlRenderer = NULL; 
		sdlFont = NULL; 
		activeAcc = BVH; 
		activeRender = CPU_RENDER; 
		perc = 0; 
		sdlSem = NULL; 
		is_result = false;
		result_img = NULL;
	}
	Context(SDL_Window* window, SDL_Renderer* renderer, TTF_Font* font = NULL) {
		sdlWindow = window; 
		sdlRenderer = renderer; 
		sdlFont = font; 
		activeAcc = BVH; 
		activeRender = CPU_RENDER; 
		perc = 0; 
		sdlSem = NULL;
		is_result = false;
		result_img = NULL;
	}

	void initButtons();
	void initSubWindows();
	void initTexts();
	void initImages();
	void initSem() { sdlSem = SDL_CreateSemaphore(1); }
	void seizeSem() { SDL_SemWait(sdlSem); }
	void leaveSem() { SDL_SemPost(sdlSem); }

	//Setters
	void setWindow(SDL_Window* window) { sdlWindow = window; }
	void setRenderer(SDL_Renderer* renderer) { sdlRenderer = renderer; }
	void setFont(TTF_Font* font) { sdlFont = font; }
	void setImage(SDL_Texture* img, int index) { if (index < IMG_CNT) allImgs[index].first = img; }
	void setPerc(int perc) { this->perc = perc; }
	void setActiveAcc(int accelerator) { activeAcc = accelerator; }
	void setActiveRender(int render) { activeRender = render; }

	//Getters
	SDL_Window* getWindow() { return sdlWindow; }
	SDL_Renderer* getRenderer() { return sdlRenderer; }
	TTF_Font* getFont() { return sdlFont; }
	Button* getButtons() { return allButtons; }
	std::pair<SDL_Texture*, SDL_Rect*> getText(int i = 0) { 
		if (i < TEXTS_CNT) return allTexts[i];
		std::pair<SDL_Texture*, SDL_Rect*> empty;
		return empty;
	}
	std::pair<SDL_Texture*, SDL_Rect*> getImage(int i = 0) {
		if (i < IMG_CNT) return allImgs[i];
	}
	int getPerc() { return perc; }
	int getActiveAcc() { return activeAcc; }
	int getActiveRender() { return activeRender; }

	//Set variables to NULL
	void clearVar() { sdlWindow = NULL; sdlRenderer = NULL; sdlFont = NULL; }

	//Help functions for allocate and setup structure 
	static SDL_Rect* allocRect(int x, int y, int w, int h);
	static SDL_Color* allocColor(int r, int g, int b, int a);
	static bool setRect(SDL_Rect* pos, int x, int y, int w, int h);
	static bool setColor(SDL_Color* color, int r = 0x00, int g = 0x00, int b = 0x00, int a = 0xFF);
	static SDL_Texture* createText(Context* sdlContext, std::string text= "");

	//Check all buttons for events
	bool handleEvent(SDL_Event* e);
	void turnButton(int button, ChainButton* chain);

	//Store all windows
	std::pair<SDL_Rect*, SDL_Color*> leftWindow;
	std::pair<SDL_Rect*, SDL_Color*> rightWindow;
	bool is_result;
	SDL_Texture* result_img;

private:
	//The window we'll be rendering to
	SDL_Window* sdlWindow;

	//The window renderer
	SDL_Renderer* sdlRenderer;

	//Globally used font
	TTF_Font* sdlFont;

	//Store all buttons
	Button allButtons[BUTTONS_CNT];
	int activeAcc;
	int activeRender;

	//Store all texts
	std::pair<SDL_Texture*, SDL_Rect*> allTexts[TEXTS_CNT];

	//Store all pictures
	std::pair<SDL_Texture*, SDL_Rect*> allImgs[IMG_CNT];
	int perc;

	//Pause button semaphore
	SDL_semaphore* sdlSem;
};

#endif // _CONTEXT_H_