#include "Context.h"

void Context::initSubWindows() {
	leftWindow = std::make_pair(allocRect(10, 10, 250, 580), allocColor(0x6F, 0x6D, 0x70, 0xFF));
	rightWindow = std::make_pair(allocRect(290, 90, 500, 500), allocColor(0x6F, 0x6D, 0x70, 0xFF));
}

void Context::initButtons() {

	//Buttons for render mode (CPU | GPU)
	allButtons[CPU_RENDER].setButton(allocRect(30, 55, 210, 40), allocColor(0xCF, 0xAF, 0xFF, 0xFF), createText(this, "CPU renderer"), true);
	allButtons[GPU_RENDER].setButton(allocRect(30, 105, 210, 40), allocColor(0xCF, 0xAF, 0xFF, 0xFF), createText(this, "GPU renderer"));
	allButtons[CPU_RENDER].addLink(GPU_RENDER);
	allButtons[GPU_RENDER].addLink(CPU_RENDER);
	//Buttons for type of Accelerated data structure
	allButtons[BVH].setButton(allocRect(30, 190, 210, 40), allocColor(0xCF, 0xAF, 0xFF, 0xFF), createText(this, "    BVH     "), true);
	allButtons[UNIFORM_GRID].setButton(allocRect(30, 240, 210, 40), allocColor(0xCF, 0xAF, 0xFF, 0xFF), createText(this, "Uniform grid"));
	allButtons[OCTREE].setButton(allocRect(30, 290, 210, 40), allocColor(0xCF, 0xAF, 0xFF, 0xFF), createText(this, "   Octree   "));
	allButtons[LIST].setButton(allocRect(30, 340, 210, 40), allocColor(0xCF, 0xAF, 0xFF, 0xFF), createText(this, "   List   "));
	//Links
	allButtons[BVH].addLink(UNIFORM_GRID);
	allButtons[BVH].addLink(OCTREE);
	allButtons[BVH].addLink(LIST);
	allButtons[UNIFORM_GRID].addLink(BVH);
	allButtons[UNIFORM_GRID].addLink(OCTREE);
	allButtons[UNIFORM_GRID].addLink(LIST);
	allButtons[OCTREE].addLink(BVH);
	allButtons[OCTREE].addLink(UNIFORM_GRID);
	allButtons[OCTREE].addLink(LIST);
	allButtons[LIST].addLink(BVH);
	allButtons[LIST].addLink(UNIFORM_GRID);
	allButtons[LIST].addLink(OCTREE);
	//Pause button
	allButtons[PAUSE].setButton(allocRect(750, 10, 40, 40), allocColor(0xCF, 0xAF, 0xFF, 0xFF), createText(this, "||"));
}

void Context::initTexts() {
	allTexts[0].first = createText(this, "Choose rendering mode");
	allTexts[0].second = allocRect(17,20,235,25);
	allTexts[1].first = createText(this, "Choose accelerated data structure");
	allTexts[1].second = allocRect(17,160,235,20);
	allTexts[2].first = createText(this, "Results preview");
	allTexts[2].second = allocRect(430, 50, 235, 35);
	allTexts[3].first = createText(this, "Progress");
	allTexts[3].second = allocRect(72, 390, 120, 35);
	allTexts[4].first = createText(this, "Seconds to render: ");
	allTexts[4].second = allocRect(290, 15, 150, 25);
}

void Context::initImages() {
	allImgs[0].second = allocRect(35, 430, 199, 150);
	allImgs[1].second = allocRect(35, 430, 199, 150);
}

SDL_Rect* Context::allocRect(int x, int y, int w, int h) {

	SDL_Rect* rect = new SDL_Rect;

	if (rect != NULL) {
		rect->x = x;
		rect->y = y;
		rect->w = w;
		rect->h = h;
	}

	return rect;
}

bool Context::setRect(SDL_Rect* pos, int x, int y, int h, int w) {

	if (pos != NULL) {
		pos->x = x;
		pos->y = y;
		pos->w = w;
		pos->h = h;
		return true;
	}

	return false;
}

SDL_Color* Context::allocColor(int r, int g, int b, int a) {

	SDL_Color* color = new SDL_Color;

	if (color != NULL) {
		color->r = r;
		color->g = g;
		color->b = b;
		color->a = a;
	}

	return color;
}

bool Context::setColor(SDL_Color* color, int r, int g, int b, int a) {

	if (color != NULL) {
		color->r = r;
		color->g = g;
		color->b = b;
		color->a = a;
		return true;
	}

	return false;
}

SDL_Texture* Context::createText(Context* sdlContext, std::string text) {
	SDL_Color black; black.r = 0x00; black.g = 0x00; black.b = 0x00; black.a = 0xFF;
	SDL_Surface* surfaceMsg = TTF_RenderText_Blended(sdlContext->getFont(), text.c_str(), black);
	SDL_Texture* texture = SDL_CreateTextureFromSurface(sdlContext->getRenderer(), surfaceMsg);
	SDL_FreeSurface(surfaceMsg);
	return texture;
}

bool Context::handleEvent(SDL_Event* e) {
	if (e->type == SDL_MOUSEMOTION || e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_MOUSEBUTTONUP)
	{

		for (int i = 0; i < BUTTONS_CNT; i++) {

			//Get mouse position
			int x, y;
			SDL_GetMouseState(&x, &y);

			//Check if mouse is in button
			bool inside = true;
			SDL_Rect* pos = allButtons[i].getPos();
			Button* button = &allButtons[i];
			bool pressed = allButtons[i].getPressed();

			//Mouse is left of the button
			if (x < pos->x)
			{
				inside = false;
			}
			//Mouse is right of the button
			else if (x > pos->x + pos->w)
			{
				inside = false;
			}
			//Mouse above the button
			else if (y < pos->y)
			{
				inside = false;
			}
			//Mouse below the button
			else if (y > pos->y + pos->h)
			{
				inside = false;
			}

			//Mouse is inside button
			if (inside) {
				//Set mouse over sprite
				switch (e->type)
				{
				case SDL_MOUSEMOTION:
					button->resetCol();
					button->subCol(32, 32, 0);
					break;

				/*case SDL_MOUSEBUTTONDOWN:
					setColor(color, 0x7C, 0x70, 0xB8, 0xFF);
					break;*/

				case SDL_MOUSEBUTTONUP:
					if (i == PAUSE)
						if (button->getPressed())
							button->setPressed(false);
						else
							button->setPressed(true);
					else
						button->setPressed(true);
					turnButton(i, button->getChain());
					break;
				}
			}
			else {
				button->resetCol();
			}
		}
		return allButtons[PAUSE].getPressed();
	}
	else if (e->type == SDL_KEYDOWN) {
		switch (e->key.keysym.sym) {
			//Pause button
			case SDLK_p:
				if (allButtons[PAUSE].getPressed())
					allButtons[PAUSE].setPressed(false);
				else
					allButtons[PAUSE].setPressed(true);
				break;
			//BVH button
			case SDLK_b:
				allButtons[BVH].setPressed(true);
				turnButton(BVH, allButtons[BVH].getChain());
				break;
			//Uniform Grid button
			case SDLK_u:
				allButtons[UNIFORM_GRID].setPressed(true);
				turnButton(UNIFORM_GRID, allButtons[UNIFORM_GRID].getChain());
				break;
			//Octree button
			case SDLK_o:
				allButtons[OCTREE].setPressed(true);
				turnButton(OCTREE, allButtons[OCTREE].getChain());
				break;
			case SDLK_l:
				allButtons[LIST].setPressed(true);
				turnButton(LIST, allButtons[LIST].getChain());
				break;
			//CPU render
			case SDLK_c:
				allButtons[CPU_RENDER].setPressed(true);
				turnButton(CPU_RENDER, allButtons[CPU_RENDER].getChain());
				break;
			//GPU render
			case SDLK_g:
				allButtons[GPU_RENDER].setPressed(true);
				turnButton(GPU_RENDER, allButtons[GPU_RENDER].getChain());
				break;
		}
		return allButtons[PAUSE].getPressed();
	}
	return allButtons[PAUSE].getPressed();
}

void Context::turnButton(int button, ChainButton* chain) {
	for (ChainButton* actual = chain; actual != NULL; actual = actual->getNextButton()) {
		allButtons[actual->getKeyButton()].setPressed(false);
	}

	switch (button) {
		case CPU_RENDER:
		case GPU_RENDER:
			activeRender = button;
			break;
		case BVH:
		case UNIFORM_GRID:
		case OCTREE:
		case LIST:
			activeAcc = button;
			break;
	}
}