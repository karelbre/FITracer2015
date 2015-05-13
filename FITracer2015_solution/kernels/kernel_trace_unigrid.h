/*
	Name: kernel_trace_unigrid.h
	Desc: Trace and intersection functions for UNIFORM GRID PT.
	Author: Karel Brezina (xbrezi13)
*/

#ifndef _KERNEL_TRACE_UNIGRID_H_
#define _KERNEL_TRACE_UNIGRID_H_

#include "kernel_types.h"
#include "kernel_functions.h"

// Intersect object without information about it.
// ray -> information about ray
// sp -> buffer of all spheres
// tr -> buffer of all triangles
// me -> buffer of all meshes
// ra_me -> buffer of size every mesh
// @return -> successful of intersect 
bool intersect_unigrid(TRay* ray, __global TSphere* sp, unsigned int cnt_sp,
	__global TTriangle* tr, unsigned int cnt_tr, __global TMesh* me,
	__global unsigned int* ra_me, unsigned int cnt_ra_me,
	__global TUniGrid* infoUniGrid, __global TBoxLink* uniGrid, __global TObject* objects)
{
	float xAxis, yAxis, zAxis;
	TVector3D dir = ray->dir;
	normalizeVector3D(&dir); // size of step

	TVector3D tDelta;
	TPoint3D tMax;
	TPoint3D step;

	tMax.x = ray->orig.x + fabs(infoUniGrid->boxMin.x);
	tMax.y = ray->orig.y + fabs(infoUniGrid->boxMin.y);
	tMax.z = fabs(infoUniGrid->boxMin.z) - ray->orig.z;

	int X, Y, Z, id;
	X = (int)(tMax.x / infoUniGrid->cell_size.x);
	Y = (int)(tMax.y / infoUniGrid->cell_size.y);
	Z = (int)(tMax.z / infoUniGrid->cell_size.z); // index to cells

	if (dir.x < 0.f) {
		step.x = -1;
		tMax.x -= X*infoUniGrid->cell_size.x;
	}
	else {
		step.x = 1;
		tMax.x = ((X + 1)*infoUniGrid->cell_size.x) - tMax.x;
	}
	if (dir.y < 0.f) {
		step.y = -1;
		tMax.y -= (Y*infoUniGrid->cell_size.y);
	}
	else {
		step.y = 1;
		tMax.y = ((Y + 1)*infoUniGrid->cell_size.y) - tMax.y;
	}
	if (dir.z < 0.f) {
		step.z = 1;
		tMax.z = ((Z + 1)*infoUniGrid->cell_size.z) - tMax.z;
	}
	else {
		step.z = -1;
		tMax.z -= (Z*infoUniGrid->cell_size.z);
	}

	tDelta.x = infoUniGrid->cell_size.x;
	tDelta.y = infoUniGrid->cell_size.y;
	tDelta.z = infoUniGrid->cell_size.z;

	int grid_size = infoUniGrid->grid_size;
	TSphere sphere;
	TTriangle triangle;

	while ((X < grid_size) && (X >= 0) &&
		(Y < grid_size) && (Y >= 0) &&
		(Z < grid_size) && (Z >= 0)) {

		id = (int)(X + Y * grid_size + Z * grid_size * grid_size);
		for (int i = uniGrid[id].objStartIndex; i < uniGrid[id].objSize; i++) {

			if (objects[i].type == SPHERE_INDEX) {
				sphere = sp[objects[i].index];

				if (sphereIntersect(&sphere, ray)) {
					return true;
				}
			}
			else {
				triangle = tr[objects[i].index];

				if (triangleIntersect(&triangle, ray, me, ra_me, cnt_ra_me)) {
					return true;
				}
			}
		}

		xAxis = (1 / fabs(dir.x) * tMax.x);
		yAxis = (1 / fabs(dir.y) * tMax.y);
		zAxis = (1 / fabs(dir.z) * tMax.z);

		if (xAxis < yAxis) {
			if (xAxis < zAxis) {
				tMax.x += tDelta.x;
				X += (int)step.x;
			}
			else {
				tMax.z += tDelta.z;
				Z += (int)step.z;
			}
		}
		else {
			if (yAxis < zAxis) {
				tMax.y += tDelta.y;
				Y += (int)step.y;
			}
			else {
				tMax.z += tDelta.z;
				Z += (int)step.z;
			}
		}
	}

	return false;
}

// Intersect object within information about it.
// ray -> information about ray
// is -> information about intersection
// sp -> buffer of all spheres
// tr -> buffer of all triangles
// me -> buffer of all meshes
// ra_me -> buffer of size every mesh
// @return -> successful of intersect 
bool intersectIs_unigrid(TRay* ray, TIntersect* is, __global TSphere* sp, unsigned int cnt_sp,
	__global TTriangle* tr, unsigned int cnt_tr, __global TMesh* me,
	__global unsigned int* ra_me, unsigned int cnt_ra_me,
	__global TUniGrid* infoUniGrid, __global TBoxLink* uniGrid, __global TObject* objects)
{
	float xAxis, yAxis, zAxis;
	TVector3D dir = ray->dir;
	normalizeVector3D(&dir); // size of step

	TVector3D tDelta;
	TPoint3D tMax;
	TPoint3D step;

	tMax.x = ray->orig.x + fabs(infoUniGrid->boxMin.x);
	tMax.y = ray->orig.y + fabs(infoUniGrid->boxMin.y);
	tMax.z = fabs(infoUniGrid->boxMin.z) - ray->orig.z;

	int X, Y, Z, id;
	X = (int)(tMax.x / infoUniGrid->cell_size.x);
	Y = (int)(tMax.y / infoUniGrid->cell_size.y);
	Z = (int)(tMax.z / infoUniGrid->cell_size.z); // index to cells

	if (dir.x < 0.f) {
		step.x = -1;
		tMax.x -= X*infoUniGrid->cell_size.x;
	}
	else {
		step.x = 1;
		tMax.x = ((X + 1)*infoUniGrid->cell_size.x) - tMax.x;
	}
	if (dir.y < 0.f) {
		step.y = -1;
		tMax.y -= (Y*infoUniGrid->cell_size.y);
	}
	else {
		step.y = 1;
		tMax.y = ((Y + 1)*infoUniGrid->cell_size.y) - tMax.y;
	}
	if (dir.z < 0.f) {
		step.z = 1;
		tMax.z = ((Z + 1)*infoUniGrid->cell_size.z) - tMax.z;
	}
	else {
		step.z = -1;
		tMax.z -= (Z*infoUniGrid->cell_size.z);
	}

	tDelta.x = infoUniGrid->cell_size.x;
	tDelta.y = infoUniGrid->cell_size.y;
	tDelta.z = infoUniGrid->cell_size.z;

	int grid_size = infoUniGrid->grid_size;
	is->hitTime = INFINITY;

	TIntersect currentIs;
	TSphere sphere;
	TTriangle triangle;

	while ((X < grid_size) && (X >= 0) &&
		(Y < grid_size) && (Y >= 0) &&
		(Z < grid_size) && (Z >= 0)) {

		id = (int)(X + Y * grid_size + Z * grid_size * grid_size);
		for (int i = uniGrid[id].objStartIndex; i < uniGrid[id].objSize; i++) {

			if (objects[i].type == SPHERE_INDEX) {
				sphere = sp[objects[i].index];

				if (sphereIntersectIs(&sphere, ray, &currentIs)) {
					if (currentIs.hitTime < is->hitTime) {
						*is = currentIs;
						is->obj_index = objects[i].index;
					}
				}
			}
			else {
				triangle = tr[objects[i].index];

				if (triangleIntersectIs(&triangle, ray, &currentIs, me, ra_me, cnt_ra_me)) {
					if (currentIs.hitTime < is->hitTime) {
						*is = currentIs;
						is->obj_index = objects[i].index;
					}
				}
			}
		}

		xAxis = (1 / fabs(dir.x) * tMax.x);
		yAxis = (1 / fabs(dir.y) * tMax.y);
		zAxis = (1 / fabs(dir.z) * tMax.z);

		if (xAxis < yAxis) {
			if (xAxis < zAxis) {
				tMax.x += tDelta.x;
				X += (int)step.x;
			}
			else {
				tMax.z += tDelta.z;
				Z += (int)step.z;
			}
		}
		else {
			if (yAxis < zAxis) {
				tMax.y += tDelta.y;
				Y += (int)step.y;
			}
			else {
				tMax.z += tDelta.z;
				Z += (int)step.z;
			}
		}
	}

	return is->hitTime != INFINITY;
}

// Compute color for pixel (start pathtracing).
// ray -> information about ray
// depth -> maximum depth of computation
// sp -> buffer of all spheres
// tr -> buffer of all triangles
// me -> buffer of all meshes
// ra_me -> buffer of size every mesh
// @return -> result color for pixel
TColor trace_unigrid(TRay ray, uint depth, uint2 seed, __global TSphere* sp, unsigned int cnt_sp,
	__global TTriangle* tr, unsigned int cnt_tr, __global TMesh* me,
	__global unsigned int* ra_me, unsigned int cnt_ra_me,
	__global TLight* lights, unsigned int cnt_li,
	__global TUniGrid* infoUniGrid, __global TBoxLink* uniGrid, __global TObject* objects)
{
	TColor stack[500];
	unsigned int stack_id = 0;

	bool isEnd = true;
	int MINIMUM_DEPTH = 4;
	float M_PI2 = 3.14159265358979323846f;
	float p_absorption = 0.1f;
	float absorption_factor = 1 / (1 - p_absorption);

	TIntersect is;
	TColor directLight, indirectLight;

	while (true) {
		directLight.x = 0.0f; directLight.y = 0.0f; directLight.z = 0.0f;
		indirectLight.x = 0.0f; indirectLight.y = 0.0f; indirectLight.z = 0.0f;
		isEnd = true;
		// Try to intersect any object.
		if (intersectIs_unigrid(&ray, &is, sp, cnt_sp, tr, cnt_tr, me, ra_me, cnt_ra_me,
			infoUniGrid, uniGrid, objects)) { // OK

			// Found intersection -> compute color.
			TMaterial material = is.material;
			// Get info about object.
			float reflectivity = material.reflectivity;
			float transparency = material.transparency;

			// Russian Roulette.
			float light_type = randomFloat(&seed, 1);

			// Next ray will be?
			if (light_type <= reflectivity) {
				// Reflected ray.
				ray = getReflectedRay(&ray, &is, sp, tr, me, ra_me); // OK
			}
			else if ((light_type - reflectivity) <= transparency) {
				// Refracted ray.
				ray = getRefractedRay(&ray, &is, sp, tr, me, ra_me); // OK
			}
			else {
				// Compute color.
				// Research if intersected point is in shadow.
				// Compute direct light.
				for (int i = 0; i < cnt_li; ++i) { // instead 5 fill lights
					TLight light = lights[i]; // OK
					TRay shadowRay = getShadowRay(&light, &is); // OK

					// Is point visible by light?
					if (!intersect_unigrid(&shadowRay, sp, cnt_sp, tr, cnt_tr, me, ra_me, cnt_ra_me, 
						infoUniGrid, uniGrid, objects)) 
					{ // OK
						float intensity = pow(shadowRay.maxT, -2);
						TColor incomingRadiance = light.radiance * intensity;
						TVector3D lightVec = light.worldPos - is.position;

						normalizeVector3D(&lightVec);
						TColor brdf = evalBRDFdiffuse(&material); // TO DO
						float incidentAngle = max(dotVector3D(lightVec, is.normal), 0.0f);
						directLight += incomingRadiance * brdf * incidentAngle;
					}
				}

				// Compute indirect light.
				if ((depth <= MINIMUM_DEPTH) || (randomFloat(&seed, 1) > p_absorption)) {
					isEnd = false;
					float theta, phi;
					TVector3D n_x, n_y, n_z;
					float x_b, y_b, z_b;
					TVector3D dir;

					// Generate random ray path.
					theta = acos(sqrt(1.0f - randomFloat(&seed, 1)));
					phi = 2.0f * M_PI2 * randomFloat(&seed, 1);

					TVector3D up;
					up.x = 1.0f; up.y = 0.0f; up.z = 0.0f;
					if (fabs(is.normal.x) > 0.75f) {
						up.x = 0.0f; up.y = 1.0f; up.z = 0.0f;
					}

					n_x = crossProductVector3D(up, is.normal);
					normalizeVector3D(&n_x);
					n_y = crossProductVector3D(n_x, is.normal);
					n_z = is.normal;

					x_b = cos(phi) * sin(theta);
					y_b = sin(phi) * sin(theta);
					z_b = cos(theta);
					dir = x_b * n_x + y_b * n_y + z_b * n_z;

					ray.orig = is.position;
					ray.dir = dir;
					ray.maxT = INFINITY;
					ray.minT = 0.001f;
					ray.dp.dx.x = 0.0f; ray.dp.dx.y = 0.0f; ray.dp.dx.z = 0.0f;
					ray.dp.dy.x = 0.0f; ray.dp.dy.y = 0.0f; ray.dp.dy.z = 0.0f;
					ray.dd.dx.x = 0.0f; ray.dd.dx.y = 0.0f; ray.dd.dx.z = 0.0f;
					ray.dd.dy.x = 0.0f; ray.dd.dy.y = 0.0f; ray.dd.dy.z = 0.0f;

					TColor brdf = evalBRDFdiffuse(&material);

					indirectLight = M_PI2 * brdf; // missing trace atd.
					if (depth > MINIMUM_DEPTH)
						indirectLight *= absorption_factor;

				}

				if (isEnd) {
					TColor pixel = directLight;
					for (int i = stack_id; i > 0; i -= 2) {
						pixel = stack[i - 2] + stack[i - 1] * pixel;
					}
					return pixel;
				}
				else {
					stack[stack_id] = directLight;
					stack[stack_id + 1] = indirectLight;
					stack_id += 2;
				}

			} // End of else branch
		}
		else {
			if (stack_id != 0) {
				TColor pixel = directLight;
				for (int i = stack_id; i > 0; i -= 2) {
					pixel = stack[i - 2] + stack[i - 1] * pixel;
				}
				return pixel;
			}
			else {
				// Intersection wasn't successfully. Return black color.
				return (TColor)(0.0f, 0.0f, 0.0f);
			}
		}
		depth++;
	} // End of while loop
} // End of function

#endif // _KERNEL_TRACE_UNIGRID_H_