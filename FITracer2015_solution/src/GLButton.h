/*
	Name: GLButton.h
	Desc: Button structure for simple control.
	Author: Karel Brezina (xbrezi13)
*/

#ifndef _GLBUTTON_H_
#define _GLBUTTON_H_

#include "GLObject.h"
#include <vector>

class GLButton : public GLObject {
public:
	GLButton();
	GLButton(unsigned int _x, unsigned int _y, unsigned int _width, unsigned int _height, std::string _text = "");

	void SetPressed(bool _pressed);
	void SetHighlight(bool _highlighted ) { highlighted = _highlighted; }

	void Highlight(bool turn_on = false);
	bool isHighlighted() { return highlighted; }
	void Press();
	bool isPressed() { return pressed; }

	bool isMouseInside(unsigned int xPos, unsigned int yPos);

	void AddConnection(GLButton* button);
	void RemoveConnection(GLButton* button);

private:
	std::vector<GLButton*> listConnections;

	bool pressed;
	bool highlighted;
};

#endif // _GLBUTTON_H_