/*
	Name: RenderingEngine.cpp
	Desc: Compute context for PT.
	Author: Karel Brezina (xbrezi13)
*/

#include "RenderEngine.h"

// Used renderer.
unsigned int usedRenderer = NO_OPTION;
// Used accelerate structure.
unsigned int usedAS = NO_OPTION;

void RenderEnginePT::InitPT(SDLGLContext* context)
{
	TCamera* cam = new TCamera;
	int err = 0;
	global_size[0] = context->GetResultResolution();
	global_size[1] = context->GetResultResolution();
	local_size[0] = global_size[0] / 50;
	local_size[1] = global_size[1] / 50;

	/*global_size[0] = 1;
	global_size[1] = 1;
	local_size[0] = 1;
	local_size[1] = 1;*/

	contextAPI = context;

	// Set device, context and command queue.
	err = gpu_pt.initGPU();
	checkError(err);

	// Build program from file.
	err = gpu_pt.createGPUprogram(&pt_prog, "./kernels/gpu_kernel.cl");
	checkError(err);

	// Load program to GPU context.
	gpu_pt.loadGPUprogram(&pt_prog);

	// Create and load kernel from loaded program.
	err = gpu_pt.loadGPUkernel(AS_LIST, "gpu_pt_list");
	checkError(err);
	err = gpu_pt.loadGPUkernel(AS_OCTREE, "gpu_pt_octree");
	checkError(err);
	err = gpu_pt.loadGPUkernel(AS_UNIFORM_GRID, "gpu_pt_unigrid");
	checkError(err);
	err = gpu_pt.loadGPUkernel(AS_BVH, "gpu_pt_bvh");
	checkError(err);
	err = gpu_pt.loadGPUkernel(AS_LIST_FIRST, "gpu_pt_list_first");
	checkError(err);
	err = gpu_pt.loadGPUkernel(AS_OCTREE_FIRST, "gpu_pt_octree_first");
	checkError(err);
	err = gpu_pt.loadGPUkernel(AS_UNIFORM_GRID_FIRST, "gpu_pt_unigrid_first");
	checkError(err);
	err = gpu_pt.loadGPUkernel(AS_BVH_FIRST, "gpu_pt_bvh_first");
	checkError(err);

	// Set working directory.
	changeToRootDirectory("data");

	// Create output image.
	output = new Image(global_size[0], global_size[1]);
	//output = new Image(100, 100);
	camera = new Camera(output);
	setupCornellCamera(camera);

	// Build scene for no ADS.
	sceneList = new Scene(new ListAccelerator());
	buildCornellScene(sceneList);
	sceneList->add(camera);
	sceneList->prepare();
	// Build scene for Octree.
	sceneOctree = new Scene(new OctreeAccelerator());
	buildCornellScene(sceneOctree);
	sceneOctree->add(camera);
	sceneOctree->prepare();
	// Build scene for Uniform Grid.
	sceneUniGrid = new Scene(new UniformAccelerator());
	buildCornellScene(sceneUniGrid);
	sceneUniGrid->add(camera);
	sceneUniGrid->prepare();
	// Build scene for BVH.
	sceneBVH = new Scene(new BVHAccelerator());
	buildCornellScene(sceneBVH);
	sceneBVH->add(camera);
	sceneBVH->prepare();
	// Prepare data for exporting to OpenCL device.
	// Get all cameras.
	camera->getSettings(*cam);
	// Get all spheres, triangles, meshes.
	sceneList->getObjects(&spheres, &triangles, &meshes, &size_meshes);
	// Get all lights.
	sceneList->getLights(&lights);

	// Write exported data to buffers.
	err = gpu_pt.createGPUimage2D(pt_col, context->GetResultTextures(), CL_MEM_WRITE_ONLY, global_size[0], global_size[1]);
	checkError(err);
	err = gpu_pt.createGPUbuffer(&pt_cam, CL_MEM_READ_ONLY, sizeof(TCamera));
	checkError(err);
	err = gpu_pt.writeGPUobjects(spheres, pt_sphere, triangles, pt_triangle, meshes,
		pt_meshes, size_meshes, pt_range_meshes);
	checkError(err);
	err = gpu_pt.writeGPUlights(lights, pt_light);
	checkError(err);
	err = gpu_pt.writeGPUdata(&pt_cam, 0, sizeof(TCamera), cam);
	checkError(err);

	unsigned int indexOctreeBuffer = 1;
	CreateOctree(&octreeBuffer, &objectBufferOct, (OctreeAccelerator*)sceneOctree->getAccelerator(), 
		indexOctreeBuffer);
	TUniGrid infoUniGrid;
	std::vector<TBoxLink> uniGridBuffer;
	CreateUniGrid(&infoUniGrid, &uniGridBuffer, &objectBufferUniGrid, 
				 (UniformAccelerator*)sceneUniGrid->getAccelerator());
	CreateBVH(&bvhBuffer, &objectBufferBVH, (BVHAccelerator*)sceneBVH->getAccelerator());

	cl_uint cnt_sphere = spheres.size();
	cl_uint cnt_triangle = triangles.size();
	cl_uint cnt_range_mesh = size_meshes.size();
	cl_uint cnt_light = lights.size();
	cl_uint cnt_octree = octreeBuffer.size();
	
	/*gpu_pt.createGPUbuffer(&pt_sphereLoc, CL_MEM_READ_WRITE, 
		(((cnt_sphere/8)+1) * sizeof(cl_uchar)*global_size[0]*global_size[1]));
	gpu_pt.createGPUbuffer(&pt_triangleLoc, CL_MEM_READ_WRITE, 
		(((cnt_triangle/8)+1) * sizeof(cl_uchar)*global_size[0]*global_size[1]));*/

	// Octree things.
	gpu_pt.createGPUbuffer(&pt_octree, CL_MEM_READ_ONLY, cnt_octree*sizeof(TOctreeBox));
	err = gpu_pt.writeGPUdata(&pt_octree, 0, cnt_octree*sizeof(TOctreeBox), octreeBuffer.data());
	checkError(err);
	gpu_pt.createGPUbuffer(&pt_objectsOct, CL_MEM_READ_ONLY, objectBufferOct.size()*sizeof(TObject));
	err = gpu_pt.writeGPUdata(&pt_objectsOct, 0, objectBufferOct.size()*sizeof(TObject), objectBufferOct.data());
	checkError(err);
	// UniGrid things.
	gpu_pt.createGPUbuffer(&pt_uniGrid, CL_MEM_READ_ONLY, sizeof(TUniGrid));
	err = gpu_pt.writeGPUdata(&pt_uniGrid, 0, sizeof(TUniGrid), &infoUniGrid);
	checkError(err);
	gpu_pt.createGPUbuffer(&pt_uniGridBuffer, CL_MEM_READ_ONLY, uniGridBuffer.size()*sizeof(TBoxLink));
	err = gpu_pt.writeGPUdata(&pt_uniGridBuffer, 0, uniGridBuffer.size()*sizeof(TBoxLink), uniGridBuffer.data());
	checkError(err);
	gpu_pt.createGPUbuffer(&pt_objectsUniGrid, CL_MEM_READ_ONLY, objectBufferUniGrid.size()*sizeof(TObject));
	err = gpu_pt.writeGPUdata(&pt_objectsUniGrid, 0, objectBufferUniGrid.size()*sizeof(TObject), objectBufferUniGrid.data());
	checkError(err);
	// BVH things.
	gpu_pt.createGPUbuffer(&pt_BVH, CL_MEM_READ_ONLY, bvhBuffer.size()*sizeof(TBVHNode));
	err = gpu_pt.writeGPUdata(&pt_BVH, 0, bvhBuffer.size()*sizeof(TBVHNode), bvhBuffer.data());
	checkError(err);
	gpu_pt.createGPUbuffer(&pt_objectsBVH, CL_MEM_READ_ONLY, objectBufferBVH.size()*sizeof(TObject));
	err = gpu_pt.writeGPUdata(&pt_objectsBVH, 0, objectBufferBVH.size()*sizeof(TObject), objectBufferBVH.data());
	checkError(err);

	// Counters.
	gpu_pt.createGPUbuffer(&pt_cntSpheres, CL_MEM_READ_ONLY, sizeof(cl_uint));
	err = gpu_pt.writeGPUdata(&pt_cntSpheres, 0, sizeof(cl_uint), &cnt_sphere);
	checkError(err);
	gpu_pt.createGPUbuffer(&pt_cntTriangles, CL_MEM_READ_ONLY, sizeof(cl_uint));
	err = gpu_pt.writeGPUdata(&pt_cntTriangles, 0, sizeof(cl_uint), &cnt_triangle);
	checkError(err);
	gpu_pt.createGPUbuffer(&pt_cntRangeMeshes, CL_MEM_READ_ONLY, sizeof(cl_uint));
	err = gpu_pt.writeGPUdata(&pt_cntRangeMeshes, 0, sizeof(cl_uint), &cnt_range_mesh);
	checkError(err);
	gpu_pt.createGPUbuffer(&pt_cntLights, CL_MEM_READ_ONLY, sizeof(cl_uint));
	err = gpu_pt.writeGPUdata(&pt_cntLights, 0, sizeof(cl_uint), &cnt_light);
	checkError(err);
	gpu_pt.createGPUbuffer(&pt_cntOctree, CL_MEM_READ_ONLY, sizeof(cl_uint));
	err = gpu_pt.writeGPUdata(&pt_cntOctree, 0, sizeof(cl_uint), &cnt_octree);
	checkError(err);

	// Set arguments for kernel with no ADS.
	gpu_pt.changeKernel(AS_LIST);
	gpu_pt.setGPUargs(2, sizeof(cl_mem), &pt_cam);
	gpu_pt.setGPUargs(3, sizeof(cl_mem), &pt_sphere);
	gpu_pt.setGPUargs(4, 1, NULL);
	gpu_pt.setGPUargs(5, sizeof(cl_mem), &pt_cntSpheres);
	gpu_pt.setGPUargs(6, sizeof(cl_mem), &pt_triangle);
	gpu_pt.setGPUargs(7, 1, NULL);
	gpu_pt.setGPUargs(8, sizeof(cl_mem), &pt_cntTriangles);
	gpu_pt.setGPUargs(9, sizeof(cl_mem), &pt_meshes);
	gpu_pt.setGPUargs(10, sizeof(cl_mem), &pt_range_meshes);
	gpu_pt.setGPUargs(11, sizeof(cl_mem), &pt_cntRangeMeshes);
	gpu_pt.setGPUargs(12, sizeof(cl_mem), &pt_light);
	gpu_pt.setGPUargs(13, sizeof(cl_mem), &pt_cntLights);

	gpu_pt.changeKernel(AS_LIST_FIRST);
	gpu_pt.setGPUargs(1, sizeof(cl_mem), &pt_cam);
	gpu_pt.setGPUargs(2, sizeof(cl_mem), &pt_sphere);
	gpu_pt.setGPUargs(3, 1, NULL);
	gpu_pt.setGPUargs(4, sizeof(cl_mem), &pt_cntSpheres);
	gpu_pt.setGPUargs(5, sizeof(cl_mem), &pt_triangle);
	gpu_pt.setGPUargs(6, 1, NULL);
	gpu_pt.setGPUargs(7, sizeof(cl_mem), &pt_cntTriangles);
	gpu_pt.setGPUargs(8, sizeof(cl_mem), &pt_meshes);
	gpu_pt.setGPUargs(9, sizeof(cl_mem), &pt_range_meshes);
	gpu_pt.setGPUargs(10, sizeof(cl_mem), &pt_cntRangeMeshes);
	gpu_pt.setGPUargs(11, sizeof(cl_mem), &pt_light);
	gpu_pt.setGPUargs(12, sizeof(cl_mem), &pt_cntLights);
	
	// Set arguments for kernel with Octree.
	gpu_pt.changeKernel(AS_OCTREE);
	gpu_pt.setGPUargs(2, sizeof(cl_mem), &pt_cam);
	gpu_pt.setGPUargs(3, sizeof(cl_mem), &pt_sphere);
	//gpu_pt.setGPUargs(4, sizeof(cl_mem), &pt_sphereLoc);
	//gpu_pt.setGPUargs(4, (((cnt_sphere / 8) + 1) * sizeof(cl_uchar)* local_size[0] * local_size[1]), NULL);
	gpu_pt.setGPUargs(4, 1, NULL);
	gpu_pt.setGPUargs(5, sizeof(cl_mem), &pt_cntSpheres);
	gpu_pt.setGPUargs(6, sizeof(cl_mem), &pt_triangle);
	//gpu_pt.setGPUargs(7, sizeof(cl_mem), &pt_triangleLoc);
	//gpu_pt.setGPUargs(7, (((cnt_triangle / 8) + 1) * sizeof(cl_uchar)* local_size[0] * local_size[1]), NULL);
	gpu_pt.setGPUargs(7, 1, NULL);
	gpu_pt.setGPUargs(8, sizeof(cl_mem), &pt_cntTriangles);
	gpu_pt.setGPUargs(9, sizeof(cl_mem), &pt_meshes);
	gpu_pt.setGPUargs(10, sizeof(cl_mem), &pt_range_meshes);
	gpu_pt.setGPUargs(11, sizeof(cl_mem), &pt_cntRangeMeshes);
	gpu_pt.setGPUargs(12, sizeof(cl_mem), &pt_light);
	gpu_pt.setGPUargs(13, sizeof(cl_mem), &pt_cntLights);
	gpu_pt.setGPUargs(16, sizeof(cl_mem), &pt_octree);
	gpu_pt.setGPUargs(17, sizeof(cl_mem), &pt_cntOctree);
	gpu_pt.setGPUargs(18, sizeof(cl_mem), &pt_objectsOct);

	gpu_pt.changeKernel(AS_OCTREE_FIRST);
	gpu_pt.setGPUargs(1, sizeof(cl_mem), &pt_cam);
	gpu_pt.setGPUargs(2, sizeof(cl_mem), &pt_sphere);
	//gpu_pt.setGPUargs(3, sizeof(cl_mem), &pt_sphereLoc);
	//gpu_pt.setGPUargs(3, (((cnt_sphere / 8) + 1) * sizeof(cl_uchar)* local_size[0] * local_size[1]), NULL);
	gpu_pt.setGPUargs(3, 1, NULL);
	gpu_pt.setGPUargs(4, sizeof(cl_mem), &pt_cntSpheres);
	gpu_pt.setGPUargs(5, sizeof(cl_mem), &pt_triangle);
	//gpu_pt.setGPUargs(6, sizeof(cl_mem), &pt_triangleLoc);
	//gpu_pt.setGPUargs(6, (((cnt_triangle / 8) + 1) * sizeof(cl_uchar)* local_size[0] * local_size[1]), NULL);
	gpu_pt.setGPUargs(6, 1, NULL);
	gpu_pt.setGPUargs(7, sizeof(cl_mem), &pt_cntTriangles);
	gpu_pt.setGPUargs(8, sizeof(cl_mem), &pt_meshes);
	gpu_pt.setGPUargs(9, sizeof(cl_mem), &pt_range_meshes);
	gpu_pt.setGPUargs(10, sizeof(cl_mem), &pt_cntRangeMeshes);
	gpu_pt.setGPUargs(11, sizeof(cl_mem), &pt_light);
	gpu_pt.setGPUargs(12, sizeof(cl_mem), &pt_cntLights);
	gpu_pt.setGPUargs(14, sizeof(cl_mem), &pt_octree);
	gpu_pt.setGPUargs(15, sizeof(cl_mem), &pt_cntOctree);
	gpu_pt.setGPUargs(16, sizeof(cl_mem), &pt_objectsOct);
	
	// Set arguments for kernel with Uniform grid.
	gpu_pt.changeKernel(AS_UNIFORM_GRID);
	gpu_pt.setGPUargs(2, sizeof(cl_mem), &pt_cam);
	gpu_pt.setGPUargs(3, sizeof(cl_mem), &pt_sphere);
	//gpu_pt.setGPUargs(4, sizeof(cl_mem), &pt_sphereLoc);
	//gpu_pt.setGPUargs(4, (((cnt_sphere / 8) + 1) * sizeof(cl_uchar)* local_size[0] * local_size[1]), NULL);
	gpu_pt.setGPUargs(4, 1, NULL);
	gpu_pt.setGPUargs(5, sizeof(cl_mem), &pt_cntSpheres);
	gpu_pt.setGPUargs(6, sizeof(cl_mem), &pt_triangle);
	//gpu_pt.setGPUargs(7, sizeof(cl_mem), &pt_triangleLoc);
	//gpu_pt.setGPUargs(7, (((cnt_triangle / 8) + 1) * sizeof(cl_uchar)* local_size[0] * local_size[1]), NULL);
	gpu_pt.setGPUargs(7, 1, NULL);
	gpu_pt.setGPUargs(8, sizeof(cl_mem), &pt_cntTriangles);
	gpu_pt.setGPUargs(9, sizeof(cl_mem), &pt_meshes);
	gpu_pt.setGPUargs(10, sizeof(cl_mem), &pt_range_meshes);
	gpu_pt.setGPUargs(11, sizeof(cl_mem), &pt_cntRangeMeshes);
	gpu_pt.setGPUargs(12, sizeof(cl_mem), &pt_light);
	gpu_pt.setGPUargs(13, sizeof(cl_mem), &pt_cntLights);
	gpu_pt.setGPUargs(16, sizeof(cl_mem), &pt_uniGrid);
	gpu_pt.setGPUargs(17, sizeof(cl_mem), &pt_uniGridBuffer);
	gpu_pt.setGPUargs(18, sizeof(cl_mem), &pt_objectsUniGrid);

	gpu_pt.changeKernel(AS_UNIFORM_GRID_FIRST);
	gpu_pt.setGPUargs(1, sizeof(cl_mem), &pt_cam);
	gpu_pt.setGPUargs(2, sizeof(cl_mem), &pt_sphere);
	//gpu_pt.setGPUargs(3, sizeof(cl_mem), &pt_sphereLoc);
	//gpu_pt.setGPUargs(3, (((cnt_sphere / 8) + 1) * sizeof(cl_uchar)* local_size[0] * local_size[1]), NULL);
	gpu_pt.setGPUargs(3, 1, NULL);
	gpu_pt.setGPUargs(4, sizeof(cl_mem), &pt_cntSpheres);
	gpu_pt.setGPUargs(5, sizeof(cl_mem), &pt_triangle);
	//gpu_pt.setGPUargs(6, sizeof(cl_mem), &pt_triangleLoc);
	//gpu_pt.setGPUargs(6, (((cnt_triangle / 8) + 1) * sizeof(cl_uchar)* local_size[0] * local_size[1]), NULL);
	gpu_pt.setGPUargs(6, 1, NULL);
	gpu_pt.setGPUargs(7, sizeof(cl_mem), &pt_cntTriangles);
	gpu_pt.setGPUargs(8, sizeof(cl_mem), &pt_meshes);
	gpu_pt.setGPUargs(9, sizeof(cl_mem), &pt_range_meshes);
	gpu_pt.setGPUargs(10, sizeof(cl_mem), &pt_cntRangeMeshes);
	gpu_pt.setGPUargs(11, sizeof(cl_mem), &pt_light);
	gpu_pt.setGPUargs(12, sizeof(cl_mem), &pt_cntLights);
	gpu_pt.setGPUargs(14, sizeof(cl_mem), &pt_uniGrid);
	gpu_pt.setGPUargs(15, sizeof(cl_mem), &pt_uniGridBuffer);
	gpu_pt.setGPUargs(16, sizeof(cl_mem), &pt_objectsUniGrid);
	
	// Set arguments for kernel with BVH.
	gpu_pt.changeKernel(AS_BVH);
	gpu_pt.setGPUargs(2, sizeof(cl_mem), &pt_cam);
	gpu_pt.setGPUargs(3, sizeof(cl_mem), &pt_sphere);
	gpu_pt.setGPUargs(4, 1, NULL);
	gpu_pt.setGPUargs(5, sizeof(cl_mem), &pt_cntSpheres);
	gpu_pt.setGPUargs(6, sizeof(cl_mem), &pt_triangle);
	gpu_pt.setGPUargs(7, 1, NULL);
	gpu_pt.setGPUargs(8, sizeof(cl_mem), &pt_cntTriangles);
	gpu_pt.setGPUargs(9, sizeof(cl_mem), &pt_meshes);
	gpu_pt.setGPUargs(10, sizeof(cl_mem), &pt_range_meshes);
	gpu_pt.setGPUargs(11, sizeof(cl_mem), &pt_cntRangeMeshes);
	gpu_pt.setGPUargs(12, sizeof(cl_mem), &pt_light);
	gpu_pt.setGPUargs(13, sizeof(cl_mem), &pt_cntLights);
	gpu_pt.setGPUargs(16, sizeof(cl_mem), &pt_BVH);
	gpu_pt.setGPUargs(17, sizeof(cl_mem), &pt_objectsBVH);

	gpu_pt.changeKernel(AS_BVH_FIRST);
	gpu_pt.setGPUargs(1, sizeof(cl_mem), &pt_cam);
	gpu_pt.setGPUargs(2, sizeof(cl_mem), &pt_sphere);
	gpu_pt.setGPUargs(3, 1, NULL);
	gpu_pt.setGPUargs(4, sizeof(cl_mem), &pt_cntSpheres);
	gpu_pt.setGPUargs(5, sizeof(cl_mem), &pt_triangle);
	gpu_pt.setGPUargs(6, 1, NULL);
	gpu_pt.setGPUargs(7, sizeof(cl_mem), &pt_cntTriangles);
	gpu_pt.setGPUargs(8, sizeof(cl_mem), &pt_meshes);
	gpu_pt.setGPUargs(9, sizeof(cl_mem), &pt_range_meshes);
	gpu_pt.setGPUargs(10, sizeof(cl_mem), &pt_cntRangeMeshes);
	gpu_pt.setGPUargs(11, sizeof(cl_mem), &pt_light);
	gpu_pt.setGPUargs(12, sizeof(cl_mem), &pt_cntLights);
	gpu_pt.setGPUargs(14, sizeof(cl_mem), &pt_BVH);
	gpu_pt.setGPUargs(15, sizeof(cl_mem), &pt_objectsBVH);

	gpu_pt.changeKernel(AS_LIST);
}

void RenderEnginePT::ComputePT()
{
	int error = 0;
	Color c;
	cl_uint seed_gpu;
	long seed_cpu = 11111;
	PathTracer rt1(sceneList, output);
	Uint32 timer = SDL_GetTicks();
	GLfloat* pixels;
	bool isNeedUpdate = false;

	// Get pixels of texture.
	pixels = contextAPI->GetActualImage();

	// If pause button pressed?
	// Check it out!
	contextAPI->SeizeSem(1);
	contextAPI->LeaveSem(1);

	if (contextAPI->isQuit()) {
		return;
	}

	CheckSettingsFirst(&rt1, &gpu_pt, contextAPI->GetActiveAS(), contextAPI->GetActiveRenderer());
	
	if (contextAPI->GetActiveRenderer() == CPU_RENDER) {
		// Compute first image on CPU.
		timer = SDL_GetTicks();
		rt1.computeFirstImage(contextAPI, pixels);
		contextAPI->SnapTime(timer);
		contextAPI->UpdateActualResult();
	}
	else {
		seed_gpu = (cl_uint)(ran1(&seed_cpu) * CL_UINT_MAX);
		gpu_pt.setGPUargs(contextAPI->GetResultIndex(), sizeof(cl_mem), &pt_col[0]);
		gpu_pt.setGPUargs(13, sizeof(cl_uint), &seed_gpu);
		// Start OpenCL program.
		timer = SDL_GetTicks();
		error = gpu_pt.startGPUprogram(pt_col, global_size, local_size);
		contextAPI->SnapTime(timer);
		checkError(error);
		contextAPI->ReloadTexturePixels();
	}
	
	while (1) {
		contextAPI->AddSample();
		// If pause button pressed?
		// Check it out!
		contextAPI->SeizeSem(1);
		contextAPI->LeaveSem(1);

		contextAPI->SnapTime(timer);

		if (contextAPI->isQuit()) {
			break;
		}

		isNeedUpdate = CheckSettings(&rt1, &gpu_pt, contextAPI->GetActiveAS(), 
									 contextAPI->GetActiveRenderer());

		if (usedRenderer == CPU_RENDER) {
			// Get pixels of texture.
			if (isNeedUpdate) {
				contextAPI->SeizeSem(0);
				contextAPI->LeaveSem(0);
			}
			timer = SDL_GetTicks();
			// Compute image on CPU.
			rt1.computeImage(contextAPI, pixels);
			contextAPI->SnapTime(timer);
			// Set flag that pixels was updated.
			contextAPI->UpdateActualResult();
			contextAPI->SeizeSem(2);
		}
		else {
			contextAPI->SeizeSem(2);
			contextAPI->LeaveSem(2);

			seed_gpu = (cl_uint)(ran1(&seed_cpu) * CL_UINT_MAX);
			SwapImages(&gpu_pt, contextAPI->GetResultIndex());

			gpu_pt.setGPUargs(14, sizeof(cl_uint), &seed_gpu);
			gpu_pt.setGPUargs(15, sizeof(cl_uint), contextAPI->GetSamplesPtr());

			// Start OpenCL program.
			if (contextAPI->isQuit()) {
				break;
			}
			timer = SDL_GetTicks();
			error = gpu_pt.startGPUprogram(pt_col, global_size, local_size);
			contextAPI->SnapTime(timer);
			if (contextAPI->isQuit()) {
				break;
			}
			checkError(error);
			// Set flag that pixels was updated.
			contextAPI->Swap();
			contextAPI->ReloadTexturePixels();
		}
	}
}

// Swap GL textures for read and write.
void RenderEnginePT::SwapImages(GPUPathtracer* gpu_pt, unsigned int indexImg)
{
	gpu_pt->setGPUargs(indexImg, sizeof(cl_mem), &pt_col[0]);
	gpu_pt->setGPUargs(indexImg?0:1, sizeof(cl_mem), &pt_col[1]);
}

void RenderEnginePT::Destroy() 
{
	//clReleaseKernel(pt_kernel);
	clReleaseMemObject(pt_col[0]);
	clReleaseMemObject(pt_col[1]);
	clReleaseMemObject(pt_cam);
	clReleaseMemObject(pt_sphere);
	clReleaseMemObject(pt_triangle);
	clReleaseMemObject(pt_meshes);
	clReleaseMemObject(pt_range_meshes);
	clReleaseMemObject(pt_light);
}

#define IA 16807
#define IM 2147483647
#define AM (1.0/IM)
#define IQ 127773
#define IR 2836
#define NTAB 32
#define NDIV (1+(IM-1)/NTAB)
#define EPS 1.2e-7
#define RNMX (1.0-EPS)
float ran1(long *idum)
/*“Minimal” random number generator of Park and Miller with Bays - Durham shuffle and added
safeguards.Returns a uniform random deviate between 0.0 and 1.0 (exclusive of the endpoint
values).Call with idum a negative integer to initialize; thereafter, do not alter idum between
successive deviates in a sequence.RNMX should approximate the largest floating value that is
less than 1.*/
{
	int j;
	long k;
	static long iy = 0;
	static long iv[NTAB];
	float temp;
	if (*idum <= 0 || !iy) {
		// Initialize.
		if (-(*idum) < 1) *idum = 1; // Be sure to prevent idum = 0.
		else *idum = -(*idum);
		for (j = NTAB + 7; j >= 0; j--) {
			// Load the shuffle table(after 8 warm - ups).
			k = (*idum) / IQ;
			*idum = IA*(*idum - k*IQ) - IR*k;
			if (*idum < 0) *idum += IM;
			if (j < NTAB) iv[j] = *idum;
		}
		iy = iv[0];
	}

	k = (*idum) / IQ; // Start here when not initializing.
	*idum = IA*(*idum - k*IQ) - IR*k; // Compute idum = (IA*idum) % IM without overif
	if (*idum < 0) *idum += IM; // flows by Schrage’s method.
	j = iy / NDIV; // Will be in the range 0..NTAB - 1.
	iy = iv[j]; // Output previously stored value and refill the
	iv[j] = *idum; // shuffle table.
	if ((temp = (float)(AM*iy)) > RNMX) return (float)RNMX; // Because users don’t expect endpoint values.
	else return temp;
}

/**
* Changes the current working directory.
*/
static bool changeDirectory(const char* directory)
{
#ifdef WIN32
	return SetCurrentDirectoryA(directory) == TRUE;
#else
	return chdir(directory) == 0;
#endif
}

/**
* Goes up the directory hierarchy until a folder named "data" is found.
*/
static void changeToRootDirectory(string directory)
{
	for (int i = 0; i < 64; i++) {
		bool found = changeDirectory(directory.c_str());

		if (!changeDirectory(".."))
			break;

		if (found)
			return;
	}

	throw runtime_error("unable to find data directory");
}

void checkError(int err) {
	if (err) {
		system("pause");
		exit(err);
	}
}

bool RenderEnginePT::CheckSettings(PathTracer* pt, GPUPathtracer* gpu_pt, 
									int pressedAS, int pressedRenderer) 
{
	bool wasChangedRender = false;
	if (usedRenderer != pressedRenderer)
		wasChangedRender = true;
	usedRenderer = pressedRenderer;

	if (usedAS != pressedAS) {
		switch (pressedAS) {
		case AS_BVH:
			cout << "Accelerate structure change to BVH.\n";
			if (usedRenderer == GPU_RENDER) {
				gpu_pt->changeKernel(AS_BVH);
			}
			else {
				pt->setScene(sceneBVH);
			}
			break;
		case AS_UNIFORM_GRID:
			cout << "Accelerate structure change to Uniform grid.\n";
			if (usedRenderer == GPU_RENDER) {
				gpu_pt->changeKernel(AS_UNIFORM_GRID);
			}
			else {
				pt->setScene(sceneUniGrid);
			}
			break;
		case AS_OCTREE:
			cout << "Accelerate structure change to Octree.\n";
			if (usedRenderer == GPU_RENDER) {
				gpu_pt->changeKernel(AS_OCTREE);
			}
			else {
				pt->setScene(sceneOctree);
			}
			break;
		case AS_LIST:
			cout << "Accelerate structure change to List.\n";
			if (usedRenderer == GPU_RENDER) {
				gpu_pt->changeKernel(AS_LIST);
			}
			else {
				pt->setScene(sceneList);
			}
			break;
		}
		usedAS = pressedAS;
	}
	
	return wasChangedRender;
}

void RenderEnginePT::CheckSettingsFirst(PathTracer* pt, GPUPathtracer* gpu_pt,
	int pressedAS, int pressedRenderer)
{
	switch (pressedAS) {
	case AS_BVH:
		if (pressedRenderer == GPU_RENDER) {
			gpu_pt->changeKernel(AS_BVH_FIRST);
		}
		else {
			pt->setScene(sceneBVH);
		}
		break;
	case AS_UNIFORM_GRID:
		if (pressedRenderer == GPU_RENDER) {
			gpu_pt->changeKernel(AS_UNIFORM_GRID_FIRST);
		}
		else {
			pt->setScene(sceneUniGrid);
		}
		break;
	case AS_OCTREE:
		if (pressedRenderer == GPU_RENDER) {
			gpu_pt->changeKernel(AS_OCTREE_FIRST);
		}
		else {
			pt->setScene(sceneOctree);
		}
		break;
	case AS_LIST:
		if (pressedRenderer == GPU_RENDER) {
			gpu_pt->changeKernel(AS_LIST_FIRST);
		}
		else {
			pt->setScene(sceneList);
		}
		break;
	}
}

unsigned int RenderEnginePT::CreateOctree(std::vector<TOctreeBox>* octreeBuffer, std::vector<TObject>* objectBuffer,
	OctreeAccelerator* octADS, unsigned int& indexOctreeBuffer)
{
	unsigned int objBufferStart = objectBuffer->size();
	unsigned int objBufferCnt = objBufferStart;
	unsigned int index = octreeBuffer->size();

	AABB box;
	box = octADS->getBox();

	TObject obj;
	TOctreeBox node;

	Point3DtoFloat3(box.mMin, node.boxMin);
	Point3DtoFloat3(box.mMax, node.boxMax);
	Point3DtoFloat3(octADS->getMiddlePoint(), node.middlePoint);
	node.leaf = octADS->isLeaf();
	
	if (node.leaf) {
		std::vector<Intersectable*> objects = octADS->getObjects();
		for (std::vector<Intersectable*>::iterator it = objects.begin(); it != objects.end(); it++) {
			if ((*it)->isSphere()) {
				obj.index = GetIndexSphere((Sphere*)*it);
				obj.type = SPHERE_INDEX;
			}
			else {
				obj.index = GetIndexTriangle((Triangle*)*it);
				obj.type = TRIANGLE_INDEX;
			}
			objectBuffer->push_back(obj);
			objBufferCnt++;
		}

		node.boxLink.objStartIndex = objBufferStart;
		node.boxLink.objSize = objBufferCnt;

		for (int i = 0; i < 8; i++) {
			node.children[i] = 0;
		}
		octreeBuffer->push_back(node);
	}
	else {
		/*for (int i = 0; i < 8; i++) {
			node.children[i] = indexOctreeBuffer;
			indexOctreeBuffer++;
		}*/

		octreeBuffer->push_back(node);

		for (int i = 0; i < 8; i++) {
			node.children[i] = CreateOctree(octreeBuffer, objectBuffer, octADS->getChild(i), indexOctreeBuffer);
		}
		octreeBuffer->at(index) = node;
	}
	return index;
}

unsigned int RenderEnginePT::GetIndexSphere(Sphere* sp)
{
	unsigned int i = 0; 
	for (std::vector<TSphere>::iterator it = spheres.begin(); it != spheres.end(); it++) {
		if (sp->getIndex() == (*it).index) {
			return i;
		}
		i++;
	}
	return 100000;
}

unsigned int RenderEnginePT::GetIndexTriangle(Triangle* tr)
{
	unsigned int i = 0;
	for (std::vector<TTriangle>::iterator it = triangles.begin(); it != triangles.end(); it++) {
		if (tr->getIndex() == (*it).index) {
			return i;
		}
		i++;
	}
	return 100000;
}

void RenderEnginePT::CreateUniGrid(TUniGrid* infoUniGrid, std::vector<TBoxLink>* uniGridBuffer,
						    std::vector<TObject>* objBufferUniGrid, UniformAccelerator* uniADS)
{
	AABB box;
	box = uniADS->getBox();
	std::vector<Intersectable*> objects = uniADS->getObjects();
	Intersectable* nodeObj;

	Point3DtoFloat3(box.mMin, infoUniGrid->boxMin);
	Point3DtoFloat3(box.mMax, infoUniGrid->boxMax);
	Point3DtoFloat3(uniADS->getWorldSize(), infoUniGrid->wotld_size);
	Point3DtoFloat3(uniADS->getCellsize(), infoUniGrid->cell_size);
	infoUniGrid->grid_size = GRID_SIZE;

	UniNode** nodeList = uniADS->getVoxels();
	UniNode* node;
	TBoxLink nodeRange;
	TObject obj;
	unsigned int objBufferStart = 0;
	unsigned int objBufferCnt = 0;

	unsigned int max_size = GRID_SIZE*GRID_SIZE*GRID_SIZE;

	for (unsigned int x = 0; x < max_size; x++) {
		node = nodeList[x];
		while (node != NULL) {
			nodeObj = objects[node->getIndex()];
			if (nodeObj->isSphere()) {
				obj.index = GetIndexSphere((Sphere*)nodeObj);
				obj.type = SPHERE_INDEX;
			}
			else {
				obj.index = GetIndexTriangle((Triangle*)nodeObj);
				obj.type = TRIANGLE_INDEX;
			}
			objBufferUniGrid->push_back(obj);
			objBufferCnt++;
			node = node->getNext();
		}
		nodeRange.objStartIndex = objBufferStart;
		nodeRange.objSize = objBufferCnt;
		uniGridBuffer->push_back(nodeRange);
		objBufferStart = objBufferCnt;
	}
}

void RenderEnginePT::CreateBVH(std::vector<TBVHNode>* nodes, std::vector<TObject>* objBufferBVH, BVHAccelerator* bvhADS)
{
	std::vector<Intersectable*> objects = bvhADS->getObjects();
	TBVHNode node;
	unsigned int leftID, rightID;

	TObject obj;
	Intersectable* nodeObj;
	unsigned int objBufferStart = 0;
	unsigned int objBufferCnt = 0;
	unsigned int index = 0;

	unsigned int maxSize = bvhADS->getNodesCnt();
	for (unsigned int i = 0; i < maxSize; i++) {
		bvhADS->getNodes(node, index, leftID, rightID);
		if (node.leaf) {
			for (unsigned int j = leftID; j < (leftID + rightID); j++) {
				nodeObj = objects[j];
				if (nodeObj->isSphere()) {
					obj.index = GetIndexSphere((Sphere*)nodeObj);
					obj.type = SPHERE_INDEX;
				}
				else {
					obj.index = GetIndexTriangle((Triangle*)nodeObj);
					obj.type = TRIANGLE_INDEX;
				}
				objBufferBVH->push_back(obj);
				objBufferCnt++;
			}
		}
		node.indexObj.objStartIndex = objBufferStart;
		node.indexObj.objSize = objBufferCnt;
		nodes->push_back(node);
		index++;
		objBufferStart = objBufferCnt;
	}
}