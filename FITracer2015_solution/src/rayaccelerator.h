/*
 *  rayaccelerator.h
 *  prTracer
 *
 *  Created by Rasmus Barringer on 2011-01-31.
 *  Copyright 2011 Lund University. All rights reserved.
 *
 */

#ifndef RAYACCELERATOR_H
#define RAYACCELERATOR_H

#include "intersectable.h"
#include <vector>

class RayAccelerator
{
public:
	virtual void build(const std::vector<Intersectable*>& objects) = 0;
	virtual bool intersect(const Ray& ray) = 0;
	virtual bool intersect(const Ray& ray, Intersection& is) = 0;
	virtual ~RayAccelerator() {}

	virtual std::vector<Intersectable*> getObjects() = 0;
};

#endif
