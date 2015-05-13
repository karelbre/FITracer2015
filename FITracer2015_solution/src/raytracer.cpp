/*
 *  raytracer.cpp
 *  asrTracer
 *
 *  Created by Petrik Clarberg on 2006-02-13.
 *  Copyright 2006 Lund University. All rights reserved.
 *
 */

#include "defines.h"
#include "scene.h"
#include "image.h"
#include "camera.h"
#include "ray.h"
#include "intersection.h"
#include "material.h"
#include "timer.h"
#include "raytracer.h"

/**
 * Creates a raytracer.
 * @param scene Point3Der to the scene.
 * @param img Point3Der to an Image object where the output will be stored.
 */
Raytracer::Raytracer(Scene* scene, Image* img) : mScene(scene), mImage(img), mCamera(0)
{
	if (!mScene || !mImage)
		throw std::runtime_error("(Raytracer::Raytracer) null pointer");
	
	if (mScene->getNumberOfCameras() < 1)
		throw std::runtime_error("scene has no camera");

	mCamera = mScene->getCamera(0); // Use the first camera.
}

