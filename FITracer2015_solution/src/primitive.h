/*
 *  primitive.h
 *  asrTracer
 *
 *  Created by Petrik Clarberg on 2006-02-14.
 *  Copyright 2006 Lund University. All rights reserved.
 *
 */

#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "node.h"

class Material;

/**
 * Base class for classes representing geometry (Mesh, Sphere, Plane). 
 * This class keeps track of the material of the primitive, and provides
 * functions for setting/getting the material.
 */
class Primitive : public Node
{
public:
	Primitive(Material* m=0);
	virtual ~Primitive() { }
	
	void setMaterial(Material* m);
	
	/// Returns a pointer to the primitive's material.
	Material* getMaterial() const { return mMaterial; }
	
protected:
	Material* mMaterial;	///< Ptr to the material used by the primitive.
};

#endif
