/*
	Name: GLButton.cpp
	Desc: Button structure for simple control.
	Author: Karel Brezina (xbrezi13)
*/

#include "GLButton.h"

GLButton::GLButton() : GLObject()
{
	pressed = false;
	highlighted = false;
}

GLButton::GLButton(unsigned int _x, unsigned int _y, unsigned int _width, unsigned int _height, std::string _text) 
	: GLObject(_x, _y, _width, _height, _text)
{
	pressed = false;
	highlighted = false;
}

void GLButton::SetPressed(bool setPressed)
{
	if (setPressed) {
		if (!pressed) {
			pressed = true;
			SubColor(0.2f, 0.2f, 0.2f);
		}
	}
	else {
		if (pressed) {
			pressed = false;
			AddColor(0.2f, 0.2f, 0.2f);
		}
	}
}

void GLButton::Highlight(bool turn_on)
{
	if (turn_on) {
		if (!highlighted) {
			SubColor(0.1f, 0.1f, 0.1f);
			highlighted = true;
		}
	}
	else {
		if (highlighted) {
			AddColor(0.1f, 0.1f, 0.1f);
			highlighted = false;
		}
	}
}

void GLButton::Press() 
{
	if (!pressed) {
		pressed = true;
		SubColor(0.2f, 0.2f, 0.2f);
			
		for (std::vector<GLButton*>::iterator it = listConnections.begin(); it != listConnections.end(); it++) {
			if ((*it)->isPressed()) {
				(*it)->SetPressed(false);
			}
		}
	}
}

bool GLButton::isMouseInside(unsigned int xPos, unsigned int yPos) {
	if ((xPos < x) || (xPos > (x + width))) {
		return false;
	}
	if ((yPos < y) || (yPos > (y + height))) {
		return false;
	}

	return true;
}

void GLButton::AddConnection(GLButton* button)
{
	listConnections.push_back(button);
}

void GLButton::RemoveConnection(GLButton* button)
{
	for (std::vector<GLButton*>::iterator it = listConnections.begin(); it != listConnections.end(); it++) {
		if (*it == button) {
			listConnections.erase(it);
		}
	}
}