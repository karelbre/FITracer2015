/*
	Name: kernel_trace_bvh.h
	Desc: Trace and intersection functions for BVH PT.
	Author: Karel Brezina (xbrezi13)
*/

#ifndef _KERNEL_TRACE_BVH_H_
#define _KERNEL_TRACE_BVH_H_

#include "kernel_types.h"
#include "kernel_functions.h"

// Intersect object without information about it.
// ray -> information about ray
// sp -> buffer of all spheres
// tr -> buffer of all triangles
// me -> buffer of all meshes
// ra_me -> buffer of size every mesh
// @return -> successful of intersect 
bool intersect_bvh(TRay* ray, __global TSphere* sp, unsigned int cnt_sp,
	__global TTriangle* tr, unsigned int cnt_tr, __global TMesh* me,
	__global unsigned int* ra_me, unsigned int cnt_ra_me,
	__global TBVHNode* bvhNodes, __global TObject* objects)
{
	__global TBVHNode* currentNode = &bvhNodes[0];
	float mMin, mMax;
	if (!boxIntersect(ray, bvhNodes[0].boxMin, bvhNodes[0].boxMax, &mMin, &mMax)) {
		return false;
	}

	TSphere sphere;
	TTriangle triangle;
	__global TBVHNode* stackNodes[100];
	unsigned int stackCnt = 0;

	while (true) {
		if ((*currentNode).leaf) {
			for (int i = (*currentNode).indexObj.objStartIndex; i < (*currentNode).indexObj.objSize; i++) {
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
		}
		else {
			__global TBVHNode* left_node = &bvhNodes[(*currentNode).indexNode];
			__global TBVHNode* right_node = &bvhNodes[(*currentNode).indexNode + 1];

			bool leftHit, rightHit;

			leftHit = boxIntersect(ray, (*left_node).boxMin, (*left_node).boxMax, &mMin, &mMax);
			rightHit = boxIntersect(ray, (*right_node).boxMin, (*right_node).boxMax, &mMin, &mMax);
			if (leftHit && rightHit) {
				currentNode = left_node;
				stackNodes[stackCnt] = right_node;
				stackCnt++;
				continue;
			}
			else if (leftHit) {
				currentNode = left_node;
				continue;
			}
			else if (rightHit) {
				currentNode = right_node;
				continue;
			}
		}

		if (stackCnt == 0) {
			return false;
		}
		stackCnt--;
		currentNode = stackNodes[stackCnt];
	}
}

// Intersect object within information about it.
// ray -> information about ray
// is -> information about intersection
// sp -> buffer of all spheres
// tr -> buffer of all triangles
// me -> buffer of all meshes
// ra_me -> buffer of size every mesh
// @return -> successful of intersect 
bool intersectIs_bvh(TRay* ray, TIntersect* is, __global TSphere* sp, unsigned int cnt_sp,
	__global TTriangle* tr, unsigned int cnt_tr, __global TMesh* me,
	__global unsigned int* ra_me, unsigned int cnt_ra_me,
	__global TBVHNode* bvhNodes, __global TObject* objects)
{
	__global TBVHNode* currentNode = &bvhNodes[0];
	float mMin, mMax;
	if (!boxIntersect(ray, bvhNodes[0].boxMin, bvhNodes[0].boxMax, &mMin, &mMax)) {
		return false;
	}

	TSphere sphere;
	TTriangle triangle;
	__global TBVHNode* stackNodes[100];
	float stackNodesMinT[100];
	unsigned int stackCnt = 0;
	TRay localRay = *ray;
	bool hit = false;

	while (true) {
		if ((*currentNode).leaf) {
			for (int i = (*currentNode).indexObj.objStartIndex; i < (*currentNode).indexObj.objSize; i++) {
				if (objects[i].type == SPHERE_INDEX) {
					sphere = sp[objects[i].index];
					if (sphereIntersectIs(&sphere, &localRay, is)) {
						localRay.maxT = is->hitTime;
						is->obj_index = objects[i].index;
						hit = true;
					}
				}
				else {
					triangle = tr[objects[i].index];
					if (triangleIntersectIs(&triangle, &localRay, is, me, ra_me, cnt_ra_me)) {
						localRay.maxT = is->hitTime;
						is->obj_index = objects[i].index;
						hit = true;
					}
				}
			}
		}
		else {
			__global TBVHNode* left_node = &bvhNodes[(*currentNode).indexNode];
			__global TBVHNode* right_node = &bvhNodes[(*currentNode).indexNode + 1];

			bool leftHit, rightHit;
			float leftMin, rightMin;

			leftHit = boxIntersect(ray, (*left_node).boxMin, (*left_node).boxMax, &mMin, &mMax);
			leftMin = mMin;
			rightHit = boxIntersect(ray, (*right_node).boxMin, (*right_node).boxMax, &mMin, &mMax);
			rightMin = mMin;
			if (leftHit && rightHit) {
				if (leftMin < rightMin) {
					currentNode = left_node;
					stackNodes[stackCnt] = right_node;
					stackNodesMinT[stackCnt] = rightMin;
					stackCnt++;
				}
				else {
					currentNode = right_node;
					stackNodes[stackCnt] = left_node;
					stackNodesMinT[stackCnt] = leftMin;
					stackCnt++;
				}
				continue;
			}
			else if (leftHit) {
				currentNode = left_node;
				continue;
			}
			else if (rightHit) {
				currentNode = right_node;
				continue;
			}
		}

		currentNode = 0;
		while (stackCnt > 0) {
			stackCnt--;
			if (stackNodesMinT[stackCnt] < localRay.maxT) {
				currentNode = stackNodes[stackCnt];
				break;
			}
		}

		if (currentNode == 0) {
			return hit;
		}
	}
}

// Compute color for pixel (start pathtracing).
// ray -> information about ray
// depth -> maximum depth of computation
// sp -> buffer of all spheres
// tr -> buffer of all triangles
// me -> buffer of all meshes
// ra_me -> buffer of size every mesh
// @return -> result color for pixel
TColor trace_bvh(TRay ray, uint depth, uint2 seed, __global TSphere* sp, unsigned int cnt_sp,
	__global TTriangle* tr, unsigned int cnt_tr, __global TMesh* me,
	__global unsigned int* ra_me, unsigned int cnt_ra_me,
	__global TLight* lights, unsigned int cnt_li,
	__global TBVHNode* bvhNodes, __global TObject* objects)
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
		if (intersectIs_bvh(&ray, &is, sp, cnt_sp, tr, cnt_tr, me, ra_me, cnt_ra_me, bvhNodes, objects)) { // Not tested
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
					if (!intersect_bvh(&shadowRay, sp, cnt_sp, tr, cnt_tr, me, ra_me, cnt_ra_me,
						bvhNodes, objects)) { // Not tested
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

#endif // _KERNEL_TRACE_BVH_H_