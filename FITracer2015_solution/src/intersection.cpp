/*
 *  intersection.cpp
 *  prTracer
 *
 *  Created by Rasmus Barringer on 2011-02-04.
 *  Copyright 2011 Lund University. All rights reserved.
 *
 */

#include "intersection.h"
#include "intersectable.h"

Ray::Differential Intersection::calculatePositionDifferential() const
{
	const Vector3D& D = mRay.dir;
	const Vector3D& N = mNormal;

	Ray::Differential dp;
	
	Vector3D negDproj = -D / D.dot(N);
	Vector3D t;

	t = mRay.dp.dx + mHitTime*mRay.dd.dx;
	dp.dx = t + t.dot(N)*negDproj;

	t = mRay.dp.dy + mHitTime*mRay.dd.dy;
	dp.dy = t + t.dot(N)*negDproj;

	return dp;
}

Ray Intersection::getReflectedRay() const
{
	const Vector3D& D = mRay.dir;
	Vector3D N = mNormal;

	Vector3D V = -D;

	float tmp = N * V;

	Vector3D R = 2 * (N * V) * N - V; // Resulting reflection vector

	Ray::Differential dp = calculatePositionDifferential();
	Ray::Differential dd;
	Vector3D dn;

	float dDotN = D.dot(N);
	
	dn = mObject->calculateNormalDifferential(mPosition, dp.dx, mFrontFacing);
	dd.dx = mRay.dd.dx - 2.0f*(dDotN*dn + (mRay.dd.dx.dot(N) + D.dot(dn))*N);
	
	dn = mObject->calculateNormalDifferential(mPosition, dp.dy, mFrontFacing);
	dd.dy = mRay.dd.dy - 2.0f*(dDotN*dn + (mRay.dd.dy.dot(N) + D.dot(dn))*N);

	return Ray(mPosition + N*0.001f, R, dp, dd);
}

Ray Intersection::getRefractedRay() const
{
	const Vector3D& D = mRay.dir;
	Vector3D N = mNormal;

	float iEta = 1.0f;
	float tEta = mMaterial->getIndexOfRefraction();

	if (!mFrontFacing)
		swap(iEta, tEta); // Inside.

	float eta = iEta / tEta;

	float r = -D * N;
	float c = 1 - eta*eta * (1 - r*r);
	if (c < 0)
		return getReflectedRay();
	Vector3D T = eta * D + (eta * r - sqrt(c)) * N; // Resulting refraction vector

	float mu = eta*N.dot(D) - N.dot(T);

	Ray::Differential dp = calculatePositionDifferential();
	Ray::Differential dd;
	Vector3D dn;

	float dmu;
	float dmu0 = (eta - eta*eta*D.dot(N) / T.dot(N));

	dn = mObject->calculateNormalDifferential(mPosition, dp.dx, mFrontFacing);
	dmu = dmu0 * (mRay.dd.dx.dot(N) + D.dot(dn));
	dd.dx = eta*mRay.dd.dx - (mu*dn + dmu*N);
	
	dn = mObject->calculateNormalDifferential(mPosition, dp.dy, mFrontFacing);
	dmu = dmu0 * (mRay.dd.dy.dot(N) + D.dot(dn));
	dd.dy = eta*mRay.dd.dy - (mu*dn + dmu*N);

	return Ray(mPosition - N*0.001f, T, dp, dd);
}

Ray Intersection::getShadowRay(PointLight *light) const
{
	// Un-normalized light vector
	Vector3D lightVec = light->getWorldPosition() - mPosition;
	
	// Length of light vector - we don't want to go beyond this in our shadow ray tests
	float len = lightVec.length();

	// Ray from hit point towards the light. Note that lightVec will be normalized by the Ray constructor
	return Ray(mPosition + mNormal * 0.001f, lightVec, 0.0f, len);
}


