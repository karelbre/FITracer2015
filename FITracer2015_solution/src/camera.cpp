/*
 *  camera.cpp
 *  asrTracer
 *
 *  Created by Petrik Clarberg on 2006-02-28.
 *  Copyright 2006 Lund University. All rights reserved.
 *
 */

#include "defines.h"
#include "ray.h"
#include "image.h"
#include "camera.h"

using namespace std;

/** Default constructor. The image buffer must be given as an argument, 
*	so the camera can extract the image dimensions automatically.
*/
Camera::Camera(Image* img) : Node(), mImage(img)
{
	if (!mImage)
		throw std::runtime_error("(Camera::Camera) image null pointer");

	std::cout << "creating camera" << std::endl;
}

/**
 * Destructor. Does nothing.
 */
Camera::~Camera()
{
}

/**
 * Setup the camera frame by specifying the camera's position, target, and up vector.
 */
void Camera::setLookAt(const Point3D& pos, const Point3D& target, const Vector3D& up, float fov, float near, float far)
{
	// Set camera origin, near/far plane.
	mOrigin = pos;
	mNearPlane = near;
	mFarPlane = far;
	
	// Setup orthogonal camera frame defined by right,forward,up vectors.
	mForward = target - pos;
	mForward.normalize();
	mRight = mForward % up;
	mRight.normalize();
	mUp = mRight % mForward;

	// Compute extent of image plane from the given field-of view,
	// where the image plane is at unit distance from the origin
	// in the local camera frame.
	float aspect = (float)mImage->getWidth() / (float)mImage->getHeight(); // Compute aspect ratio.
	mImageExtentX = std::tan(0.5f * fov * M_PI/180.0f);
	mImageExtentY = std::tan(0.5f * fov/aspect * M_PI/180.0f);
	mFieldOfView = fov;
}

/**
 * Returns a ray that goes through image plane location (x,y),
 * where the coordinate is given in terms of image pixel dimensions.
 */
Ray Camera::getRay(float x, float y) const
{
	Vector3D right = 2.0f/(float)mImage->getWidth() * mImageExtentX * mRight;
	Vector3D up = -2.0f/(float)mImage->getHeight() * mImageExtentY * mUp;
	Vector3D view = mForward - mImageExtentX * mRight + mImageExtentY * mUp;

	Vector3D d = view + x*right + y*up;
	float dLength = d.length();

	float r = 1.0f / (dLength*dLength*dLength);

	Ray::Differential dp(Vector3D(0.0f, 0.0f, 0.0f), Vector3D(0.0f, 0.0f, 0.0f));
	Ray::Differential dd((d.dot(d) * right - d.dot(right) * d) * r, (d.dot(d) * up - d.dot(up) * d) * r);

	d /= dLength;
	return Ray(mOrigin, d, dp, dd, mNearPlane, mFarPlane);
}

/**
 * Prepare the camera for use in raytracing by transforming its frame to world coordinates.
 */
void Camera::prepare()
{
	// Transform coordinate origin and frame to world space.
	mOrigin = mWorldTransform * mOrigin;
	mForward = mWorldTransform * mForward;
	mRight = mWorldTransform * mRight;
	mUp = mWorldTransform * mUp;
}

/// Get actual settings for kernel structure.
/// cam is reference to kernel structure.
/// ---
void Camera::getSettings(TCamera& cam) {
	cam.mImage.height = mImage->getHeight();
	cam.mImage.width = mImage->getWidth();
	Point3DtoFloat3(mOrigin, cam.mOrigin);
	Vector3DToFloat3(mForward, cam.mForward);
	Vector3DToFloat3(mRight, cam.mRight);
	Vector3DToFloat3(mUp, cam.mUp);
	cam.mNearPlane = mNearPlane;
	cam.mFarPlane = mFarPlane;
	cam.mFieldOfView = mFieldOfView;
	cam.mImageExtentX = mImageExtentX;
	cam.mImageExtentY = mImageExtentY;
}
