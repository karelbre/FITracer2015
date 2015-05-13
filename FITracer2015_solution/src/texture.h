/*
 *  texture.h
 *  prTracer
 *
 *  Created by Rasmus Barringer on 2011-02-01.
 *  Copyright 2011 Lund University. All rights reserved.
 *
 */

#ifndef TEXTURE_H
#define TEXTURE_H 

#include "image.h"
#include "matrix.h"

class Texture
{
private:
	int width;
	int height;
	int mipCount;
	Color** data;

public:
	Texture(const Image& image);
	Color get(float x, float y) const;
	Color get(float x, float y, float mip) const;
	Color get(float x, float y, const UV& ddx, const UV& ddy) const;
	Color getAnisotropic(float x, float y, const UV& ddx, const UV& ddy) const;
	~Texture();

private:
	Color getInterpolated(float x, float y, int mipIndex) const;
	Color getTexel(int x, int y, int mipIndex) const;
};

#endif
