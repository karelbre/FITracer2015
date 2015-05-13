/*
	Name: gpu_kernel.cl
	Desc: File include kernels for PT with all accelerated data structures. 
	Author: Karel Brezina (xbrezi13)
*/

#include "kernel_RNG.h"
#include "kernel_trace_list.h"
#include "kernel_trace_octree.h"
#include "kernel_trace_unigrid.h"
#include "kernel_trace_bvh.h"

// Enable printf on AMD's OpenCL platform.
#pragma OPENCL EXTENSION cl_amd_printf : enable

// Depth of path computation.
#define DEPTH 5

/////////////////
// List kernels //
////////////////

// Kernel for compute with no accelerated data structure.
// inPixelColor -> texture with actual progress
// outPixelColor -> texture for new result
// cam -> set camera
// spheres -> buffer of all spheres
// cnt_spheres -> count of sphere's buffer
// triangles -> buffer of all triangles
// cnt_triangles -> count of triangle's buffer
// meshes -> buffer of all meshes
// range_meshes -> buffer with ranges of every mesh
// size_ra_me -> count of range's buffer
// light -> buffer of all lights
// size_li -> count of light's buffer
// seed -> seed for random number generator
// samples -> number of samples in texture
__kernel void gpu_pt_list(
	__read_only image2d_t inPixelColor, // 0
	__write_only image2d_t outPixelColor, // 1
	__global TCamera* cam, // 2
	__global TSphere* spheres, // 3
	__local unsigned int* spheresMem, // 4
	__global unsigned int* cnt_spheres, // 5
	__global TTriangle* triangles, // 6
	__local unsigned int* trianglesMem, // 7
	__global unsigned int* cnt_triangles, // 8
	__global TMesh* meshes, // 9
	__global unsigned int* range_meshes, // 10
	__global unsigned int* size_ra_me, // 11
	__global TLight* lights, // 12
	__global unsigned int* size_li, // 13
	unsigned int seed, // 14
	unsigned int samples // 15
	) 
{
	// Get index of pixel's width.
	unsigned int x = get_global_id(0); 
	// Get index of pixel's height.
	unsigned int y = get_global_id(1);  

	// Set camera as local var.
	TCamera cam2 = cam[0];

	// Set random range <0,1) to pixel coordinate.
	uint2 nums; nums.x = x + seed; nums.y = y + seed;
	float sx = x + randomFloat(&nums, 1);
	float sy = y + randomFloat(&nums, 1);

	// Compute ray structure.
	TRay ray = getRay(cam2, sx, sy);
	unsigned int cnt_SpheresLoc = cnt_spheres[0];
	unsigned int cnt_TrianglesLoc = cnt_triangles[0];
	unsigned int cnt_RangeMeshesLoc = size_ra_me[0];
	unsigned int cnt_LightsLoc = size_li[0];

	// Compute color of pixel.
	TColor res = trace_list(ray, DEPTH, nums, spheres, cnt_SpheresLoc, triangles, cnt_TrianglesLoc,
		meshes, range_meshes, cnt_RangeMeshesLoc, lights, cnt_LightsLoc);

	// Computed color.
	float4 computeColor; computeColor.xyz = res.xyz; computeColor.w = 1.f;

	// Set sampler.
	const sampler_t samplerTex = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_NONE | CLK_FILTER_NEAREST;
	// Get actual color in texture.
	int2 pos; pos.x = x; pos.y = y;
	float4 actualColor = read_imagef(inPixelColor, samplerTex, pos);

	// Mix both colors.
	float4 majorColor = actualColor * (samples / (samples + 1.f));
	actualColor = majorColor + (computeColor * (1.f / (samples + 1.f)));

	// Write result color.
	write_imagef(outPixelColor, pos, actualColor);
}

// Kernel for compute first pixel with no accelerated data structure.
// outPixelColor -> texture for new result
// cam -> set camera
// spheres -> buffer of all spheres
// cnt_spheres -> count of sphere's buffer
// triangles -> buffer of all triangles
// cnt_triangles -> count of triangle's buffer
// meshes -> buffer of all meshes
// range_meshes -> buffer with ranges of every mesh
// size_ra_me -> count of range's buffer
// light -> buffer of all lights
// size_li -> count of light's buffer
// seed -> seed for random number generator
__kernel void gpu_pt_list_first(
	__write_only image2d_t outPixelColor, // 0
	__global TCamera* cam, // 1
	__global TSphere* spheres, // 2
	__local unsigned int* spheresMem, // 3
	__global unsigned int* cnt_spheres, // 4
	__global TTriangle* triangles, // 5
	__local unsigned int* trianglesMem, // 6
	__global unsigned int* cnt_triangles, // 7
	__global TMesh* meshes, // 8
	__global unsigned int* range_meshes, // 9
	__global unsigned int* size_ra_me, // 10
	__global TLight* lights, // 11
	__global unsigned int* size_li, // 12
	unsigned int seed // 13
	)
{
	// Get index of pixel's width.
	unsigned int x = get_global_id(0);
	// Get index of pixel's height.
	unsigned int y = get_global_id(1);

	// Set camera as local var.
	TCamera cam2 = cam[0];

	// Set random range <0,1) to pixel coordinate.
	uint2 nums; nums.x = x + seed; nums.y = y + seed;
	float sx = x + randomFloat(&nums, 1);
	float sy = y + randomFloat(&nums, 1);

	// Compute ray structure.
	TRay ray = getRay(cam2, sx, sy);
	unsigned int cnt_SpheresLoc = cnt_spheres[0];
	unsigned int cnt_TrianglesLoc = cnt_triangles[0];
	unsigned int cnt_RangeMeshesLoc = size_ra_me[0];
	unsigned int cnt_LightsLoc = size_li[0];

	// Compute color of pixel.
	TColor res = trace_list(ray, DEPTH, nums, spheres, cnt_SpheresLoc, triangles, cnt_TrianglesLoc,
		meshes, range_meshes, cnt_RangeMeshesLoc, lights, cnt_LightsLoc);

	// Computed color.
	float4 computeColor; computeColor.xyz = res.xyz; computeColor.w = 1.f;
	int2 pos; pos.x = x; pos.y = y;
	// Write result color.
	write_imagef(outPixelColor, pos, computeColor);
}

////////////////////
// Octree kernels. //
///////////////////

// Kernel for compute with octree accelerated data structure.
// inPixelColor -> texture with actual progress
// outPixelColor -> texture for new result
// cam -> set camera
// spheres -> buffer of all spheres
// cnt_spheres -> count of sphere's buffer
// triangles -> buffer of all triangles
// cnt_triangles -> count of triangle's buffer
// meshes -> buffer of all meshes
// range_meshes -> buffer with ranges of every mesh
// size_ra_me -> count of range's buffer
// light -> buffer of all lights
// size_li -> count of light's buffer
// seed -> seed for random number generator
// samples -> number of samples in texture
// octree -> buffer of all octree data structures
// octree_size -> count of octree's buffer
// objects -> indexes of objects belong to octree
__kernel void gpu_pt_octree(
	__read_only image2d_t inPixelColor, // 0
	__write_only image2d_t outPixelColor, // 1
	__global TCamera* cam, // 2
	__global TSphere* spheres, // 3
	__local unsigned char* spheresMem, // 4
	//__global unsigned char* spheresMem, // 4
	__global unsigned int* cnt_spheres, // 5
	__global TTriangle* triangles, // 6
	__local unsigned char* trianglesMem, // 7
	//__global unsigned char* trianglesMem, // 7
	__global unsigned int* cnt_triangles, // 8
	__global TMesh* meshes, // 9
	__global unsigned int* range_meshes, // 10
	__global unsigned int* size_ra_me, // 11
	__global TLight* lights, // 12
	__global unsigned int* size_li, // 13
	unsigned int seed, // 14
	unsigned int samples, // 15
	__global TOctreeBox* octree, // 16
	__global unsigned int* octree_size, // 17
	__global TObject* objects // 18
	)
{
	// Get index of pixel's width.
	unsigned int x = get_global_id(0);
	// Get index of pixel's height.
	unsigned int y = get_global_id(1);

	unsigned int cnt_SpheresLoc = cnt_spheres[0];
	unsigned int cnt_TrianglesLoc = cnt_triangles[0];
	unsigned int cnt_RangeMeshesLoc = size_ra_me[0];
	unsigned int cnt_LightsLoc = size_li[0];
	unsigned int cnt_OctreeLoc = octree_size[0];

	// Set camera as local var.
	TCamera cam2 = cam[0];

	// Set random range <0,1) to pixel coordinate.
	uint2 nums; nums.x = x + seed; nums.y = y + seed;
	float sx = x + randomFloat(&nums, 1);
	float sy = y + randomFloat(&nums, 1);

	// Compute ray structure.
	TRay ray = getRay(cam2, sx, sy);

	// Compute pixel.
	TColor res = trace_octree(ray, DEPTH, nums, spheres, cnt_SpheresLoc, triangles, cnt_TrianglesLoc,
		meshes, range_meshes, cnt_RangeMeshesLoc, lights, cnt_LightsLoc, octree, cnt_OctreeLoc, objects);

	// Computed color.
	float4 computeColor; computeColor.xyz = res.xyz; computeColor.w = 1.f;

	// Set sampler.
	const sampler_t samplerTex = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_NONE | CLK_FILTER_NEAREST;
	// Get actual color in texture.
	int2 pos; pos.x = x; pos.y = y;
	float4 actualColor = read_imagef(inPixelColor, samplerTex, pos);

	// Mix both colors.
	float4 majorColor = actualColor * (samples / (samples + 1.f));
	actualColor = majorColor + (computeColor * (1.f / (samples + 1.f)));

	// Write result color.
	write_imagef(outPixelColor, pos, actualColor);
}

// Kernel for first compute with octree accelerated data structure.
// outPixelColor -> texture for new result
// cam -> set camera
// spheres -> buffer of all spheres
// cnt_spheres -> count of sphere's buffer
// triangles -> buffer of all triangles
// cnt_triangles -> count of triangle's buffer
// meshes -> buffer of all meshes
// range_meshes -> buffer with ranges of every mesh
// size_ra_me -> count of range's buffer
// light -> buffer of all lights
// size_li -> count of light's buffer
// seed -> seed for random number generator
// octree -> buffer of all octree data structures
// octree_size -> count of octree's buffer
// objects -> indexes of objects belong to octree
__kernel void gpu_pt_octree_first(
	__write_only image2d_t outPixelColor, // 0
	__global TCamera* cam, // 1
	__global TSphere* spheres, // 2
	__local unsigned char* spheresMem, // 3
	//__global unsigned char* spheresMem, // 3
	__global unsigned int* cnt_spheres, // 4
	__global TTriangle* triangles, // 5
	__local unsigned char* trianglesMem, // 6
	//__global unsigned char* trianglesMem, // 6
	__global unsigned int* cnt_triangles, // 7
	__global TMesh* meshes, // 8
	__global unsigned int* range_meshes, // 9
	__global unsigned int* size_ra_me, // 10
	__global TLight* lights, // 11
	__global unsigned int* size_li, // 12
	unsigned int seed, // 13
	__global TOctreeBox* octree, // 14
	__global unsigned int* octree_size, // 15
	__global TObject* objects // 16
	)
{
	// Get index of pixel's width.
	unsigned int x = get_global_id(0);
	// Get index of pixel's height.
	unsigned int y = get_global_id(1);

	unsigned int cnt_SpheresLoc = cnt_spheres[0];
	unsigned int cnt_TrianglesLoc = cnt_triangles[0];
	unsigned int cnt_RangeMeshesLoc = size_ra_me[0];
	unsigned int cnt_LightsLoc = size_li[0];
	unsigned int cnt_OctreeLoc = octree_size[0];

	// Set camera as local var.
	TCamera cam2 = cam[0];

	// Set random range <0,1) to pixel coordinate.
	uint2 nums; nums.x = x + seed; nums.y = y + seed;
	float sx = x + randomFloat(&nums, 1);
	float sy = y + randomFloat(&nums, 1);

	// Compute ray structure.
	TRay ray = getRay(cam2, sx, sy);

	// Compute pixel.
	TColor res = trace_octree(ray, DEPTH, nums, spheres, cnt_SpheresLoc, triangles, cnt_TrianglesLoc,
		meshes, range_meshes, cnt_RangeMeshesLoc, lights, cnt_LightsLoc, octree, cnt_OctreeLoc, objects);

	// Computed color.
	float4 computeColor; computeColor.xyz = res.xyz; computeColor.w = 1.f;
	int2 pos; pos.x = x; pos.y = y;
	// Write result color.
	write_imagef(outPixelColor, pos, computeColor);
}

/////////////////////////
// Uniform grid kernels //
////////////////////////

// Kernel for compute with with uniform grid accelerated data structure.
// inPixelColor -> texture with actual progress
// outPixelColor -> texture for new result
// cam -> set camera
// spheres -> buffer of all spheres
// cnt_spheres -> count of sphere's buffer
// triangles -> buffer of all triangles
// cnt_triangles -> count of triangle's buffer
// meshes -> buffer of all meshes
// range_meshes -> buffer with ranges of every mesh
// size_ra_me -> count of range's buffer
// light -> buffer of all lights
// size_li -> count of light's buffer
// seed -> seed for random number generator
// samples -> number of samples in texture
// infoUniGrid -> control infos about Uniform grid data structure
// uniGrid -> buffer of all uniform grid data structure
// objects -> indexes of objects in uniform grid
__kernel void gpu_pt_unigrid(
	__read_only image2d_t inPixelColor, // 0
	__write_only image2d_t outPixelColor, // 1
	__global TCamera* cam, // 2
	__global TSphere* spheres, // 3
	__local unsigned char* spheresMem, // 4
	//__global unsigned char* spheresMem, // 4
	__global unsigned int* cnt_spheres, // 5
	__global TTriangle* triangles, // 6
	__local unsigned char* trianglesMem, // 7
	//__global unsigned char* trianglesMem, // 7
	__global unsigned int* cnt_triangles, // 8
	__global TMesh* meshes, // 9
	__global unsigned int* range_meshes, // 10
	__global unsigned int* size_ra_me, // 11
	__global TLight* lights, // 12
	__global unsigned int* size_li, // 13
	unsigned int seed, // 14
	unsigned int samples, // 15
	__global TUniGrid* infoUniGrid, // 16
	__global TBoxLink* uniGrid, // 17
	__global TObject* objects // 18
	)
{
	// Get index of pixel's width.
	unsigned int x = get_global_id(0);
	// Get index of pixel's height.
	unsigned int y = get_global_id(1);

	unsigned int cnt_SpheresLoc = cnt_spheres[0];
	unsigned int cnt_TrianglesLoc = cnt_triangles[0];
	unsigned int cnt_RangeMeshesLoc = size_ra_me[0];
	unsigned int cnt_LightsLoc = size_li[0];

	// Set camera as local var.
	TCamera cam2 = cam[0];

	// Set random range <0,1) to pixel coordinate.
	uint2 nums; nums.x = x + seed; nums.y = y + seed;
	float sx = x + randomFloat(&nums, 1);
	float sy = y + randomFloat(&nums, 1);

	// Compute ray structure.
	TRay ray = getRay(cam2, sx, sy);
	ray.rayID = true;

	// Compute pixel.
	TColor res = trace_unigrid(ray, DEPTH, nums, spheres, cnt_SpheresLoc, triangles, cnt_TrianglesLoc,
		meshes, range_meshes, cnt_RangeMeshesLoc, lights, cnt_LightsLoc, infoUniGrid, uniGrid, objects);

	// Computed color.
	float4 computeColor; computeColor.xyz = res.xyz; computeColor.w = 1.f;

	// Set sampler.
	const sampler_t samplerTex = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_NONE | CLK_FILTER_NEAREST;
	// Get actual color in texture.
	int2 pos; pos.x = x; pos.y = y;
	float4 actualColor = read_imagef(inPixelColor, samplerTex, pos);

	// Mix both colors.
	float4 majorColor = actualColor * (samples / (samples + 1.f));
	actualColor = majorColor + (computeColor * (1.f / (samples + 1.f)));

	// Write result color.
	write_imagef(outPixelColor, pos, actualColor);
}

// Kernel for first compute with with uniform grid accelerated data structure.
// outPixelColor -> texture for new result
// cam -> set camera
// spheres -> buffer of all spheres
// cnt_spheres -> count of sphere's buffer
// triangles -> buffer of all triangles
// cnt_triangles -> count of triangle's buffer
// meshes -> buffer of all meshes
// range_meshes -> buffer with ranges of every mesh
// size_ra_me -> count of range's buffer
// light -> buffer of all lights
// size_li -> count of light's buffer
// seed -> seed for random number generator
// samples -> number of samples in texture
// infoUniGrid -> control infos about Uniform grid data structure
// uniGrid -> buffer of all uniform grid data structure
// objects -> indexes of objects in uniform grid
__kernel void gpu_pt_unigrid_first(
	__write_only image2d_t outPixelColor, // 0
	__global TCamera* cam, // 1
	__global TSphere* spheres, // 2
	__local unsigned char* spheresMem, // 3
	//__global unsigned char* spheresMem, // 3
	__global unsigned int* cnt_spheres, // 4
	__global TTriangle* triangles, // 5
	__local unsigned char* trianglesMem, // 6
	//__global unsigned char* trianglesMem, // 6
	__global unsigned int* cnt_triangles, // 7
	__global TMesh* meshes, // 8
	__global unsigned int* range_meshes, // 9
	__global unsigned int* size_ra_me, // 10
	__global TLight* lights, // 11
	__global unsigned int* size_li, // 12
	unsigned int seed, // 13
	__global TUniGrid* infoUniGrid, // 14
	__global TBoxLink* uniGrid, // 15
	__global TObject* objects // 16
	)
{
	// Get index of pixel's width.
	unsigned int x = get_global_id(0);
	// Get index of pixel's height.
	unsigned int y = get_global_id(1);

	unsigned int cnt_SpheresLoc = cnt_spheres[0];
	unsigned int cnt_TrianglesLoc = cnt_triangles[0];
	unsigned int cnt_RangeMeshesLoc = size_ra_me[0];
	unsigned int cnt_LightsLoc = size_li[0];

	// Set camera as local var.
	TCamera cam2 = cam[0];

	// Set random range <0,1) to pixel coordinate.
	uint2 nums; nums.x = x + seed; nums.y = y + seed;
	float sx = x + randomFloat(&nums, 1);
	float sy = y + randomFloat(&nums, 1);

	// Compute ray structure.
	TRay ray = getRay(cam2, sx, sy);

	// Compute pixel.
	TColor res = trace_unigrid(ray, DEPTH, nums, spheres, cnt_SpheresLoc, triangles, cnt_TrianglesLoc,
		meshes, range_meshes, cnt_RangeMeshesLoc, lights, cnt_LightsLoc, infoUniGrid, uniGrid, objects);

	// Computed color.
	float4 computeColor; computeColor.xyz = res.xyz; computeColor.w = 1.f;
	int2 pos; pos.x = x; pos.y = y;
	// Write result color.
	write_imagef(outPixelColor, pos, computeColor);
}

////////////////
// BVH kernels //
///////////////

// Kernel for compute with BVH accelerated data structure.
// inPixelColor -> texture with actual progress
// outPixelColor -> texture for new result
// cam -> set camera
// spheres -> buffer of all spheres
// cnt_spheres -> count of sphere's buffer
// triangles -> buffer of all triangles
// cnt_triangles -> count of triangle's buffer
// meshes -> buffer of all meshes
// range_meshes -> buffer with ranges of every mesh
// size_ra_me -> count of range's buffer
// light -> buffer of all lights
// size_li -> count of light's buffer
// seed -> seed for random number generator
// samples -> number of samples in texture
// bvhNodes -> buffer of all BVH data structures
// objects -> indexes of objects in bvh 
__kernel void gpu_pt_bvh(
	__read_only image2d_t inPixelColor, // 0
	__write_only image2d_t outPixelColor, // 1
	__global TCamera* cam, // 2
	__global TSphere* spheres, // 3
	__local unsigned int* spheresMem, // 4
	__global unsigned int* cnt_spheres, // 5
	__global TTriangle* triangles, // 6
	__local unsigned int* trianglesMem, // 7
	__global unsigned int* cnt_triangles, // 8
	__global TMesh* meshes, // 9
	__global unsigned int* range_meshes, // 10
	__global unsigned int* size_ra_me, // 11
	__global TLight* lights, // 12
	__global unsigned int* size_li, // 13
	unsigned int seed, // 14
	unsigned int samples, // 15
	__global TBVHNode* bvhNodes, // 16
	__global TObject* objects // 17
	)
{
	// Get index of pixel's width.
	unsigned int x = get_global_id(0);
	// Get index of pixel's height.
	unsigned int y = get_global_id(1);

	// Set camera as local var.
	TCamera cam2 = cam[0];

	// Set random range <0,1) to pixel coordinate.
	uint2 nums; nums.x = x + seed; nums.y = y + seed;
	float sx = x + randomFloat(&nums, 1);
	float sy = y + randomFloat(&nums, 1);

	// Compute ray structure.
	TRay ray = getRay(cam2, sx, sy);
	unsigned int cnt_SpheresLoc = cnt_spheres[0];
	unsigned int cnt_TrianglesLoc = cnt_triangles[0];
	unsigned int cnt_RangeMeshesLoc = size_ra_me[0];
	unsigned int cnt_LightsLoc = size_li[0];

	// Compute pixel.
	TColor res = trace_bvh(ray, DEPTH, nums, spheres, cnt_SpheresLoc, triangles, cnt_TrianglesLoc,
		meshes, range_meshes, cnt_RangeMeshesLoc, lights, cnt_LightsLoc, bvhNodes, objects);

	// Computed color.
	float4 computeColor; computeColor.xyz = res.xyz; computeColor.w = 1.f;

	// Set sampler.
	const sampler_t samplerTex = CLK_NORMALIZED_COORDS_FALSE | CLK_ADDRESS_NONE | CLK_FILTER_NEAREST;
	// Get actual color in texture.
	int2 pos; pos.x = x; pos.y = y;
	float4 actualColor = read_imagef(inPixelColor, samplerTex, pos);

	// Mix both colors.
	float4 majorColor = actualColor * (samples / (samples + 1.f));
	actualColor = majorColor + (computeColor * (1.f / (samples + 1.f)));

	// Write result color.
	write_imagef(outPixelColor, pos, actualColor);
}

// Kernel for first compute with BVH accelerated data structure.
// outPixelColor -> texture for new result
// cam -> set camera
// spheres -> buffer of all spheres
// cnt_spheres -> count of sphere's buffer
// triangles -> buffer of all triangles
// cnt_triangles -> count of triangle's buffer
// meshes -> buffer of all meshes
// range_meshes -> buffer with ranges of every mesh
// size_ra_me -> count of range's buffer
// light -> buffer of all lights
// size_li -> count of light's buffer
// seed -> seed for random number generator
// bvhNodes -> buffer of all BVH data structures
// objects -> indexes of objects in bvh 
__kernel void gpu_pt_bvh_first(
	__write_only image2d_t outPixelColor, // 0
	__global TCamera* cam, // 1
	__global TSphere* spheres, // 2
	__local unsigned int* spheresMem, // 3
	__global unsigned int* cnt_spheres, // 4
	__global TTriangle* triangles, // 5
	__local unsigned int* trianglesMem, // 6
	__global unsigned int* cnt_triangles, // 7
	__global TMesh* meshes, // 8
	__global unsigned int* range_meshes, // 9
	__global unsigned int* size_ra_me, // 10
	__global TLight* lights, // 11
	__global unsigned int* size_li, // 12
	unsigned int seed, // 13
	__global TBVHNode* bvhNodes, // 14
	__global TObject* objects // 15
	)
{
	// Get index of pixel's width.
	unsigned int x = get_global_id(0);
	// Get index of pixel's height.
	unsigned int y = get_global_id(1);

	// Set camera as local var.
	TCamera cam2 = cam[0];

	// Set random range <0,1) to pixel coordinate.
	uint2 nums; nums.x = x + seed; nums.y = y + seed;
	float sx = x + randomFloat(&nums, 1);
	float sy = y + randomFloat(&nums, 1);

	// Compute ray structure.
	TRay ray = getRay(cam2, sx, sy);
	unsigned int cnt_SpheresLoc = cnt_spheres[0];
	unsigned int cnt_TrianglesLoc = cnt_triangles[0];
	unsigned int cnt_RangeMeshesLoc = size_ra_me[0];
	unsigned int cnt_LightsLoc = size_li[0];

	// Compute pixel.
	TColor res = trace_bvh(ray, DEPTH, nums, spheres, cnt_SpheresLoc, triangles, cnt_TrianglesLoc,
		meshes, range_meshes, cnt_RangeMeshesLoc, lights, cnt_LightsLoc, bvhNodes, objects);

	// Computed color.
	float4 computeColor; computeColor.xyz = res.xyz; computeColor.w = 1.f;
	int2 pos; pos.x = x; pos.y = y;
	// Write result color.
	write_imagef(outPixelColor, pos, computeColor);
}