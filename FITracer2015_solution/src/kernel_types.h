/*
	Name: kernel_types.h
	Desc: Data structures for compute PT.
	Author: Karel Brezina (xbrezi13)
*/

#ifndef _KERNEL_TYPES_
#define _KERNEL_TYPES_

#define SPHERE_INDEX 0
#define TRIANGLE_INDEX 1

// Basic structures.
// Almost all structures are equialent of class's data part of 
// CPU version pathtracing. Here they are marked as T... 
// Used for export data to compute on OpenCL device.

// RGB representation of color. 
typedef float3 TColor;
// XYZ representation of vector.
typedef float3 TVector3D;
// XYZ representation of point. 
typedef float3 TPoint3D;
// 4x4 matrice. 
typedef struct {
	float4 m[4];
} TMatrix;
// Store diff between two vectors or points. 
typedef struct {
	TVector3D dx;
	TVector3D dy;
} TDiff;
// Store info about compute screen.
typedef struct {
	uint width;
	uint height;
} TImage;
// Store info about object material.
typedef struct {
	TColor color;
	float reflectivity;
	float transparency;
	float refractionIndex;
	bool isMat;
} TMaterial;
// Index to texture.
typedef struct {
	float2 uv;
} TUV;
// Represent vertex of triangle
typedef struct {
	int p;
	int n;
	int t;
} TVertex;

// Advanced structures.
// Consist from basic structures.

// Store info about computed ray
typedef struct {
	TDiff dp;
	TDiff dd;
	TPoint3D orig;
	TVector3D dir;
	float minT;
	float maxT;
} TRay;

// Store info about set camera.
typedef struct {
	TPoint3D origin;				///< Position of camera origin.
	TVector3D forward;			///< Camera frame forward vector.
	TVector3D right;				///< Camera frame right vector.
	TVector3D up;					///< Camera frame up vector.
	TImage image;				///< Ptr to the output image.
	float nearPlane;			///< Distance to near viewing plane.
	float farPlane;			///< Distance to far viewing plane.
	float fieldOfView;			///< Horizontal field-of-view
	float imageExtentX;		///< Extent of image plane in positive x direction.
	float imageExtentY;		///< Extent of image plane in positive y direction.
} TCamera;

// Store info about intersection.
typedef struct {
	TRay ray;
	TMaterial material;
	TPoint3D position;
	TVector3D normal;
	TVector3D view;
	TUV texture;
	TUV hitparam;
	float hitTime;
	uint obj_index;
	uint mesh_index;
	bool isSphere;
	bool frontFacing;
} TIntersect;
// Store info about sphere.
typedef struct {
	TMatrix InvWorldTransform;
	TMatrix WorldTransform;
	TMaterial material;
	float radius;
	uint index;
} TSphere;
// Store info about triangle.
typedef struct {
	TMaterial material;
	TVector3D planes[3];
	TVector3D planeOffsets;
	TVertex vtx[3];
	uint mesh;
	uint index;
} TTriangle;
// Store info about mesh. Addition structure for triangle intersection.
typedef struct {
	TMaterial material;
	TPoint3D origVtxP;
	TVector3D origVtxN;
	TPoint3D vtxP;
	TVector3D vtxN;
	TUV vtxUV;
} TMesh;
// Store info about light.
typedef struct {
	TColor col;
	TPoint3D position;
	TColor radiance;
	TPoint3D worldPos;
	float intensity;
} TLight;
// Store index to Sphere/Triangle buffer.
typedef struct {
	uint index;
	char type;
} TObject;
// Range of objects in Octreebox.
typedef struct {
	uint objStartIndex;
	uint objSize;
} TBoxLink;
// Octree node.
typedef struct {
	TPoint3D boxMin;
	TPoint3D boxMax;
	TPoint3D middlePoint;
	TBoxLink boxLink;
	uint children[8];
	uint leaf;
} TOctreeBox;

typedef struct {
	TPoint3D boxMin;
	TPoint3D boxMax;
	TPoint3D world_size;
	TPoint3D cell_size;
	uint grid_size;
} TUniGrid;

typedef struct {
	TPoint3D boxMin;
	TPoint3D boxMax;
	TBoxLink indexObj;
	uint leaf;
	uint indexNode;
} TBVHNode;

// Following functions are needed for compute pathtracing on OpenCL.

// Random number generator in range <0, 1)
// seed -> initial value of generator
// @return -> computed number
float getUniform(int a, int c, int m, float* x) 
{
	float num = a*(*x) + c;
	int num_mod = num / m;
	num = num - num_mod*m;

	*x = num / m;
	return *x;
}

// Multification of matrice and point.
// mat -> matrice
// mul -> point
// @result -> result of operation (point)
TPoint3D matrix_mul_point3D(const TMatrix* mat, const TPoint3D mul) 
{
	TPoint3D r;
	r.x = mat->m[0].x * mul.x + mat->m[0].y * mul.y + mat->m[0].z * mul.z + mat->m[0].w;
	r.y = mat->m[1].x * mul.x + mat->m[1].y * mul.y + mat->m[1].z * mul.z + mat->m[1].w;
	r.z = mat->m[2].x * mul.x + mat->m[2].y * mul.y + mat->m[2].z * mul.z + mat->m[2].w;
	return r;
}

// Multification of matrice and vector.
// mat -> matrice
// mul -> vector
// @result -> result of operation (vector)
TVector3D matrix_mul_vector3D(const TMatrix* mat, const TVector3D mul) 
{
	TVector3D r;
	r.x = mat->m[0].x * mul.x + mat->m[0].y * mul.y + mat->m[0].z * mul.z;
	r.y = mat->m[1].x * mul.x + mat->m[1].y * mul.y + mat->m[1].z * mul.z;
	r.z = mat->m[2].x * mul.x + mat->m[2].y * mul.y + mat->m[2].z * mul.z;
	return r;
}

// Solve quadratic function (for sphere intersection)
// A, B, C -> points of intersect 
// t0, t1 -> roots (maximum is two results)
// @result -> discriminant isn't neg
bool solveQuadratic(float A, float B, float C, float* t0, float* t1) 
{
	float d = B*B - 4.0f*A*C;

	if (d < 0.0f)
		return false;
	//printf("4. dale je: %f %f %f\n", A, B, C);
	d = sqrt(d);
	//printf("5. dale je: %f %f %f\n", A, B, C);

	(*t0) = (-B - d) / (2.0f*A);
	(*t1) = (-B + d) / (2.0f*A);

	return true;
}

// Length of Vector.
// num -> vector
// @return -> result of computation
float lengthVector3D(TVector3D num) 
{
	return sqrt(num.x*num.x + num.y*num.y + num.z*num.z);
}

// Dot product of vector.
// a -> first vector
// b -> second vector
// @result -> result of computation
float dotVector3D(TVector3D a, TVector3D b) 
{
	return a.x*b.x + a.y*b.y + a.z*b.z;
}

// Normalize of vector
// num -> vector
void normalizeVector3D(TVector3D* num) 
{
	float l = 1.0f / lengthVector3D(*num);
	(*num).x *= l; (*num).y *= l; (*num).z *= l;
}

// Get index to mesh where triangle belongs to.
// range_meshes -> buffer with ranges of meshes
// max -> size of range_meshes
// mesh -> index to range_mesh
// @return -> index of mesh beginning
uint getBeginMesh(__global uint* range_meshes, uint max, uint mesh) 
{
	if (mesh >= max) {
		//printf((__constant char *)"Kernel error: getBeginMesh() get higher index to range_meshes: %d.\n", mesh);
		return 0;
	}

	int index = 0;
	for (int i = 0; i < mesh; i++) {
		index += range_meshes[i];
	}

	return index;
}

// Get vertex position.
// meshes -> buffer with all meshes
// index -> index to meshes
// @result -> vertex position
float3 getVtxPosition(__global TMesh* meshes, uint index) 
{
	return meshes[index].vtxP;
}

// Get normal position.
// meshes -> buffer with all meshes
// index -> index to meshes
// @result -> normal position
float3 getVtxNormal(__global TMesh* meshes, uint index) 
{
	return meshes[index].vtxN;
}

// Get texture position.
// meshes -> buffer with all meshes
// index -> index to meshes
// @result -> texture position
TUV getVtxTexture(__global TMesh* meshes, uint index) 
{
	return meshes[index].vtxUV;
}

// Cross product of vectors
// u, v -> vectors
// @result -> result of computation
float3 crossProductVector3D(TVector3D u, TVector3D v) 
{	
	float3 num; 
	num.x = u.y*v.z - u.z*v.y;
	num.y = u.z*v.x - u.x*v.z;
	num.z = u.x*v.y - u.y*v.x;
	return num;
}

// Multification of two vectors
// u, v -> vectors
// @result -> result of computation
float3 floatMultVector3D(float u, TVector3D v) 
{
	float3 num;
	num.x = u*v.x;
	num.y = u*v.y;
	num.z = u*v.z;
	return num;
}

// Multification of two vectors
// u, v -> vectors
// @result -> result of computation
float3 floatDivVector3D(TVector3D u, float v) 
{
	float3 num;
	num.x = u.x/v;
	num.y = u.y/v;
	num.z = u.z/v;
	return num;
}

// Multification of vector and UV coordinate.
// u -> vectors
// v -> UV coord
// @result -> result of computation
TUV floatMultUV(float u, TUV uv) 
{
	TUV num;
	num.uv.x = u * uv.uv.x;
	num.uv.y = u * uv.uv.y;
	return num;
}

// Addition of two UV coordinates.
// uv1, uv2 -> UV coordinate
// @result -> result of computation
TUV UVAddUV(TUV uv1, TUV uv2) 
{
	TUV num;
	num.uv.x = uv1.uv.x + uv2.uv.x;
	num.uv.y = uv1.uv.y + uv2.uv.y;
	return num;
}

// Negation of vector
// u -> vector
// @result -> result of computation
float3 minusVector3D(float3 u) 
{
	float3 num;
	num.x = -u.x;
	num.y = -u.y;
	num.z = -u.z;
	return num;
}

// Swap value of floats
// u,v -> floats
// @result -> result of computation
void swapFloat(float* u, float* v)
{
	float tmp = *u;
	*u = *v;
	*v = tmp;
}

TPoint3D PointMinusPoint(TPoint3D t1, TPoint3D t2)
{
	t1.x -= t2.x;
	t1.y -= t2.y;
	t1.z -= t2.z;
	return t1;
}

TPoint3D PointDivPoint(TPoint3D t1, TPoint3D t2)
{
	t1.x /= t2.x;
	t1.y /= t2.y;
	t1.z /= t2.z;
	return t1;
}

#endif // _KERNEL_TYPES_