/*
	Name: kernel_functions.h
	Desc: OpenCL functions for compute PT.
	Author: Karel Brezina (xbrezi13)
*/

#ifndef _KERNEL_FUNCTION_H_
#define _KERNEL_FUNCTION_H_

#include "kernel_types.h"

// Compute ray for every pixel.
// cam -> camera view
// x -> index of x axis
// y -> index of y axis
// @return -> computed ray pro pixel of image
TRay getRay(TCamera cam, float x, float y) 
{
	TVector3D right = 2.0f / convert_float(cam.image.width) * cam.imageExtentX * cam.right;
	TVector3D up = -2.0f / convert_float(cam.image.height) * cam.imageExtentY * cam.up;
	TVector3D view = cam.forward - cam.imageExtentX * cam.right + cam.imageExtentY * cam.up;

	TVector3D d = view + x*right + y*up;
	float dLength = lengthVector3D(d);

	float r = 1.0f / (dLength*dLength*dLength);

	TDiff dp, dd;
	dp.dx = (float3)(0.0f, 0.0f, 0.0f);
	dp.dy = (float3)(0.0f, 0.0f, 0.0f);
	dd.dx = (dotVector3D(d, d) * right - dotVector3D(d, right) * d) * r;
	dd.dy = (dotVector3D(d, d) * up - dotVector3D(d, up) * d) * r;

	d /= dLength;

	TRay ray;
	ray.dp = dp;
	ray.dd = dd;
	ray.orig = cam.origin;
	ray.dir = d;
	ray.minT = cam.nearPlane;
	ray.maxT = cam.farPlane;

	return ray;
}

// Compute shadow ray.
// light -> info about light
// is -> info about last intersection
// @return -> computed shadow ray
TRay getShadowRay(TLight* light, TIntersect* is)
{
	TVector3D lightVec = light->worldPos - is->position;

	float len = lengthVector3D(lightVec);

	TRay ray;
	ray.orig = is->position + is->normal*0.001f;
	ray.dir = lightVec;
	normalizeVector3D(&(ray.dir));
	ray.minT = 0.0f;
	ray.maxT = len;

	return ray;
}

// Compute diff position.
// is -> information about intersection
// @return -> computed diff position
TDiff calculatePositionDifferential(TIntersect* is) 
{
	float3 D = is->ray.dir;
	float3 N = is->normal;

	TDiff dp;

	float3 negDproj = minusVector3D(D) / dotVector3D(D, N);
	float3 t;

	t = is->ray.dp.dx + is->hitTime * is->ray.dd.dx;
	dp.dx = t + (dotVector3D(t, N) * negDproj);

	t = is->ray.dp.dy + is->hitTime * is->ray.dd.dy;
	dp.dy = t + (dotVector3D(t, N) * negDproj);

	return dp;
}

// Compute diff normal for sphere.
// radius -> radius of sphere
// dp -> different of position origin ray and sphere
// isFrontFacing -> is intersection point visible?
// @return -> computed diff normal
TVector3D calculateNormalDifferentialSphere(float radius, TVector3D* dp, bool isFrontFacing) 
{
	float sign = isFrontFacing ? 1.0f : -1.0f;
	float3 num = floatMultVector3D(sign, *dp);
	return  floatDivVector3D(num, radius);
}

// Compute diff normal for triangle.
// tr -> buffer with all triangles
// me -> buffer with all meshes
// is -> infos about last intersection
// dp -> different of position origin ray and triangle
// @return -> computed diff normal
float3 calculateNormalDifferentialTriangle(__global TTriangle* _tr, __global TMesh* me, 
	TIntersect* is, TVector3D* dp)
{
	TTriangle tr = _tr[is->obj_index];

	TVector3D n0 = getVtxNormal(me, is->mesh_index + 0);
	TVector3D n1 = getVtxNormal(me, is->mesh_index + 1);
	TVector3D n2 = getVtxNormal(me, is->mesh_index + 2);

	TVector3D plane0 = dotVector3D(tr.planes[0], is->position) + tr.planeOffsets.x;
	TVector3D plane1 = dotVector3D(tr.planes[1], is->position) + tr.planeOffsets.y;
	TVector3D plane2 = dotVector3D(tr.planes[2], is->position) + tr.planeOffsets.z;

	TVector3D n = plane0*n0 + plane1*n1 + plane2*n2;
	TVector3D dn = dotVector3D(tr.planes[0], *dp)*n0 + 
				   dotVector3D(tr.planes[1], *dp)*n1 +
				   dotVector3D(tr.planes[1], *dp)*n2;

	float sign = is->frontFacing ? 1.0f : -1.0f;

	float nl = lengthVector3D(n);
	return sign * (dotVector3D(n, n)*dn - dotVector3D(n, dn)*n) / (n1*n1*n1);
}

// Compute ray reflected from intersected point.
// is -> information about intersection
// tr -> buffer of all triangles
// me -> buffer of all meshes
// ra_me -> range of mesh buffer
// @return -> computed reflect ray
TRay getReflectedRay(TRay* _ray, TIntersect* is, __global TSphere* sp, __global TTriangle* tr,
	__global TMesh* me, __global unsigned int* ra_me)
{
	float3 D = is->ray.dir;
	float3 N = is->normal;

	float3 V = minusVector3D(D);
	float3 R = 2 * (dotVector3D(N, V)) * N - V;

	TDiff dp = calculatePositionDifferential(is);
	TDiff dd;
	float3 dn;

	float dDotN = dotVector3D(D, N);
	
	if (is->isSphere) {
		dn = calculateNormalDifferentialSphere(sp[is->obj_index].radius, &(dp.dx), is->frontFacing);
	}
	else {
		dn = calculateNormalDifferentialTriangle(tr, me, is, &(dp.dx));
	}

	dd.dx = _ray->dd.dx - 2.0f*(dDotN*dn + (dotVector3D(_ray->dd.dy, N)
			+ dotVector3D(D, dn)) * N);
	
	if (is->isSphere) {
		dn = calculateNormalDifferentialSphere(sp[is->obj_index].radius, &(dp.dy), is->frontFacing);
	}
	else {
		dn = calculateNormalDifferentialTriangle(tr, me, is, &(dp.dy));
	}

	dd.dy = _ray->dd.dy - 2.0f*(dDotN*dn + (dotVector3D(_ray->dd.dy, N)
		+ dotVector3D(D, dn)) * N);

	TRay ray;
	ray.orig = is->position + N*0.001f;
	ray.dir = R;
	normalizeVector3D(&(ray.dir));
	ray.dp = dp;
	ray.dd = dd;
	ray.minT = 0.001f;
	ray.maxT = INFINITY;

	return ray;
}

// Compute ray refracted from intersected point.
// ray -> incoming ray
// is -> information about intersection
// sp -> buffer of all spheres
// tr -> buffer of all triangles
// me -> buffer of all meshes
// ra_me -> range of mesh buffer
// @return -> computed refract ray
TRay getRefractedRay(TRay* _ray, TIntersect* is, __global TSphere* sp, __global TTriangle* tr,
	__global TMesh* me, __global unsigned int* ra_me)
{
	TVector3D D = _ray->dir;
	TVector3D N = is->normal;

	float iEta = 1.0f;
	float tEta = is->material.refractionIndex;

	if (is->frontFacing)
		swapFloat(&iEta, &tEta);

	float eta = iEta / tEta;

	float r = dotVector3D(minusVector3D(D), N);
	float c = 1 - eta*eta * (1 - r*r);
	if (c < 0) {
		return getReflectedRay(_ray, is, sp, tr, me, ra_me);
	}

	TVector3D T = floatMultVector3D(eta, D) + floatMultVector3D((eta * r - sqrt(c)), N);

	float mu = eta*dotVector3D(N, D) - dotVector3D(N, T);

	TDiff dp = calculatePositionDifferential(is);
	TDiff dd;
	TVector3D dn;

	float dmu;
	float dmu0 = (eta - eta*eta* dotVector3D(D, N) / dotVector3D(T, N));

	if (is->isSphere) {
		dn = calculateNormalDifferentialSphere(sp[is->obj_index].radius, &(dp.dx), is->frontFacing);
	}
	else {
		dn = calculateNormalDifferentialTriangle(tr, me, is, &(dp.dx));
	}
	dmu = dmu0 * (dotVector3D(_ray->dd.dx, N) + dotVector3D(D, dn));
	dd.dx = eta*_ray->dd.dx - (floatMultVector3D(mu, dn) + floatMultVector3D(dmu, N));

	if (is->isSphere) {
		dn = calculateNormalDifferentialSphere(sp[is->obj_index].radius, &(dp.dy), is->frontFacing);
	}
	else {
		dn = calculateNormalDifferentialTriangle(tr, me, is, &(dp.dy));
	}
	dmu = dmu0 * (dotVector3D(_ray->dd.dy, N) + dotVector3D(D, dn));
	dd.dy = eta*_ray->dd.dy - (floatMultVector3D(mu, dn) + floatMultVector3D(dmu, N));

	TRay ray;
	ray.orig = is->position - N*0.001f;
	ray.dir = T;
	normalizeVector3D(&(ray.dir));
	ray.dp = dp;
	ray.dd = dd;
	ray.minT = 0.001f;
	ray.maxT = INFINITY;

	return ray;
}

// Compute if ray intersect sphere without information about it.
// sp -> information about sphere
// ray -> information about ray
// @return -> successful of intersect sphere
bool sphereIntersect(TSphere* sp, TRay* ray) 
{
	float3 o = matrix_mul_point3D(&(sp->InvWorldTransform), ray->orig);
	float3 d = matrix_mul_vector3D(&(sp->InvWorldTransform), ray->dir);

	float A = d.x*d.x + d.y*d.y + d.z*d.z;
	float B = 2.0f * (d.x*o.x + d.y*o.y + d.z*o.z);
	float C = o.x*o.x + o.y*o.y + o.z*o.z - sp->radius*sp->radius;

	float t0, t1;
	if (!solveQuadratic(A, B, C, &t0, &t1)) return false;

	if ((t0 > ray->maxT) || (t1 < ray->minT)) return false;
	if ((t0 < ray->minT) || (t1 > ray->maxT)) return false;

	return true;
}

// Compute if ray intersect sphere within information about it.
// sp -> information about sphere
// ray -> information about ray
// is -> information about intersect
// @return -> successful of intersect sphere
bool sphereIntersectIs(const TSphere* sp, const TRay* ray, TIntersect* is) 
{
	float M_PI2 = 3.14159265358f;
	float3 o = matrix_mul_point3D(&(sp->InvWorldTransform), ray->orig);
	float3 d = matrix_mul_vector3D(&(sp->InvWorldTransform), ray->dir);

	float A = d.x*d.x + d.y*d.y + d.z*d.z;
	float B = 2.0f * (d.x*o.x + d.y*o.y + d.z*o.z);
	float C = o.x*o.x + o.y*o.y + o.z*o.z - sp->radius*sp->radius;

	float t0, t1;
	if (!solveQuadratic(A, B, C, &t0, &t1)) return false;

	if ((t0 > ray->maxT) || (t1 < ray->minT)) return false;
	if ((t0 < ray->minT) && (t1 > ray->maxT)) return false;

	float t = t0 < ray->minT ? t1 : t0;
	float3 p = o + t*d;
	float3 n = p;
	n /= sp->radius;

	float u = atan2(-p.z, p.x) / (2.0f * M_PI2);
	if (u < 0.0f) u += 1.0f;
	float v = acos(p.y / sp->radius) / M_PI2;
	if (v != v)
		v = p.y > 0.0f ? 0.0f : 1.0f;

	is->ray = *ray;
	is->material = sp->material;
	is->hitTime = t;
	is->position = matrix_mul_point3D(&(sp->WorldTransform), p);

	is->normal = matrix_mul_vector3D(&(sp->WorldTransform), n);
	normalizeVector3D(&(is->normal));
	is->view = -ray->dir;
	is->frontFacing = dotVector3D(is->view, is->normal) > 0.0f;
	if (!is->frontFacing) is->normal = -is->normal;
	is->texture.uv.x = u;
	is->texture.uv.y = v;
	is->isSphere = true;

	return true;
}

// Compute if ray intersect triangle without information about it.
// tr -> information about triangle
// ray -> information about ray
// me -> buffer of all meshes
// ra_me -> buffer of ranges of meshes
// _cnt_meshes -> count of ranges buffer
// @return -> successful of intersect triangle
bool triangleIntersect(TTriangle* tr, TRay* ray, __global TMesh* me,
	__global unsigned int* ra_me, uint _cnt_meshes) 
{
	float t, v, w;
	float eps = 0.001f;
	float3 P = ray->orig;
	float3 D = ray->dir;

	uint index = getBeginMesh(ra_me, _cnt_meshes, tr->mesh);
	float3 v0 = getVtxPosition(me, index + tr->vtx[0].p);
	float3 v1 = getVtxPosition(me, index + tr->vtx[1].p);
	float3 v2 = getVtxPosition(me, index + tr->vtx[2].p);

	float3 e1 = v1 - v0;
	float3 e2 = v2 - v0;

	float3 N = crossProductVector3D(e1, e2);
	float s = -dotVector3D(D, N);

	if (abs(convert_long(s)) < eps)
		return false;

	float s_inv = 1 / s;

	float3 R = P - v0;
	t = dotVector3D(R, N) * s_inv;

	if ((t <= ray->minT) || (t >= ray->maxT))
		return false;

	float3 Q = -crossProductVector3D(D, R);
	v = dotVector3D(Q, e2) * s_inv;

	if (v < 0)
		return false;

	float3 minus_e1 = minusVector3D(e1);
	w = dotVector3D(minus_e1, Q) * s_inv;

	if ((w < 0) || (v + w) > 1)
		return false;

	return true;
}

// Compute if ray intersect triangle within information about it.
// tr -> information about triangle
// ray -> information about ray
// is -> information about intersection
// me -> buffer of all meshes
// ra_me -> buffer of ranges of meshes
// _cnt_meshes -> count of ranges buffer
// @return -> successful of intersect triangle
bool triangleIntersectIs(TTriangle* tr, TRay* ray, TIntersect* is,
	__global TMesh* me, __global unsigned int* ra_me, uint _cnt_meshes) 
{
	
	float t, u, v, w;
	float eps = 0.001f;
	float3 P = ray->orig;
	float3 D = ray->dir;

	uint index = getBeginMesh(ra_me, _cnt_meshes, tr->mesh);
	float3 v0 = getVtxPosition(me, index + tr->vtx[0].p);
	float3 v1 = getVtxPosition(me, index + tr->vtx[1].p);
	float3 v2 = getVtxPosition(me, index + tr->vtx[2].p);

	float3 e1 = v1 - v0;
	float3 e2 = v2 - v0;

	float3 N = crossProductVector3D(e1, e2); 
	float s = -dotVector3D(D, N);

	if (abs(convert_long(s)) < eps)
		return false;

	float s_inv = 1 / s;

	float3 R = P - v0;
	t = dotVector3D(R, N) * s_inv;

	if ((t <= ray->minT) || (t >= ray->maxT))
		return false;

	float3 Q = -crossProductVector3D(D, R);
	v = dotVector3D(Q, e2) * s_inv;
	
	if (v < 0)
		return false;

	float3 minus_e1 = minusVector3D(e1);
	w = dotVector3D(minus_e1, Q) * s_inv;

	if ((w < 0) || (v + w) > 1)
		return false;

	u = 1 - v - w;

	is->ray = *ray;
	is->material = tr->material;
	if (!is->material.isMat) {
		is->material = me[index].material;
	}
	is->position = ray->orig + (t * ray->dir);

	float3 vtxN0 = getVtxNormal(me, index + tr->vtx[0].n);
	float3 vtxN1 = getVtxNormal(me, index + tr->vtx[1].n);
	float3 vtxN2 = getVtxNormal(me, index + tr->vtx[2].n);

	is->normal = floatMultVector3D(u, vtxN0) +
		floatMultVector3D(v, vtxN1) +
		floatMultVector3D(w, vtxN2);

	normalizeVector3D(&(is->normal));

	is->view = minusVector3D(ray->dir);
	is->frontFacing = dotVector3D(is->view, is->normal) > 0.0f;
	if (is->frontFacing) is->frontFacing = -is->frontFacing;

	TUV vtxUV0 = getVtxTexture(me, index + tr->vtx[0].t);
	TUV vtxUV1 = getVtxTexture(me, index + tr->vtx[1].t);
	TUV vtxUV2 = getVtxTexture(me, index + tr->vtx[2].t);

	vtxUV0 = floatMultUV(u, vtxUV0);
	vtxUV1 = floatMultUV(v, vtxUV1);
	vtxUV2 = floatMultUV(w, vtxUV2);
	TUV added = UVAddUV(vtxUV1, vtxUV2);

	is->texture = UVAddUV(vtxUV0, added);

	is->hitTime = t;
	is->hitparam.uv.x = u;
	is->hitparam.uv.y = v;

	is->isSphere = false;
	is->mesh_index = index;

	return true;
}

// Compute BRDF to get color of material.
// material -> information about intersected object's material
// @return -> computed color for point of intersect
TColor evalBRDFdiffuse(TMaterial* material) 
{
	float M_PI2 = 3.14159265358f;
	return floatDivVector3D(material->color, M_PI2);
}

float getItemFloat3(float3* vec, unsigned int i) 
{
	if (i == 0) {
		return (*vec).x;
	}
	else if (i == 1) {
		return (*vec).y;
	}
	else {
		return (*vec).z;
	}
}

void swap(float* t1, float* t2)
{
	float t = *t1;
	*t1 = *t2;
	*t2 = t;
}

bool boxIntersect(TRay* ray, TPoint3D mMin, TPoint3D mMax, float* tMin, float* tMax) 
{
	float t0 = ray->minT;
	float t1 = ray->maxT;

	// Loop over the three axes and compute the hit time for the
	// two axis-aligned bounding box planes in each, decreasing the
	// parametric range of the ray until start>end time, which means
	// the ray missed the box, or until we finish which means there
	// is an intersection.
	for (int i = 0; i < 2; i++) {
		float invDir = 1.0f / getItemFloat3(&(ray->dir), i);
		float tNear = (getItemFloat3(&mMin, i) - getItemFloat3(&(ray->orig), i)) * invDir;
		float tFar = (getItemFloat3(&mMax, i) - getItemFloat3(&(ray->orig), i)) * invDir;

		if (tNear > tFar) swap(&tNear, &tFar);

		if (tNear > t0) t0 = tNear;
		if (tFar < t1) t1 = tFar;
		if (t0 > t1) return false;
	}

	*tMin = t0;
	*tMax = t1;
	return true;
}

#endif // _KERNEL_FUNCTION_H_