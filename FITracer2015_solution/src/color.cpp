/*
 *  color.cpp
 *  asrTracer
 *
 *  Created by Petrik Clarberg on 2006-02-13.
 *  Copyright 2006 Lund University. All rights reserved.
 *
 */

#include <iomanip>
#include "color.h"

// Definition of friend functions follows

/// Component-wise multiplication with scalar (mixed-mode).
Color operator*(float s, const Color& c)
{
	return Color(s*c.r, s*c.g, s*c.b);
}

/// Write color values to an output stream in text format.
std::ostream& operator<<(std::ostream& os, const Color& c)
{
	std::streamsize p = os.precision();
	std::streamsize w = os.width();
	os	<< std::setprecision(5) << std::setw(10) << c.r
		<< std::setprecision(5) << std::setw(10) << c.g
		<< std::setprecision(5) << std::setw(10) << c.b << std::endl;
	os.precision(p);
	os.width(w);
	return os;
}
