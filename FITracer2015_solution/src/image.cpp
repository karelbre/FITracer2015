/*
 *  image.cpp
 *  asrTracer
 *
 *  Created by Petrik Clarberg on 2006-02-13.
 *  Copyright 2006 Lund University. All rights reserved.
 *
 */

#include "defines.h"
#include "image.h"
#include <stdexcept>
#include <algorithm>

//#define SUPPORT_EXR
#define SUPPORT_PFM
#define SUPPORT_PNG

#ifdef SUPPORT_EXR
#include <ImfRgba.h>
#include <ImfRgbaFile.h>
#include <ImfArray.h>
#include <ImfStringAttribute.h>

static const char EXR_OUTPUT_COMMENT[] = "PR Raytracer";
#endif

#ifdef SUPPORT_PFM
#include <fstream>
#include <vector>
#include "pfm/config.hpp"
#include "pfm/pfm.hpp"
#include "pfm/byte_order.hpp"
#include "pfm/color_pixel.hpp"
#include "pfm/pfm_input_file.hpp"
#include "pfm/pfm_output_file.hpp"

using namespace pfm;
#endif

#ifdef SUPPORT_PNG
#include "lodepng/lodepng.h"

inline float clamp(float x) {
	return x < 0.0f ? 0.0f : (x > 1.0f ? 1.0f : x);
}

inline int toneMap(float x) {
	const float exposure = 0.0f;
	const float gamma = 2.2f;
	
	float m = powf(2.0f, exposure);
	float g = 1.0f / gamma;
	
	x *= m;
	x = powf(x, g);	
	
	return (int)(clamp(x)*255.0f+0.5f);
}
#endif

using namespace std;

/**
 * Default constructor. Creates an uninitialized image.
 */
Image::Image() : mPixels(0), mWidth(0), mHeight(0), samples(0.0)
{
}

/**
 * Creates an image buffer of the specified size.
 * \param width Width in pixels
 * \param height Height in pixels
 */
Image::Image(int width, int height) : mWidth(width), mHeight(height), mPixels(new Color[width*height]), samples(0.0)
{
	std::cout << "creating image (" << mWidth << "x" << height << ")" << std::endl;
	
	if (width <= 0 || height <= 0)
		throw std::runtime_error("(Image::Image) image dimensions must be larger than 0");
}

/**
 * Destroys the object.
 */
Image::~Image()
{
	if (mPixels)
		delete [] mPixels;
}

/**
 * Loads an image from a file.
 * Currently, OpenEXR (.exr) is the only supported format.
 */
void Image::load(const std::string& filename)
{
#ifdef SUPPORT_EXR
	if (isFormat(filename, ".exr")) {
		int width, height;
		Imf::Array2D<Imf::Rgba> pixels;
		
		Imf::RgbaInputFile file(filename.c_str());
		Imath::Box2i dw = file.dataWindow();
		width  = dw.max.x - dw.min.x + 1;
		height = dw.max.y - dw.min.y + 1;

		pixels.resizeErase(height,width);
		file.setFrameBuffer(&pixels[0][0] - dw.min.x - dw.min.y * width, 1, width);
		file.readPixels(dw.min.y, dw.max.y);
		
		if (mPixels)
			delete[] mPixels;

		mPixels = new Color[width*height];
		mWidth = width;
		mHeight = height;
		
		int k = 0;
		for (int y = 0; y < height; y++) {
			for (int x = 0; x < width; x++) {
				const Imf::Rgba& c = pixels[y][x];
				mPixels[k++] = Color(c.r,c.g,c.b);
			}
		}
		return;
	}
#endif
	
#ifdef SUPPORT_PFM
	if (isFormat(filename, ".pfm")) {
		ifstream istream(filename.c_str());
		pfm_input_file pfm_input_file(istream);
		
		format_type format;
		size_t width, height;
		byte_order_type input_byte_order;
		double scale;
		pfm_input_file.read_header(format, width, height, input_byte_order, scale);
		
		if (mPixels)
			delete[] mPixels;
		
		mPixels = new Color[width*height];
		mWidth = (int)width;
		mHeight = (int)height;
		
		vector<color_pixel> scanline(mWidth);
		
		for (int row = 0; row < mHeight; ++row) {
			pfm_input_file.read_color_scanline(&scanline[0], mWidth);
			
			for (int x = 0; x < mWidth; ++x) {
				const color_pixel& s = scanline[x];
				Color& d = mPixels[row*mWidth+x];
				d = Color(s[0], s[1], s[2]);
			}
		}
		return;
	}
#endif
	
#ifdef SUPPORT_PNG
	if (isFormat(filename, ".png")) {
		unsigned width, height;
		std::vector<unsigned char> image;
		
		LodePNG::decode(image, width, height, filename);
		
		if (mPixels)
			delete[] mPixels;
		
		mPixels = new Color[width*height];
		mWidth = width;
		mHeight = height;
		
		int k = 0;
		for (int y = 0; y < mHeight; y++) {
			for (int x = 0; x < mWidth; x++) {
				const unsigned char* c = &image[(y*width + x)*4];
				mPixels[k++] = Color((float)c[0], (float)c[1], (float)c[2]) * (1.0f/255.0f);
			}
		}
		return;
	}
#endif
	
	throw std::runtime_error("unknown format for " + filename);
}

//inline float correct(float x){ return (powf(x,1.66f)) * 255.0f * powf(10.0f, 1.66f); }

/**
 * Writes the image to a file.
 * The file format is chosen based on the file ending, but currently
 * only OpenEXR (.exr) high-dynamic range images are supported.
 * \param filename Filename to store the image in
 */
void Image::save(const std::string& filename) const
{
	cout << "writing image (" << filename << ")" << endl;

#ifdef SUPPORT_EXR
	if (isFormat(filename, ".exr")) {
		int n = mWidth * mHeight;
		Imf::Rgba* data = new Imf::Rgba[n];

		for(int i = 0; i < n; i++) {
			Color& c = mPixels[i];
			data[i] = Imf::Rgba((half)c.r, (half)c.g, (half)c.b, 1.0);
		}

		try {
			Imf::Header header(mWidth, mHeight);
			header.insert("comment", Imf::StringAttribute(EXR_OUTPUT_COMMENT));

			Imf::RgbaOutputFile file(filename.c_str(), header, Imf::WRITE_RGBA);
			file.setFrameBuffer(data, 1, mWidth);
			file.writePixels(mHeight);
		}
		catch(const std::exception& e) {
			cerr << e.what() << endl;
		}
		delete[] data;
		return;
	}
#endif
	
#ifdef SUPPORT_PFM
	if (isFormat(filename, ".pfm")) {
		ofstream ostream(filename.c_str());
		
		if (!ostream) {
			cerr << "unable to write " << filename << endl;
			return;
		}
		
		pfm_output_file pfm_output_file(ostream);
		pfm_output_file.write_header(color_format, mWidth, mHeight, host_byte_order, 1.0f);

		vector<color_pixel> scanline(mWidth);
		
		for (int row = 0; row < mHeight; ++row) {
			for (int x = 0; x < mWidth; ++x) {
				Color& c = mPixels[row*mWidth+x];
				scanline[x] = color_pixel(c.r, c.g, c.b);
			}
			
			pfm_output_file.write_color_scanline(&scanline[0], mWidth);
		}
		return;
	}
#endif
	
#ifdef SUPPORT_PNG
	if (isFormat(filename, ".png")) {
		int pixelCount = mWidth*mHeight;
		std::vector<unsigned char> image(pixelCount * 4);
		
		for(int i = 0; i < pixelCount; ++i) {
			image[i*4+0] = toneMap(mPixels[i].r);
			image[i*4+1] = toneMap(mPixels[i].g);
			image[i*4+2] = toneMap(mPixels[i].b);
			image[i*4+3] = 0xff;
		}
		
		LodePNG::encode(filename, image, mWidth, mHeight);
		return;
	}
#endif
	
	cout << "unknown format for " << filename << endl;
}

/**
 * Sets the pixel at (x,y) to color c.
 */
void Image::setPixel(int x, int y, const Color& c)
{
	if (x < 0 || x >= mWidth || y < 0 || y >= mHeight) 
		throw std::runtime_error("(Image::setPixel) pixel coordinate outside image");
	
	mPixels[y*mWidth + x] = c;
}

void Image::addSamplePixel(int x, int y, const Color& c) {
	if (x < 0 || x >= mWidth || y < 0 || y >= mHeight)
		throw std::runtime_error("(Image::setPixel) pixel coordinate outside image");

	Color mix = mPixels[y*mWidth + x] * (samples / (samples + 1));
	mPixels[y*mWidth + x] = mix + (c * (1 / (samples + 1)));
}

/**
 * Returns the pixel value at (x,y) in the reference c.
 * Note that the range of the parameters is not checked.
 */
void Image::getPixel(int x, int y, Color& c) const
{
	c = mPixels[y*mWidth + x];
}

/**
 * Returns the pixel at (x,y).
 * Note that the range of the parameters is not checked.
 */
Color Image::getPixel(int x, int y) const
{
	return mPixels[y*mWidth + x];
}

bool Image::isFormat(const std::string& filename, const char* ext) {
	if (filename.size() < strlen(ext))
		return false;

	string lowercase(filename.size(), ' ');
	transform(filename.begin(), filename.end(), lowercase.begin(), (int(*)(int))tolower);

	return equal(lowercase.begin() + lowercase.size() - strlen(ext), lowercase.end(), ext);

}
