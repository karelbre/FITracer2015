#include "defines.h"
#include "intersection.h"
#include "emissive.h"

using namespace std;

Emissive::Emissive(const Color& c) : Material(0.0f, 0.0f, 0.0f), mColor(c)
{
}

Color Emissive::evalBRDF(const Intersection& is, const Vector3D& L)
{
	return mColor;
}


