#ifndef _BUTTON_H_
#define _BUTTON_H_

#include <SDL.h>

class ChainButton {
public:
	ChainButton() { button = 0; next = NULL; }
	ChainButton(int button) { this->button = button; next = NULL; }
	~ChainButton() {
		ChainButton* actual = this->next;
		ChainButton* remove;
		while (actual != NULL) {
			remove = actual;
			actual = actual->next;
			delete remove;
		}
	}

	bool setButton(int button) {
		ChainButton* actual;

		for (actual = this; actual->next != NULL; actual = actual->next) {
			if (actual->button == button)
				return false;
		}

		if (actual->button == button)
			return false;

		actual->next = new ChainButton(button);
		return true;
	}

	ChainButton* getNextButton() { return next; }
	int getKeyButton() { return button; }
private:
	int button;
	ChainButton* next;
};

class Button {
public:
	//Constructor
	Button() { chain = NULL; rect = NULL; color = NULL; pressed = true; text = NULL; }
	~Button() {}

	//Setters
	void setButton(SDL_Rect* rect, SDL_Color* color, SDL_Texture* text, bool pressed = false) {
		this->rect = rect;
		this->color = color;
		this->pressed = pressed;
		this->text = text;
		if (color != NULL) {
			col_orig = *color;
			col_press = *color;
			col_press.r -= 64; col_press.g -= 64; col_press.b -= 64;
		}
	}
	void setPos(SDL_Rect* rect) { this->rect = rect; }
	void setCol(SDL_Color* color) { this->color = color; }
	void setText(SDL_Texture* text) { 
		this->text = text;
	}
		
	void setPressed(bool state) { 
		pressed = state; 
		if (color != NULL)
			if (pressed)
				*color = col_press;
			else
				*color = col_orig;
	}

	//Getters
	SDL_Rect* getPos() { return rect; }
	SDL_Color* getCol() { return color; }
	bool getPressed() { return pressed; }
	SDL_Texture* getText() { return text; }

	void addCol(int r, int g, int b) { color->r += r; color->g += g; color->b += b; }
	void subCol(int r, int g, int b) { color->r -= r; color->g -= g; color->b -= b; }
	void resetCol() { 
		if (pressed) 
			*color = col_press; 
		else 
			*color = col_orig; 
	}

	void addLink(int link_index) {
		if (chain == NULL) {
			chain = new ChainButton(link_index);
		}
		else {
			chain->setButton(link_index);
		}
	}

	void setChain(ChainButton* chain) { this->chain = chain; }
	ChainButton* getChain() { return chain; }

	SDL_Color col_orig;
	SDL_Color col_press;

private:

	ChainButton* chain;
	//Position of button
	SDL_Rect* rect;
	//Color of button
	SDL_Color* color;
	//Is button pressed
	bool pressed;
	//Text on button
	SDL_Texture* text;
};

#endif // _BUTTON_H_