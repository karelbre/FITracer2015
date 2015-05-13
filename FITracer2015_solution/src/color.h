/*
 *  color.h
 *  asrTracer
 *
 *  Created by Petrik Clarberg on 2006-02-13.
 *  Copyright 2006 Lund University. All rights reserved.
 *
 */

#ifndef COLOR_H
#define COLOR_H

#include <stdexcept>
#include <iostream>
#include <algorithm>

/**
 * Class representing an RGB color value. 
 * Each color component (r,g,b) is stored as a float and is not limited
 * to any specific range since we are working with high-dynamic range images.
 * The most common arithmetic operations have been implemented, so that
 * two colors can be added, multiplied, etc simply by: c1+c2, c1*c2, etc.
 * All arithmetic operations work element-wise.
 */
class Color
{
public:
	float r;		///< Red component.
	float g;		///< Green component.
	float b;		///< Blue component.
	
public:
	/// Initializes the color to black (0,0,0).
	Color() : r(0.0f), g(0.0f), b(0.0f) { }
	
	/// Initializes the color to (r,g,b).
	Color(float _r, float _g, float _b) : r(_r), g(_g), b(_b) { }

	/// Component-wise addition with color.
	Color operator+(const Color& c) const { return Color(r+c.r, g+c.g, b+c.b); }
	
	/// Component-wise addition with color in place.
	Color& operator+=(const Color& c) { r+=c.r; g+=c.g; b+=c.b; return *this; }
	
	/// Component-wise negation.
	Color operator-() { return Color(-r, -g, -b); }
	
	/// Component-wise subtraction with color.
	Color operator-(const Color& c) const { return Color(r-c.r, g-c.g, b-c.b); }
	
	/// Component-wise subtraction with color in place.
	Color& operator-=(const Color& c) { r-=c.r; g-=c.g; b-=c.b; return *this; }
	
	/// Component-wise multiplication with scalar (mixed-mode).
	friend Color operator*(float s, const Color& c);
	
	/// Component-wise multiplication with scalar.
	Color operator*(float s) const { return Color(s*r, s*g, s*b); }
	
	/// Component-wise multiplication with scalar (in place).
	Color& operator*=(float s) { r*=s; g*=s; b*=s; return *this; }
	
	/// Component-wise multiplication with color.
	Color operator*(const Color& c) const { return Color(r*c.r, g*c.g, b*c.b); }
	
	/// Component-wise multiplication with color in place.
	Color& operator*=(const Color& c) { r*=c.r; g*=c.g; b*=c.b; return *this; }
	
	/// Component-wise division with scalar.
	Color operator/(float s) const { s=1.0f/s; return Color(s*r, s*g, s*b); }

	/// Component-wise division with scalar (in place).
	Color& operator/=(float s) { s=1.0f/s; r*=s; g*=s; b*=s; return *this; }	
	
	/// Component-wise division with color.
	Color operator/(const Color& c) const { return Color(r/c.r, g/c.g, b/c.b); }
	
	/// Component-wise division with color in place.
	Color& operator/=(const Color& c) { r/=c.r; g/=c.g; b/=c.b; return *this; }
	
	/// Write color values to an output stream in text format.
	friend std::ostream& operator<<(std::ostream& os, const Color& c);
};

#endif
