/*
 *  pointlight.cpp
 *  asrTracer
 *
 *  Created by Petrik Clarberg on 2006-02-28.
 *  Copyright 2006 Lund University. All rights reserved.
 *
 */

#include "defines.h"
#include "pointlight.h"

using namespace std;
	
/**
 * Default constructor. Creates a white light at origin.
 */
PointLight::PointLight() : mPosition(Point3D(0,0,0)), mColor(Color(1,1,1)), mIntensity(1.0f)
{
}

/**
 * Constructs a light initialized to position p, color c, and intensity i.
 */
PointLight::PointLight(const Point3D& p, const Color& c, float i) : mPosition(p), mColor(c), mIntensity(i)
{
}

/**
 * Destructor. Does nothing.
 */
PointLight::~PointLight()
{
}

/**
 * Prepares the point light for rendering. Its cached attributes
 * (radiance, world position) are computed and stored locally.
 */
void PointLight::prepare()
{
	mRadiance = mIntensity * mColor;
	mWorldPos = mWorldTransform * mPosition;
}
