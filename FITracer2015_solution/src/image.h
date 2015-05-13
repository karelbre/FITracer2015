/*
 *  image.h
 *  asrTracer
 *
 *  Created by Petrik Clarberg on 2006-02-13.
 *  Copyright 2006 Lund University. All rights reserved.
 *
 */
#ifndef IMAGE_H
#define IMAGE_H

#include "color.h"

/**
 * Class representing an image of size width x height pixels.
 * Each pixel in the image is a Color object with red, green, blue components. 
 * There are functions for loading and saving the image in OpenEXR (.exr) format, 
 * and for getting and setting pixel values. This class is used as frame buffer
 * in the raytracer, storing the output color of each pixel, and also used in
 * the LightProbe class for representing the environment map.
 */
class Image
{
public:
	Image();
	Image(int width, int height);
	~Image();

	void init() {
		if (mWidth == 0)
			return;

		if (mPixels != NULL) {
			delete[] mPixels;
			mPixels = new Color[mWidth * mHeight];
		}
		else {
			mPixels = new Color[mWidth * mHeight];
		}
	}
	void load(const std::string& filename);
	void save(const std::string& filename) const;
	void addSamplePixel(int x, int y, const Color& c);
	void setPixel(int x, int y, const Color& c);
	void getPixel(int x, int y, Color& c) const;
	Color getPixel(int x, int y) const;
	float getSample() { return samples; }
	void addSamples() { samples++; }

	void setWidth(int width) { mWidth = width; }
	/// Returns the width in pixels.
	int getWidth() const { return mWidth; }

	void setHeight(int height) { mHeight = height; }
	/// Returns the height in pixels.
	int getHeight() const { return mHeight; }	
	
private:
	static bool isFormat(const std::string& filename, const char* ext);
	
	float samples;
	int mWidth;			///< Width of image in pixels.
	int mHeight;		///< Height of image in pixels.
	Color* mPixels;		///< Array of pixels.
};

#endif
