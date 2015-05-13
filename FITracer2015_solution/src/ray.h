/*
 *  ray.h
 *  asrTracer
 *
 *  Created by Petrik Clarberg on 2006-02-24.
 *  Copyright 2006 Lund University. All rights reserved.
 *
 */

#ifndef RAY_H
#define RAY_H

#include "defines.h"
#include "matrix.h"

/**
 * Class representing a ray in 3D space.
 * The ray's attributes are publicly accessible, and consist
 * of the ray origin and direction, plus parametric min/max
 * along the ray. A point along the ray can be found by taking
 * origin + t*dir, where t is the time parameter specifying how
 * far away from the origin we are.
 */
class Ray
{
public:
	struct Differential
	{
		Vector3D dx;
		Vector3D dy;
		
		Differential() {}
		Differential(const Vector3D& dx, const Vector3D& dy) : dx(dx), dy(dy) {}
	};

	Point3D orig;		///< Origin of ray.
	Vector3D dir;		///< Direction of ray.
	float minT;			///< Start time of ray.
	float maxT;			///< End time of ray.
	Differential dp;	///< Origin ray differential.
	Differential dd;	///< Direction ray differential.
	int ID;
	
public:
	/// Default Constructor. The position and direction are left
	/// un-initialized, but the time is set to 0 and infinity respectively.
	Ray() : minT(0.001f), maxT(INF) { }
	
	Ray(const Point3D& o, const Vector3D& d, float mint=0.001f, float maxt=INF)
	: orig(o), dir(d), minT(mint), maxT(maxt)
	{
		dir.normalize();
	}
	
	/// Constructor initializing the ray's origin and direction,
	/// and optionally the time parameters. The direction is normalized.
	Ray(const Point3D& o, const Vector3D& d, const Differential& dp = Differential(), const Differential& dd = Differential(), float mint=0.001f, float maxt=INF)
		: orig(o), dir(d), dp(dp), dd(dd), minT(mint), maxT(maxt)
	{
		dir.normalize();
	}
	
	/// Destructor.
	~Ray() { }
};

#endif
