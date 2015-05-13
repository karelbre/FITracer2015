/*
 *  intersectable.h
 *  asrTracer
 *
 *  Created by Petrik Clarberg on 2006-02-24.
 *  Copyright 2006 Lund University. All rights reserved.
 *
 */

#ifndef INTERSECTABLE_H
#define INTERSECTABLE_H

#include "ray.h"
#include "aabb.h"
#include "intersection.h"
	
/**
 * Interface that needs to be implemented by all intersectable primitives
 * (Sphere, Triangle). The intersect(const Ray& ray) function returns
 * true if the ray intersects the object, but it does not return any info
 * about the intersection point. If needed, use the 
 * intersect(const Ray& ray, Intersection& is) function instead, which computes
 * all necessary data and stores it in the supplied Intersection object.
 * Note that this function is slower than the simple form, and should only be
 * used if the additional information is needed.
 */
class Intersectable
{
public:
	virtual ~Intersectable() { }
	virtual bool intersect(const Ray& ray) const = 0;
	virtual bool intersect(const Ray& ray, Intersection& is) const = 0;
	virtual void getAABB(AABB& bb) const = 0;
	virtual UV calculateTextureDifferential(const Point3D& p, const Vector3D& dp) const = 0;
	virtual Vector3D calculateNormalDifferential(const Point3D& p, const Vector3D& dp, bool isFrontFacing) const = 0;
 
	virtual bool isSphere() const = 0;
	virtual void setObj(void* obj) = 0;
	virtual unsigned int getIndex() = 0;
	int rayID = 0;
	unsigned int index;
};

#endif
