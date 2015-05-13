/*
 *  material.h
 *  asrTracer
 *
 *  Created by Petrik Clarberg on 2006-03-02.
 *  Copyright 2006 Lund University. All rights reserved.
 *
 */

#ifndef MATERIAL_H
#define MATERIAL_H

#include "matrix.h"
#include "color.h"
#include "gpu_types.h"
#include "diffuse.h"

class Intersection;

/**
 * Base class for classes representing materials (Diffuse, Phong, Checker).
 * This class keeps track of the reflectivity, transparency and refraction 
 * index of the material, while the sub classes implement the various BRDFs.
 */
class Material
{
public:
	Material(float r=0.0f, float t=0.0f, float n=1.0f) : mReflectivity(r), mTransparency(t), mRefractionIndex(n) { }
	virtual ~Material() { }

	/// Returns the BRDF at the intersection is for the light direction L.
	virtual Color evalBRDF(const Intersection& is, const Vector3D& L) = 0;
		
	/// Returns the reflectivity of the material in the range [0,1] at the intersection,
	/// where 0 means not reflective at all, and 1 gives a perfect mirror.
	virtual float getReflectivity(const Intersection& is) const { return mReflectivity; }
	virtual float getReflectivity() const { return mReflectivity; }
	
	/// Sets the reflectivity of the material.
	virtual void setReflectivity(float r) { mReflectivity=r; }
	
	/// Returns the transparency of the material in the range [0,1] at the intersection,
	/// where 0 is fully opaque, and 1 is fully transparent.
	virtual float getTransparency(const Intersection& is) const { return mTransparency; }
	virtual float getTransparency() const { return mTransparency; }

	/// Sets the transparency of the material.
	virtual void setTransparency(float t) { mTransparency=t; }
	
	/// Returns the index of refraction for the material.
	virtual float getIndexOfRefraction() const { return mRefractionIndex; }
	
	/// Sets the index of refraction. Empty space is 1, denser materials higher.
	virtual void setIndexOfRefraction(float n) { mRefractionIndex=n; }

	/// Sets the name of the material.
	void setName(const std::string &name) { mName = name; }

	/// Sets the name of the material.
	std::string getName() const { return mName; }

	void getMaterial(TMaterial& mat) {
		mat.reflectivity = mReflectivity;
		mat.transparency = mTransparency;
		mat.refractionIndex = mRefractionIndex;
	}

protected:
	std::string mName;			///< Optional name of material.
	float mReflectivity;		///< The reflectivity of the material.
	float mTransparency;		///< The transparency of the material.
	float mRefractionIndex;		///< Index of refraction for the material.
};

#endif
