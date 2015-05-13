#ifndef _GPU_PATHTRACER_H_
#define _GPU_PATHTRACER_H_

#include <stdio.h>
#include <assert.h>
#include <new>
#include <vector>
#include "cl_gl.h"
#include "SDLGLContext.h"
#include "SDL_opengl.h"
#include "../include/cl.h"
#include "../include/cl_platform.h"
#include "gpu_types.h"

#define MAX_DEVICES 10

// Simple API for better control.
class GPUPathtracer {
public:
	// Constructors and destructors.
	GPUPathtracer();
	~GPUPathtracer();

	// Initialize important variables.
	int initGPU();
	// Load OpenCL program from file.
	void loadGPUprogram(cl_program* program);
	// Change kernel to computation.
	void changeKernel(unsigned int kernel_type);
	// Set argument of kernel.
	void setGPUargs(unsigned int index, size_t size, void* data);
	// Create OpenCL program from string.
	int createGPUprogram(cl_program* program, char* file_name);
	// Create kernel of loaded OpenCL program.
	int loadGPUkernel(unsigned int kernel_type, char* kernel_name);
	// Create OpenCL buffer.
	int createGPUbuffer(cl_mem *buffer, cl_mem_flags flag, size_t size);
	// Create OpenCL 2D image.
	int createGPUimage2D(cl_mem *image, cl_GLuint* textures, cl_mem_flags flag, size_t width, size_t height);
	// Start loaded kernel.
	int startGPUprogram(cl_mem* imagesGL, size_t* global_size, size_t* local_size);
	// Read data from buffer.
	int readGPUbuffer(cl_mem* buffer, size_t offset, size_t size, void* data, int type);
	// Read data from image.
	int readGPUimage(cl_mem* image, size_t off_x, size_t off_y, size_t size, void* data);
	// Write data to input buffer.
	int writeGPUdata(cl_mem* buffer, size_t offset, size_t size, void* data);
	// Write objects to buffers.
	int writeGPUobjects(
		std::vector<TSphere>& sp, cl_mem& pt_sphere,
		std::vector<TTriangle>& tr, cl_mem& pt_triangle,
		std::vector<TMesh>& me, cl_mem& pt_meshes,
		std::vector<unsigned int>& ra_me, cl_mem& pt_range_meshes,
		cl_mem_flags flag = CL_MEM_READ_ONLY);
	// Write lights to buffer.
	int writeGPUlights(std::vector<TLight>& li, cl_mem& pt_light, 
							cl_mem_flags flag = CL_MEM_READ_ONLY);
	// Wait for complete transfer data.
	void flush() { clFlush(h_cmd_queue); }
	// Complete all operations on OpenCL device.
	void finish() { clFinish(h_cmd_queue); }

private:
	cl_uint n_count; // Count of all platforms.
	cl_platform_id h_platform; // Id of choosen platform.
	cl_device_id h_device; // Id of choosen device.
	cl_device_id h_devices[MAX_DEVICES]; // Id of choosen device.
	unsigned int cl_device_cnt; // Number of max devices.

	cl_context h_context; // Context of used device.

	cl_command_queue h_cmd_queue; // Queue of used context.
	cl_program* h_program; // Loaded program.

	cl_build_status n_build_status; // Is important???
	cl_kernel* actualKernel;
	cl_kernel* kernelList; // Compute pt with no accelerate data structures.
	cl_kernel* kernelListFirst; // Only for first compute ...
	cl_kernel* kernelOctree; // Compute pt with Octree structures.
	cl_kernel* kernelOctreeFirst; // Only for first compute ...
	cl_kernel* kernelUniGrid; // Compute pt with Uniform grid structures.
	cl_kernel* kernelUniGridFirst; // Only for first compute ...
	cl_kernel* kernelBVH; // Compute pt with BVH structures.
	cl_kernel* kernelBVHFirst; // Only for first compute ...

	// Choose device for compute.
	cl_device_id ChooseDevice(cl_device_id *h_device, cl_uint size);
	// Read file.
	char *p_s_ReadFile(const char *p_s_filename);
	// Print all errors and warnings.
	bool Show_CompilerOutput(cl_program h_program, cl_device_id h_device);
	// Test memory on right allocation.
	bool testAllocMemory(void* pointer);
};

#endif // _GPU_PATHTRACER_H_