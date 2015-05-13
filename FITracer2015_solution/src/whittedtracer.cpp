/*
 *  whittedtracer.cpp
 *  asrTracer
 *
 *  Created by Petrik Clarberg on 2006-03-10.
 *  Copyright 2006 Lund University. All rights reserved.
 *
 */

#include "defines.h"
#include "scene.h"
#include "camera.h"
#include "ray.h"
#include "intersection.h"
#include "material.h"
#include "whittedtracer.h"
#include "timer.h"
#include "image.h"

/**
 * Creates a Whitted raytracer. The parameters are passed on to the base class constructor.
 */
WhittedTracer::WhittedTracer(Scene* scene, Image* img) : Raytracer(scene,img)
{
}


WhittedTracer::~WhittedTracer()
{
}

/**
 * Raytraces the scene by calling tracePixel() for each pixel in the output image.
 * The function tracePixel() is responsible for computing the output color of the
 * pixel. This should be done in a sub class derived from the Raytracer base class.
 */
void WhittedTracer::computeImage()
{
	std::cout << "Raytracing..." << std::endl;
	//Timer timer;
	
	Color c;
	int width = mImage->getWidth();
	int height = mImage->getHeight();
	int proc = 0;

	// Loop over all pixels in the image
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			// Raytrace the pixel at (x,y).
			c = tracePixel(x,y);
			
			// Store the result in the image.
			mImage->setPixel(x,y,c);
		}
		
		// Print progress approximately every 5%.
		if ((y+1) % (height/100) == 0 || (y+1) == height)
			std::cout << (100*(y+1)/height) << "% done" << std::endl;
	}
	
	//std::cout << "Done in " << timer.stop() << " seconds" << std::endl;
}

/**
 * Compute the color of the pixel at (x,y) by raytracing. 
 * The default implementation here just traces through the center of
 * the pixel.
 */
Color WhittedTracer::tracePixel(int x, int y)
{
	Color pixelColor = Color(0.0f, 0.0f, 0.0f);

	float cx = (float)x + 0.5f;
	float cy = (float)y + 0.5f;
	
	// Let the camera setup the ray.
	Ray ray = mCamera->getRay(cx,cy);
	
	return trace(ray, 2);

	static int NUMBER_OF_SAMPLES = 3;

	/*Color pixelColor = Color(0.0f, 0.0f, 0.0f);
	float s_inv = 1.0f / NUMBER_OF_SAMPLES;

	for (int i = 0; i < NUMBER_OF_SAMPLES; ++i) {
		for (int j = 0; j < NUMBER_OF_SAMPLES; ++j) {
			float sx = static_cast<float>(x)+(i + uniform()) * s_inv;
			float sy = static_cast<float>(y)+(j + uniform()) * s_inv;

			// Let the camera setup the ray.
			Ray ray = mCamera->getRay(sx, sy);

			pixelColor += trace(ray, 1);
		}
	}

	return pow(s_inv, 2) * pixelColor;*/
}

/**
 * Computes the radiance returned by tracing the ray r.
 */
Color WhittedTracer::trace(const Ray& ray, int depth)
{

	Intersection is;

	if (mScene->intersect(ray, is)) {
		
		Color reflection, refraction;
		if (depth > 0) {
			Ray r = is.getReflectedRay();
			reflection = trace(r, depth - 1);

			r = is.getRefractedRay();
			refraction = trace(r, depth - 1);
		}

		int cLight = mScene->getNumberOfLights();
		Color fin;

		for (int i = 0; i < cLight; i++) {
			PointLight* light = mScene->getLight(i);

			Ray r = is.getShadowRay(light);
			if (mScene->intersect(r))
				continue;

			Vector3D lightVec = light->getWorldPosition() - is.mPosition;
			lightVec.normalize();

			


			fin +=  light->getRadiance() * is.mMaterial->evalBRDF(is, lightVec) * (maxT(lightVec.dot(is.mNormal), 0.0f));
		}

		fin += (1 - is.mMaterial->getReflectivity(is) - is.mMaterial->getTransparency(is)) * fin;
		fin	+= is.mMaterial->getReflectivity(is) * reflection;
		fin += is.mMaterial->getTransparency(is) * refraction;

		return fin;
	}
	else
		return Color(0, 0, 0);
}
// OK