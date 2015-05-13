/*
	Name: pathtracer.cpp
	Desc: CPU compute PT.
	Author: FredrikAppelros on GitHub, Karel Brezina (xbrezi13)
*/

#ifndef PATHTRACER_H
#define PATHTRACER_H

#include "raytracer.h"

class PathTracer : public Raytracer
{
public:
	PathTracer(Scene* scene, Image* img);
	~PathTracer();

	void computeFirstImage(SDLGLContext* context, float* data);
	virtual void computeImage(SDLGLContext* context, float* data);
	void setScene(Scene* scene) { this->mScene = scene; }
	
protected:
	Color tracePixel(int x, int y, int& ID);
	Color trace(const Ray& ray, int depth, int& ID);
};

#endif
