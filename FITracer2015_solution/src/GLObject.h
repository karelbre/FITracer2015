/*
	Name: GLObject.h
	Desc: Base structure for create OpenGL elements.
	Author: Karel Brezina (xbrezi13)
*/

#ifndef _GLOBJECT_H_
#define _GLOBJECT_H_

#include <string>

typedef struct {
	float r;
	float g;
	float b;
	float a;
} GLColor;

typedef struct {
	float x;
	float y;
} GLVertex;

typedef struct {
	float x;
	float y;
} GLTexCoord;

typedef struct {
	GLVertex vertex;
	GLTexCoord texCoord;
} GLVertexData;

class GLObject {
public:
	GLObject();
	GLObject(unsigned int _x, unsigned int _y, unsigned int _width, unsigned int _height, std::string _text = "");

	void SetColor(float red, float green, float blue, float alpha); 
	void SetColor(GLColor _color);
	void SetPosition(unsigned int _x, unsigned int _y, unsigned int _width, unsigned int _height, std::string _text = "");
	void SetX(unsigned int _x) { x = _x; }
	void SetY(unsigned int _y) { y = _y; }
	void SetWidth(unsigned int _width) { width = _width; }
	void SetHeight(unsigned int _height) { height = _height; }
	void SetText(std::string _text) { text = _text; }
	void SetID(unsigned int _ID) { ID = _ID; }

	std::string GetText() { return text; }
	GLColor GetColor() { return color; }
	GLVertexData* GetVertices(unsigned int screen_width, unsigned int screen_height);
	GLVertexData* GetTextVertices(unsigned int screen_width, unsigned int screen_height);
	unsigned int GetID() { return ID; }

	void AddColor(float red, float green, float blue);
	void SubColor(float red, float green, float blue);
	void AddAlpha(float alpha) { color.a += alpha; }
	void SubAlpha(float alpha) { color.a -= alpha; }
protected:
	unsigned int x;
	unsigned int y;
	unsigned int width;
	unsigned int height;

	unsigned int ID;
	std::string text;
	GLColor color;
};

#endif // _GLOBJECT_H_