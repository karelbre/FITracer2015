/*
 *  camera.h
 *  asrTracer
 *
 *  Created by Petrik Clarberg on 2006-02-14.
 *  Copyright 2006 Lund University. All rights reserved.
 *
 */

#ifndef CAMERA_H
#define CAMERA_H

#include "node.h"

//#define KERNEL_TYPES 1 

//#ifdef KERNEL_TYPES
#include "gpu_types.h"
//#endif

class Image;	
class Ray;

/**
 * Class representing a simple perspective camera. 
 * The camera needs to know which Image object is used for the 
 * output image in order to extract the image dimensions.
 * The camera can be conveniently setup using the setLookAt()
 * function, which takes a position, target and up vector.
 * The field of view parameter measures the FOV in degrees
 * horizontally.
 */
class Camera : public Node
{
public:
	Camera(Image* img);
	virtual ~Camera();
	
	void setLookAt(const Point3D& pos, const Point3D& target, const Vector3D& up, float fov, float near=1.0f, float far=1000.0f);
	virtual Ray getRay(float x, float y) const;

	void getSettings(TCamera& cam);

protected:
	void prepare();
	
protected:
	Image* mImage;				///< Ptr to the output image.
	Point3D mOrigin;				///< Position of camera origin.
	Vector3D mForward;			///< Camera frame forward vector.
	Vector3D mRight;				///< Camera frame right vector.
	Vector3D mUp;					///< Camera frame up vector.
	float mNearPlane;			///< Distance to near viewing plane.
	float mFarPlane;			///< Distance to far viewing plane.
	float mFieldOfView;			///< Horizontal field-of-view
	float mImageExtentX;		///< Extent of image plane in positive x direction.
	float mImageExtentY;		///< Extent of image plane in positive y direction.
};

#endif
