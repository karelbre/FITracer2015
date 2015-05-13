/*
 *  lightprobe.h
 *  asrTracer
 *
 *  Created by Petrik Clarberg on 2006-03-10.
 *  Copyright 2006 Lund University. All rights reserved.
 *
 */

#ifndef LIGHTPROBE_H
#define LIGHTPROBE_H

#include "image.h"

class Color;

/**
 * Class representing a light probe (environment map). 
 * The light probes are stored as high-dynamic range
 * files in angular format, which represent the radiance in each
 * world space direction. The radiance in a specific direction is
 * returned by the getRadiance() function.
 */
class LightProbe
{
public:
	LightProbe();
	LightProbe(const std::string& filename);
	virtual ~LightProbe();

	void load(const std::string& filename);
	Color getRadiance(const Vector3D& d) const;
	
protected:
	Image mProbe;			///< The image where the light probe is stored.
};

#endif
