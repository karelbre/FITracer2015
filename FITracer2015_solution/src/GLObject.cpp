/*
	Name: GLObject.h
	Desc: Base structure for create OpenGL elements.
	Author: Karel Brezina (xbrezi13)
*/

#include "GLObject.h"

GLObject::GLObject() 
	: x(0), y(0), width(0), height(0), text("")
{
	color.r = 0.f;
	color.g = 0.f;
	color.b = 0.f;
	color.a = 1.f;
}

GLObject::GLObject(unsigned int _x, unsigned int _y, unsigned int _width, unsigned int _height, std::string _text)
	: x(_x), y(_y), width(_width), height(_height), text(_text)
{
	color.r = 0.f;
	color.g = 0.f;
	color.b = 0.f;
	color.a = 1.f;
}

void GLObject::SetColor(float red, float green, float blue, float alpha)
{
	color.r = red;
	color.g = green;
	color.b = blue;
	color.a = alpha;
}

void GLObject::SetColor(GLColor _color)
{
	color = _color;
}

void GLObject::SetPosition(unsigned int _x, unsigned int _y, unsigned int _width, unsigned int _height, std::string _text)
{
	x = _x;
	y = _y;
	width = _width;
	height = _height;
	text = _text;
}

GLVertexData* GLObject::GetVertices(unsigned int screen_width, unsigned int screen_height)
{
	screen_width /= 2;
	screen_height /= 2;

	GLVertexData* vertices = new GLVertexData[4];

	// Right upper corner.
	vertices[0].vertex.x = ((float)(x + width) / screen_width) - 1.f;
	vertices[0].vertex.y = ((float)y / screen_height) - 1.f;
	vertices[0].texCoord.x = 1.f;
	vertices[0].texCoord.y = 1.f;
	// Left upper corner.
	vertices[1].vertex.x = ((float)x / screen_width) - 1.f;
	vertices[1].vertex.y = ((float)y / screen_height) - 1.f;
	vertices[1].texCoord.x = 0.f;
	vertices[1].texCoord.y = 1.f;
	// Left lower corner.
	vertices[2].vertex.x = ((float)x / screen_width) - 1.f;
	vertices[2].vertex.y = ((float)(y + height) / screen_height) - 1.f;
	vertices[2].texCoord.x = 0.f;
	vertices[2].texCoord.y = 0.f;
	// Right lower corner.
	vertices[3].vertex.x = ((float)(x + width) / screen_width) - 1.f;
	vertices[3].vertex.y = ((float)(y + height) / screen_height) - 1.f;
	vertices[3].texCoord.x = 1.f;
	vertices[3].texCoord.y = 0.f;

	return vertices;
}

GLVertexData* GLObject::GetTextVertices(unsigned int screen_width, unsigned int screen_height)
{
	GLVertexData* origVertices = GetVertices(screen_width, screen_height);

	float xDiff = origVertices[0].vertex.x - origVertices[1].vertex.x;
	float yDiff = origVertices[1].vertex.y - origVertices[2].vertex.y;
	int size = text.size();
	if (size <= 3)
		xDiff /= 3;
	else
		xDiff /= size-1;
	yDiff /= 5;

	origVertices[0].vertex.x -= xDiff;
	origVertices[0].vertex.y -= yDiff;

	origVertices[1].vertex.x += xDiff;
	origVertices[1].vertex.y -= yDiff;

	origVertices[2].vertex.x += xDiff;
	origVertices[2].vertex.y += yDiff;

	origVertices[3].vertex.x -= xDiff;
	origVertices[3].vertex.y += yDiff;

	return origVertices;
}

void GLObject::AddColor(float red, float green, float blue) 
{
	color.r += red;
	color.g += green;
	color.b += blue;
}

void GLObject::SubColor(float red, float green, float blue)
{
	color.r -= red;
	color.g -= green;
	color.b -= blue;
}


