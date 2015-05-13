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
	TVector3D delta = ray->dir;
	normalizeVector3D(&delta);

	TPoint3D pos;
	pos.x = fabs(infoUniGrid->boxMin.x);
	pos.y = fabs(infoUniGrid->boxMin.y);
	pos.z = fabs(infoUniGrid->boxMin.z); // pos coordinates

	TPoint3D diff;
	diff.x = ray->orig.x + pos.x; 
	diff.y = ray->orig.y + pos.y;
	diff.z = pos.z - ray->orig.z; // nearest axis

	TPoint3D index;
	index.x = (float)((int)(diff.x / infoUniGrid->cell_size.x));
	index.y = (float)((int)(diff.y / infoUniGrid->cell_size.y));
	index.z = (float)((int)(diff.z / infoUniGrid->cell_size.z)); // index to cells

	diff.x /= infoUniGrid->cell_size.x; diff.y /= infoUniGrid->cell_size.y; diff.z /= infoUniGrid->cell_size.z;
	diff.x -= (int)(diff.x); diff.y -= (int)(diff.y); diff.z -= (int)(diff.z);

	int id;
	int addX, addY, addZ;

	if (delta.x < 0.f) {
		addX = -1;
	}
	else {
		addX = 1;
	}
	if (delta.y < 0.f) {
		addY = -1;
	}
	else {
		addY = 1;
	}
	if (delta.z < 0.f) {
		addZ = 1;
	}
	else {
		addZ = -1;
	}

	float controlX, controlY, controlZ;
	TSphere sphere;
	TTriangle triangle;

	while ((index.x < infoUniGrid->grid_size) && (index.x >= 0.f) &&
		   (index.y < infoUniGrid->grid_size) && (index.y >= 0.f) &&
		   (index.z < infoUniGrid->grid_size) && (index.z >= 0.f)) {

		id = (int)(index.x + 
			 index.y * infoUniGrid->grid_size +
		     index.z * infoUniGrid->grid_size * infoUniGrid->grid_size);

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

		// Convert to positive value
		if (diff.x < 0.f) {
			diff.x += 1.f;
		}
		else if (diff.x > 1.f) {
			diff.x -= 1.f;
		}
		if (diff.y < 0.f) {
			diff.y += 1.f;
		}
		else if (diff.y > 1.f) {
			diff.y -= 1.f;
		}
		if (diff.z < 0.f) {
			diff.z += 1.f;
		}
		else if (diff.z > 1.f) {
			diff.z -= 1.f;
		}

		// Position in voxel
		controlX = diff.x;
		controlY = diff.y;
		controlZ = diff.z;

		if (addX > 0) {
			controlX = 1.0f - controlX;
		}
		if (addY > 0) {
			controlY = 1.0f - controlY;
		}
		if (addZ < 0) {
			controlZ = 1.0f - controlZ;
		}

		if (controlX < controlY) {
			if (controlX < controlZ) {
				index.x += addX;
				diff.x += delta.x;
			}
			else {
				index.z += addZ;
				diff.z += delta.z;
			}
		}
		else {
			if (controlY < controlZ) {
				index.y += addY;
				diff.y += delta.y;
			}
			else {
				index.z += addZ;
				diff.z += delta.z;
			}
		}
	}

	return false; // None object was intersected.
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
	TVector3D delta = ray->dir;
	normalizeVector3D(&delta);

	TPoint3D pos;
	pos.x = fabs(infoUniGrid->boxMin.x);
	pos.y = fabs(infoUniGrid->boxMin.y);
	pos.z = fabs(infoUniGrid->boxMin.z); // pos coordinates

	TPoint3D diff;
	diff.x = ray->orig.x + pos.x;
	diff.y = ray->orig.y + pos.y;
	diff.z = pos.z - ray->orig.z; // nearest axis

	TPoint3D index;
	index.x = (float)((int)(diff.x / infoUniGrid->cell_size.x));
	index.y = (float)((int)(diff.y / infoUniGrid->cell_size.y));
	index.z = (float)((int)(diff.z / infoUniGrid->cell_size.z)); // index to cells

	diff.x /= infoUniGrid->cell_size.x; diff.y /= infoUniGrid->cell_size.y; diff.z /= infoUniGrid->cell_size.z;
	diff.x -= (int)(diff.x); diff.y -= (int)(diff.y); diff.z -= (int)(diff.z);

	int id;
	int addX, addY, addZ;

	if (delta.x < 0.f) {
		addX = -1;
	}
	else {
		addX = 1;
	}
	if (delta.y < 0.f) {
		addY = -1;
	}
	else {
		addY = 1;
	}
	if (delta.z < 0.f) {
		addZ = 1;
	}
	else {
		addZ = -1;
	}

	is->hitTime = INFINITY;

	float controlX, controlY, controlZ;
	TSphere sphere;
	TTriangle triangle;

	while ((index.x < infoUniGrid->grid_size) && (index.x >= 0.f) &&
		   (index.y < infoUniGrid->grid_size) && (index.y >= 0.f) &&
		   (index.z < infoUniGrid->grid_size) && (index.z >= 0.f)) {

		id = (int)(index.x + index.y * infoUniGrid->grid_size +
			index.z * infoUniGrid->grid_size * infoUniGrid->grid_size);
		TIntersect currentIs;

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

		// Convert to positive value
		if (diff.x < 0.f) {
			diff.x += 1.f;
		}
		else if (diff.x > 1.f) {
			diff.x -= 1.f;
		}
		if (diff.y < 0.f) {
			diff.y += 1.f;
		}
		else if (diff.y > 1.f) {
			diff.y -= 1.f;
		}
		if (diff.z < 0.f) {
			diff.z += 1.f;
		}
		else if (diff.z > 1.f) {
			diff.z -= 1.f;
		}

		// Position in voxel
		controlX = diff.x;
		controlY = diff.y;
		controlZ = diff.z;

		if (addX > 0) {
			controlX = 1.0f - controlX;
		}
		if (addY > 0) {
			controlY = 1.0f - controlY;
		}
		if (addZ < 0) {
			controlZ = 1.0f - controlZ;
		}

		if (controlX < controlY) {
			if (controlX < controlZ) {
				index.x += addX;
				diff.x += delta.x;
			}
			else {
				index.z += addZ;
				diff.z += delta.z;
			}
		}
		else {
			if (controlY < controlZ) {
				index.y += addY;
				diff.y += delta.y;
			}
			else {
				index.z += addZ;
				diff.z += delta.z;
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
										   infoUniGrid, uniGrid, objects)) { // OK
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