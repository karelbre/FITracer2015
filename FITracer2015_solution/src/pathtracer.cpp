/*
	Name: pathtracer.cpp
	Desc: CPU compute PT.
	Author: FredrikAppelros on GitHub, Karel Brezina (xbrezi13)
*/

#include "defines.h"
#include "scene.h"
#include "camera.h"
#include "ray.h"
#include "intersection.h"
#include "material.h"
#include "emissive.h"
#include "pathtracer.h"
#include "timer.h"
#include "image.h"
#include "lightprobe.h"
#include "SDLGLContext.h"

PathTracer::PathTracer(Scene* scene, Image* img) : Raytracer(scene,img)
{
}

PathTracer::~PathTracer()
{
}

void PathTracer::computeFirstImage(SDLGLContext* context, GLfloat* pixels)
{
	std::cout << "Pathtracing..." << std::endl;

	Color c;
	int index = 0;
	int width = mImage->getWidth();
	int height = mImage->getHeight();

	int lines = 0;
	int ID = 0;
//	#pragma omp parallel for 
	for (int y = 0; y < height; y++) { 
		for (int x = 0; x < width; x++) { 
			context->SeizeSem(1);
			Color c = tracePixel(x,y, ID); 
			pixels[index] = c.r;
			pixels[index+1] = c.g;
			pixels[index+2] = c.b;
			pixels[index+3] = 1.f;
			index += 4;
			context->LeaveSem(1);
		} 
//		#pragma omp critical 
		{ 
			lines++; 
			if (lines % ((height / 100)) == 0 || lines == height) {
				int perc = (100 * lines / height);
				std::cout << perc << "%" << std::endl;
				context->SetPercent(perc);
			}	
		} 
	} 
}

void PathTracer::computeImage(SDLGLContext* context, GLfloat* pixels)
{
	std::cout << "Pathtracing..." << std::endl;

	Color c;
	int index = 0;
	int width = mImage->getWidth();
	int height = mImage->getHeight();
	unsigned int samples = context->GetSamples();
	float majorColor = (float)((samples) / (samples + 1.0));
	float minorColor = (float)(1.0 / (samples + 1.0));

	int lines = 0;
	int ID = 0;
//#pragma omp parallel for 
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			context->SeizeSem(1);
			Color c = tracePixel(x, y, ID);
			pixels[index] = (pixels[index] * majorColor) + (c.r * minorColor);
			pixels[index + 1] = (pixels[index + 1] * majorColor) + (c.g * minorColor);
			pixels[index + 2] = (pixels[index + 2] * majorColor) + (c.b * minorColor);
			pixels[index + 3] = 1.f;
			index += 4;
			context->LeaveSem(1);
		}
//#pragma omp critical 
		{
			lines++;
			if (lines % ((height / 100)) == 0 || lines == height) {
				int perc = (100 * lines / height);
				std::cout << perc << "%" << std::endl;
				context->SetPercent(perc);
			}
		}
	}
}

Color PathTracer::tracePixel(int x, int y, int& ID)
{
	Color pixelColor;

	if (x == 81 && y == 32) {
		printf("");
	}

	float sx = float(static_cast<float>(x)+(1.0 + uniform()));
	float sy = float(static_cast<float>(y)+(1.0 + uniform()));

	// Let the camera setup the ray.
	Ray ray = mCamera->getRay(sx,sy);
	ID++;
	ray.ID = ID;

	pixelColor += trace(ray, 0, ID);

	return pixelColor;
}

Color PathTracer::trace(const Ray& ray, int depth, int& ID)
{
	static int MINIMUM_DEPTH = 4;
	static float p_absorption = 0.1f;
	static float absorption_factor = 1 / (1 - p_absorption);
	float seed = 0.1f;

	Intersection is;
	is.mHitTime = INF;
	if (mScene->intersect(ray, is)) {
		Color emittedLight, directLight, indirectLight;
		Material* material = is.mMaterial;

		Emissive* em = dynamic_cast<Emissive*>(material);
		if(em)
			return em->evalBRDF(is, Vector3D());

		float reflectivity = material->getReflectivity(is);
		float transparency = material->getTransparency(is);

		float light_type = uniform();

		if (light_type <= reflectivity) {
			Ray reflectedRay = is.getReflectedRay();
			ID++;
			reflectedRay.ID = ID;
			emittedLight = trace(reflectedRay, depth + 1, ID);
		} else if (light_type - reflectivity <= transparency) {
			Ray refractedRay = is.getRefractedRay();
			ID++;
			refractedRay.ID = ID;
			emittedLight = trace(refractedRay, depth + 1, ID);
		} else {
			// Direct lighting
			for (int i = 0; i < mScene->getNumberOfLights(); ++i) {
				PointLight* light = mScene->getLight(i);

				Ray shadowRay = is.getShadowRay(light);
				ID++;
				shadowRay.ID = ID;
				if (mScene->intersect(shadowRay))
					continue;

				float intensity = pow(shadowRay.maxT, -2);
				Color incomingRadiance = light->getRadiance() * intensity;
				Vector3D lightVec = light->getWorldPosition() - is.mPosition;
				lightVec.normalize();
				Color brdf = material->evalBRDF(is, lightVec);
				float incidentAngle = max(lightVec * is.mNormal, 0.0f);
				directLight += incomingRadiance * brdf * incidentAngle;
			}	

			// Indirect lighting
			if (depth <= MINIMUM_DEPTH || uniform() > p_absorption) {
				float theta, phi;
				Vector3D n_x, n_y, n_z;
				float x_b, y_b, z_b;
				Vector3D dir_b, dir;
			
				theta = acos(sqrt(1 - uniform()));
				phi = float(2 * M_PI * uniform());

				Vector3D up(1.0f, 0.0f, 0.0f);
				if (fabsf(is.mNormal.x) > 0.75f) {
					up = Vector3D(0.0f, 1.0f, 0.0f);
				}

				n_x = up % is.mNormal;
				n_x.normalize();
				n_y = n_x % is.mNormal;
				n_z = is.mNormal;

				x_b = cos(phi) * sin(theta);
				y_b = sin(phi) * sin(theta);
				z_b = cos(theta);
				dir = x_b * n_x + y_b * n_y + z_b * n_z;

				Ray pathRay;
				ID++;
				pathRay.ID = ID;
				pathRay.orig = is.mPosition;
				pathRay.dir = dir;
				Color brdf = material->evalBRDF(is, dir);
				indirectLight = (float)M_PI * trace(pathRay, depth + 1, ID) * brdf;
				if (depth > MINIMUM_DEPTH) {
					indirectLight *= absorption_factor;
				}
			}

			emittedLight = directLight + indirectLight;
		}
		
		return emittedLight;
	} 
	return Color(0,0,0);
}