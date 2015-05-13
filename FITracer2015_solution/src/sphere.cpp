/*
 *  sphere.cpp
 *  asrTracer
 *
 *  Created by Petrik Clarberg on 2006-02-14.
 *  Copyright 2006 Lund University. All rights reserved.
 *
 */

#include "defines.h"
#include "sphere.h"
	
/**
 * Creates a sphere primitive.
 */
Sphere::Sphere() : Primitive(), mRadius(0.5f)
{

}

/**
 * Creates a sphere at origin with radius r and material m.
 */
Sphere::Sphere(float r, Material* m) : Primitive(m), mRadius(r)
{
}

/**
 * Destroys the sphere.
 */
Sphere::~Sphere()
{
}

/**
 * Sets the radius of the sphere.
 */
void Sphere::setRadius(float r)
{
	mRadius = r;
}

/**
 * Prepare sphere for rendering. This function computes the world->object
 * transform (inverse of the mWorldTransform matrix), so that intersection
 * tests can be performed in object space.
 */
void Sphere::prepare()
{
	// Compute inverse world transform.
	mInvWorldTransform = mWorldTransform.inverse();
}

/**
 * Append all intersectable geometry in this object to the array.
 * In this case, the Sphere itself is intersectable, so just add the this pointer.
 */
void Sphere::getGeometry(std::vector<Intersectable*>& geometry)
{
	geometry.push_back(this);
}

/**
 * Compute the two solutions to the quadratic A*t^2 + B*t + C = 0 and return
 * true if there are real solutions, and false if no real solutions.
 */
bool Sphere::solveQuadratic(float A, float B, float C, float& t0, float& t1) const
{
	// The solutions are given by:
	//
	//   t0 = (-B - sqrt(B^2-4AC) / 2A
	//   t1 = (-B + sqrt(B^2-4AC) / 2A
	//
	// We first compute the discriminant B^2-4AC, if this is negative,
	// there are no real solutions and we return false.
	// This method of solving the quadratic can be numerically
	// instable if B is close to the B^2-4AC, to avoid problems
	// a more stable form can be used (see PRT book).	

	float d = B*B - 4.0f*A*C;
	
	if (d < 0.0f)
		return false;
	
	d = std::sqrt(d);
	
	t0 = (-B - d) / (2.0f*A);
	t1 = (-B + d) / (2.0f*A);
	return true;
}

// Implementation of the Intersectable interface.

/**
 * Returns true if the ray intersects the sphere.
 */
bool Sphere::intersect(const Ray& ray) const
{
	// First, translate the ray to object space.
	Point3D o = mInvWorldTransform * ray.orig;
	Vector3D d = mInvWorldTransform * ray.dir;
	
	// Compute polynom coefficients.
	float A = d.x*d.x + d.y*d.y + d.z*d.z;
	float B = 2.0f * ( d.x*o.x + d.y*o.y + d.z*o.z );
	float C = o.x*o.x + o.y*o.y + o.z*o.z - mRadius*mRadius;

	// Solve quadratic equation for ray enter/exit point t0,t1 respectively.
	float t0, t1;
	if(!solveQuadratic(A,B,C,t0,t1)) return false;	// no real solutions -> ray missed

	if(t0>ray.maxT || t1<ray.minT) return false;	// sphere before/after ray
	if(t0<ray.minT && t1>ray.maxT) return false;	// ray inside sphere
	return true;
}

/**
 * Returns true if the ray intersects the sphere. 
 * Information about the hit is returned in the Intersection object.
 */
bool Sphere::intersect(const Ray& ray, Intersection& isect) const
{
	// First, translate the ray to object space.
	Point3D o = mInvWorldTransform * ray.orig;
	Vector3D d = mInvWorldTransform * ray.dir;
	
	// Compute polynom coefficients.
	float A = d.x*d.x + d.y*d.y + d.z*d.z;
	float B = 2.0f * ( d.x*o.x + d.y*o.y + d.z*o.z );
	float C = o.x*o.x + o.y*o.y + o.z*o.z - mRadius*mRadius;
	
	// Solve quadratic equation for ray enter/exit point t0,t1 respectively.
	float t0, t1;
	if(!solveQuadratic(A,B,C,t0,t1)) return false;	// no real solutions -> ray missed

	if(t0>ray.maxT || t1<ray.minT) return false;	// sphere before/after ray	
	if(t0<ray.minT && t1>ray.maxT) return false;	// ray inside sphere

	// Compute hit position & normal at hit point.
	float t = t0<ray.minT ? t1 : t0;		// ray hit time
	Point3D p = o + t*d;						// hit point in object space
	Vector3D n = p;				// since sphere is centered about origin in object space,
	n /= mRadius;
	
	// Compute spherical coordinates (theta,phi) in the range [0,1] for use as texture coords.
	float u = std::atan2(-p.z,p.x) / (2.0f*M_PI);	// phi (angle around perimeter, CCW seen from top)
	if(u<0.0f) u+=1.0f;
	float v = std::acos(p.y/mRadius) / M_PI;		// theta (angle from top of sphere)
	
	if (v != v) // Check for NaN
		v = p.y > 0.0f ? 0.0f : 1.0f;
	
	// Compute info about the intersection point.
	isect.mRay = ray;
	isect.mObject = this;					// The object by the ray (this object itself).
	isect.mMaterial = getMaterial();		// Store ptr to the material.
	isect.mHitTime = t;						// Store hit time parameter t.
	isect.mPosition = mWorldTransform * p;	// Store world space hit point.
	isect.mNormal = mWorldTransform * n;	// Store world space normal.
	isect.mNormal.normalize();	// just in case
	isect.mView = -ray.dir;					// View direction is negative ray direction.
	isect.mFrontFacing = isect.mView.dot(isect.mNormal) > 0.0f;
	if (!isect.mFrontFacing) isect.mNormal = -isect.mNormal;
	isect.mTexture = UV(u,v);				// Use spherical coordinates as texture coords.
	isect.mHitParam = UV(u,v);				// Store spherical coordinates.

	return true;
}

/**
 * Computes an axis-aligned bounding box enclosing the sphere in world space.
 */
void Sphere::getAABB(AABB& bb) const
{
	// To account for non-uniform scaling and arbitrary rotation/translation,
	// we compute the location of the eight corners of a box enclosing the sphere
	// in object space, and then setup the box that includes all corners translated
	// to world space. This works but is not always optimal.
	bb = AABB();
	
	for(int i = 0; i < 8; i++) {
		Point3D p( (float)(2*(i&1)-1) * mRadius,
				 (float)(2*((i&2)>>1)-1) * mRadius,
				 (float)(2*((i&4)>>2)-1) * mRadius );
		bb.include(mWorldTransform * p);
	}	
}

UV Sphere::calculateTextureDifferential(const Point3D& p, const Vector3D& dp) const
{
	Point3D lp = mInvWorldTransform * p;
	Vector3D ldp = mInvWorldTransform * dp;
	
	float du = (ldp.z*lp.x - lp.z*ldp.x) / (lp.x*lp.x + lp.z*lp.z) * (-0.5f/M_PI);
	float dv = ldp.y/(mRadius*sqrtf(1.0f - lp.y*lp.y/(mRadius*mRadius))) * (-1.0f/M_PI);
	
	return UV(du, dv);
}

Vector3D Sphere::calculateNormalDifferential(const Point3D& p, const Vector3D& dp, bool isFrontFacing) const
{
	float sign = isFrontFacing ? 1.0f : -1.0f;
	return sign * dp / mRadius;
}
