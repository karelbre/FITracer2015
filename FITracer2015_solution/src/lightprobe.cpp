/*
 *  lightprobe.cpp
 *  asrTracer
 *
 *  Created by Petrik Clarberg on 2006-03-10.
 *  Copyright 2006 Lund University. All rights reserved.
 *
 */

#include "defines.h"
#include "lightprobe.h"

/**
 * Default constructor. Creates an uninitialized light probe.
 */
LightProbe::LightProbe()
{
}

/**
 * Constructor loading a light probe from the given filename.
 */
LightProbe::LightProbe(const std::string& filename)
{
	load(filename);
}

/**
 * Destructor. Does nothing.
 */
LightProbe::~LightProbe()
{
}

/**
 * Loads a light probe from the given filename.
 * The light probe must be in angular format.
 */
void LightProbe::load(const std::string& filename)
{
	std::cout << "loading light probe (" << filename << ")" << std::endl;
	mProbe.load(filename);
}

/**
 * Returns the radiance for direction d. 
 * The direction is assumed to be normalized to unit length.
 */
Color LightProbe::getRadiance(const Vector3D& d) const
{
	float len = sqrt(d.x*d.x + d.y*d.y);
	float r = (len < epsilon) ? 0.0f : (float)std::acos(-d.z)/(2.0f*M_PI*len);
	float u = r*d.x + 0.5f;
	float v = 0.5f - r*d.y;
	int w = mProbe.getWidth(), h = mProbe.getHeight();

	// Bilinear filtering
	float xf = w*u;
	float yf = h*v;
	
	int x1 = std::min(std::max((int)std::floor(xf), 0), w-1);
	int x2 = std::min(std::max((int)std::ceil(xf), 0), w-1);
	int y1 = std::min(std::max((int)std::floor(yf), 0), h-1);
	int y2 = std::min(std::max((int)std::ceil(yf), 0), h-1);
	
	// Interpolation parameters
	float up = std::min(std::max(xf-(float)x1, 0.0f), 1.0f);
	float vp = std::min(std::max(yf-(float)y1, 0.0f), 1.0f);
	
	Color c1 = mProbe.getPixel(x1,y1);
	Color c2 = mProbe.getPixel(x2,y1);
	Color c3 = mProbe.getPixel(x1,y2);
	Color c4 = mProbe.getPixel(x2,y2);
	
	return (1.0f-up)*(1.0f-vp)*c1 + up*(1.0f-vp)*c2 + (1.0f-up)*vp*c3 + up*vp*c4;

#if 0
	// Single sample
	int x = std::min(std::max((int)std::floor((float)w*u), 0), w-1);
	int y = std::min(std::max((int)std::floor((float)h*v), 0), h-1);	
	Color c;
	mProbe.getPixel(x,y,c);
	return c;
#endif
}
