// Start of gpu_pathtracer.cpp

#include "gpu_pathtracer.h"

// Constructor. Is need call initGPU().
GPUPathtracer::GPUPathtracer() 
{
	h_program = NULL;
	kernelList = NULL;
	kernelOctree = NULL;
	kernelUniGrid = NULL;
	kernelBVH = NULL;
	cl_device_cnt = MAX_DEVICES;
}

// Destructor. Free all OpenCL variables.
GPUPathtracer::~GPUPathtracer() 
{
	int n_result;
	//n_result = clReleaseKernel(h_hello_world);
	
	n_result = clReleaseCommandQueue(h_cmd_queue);
	n_result = clReleaseContext(h_context);

	if (h_program != NULL) 
		n_result = clReleaseProgram(*h_program);
}

// Initialize important OpenCL variables.
// @return -> returns error code
int GPUPathtracer::initGPU()
{
	// Get all platforms.
	int n_result = clGetPlatformIDs(1, &h_platform, &n_count);
	if (n_result != CL_SUCCESS) {
		fprintf(stderr, "OpenCL error: clGetPlatformIDs() failed\n");
		return -1;
	}

	// Set options for context.
	cl_context_properties p_props[] = { 
		CL_CONTEXT_PLATFORM, (cl_context_properties)h_platform, 
		CL_GL_CONTEXT_KHR, (cl_context_properties)wglGetCurrentContext(),
		CL_WGL_HDC_KHR, (cl_context_properties)wglGetCurrentDC(),
		0, 0 };
	/*
	cl_device_id* cdDevices;
	size_t uiDevCount;
	// Get the number of GPU devices available to the platform
	size_t errNum = clGetDeviceIDs(h_platform, CL_DEVICE_TYPE_GPU, 0, NULL, &uiDevCount);

	// Create the device list
	cdDevices = new cl_device_id[uiDevCount];
	errNum = clGetDeviceIDs(h_platform, CL_DEVICE_TYPE_GPU, uiDevCount, cdDevices, NULL);

	char buffer[100];
	clGetDeviceInfo(cdDevices[0], CL_DEVICE_NAME, 100, buffer, &errNum);
	std::cout << ">> " << buffer << std::endl;
	clGetDeviceInfo(cdDevices[0], CL_DEVICE_VENDOR, 100, buffer, &errNum);
	std::cout << ">> " << buffer << std::endl;
	clGetDeviceInfo(cdDevices[0], CL_DEVICE_VERSION, 100, buffer, &errNum);
	std::cout << ">> " << buffer << std::endl;
	clGetDeviceInfo(cdDevices[0], CL_DRIVER_VERSION, 100, buffer, &errNum);
	std::cout << ">> " << buffer << std::endl;
	std::cout << "--------------------\n";
	*/

	clGetGLContextInfoKHR_fn clGetGLContextInfoKHR =
		(clGetGLContextInfoKHR_fn)clGetExtensionFunctionAddressForPlatform(h_platform, "clGetGLContextInfoKHR");
	if (!clGetGLContextInfoKHR) {
		fprintf(stderr, "error: clGetGLContextInfoKHR() not supported\n");
		return -2;
	}
	n_result = clGetGLContextInfoKHR(p_props, CL_DEVICES_FOR_GL_CONTEXT_KHR,
		cl_device_cnt * sizeof(cl_device_id), &h_devices, &n_count);
	n_count /= sizeof(cl_device_id);
	if (!n_count) {
		fprintf(stderr, "error: no CL_DEVICES_FOR_GL_CONTEXT_KHR device found\n");
		return -3;
	}

	h_device = ChooseDevice(h_devices, n_count);
	// Create context for concrete device.
	h_context = clCreateContext(p_props, 1, &h_device, 0, 0, &n_result);
	if (n_result != CL_SUCCESS) {
		fprintf(stderr, "OpenCL error: clCreateContext() failed\n");
		return -4;
	}

	// Create command queue for concrete device.
	h_cmd_queue = clCreateCommandQueue(h_context, h_device, 
		CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE, &n_result);
	if (n_result != CL_SUCCESS) {
		fprintf(stderr, "OpenCL error: clCreateCommandQueue() failed\n");
		return -5;
	}

	return 0;
}

#define BUFFER_SIZE 100

// User have to choose device for OpenCL PT.
// h_device -> list of available devices
// size -> size of list
// @return -> choosen device
cl_device_id GPUPathtracer::ChooseDevice(cl_device_id *h_device, cl_uint size)
{
	size_t result_cnt = 0;
	size_t size_buffer = BUFFER_SIZE;
	char buffer[BUFFER_SIZE];

	std::cout << "## Devices support OpenCL ##\n\n";

	for (unsigned int i = 0; i < size; i++) {
		std::cout << ">> " << i + 1 << "\n";
		clGetDeviceInfo(h_device[i], CL_DEVICE_NAME, BUFFER_SIZE, buffer, &result_cnt);
		std::cout << ">> " << buffer << std::endl;
		clGetDeviceInfo(h_device[i], CL_DEVICE_VENDOR, BUFFER_SIZE, buffer, &result_cnt);
		std::cout << ">> " << buffer << std::endl;
		clGetDeviceInfo(h_device[i], CL_DEVICE_VERSION, BUFFER_SIZE, buffer, &result_cnt);
		std::cout << ">> " << buffer << std::endl;
		clGetDeviceInfo(h_device[i], CL_DRIVER_VERSION, BUFFER_SIZE, buffer, &result_cnt);
		std::cout << ">> " << buffer << std::endl;
		std::cout << "--------------------\n";
	}

	std::cout << "\nChoose device from list (number): ";

	std::string temp;
	getline(std::cin, temp);
	unsigned int select_number = std::atoi(temp.c_str());

	while ((select_number == 0) || ((select_number-1) >= size)) {
		std::cout << "Wrong option.\nPlease write number from upper list: ";
		getline(std::cin, temp);
		select_number = std::atoi(temp.c_str());
	}

	return h_device[select_number-1];
}

// Set program as actual used program.
// program -> OpenCL program
void GPUPathtracer::loadGPUprogram(cl_program* program) 
{ 
	h_program = program; 
}

//
void GPUPathtracer::changeKernel(unsigned int kernel_type)
{
	switch (kernel_type) {
	case AS_LIST:
		actualKernel = kernelList;
		break;
	case AS_OCTREE:
		actualKernel = kernelOctree;
		break;
	case AS_UNIFORM_GRID:
		actualKernel = kernelUniGrid;
		break;
	case AS_BVH:
		actualKernel = kernelBVH;
		break;
	case AS_LIST_FIRST:
		actualKernel = kernelListFirst;
		break;
	case AS_OCTREE_FIRST:
		actualKernel = kernelOctreeFirst;
		break;
	case AS_UNIFORM_GRID_FIRST:
		actualKernel = kernelUniGridFirst;
		break;
	case AS_BVH_FIRST:
		actualKernel = kernelBVHFirst;
		break;
	}
}

// Set argument of default kernel.
// index -> index of set argument.
// size -> size of data type.
// data -> input/output data.
void GPUPathtracer::setGPUargs(unsigned int index, size_t size, void* data) 
{ 
	clSetKernelArg(*actualKernel, index, size, data);
}

// Create OpenCL program from file.
// program -> OpenCL program
// file_name -> name of OpenCL file
// @return -> returns error code
int GPUPathtracer::createGPUprogram(cl_program* program, char* file_name) 
{
	// Set file name?
	if (file_name == NULL) {
		fprintf(stderr, "createGPUprogram error: file name is not set.\n");
		return -1; 
	}

	// Is var allocated?
	if (program == NULL) {
		program = new cl_program();
		// Test right allocate.
		if (!testAllocMemory(program)) {
			fprintf(stderr, "createGPUprogram error: cannot allocate memory.\n");
			return -2;
		}
	}
	
	// Error variable.
	int n_result;
	// Buffer for read file.
	char *p_s_program;

	// Read file contains OpenCL program.
	if (!(p_s_program = p_s_ReadFile(file_name))) {
		fprintf(stderr, "OpenCL error: p_s_ReadFile(\"%s\") failed\n", file_name);
		return -3;
	}

	// Create OpenCL program from readed file.
	*program = clCreateProgramWithSource(h_context, 1,
		(const char**)&p_s_program, 0, &n_result);
	if (n_result != CL_SUCCESS) {
		fprintf(stderr, "OpenCL error: clCreateProgramWithSource() failed\n");
		delete[] p_s_program;
		return -4;
	}

	// Build OpenCL program.
	n_result = clBuildProgram(*program, 1,
	&h_device, "-I ./kernels/", 0, 0); // #define the UNUSED decorator
	if (n_result != CL_SUCCESS) {
		fprintf(stderr, "OpenCL error: clBuildProgram() failed\n");
		delete[] p_s_program;
		// Print error messages.
		Show_CompilerOutput(*program, h_device);
		return -5;
	}

	// No more used variable.
	delete[] p_s_program;

	cl_build_status n_build_status;
	// Check until build was complete successfully.
	for (;;) {
		size_t n_size;
		n_result = clGetProgramBuildInfo(*program, h_device,
			CL_PROGRAM_BUILD_STATUS, sizeof(cl_build_status), &n_build_status, &n_size);
		if (n_result != CL_SUCCESS) {
			fprintf(stderr, "OpenCL error: clGetProgramBuildInfo() failed\n");
			return -6;
		}
		assert(n_size == sizeof(cl_build_status));
		if (n_build_status == CL_BUILD_IN_PROGRESS) {
			//sleep(1); // howgh
			continue;
		}
		else if (n_build_status != CL_BUILD_SUCCESS) {
			fprintf(stderr, "OpenCL error: n_build_status != CL_BUILD_SUCCESS\n");
			Show_CompilerOutput(*h_program, h_device);
			return -7;
		}
		else
			return 0;
	}
}

// Create kernel of loaded OpenCL program.
// kernel_name -> name of searched kernel
// kernel -> pointer to kernel
// @return -> returns flag of successfully
int GPUPathtracer::loadGPUkernel(unsigned int kernel_type, char* kernel_name)
{
	cl_kernel* kernel = NULL;

	// Is name of kernel set?
	if (kernel_name == NULL) {
		fprintf(stderr, "loadGPUkernel error: name of kernel not set.\n");
		return -1;
	}
	
	// Is kernel allocated?
	if (kernel == NULL) {
		kernel = new cl_kernel();
		// Test right allocation.
		if (!testAllocMemory(kernel)) {
			fprintf(stderr, "loadGPUkernel error: cannot allocate memory.\n");
			return -2;
		}
	}

	// Error code variable.
	int n_result;

	// Search and create kernel in source file.
	*kernel = clCreateKernel(*h_program, kernel_name, &n_result);
	if (n_result != CL_SUCCESS) {
		fprintf(stderr, "OpenCL error: clCreateKernel cannot create kernel %s.\n", kernel_name);
		return -3;
	}

	switch (kernel_type) {
	case AS_LIST:
		kernelList = kernel;
		return 0;
	case AS_OCTREE:
		kernelOctree = kernel;
		return 0;
	case AS_UNIFORM_GRID:
		kernelUniGrid = kernel;
		return 0;
	case AS_BVH:
		kernelBVH = kernel;
		return 0;
	case AS_LIST_FIRST:
		kernelListFirst = kernel;
		return 0;
	case AS_OCTREE_FIRST:
		kernelOctreeFirst = kernel;
		return 0;
	case AS_UNIFORM_GRID_FIRST:
		kernelUniGridFirst = kernel;
		return 0;
	case AS_BVH_FIRST:
		kernelBVHFirst = kernel;
		return 0;
	}

	fprintf(stderr, "loadGPUkernel error: Cannot load kernel. Wrong type of accelerate data structure?\n");
	return 1;
}

// Create buffer alocated on GPU.
// buffer -> created buffer
// flag -> option of creation
// size -> size of created buffer
// @return -> returns flag of successfully
int GPUPathtracer::createGPUbuffer(cl_mem *buffer, cl_mem_flags flag, size_t size) 
{
	// Is buffer allocated?
	if (buffer == NULL) {
		buffer = new cl_mem();
		// Test right allocation.
		if (!testAllocMemory(buffer)) {
			fprintf(stderr, "createGPUkernel error: cannot allocate memory.\n");
			return -1;
		}
	}

	// Error code variable.
	int n_result;

	// Allocate memory on OpenCL device.
	*buffer = clCreateBuffer(h_context,
		flag, size * sizeof(char), 0, &n_result);
	if (n_result != CL_SUCCESS) {
		fprintf(stderr, "OpenCL error: clCreateBuffer() cannot create buffer.\n");
		return -2;
	}

	return 0;
}

//
int GPUPathtracer::createGPUimage2D(cl_mem *image, cl_GLuint* textures, 
									cl_mem_flags flag, size_t width, size_t height) {

	cl_image_format format = {CL_RGBA, CL_FLOAT};

	// Is buffer allocated?
	if (image == NULL) {
		image = new cl_mem();
		// Test right allocation.
		if (!testAllocMemory(image)) {
			fprintf(stderr, "createGPUkernel error: cannot allocate memory.\n");
			return -1;
		}
	}

	int n_result;
	image[0] = clCreateFromGLTexture2D(h_context, CL_MEM_READ_WRITE,
			 GL_TEXTURE_2D, 0, textures[0], &n_result);

	if (n_result != CL_SUCCESS) {
		fprintf(stderr, "OpenCL error: clCreateFromGLTexture2D() failed.\n");
		return -2;
	}

	image[1] = clCreateFromGLTexture2D(h_context, CL_MEM_READ_WRITE,
		GL_TEXTURE_2D, 0, textures[1], &n_result);

	if (n_result != CL_SUCCESS) {
		fprintf(stderr, "OpenCL error: clCreateFromGLTexture2D() failed.\n");
		return -2;
	}

	return 0;
}


// Start loaded program.
// block_size -> thread count of one block.
// work_size -> thread count of all blocks.
// @return -> returns flag of successfully.
int GPUPathtracer::startGPUprogram(cl_mem* imagesGL, size_t* global_size, size_t* local_size) 
{
	// Error code variable.
	int n_result;

	// Acquire ownership of GL texture for OpenCL Image
	n_result = clEnqueueAcquireGLObjects(h_cmd_queue, 2, imagesGL, 0, 0, 0);
	// Start compute.
	n_result = clEnqueueNDRangeKernel(h_cmd_queue, *actualKernel,
		2, 0, global_size, local_size, 0, 0, 0);
	if (n_result != CL_SUCCESS) {
		fprintf(stderr, "OpenCL error: %d clEnqueueNDRangeKernel() failed\n", n_result);
		return -1;
	}
	clFlush(h_cmd_queue);
	clFinish(h_cmd_queue);
	// Release ownership of GL texture for OpenCL Image
	n_result = clEnqueueReleaseGLObjects(h_cmd_queue, 2, imagesGL, 0, 0, 0);

	return 0;
}

// Read data from GPU buffer.
// buffer -> GPU buffer
// size -> size of memory pointed by arg data
// data -> memory where readed data will be saved
// type -> type of cl_mem
// @return -> returns flag of successfully
int GPUPathtracer::readGPUbuffer(cl_mem* buffer, size_t offset, size_t size, void* data, int type) 
{	
	// Is buffer set?
	if (buffer == NULL) {
		fprintf(stderr, "readGPUdata() error: NULL pointer.\n");
		return -1;
	}

	// Is memory allocated?
	if (data == NULL) {
		data = (void*) malloc(size * sizeof(char));
		// Test right allocation.
		if (!testAllocMemory(data)) {
			fprintf(stderr, "readGPUdata() error: cannot alocate memory.\n");
			return -2;
		}
	}
	// Error code variable.
	int n_result;

	// Read GPU buffer. 
	n_result = clEnqueueReadBuffer(h_cmd_queue, *buffer,
		true, offset, size * sizeof(char), data, 0, 0, 0);
	if (n_result != CL_SUCCESS) {
		fprintf(stderr, "OpenCL error: cannot read data from clEnqueueReadBuffer.\n");
		return -3;
	}

	return 0;
}

// Read data from GPU buffer.
// buffer -> GPU buffer
// size -> size of memory pointed by arg data
// data -> memory where readed data will be saved
// type -> type of cl_mem
// @return -> returns flag of successfully
int GPUPathtracer::readGPUimage(cl_mem* image, size_t off_x, size_t off_y, size_t size, void* data) 
{
	// Is buffer set?
	if (image == NULL) {
		fprintf(stderr, "readGPUdata() error: NULL pointer.\n");
		return -1;
	}

	// Is memory allocated?
	if (data == NULL) {
		data = (void*)malloc(size * sizeof(char));
		// Test right allocation.
		if (!testAllocMemory(data)) {
			fprintf(stderr, "readGPUdata() error: cannot alocate memory.\n");
			return -2;
		}
	}
	// Error code variable.
	int n_result;

	// Read GPU image. 
	size_t origin[] = { off_x, off_y, 0 };
	size_t region[] = { size, size, 1 };
	n_result = clEnqueueReadImage(h_cmd_queue, *image,
		true, origin, region, 0, 0, data, 0, NULL, NULL);
	if (n_result != CL_SUCCESS) {
		fprintf(stderr, "OpenCL error: cannot read data from clEnqueueReadBuffer.\n");
		return -3;
	}

	return 0;
}

// Write data to GPU buffer.
// buffer -> handle to GPU buffer
// offset -> begin of data place in buffer
// size -> size of writed data
// data -> source of data
// @return -> successfully of operation
int GPUPathtracer::writeGPUdata(cl_mem* buffer, size_t offset, size_t size, void* data) 
{
	// Is buffer and data set?
	if ((buffer == NULL) || (data == NULL)) {
		fprintf(stderr, "readGPUdata() error: buffer or data not set.\n");
		return -1;
	}
	// Error code variable.
	int n_result;
	
	// Write data to GPU buffer.
	n_result = clEnqueueWriteBuffer(h_cmd_queue, *buffer,
		true, offset, size, data, 0, 0, 0);
	if (n_result != CL_SUCCESS) {
		fprintf(stderr, "OpenCL error: cannot read data from clEnqueueReadBuffer.\n");
		return -2;
	}

	return 0;

}

// Write spheres, triangles, meshes to GPU buffers.
// sp -> list of spheres
// pt_sphere -> buffer for spheres
// tr -> list of triangles
// pt_triangle -> buffer for triangles
// me -> list of meshes
// pt_meshes -> buffer for meshes
// ra_me -> list of ranges of meshes
// pt_range_meshes -> buffer for ranges
// flag -> option of buffers creation
// @return -> return error code
int GPUPathtracer::writeGPUobjects(
	std::vector<TSphere>& sp, cl_mem& pt_sphere,
	std::vector<TTriangle>& tr, cl_mem& pt_triangle, 
	std::vector<TMesh>& me, cl_mem& pt_meshes,
	std::vector<unsigned int>& ra_me, cl_mem& pt_range_meshes, 
	cl_mem_flags flag) 
{
	// Error code variable.
	int n_result;

	// Allocate memory for GPU buffer.
	pt_sphere = clCreateBuffer(h_context,
		flag, sp.size() * sizeof(TSphere), 0, &n_result);
	if (n_result != CL_SUCCESS) {
		fprintf(stderr, "OpenCL error: clCreateBuffer() cannot create buffer.\n");
		return -1;
	}

	pt_triangle = clCreateBuffer(h_context,
		flag, tr.size() * sizeof(TTriangle), 0, &n_result);
	if (n_result != CL_SUCCESS) {
		fprintf(stderr, "OpenCL error: clCreateBuffer() cannot create buffer.\n");
		return -1;
	}

	pt_meshes = clCreateBuffer(h_context,
		flag, me.size() * sizeof(TMesh), 0, &n_result);
	if (n_result != CL_SUCCESS) {
		fprintf(stderr, "OpenCL error: clCreateBuffer() cannot create buffer.\n");
		return -1;
	}

	pt_range_meshes = clCreateBuffer(h_context,
		flag, ra_me.size() * sizeof(unsigned int), 0, &n_result);
	if (n_result != CL_SUCCESS) {
		fprintf(stderr, "OpenCL error: clCreateBuffer() cannot create buffer.\n");
		return -1;
	}

	// Sizes of kernel structures.
	size_t size_sp = sizeof(TSphere), size_tr = sizeof(TTriangle);
	size_t size_me = sizeof(TMesh), size_ra_me = sizeof(unsigned int);

	// Write all spheres to buffer.
	int max = sp.size();
	for (int i = 0; i < max; i++) {
		n_result = clEnqueueWriteBuffer(h_cmd_queue, pt_sphere,
			true, size_sp * i, size_sp, &(sp[i]), 0, 0, 0);

		if (n_result != CL_SUCCESS) {
			fprintf(stderr, "OpenCL error: cannot read data from clEnqueueReadBuffer.\n");
			return -2;
		}
	}

	// Write all triangles to buffer.
	max = tr.size();
	for (int i = 0; i < max; i++) {
		n_result = clEnqueueWriteBuffer(h_cmd_queue, pt_triangle,
			true, size_tr * i, size_tr, &(tr[i]), 0, 0, 0);

		if (n_result != CL_SUCCESS) {
			fprintf(stderr, "OpenCL error: cannot read data from clEnqueueReadBuffer.\n");
			return -3;
		}
	}

	// Write all meshes to buffer.
	max = me.size();
	for (int i = 0; i < max; i++) {
		n_result = clEnqueueWriteBuffer(h_cmd_queue, pt_meshes,
			true, size_me * i, size_me, &(me[i]), 0, 0, 0);

		if (n_result != CL_SUCCESS) {
			fprintf(stderr, "OpenCL error: cannot read data from clEnqueueReadBuffer.\n");
			return -4;
		}
	}
	
	// Write all ranges of meshes to buffer.
	max = ra_me.size();
	for (int i = 0; i < max; i++) {
		n_result = clEnqueueWriteBuffer(h_cmd_queue, pt_range_meshes,
			true, size_ra_me * i, size_ra_me, &(ra_me[i]), 0, 0, 0);

		if (n_result != CL_SUCCESS) {
			fprintf(stderr, "OpenCL error: cannot read data from clEnqueueReadBuffer.\n");
			return -5;
		}
	}

	return 0;
}

// Write light objects to GPU buffer.
// li -> list of lights
// pt_light -> buffer for lights
// flag -> buffer's options
// @return -> return error code
int GPUPathtracer::writeGPUlights(std::vector<TLight>& li, cl_mem& pt_light, cl_mem_flags flag)
{
	// Error code variable.
	int n_result;
	int max = li.size();
	int size_li = sizeof(TLight);

	// Allocate memory for GPU buffer.
	pt_light = clCreateBuffer(h_context,
		flag, max * sizeof(TSphere), 0, &n_result);
	if (n_result != CL_SUCCESS) {
		fprintf(stderr, "OpenCL error: clCreateBuffer() cannot create buffer.\n");
		return -1;
	}

	// Write all ranges of meshes to buffer.
	for (int i = 0; i < max; i++) {
		n_result = clEnqueueWriteBuffer(h_cmd_queue, pt_light,
			true, size_li * i, size_li, &(li[i]), 0, 0, 0);

		if (n_result != CL_SUCCESS) {
			fprintf(stderr, "OpenCL error: cannot read data from clEnqueueReadBuffer.\n");
			return -2;
		}
	}

	return 0;
}

// Code was get from Ing. Polok's personal site. 
// Read file.
// p_s_filename -> file name
// @return -> returns loaded file as "string"
char* GPUPathtracer::p_s_ReadFile(const char* p_s_filename) 
{	
	FILE *p_fr;
#if defined(_MSC_VER) && !defined(__MWERKS__) && _MSC_VER >= 1400
	if (fopen_s(&p_fr, p_s_filename, "rb"))
#else //_MSC_VER && !__MWERKS__ && _MSC_VER >= 1400
	if (!(p_fr = fopen(p_s_filename, "rb")))
#endif //_MSC_VER && !__MWERKS__ && _MSC_VER >= 1400
		return 0;
	
	int n_length;
	if (fseek(p_fr, 0, SEEK_END) ||
		(n_length = ftell(p_fr)) < 0 ||
		fseek(p_fr, 0, SEEK_SET)) {
		fclose(p_fr);
		return 0;
	}
	
	char *p_s_file = new(std::nothrow) char[n_length + 1];
	if (!p_s_file) {
		fclose(p_fr);
		return 0;
	}
	
	if (fread(p_s_file, sizeof(char), n_length, p_fr) != n_length) {
		delete[] p_s_file;
		fclose(p_fr);
		return 0;
	}
	
	p_s_file[n_length] = 0; // add a terminating null
	fclose(p_fr);
	return p_s_file;
}

// Code was get from Ing. Polok's personal site. 
// Print information about compile.
// h_program -> OpenCL program
// h_device -> device where is OpenCL program compiled
// @return -> returns flag of successfully compile
bool GPUPathtracer::Show_CompilerOutput(cl_program h_program, cl_device_id h_device)
{
	size_t n_size;
	int n_result = clGetProgramBuildInfo(h_program, h_device,
		CL_PROGRAM_BUILD_LOG, 0, 0, &n_size);
	if (n_result != CL_SUCCESS) {
		fprintf(stderr, "error: clGetProgramBuildInfo() failed\n");
		return false;
	}
	char *p_s_build_status = new(std::nothrow) char[n_size + 1];
	if (!p_s_build_status) {
		fprintf(stderr, "error: not enough memory\n");
		return false;
	}
	n_result = clGetProgramBuildInfo(h_program, h_device,
		CL_PROGRAM_BUILD_LOG, n_size, p_s_build_status, &n_size);
	if (n_result != CL_SUCCESS) {
		fprintf(stderr, "error: clGetProgramBuildInfo() failed\n");
		delete[] p_s_build_status;
		return false;
	}
	p_s_build_status[n_size] = 0; // just to make sure

	fprintf(stderr, "%s\n", p_s_build_status);

	delete[] p_s_build_status;

	return true;
}

// Test if memory is alocated.
// pointer -> tested memory
// @return -> returns flag if memory is allocated
bool GPUPathtracer::testAllocMemory(void* pointer) {
	if (pointer == NULL)
		return false;
	else
		return true;
}

// End of gpu_pathtracer.cpp