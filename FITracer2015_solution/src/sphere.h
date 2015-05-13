/*
 *  sphere.h
 *  asrTracer
 *
 *  Created by Petrik Clarberg on 2006-02-14.
 *  Copyright 2006 Lund University. All rights reserved.
 *
 */

#ifndef SPHERE_H
#define SPHERE_H

#include "primitive.h"
#include "intersectable.h"
#include "gpu_types.h"

/**
 * Class representing a sphere.
 * The sphere has a radius and is centered around origin by default.
 * By setting its transform, the translation/scale/orientation can
 * be changed. The ray/sphere intersection is done by transforming the
 * ray to object space, and solving the quadratic equation for the
 * sphere: x^2 + y^2 + z^2 = r^2.
 */
class Sphere : public Primitive, public Intersectable
{
public:
	Sphere();
	Sphere(float radius, Material* m=0);
	virtual ~Sphere();

	void setRadius(float r);
	
	// Implementation of the Intersectable interface.
	bool intersect(const Ray& ray) const;
	bool intersect(const Ray& ray, Intersection& isect) const;
	void getAABB(AABB& bb) const;
	UV calculateTextureDifferential(const Point3D& p, const Vector3D& dp) const;
	Vector3D calculateNormalDifferential(const Point3D& p, const Vector3D& dp, bool isFrontFacing) const;

	bool isSphere() const { return true; }

	void setObj(void* obj) {
		((TSphere*)obj)->radius = mRadius;
		setMatrixes(((TSphere*)obj)->InvWorldTransform, ((TSphere*)obj)->WorldTransform);
		setMaterial(((TSphere*)obj)->material);
	}

	unsigned int getIndex() { return index; }

	unsigned int setIndex(unsigned int _index)
	{
		index = _index;
		return index + 1;
	}

protected:
	void prepare();
	void getGeometry(std::vector<Intersectable*>& geometry);
	bool solveQuadratic(float A, float B, float C, float& t0, float& t1) const;
	
	void setMatrixes(TMatrix& mat, TMatrix& mat2) {
		for (int i = 0; i < 4; i++) {
			mat.m[i].s[0] = mInvWorldTransform.getNumIndex((i * 4) + 0);
			mat.m[i].s[1] = mInvWorldTransform.getNumIndex((i * 4) + 1);
			mat.m[i].s[2] = mInvWorldTransform.getNumIndex((i * 4) + 2);
			mat.m[i].s[3] = mInvWorldTransform.getNumIndex((i * 4) + 3);
		}

		for (int i = 0; i < 4; i++) {
			mat2.m[i].s[0] = mWorldTransform.getNumIndex((i * 4) + 0);
			mat2.m[i].s[1] = mWorldTransform.getNumIndex((i * 4) + 1);
			mat2.m[i].s[2] = mWorldTransform.getNumIndex((i * 4) + 2);
			mat2.m[i].s[3] = mWorldTransform.getNumIndex((i * 4) + 3);
		}
	}


	void setMaterial(TMaterial& mat) {
		((Diffuse*)this->mMaterial)->getColor(mat.color);
		((Diffuse*)this->mMaterial)->getMaterial(mat);
		mat.isMat = true;
	}

protected:
	float mRadius;					///< Radius of sphere.
	Matrix mInvWorldTransform;		///< World->Object transform.	
};

#endif
