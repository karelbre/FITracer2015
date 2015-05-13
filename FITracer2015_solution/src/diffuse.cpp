/*
 *  diffuse.cpp
 *  asrTracer
 *
 *  Created by Petrik Clarberg on 2006-03-02.
 *  Copyright 2006 Lund University. All rights reserved.
 *
 */

#include "defines.h"
#include "intersection.h"
#include "diffuse.h"

using namespace std;

/**
 * Constructs a diffuse (Lambertian) material with color c,
 * and (optionally) reflectivity r and transparency t,
 * and index of refraction n.
 */
Diffuse::Diffuse(const Color& c, float r, float t, float n) : Material(r, t, n), mDiffColor(c)
{
}

/**
 * Returns the BRDF at the intersection is for the light direction L.
 */
Color Diffuse::evalBRDF(const Intersection& is, const Vector3D& L)
{
	return mDiffColor / M_PI;
}


