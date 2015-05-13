/*
 *  texture.cpp
 *  prTracer
 *
 *  Created by Rasmus Barringer on 2011-02-01.
 *  Copyright 2011 Lund University. All rights reserved.
 *
 */

#include "defines.h"
#include "texture.h"
#include <assert.h>

float log2ff(float x)
{
	return log10f(x)/log10f(2.0f);
}

Texture::Texture(const Image& image)
{
	width = image.getWidth();
	height = image.getHeight();

	int size = width < height ? width : height;
	mipCount = 0;
	
	while (size) {
		size >>= 1;
		mipCount++;
	}
	
	data = new Color*[mipCount];
	data[0] = new Color[width * height];

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++)
			image.getPixel(x, y, data[0][y*width + x]);
	}

	for (int i = 1; i < mipCount; i++) {
		int lastMipWidth = width >> (i-1);
		
		int mipWidth = width >> i;
		int mipHeight = height >> i;
		
		data[i] = new Color[mipWidth * mipHeight];
		
		for (int y = 0; y < mipHeight; y++) {
			for (int x = 0; x < mipWidth; x++) {
				Color sum = data[i-1][2*y*lastMipWidth + 2*x] +
				data[i-1][2*y*lastMipWidth + 2*x+1] +
				data[i-1][(2*y+1)*lastMipWidth + 2*x] +
				data[i-1][(2*y+1)*lastMipWidth + 2*x+1];
				
				data[i][y*mipWidth + x] = sum/4;
			}
		}
	}
}

Color Texture::get(float x, float y) const
{
	return getInterpolated(x, y, 0);
}

Color Texture::get(float x, float y, float mip) const
{
	int mipIndex = (int)floor(mip);
	
	if (mipIndex < 0) {
		mipIndex = 0;
		mip = 0.0f;
	}
	else if (mipIndex >= mipCount) {
		mipIndex = mipCount-1;
		mip = (float)mipIndex;
	}
	else if (mip < 0.0f) {
		mip = 0.0f;
	}
	
	if (mipIndex < mipCount-1) {
		float sm = mip-(float)mipIndex;
		return getInterpolated(x, y, mipIndex)*(1.0f-sm) + getInterpolated(x, y, mipIndex+1)*sm;
	}
	else {
		return getInterpolated(x, y, mipIndex);
	}
}

Color Texture::get(float x, float y, const UV& ddx, const UV& ddy) const
{
	Vector2D sddx(ddx.u*width, ddy.u*width);
	Vector2D sddy(ddx.v*height, ddy.v*height);

	const float mipBias = 0.0f;
	float mip = mipBias + 0.5f*log2ff(maxT(sddx.dot(sddx), sddy.dot(sddy)));
	
	return get(x, y, mip);
}

Color Texture::getAnisotropic(float x, float y, const UV& ddx, const UV& ddy) const
{
	Vector2D sddx(ddx.u*width, ddy.u*width);
	Vector2D sddy(ddx.v*height, ddy.v*height);
	
	const float mipBias = 0.0f;
	float mip = mipBias + 0.5f*log2ff(minT(sddx.dot(sddx), sddy.dot(sddy)));

	Vector2D major(ddx.u, ddx.v);
	Vector2D minor(ddy.u, ddy.v);
	
	if (major.length2() < minor.length2())
		std::swap(major, minor);

	Color sample(0.0f, 0.0f, 0.0f);
	int sampleCount = (int)ceil(major.length() / minor.length());
	
	if (sampleCount <= 1)
		return get(x, y, mip);
	
	if (sampleCount > 512)
		sampleCount = 512;
	
	for (int i = 0; i < sampleCount; i++) {
		Vector2D s(x, y);
		s += major*((i/(float)(sampleCount-1)) - 0.5f);

		sample += get(s.x, s.y, mip);
	}
	
	return sample / (float) sampleCount;
}

Texture::~Texture()
{
	for (int i = 0; i < mipCount; i++)
		delete [] data[i];
	
	delete [] data;
}

Color Texture::getInterpolated(float x, float y, int mipIndex) const
{
	int mipWidth = width >> mipIndex;
	int mipHeight = height >> mipIndex;
	
	x *= mipWidth;
	y *= mipHeight;
	
	int ix = (int)floor(x);
	int iy = (int)floor(y);
	
	float sx = x - (float)ix;
	float sy = y - (float)iy;
	
	Color c0 = getTexel(ix, iy, mipIndex)*(1.0f-sx) + getTexel(ix+1, iy, mipIndex)*sx;
	Color c1 = getTexel(ix, iy+1, mipIndex)*(1.0f-sx) + getTexel(ix+1, iy+1, mipIndex)*sx;
	
	return c0*(1.0f-sy) + c1*sy;
}

Color Texture::getTexel(int x, int y, int mipIndex) const
{
	int mipWidth = width >> mipIndex;
	int mipHeight = height >> mipIndex;
	
	x %= mipWidth;
	y %= mipHeight;
	
	if (x < 0)
		x += mipWidth;
	
	if (y < 0)
		y += mipHeight;

	return data[mipIndex][y*mipWidth + x];
}
