/*
	Name: gpu_types.h
	Desc: Data structures for export on OpenCL device. 
	Author: Karel Brezina (xbrezi13)
*/

#ifndef _GPU_TYPES_H_
#define _GPU_TYPES_H_

#include "../include/cl_platform.h"
#include "matrix.h"

#define SPHERE_INDEX 0
#define TRIANGLE_INDEX 1

// Basic structures.
// Almost all structures are equialent of class's data part of 
// CPU version pathtracing. Here they are marked as T... 
// Used for export data to compute on OpenCL device.

// RGB representation of color. 
typedef cl_float3 TColor;
// XYZ representation of vector.
typedef cl_float3 TVector3D;
// XYZ representation of point. 
typedef cl_float3 TPoint3D;
// 4x4 matrice. 
typedef struct {
	cl_float4 m[4];
} TMatrix;
// Store diff between two vectors or points. 
typedef struct {
	TVector3D dx;
	TVector3D dy;
} TDiff;
// Store info about compute screen.
typedef struct {
	cl_uint width;
	cl_uint height;
} TImage;
// Store info about object material.
typedef struct {
	TColor color;
	cl_float reflectivity;
	cl_float transparency;
	cl_float refractionIndex;
	bool isMat;
} TMaterial;
// Index to texture.
typedef struct {
	cl_float2 uv;
} TUV;
// Represent vertex of triangle
typedef struct {
	cl_int p;
	cl_int n;
	cl_int t;
} TVertex;

// Advanced structures.
// Consist from basic structures.

// Store info about computed ray
typedef struct {
	TDiff dp;
	TDiff dd;
	TPoint3D orig;
	TVector3D dir;
	cl_float minT;
	cl_float maxT;
	cl_char padding[8];
} TRay;

// Store info about set camera.
typedef struct {
	TPoint3D mOrigin;				///< Position of camera origin.
	TVector3D mForward;			///< Camera frame forward vector.
	TVector3D mRight;				///< Camera frame right vector.
	TVector3D mUp;					///< Camera frame up vector.
	TImage mImage;				///< Ptr to the output image.
	cl_float mNearPlane;			///< Distance to near viewing plane.
	cl_float mFarPlane;			///< Distance to far viewing plane.
	cl_float mFieldOfView;			///< Horizontal field-of-view
	cl_float mImageExtentX;		///< Extent of image plane in positive x direction.
	cl_float mImageExtentY;		///< Extent of image plane in positive y direction.
	cl_char padding[4];
} TCamera;
// Store info about sphere.
typedef struct {
	TMatrix InvWorldTransform;
	TMatrix WorldTransform;
	TMaterial material;
	cl_float radius;
	cl_uint index;
	cl_uint rayID;
	cl_char padding[4];
} TSphere;
// Store info about triangle.
typedef struct {
	TMaterial material;
	TVector3D planes[3];
	TVector3D planeOffsets;
	TVertex vtx[3];
	cl_uint mesh;
	cl_uint index;
	cl_uint rayID;
	//cl_char padding[4];
} TTriangle;
// Store info about mesh. Addition structure for triangle intersection.
typedef struct {
	TMaterial material;
	TPoint3D origVtxP;
	TVector3D origVtxN;
	TPoint3D vtxP;
	TVector3D vtxN;
	TUV vtxUV;
	cl_char padding[8];
} TMesh;
// Store info about light.
typedef struct {
	TColor col;
	TPoint3D position;
	TColor radiance;
	TPoint3D worldPos;
	cl_float intensity;
	cl_char padding[12];
} TLight;

// Help functions for exporting data.
static inline void Vector3DToFloat3(Vector3D& src, cl_float3& dest) {
	dest.s[0] = src.x;
	dest.s[1] = src.y;
	dest.s[2] = src.z;
}

static inline void Point3DtoFloat3(Point3D& src, cl_float3& dest) {
	dest.s[0] = src.x;
	dest.s[1] = src.y;
	dest.s[2] = src.z;
}

static inline void setColor(TColor& col, float r, float g, float b) {
	col.s[0] = r;
	col.s[1] = g;
	col.s[2] = b;
}

struct TObject {
	cl_uint index;
	cl_char type;
};

struct TBoxLink {
	cl_uint objStartIndex;
	cl_uint objSize;
};

struct TOctreeBox {
	TPoint3D boxMin;
	TPoint3D boxMax;
	TPoint3D middlePoint;
	TBoxLink boxLink;
	cl_uint children[8];
	cl_uint leaf;
	cl_char padding2[1];
};

struct TUniGrid {
	TPoint3D boxMin;
	TPoint3D boxMax;
	TPoint3D wotld_size;
	TPoint3D cell_size;
	cl_uint grid_size;
	cl_char padding[12];
};

struct TBVHNode {
	TPoint3D boxMin;
	TPoint3D boxMax;
	TBoxLink indexObj;
	cl_uint leaf;
	cl_uint indexNode;
};

#endif // _GPU_TYPES_H_