/*
 *  listaccelerator.cpp
 *  prTracer
 *
 *  Created by Rasmus Barringer on 2011-01-31.
 *  Copyright 2011 Lund University. All rights reserved.
 *
 */

#include "listaccelerator.h"

void ListAccelerator::build(const std::vector<Intersectable*>& objects)
{
	this->objects = objects;
}

bool ListAccelerator::intersect(const Ray& ray)
{
	std::vector<Intersectable*>::iterator i;
	for (i = objects.begin(); i != objects.end(); ++i) {
		if ((*i)->intersect(ray))
			return true;
	}
	return false;
}

bool ListAccelerator::intersect(const Ray& ray, Intersection& is)
{
	is.mHitTime = INF;

	std::vector<Intersectable*>::iterator i;
	for (i = objects.begin(); i != objects.end(); ++i) {
		Intersection currentIs;
		
		if ((*i)->intersect(ray, currentIs)) {
			if (currentIs.mHitTime < is.mHitTime)
				is = currentIs;
		}
	}

	return is.mHitTime != INF;
}
