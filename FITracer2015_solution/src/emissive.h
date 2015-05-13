#ifndef EMISSIVE_H
#define EMISSIVE_H

#include "material.h"

class Emissive : public Material
{
public:
	Emissive(const Color& c);
	Color evalBRDF(const Intersection& is, const Vector3D& L);

protected:
	Color mColor;
};

#endif
