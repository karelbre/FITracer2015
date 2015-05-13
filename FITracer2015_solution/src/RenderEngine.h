/*
	Name: RenderEngine.h
	Desc: Context for compute PT.
	Author: Karel Brezina (xbrezi13)
*/

#ifndef _RENDER_ENGINE_H_
#define _RENDER_ENGINE_H_

#include <iostream>
#include "defines.h"
#include "scene.h"
#include "image.h"
#include "camera.h"
#include "mesh.h"
#include "sphere.h"
#include "diffuse.h"
#include "whittedtracer.h"
#include "pointlight.h"
#include "lightprobe.h"
#include "listaccelerator.h"
#include "texture.h"
#include "bvhaccelerator.h"
#include "cornellscene.h"
#include "pathtracer.h"
#include <string>
#include "octreeaccelerator.h"
#include "uniformaccelerator.h"
#include "SDLGLContext.h"
#include "gpu_pathtracer.h"
#include "gpu_types.h"

using namespace std;

float ran1(long *idum);
static bool changeDirectory(const char* directory);
static void changeToRootDirectory(string directory);
void checkError(int err);

class RenderEnginePT {
public:
	void InitPT(SDLGLContext* context);
	void ComputePT();
	void Destroy();
private:
	void SwapImages(GPUPathtracer* gpu_pt, unsigned int indexImg);
	bool CheckSettings(PathTracer* pt, GPUPathtracer* gpu_pt, int pressedAS, int pressedRenderer);
	void CheckSettingsFirst(PathTracer* pt, GPUPathtracer* gpu_pt, int pressedAS, int pressedRenderer);
	unsigned int CreateOctree(std::vector<TOctreeBox>* octreeBuffer, std::vector<TObject>* objectBuffer,
		OctreeAccelerator* octADS, unsigned int& indexOctreeBuffer);
	void CreateUniGrid(TUniGrid* infoUniGrid, std::vector<TBoxLink>* uniGridBuffer, 
					   std::vector<TObject>* objBufferUniGrid, UniformAccelerator* uniADS);
	void CreateBVH(std::vector<TBVHNode>* nodes,std::vector<TObject>* objBufferBVH, BVHAccelerator* bvhADS);

	unsigned int GetIndexSphere(Sphere* sp);
	unsigned int GetIndexTriangle(Triangle* tr);
private:
	GPUPathtracer gpu_pt;
	SDLGLContext* contextAPI;
	size_t local_size[2];
	size_t global_size[2];

	Scene* sceneList;
	Scene* sceneOctree;
	Scene* sceneUniGrid;
	Scene* sceneBVH;
	Image* output;
	Camera* camera;

	cl_program pt_prog;
	cl_kernel pt_kernel;

	std::vector<TSphere> spheres;
	std::vector<TTriangle> triangles;
	std::vector<TMesh> meshes;
	std::vector<unsigned int> size_meshes;
	std::vector<TLight> lights;
	std::vector<TOctreeBox> octreeBuffer;
	std::vector<TObject> objectBufferOct;
	std::vector<TObject> objectBufferUniGrid;
	std::vector<TBVHNode> bvhBuffer;
	std::vector<TObject> objectBufferBVH;

	cl_mem pt_col[2];
	cl_mem pt_cam;
	cl_mem pt_sphere;
	cl_mem pt_sphereLoc;
	cl_mem pt_triangle;
	cl_mem pt_triangleLoc;
	cl_mem pt_meshes;
	cl_mem pt_range_meshes;
	cl_mem pt_light;
	cl_mem pt_octree;
	cl_mem pt_objectsOct;
	cl_mem pt_uniGrid;
	cl_mem pt_uniGridBuffer;
	cl_mem pt_objectsUniGrid;
	cl_mem pt_BVH;
	cl_mem pt_objectsBVH;

	cl_mem pt_cntSpheres;
	cl_mem pt_cntTriangles;
	cl_mem pt_cntRangeMeshes;
	cl_mem pt_cntLights;
	cl_mem pt_cntOctree;
};

#endif // _RENDER_ENGINE_H_