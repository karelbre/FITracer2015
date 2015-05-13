/*
 *  primitive.cpp
 *  asrTracer
 *
 *  Created by Petrik Clarberg on 2006-03-10.
 *  Copyright 2006 Lund University. All rights reserved.
 *
 */

#include "defines.h"
#include "primitive.h"

/**
 * Base class constructor for primitives, sets the material to m.
 * If m is 0, the primitive is assigned the default material.
 */
Primitive::Primitive(Material* m) : Node(), mMaterial(m)
{
	if (mMaterial == 0)
		mMaterial = &DEFAULT_MATERIAL;
}

/**
 * Sets the material of the primitive to the material pointed to by m,
 * or if m is 0, the material is set to the default material (diffuse gray).
 */
void Primitive::setMaterial(Material* m)
{
	mMaterial = (m==0) ? &DEFAULT_MATERIAL : m;
}
