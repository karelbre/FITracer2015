/*
								+--------------------------------+
								|                                |
								|     ***   OpenGL 3.0   ***     |
								|                                |
								|  Copyright © -tHE SWINe- 2009  |
								|                                |
								|           OpenGL30.h           |
								|                                |
								+--------------------------------+
*/

/**
 *	@file gl3/OpenGL30.h
 *	@author -tHE SWINe-
 *	@brief OpenGL 3.0 extension entry pointers
 *
 *	This header contains entry points of relevant OpenGL 3 functions / extensions.
 *		It was prepared from gl3.h, available from <www.opengl.org>
 *
 *	@date 2009-11-28
 *	core functionality / core extensions:
 *		OpenGL 1.0
 *		OpenGL 1.1
 *		OpenGL 1.2
 *		OpenGL 1.3
 *		OpenGL 1.4
 *		OpenGL 1.5
 *		OpenGL 2.0
 *		OpenGL 2.1
 *		OpenGL 3.0
 *			GL_ARB_depth_buffer_float
 *			GL_ARB_framebuffer_object
 *			GL_ARB_framebuffer_sRGB
 *			GL_ARB_half_float_vertex
 *			GL_ARB_map_buffer_range
 *			GL_ARB_texture_compression_rgtc
 *			GL_ARB_texture_rg
 *			GL_ARB_vertex_array_object
 *		OpenGL 3.1
 *			GL_ARB_uniform_buffer_object
 *			GL_ARB_draw_instanced
 *			GL_ARB_copy_buffer
 *		OpenGL 3.2
 *			GL_ARB_depth_clamp
 *			GL_ARB_draw_elements_base_vertex
 *			GL_ARB_fragment_coord_conventions
 *			GL_ARB_provoking_vertex
 *			GL_ARB_seamless_cube_map
 *			GL_ARB_sync
 *			GL_ARB_texture_multisample
 *			GL_ARB_vertex_array_bgra
 *
 *	present extensions:
 *		GL_EXT_draw_buffers2			(separate blend enables, separate color masks for mutliple draw buffers)
 *		GL_ARB_draw_buffers_blend		(different blend equations for multiple draw buffers)
 *		GL_ARB_sample_shading			(enforced shading for multisample antialiassing)
 *		GL_ARB_texture_cube_map_array	(texture array for cube-maps)
 *		GL_ARB_texture_gather			(new functions for GLSL, returns x components of four sampled texels, lets shader do it's own filtering, instead of bilinear. without GL_EXT_texture_swizzle it's only monochrome though)
 *		GL_ARB_texture_query_lod		(new functions for GLSL, returns LOD sampler would use for texture (instead of sampled color))
 *		GL_EXT_timer_query				(new query object target for GPU timing)
 *		GL_EXT_gpu_program_parameters	(single call to fill multiple GPU program parameters; useless because we do not have programs anymore - only shaders)
 *		GL_EXT_bindable_uniform			(provides bindable buffers to contain uniform variables, so they don't have to be specified)
 *		GL_ARB_instanced_arrays			(sets divisor to vertex attributes; when set to n = nonzero, a particular attribute advances every n vertices; similar to direct-x function SetStreamFrequency)
 *		GL_ARB_shader_texture_lod		(additions to GLSL; contains no new tokens for GL)
 *		GL_ARB_texture_buffer_object	(new texture type, buffer object is now bindable as large 1D texture; it's unclear how exactly is it used)
 *
 *		GL_S3_s3tc						(pixel formats)
 *		GL_EXT_texture_compression_latc	(pixel formats)
 *		GL_EXT_texture_compression_s3tc	(pixel formats)
 *		GL_EXT_texture_filter_anisotropic	(anisotropic texture filter control)
 *		GL_EXT_abgr						(ABGR pixel format)
 *
 *	new extensions which are not present yet:
 *		GL_ARB_compatibility - a big one, fork of OpenGL 3.1
 *
 *	removed:
 *		GL_ARB_texture_compression - promoted to core in OpenGL 1.3
 *		GL_ARB_vertex_blend - relies on deprecated parts of fixed-function pipeline
 *		GL_ARB_matrix_palette - relies on deprecated parts of fixed-function pipeline
 *		GL_EXT_compiled_vertex_array - is now obsolete, extension wasn't updated for a long time
 *		GL_EXT_vertex_array - is obsolete, became core and in turn, became deprecated
 *		GL_ARB_vertex_program, GL_ARB_fragment_program - deprecated; only shading language 1.30 is supported
 *		GL_ARB_vertex_shader, GL_ARB_fragment_shader, GL_ARB_shader_object - became core in OpenGL 2.0
 *		GL_ARB_vertex_buffer_object - became core in OpenGL 1.5
 *		GL_ARB_pixel_buffer_object - became core in OpenGL 2.1
 *		GL_ARB_occlusion_query - became core in OpenGL 1.5
 *		GL_ARB_multitexture - became core in OpenGL 1.3; n_GetMultitextureFuncPointers-  was merged into n_GetGL13FuncPointers- 
 *		GL_EXT_packed_depth_stencil - part of GL_ARB_framebuffer_object, which is core in OpenGL 3.0
 *		GL_EXT_framebuffer_object - part of GL_ARB_framebuffer_object, which is core in OpenGL 3.0
 *		GL_EXT_draw_instanced - became core in OpenGL 3.1
 *
 *	not present:
 *		GL_ARB_imaging (part of OpenGL 1.2 core) - seems to be deprecated
 *		GL_ARB_color_buffer_float - superseded by OpenGL 3.0
 *		GL_NV_fence - superseded by GL_ARB_sync
 *		GL_EXT_texture_integer - superseded by OpenGL 3.0
 *		GL_EXT_gpu_shader4 - superseded by OpenGL 3.0, OpenGL 3.1
 *		GL_EXT_geometry_shader4 - superseded by GL_ARB_framebuffer_object, OpenGL 3.2
 *		GL_EXT_texture_array - superseded by GL_ARB_framebuffer_object, OpenGL 3.0
 *		GL_EXT_texture_buffer_object - superseded by OpenGL 3.1
 *		GL_EXT_blend_minmax - superseded by GL_ARB_imaging, OpenGL 1.2
 *		GL_EXT_blend_subtract - superseded by GL_ARB_imaging, OpenGL 1.2
 *		GL_ARB_geometry_shader4 - superseded by GL_ARB_framebuffer_object, OpenGL 3.2
 *		GL_ARB_draw_instanced - directly part of OpenGL 3.1 core
 *		GL_ARB_half_float_pixel - superseded by GL_ARB_half_float_vertex, part of OpenGL 3.0 core
 *		GL_ARB_texture_float - part of OpenGL 3.0 core
 *		GL_ARB_pixel_buffer_object - part of OpenGL 2.1 core
 *
 *	@date 2010-09-19
 *
 *	added the following extensions:
 *		GL_ARB_shading_language_include
 *		GL_ARB_texture_compression_bptc
 *		GL_ARB_blend_func_extended
 *		GL_ARB_explicit_attrib_location
 *		GL_ARB_occlusion_query2
 *		GL_ARB_sampler_objects
 *		GL_ARB_shader_bit_encoding
 *		GL_ARB_texture_rgb10_a2ui
 *		GL_ARB_texture_swizzle
 *		GL_ARB_timer_query
 *		GL_ARB_vertex_type_2_10_10_10_rev
 *		GL_ARB_draw_indirect
 *		GL_ARB_gpu_shader5
 *		GL_ARB_gpu_shader_fp64
 *		GL_ARB_shader_subroutine
 *		GL_ARB_tessellation_shader
 *		GL_ARB_texture_buffer_object_rgb32
 *		GL_ARB_transform_feedback2
 *		GL_ARB_transform_feedback3
 *		GL_ARB_es2_compatibility
 *		GL_ARB_get_program_binary
 *		GL_ARB_separate_shader_objects
 *		GL_ARB_shader_precision
 *		GL_ARB_vertex_attrib64bit
 *		GL_ARB_viewport_array
 *		GL_ARB_cl_event
 *		GL_ARB_debug_output
 *		GL_ARB_robustness
 *		GL_ARB_shader_stencil_export
 *
 *	@date 2010-10-29
 *
 *	Unified windows detection macro to "#if defined(_WIN32) || defined(_WIN64)".
 *
 *	@date 2011-07-15
 *
 *	removed glFramebufferTextureFace(), which was a bug in the documentation. OpenGL 3.2 support
 *	is now reported complete (CGL3ExtensionHandler::n_GetGL32FuncPointers() returns 0,
 *	and not 1 as it did).
 *
 *	re-organized core functionality (the extensions were all present already):
 *		OpenGL 3.3:
 *			GL_ARB_shader_bit_encoding
 *			GL_ARB_blend_func_extended
 *			GL_ARB_explicit_attrib_location
 *			GL_ARB_occlusion_query2
 *			GL_ARB_sampler_objects
 *			GL_ARB_texture_rgb10_a2ui
 *			GL_ARB_texture_swizzle
 *			GL_ARB_timer_query
 *			GL_ARB_instanced_arrays
 *			GL_ARB_vertex_type_2_10_10_10_rev
 *
 */

/*
** Copyright (c) 2007-2009 The Khronos Group Inc.
** 
** Permission is hereby granted, free of charge, to any person obtaining a
** copy of this software and/or associated documentation files (the
** "Materials"), to deal in the Materials without restriction, including
** without limitation the rights to use, copy, modify, merge, publish,
** distribute, sublicense, and/or sell copies of the Materials, and to
** permit persons to whom the Materials are furnished to do so, subject to
** the following conditions:
** 
** The above copyright notice and this permission notice shall be included
** in all copies or substantial portions of the Materials.
** 
** THE MATERIALS ARE PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
** EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
** MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
** IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
** CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
** TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
** MATERIALS OR THE USE OR OTHER DEALINGS IN THE MATERIALS.
*/

/*
** License Applicability. Except to the extent portions of this file are
** made subject to an alternative license as permitted in the SGI Free
** Software License B, Version 1.1 (the "License"), the contents of this
** file are subject only to the provisions of the License. You may not use
** this file except in compliance with the License. You may obtain a copy
** of the License at Silicon Graphics, Inc., attn: Legal Services, 1600
** Amphitheatre Parkway, Mountain View, CA 94043-1351, or at:
** 
** http://oss.sgi.com/projects/FreeB
** 
** Note that, as provided in the License, the Software is distributed on an
** "AS IS" basis, with ALL EXPRESS AND IMPLIED WARRANTIES AND CONDITIONS
** DISCLAIMED, INCLUDING, WITHOUT LIMITATION, ANY IMPLIED WARRANTIES AND
** CONDITIONS OF MERCHANTABILITY, SATISFACTORY QUALITY, FITNESS FOR A
** PARTICULAR PURPOSE, AND NON-INFRINGEMENT.
** 
** Original Code. The Original Code is: OpenGL Sample Implementation,
** Version 1.2.1, released January 26, 2000, developed by Silicon Graphics,
** Inc. The Original Code is Copyright (c) 1991-2004 Silicon Graphics, Inc.
** Copyright in any portions created by third parties is as indicated
** elsewhere herein. All Rights Reserved.
** 
** Additional Notice Provisions: This software was created using the
** OpenGL(R) version 1.2.1 Sample Implementation published by SGI, but has
** not been independently verified as being compliant with the OpenGL(R)
** version 1.2.1 Specification.
*/

#ifndef __OPENGL30_TOOL_INCLUDED
#define __OPENGL30_TOOL_INCLUDED

#include <string>
#include "Integer.h"
#if defined(_WIN32) || defined (_WIN64)
#define WIN32_LEAND_AND_MEAN 1
#include <windows.h>
#endif // _WIN32, _WIN64

/*
 *	makes life a little bit easier when including gl.h by mistake
 */
#if defined(__gl_h_) || defined(__GL_H__) || defined(GL_VERSION_1_1)
#error("warning: gl.h must NOT be included in files using OpenGL30.h")
#endif

/*
 *	makes life a little bit easier when including glext.h by mistake
 */
#if defined(__glext_h_) || defined(GL_GLEXT_VERSION)
#error("glext.h must NOT be included in files using OpenGL30.h")
#endif

#define __gl_h_
#define __GL_H__
// does not include gl.h, defines GL up to 1.1 below, want to prevent gl.h
// header inclusion

#define __glext_h_
#define __GLEXT_H__
// glext.h is not included anymore, it does contain significant amount of
// deprecated functionality, want to prevent glext.h header inclusion

/**
 *	@def NULL_FUNCTION_POINTERS
 *	@brief if defined, initializes entry function pointers to null
 *
 *	In case NULL_FUNCTION_POINTERS is defined (default), all extension function pointers
 *		are initialized to 0, but user never needs to compare individual function values
 *		so it's posible to save some space in executable file by skipping the initialization.
 */
#define NULL_FUNCTION_POINTERS

/**
 *	@def FIX_GL_FUNCTION_LINKING_CONFLICTS
 *	@brief avoids function linking conflicts by renaming function pointers, and fixing
 *		names using #define
 *
 *	Some compilers may contain newer OpenGL libraries than this header requires
 *		(OpenGL 1.1, as supported by Microsoft Visual studio family), so names of function
 *		pointers, such as <tt>glBlendColor</tt> are present in static library, as well as in this
 *		file, causing link-time error. This is fixed by renaming the function, here
 *		<tt>glBlendColor_ptr</tt> would be used, and fixing it by macro, such as <tt>#define
 *		glBlendColor_ptr glBlendColor</tt>. The downside of this is, the Visual Studio edior
 *		will no longer show pop-up function parameters (intellisense), so this is best
 *		avoided if possible.
 */
#define FIX_GL_FUNCTION_LINKING_CONFLICTS

/**
 *	@brief class for handling OpenGL extensions
 *
 *	Supplies means to get OpenGL function entry point, check extension support, or get
 *	OpenGL version. Functions with name n_Get*FuncPointers() returns number of function
 *	pointers that were not found (zero is success).
 */
class CGL3ExtensionHandler {
public:
	/**
	 *	@brief compares OpenGL version to reference
	 *
	 *	Compares supplied version to current OpenGL implementation version and returns
	 *		true if current version is greater or equal (supplied version is supported).
	 *
	 *	@param[in] n_min_major is reference minimal major version number
	 *	@param[in] n_min_minor is reference minimal minor version number
	 *
	 *	@return Returns true in case OpenGL version  is greater or equal to n_min_major.n_min_minor.
	 */
	static bool b_Support_OpenGL(int n_min_major = 3, int n_min_minor = 0);

	/**
	 *	@brief returns OpenGL version
	 *
	 *	Returns OpenGL version as (comparable) integer value.
	 *
	 *	@return Returns OpenGL version as major * 10 + minor (ie. for OpenGL 3.2 returns 32).
	 */
	static int n_OpenGL_Version();

	/**
	 *	@brief returns OpenGL version major
	 *
	 *	@return Returns OpenGL version as major (ie. for OpenGL 3.2 returns 3).
	 */
	static int n_OpenGL_Version_Major();

	/**
	 *	@brief returns OpenGL version minor
	 *
	 *	@return Returns OpenGL version as minor (ie. for OpenGL 3.2 returns 2).
	 */
	static int n_OpenGL_Version_Minor();

	/**
	 *	@brief checks extension support
	 *
	 *	Checks support of a given OpenGL extension, returns true if extension is supported.
	 *
	 *	@param[in] p_s_extension_name is null-terminated string, containing name
	 *		of OpenGL extension encoded in UTF-8
	 *
	 *	@return Returns true in case p_s_extension_name is supported, otherwise false.
	 */
	static bool b_SupportedExtension(const char *p_s_extension_name);

	/**
	 *	@brief gets extension string
	 *
	 *	OpenGL 3.0 doesn't have classic extension string, it returns names of individual
	 *		extensions instead. This is to prevent buffer overruns, people always seem
	 *		to be copying OpenGL extensions to fixed-size buffers, causing older software
	 *		from times when extension string was shorter to fail on modern hardware.
	 *		This functions returns "classic" OpenGL extension string in std::string,
	 *		encoded in UTF-8, names of individual extensions are separated by space (' ')
	 *		character.
	 *
	 *	@return Returns true on success, false on failure (not enough memory).
	 */
	static bool Get_ExtensionString(std::string &r_s_extension_list);

#if defined(_WIN32) || defined (_WIN64)
	/**
	 *	@brief checks extension support
	 *
	 *	Checks support of a given WGL extension, returns true if extension is supported.
	 *
	 *	@param[in] p_s_extension_name is null-terminated string, containing name
	 *		of WGL extension encoded in UTF-8
	 *
	 *	@return Returns true in case p_s_extension_name is supported, otherwise false.
	 */
	static bool b_SupportedWGLExtension(const char *p_s_extension_name);
#endif // _WIN32 || _WIN64

	/**
	 *	@brief gets entry points of all extensions
	 *
	 *	Gets pointers of function entry points for all supported extensions and OpenGL versions.
	 *
	 *	@return Returns number of missing functions (zero means success).
	 *
	 *	@note Note this is aimed at simple and test implementations without
	 *		much need for error checking. it is always better to call individual
	 *		functions, because in case this fails, there's no telling
	 *		which extension exactly failed to initialize.
	 */
	static int n_GetAllSupportedExtensionsFuncPointers();

	/**
	 *	@brief gets entry points of all OpenGL core functions
	 *
	 *	Gets pointers of function entry points for all supported OpenGL versions.
	 *
	 *	@return Returns number of missing functions (zero means success).
	 *
	 *	@note Note this is aimed at simple and test implementations without
	 *		much need for error checking. it is always better to call individual
	 *		functions, because in case this fails, there's no telling
	 *		which extension exactly failed to initialize.
	 */
	static int n_GetAllOpenGLCoreFuncPointers();

	/**
	 *	@brief gets entry points of all OpenGL core functions, up to the version specified
	 *
	 *	Gets pointers of function entry points for all supported OpenGL versions, up to the version specified.
	 *
	 *	@param[in] n_major is required OpenGL version major
	 *	@param[in] n_minor is required OpenGL version minor
	 *
	 *	@return Returns number of missing functions (zero means success).
	 *
	 *	@note Note this is aimed at simple and test implementations without
	 *		much need for error checking. it is always better to call individual
	 *		functions, because in case this fails, there's no telling
	 *		which extension exactly failed to initialize.
	 */
	static int n_GetOpenGLCoreFuncPointers(int n_major = 3, int n_minor = 3);

	/**
	 *	@brief gets OpenGL 1.2 core function pointers, and sets GLEH_OPENGL_1_2
	 *
	 *	@return Returns number of missing functions (zero means success).
	 */
	static int n_GetGL12FuncPointers();

	/**
	 *	@brief gets OpenGL 1.3 core function pointers, and sets GLEH_OPENGL_1_3
	 *
	 *	@return Returns number of missing functions (zero means success).
	 */
	static int n_GetGL13FuncPointers();

	/**
	 *	@brief gets OpenGL 1.4 core function pointers, and sets GLEH_OPENGL_1_4
	 *
	 *	@return Returns number of missing functions (zero means success).
	 */
	static int n_GetGL14FuncPointers();

	/**
	 *	@brief gets OpenGL 1.5 core function pointers, and sets GLEH_OPENGL_1_5
	 *
	 *	@return Returns number of missing functions (zero means success).
	 */
	static int n_GetGL15FuncPointers();

	/**
	 *	@brief gets OpenGL 2.0 core function pointers, and sets GLEH_OPENGL_2_0
	 *
	 *	@return Returns number of missing functions (zero means success).
	 */
	static int n_GetGL20FuncPointers();

	/**
	 *	@brief gets OpenGL 2.1 core function pointers, and sets GLEH_OPENGL_2_1
	 *
	 *	@return Returns number of missing functions (zero means success).
	 */
	static int n_GetGL21FuncPointers();

	/**
	 *	@brief gets OpenGL 3.0 core function pointers, and sets GLEH_OPENGL_3_0
	 *
	 *	@return Returns number of missing functions (zero means success).
	 *
	 *	@note This gets function pointers for the following core extensions as well:
	 *		GL_ARB_framebuffer_object, GL_ARB_map_buffer_range and GL_ARB_vertex_array_object.
	 */
	static int n_GetGL30FuncPointers();

	/**
	 *	@brief gets OpenGL 3.1 core function pointers, and sets GLEH_OPENGL_3_1
	 *
	 *	@return Returns number of missing functions (zero means success).
	 *
	 *	@note This gets function pointers for the following core extensions as well:
	 *		GL_ARB_uniform_buffer_object, GL_ARB_draw_instanced and GL_ARB_copy_buffer.
	 */
	static int n_GetGL31FuncPointers();

	/**
	 *	@brief gets OpenGL 3.2 core function pointers, and sets GLEH_OPENGL_3_2
	 *
	 *	@return Returns number of missing functions (zero means success).
	 *
	 *	@note This gets function pointers for the following core extensions as well:
	 *		GL_ARB_draw_elements_base_vertex, GL_ARB_provoking_vertex, GL_ARB_sync
	 *		and GL_ARB_texture_multisample.
	 */
	static int n_GetGL32FuncPointers();

	/**
	 *	@brief gets OpenGL 3.3 core function pointers, and sets GLEH_OPENGL_3_3
	 *
	 *	@return Returns number of missing functions (zero means success).
	 *
	 *	@note This gets function pointers from the following core extensions:
	 *		GL_ARB_shader_bit_encoding, GL_ARB_blend_func_extended,
	 *		GL_ARB_explicit_attrib_location, GL_ARB_occlusion_query2,
	 *		GL_ARB_sampler_objects, GL_ARB_texture_rgb10_a2ui,
	 *		GL_ARB_texture_swizzle, GL_ARB_timer_query, GL_ARB_instanced_arrays
	 *		and GL_ARB_vertex_type_2_10_10_10_rev.
	 */
	static int n_GetGL33FuncPointers();

	/**
	 *	@brief gets GL_EXT_draw_buffers2 function pointers, and sets GLEH_EXT_draw_buffers2
	 *
	 *	@return Returns number of missing functions (zero means success).
	 */
	static int n_GetEXTDrawBuffers2FuncPointers();

	/**
	 *	@brief gets GL_ARB_draw_buffers_blend function pointers, and sets
	 *		GLEH_ARB_draw_buffers_blend
	 *
	 *	@return Returns number of missing functions (zero means success).
	 */
	static int n_GetARBDrawBuffersBlendFuncPointers();

	/**
	 *	@brief gets GL_ARB_sample_shading function pointers, and sets
	 *		GLEH_ARB_sample_shading
	 *
	 *	@return Returns number of missing functions (zero means success).
	 */
	static int n_GetARBSampleShadingFuncPointers();

	/**
	 *	@brief gets GL_EXT_timer_query function pointers, and sets GLEH_EXT_timer_query
	 *
	 *	@return Returns number of missing functions (zero means success).
	 */
	static int n_GetEXTTimerQueryFuncPointers();

	/**
	 *	@brief gets GL_EXT_gpu_program_parameters function pointers, and sets
	 *		GLEH_EXT_gpu_program_parameters
	 *
	 *	@return Returns number of missing functions (zero means success).
	 */
	static int n_GetEXTGPUProgramParametersFuncPointers();

	/**
	 *	@brief gets GL_EXT_bindable_uniform function pointers, and sets
	 *		GLEH_EXT_bindable_uniform
	 *
	 *	@return Returns number of missing functions (zero means success).
	 */
	static int n_GetEXTBindableUniformFuncPointers();

	/**
	 *	@brief gets GL_EXT_abgr function pointers, and sets GLEH_EXT_abgr
	 *
	 *	@return Returns number of missing functions (zero means success).
	 *
	 *	@note this extension has no functions, so this always returns zero.
	 */
	static int n_GetEXTABGRFuncPointers();

	/**
	 *	@brief gets GL_EXT_texture_filter_anisotropic function pointers,
	 *		and sets GLEH_EXT_texture_filter_anisotropic
	 *
	 *	@return Returns number of missing functions (zero means success).
	 *
	 *	@note this extension has no functions, so this always returns zero.
	 */
	static int n_GetEXTTextureFilterAnisotropicFuncPointers();

	/**
	 *	@brief gets GL_EXT_texture_compression_latc function pointers, and
	 *		sets GLEH_EXT_texture_compression_latc
	 *
	 *	@return Returns number of missing functions (zero means success).
	 *
	 *	@note this extension has no functions, so this always returns zero.
	 */
	static int n_GetEXTTextureCompressionLatcFuncPointers();

	/**
	 *	@brief gets GL_EXT_texture_compression_s3tc function pointers, and
	 *		sets GLEH_EXT_texture_compression_s3tc
	 *
	 *	@return Returns number of missing functions (zero means success).
	 *
	 *	@note this extension has no functions, so this always returns zero.
	 */
	static int n_GetEXTTextureCompressionS3tcFuncPointers();

	/**
	 *	@brief gets GL_S3_s3tc function pointers, and sets GLEH_S3_s3tc
	 *
	 *	@return Returns number of missing functions (zero means success).
	 *
	 *	@note this extension has no functions, so this always returns zero.
	 */
	static int n_GetS3S3tcFuncPointers();

	/**
	 *	@brief gets GL_ARB_texture_query_lod function pointers, and sets
	 *		GLEH_ARB_texture_query_lod
	 *
	 *	@return Returns number of missing functions (zero means success).
	 *
	 *	@note this extension has no functions, so this always returns zero.
	 */
	static int n_GetARBTextureQueryLodFuncPointers();

	/**
	 *	@brief gets GL_EXT_texture_swizzle function pointers, and sets
	 *		GLEH_EXT_texture_swizzle
	 *
	 *	@return Returns number of missing functions (zero means success).
	 *
	 *	@note this extension has no functions, so this always returns zero.
	 */
	static int n_GetEXTTextureSwizzleFuncPointers();

	/**
	 *	@brief gets GL_ARB_texture_gather function pointers, and sets
	 *		GLEH_ARB_texture_gather
	 *
	 *	@return Returns number of missing functions (zero means success).
	 *
	 *	@note this extension has no functions, so this always returns zero.
	 */
	static int n_GetARBTextureGatherFuncPointers();

	/**
	 *	@brief gets GL_ARB_texture_cube_map_array function pointers, and
	 *		sets GLEH_ARB_texture_cube_map_array
	 *
	 *	@return Returns number of missing functions (zero means success).
	 *
	 *	@note this extension has no functions, so this always returns zero.
	 */
	static int n_GetARBTextureCubeMapArrayFuncPointers();

	/**
	 *	@brief gets GL_ARB_instanced_arrays function pointers, and sets GLEH_ARB_instanced_arrays
	 *
	 *	@return Returns number of missing functions (zero means success).
	 */
	static int n_GetARBInstancedArraysFuncPointers();

	/**
	 *	@brief gets GL_ARB_shader_texture_lod function pointers, and sets GLEH_ARB_shader_texture_lod
	 *
	 *	@return Returns number of missing functions (zero means success).
	 *
	 *	@note this extension has no functions, so this always returns zero.
	 */
	static int n_GetARBShaderTextureLodFuncPointers();

	/**
	 *	@brief gets GL_ARB_texture_buffer_object function pointers, and sets GLEH_ARB_texture_buffer_object
	 *
	 *	@return Returns number of missing functions (zero means success).
	 */
	static int n_GetARBTextureBufferObjectFuncPointers();

	/**
	 *	@brief gets GL_ARB_shading_language_include function pointers, and sets GLEH_ARB_shading_language_include
	 *
	 *	@return Returns number of missing functions (zero means success).
	 */
	static int n_GetARBShadingLanguageIncludeFuncPointers();

	/**
	 *	@brief gets GL_ARB_texture_compression_bptc function pointers, and sets GLEH_ARB_texture_compression_bptc
	 *
	 *	@return Returns number of missing functions (zero means success).
	 *
	 *	@note this extension has no functions, so this always returns zero.
	 */
	static int n_GetARBTextureCompressionBptcFuncPointers();

	/**
	 *	@brief gets GL_ARB_blend_func_extended function pointers, and sets GLEH_ARB_blend_func_extended
	 *
	 *	@return Returns number of missing functions (zero means success).
	 */
	static int n_GetARBBlendFuncExtendedFuncPointers();

	/**
	 *	@brief gets GL_ARB_explicit_attrib_location function pointers, and sets GLEH_ARB_explicit_attrib_location
	 *
	 *	@return Returns number of missing functions (zero means success).
	 *
	 *	@note this extension has no functions, so this always returns zero.
	 */
	static int n_GetARBExplicitAttribLocationFuncPointers();

	/**
	 *	@brief gets GL_ARB_occlusion_query2 function pointers, and sets GLEH_ARB_occlusion_query2
	 *
	 *	@return Returns number of missing functions (zero means success).
	 *
	 *	@note this extension has no functions, so this always returns zero.
	 */
	static int n_GetARBOcclusionQuery2FuncPointers();

	/**
	 *	@brief gets GL_ARB_sampler_objects function pointers, and sets GLEH_ARB_sampler_objects
	 *
	 *	@return Returns number of missing functions (zero means success).
	 */
	static int n_GetARBSamplerObjectsFuncPointers();

	/**
	 *	@brief gets GL_ARB_shader_bit_encoding function pointers, and sets GLEH_ARB_shader_bit_encoding
	 *
	 *	@return Returns number of missing functions (zero means success).
	 *
	 *	@note this extension has no functions, so this always returns zero.
	 */
	static int n_GetARBShaderBitEncodingFuncPointers();

	/**
	 *	@brief gets GL_ARB_texture_rgb10_a2ui function pointers, and sets GLEH_ARB_texture_rgb10_a2ui
	 *
	 *	@return Returns number of missing functions (zero means success).
	 *
	 *	@note this extension has no functions, so this always returns zero.
	 */
	static int n_GetARBTextureRgb10A2uiFuncPointers();

	/**
	 *	@brief gets GL_ARB_texture_swizzle function pointers, and sets GLEH_ARB_texture_swizzle
	 *
	 *	@return Returns number of missing functions (zero means success).
	 *
	 *	@note this extension has no functions, so this always returns zero.
	 */
	static int n_GetARBTextureSwizzleFuncPointers();

	/**
	 *	@brief gets GL_ARB_timer_query function pointers, and sets GLEH_ARB_timer_query
	 *
	 *	@return Returns number of missing functions (zero means success).
	 */
	static int n_GetARBTimerQueryFuncPointers();

	/**
	 *	@brief gets GL_ARB_vertex_type_2_10_10_10_rev function pointers, and sets GLEH_ARB_vertex_type_2_10_10_10_rev
	 *
	 *	@return Returns number of missing functions (zero means success).
	 */
	static int n_GetARBVertexType2101010RevFuncPointers();

	/**
	 *	@brief gets GL_ARB_draw_indirect function pointers, and sets GLEH_ARB_draw_indirect
	 *
	 *	@return Returns number of missing functions (zero means success).
	 */
	static int n_GetARBDrawIndirectFuncPointers();

	/**
	 *	@brief gets GL_ARB_gpu_shader5 function pointers, and sets GLEH_ARB_gpu_shader5
	 *
	 *	@return Returns number of missing functions (zero means success).
	 *
	 *	@note this extension has no functions, so this always returns zero.
	 */
	static int n_GetARBGpuShader5FuncPointers();

	/**
	 *	@brief gets GL_ARB_gpu_shader_fp64 function pointers, and sets GLEH_ARB_gpu_shader_fp64
	 *
	 *	@return Returns number of missing functions (zero means success).
	 */
	static int n_GetARBGpuShaderFp64FuncPointers();

	/**
	 *	@brief gets GL_ARB_shader_subroutine function pointers, and sets GLEH_ARB_shader_subroutine
	 *
	 *	@return Returns number of missing functions (zero means success).
	 */
	static int n_GetARBShaderSubroutineFuncPointers();

	/**
	 *	@brief gets GL_ARB_tessellation_shader function pointers, and sets GLEH_ARB_tessellation_shader
	 *
	 *	@return Returns number of missing functions (zero means success).
	 */
	static int n_GetARBTessellationShaderFuncPointers();

	/**
	 *	@brief gets GL_ARB_texture_buffer_object_rgb32 function pointers, and sets GLEH_ARB_texture_buffer_object_rgb32
	 *
	 *	@return Returns number of missing functions (zero means success).
	 *
	 *	@note this extension has no functions, so this always returns zero.
	 */
	static int n_GetARBTextureBufferObjectRgb32FuncPointers();

	/**
	 *	@brief gets GL_ARB_transform_feedback2 function pointers, and sets GLEH_ARB_transform_feedback2
	 *
	 *	@return Returns number of missing functions (zero means success).
	 */
	static int n_GetARBTransformFeedback2FuncPointers();

	/**
	 *	@brief gets GL_ARB_transform_feedback3 function pointers, and sets GLEH_ARB_transform_feedback3
	 *
	 *	@return Returns number of missing functions (zero means success).
	 */
	static int n_GetARBTransformFeedback3FuncPointers();

	/**
	 *	@brief gets GL_ARB_ES2_compatibility function pointers, and sets GLEH_ARB_ES2_compatibility
	 *
	 *	@return Returns number of missing functions (zero means success).
	 */
	static int n_GetARBES2CompatibilityFuncPointers();

	/**
	 *	@brief gets GL_ARB_get_program_binary function pointers, and sets GLEH_ARB_get_program_binary
	 *
	 *	@return Returns number of missing functions (zero means success).
	 */
	static int n_GetARBGetProgramBinaryFuncPointers();

	/**
	 *	@brief gets GL_ARB_separate_shader_objects function pointers, and sets GLEH_ARB_separate_shader_objects
	 *
	 *	@return Returns number of missing functions (zero means success).
	 */
	static int n_GetARBSeparateShaderObjectsFuncPointers();

	/**
	 *	@brief gets GL_ARB_shader_precision function pointers, and sets GLEH_ARB_shader_precision
	 *
	 *	@return Returns number of missing functions (zero means success).
	 *
	 *	@note this extension has no functions, so this always returns zero.
	 */
	static int n_GetARBShaderPrecisionFuncPointers();

	/**
	 *	@brief gets GL_ARB_vertex_attrib_64bit function pointers, and sets GLEH_ARB_vertex_attrib_64bit
	 *
	 *	@return Returns number of missing functions (zero means success).
	 */
	static int n_GetARBVertexAttrib64bitFuncPointers();

	/**
	 *	@brief gets GL_ARB_viewport_array function pointers, and sets GLEH_ARB_viewport_array
	 *
	 *	@return Returns number of missing functions (zero means success).
	 */
	static int n_GetARBViewportArrayFuncPointers();

	/**
	 *	@brief gets GL_ARB_cl_event function pointers, and sets GLEH_ARB_cl_event
	 *
	 *	@return Returns number of missing functions (zero means success).
	 */
	static int n_GetARBClEventFuncPointers();

	/**
	 *	@brief gets GL_ARB_debug_output function pointers, and sets GLEH_ARB_debug_output
	 *
	 *	@return Returns number of missing functions (zero means success).
	 */
	static int n_GetARBDebugOutputFuncPointers();

	/**
	 *	@brief gets GL_ARB_robustness function pointers, and sets GLEH_ARB_robustness
	 *
	 *	@return Returns number of missing functions (zero means success).
	 */
	static int n_GetARBRobustnessFuncPointers();

	/**
	 *	@brief gets GL_ARB_shader_stencil_export function pointers, and sets GLEH_ARB_shader_stencil_export
	 *
	 *	@return Returns number of missing functions (zero means success).
	 *
	 *	@note this extension has no functions, so this always returns zero.
	 */
	static int n_GetARBShaderStencilExportFuncPointers();

private:
	// core in OpenGL 3.0 (not part of the call)
	static int n_GetARBFramebufferObjectFuncPointers();
	static int n_GetARBMapBufferRangeFuncPointers();
	static int n_GetARBVertexArrayObjectFuncPointers();

	// core in OpenGL 3.1 (not part of the call)
	static int n_GetARBUniformBufferObjectFuncPointers(); 
	static int n_GetARBDrawInstancedFuncPointers();
	static int n_GetARBCopyBufferFuncPointers();

	// core in OpenGL 3.2 (not part of the call)
	static int n_GetARBDrawElementsBaseVertexFuncPointers();
	static int n_GetARBProvokingVertexFuncPointers();
	static int n_GetARBSyncFuncPointers();
	static int n_GetARBTextureMultisampleFuncPointers();
};

#ifdef FIX_GL_FUNCTION_LINKING_CONFLICTS
#define __ConCat__(a,b) a##b
#define GLFuncPtrName(f) __ConCat__(f,_ptr)
#else // FIX_GL_FUNCTION_LINKING_CONFLICTS
#define GLFuncPtrName(f) f
#endif // FIX_GL_FUNCTION_LINKING_CONFLICTS

typedef unsigned int GLenum;
typedef unsigned char GLboolean;
typedef unsigned int GLbitfield;
typedef signed char GLbyte;
typedef short GLshort;
typedef int GLint;
typedef int GLsizei;
typedef unsigned char GLubyte;
typedef unsigned short GLushort;
typedef unsigned int GLuint;
typedef float GLfloat;
typedef float GLclampf;
typedef double GLdouble;
typedef double GLclampd;
typedef void GLvoid;
// base OpenGL data types

typedef ptrdiff_t GLintptr;
typedef ptrdiff_t GLsizeiptr;
// OpenGL 1.5 data types (GL types for handling large vertex buffer objects)

typedef char GLchar;
// OpenGL 2.0 data types (GL type for program/shader text)

typedef ptrdiff_t GLintptrARB;
typedef ptrdiff_t GLsizeiptrARB;
// GL_ARB_vertex_buffer_object data types (GL types for handling large vertex buffer objects)

typedef char GLcharARB;
typedef unsigned int GLhandleARB;
// GL_ARB_shader_objects data types (GL types for handling shader object handles and program/shader text)

typedef unsigned short GLhalfARB;
// GL_ARB_half_float_pixel data types ("half" precision (s10e5) float data in host memory)

typedef unsigned short GLhalfNV;
// GL_NV_half_float data types ("half" precision (s10e5) float data in host memory)

typedef int64_t GLint64EXT;
typedef uint64_t GLuint64EXT;
// GL_EXT_timer_query data types

typedef int64_t GLint64;
typedef uint64_t GLuint64;
typedef struct __GLsync *GLsync;
// GL_ARB_sync data types

typedef unsigned short GLhalf;
// GL_NV_half_float data types

#ifndef APIENTRY
#define ApiEntry
#else // APIENTRY
#define ApiEntry APIENTRY
#endif // APIENTRY
#ifndef GLStaticApi
#define GLStaticApi extern
#endif // GLStaticApi
// defines for OpenGL 1.0 / OpenGL 1.1 (staticaly linked) functions

#if defined(_WIN32) || defined (_WIN64)
#define GLApi __stdcall
#else // _WIN32 || _WIN64
#define GLApi 
#endif // _WIN32 || _WIN64
// defines for newer (dynamicaly linked) functions

//							    --- OpenGL 1.0 ---

#ifndef GL_VERSION_1_0
#define GL_VERSION_1_0 1

#define GLEH_OPENGL_1_0 true

#ifdef __cplusplus
extern "C" {
#endif

GLStaticApi void ApiEntry glCullFace (GLenum);
GLStaticApi void ApiEntry glFrontFace (GLenum);
GLStaticApi void ApiEntry glHint (GLenum, GLenum);
GLStaticApi void ApiEntry glLineWidth (GLfloat);
GLStaticApi void ApiEntry glPointSize (GLfloat);
GLStaticApi void ApiEntry glPolygonMode (GLenum, GLenum);
GLStaticApi void ApiEntry glScissor (GLint, GLint, GLsizei, GLsizei);
GLStaticApi void ApiEntry glTexParameterf (GLenum, GLenum, GLfloat);
GLStaticApi void ApiEntry glTexParameterfv (GLenum, GLenum, const GLfloat *);
GLStaticApi void ApiEntry glTexParameteri (GLenum, GLenum, GLint);
GLStaticApi void ApiEntry glTexParameteriv (GLenum, GLenum, const GLint *);
GLStaticApi void ApiEntry glTexImage1D (GLenum, GLint, GLint, GLsizei, GLint, GLenum, GLenum, const GLvoid *);
GLStaticApi void ApiEntry glTexImage2D (GLenum, GLint, GLint, GLsizei, GLsizei, GLint, GLenum, GLenum, const GLvoid *);
GLStaticApi void ApiEntry glDrawBuffer (GLenum);
GLStaticApi void ApiEntry glClear (GLbitfield);
GLStaticApi void ApiEntry glClearColor (GLclampf, GLclampf, GLclampf, GLclampf);
GLStaticApi void ApiEntry glClearStencil (GLint);
GLStaticApi void ApiEntry glClearDepth (GLclampd);
GLStaticApi void ApiEntry glStencilMask (GLuint);
GLStaticApi void ApiEntry glColorMask (GLboolean, GLboolean, GLboolean, GLboolean);
GLStaticApi void ApiEntry glDepthMask (GLboolean);
GLStaticApi void ApiEntry glDisable (GLenum);
GLStaticApi void ApiEntry glEnable (GLenum);
GLStaticApi void ApiEntry glFinish (void);
GLStaticApi void ApiEntry glFlush (void);
GLStaticApi void ApiEntry glBlendFunc (GLenum, GLenum);
GLStaticApi void ApiEntry glLogicOp (GLenum);
GLStaticApi void ApiEntry glStencilFunc (GLenum, GLint, GLuint);
GLStaticApi void ApiEntry glStencilOp (GLenum, GLenum, GLenum);
GLStaticApi void ApiEntry glDepthFunc (GLenum);
GLStaticApi void ApiEntry glPixelStoref (GLenum, GLfloat);
GLStaticApi void ApiEntry glPixelStorei (GLenum, GLint);
GLStaticApi void ApiEntry glReadBuffer (GLenum);
GLStaticApi void ApiEntry glReadPixels (GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, GLvoid *);
GLStaticApi void ApiEntry glGetBooleanv (GLenum, GLboolean *);
GLStaticApi void ApiEntry glGetDoublev (GLenum, GLdouble *);
GLStaticApi GLenum ApiEntry glGetError (void);
GLStaticApi void ApiEntry glGetFloatv (GLenum, GLfloat *);
GLStaticApi void ApiEntry glGetIntegerv (GLenum, GLint *);
GLStaticApi const GLubyte * ApiEntry glGetString (GLenum);
GLStaticApi void ApiEntry glGetTexImage (GLenum, GLint, GLenum, GLenum, GLvoid *);
GLStaticApi void ApiEntry glGetTexParameterfv (GLenum, GLenum, GLfloat *);
GLStaticApi void ApiEntry glGetTexParameteriv (GLenum, GLenum, GLint *);
GLStaticApi void ApiEntry glGetTexLevelParameterfv (GLenum, GLint, GLenum, GLfloat *);
GLStaticApi void ApiEntry glGetTexLevelParameteriv (GLenum, GLint, GLenum, GLint *);
GLStaticApi GLboolean ApiEntry glIsEnabled (GLenum);
GLStaticApi void ApiEntry glDepthRange (GLclampd, GLclampd);
GLStaticApi void ApiEntry glViewport (GLint, GLint, GLsizei, GLsizei);

#ifdef __cplusplus
}
#endif

#endif // GL_VERSION_1_0

//							    --- OpenGL 1.1 ---

#ifndef GL_VERSION_1_1
#define GL_VERSION_1_1 1

#define GLEH_OPENGL_1_1 true

/* AttribMask */
#define GL_DEPTH_BUFFER_BIT               0x00000100
#define GL_STENCIL_BUFFER_BIT             0x00000400
#define GL_COLOR_BUFFER_BIT               0x00004000
/* Boolean */
#define GL_FALSE                          0
#define GL_TRUE                           1
/* BeginMode */
#define GL_POINTS                         0x0000
#define GL_LINES                          0x0001
#define GL_LINE_LOOP                      0x0002
#define GL_LINE_STRIP                     0x0003
#define GL_TRIANGLES                      0x0004
#define GL_TRIANGLE_STRIP                 0x0005
#define GL_TRIANGLE_FAN                   0x0006
/* AlphaFunction */
#define GL_NEVER                          0x0200
#define GL_LESS                           0x0201
#define GL_EQUAL                          0x0202
#define GL_LEQUAL                         0x0203
#define GL_GREATER                        0x0204
#define GL_NOTEQUAL                       0x0205
#define GL_GEQUAL                         0x0206
#define GL_ALWAYS                         0x0207
/* BlendingFactorDest */
#define GL_ZERO                           0
#define GL_ONE                            1
#define GL_SRC_COLOR                      0x0300
#define GL_ONE_MINUS_SRC_COLOR            0x0301
#define GL_SRC_ALPHA                      0x0302
#define GL_ONE_MINUS_SRC_ALPHA            0x0303
#define GL_DST_ALPHA                      0x0304
#define GL_ONE_MINUS_DST_ALPHA            0x0305
/* BlendingFactorSrc */
#define GL_DST_COLOR                      0x0306
#define GL_ONE_MINUS_DST_COLOR            0x0307
#define GL_SRC_ALPHA_SATURATE             0x0308
/* DrawBufferMode */
#define GL_NONE                           0
#define GL_FRONT_LEFT                     0x0400
#define GL_FRONT_RIGHT                    0x0401
#define GL_BACK_LEFT                      0x0402
#define GL_BACK_RIGHT                     0x0403
#define GL_FRONT                          0x0404
#define GL_BACK                           0x0405
#define GL_LEFT                           0x0406
#define GL_RIGHT                          0x0407
#define GL_FRONT_AND_BACK                 0x0408
/* ErrorCode */
#define GL_NO_ERROR                       0
#define GL_INVALID_ENUM                   0x0500
#define GL_INVALID_VALUE                  0x0501
#define GL_INVALID_OPERATION              0x0502
#define GL_OUT_OF_MEMORY                  0x0505
/* FrontFaceDirection */
#define GL_CW                             0x0900
#define GL_CCW                            0x0901
/* GetPName */
#define GL_POINT_SIZE                     0x0B11
#define GL_POINT_SIZE_RANGE               0x0B12
#define GL_POINT_SIZE_GRANULARITY         0x0B13
#define GL_LINE_SMOOTH                    0x0B20
#define GL_LINE_WIDTH                     0x0B21
#define GL_LINE_WIDTH_RANGE               0x0B22
#define GL_LINE_WIDTH_GRANULARITY         0x0B23
#define GL_POLYGON_SMOOTH                 0x0B41
#define GL_CULL_FACE                      0x0B44
#define GL_CULL_FACE_MODE                 0x0B45
#define GL_FRONT_FACE                     0x0B46
#define GL_DEPTH_RANGE                    0x0B70
#define GL_DEPTH_TEST                     0x0B71
#define GL_DEPTH_WRITEMASK                0x0B72
#define GL_DEPTH_CLEAR_VALUE              0x0B73
#define GL_DEPTH_FUNC                     0x0B74
#define GL_STENCIL_TEST                   0x0B90
#define GL_STENCIL_CLEAR_VALUE            0x0B91
#define GL_STENCIL_FUNC                   0x0B92
#define GL_STENCIL_VALUE_MASK             0x0B93
#define GL_STENCIL_FAIL                   0x0B94
#define GL_STENCIL_PASS_DEPTH_FAIL        0x0B95
#define GL_STENCIL_PASS_DEPTH_PASS        0x0B96
#define GL_STENCIL_REF                    0x0B97
#define GL_STENCIL_WRITEMASK              0x0B98
#define GL_VIEWPORT                       0x0BA2
#define GL_DITHER                         0x0BD0
#define GL_BLEND_DST                      0x0BE0
#define GL_BLEND_SRC                      0x0BE1
#define GL_BLEND                          0x0BE2
#define GL_LOGIC_OP_MODE                  0x0BF0
#define GL_COLOR_LOGIC_OP                 0x0BF2
#define GL_DRAW_BUFFER                    0x0C01
#define GL_READ_BUFFER                    0x0C02
#define GL_SCISSOR_BOX                    0x0C10
#define GL_SCISSOR_TEST                   0x0C11
#define GL_COLOR_CLEAR_VALUE              0x0C22
#define GL_COLOR_WRITEMASK                0x0C23
#define GL_DOUBLEBUFFER                   0x0C32
#define GL_STEREO                         0x0C33
#define GL_LINE_SMOOTH_HINT               0x0C52
#define GL_POLYGON_SMOOTH_HINT            0x0C53
#define GL_UNPACK_SWAP_BYTES              0x0CF0
#define GL_UNPACK_LSB_FIRST               0x0CF1
#define GL_UNPACK_ROW_LENGTH              0x0CF2
#define GL_UNPACK_SKIP_ROWS               0x0CF3
#define GL_UNPACK_SKIP_PIXELS             0x0CF4
#define GL_UNPACK_ALIGNMENT               0x0CF5
#define GL_PACK_SWAP_BYTES                0x0D00
#define GL_PACK_LSB_FIRST                 0x0D01
#define GL_PACK_ROW_LENGTH                0x0D02
#define GL_PACK_SKIP_ROWS                 0x0D03
#define GL_PACK_SKIP_PIXELS               0x0D04
#define GL_PACK_ALIGNMENT                 0x0D05
#define GL_MAX_TEXTURE_SIZE               0x0D33
#define GL_MAX_VIEWPORT_DIMS              0x0D3A
#define GL_SUBPIXEL_BITS                  0x0D50
#define GL_TEXTURE_1D                     0x0DE0
#define GL_TEXTURE_2D                     0x0DE1
#define GL_POLYGON_OFFSET_UNITS           0x2A00
#define GL_POLYGON_OFFSET_POINT           0x2A01
#define GL_POLYGON_OFFSET_LINE            0x2A02
#define GL_POLYGON_OFFSET_FILL            0x8037
#define GL_POLYGON_OFFSET_FACTOR          0x8038
#define GL_TEXTURE_BINDING_1D             0x8068
#define GL_TEXTURE_BINDING_2D             0x8069
/* GetTextureParameter */
#define GL_TEXTURE_WIDTH                  0x1000
#define GL_TEXTURE_HEIGHT                 0x1001
#define GL_TEXTURE_INTERNAL_FORMAT        0x1003
#define GL_TEXTURE_BORDER_COLOR           0x1004
#define GL_TEXTURE_BORDER                 0x1005
#define GL_TEXTURE_RED_SIZE               0x805C
#define GL_TEXTURE_GREEN_SIZE             0x805D
#define GL_TEXTURE_BLUE_SIZE              0x805E
#define GL_TEXTURE_ALPHA_SIZE             0x805F
/* HintMode */
#define GL_DONT_CARE                      0x1100
#define GL_FASTEST                        0x1101
#define GL_NICEST                         0x1102
/* DataType */
#define GL_BYTE                           0x1400
#define GL_UNSIGNED_BYTE                  0x1401
#define GL_SHORT                          0x1402
#define GL_UNSIGNED_SHORT                 0x1403
#define GL_INT                            0x1404
#define GL_UNSIGNED_INT                   0x1405
#define GL_FLOAT                          0x1406
#define GL_DOUBLE                         0x140A
/* LogicOp */
#define GL_CLEAR                          0x1500
#define GL_AND                            0x1501
#define GL_AND_REVERSE                    0x1502
#define GL_COPY                           0x1503
#define GL_AND_INVERTED                   0x1504
#define GL_NOOP                           0x1505
#define GL_XOR                            0x1506
#define GL_OR                             0x1507
#define GL_NOR                            0x1508
#define GL_EQUIV                          0x1509
#define GL_INVERT                         0x150A
#define GL_OR_REVERSE                     0x150B
#define GL_COPY_INVERTED                  0x150C
#define GL_OR_INVERTED                    0x150D
#define GL_NAND                           0x150E
#define GL_SET                            0x150F
/* MatrixMode (for gl3.h, FBO attachment type) */
#define GL_TEXTURE                        0x1702
/* PixelCopyType */
#define GL_COLOR                          0x1800
#define GL_DEPTH                          0x1801
#define GL_STENCIL                        0x1802
/* PixelFormat */
#define GL_STENCIL_INDEX                  0x1901
#define GL_DEPTH_COMPONENT                0x1902
#define GL_RED                            0x1903
#define GL_GREEN                          0x1904
#define GL_BLUE                           0x1905
#define GL_ALPHA                          0x1906
#define GL_RGB                            0x1907
#define GL_RGBA                           0x1908
/* PolygonMode */
#define GL_POINT                          0x1B00
#define GL_LINE                           0x1B01
#define GL_FILL                           0x1B02
/* StencilOp */
#define GL_KEEP                           0x1E00
#define GL_REPLACE                        0x1E01
#define GL_INCR                           0x1E02
#define GL_DECR                           0x1E03
/* StringName */
#define GL_VENDOR                         0x1F00
#define GL_RENDERER                       0x1F01
#define GL_VERSION                        0x1F02
#define GL_EXTENSIONS                     0x1F03
/* TextureMagFilter */
#define GL_NEAREST                        0x2600
#define GL_LINEAR                         0x2601
/* TextureMinFilter */
#define GL_NEAREST_MIPMAP_NEAREST         0x2700
#define GL_LINEAR_MIPMAP_NEAREST          0x2701
#define GL_NEAREST_MIPMAP_LINEAR          0x2702
#define GL_LINEAR_MIPMAP_LINEAR           0x2703
/* TextureParameterName */
#define GL_TEXTURE_MAG_FILTER             0x2800
#define GL_TEXTURE_MIN_FILTER             0x2801
#define GL_TEXTURE_WRAP_S                 0x2802
#define GL_TEXTURE_WRAP_T                 0x2803
/* TextureTarget */
#define GL_PROXY_TEXTURE_1D               0x8063
#define GL_PROXY_TEXTURE_2D               0x8064
/* TextureWrapMode */
#define GL_REPEAT                         0x2901
/* PixelInternalFormat */
#define GL_R3_G3_B2                       0x2A10
#define GL_RGB4                           0x804F
#define GL_RGB5                           0x8050
#define GL_RGB8                           0x8051
#define GL_RGB10                          0x8052
#define GL_RGB12                          0x8053
#define GL_RGB16                          0x8054
#define GL_RGBA2                          0x8055
#define GL_RGBA4                          0x8056
#define GL_RGB5_A1                        0x8057
#define GL_RGBA8                          0x8058
#define GL_RGB10_A2                       0x8059
#define GL_RGBA12                         0x805A
#define GL_RGBA16                         0x805B

#ifdef __cplusplus
extern "C" {
#endif

GLStaticApi void ApiEntry glDrawArrays (GLenum, GLint, GLsizei);
GLStaticApi void ApiEntry glDrawElements (GLenum, GLsizei, GLenum, const GLvoid *);
GLStaticApi void ApiEntry glGetPointerv (GLenum, GLvoid* *);
GLStaticApi void ApiEntry glPolygonOffset (GLfloat, GLfloat);
GLStaticApi void ApiEntry glCopyTexImage1D (GLenum, GLint, GLenum, GLint, GLint, GLsizei, GLint);
GLStaticApi void ApiEntry glCopyTexImage2D (GLenum, GLint, GLenum, GLint, GLint, GLsizei, GLsizei, GLint);
GLStaticApi void ApiEntry glCopyTexSubImage1D (GLenum, GLint, GLint, GLint, GLint, GLsizei);
GLStaticApi void ApiEntry glCopyTexSubImage2D (GLenum, GLint, GLint, GLint, GLint, GLint, GLsizei, GLsizei);
GLStaticApi void ApiEntry glTexSubImage1D (GLenum, GLint, GLint, GLsizei, GLenum, GLenum, const GLvoid *);
GLStaticApi void ApiEntry glTexSubImage2D (GLenum, GLint, GLint, GLint, GLsizei, GLsizei, GLenum, GLenum, const GLvoid *);
GLStaticApi void ApiEntry glBindTexture (GLenum, GLuint);
GLStaticApi void ApiEntry glDeleteTextures (GLsizei, const GLuint *);
GLStaticApi void ApiEntry glGenTextures (GLsizei, GLuint *);
GLStaticApi GLboolean ApiEntry glIsTexture (GLuint);

#ifdef __cplusplus
}
#endif

#endif // GL_VERSION_1_1

//							    --- OpenGL 1.2 ---

#ifndef GL_VERSION_1_2
#define GL_VERSION_1_2 1
#define __GENERATE_GL_12__

extern bool GLEH_OPENGL_1_2; /**< set if GL_OPENGL_1_2 is supported */

#define GL_UNSIGNED_BYTE_3_3_2            0x8032
#define GL_UNSIGNED_SHORT_4_4_4_4         0x8033
#define GL_UNSIGNED_SHORT_5_5_5_1         0x8034
#define GL_UNSIGNED_INT_8_8_8_8           0x8035
#define GL_UNSIGNED_INT_10_10_10_2        0x8036
//#define GL_RESCALE_NORMAL                 0x803A // deprecated
#define GL_TEXTURE_BINDING_3D             0x806A
#define GL_PACK_SKIP_IMAGES               0x806B
#define GL_PACK_IMAGE_HEIGHT              0x806C
#define GL_UNPACK_SKIP_IMAGES             0x806D
#define GL_UNPACK_IMAGE_HEIGHT            0x806E
#define GL_TEXTURE_3D                     0x806F
#define GL_PROXY_TEXTURE_3D               0x8070
#define GL_TEXTURE_DEPTH                  0x8071
#define GL_TEXTURE_WRAP_R                 0x8072
#define GL_MAX_3D_TEXTURE_SIZE            0x8073
#define GL_UNSIGNED_BYTE_2_3_3_REV        0x8362
#define GL_UNSIGNED_SHORT_5_6_5           0x8363
#define GL_UNSIGNED_SHORT_5_6_5_REV       0x8364
#define GL_UNSIGNED_SHORT_4_4_4_4_REV     0x8365
#define GL_UNSIGNED_SHORT_1_5_5_5_REV     0x8366
#define GL_UNSIGNED_INT_8_8_8_8_REV       0x8367
#define GL_UNSIGNED_INT_2_10_10_10_REV    0x8368
#define GL_BGR                            0x80E0
#define GL_BGRA                           0x80E1
#define GL_MAX_ELEMENTS_VERTICES          0x80E8
#define GL_MAX_ELEMENTS_INDICES           0x80E9
#define GL_CLAMP_TO_EDGE                  0x812F
#define GL_TEXTURE_MIN_LOD                0x813A
#define GL_TEXTURE_MAX_LOD                0x813B
#define GL_TEXTURE_BASE_LEVEL             0x813C
#define GL_TEXTURE_MAX_LEVEL              0x813D
//#define GL_LIGHT_MODEL_COLOR_CONTROL      0x81F8 // deprecated
//#define GL_SINGLE_COLOR                   0x81F9 // deprecated
//#define GL_SEPARATE_SPECULAR_COLOR        0x81FA // deprecated
#define GL_SMOOTH_POINT_SIZE_RANGE        0x0B12
#define GL_SMOOTH_POINT_SIZE_GRANULARITY  0x0B13
#define GL_SMOOTH_LINE_WIDTH_RANGE        0x0B22
#define GL_SMOOTH_LINE_WIDTH_GRANULARITY  0x0B23
//#define GL_ALIASED_POINT_SIZE_RANGE       0x846D // deprecated
#define GL_ALIASED_LINE_WIDTH_RANGE       0x846E

extern void (GLApi *GLFuncPtrName(glBlendColor))(GLclampf, GLclampf, GLclampf, GLclampf);
extern void (GLApi *GLFuncPtrName(glBlendEquation))(GLenum);
extern void (GLApi *GLFuncPtrName(glDrawRangeElements))(GLenum, GLuint, GLuint, GLsizei, GLenum, const GLvoid*);
extern void (GLApi *GLFuncPtrName(glTexImage3D))(GLenum, GLint, GLint, GLsizei, GLsizei, GLsizei, GLint, GLenum, GLenum, const GLvoid*);
extern void (GLApi *GLFuncPtrName(glTexSubImage3D))(GLenum, GLint, GLint, GLint, GLint, GLsizei, GLsizei, GLsizei, GLenum, GLenum, const GLvoid*);
extern void (GLApi *GLFuncPtrName(glCopyTexSubImage3D))(GLenum, GLint, GLint, GLint, GLint, GLint, GLint, GLsizei, GLsizei);

#define glBlendColor GLFuncPtrName(glBlendColor)
#define glBlendEquation GLFuncPtrName(glBlendEquation)
#define glDrawRangeElements GLFuncPtrName(glDrawRangeElements)
#define glTexImage3D GLFuncPtrName(glTexImage3D)
#define glTexSubImage3D GLFuncPtrName(glTexSubImage3D)
#define glCopyTexSubImage3D GLFuncPtrName(glCopyTexSubImage3D)

#endif // GL_VERSION_1_2

//							    --- GL_ARB_imaging (part of OpenGL 1.2 core) ---

#ifndef GL_ARB_imaging
#define GL_ARB_imaging 1
#define __GENERATE_GL_ARB_imaging__

extern bool GLEH_ARB_imaging; /**< set if GL_ARB_imaging is supported */

#define GL_CONSTANT_COLOR                 0x8001
#define GL_ONE_MINUS_CONSTANT_COLOR       0x8002
#define GL_CONSTANT_ALPHA                 0x8003
#define GL_ONE_MINUS_CONSTANT_ALPHA       0x8004
#define GL_BLEND_COLOR                    0x8005
#define GL_FUNC_ADD                       0x8006
#define GL_MIN                            0x8007
#define GL_MAX                            0x8008
#define GL_BLEND_EQUATION                 0x8009
#define GL_FUNC_SUBTRACT                  0x800A
#define GL_FUNC_REVERSE_SUBTRACT          0x800B

// it is unclear whether this is deprecated (it seems to be)
/*#define GL_CONVOLUTION_1D                 0x8010
#define GL_CONVOLUTION_2D                 0x8011
#define GL_SEPARABLE_2D                   0x8012
#define GL_CONVOLUTION_BORDER_MODE        0x8013
#define GL_CONVOLUTION_FILTER_SCALE       0x8014
#define GL_CONVOLUTION_FILTER_BIAS        0x8015
#define GL_REDUCE                         0x8016
#define GL_CONVOLUTION_FORMAT             0x8017
#define GL_CONVOLUTION_WIDTH              0x8018
#define GL_CONVOLUTION_HEIGHT             0x8019
#define GL_MAX_CONVOLUTION_WIDTH          0x801A
#define GL_MAX_CONVOLUTION_HEIGHT         0x801B
#define GL_POST_CONVOLUTION_RED_SCALE     0x801C
#define GL_POST_CONVOLUTION_GREEN_SCALE   0x801D
#define GL_POST_CONVOLUTION_BLUE_SCALE    0x801E
#define GL_POST_CONVOLUTION_ALPHA_SCALE   0x801F
#define GL_POST_CONVOLUTION_RED_BIAS      0x8020
#define GL_POST_CONVOLUTION_GREEN_BIAS    0x8021
#define GL_POST_CONVOLUTION_BLUE_BIAS     0x8022
#define GL_POST_CONVOLUTION_ALPHA_BIAS    0x8023
#define GL_HISTOGRAM                      0x8024
#define GL_PROXY_HISTOGRAM                0x8025
#define GL_HISTOGRAM_WIDTH                0x8026
#define GL_HISTOGRAM_FORMAT               0x8027
#define GL_HISTOGRAM_RED_SIZE             0x8028
#define GL_HISTOGRAM_GREEN_SIZE           0x8029
#define GL_HISTOGRAM_BLUE_SIZE            0x802A
#define GL_HISTOGRAM_ALPHA_SIZE           0x802B
#define GL_HISTOGRAM_LUMINANCE_SIZE       0x802C
#define GL_HISTOGRAM_SINK                 0x802D
#define GL_MINMAX                         0x802E
#define GL_MINMAX_FORMAT                  0x802F
#define GL_MINMAX_SINK                    0x8030
#define GL_TABLE_TOO_LARGE                0x8031
#define GL_COLOR_MATRIX                   0x80B1
#define GL_COLOR_MATRIX_STACK_DEPTH       0x80B2
#define GL_MAX_COLOR_MATRIX_STACK_DEPTH   0x80B3
#define GL_POST_COLOR_MATRIX_RED_SCALE    0x80B4
#define GL_POST_COLOR_MATRIX_GREEN_SCALE  0x80B5
#define GL_POST_COLOR_MATRIX_BLUE_SCALE   0x80B6
#define GL_POST_COLOR_MATRIX_ALPHA_SCALE  0x80B7
#define GL_POST_COLOR_MATRIX_RED_BIAS     0x80B8
#define GL_POST_COLOR_MATRIX_GREEN_BIAS   0x80B9
#define GL_POST_COLOR_MATRIX_BLUE_BIAS    0x80BA
#define GL_POST_COLOR_MATRIX_ALPHA_BIAS   0x80BB
#define GL_COLOR_TABLE                    0x80D0
#define GL_POST_CONVOLUTION_COLOR_TABLE   0x80D1
#define GL_POST_COLOR_MATRIX_COLOR_TABLE  0x80D2
#define GL_PROXY_COLOR_TABLE              0x80D3
#define GL_PROXY_POST_CONVOLUTION_COLOR_TABLE 0x80D4
#define GL_PROXY_POST_COLOR_MATRIX_COLOR_TABLE 0x80D5
#define GL_COLOR_TABLE_SCALE              0x80D6
#define GL_COLOR_TABLE_BIAS               0x80D7
#define GL_COLOR_TABLE_FORMAT             0x80D8
#define GL_COLOR_TABLE_WIDTH              0x80D9
#define GL_COLOR_TABLE_RED_SIZE           0x80DA
#define GL_COLOR_TABLE_GREEN_SIZE         0x80DB
#define GL_COLOR_TABLE_BLUE_SIZE          0x80DC
#define GL_COLOR_TABLE_ALPHA_SIZE         0x80DD
#define GL_COLOR_TABLE_LUMINANCE_SIZE     0x80DE
#define GL_COLOR_TABLE_INTENSITY_SIZE     0x80DF
#define GL_CONSTANT_BORDER                0x8151
#define GL_REPLICATE_BORDER               0x8153
#define GL_CONVOLUTION_BORDER_COLOR       0x8154*/

// it is unclear whether this is deprecated (it seems to be)
/*extern void (GLApi *GLFuncPtrName(glColorTable))(GLenum, GLenum, GLsizei, GLenum, GLenum, const GLvoid*);
extern void (GLApi *GLFuncPtrName(glColorTableParameterfv))(GLenum, GLenum, const GLfloat*);
extern void (GLApi *GLFuncPtrName(glColorTableParameteriv))(GLenum, GLenum, const GLint*);
extern void (GLApi *GLFuncPtrName(glCopyColorTable))(GLenum, GLenum, GLint, GLint, GLsizei);
extern void (GLApi *GLFuncPtrName(glGetColorTable))(GLenum, GLenum, GLenum, GLvoid*);
extern void (GLApi *GLFuncPtrName(glGetColorTableParameterfv))(GLenum, GLenum, GLfloat*);
extern void (GLApi *GLFuncPtrName(glGetColorTableParameteriv))(GLenum, GLenum, GLint*);
extern void (GLApi *GLFuncPtrName(glColorSubTable))(GLenum, GLsizei, GLsizei, GLenum, GLenum, const GLvoid*);
extern void (GLApi *GLFuncPtrName(glCopyColorSubTable))(GLenum, GLsizei, GLint, GLint, GLsizei);
extern void (GLApi *GLFuncPtrName(glConvolutionFilter1D))(GLenum, GLenum, GLsizei, GLenum, GLenum, const GLvoid*);
extern void (GLApi *GLFuncPtrName(glConvolutionFilter2D))(GLenum, GLenum, GLsizei, GLsizei, GLenum, GLenum, const GLvoid*);
extern void (GLApi *GLFuncPtrName(glConvolutionParameterf))(GLenum, GLenum, GLfloat);
extern void (GLApi *GLFuncPtrName(glConvolutionParameterfv))(GLenum, GLenum, const GLfloat*);
extern void (GLApi *GLFuncPtrName(glConvolutionParameteri))(GLenum, GLenum, GLint);
extern void (GLApi *GLFuncPtrName(glConvolutionParameteriv))(GLenum, GLenum, const GLint*);
extern void (GLApi *GLFuncPtrName(glCopyConvolutionFilter1D))(GLenum, GLenum, GLint, GLint, GLsizei);
extern void (GLApi *GLFuncPtrName(glCopyConvolutionFilter2D))(GLenum, GLenum, GLint, GLint, GLsizei, GLsizei);
extern void (GLApi *GLFuncPtrName(glGetConvolutionFilter))(GLenum, GLenum, GLenum, GLvoid*);
extern void (GLApi *GLFuncPtrName(glGetConvolutionParameterfv))(GLenum, GLenum, GLfloat*);
extern void (GLApi *GLFuncPtrName(glGetConvolutionParameteriv))(GLenum, GLenum, GLint*);
extern void (GLApi *GLFuncPtrName(glGetSeparableFilter))(GLenum, GLenum, GLenum, GLvoid *, GLvoid *, GLvoid*);
extern void (GLApi *GLFuncPtrName(glSeparableFilter2D))(GLenum, GLenum, GLsizei, GLsizei, GLenum, GLenum, const GLvoid *, const GLvoid*);
extern void (GLApi *GLFuncPtrName(glGetHistogram))(GLenum, GLboolean, GLenum, GLenum, GLvoid*);
extern void (GLApi *GLFuncPtrName(glGetHistogramParameterfv))(GLenum, GLenum, GLfloat*);
extern void (GLApi *GLFuncPtrName(glGetHistogramParameteriv))(GLenum, GLenum, GLint*);
extern void (GLApi *GLFuncPtrName(glGetMinmax))(GLenum, GLboolean, GLenum, GLenum, GLvoid*);
extern void (GLApi *GLFuncPtrName(glGetMinmaxParameterfv))(GLenum, GLenum, GLfloat*);
extern void (GLApi *GLFuncPtrName(glGetMinmaxParameteriv))(GLenum, GLenum, GLint*);
extern void (GLApi *GLFuncPtrName(glHistogram))(GLenum, GLsizei, GLenum, GLboolean);
extern void (GLApi *GLFuncPtrName(glMinmax))(GLenum, GLenum, GLboolean);
extern void (GLApi *GLFuncPtrName(glResetHistogram))(GLenum);
extern void (GLApi *GLFuncPtrName(glResetMinmax))(GLenum);*/

// it is unclear whether this is deprecated (it seems to be)
/*#define glColorTable GLFuncPtrName(glColorTable)
#define glColorTableParameterfv GLFuncPtrName(glColorTableParameterfv)
#define glColorTableParameteriv GLFuncPtrName(glColorTableParameteriv)
#define glCopyColorTable GLFuncPtrName(glCopyColorTable)
#define glGetColorTable GLFuncPtrName(glGetColorTable)
#define glGetColorTableParameterfv GLFuncPtrName(glGetColorTableParameterfv)
#define glGetColorTableParameteriv GLFuncPtrName(glGetColorTableParameteriv)
#define glColorSubTable GLFuncPtrName(glColorSubTable)
#define glCopyColorSubTable GLFuncPtrName(glCopyColorSubTable)
#define glConvolutionFilter1D GLFuncPtrName(glConvolutionFilter1D)
#define glConvolutionFilter2D GLFuncPtrName(glConvolutionFilter2D)
#define glConvolutionParameterf GLFuncPtrName(glConvolutionParameterf)
#define glConvolutionParameterfv GLFuncPtrName(glConvolutionParameterfv)
#define glConvolutionParameteri GLFuncPtrName(glConvolutionParameteri)
#define glConvolutionParameteriv GLFuncPtrName(glConvolutionParameteriv)
#define glCopyConvolutionFilter1D GLFuncPtrName(glCopyConvolutionFilter1D)
#define glCopyConvolutionFilter2D GLFuncPtrName(glCopyConvolutionFilter2D)
#define glGetConvolutionFilter GLFuncPtrName(glGetConvolutionFilter)
#define glGetConvolutionParameterfv GLFuncPtrName(glGetConvolutionParameterfv)
#define glGetConvolutionParameteriv GLFuncPtrName(glGetConvolutionParameteriv)
#define glGetSeparableFilter GLFuncPtrName(glGetSeparableFilter)
#define glSeparableFilter2D GLFuncPtrName(glSeparableFilter2D)
#define glGetHistogram GLFuncPtrName(glGetHistogram)
#define glGetHistogramParameterfv GLFuncPtrName(glGetHistogramParameterfv)
#define glGetHistogramParameteriv GLFuncPtrName(glGetHistogramParameteriv)
#define glGetMinmax GLFuncPtrName(glGetMinmax)
#define glGetMinmaxParameterfv GLFuncPtrName(glGetMinmaxParameterfv)
#define glGetMinmaxParameteriv GLFuncPtrName(glGetMinmaxParameteriv)
#define glHistogram GLFuncPtrName(glHistogram)
#define glMinmax GLFuncPtrName(glMinmax)
#define glResetHistogram GLFuncPtrName(glResetHistogram)
#define glResetMinmax GLFuncPtrName(glResetMinmax)*/

#endif // GL_ARB_imaging

//							    --- OpenGL 1.3 ---

#ifndef GL_VERSION_1_3
#define GL_VERSION_1_3 1
#define __GENERATE_GL_13__

extern bool GLEH_OPENGL_1_3; /**< set if GL_OPENGL_1_3 is supported */

#define GL_TEXTURE0                       0x84C0
#define GL_TEXTURE1                       0x84C1
#define GL_TEXTURE2                       0x84C2
#define GL_TEXTURE3                       0x84C3
#define GL_TEXTURE4                       0x84C4
#define GL_TEXTURE5                       0x84C5
#define GL_TEXTURE6                       0x84C6
#define GL_TEXTURE7                       0x84C7
#define GL_TEXTURE8                       0x84C8
#define GL_TEXTURE9                       0x84C9
#define GL_TEXTURE10                      0x84CA
#define GL_TEXTURE11                      0x84CB
#define GL_TEXTURE12                      0x84CC
#define GL_TEXTURE13                      0x84CD
#define GL_TEXTURE14                      0x84CE
#define GL_TEXTURE15                      0x84CF
#define GL_TEXTURE16                      0x84D0
#define GL_TEXTURE17                      0x84D1
#define GL_TEXTURE18                      0x84D2
#define GL_TEXTURE19                      0x84D3
#define GL_TEXTURE20                      0x84D4
#define GL_TEXTURE21                      0x84D5
#define GL_TEXTURE22                      0x84D6
#define GL_TEXTURE23                      0x84D7
#define GL_TEXTURE24                      0x84D8
#define GL_TEXTURE25                      0x84D9
#define GL_TEXTURE26                      0x84DA
#define GL_TEXTURE27                      0x84DB
#define GL_TEXTURE28                      0x84DC
#define GL_TEXTURE29                      0x84DD
#define GL_TEXTURE30                      0x84DE
#define GL_TEXTURE31                      0x84DF
#define GL_ACTIVE_TEXTURE                 0x84E0
//#define GL_CLIENT_ACTIVE_TEXTURE          0x84E1 // deprecated
//#define GL_MAX_TEXTURE_UNITS              0x84E2 // deprecated
//#define GL_TRANSPOSE_MODELVIEW_MATRIX     0x84E3 // deprecated
//#define GL_TRANSPOSE_PROJECTION_MATRIX    0x84E4 // deprecated
//#define GL_TRANSPOSE_TEXTURE_MATRIX       0x84E5 // deprecated
//#define GL_TRANSPOSE_COLOR_MATRIX         0x84E6 // deprecated
#define GL_MULTISAMPLE                    0x809D
#define GL_SAMPLE_ALPHA_TO_COVERAGE       0x809E
#define GL_SAMPLE_ALPHA_TO_ONE            0x809F
#define GL_SAMPLE_COVERAGE                0x80A0
#define GL_SAMPLE_BUFFERS                 0x80A8
#define GL_SAMPLES                        0x80A9
#define GL_SAMPLE_COVERAGE_VALUE          0x80AA
#define GL_SAMPLE_COVERAGE_INVERT         0x80AB
//#define GL_MULTISAMPLE_BIT                0x20000000 // deprecated
//#define GL_NORMAL_MAP                     0x8511 // deprecated
//#define GL_REFLECTION_MAP                 0x8512 // deprecated
#define GL_TEXTURE_CUBE_MAP               0x8513
#define GL_TEXTURE_BINDING_CUBE_MAP       0x8514
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X    0x8515
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X    0x8516
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y    0x8517
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y    0x8518
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z    0x8519
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z    0x851A
#define GL_PROXY_TEXTURE_CUBE_MAP         0x851B
#define GL_MAX_CUBE_MAP_TEXTURE_SIZE      0x851C
//#define GL_COMPRESSED_ALPHA               0x84E9 // deprecated
//#define GL_COMPRESSED_LUMINANCE           0x84EA // deprecated
//#define GL_COMPRESSED_LUMINANCE_ALPHA     0x84EB // deprecated
//#define GL_COMPRESSED_INTENSITY           0x84EC // deprecated
#define GL_COMPRESSED_RGB                 0x84ED
#define GL_COMPRESSED_RGBA                0x84EE
#define GL_TEXTURE_COMPRESSION_HINT       0x84EF
#define GL_TEXTURE_COMPRESSED_IMAGE_SIZE  0x86A0
#define GL_TEXTURE_COMPRESSED             0x86A1
#define GL_NUM_COMPRESSED_TEXTURE_FORMATS 0x86A2
#define GL_COMPRESSED_TEXTURE_FORMATS     0x86A3
#define GL_CLAMP_TO_BORDER                0x812D
//#define GL_COMBINE                        0x8570 // deprecated
//#define GL_COMBINE_RGB                    0x8571 // deprecated
//#define GL_COMBINE_ALPHA                  0x8572 // deprecated
//#define GL_SOURCE0_RGB                    0x8580 // deprecated
//#define GL_SOURCE1_RGB                    0x8581 // deprecated
//#define GL_SOURCE2_RGB                    0x8582 // deprecated
//#define GL_SOURCE0_ALPHA                  0x8588 // deprecated
//#define GL_SOURCE1_ALPHA                  0x8589 // deprecated
//#define GL_SOURCE2_ALPHA                  0x858A // deprecated
//#define GL_OPERAND0_RGB                   0x8590 // deprecated
//#define GL_OPERAND1_RGB                   0x8591 // deprecated
//#define GL_OPERAND2_RGB                   0x8592 // deprecated
//#define GL_OPERAND0_ALPHA                 0x8598 // deprecated
//#define GL_OPERAND1_ALPHA                 0x8599 // deprecated
//#define GL_OPERAND2_ALPHA                 0x859A // deprecated
//#define GL_RGB_SCALE                      0x8573 // deprecated
//#define GL_ADD_SIGNED                     0x8574 // deprecated
//#define GL_INTERPOLATE                    0x8575 // deprecated
//#define GL_SUBTRACT                       0x84E7 // deprecated
//#define GL_CONSTANT                       0x8576 // deprecated
//#define GL_PRIMARY_COLOR                  0x8577 // deprecated
//#define GL_PREVIOUS                       0x8578 // deprecated
//#define GL_DOT3_RGB                       0x86AE // deprecated
//#define GL_DOT3_RGBA                      0x86AF // deprecated

extern void (GLApi *GLFuncPtrName(glActiveTexture))(GLenum);
extern void (GLApi *GLFuncPtrName(glSampleCoverage))(GLclampf, GLboolean);
extern void (GLApi *GLFuncPtrName(glCompressedTexImage3D))(GLenum, GLint, GLenum, GLsizei, GLsizei, GLsizei, GLint, GLsizei, const GLvoid*);
extern void (GLApi *GLFuncPtrName(glCompressedTexImage2D))(GLenum, GLint, GLenum, GLsizei, GLsizei, GLint, GLsizei, const GLvoid*);
extern void (GLApi *GLFuncPtrName(glCompressedTexImage1D))(GLenum, GLint, GLenum, GLsizei, GLint, GLsizei, const GLvoid*);
extern void (GLApi *GLFuncPtrName(glCompressedTexSubImage3D))(GLenum, GLint, GLint, GLint, GLint, GLsizei, GLsizei, GLsizei, GLenum, GLsizei, const GLvoid*);
extern void (GLApi *GLFuncPtrName(glCompressedTexSubImage2D))(GLenum, GLint, GLint, GLint, GLsizei, GLsizei, GLenum, GLsizei, const GLvoid*);
extern void (GLApi *GLFuncPtrName(glCompressedTexSubImage1D))(GLenum, GLint, GLint, GLsizei, GLenum, GLsizei, const GLvoid*);
extern void (GLApi *GLFuncPtrName(glGetCompressedTexImage))(GLenum, GLint, GLvoid*);

// functions below are deprecated
/*extern void (GLApi *GLFuncPtrName(glLoadTransposeMatrixf))(const GLfloat*);
extern void (GLApi *GLFuncPtrName(glLoadTransposeMatrixd))(const GLdouble*);
extern void (GLApi *GLFuncPtrName(glMultTransposeMatrixf))(const GLfloat*);
extern void (GLApi *GLFuncPtrName(glMultTransposeMatrixd))(const GLdouble*);
extern void (GLApi *GLFuncPtrName(glClientActiveTexture))(GLenum);
extern void (GLApi *GLFuncPtrName(glMultiTexCoord1d))(GLenum, GLdouble);
extern void (GLApi *GLFuncPtrName(glMultiTexCoord1dv))(GLenum, const GLdouble*);
extern void (GLApi *GLFuncPtrName(glMultiTexCoord1f))(GLenum, GLfloat);
extern void (GLApi *GLFuncPtrName(glMultiTexCoord1fv))(GLenum, const GLfloat*);
extern void (GLApi *GLFuncPtrName(glMultiTexCoord1i))(GLenum, GLint);
extern void (GLApi *GLFuncPtrName(glMultiTexCoord1iv))(GLenum, const GLint*);
extern void (GLApi *GLFuncPtrName(glMultiTexCoord1s))(GLenum, GLshort);
extern void (GLApi *GLFuncPtrName(glMultiTexCoord1sv))(GLenum, const GLshort*);
extern void (GLApi *GLFuncPtrName(glMultiTexCoord2d))(GLenum, GLdouble, GLdouble);
extern void (GLApi *GLFuncPtrName(glMultiTexCoord2dv))(GLenum, const GLdouble*);
extern void (GLApi *GLFuncPtrName(glMultiTexCoord2f))(GLenum, GLfloat, GLfloat);
extern void (GLApi *GLFuncPtrName(glMultiTexCoord2fv))(GLenum, const GLfloat*);
extern void (GLApi *GLFuncPtrName(glMultiTexCoord2i))(GLenum, GLint, GLint);
extern void (GLApi *GLFuncPtrName(glMultiTexCoord2iv))(GLenum, const GLint*);
extern void (GLApi *GLFuncPtrName(glMultiTexCoord2s))(GLenum, GLshort, GLshort);
extern void (GLApi *GLFuncPtrName(glMultiTexCoord2sv))(GLenum, const GLshort*);
extern void (GLApi *GLFuncPtrName(glMultiTexCoord3d))(GLenum, GLdouble, GLdouble, GLdouble);
extern void (GLApi *GLFuncPtrName(glMultiTexCoord3dv))(GLenum, const GLdouble*);
extern void (GLApi *GLFuncPtrName(glMultiTexCoord3f))(GLenum, GLfloat, GLfloat, GLfloat);
extern void (GLApi *GLFuncPtrName(glMultiTexCoord3fv))(GLenum, const GLfloat*);
extern void (GLApi *GLFuncPtrName(glMultiTexCoord3i))(GLenum, GLint, GLint, GLint);
extern void (GLApi *GLFuncPtrName(glMultiTexCoord3iv))(GLenum, const GLint*);
extern void (GLApi *GLFuncPtrName(glMultiTexCoord3s))(GLenum, GLshort, GLshort, GLshort);
extern void (GLApi *GLFuncPtrName(glMultiTexCoord3sv))(GLenum, const GLshort*);
extern void (GLApi *GLFuncPtrName(glMultiTexCoord4d))(GLenum, GLdouble, GLdouble, GLdouble, GLdouble);
extern void (GLApi *GLFuncPtrName(glMultiTexCoord4dv))(GLenum, const GLdouble*);
extern void (GLApi *GLFuncPtrName(glMultiTexCoord4f))(GLenum, GLfloat, GLfloat, GLfloat, GLfloat);
extern void (GLApi *GLFuncPtrName(glMultiTexCoord4fv))(GLenum, const GLfloat*);
extern void (GLApi *GLFuncPtrName(glMultiTexCoord4i))(GLenum, GLint, GLint, GLint, GLint);
extern void (GLApi *GLFuncPtrName(glMultiTexCoord4iv))(GLenum, const GLint*);
extern void (GLApi *GLFuncPtrName(glMultiTexCoord4s))(GLenum, GLshort, GLshort, GLshort, GLshort);
extern void (GLApi *GLFuncPtrName(glMultiTexCoord4sv))(GLenum, const GLshort*);*/

#define glActiveTexture GLFuncPtrName(glActiveTexture)
#define glSampleCoverage GLFuncPtrName(glSampleCoverage)
#define glCompressedTexImage3D GLFuncPtrName(glCompressedTexImage3D)
#define glCompressedTexImage2D GLFuncPtrName(glCompressedTexImage2D)
#define glCompressedTexImage1D GLFuncPtrName(glCompressedTexImage1D)
#define glCompressedTexSubImage3D GLFuncPtrName(glCompressedTexSubImage3D)
#define glCompressedTexSubImage2D GLFuncPtrName(glCompressedTexSubImage2D)
#define glCompressedTexSubImage1D GLFuncPtrName(glCompressedTexSubImage1D)
#define glGetCompressedTexImage GLFuncPtrName(glGetCompressedTexImage)

// functions below are deprecated
/*#define glLoadTransposeMatrixf GLFuncPtrName(glLoadTransposeMatrixf)
#define glLoadTransposeMatrixd GLFuncPtrName(glLoadTransposeMatrixd)
#define glMultTransposeMatrixf GLFuncPtrName(glMultTransposeMatrixf)
#define glMultTransposeMatrixd GLFuncPtrName(glMultTransposeMatrixd)
#define glClientActiveTexture GLFuncPtrName(glClientActiveTexture)
#define glMultiTexCoord1d GLFuncPtrName(glMultiTexCoord1d)
#define glMultiTexCoord1dv GLFuncPtrName(glMultiTexCoord1dv)
#define glMultiTexCoord1f GLFuncPtrName(glMultiTexCoord1f)
#define glMultiTexCoord1fv GLFuncPtrName(glMultiTexCoord1fv)
#define glMultiTexCoord1i GLFuncPtrName(glMultiTexCoord1i)
#define glMultiTexCoord1iv GLFuncPtrName(glMultiTexCoord1iv)
#define glMultiTexCoord1s GLFuncPtrName(glMultiTexCoord1s)
#define glMultiTexCoord1sv GLFuncPtrName(glMultiTexCoord1sv)
#define glMultiTexCoord2d GLFuncPtrName(glMultiTexCoord2d)
#define glMultiTexCoord2dv GLFuncPtrName(glMultiTexCoord2dv)
#define glMultiTexCoord2f GLFuncPtrName(glMultiTexCoord2f)
#define glMultiTexCoord2fv GLFuncPtrName(glMultiTexCoord2fv)
#define glMultiTexCoord2i GLFuncPtrName(glMultiTexCoord2i)
#define glMultiTexCoord2iv GLFuncPtrName(glMultiTexCoord2iv)
#define glMultiTexCoord2s GLFuncPtrName(glMultiTexCoord2s)
#define glMultiTexCoord2sv GLFuncPtrName(glMultiTexCoord2sv)
#define glMultiTexCoord3d GLFuncPtrName(glMultiTexCoord3d)
#define glMultiTexCoord3dv GLFuncPtrName(glMultiTexCoord3dv)
#define glMultiTexCoord3f GLFuncPtrName(glMultiTexCoord3f)
#define glMultiTexCoord3fv GLFuncPtrName(glMultiTexCoord3fv)
#define glMultiTexCoord3i GLFuncPtrName(glMultiTexCoord3i)
#define glMultiTexCoord3iv GLFuncPtrName(glMultiTexCoord3iv)
#define glMultiTexCoord3s GLFuncPtrName(glMultiTexCoord3s)
#define glMultiTexCoord3sv GLFuncPtrName(glMultiTexCoord3sv)
#define glMultiTexCoord4d GLFuncPtrName(glMultiTexCoord4d)
#define glMultiTexCoord4dv GLFuncPtrName(glMultiTexCoord4dv)
#define glMultiTexCoord4f GLFuncPtrName(glMultiTexCoord4f)
#define glMultiTexCoord4fv GLFuncPtrName(glMultiTexCoord4fv)
#define glMultiTexCoord4i GLFuncPtrName(glMultiTexCoord4i)
#define glMultiTexCoord4iv GLFuncPtrName(glMultiTexCoord4iv)
#define glMultiTexCoord4s GLFuncPtrName(glMultiTexCoord4s)
#define glMultiTexCoord4sv GLFuncPtrName(glMultiTexCoord4sv)*/

#endif // GL_VERSION_1_3

//							    --- OpenGL 1.4 ---

#ifndef GL_VERSION_1_4
#define GL_VERSION_1_4 1
#define __GENERATE_GL_14__

extern bool GLEH_OPENGL_1_4; /**< set if GL_OPENGL_1_4 is supported */

#define GL_BLEND_DST_RGB                  0x80C8
#define GL_BLEND_SRC_RGB                  0x80C9
#define GL_BLEND_DST_ALPHA                0x80CA
#define GL_BLEND_SRC_ALPHA                0x80CB
//#define GL_POINT_SIZE_MIN                 0x8126 // deprecated
//#define GL_POINT_SIZE_MAX                 0x8127 // deprecated
#define GL_POINT_FADE_THRESHOLD_SIZE      0x8128
//#define GL_POINT_DISTANCE_ATTENUATION     0x8129 // deprecated
//#define GL_GENERATE_MIPMAP                0x8191 // deprecated
//#define GL_GENERATE_MIPMAP_HINT           0x8192 // deprecated
#define GL_DEPTH_COMPONENT16              0x81A5
#define GL_DEPTH_COMPONENT24              0x81A6
#define GL_DEPTH_COMPONENT32              0x81A7
#define GL_MIRRORED_REPEAT                0x8370
//#define GL_FOG_COORDINATE_SOURCE          0x8450 // deprecated
//#define GL_FOG_COORDINATE                 0x8451 // deprecated
//#define GL_FRAGMENT_DEPTH                 0x8452 // deprecated
//#define GL_CURRENT_FOG_COORDINATE         0x8453 // deprecated
//#define GL_FOG_COORDINATE_ARRAY_TYPE      0x8454 // deprecated
//#define GL_FOG_COORDINATE_ARRAY_STRIDE    0x8455 // deprecated
//#define GL_FOG_COORDINATE_ARRAY_POINTER   0x8456 // deprecated
//#define GL_FOG_COORDINATE_ARRAY           0x8457 // deprecated
//#define GL_COLOR_SUM                      0x8458 // deprecated
//#define GL_CURRENT_SECONDARY_COLOR        0x8459 // deprecated
//#define GL_SECONDARY_COLOR_ARRAY_SIZE     0x845A // deprecated
//#define GL_SECONDARY_COLOR_ARRAY_TYPE     0x845B // deprecated
//#define GL_SECONDARY_COLOR_ARRAY_STRIDE   0x845C // deprecated
//#define GL_SECONDARY_COLOR_ARRAY_POINTER  0x845D // deprecated
//#define GL_SECONDARY_COLOR_ARRAY          0x845E // deprecated
#define GL_MAX_TEXTURE_LOD_BIAS           0x84FD
//#define GL_TEXTURE_FILTER_CONTROL         0x8500 // deprecated
#define GL_TEXTURE_LOD_BIAS               0x8501
#define GL_INCR_WRAP                      0x8507
#define GL_DECR_WRAP                      0x8508
#define GL_TEXTURE_DEPTH_SIZE             0x884A
//#define GL_DEPTH_TEXTURE_MODE             0x884B // deprecated
#define GL_TEXTURE_COMPARE_MODE           0x884C
#define GL_TEXTURE_COMPARE_FUNC           0x884D
//#define GL_COMPARE_R_TO_TEXTURE           0x884E // deprecated

extern void (GLApi *GLFuncPtrName(glBlendFuncSeparate))(GLenum, GLenum, GLenum, GLenum);

// functions below are deprecated
/*extern void (GLApi *GLFuncPtrName(glFogCoordf))(GLfloat);
extern void (GLApi *GLFuncPtrName(glFogCoordfv))(const GLfloat*);
extern void (GLApi *GLFuncPtrName(glFogCoordd))(GLdouble);
extern void (GLApi *GLFuncPtrName(glFogCoorddv))(const GLdouble*);
extern void (GLApi *GLFuncPtrName(glFogCoordPointer))(GLenum, GLsizei, const GLvoid*);*/

extern void (GLApi *GLFuncPtrName(glMultiDrawArrays))(GLenum, GLint *, GLsizei *, GLsizei);
extern void (GLApi *GLFuncPtrName(glMultiDrawElements))(GLenum, const GLsizei *, GLenum, const GLvoid* *, GLsizei);
extern void (GLApi *GLFuncPtrName(glPointParameterf))(GLenum, GLfloat);
extern void (GLApi *GLFuncPtrName(glPointParameterfv))(GLenum, const GLfloat*);
extern void (GLApi *GLFuncPtrName(glPointParameteri))(GLenum, GLint);
extern void (GLApi *GLFuncPtrName(glPointParameteriv))(GLenum, const GLint*);

// functions below are deprecated
/*extern void (GLApi *GLFuncPtrName(glSecondaryColor3b))(GLbyte, GLbyte, GLbyte);
extern void (GLApi *GLFuncPtrName(glSecondaryColor3bv))(const GLbyte*);
extern void (GLApi *GLFuncPtrName(glSecondaryColor3d))(GLdouble, GLdouble, GLdouble);
extern void (GLApi *GLFuncPtrName(glSecondaryColor3dv))(const GLdouble*);
extern void (GLApi *GLFuncPtrName(glSecondaryColor3f))(GLfloat, GLfloat, GLfloat);
extern void (GLApi *GLFuncPtrName(glSecondaryColor3fv))(const GLfloat*);
extern void (GLApi *GLFuncPtrName(glSecondaryColor3i))(GLint, GLint, GLint);
extern void (GLApi *GLFuncPtrName(glSecondaryColor3iv))(const GLint*);
extern void (GLApi *GLFuncPtrName(glSecondaryColor3s))(GLshort, GLshort, GLshort);
extern void (GLApi *GLFuncPtrName(glSecondaryColor3sv))(const GLshort*);
extern void (GLApi *GLFuncPtrName(glSecondaryColor3ub))(GLubyte, GLubyte, GLubyte);
extern void (GLApi *GLFuncPtrName(glSecondaryColor3ubv))(const GLubyte*);
extern void (GLApi *GLFuncPtrName(glSecondaryColor3ui))(GLuint, GLuint, GLuint);
extern void (GLApi *GLFuncPtrName(glSecondaryColor3uiv))(const GLuint*);
extern void (GLApi *GLFuncPtrName(glSecondaryColor3us))(GLushort, GLushort, GLushort);
extern void (GLApi *GLFuncPtrName(glSecondaryColor3usv))(const GLushort*);
extern void (GLApi *GLFuncPtrName(glSecondaryColorPointer))(GLint, GLenum, GLsizei, const GLvoid*);
extern void (GLApi *GLFuncPtrName(glWindowPos2d))(GLdouble, GLdouble);
extern void (GLApi *GLFuncPtrName(glWindowPos2dv))(const GLdouble*);
extern void (GLApi *GLFuncPtrName(glWindowPos2f))(GLfloat, GLfloat);
extern void (GLApi *GLFuncPtrName(glWindowPos2fv))(const GLfloat*);
extern void (GLApi *GLFuncPtrName(glWindowPos2i))(GLint, GLint);
extern void (GLApi *GLFuncPtrName(glWindowPos2iv))(const GLint*);
extern void (GLApi *GLFuncPtrName(glWindowPos2s))(GLshort, GLshort);
extern void (GLApi *GLFuncPtrName(glWindowPos2sv))(const GLshort*);
extern void (GLApi *GLFuncPtrName(glWindowPos3d))(GLdouble, GLdouble, GLdouble);
extern void (GLApi *GLFuncPtrName(glWindowPos3dv))(const GLdouble*);
extern void (GLApi *GLFuncPtrName(glWindowPos3f))(GLfloat, GLfloat, GLfloat);
extern void (GLApi *GLFuncPtrName(glWindowPos3fv))(const GLfloat*);
extern void (GLApi *GLFuncPtrName(glWindowPos3i))(GLint, GLint, GLint);
extern void (GLApi *GLFuncPtrName(glWindowPos3iv))(const GLint*);
extern void (GLApi *GLFuncPtrName(glWindowPos3s))(GLshort, GLshort, GLshort);
extern void (GLApi *GLFuncPtrName(glWindowPos3sv))(const GLshort*);*/

#define glBlendFuncSeparate GLFuncPtrName(glBlendFuncSeparate)

// functions below are deprecated
/*#define glFogCoordf GLFuncPtrName(glFogCoordf)
#define glFogCoordfv GLFuncPtrName(glFogCoordfv)
#define glFogCoordd GLFuncPtrName(glFogCoordd)
#define glFogCoorddv GLFuncPtrName(glFogCoorddv)
#define glFogCoordPointer GLFuncPtrName(glFogCoordPointer)*/

#define glMultiDrawArrays GLFuncPtrName(glMultiDrawArrays)
#define glMultiDrawElements GLFuncPtrName(glMultiDrawElements)
#define glPointParameterf GLFuncPtrName(glPointParameterf)
#define glPointParameterfv GLFuncPtrName(glPointParameterfv)
#define glPointParameteri GLFuncPtrName(glPointParameteri)
#define glPointParameteriv GLFuncPtrName(glPointParameteriv)

// functions below are deprecated
/*#define glSecondaryColor3b GLFuncPtrName(glSecondaryColor3b)
#define glSecondaryColor3bv GLFuncPtrName(glSecondaryColor3bv)
#define glSecondaryColor3d GLFuncPtrName(glSecondaryColor3d)
#define glSecondaryColor3dv GLFuncPtrName(glSecondaryColor3dv)
#define glSecondaryColor3f GLFuncPtrName(glSecondaryColor3f)
#define glSecondaryColor3fv GLFuncPtrName(glSecondaryColor3fv)
#define glSecondaryColor3i GLFuncPtrName(glSecondaryColor3i)
#define glSecondaryColor3iv GLFuncPtrName(glSecondaryColor3iv)
#define glSecondaryColor3s GLFuncPtrName(glSecondaryColor3s)
#define glSecondaryColor3sv GLFuncPtrName(glSecondaryColor3sv)
#define glSecondaryColor3ub GLFuncPtrName(glSecondaryColor3ub)
#define glSecondaryColor3ubv GLFuncPtrName(glSecondaryColor3ubv)
#define glSecondaryColor3ui GLFuncPtrName(glSecondaryColor3ui)
#define glSecondaryColor3uiv GLFuncPtrName(glSecondaryColor3uiv)
#define glSecondaryColor3us GLFuncPtrName(glSecondaryColor3us)
#define glSecondaryColor3usv GLFuncPtrName(glSecondaryColor3usv)
#define glSecondaryColorPointer GLFuncPtrName(glSecondaryColorPointer)
#define glWindowPos2d GLFuncPtrName(glWindowPos2d)
#define glWindowPos2dv GLFuncPtrName(glWindowPos2dv)
#define glWindowPos2f GLFuncPtrName(glWindowPos2f)
#define glWindowPos2fv GLFuncPtrName(glWindowPos2fv)
#define glWindowPos2i GLFuncPtrName(glWindowPos2i)
#define glWindowPos2iv GLFuncPtrName(glWindowPos2iv)
#define glWindowPos2s GLFuncPtrName(glWindowPos2s)
#define glWindowPos2sv GLFuncPtrName(glWindowPos2sv)
#define glWindowPos3d GLFuncPtrName(glWindowPos3d)
#define glWindowPos3dv GLFuncPtrName(glWindowPos3dv)
#define glWindowPos3f GLFuncPtrName(glWindowPos3f)
#define glWindowPos3fv GLFuncPtrName(glWindowPos3fv)
#define glWindowPos3i GLFuncPtrName(glWindowPos3i)
#define glWindowPos3iv GLFuncPtrName(glWindowPos3iv)
#define glWindowPos3s GLFuncPtrName(glWindowPos3s)
#define glWindowPos3sv GLFuncPtrName(glWindowPos3sv)*/

#endif // GL_VERSION_1_4

//							    --- OpenGL 1.5 ---

#ifndef GL_VERSION_1_5
#define GL_VERSION_1_5 1
#define __GENERATE_GL_15__

extern bool GLEH_OPENGL_1_5; /**< set if GL_OPENGL_1_5 is supported */

#define GL_BUFFER_SIZE                    0x8764
#define GL_BUFFER_USAGE                   0x8765
#define GL_QUERY_COUNTER_BITS             0x8864
#define GL_CURRENT_QUERY                  0x8865
#define GL_QUERY_RESULT                   0x8866
#define GL_QUERY_RESULT_AVAILABLE         0x8867
#define GL_ARRAY_BUFFER                   0x8892
#define GL_ELEMENT_ARRAY_BUFFER           0x8893
#define GL_ARRAY_BUFFER_BINDING           0x8894
#define GL_ELEMENT_ARRAY_BUFFER_BINDING   0x8895
//#define GL_VERTEX_ARRAY_BUFFER_BINDING    0x8896 // deprecated
//#define GL_NORMAL_ARRAY_BUFFER_BINDING    0x8897 // deprecated
//#define GL_COLOR_ARRAY_BUFFER_BINDING     0x8898 // deprecated
//#define GL_INDEX_ARRAY_BUFFER_BINDING     0x8899 // deprecated
//#define GL_TEXTURE_COORD_ARRAY_BUFFER_BINDING 0x889A // deprecated
//#define GL_EDGE_FLAG_ARRAY_BUFFER_BINDING 0x889B // deprecated
//#define GL_SECONDARY_COLOR_ARRAY_BUFFER_BINDING 0x889C // deprecated
//#define GL_FOG_COORDINATE_ARRAY_BUFFER_BINDING 0x889D // deprecated
//#define GL_WEIGHT_ARRAY_BUFFER_BINDING    0x889E // deprecated
#define GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING 0x889F
#define GL_READ_ONLY                      0x88B8
#define GL_WRITE_ONLY                     0x88B9
#define GL_READ_WRITE                     0x88BA
#define GL_BUFFER_ACCESS                  0x88BB
#define GL_BUFFER_MAPPED                  0x88BC
#define GL_BUFFER_MAP_POINTER             0x88BD
#define GL_STREAM_DRAW                    0x88E0
#define GL_STREAM_READ                    0x88E1
#define GL_STREAM_COPY                    0x88E2
#define GL_STATIC_DRAW                    0x88E4
#define GL_STATIC_READ                    0x88E5
#define GL_STATIC_COPY                    0x88E6
#define GL_DYNAMIC_DRAW                   0x88E8
#define GL_DYNAMIC_READ                   0x88E9
#define GL_DYNAMIC_COPY                   0x88EA
#define GL_SAMPLES_PASSED                 0x8914
//#define GL_FOG_COORD_SRC                  GL_FOG_COORDINATE_SOURCE // deprecated
//#define GL_FOG_COORD                      GL_FOG_COORDINATE // deprecated
//#define GL_CURRENT_FOG_COORD              GL_CURRENT_FOG_COORDINATE // deprecated
//#define GL_FOG_COORD_ARRAY_TYPE           GL_FOG_COORDINATE_ARRAY_TYPE // deprecated
//#define GL_FOG_COORD_ARRAY_STRIDE         GL_FOG_COORDINATE_ARRAY_STRIDE // deprecated
//#define GL_FOG_COORD_ARRAY_POINTER        GL_FOG_COORDINATE_ARRAY_POINTER // deprecated
//#define GL_FOG_COORD_ARRAY                GL_FOG_COORDINATE_ARRAY // deprecated
//#define GL_FOG_COORD_ARRAY_BUFFER_BINDING GL_FOG_COORDINATE_ARRAY_BUFFER_BINDING // deprecated
//#define GL_SRC0_RGB                       GL_SOURCE0_RGB // deprecated
//#define GL_SRC1_RGB                       GL_SOURCE1_RGB // deprecated
//#define GL_SRC2_RGB                       GL_SOURCE2_RGB // deprecated
//#define GL_SRC0_ALPHA                     GL_SOURCE0_ALPHA // deprecated
//#define GL_SRC1_ALPHA                     GL_SOURCE1_ALPHA // deprecated
//#define GL_SRC2_ALPHA                     GL_SOURCE2_ALPHA // deprecated

extern void (GLApi *GLFuncPtrName(glGenQueries))(GLsizei, GLuint*);
extern void (GLApi *GLFuncPtrName(glDeleteQueries))(GLsizei, const GLuint*);
extern GLboolean (GLApi *GLFuncPtrName(glIsQuery))(GLuint);
extern void (GLApi *GLFuncPtrName(glBeginQuery))(GLenum, GLuint);
extern void (GLApi *GLFuncPtrName(glEndQuery))(GLenum);
extern void (GLApi *GLFuncPtrName(glGetQueryiv))(GLenum, GLenum, GLint*);
extern void (GLApi *GLFuncPtrName(glGetQueryObjectiv))(GLuint, GLenum, GLint*);
extern void (GLApi *GLFuncPtrName(glGetQueryObjectuiv))(GLuint, GLenum, GLuint*);
extern void (GLApi *GLFuncPtrName(glBindBuffer))(GLenum, GLuint);
extern void (GLApi *GLFuncPtrName(glDeleteBuffers))(GLsizei, const GLuint*);
extern void (GLApi *GLFuncPtrName(glGenBuffers))(GLsizei, GLuint*);
extern GLboolean (GLApi *GLFuncPtrName(glIsBuffer))(GLuint);
extern void (GLApi *GLFuncPtrName(glBufferData))(GLenum, GLsizeiptr, const GLvoid *, GLenum);
extern void (GLApi *GLFuncPtrName(glBufferSubData))(GLenum, GLintptr, GLsizeiptr, const GLvoid*);
extern void (GLApi *GLFuncPtrName(glGetBufferSubData))(GLenum, GLintptr, GLsizeiptr, GLvoid*);
extern GLvoid* (GLApi *GLFuncPtrName(glMapBuffer))(GLenum, GLenum);
extern GLboolean (GLApi *GLFuncPtrName(glUnmapBuffer))(GLenum);
extern void (GLApi *GLFuncPtrName(glGetBufferParameteriv))(GLenum, GLenum, GLint*);
extern void (GLApi *GLFuncPtrName(glGetBufferPointerv))(GLenum, GLenum, GLvoid**);

#define glGenQueries GLFuncPtrName(glGenQueries)
#define glDeleteQueries GLFuncPtrName(glDeleteQueries)
#define glIsQuery GLFuncPtrName(glIsQuery)
#define glBeginQuery GLFuncPtrName(glBeginQuery)
#define glEndQuery GLFuncPtrName(glEndQuery)
#define glGetQueryiv GLFuncPtrName(glGetQueryiv)
#define glGetQueryObjectiv GLFuncPtrName(glGetQueryObjectiv)
#define glGetQueryObjectuiv GLFuncPtrName(glGetQueryObjectuiv)
#define glBindBuffer GLFuncPtrName(glBindBuffer)
#define glDeleteBuffers GLFuncPtrName(glDeleteBuffers)
#define glGenBuffers GLFuncPtrName(glGenBuffers)
#define glIsBuffer GLFuncPtrName(glIsBuffer)
#define glBufferData GLFuncPtrName(glBufferData)
#define glBufferSubData GLFuncPtrName(glBufferSubData)
#define glGetBufferSubData GLFuncPtrName(glGetBufferSubData)
#define glMapBuffer GLFuncPtrName(glMapBuffer)
#define glUnmapBuffer GLFuncPtrName(glUnmapBuffer)
#define glGetBufferParameteriv GLFuncPtrName(glGetBufferParameteriv)
#define glGetBufferPointerv GLFuncPtrName(glGetBufferPointerv)

#endif // GL_VERSION_1_5

//								--- OpenGL 2.0 ---

#ifndef GL_VERSION_2_0
#define GL_VERSION_2_0 1
#define __GENERATE_GL_20__

extern bool GLEH_OPENGL_2_0; /**< set if GL_OPENGL_2_0 is supported */

#define GL_BLEND_EQUATION_RGB             0x8009
#define GL_VERTEX_ATTRIB_ARRAY_ENABLED    0x8622
#define GL_VERTEX_ATTRIB_ARRAY_SIZE       0x8623
#define GL_VERTEX_ATTRIB_ARRAY_STRIDE     0x8624
#define GL_VERTEX_ATTRIB_ARRAY_TYPE       0x8625
#define GL_CURRENT_VERTEX_ATTRIB          0x8626
#define GL_VERTEX_PROGRAM_POINT_SIZE      0x8642
//#define GL_VERTEX_PROGRAM_TWO_SIDE				0x8643 // deprecated
#define GL_VERTEX_ATTRIB_ARRAY_POINTER    0x8645
//#define GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING	0x889F // deprecated
#define GL_STENCIL_BACK_FUNC              0x8800
#define GL_STENCIL_BACK_FAIL              0x8801
#define GL_STENCIL_BACK_PASS_DEPTH_FAIL   0x8802
#define GL_STENCIL_BACK_PASS_DEPTH_PASS   0x8803
#define GL_MAX_DRAW_BUFFERS               0x8824
#define GL_DRAW_BUFFER0                   0x8825
#define GL_DRAW_BUFFER1                   0x8826
#define GL_DRAW_BUFFER2                   0x8827
#define GL_DRAW_BUFFER3                   0x8828
#define GL_DRAW_BUFFER4                   0x8829
#define GL_DRAW_BUFFER5                   0x882A
#define GL_DRAW_BUFFER6                   0x882B
#define GL_DRAW_BUFFER7                   0x882C
#define GL_DRAW_BUFFER8                   0x882D
#define GL_DRAW_BUFFER9                   0x882E
#define GL_DRAW_BUFFER10                  0x882F
#define GL_DRAW_BUFFER11                  0x8830
#define GL_DRAW_BUFFER12                  0x8831
#define GL_DRAW_BUFFER13                  0x8832
#define GL_DRAW_BUFFER14                  0x8833
#define GL_DRAW_BUFFER15                  0x8834
#define GL_BLEND_EQUATION_ALPHA           0x883D
#define GL_MAX_VERTEX_ATTRIBS             0x8869
#define GL_VERTEX_ATTRIB_ARRAY_NORMALIZED 0x886A
#define GL_MAX_TEXTURE_IMAGE_UNITS        0x8872
#define GL_FRAGMENT_SHADER                0x8B30
#define GL_VERTEX_SHADER                  0x8B31
#define GL_MAX_FRAGMENT_UNIFORM_COMPONENTS 0x8B49
#define GL_MAX_VERTEX_UNIFORM_COMPONENTS  0x8B4A
//#define GL_MAX_VARYING_FLOATS             0x8B4B // deprecated
#define GL_MAX_VERTEX_TEXTURE_IMAGE_UNITS 0x8B4C
#define GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS 0x8B4D
//#define GL_MAX_TEXTURE_COORDS					0x8871 // deprecated
#define GL_SHADER_TYPE                    0x8B4F
#define GL_FLOAT_VEC2                     0x8B50
#define GL_FLOAT_VEC3                     0x8B51
#define GL_FLOAT_VEC4                     0x8B52
#define GL_INT_VEC2                       0x8B53
#define GL_INT_VEC3                       0x8B54
#define GL_INT_VEC4                       0x8B55
#define GL_BOOL                           0x8B56
#define GL_BOOL_VEC2                      0x8B57
#define GL_BOOL_VEC3                      0x8B58
#define GL_BOOL_VEC4                      0x8B59
#define GL_FLOAT_MAT2                     0x8B5A
#define GL_FLOAT_MAT3                     0x8B5B
#define GL_FLOAT_MAT4                     0x8B5C
#define GL_SAMPLER_1D                     0x8B5D
#define GL_SAMPLER_2D                     0x8B5E
#define GL_SAMPLER_3D                     0x8B5F
#define GL_SAMPLER_CUBE                   0x8B60
#define GL_SAMPLER_1D_SHADOW              0x8B61
#define GL_SAMPLER_2D_SHADOW              0x8B62
#define GL_DELETE_STATUS                  0x8B80
#define GL_COMPILE_STATUS                 0x8B81
#define GL_LINK_STATUS                    0x8B82
#define GL_VALIDATE_STATUS                0x8B83
#define GL_INFO_LOG_LENGTH                0x8B84
#define GL_ATTACHED_SHADERS               0x8B85
#define GL_ACTIVE_UNIFORMS                0x8B86
#define GL_ACTIVE_UNIFORM_MAX_LENGTH      0x8B87
#define GL_SHADER_SOURCE_LENGTH           0x8B88
#define GL_ACTIVE_ATTRIBUTES              0x8B89
#define GL_ACTIVE_ATTRIBUTE_MAX_LENGTH    0x8B8A
#define GL_FRAGMENT_SHADER_DERIVATIVE_HINT 0x8B8B
#define GL_SHADING_LANGUAGE_VERSION       0x8B8C
#define GL_CURRENT_PROGRAM                0x8B8D
//#define GL_POINT_SPRITE 						0x8861 // deprecated
//#define GL_COORD_REPLACE 						0x8862 // deprecated
#define GL_POINT_SPRITE_COORD_ORIGIN      0x8CA0
#define GL_LOWER_LEFT                     0x8CA1
#define GL_UPPER_LEFT                     0x8CA2
#define GL_STENCIL_BACK_REF               0x8CA3
#define GL_STENCIL_BACK_VALUE_MASK        0x8CA4
#define GL_STENCIL_BACK_WRITEMASK         0x8CA5

extern void (GLApi *GLFuncPtrName(glBlendEquationSeparate))(GLenum modeRGB, GLenum modeAlpha);
extern void (GLApi *GLFuncPtrName(glDrawBuffers))(GLsizei n, const GLenum *bufs);
extern void (GLApi *GLFuncPtrName(glStencilOpSeparate))(GLenum face, GLenum fail, GLenum zfail, GLenum zpass);
extern void (GLApi *GLFuncPtrName(glStencilFuncSeparate))(GLenum face, GLenum func, GLint ref, GLuint mask);
extern void (GLApi *GLFuncPtrName(glStencilMaskSeparate))(GLenum face, GLuint mask);
extern void (GLApi *GLFuncPtrName(glAttachShader))(GLuint program, GLuint shader);
extern void (GLApi *GLFuncPtrName(glBindAttribLocation))(GLuint program, GLuint index, const GLchar *name);
extern void (GLApi *GLFuncPtrName(glCompileShader))(GLuint shader);
extern GLuint (GLApi *GLFuncPtrName(glCreateProgram))(void);
extern GLuint (GLApi *GLFuncPtrName(glCreateShader))(GLenum type);
extern void (GLApi *GLFuncPtrName(glDeleteProgram))(GLuint program);
extern void (GLApi *GLFuncPtrName(glDeleteShader))(GLuint shader);
extern void (GLApi *GLFuncPtrName(glDetachShader))(GLuint program, GLuint shader);
extern void (GLApi *GLFuncPtrName(glDisableVertexAttribArray))(GLuint index);
extern void (GLApi *GLFuncPtrName(glEnableVertexAttribArray))(GLuint index);
extern void (GLApi *GLFuncPtrName(glGetActiveAttrib))(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name);
extern void (GLApi *GLFuncPtrName(glGetActiveUniform))(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name);
extern void (GLApi *GLFuncPtrName(glGetAttachedShaders))(GLuint program, GLsizei maxCount, GLsizei *count, GLuint *shaders);
extern GLint (GLApi *GLFuncPtrName(glGetAttribLocation))(GLuint program, const GLchar *name);
extern void (GLApi *GLFuncPtrName(glGetProgramiv))(GLuint program, GLenum pname, GLint *params);
extern void (GLApi *GLFuncPtrName(glGetProgramInfoLog))(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
extern void (GLApi *GLFuncPtrName(glGetShaderiv))(GLuint program, GLenum pname, GLint *params);
extern void (GLApi *GLFuncPtrName(glGetShaderInfoLog))(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog);
extern void (GLApi *GLFuncPtrName(glShaderSource))(GLuint shader, GLsizei count, const GLchar **string, const GLint *length);
extern GLint (GLApi *GLFuncPtrName(glGetUniformLocation))(GLuint program, const GLchar *name);
extern void (GLApi *GLFuncPtrName(glGetUniformfv))(GLuint program, GLint location, GLfloat *params);
extern void (GLApi *GLFuncPtrName(glGetUniformiv))(GLuint program, GLint location, GLint *params);
extern void (GLApi *GLFuncPtrName(glGetVertexAttribdv))(GLuint index, GLenum pname, GLdouble *params);
extern void (GLApi *GLFuncPtrName(glGetVertexAttribfv))(GLuint index, GLenum pname, GLfloat *params);
extern void (GLApi *GLFuncPtrName(glGetVertexAttribiv))(GLuint index, GLenum pname, GLint *params);
extern void (GLApi *GLFuncPtrName(glGetVertexAttribPointerv))(GLuint index, GLenum pname, GLvoid* *pointer);
extern GLboolean (GLApi *GLFuncPtrName(glIsProgram))(GLuint program);
extern GLboolean (GLApi *GLFuncPtrName(glIsShader))(GLuint shader);
extern void (GLApi *GLFuncPtrName(glLinkProgram))(GLuint program);
extern void (GLApi *GLFuncPtrName(glGetShaderSource))(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source);
extern void (GLApi *GLFuncPtrName(glUseProgram))(GLuint program);
extern void (GLApi *GLFuncPtrName(glUniform1f))(GLint location, GLfloat v0);
extern void (GLApi *GLFuncPtrName(glUniform2f))(GLint location, GLfloat v0, GLfloat v1);
extern void (GLApi *GLFuncPtrName(glUniform3f))(GLint location, GLfloat v0, GLfloat v1, GLfloat v2);
extern void (GLApi *GLFuncPtrName(glUniform4f))(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3);
extern void (GLApi *GLFuncPtrName(glUniform1i))(GLint location, GLint v0);
extern void (GLApi *GLFuncPtrName(glUniform2i))(GLint location, GLint v0, GLint v1);
extern void (GLApi *GLFuncPtrName(glUniform3i))(GLint location, GLint v0, GLint v1, GLint v2);
extern void (GLApi *GLFuncPtrName(glUniform4i))(GLint location, GLint v0, GLint v1, GLint v2, GLint v3);
extern void (GLApi *GLFuncPtrName(glUniform1fv))(GLint location, GLsizei count, const GLfloat *value);
extern void (GLApi *GLFuncPtrName(glUniform2fv))(GLint location, GLsizei count, const GLfloat *value);
extern void (GLApi *GLFuncPtrName(glUniform3fv))(GLint location, GLsizei count, const GLfloat *value);
extern void (GLApi *GLFuncPtrName(glUniform4fv))(GLint location, GLsizei count, const GLfloat *value);
extern void (GLApi *GLFuncPtrName(glUniform1iv))(GLint location, GLsizei count, const GLint *value);
extern void (GLApi *GLFuncPtrName(glUniform2iv))(GLint location, GLsizei count, const GLint *value);
extern void (GLApi *GLFuncPtrName(glUniform3iv))(GLint location, GLsizei count, const GLint *value);
extern void (GLApi *GLFuncPtrName(glUniform4iv))(GLint location, GLsizei count, const GLint *value);
extern void (GLApi *GLFuncPtrName(glUniformMatrix2fv))(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern void (GLApi *GLFuncPtrName(glUniformMatrix3fv))(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern void (GLApi *GLFuncPtrName(glUniformMatrix4fv))(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value);
extern void (GLApi *GLFuncPtrName(glValidateProgram))(GLuint program);
extern void (GLApi *GLFuncPtrName(glVertexAttrib1d))(GLuint index, GLdouble x);
extern void (GLApi *GLFuncPtrName(glVertexAttrib1dv))(GLuint index, const GLdouble *v);
extern void (GLApi *GLFuncPtrName(glVertexAttrib1f))(GLuint index, GLfloat x);
extern void (GLApi *GLFuncPtrName(glVertexAttrib1fv))(GLuint index, const GLfloat *v);
extern void (GLApi *GLFuncPtrName(glVertexAttrib1s))(GLuint index, GLshort x);
extern void (GLApi *GLFuncPtrName(glVertexAttrib1sv))(GLuint index, const GLshort *v);
extern void (GLApi *GLFuncPtrName(glVertexAttrib2d))(GLuint index, GLdouble x, GLdouble y);
extern void (GLApi *GLFuncPtrName(glVertexAttrib2dv))(GLuint index, const GLdouble *v);
extern void (GLApi *GLFuncPtrName(glVertexAttrib2f))(GLuint index, GLfloat x, GLfloat y);
extern void (GLApi *GLFuncPtrName(glVertexAttrib2fv))(GLuint index, const GLfloat *v);
extern void (GLApi *GLFuncPtrName(glVertexAttrib2s))(GLuint index, GLshort x, GLshort y);
extern void (GLApi *GLFuncPtrName(glVertexAttrib2sv))(GLuint index, const GLshort *v);
extern void (GLApi *GLFuncPtrName(glVertexAttrib3d))(GLuint index, GLdouble x, GLdouble y, GLdouble z);
extern void (GLApi *GLFuncPtrName(glVertexAttrib3dv))(GLuint index, const GLdouble *v);
extern void (GLApi *GLFuncPtrName(glVertexAttrib3f))(GLuint index, GLfloat x, GLfloat y, GLfloat z);
extern void (GLApi *GLFuncPtrName(glVertexAttrib3fv))(GLuint index, const GLfloat *v);
extern void (GLApi *GLFuncPtrName(glVertexAttrib3s))(GLuint index, GLshort x, GLshort y, GLshort z);
extern void (GLApi *GLFuncPtrName(glVertexAttrib3sv))(GLuint index, const GLshort *v);
extern void (GLApi *GLFuncPtrName(glVertexAttrib4Nbv))(GLuint index, const GLbyte *v);
extern void (GLApi *GLFuncPtrName(glVertexAttrib4Niv))(GLuint index, const GLint *v);
extern void (GLApi *GLFuncPtrName(glVertexAttrib4Nsv))(GLuint index, const GLshort *v);
extern void (GLApi *GLFuncPtrName(glVertexAttrib4Nub))(GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w);
extern void (GLApi *GLFuncPtrName(glVertexAttrib4Nubv))(GLuint index, const GLubyte *v);
extern void (GLApi *GLFuncPtrName(glVertexAttrib4Nuiv))(GLuint index, const GLuint *v);
extern void (GLApi *GLFuncPtrName(glVertexAttrib4Nusv))(GLuint index, const GLushort *v);
extern void (GLApi *GLFuncPtrName(glVertexAttrib4bv))(GLuint index, const GLbyte *v);
extern void (GLApi *GLFuncPtrName(glVertexAttrib4d))(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w);
extern void (GLApi *GLFuncPtrName(glVertexAttrib4dv))(GLuint index, const GLdouble *v);
extern void (GLApi *GLFuncPtrName(glVertexAttrib4f))(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w);
extern void (GLApi *GLFuncPtrName(glVertexAttrib4fv))(GLuint index, const GLfloat *v);
extern void (GLApi *GLFuncPtrName(glVertexAttrib4iv))(GLuint index, const GLint *v);
extern void (GLApi *GLFuncPtrName(glVertexAttrib4s))(GLuint index, GLshort x, GLshort y, GLshort z, GLshort w);
extern void (GLApi *GLFuncPtrName(glVertexAttrib4sv))(GLuint index, const GLshort *v);
extern void (GLApi *GLFuncPtrName(glVertexAttrib4ubv))(GLuint index, const GLubyte *v);
extern void (GLApi *GLFuncPtrName(glVertexAttrib4uiv))(GLuint index, const GLuint *v);
extern void (GLApi *GLFuncPtrName(glVertexAttrib4usv))(GLuint index, const GLushort *v);
extern void (GLApi *GLFuncPtrName(glVertexAttribPointer))(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer);

#define glBlendEquationSeparate GLFuncPtrName(glBlendEquationSeparate)
#define glDrawBuffers GLFuncPtrName(glDrawBuffers)
#define glStencilOpSeparate GLFuncPtrName(glStencilOpSeparate)
#define glStencilFuncSeparate GLFuncPtrName(glStencilFuncSeparate)
#define glStencilMaskSeparate GLFuncPtrName(glStencilMaskSeparate)
#define glAttachShader GLFuncPtrName(glAttachShader)
#define glBindAttribLocation GLFuncPtrName(glBindAttribLocation)
#define glCompileShader GLFuncPtrName(glCompileShader)
#define glCreateProgram GLFuncPtrName(glCreateProgram)
#define glCreateShader GLFuncPtrName(glCreateShader)
#define glDeleteProgram GLFuncPtrName(glDeleteProgram)
#define glDeleteShader GLFuncPtrName(glDeleteShader)
#define glDetachShader GLFuncPtrName(glDetachShader)
#define glDisableVertexAttribArray GLFuncPtrName(glDisableVertexAttribArray)
#define glEnableVertexAttribArray GLFuncPtrName(glEnableVertexAttribArray)
#define glGetActiveAttrib GLFuncPtrName(glGetActiveAttrib)
#define glGetActiveUniform GLFuncPtrName(glGetActiveUniform)
#define glGetAttachedShaders GLFuncPtrName(glGetAttachedShaders)
#define glGetAttribLocation GLFuncPtrName(glGetAttribLocation)
#define glGetProgramiv GLFuncPtrName(glGetProgramiv)
#define glGetProgramInfoLog GLFuncPtrName(glGetProgramInfoLog)
#define glGetShaderiv GLFuncPtrName(glGetShaderiv)
#define glGetShaderInfoLog GLFuncPtrName(glGetShaderInfoLog)
#define glShaderSource GLFuncPtrName(glShaderSource)
#define glGetUniformLocation GLFuncPtrName(glGetUniformLocation)
#define glGetUniformfv GLFuncPtrName(glGetUniformfv)
#define glGetUniformiv GLFuncPtrName(glGetUniformiv)
#define glGetVertexAttribdv GLFuncPtrName(glGetVertexAttribdv)
#define glGetVertexAttribfv GLFuncPtrName(glGetVertexAttribfv)
#define glGetVertexAttribiv GLFuncPtrName(glGetVertexAttribiv)
#define glGetVertexAttribPointerv GLFuncPtrName(glGetVertexAttribPointerv)
#define glIsProgram GLFuncPtrName(glIsProgram)
#define glIsShader GLFuncPtrName(glIsShader)
#define glLinkProgram GLFuncPtrName(glLinkProgram)
#define glGetShaderSource GLFuncPtrName(glGetShaderSource)
#define glUseProgram GLFuncPtrName(glUseProgram)
#define glUniform1f GLFuncPtrName(glUniform1f)
#define glUniform2f GLFuncPtrName(glUniform2f)
#define glUniform3f GLFuncPtrName(glUniform3f)
#define glUniform4f GLFuncPtrName(glUniform4f)
#define glUniform1i GLFuncPtrName(glUniform1i)
#define glUniform2i GLFuncPtrName(glUniform2i)
#define glUniform3i GLFuncPtrName(glUniform3i)
#define glUniform4i GLFuncPtrName(glUniform4i)
#define glUniform1fv GLFuncPtrName(glUniform1fv)
#define glUniform2fv GLFuncPtrName(glUniform2fv)
#define glUniform3fv GLFuncPtrName(glUniform3fv)
#define glUniform4fv GLFuncPtrName(glUniform4fv)
#define glUniform1iv GLFuncPtrName(glUniform1iv)
#define glUniform2iv GLFuncPtrName(glUniform2iv)
#define glUniform3iv GLFuncPtrName(glUniform3iv)
#define glUniform4iv GLFuncPtrName(glUniform4iv)
#define glUniformMatrix2fv GLFuncPtrName(glUniformMatrix2fv)
#define glUniformMatrix3fv GLFuncPtrName(glUniformMatrix3fv)
#define glUniformMatrix4fv GLFuncPtrName(glUniformMatrix4fv)
#define glValidateProgram GLFuncPtrName(glValidateProgram)
#define glVertexAttrib1d GLFuncPtrName(glVertexAttrib1d)
#define glVertexAttrib1dv GLFuncPtrName(glVertexAttrib1dv)
#define glVertexAttrib1f GLFuncPtrName(glVertexAttrib1f)
#define glVertexAttrib1fv GLFuncPtrName(glVertexAttrib1fv)
#define glVertexAttrib1s GLFuncPtrName(glVertexAttrib1s)
#define glVertexAttrib1sv GLFuncPtrName(glVertexAttrib1sv)
#define glVertexAttrib2d GLFuncPtrName(glVertexAttrib2d)
#define glVertexAttrib2dv GLFuncPtrName(glVertexAttrib2dv)
#define glVertexAttrib2f GLFuncPtrName(glVertexAttrib2f)
#define glVertexAttrib2fv GLFuncPtrName(glVertexAttrib2fv)
#define glVertexAttrib2s GLFuncPtrName(glVertexAttrib2s)
#define glVertexAttrib2sv GLFuncPtrName(glVertexAttrib2sv)
#define glVertexAttrib3d GLFuncPtrName(glVertexAttrib3d)
#define glVertexAttrib3dv GLFuncPtrName(glVertexAttrib3dv)
#define glVertexAttrib3f GLFuncPtrName(glVertexAttrib3f)
#define glVertexAttrib3fv GLFuncPtrName(glVertexAttrib3fv)
#define glVertexAttrib3s GLFuncPtrName(glVertexAttrib3s)
#define glVertexAttrib3sv GLFuncPtrName(glVertexAttrib3sv)
#define glVertexAttrib4Nbv GLFuncPtrName(glVertexAttrib4Nbv)
#define glVertexAttrib4Niv GLFuncPtrName(glVertexAttrib4Niv)
#define glVertexAttrib4Nsv GLFuncPtrName(glVertexAttrib4Nsv)
#define glVertexAttrib4Nub GLFuncPtrName(glVertexAttrib4Nub)
#define glVertexAttrib4Nubv GLFuncPtrName(glVertexAttrib4Nubv)
#define glVertexAttrib4Nuiv GLFuncPtrName(glVertexAttrib4Nuiv)
#define glVertexAttrib4Nusv GLFuncPtrName(glVertexAttrib4Nusv)
#define glVertexAttrib4bv GLFuncPtrName(glVertexAttrib4bv)
#define glVertexAttrib4d GLFuncPtrName(glVertexAttrib4d)
#define glVertexAttrib4dv GLFuncPtrName(glVertexAttrib4dv)
#define glVertexAttrib4f GLFuncPtrName(glVertexAttrib4f)
#define glVertexAttrib4fv GLFuncPtrName(glVertexAttrib4fv)
#define glVertexAttrib4iv GLFuncPtrName(glVertexAttrib4iv)
#define glVertexAttrib4s GLFuncPtrName(glVertexAttrib4s)
#define glVertexAttrib4sv GLFuncPtrName(glVertexAttrib4sv)
#define glVertexAttrib4ubv GLFuncPtrName(glVertexAttrib4ubv)
#define glVertexAttrib4uiv GLFuncPtrName(glVertexAttrib4uiv)
#define glVertexAttrib4usv GLFuncPtrName(glVertexAttrib4usv)
#define glVertexAttribPointer GLFuncPtrName(glVertexAttribPointer)

#endif // GL_VERSION_2_0

//								--- OpenGL 2.1 ---

#ifndef GL_VERSION_2_1
#define GL_VERSION_2_1 1
#define __GENERATE_GL_21__

extern bool GLEH_OPENGL_2_1; /**< set if GL_OPENGL_2_1 is supported */

//#define GL_CURRENT_RASTER_SECONDARY_COLOR 0x845F // deprecated
#define GL_PIXEL_PACK_BUFFER              0x88EB
#define GL_PIXEL_UNPACK_BUFFER            0x88EC
#define GL_PIXEL_PACK_BUFFER_BINDING      0x88ED
#define GL_PIXEL_UNPACK_BUFFER_BINDING    0x88EF
#define GL_FLOAT_MAT2x3                   0x8B65
#define GL_FLOAT_MAT2x4                   0x8B66
#define GL_FLOAT_MAT3x2                   0x8B67
#define GL_FLOAT_MAT3x4                   0x8B68
#define GL_FLOAT_MAT4x2                   0x8B69
#define GL_FLOAT_MAT4x3                   0x8B6A
#define GL_SRGB                           0x8C40
#define GL_SRGB8                          0x8C41
#define GL_SRGB_ALPHA                     0x8C42
#define GL_SRGB8_ALPHA8                   0x8C43
//#define GL_SLUMINANCE_ALPHA               0x8C44 // deprecated
//#define GL_SLUMINANCE8_ALPHA8             0x8C45 // deprecated
//#define GL_SLUMINANCE                     0x8C46 // deprecated
//#define GL_SLUMINANCE8                    0x8C47 // deprecated
#define GL_COMPRESSED_SRGB                0x8C48
#define GL_COMPRESSED_SRGB_ALPHA          0x8C49
//#define GL_COMPRESSED_SLUMINANCE          0x8C4A // deprecated
//#define GL_COMPRESSED_SLUMINANCE_ALPHA    0x8C4B // deprecated

extern void (GLApi *GLFuncPtrName(glUniformMatrix2x3fv))(GLint, GLsizei, GLboolean, const GLfloat*);
extern void (GLApi *GLFuncPtrName(glUniformMatrix3x2fv))(GLint, GLsizei, GLboolean, const GLfloat*);
extern void (GLApi *GLFuncPtrName(glUniformMatrix2x4fv))(GLint, GLsizei, GLboolean, const GLfloat*);
extern void (GLApi *GLFuncPtrName(glUniformMatrix4x2fv))(GLint, GLsizei, GLboolean, const GLfloat*);
extern void (GLApi *GLFuncPtrName(glUniformMatrix3x4fv))(GLint, GLsizei, GLboolean, const GLfloat*);
extern void (GLApi *GLFuncPtrName(glUniformMatrix4x3fv))(GLint, GLsizei, GLboolean, const GLfloat*);

#define glUniformMatrix2x3fv GLFuncPtrName(glUniformMatrix2x3fv)
#define glUniformMatrix3x2fv GLFuncPtrName(glUniformMatrix3x2fv)
#define glUniformMatrix2x4fv GLFuncPtrName(glUniformMatrix2x4fv)
#define glUniformMatrix4x2fv GLFuncPtrName(glUniformMatrix4x2fv)
#define glUniformMatrix3x4fv GLFuncPtrName(glUniformMatrix3x4fv)
#define glUniformMatrix4x3fv GLFuncPtrName(glUniformMatrix4x3fv)

#endif // GL_VERSION_2_1

//								--- OpenGL 3.0 ---

#ifndef GL_VERSION_3_0
#define GL_VERSION_3_0 1
#define __GENERATE_GL_30__

extern bool GLEH_OPENGL_3_0; /**< set if GL_OPENGL_3_0 is supported */

#define GL_COMPARE_REF_TO_TEXTURE         0x884E
#define GL_CLIP_DISTANCE0                 0x3000
#define GL_CLIP_DISTANCE1                 0x3001
#define GL_CLIP_DISTANCE2                 0x3002
#define GL_CLIP_DISTANCE3                 0x3003
#define GL_CLIP_DISTANCE4                 0x3004
#define GL_CLIP_DISTANCE5                 0x3005
#define GL_CLIP_DISTANCE6                 0x3006
#define GL_CLIP_DISTANCE7                 0x3007
#define GL_MAX_CLIP_DISTANCES             0x0D32
#define GL_MAJOR_VERSION                  0x821B
#define GL_MINOR_VERSION                  0x821C
#define GL_NUM_EXTENSIONS                 0x821D
#define GL_CONTEXT_FLAGS                  0x821E
#define GL_DEPTH_BUFFER                   0x8223
#define GL_STENCIL_BUFFER                 0x8224
#define GL_COMPRESSED_RED                 0x8225
#define GL_COMPRESSED_RG                  0x8226
#define GL_CONTEXT_FLAG_FORWARD_COMPATIBLE_BIT 0x0001
#define GL_RGBA32F                        0x8814
#define GL_RGB32F                         0x8815
#define GL_RGBA16F                        0x881A
#define GL_RGB16F                         0x881B
#define GL_VERTEX_ATTRIB_ARRAY_INTEGER    0x88FD
#define GL_MAX_ARRAY_TEXTURE_LAYERS       0x88FF
#define GL_MIN_PROGRAM_TEXEL_OFFSET       0x8904
#define GL_MAX_PROGRAM_TEXEL_OFFSET       0x8905
#define GL_CLAMP_READ_COLOR               0x891C
#define GL_FIXED_ONLY                     0x891D
#define GL_MAX_VARYING_COMPONENTS         0x8B4B
#define GL_TEXTURE_1D_ARRAY               0x8C18
#define GL_PROXY_TEXTURE_1D_ARRAY         0x8C19
#define GL_TEXTURE_2D_ARRAY               0x8C1A
#define GL_PROXY_TEXTURE_2D_ARRAY         0x8C1B
#define GL_TEXTURE_BINDING_1D_ARRAY       0x8C1C
#define GL_TEXTURE_BINDING_2D_ARRAY       0x8C1D
#define GL_R11F_G11F_B10F                 0x8C3A
#define GL_UNSIGNED_INT_10F_11F_11F_REV   0x8C3B
#define GL_RGB9_E5                        0x8C3D
#define GL_UNSIGNED_INT_5_9_9_9_REV       0x8C3E
#define GL_TEXTURE_SHARED_SIZE            0x8C3F
#define GL_TRANSFORM_FEEDBACK_VARYING_MAX_LENGTH 0x8C76
#define GL_TRANSFORM_FEEDBACK_BUFFER_MODE 0x8C7F
#define GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_COMPONENTS 0x8C80
#define GL_TRANSFORM_FEEDBACK_VARYINGS    0x8C83
#define GL_TRANSFORM_FEEDBACK_BUFFER_START 0x8C84
#define GL_TRANSFORM_FEEDBACK_BUFFER_SIZE 0x8C85
#define GL_PRIMITIVES_GENERATED           0x8C87
#define GL_TRANSFORM_FEEDBACK_PRIMITIVES_WRITTEN 0x8C88
#define GL_RASTERIZER_DISCARD             0x8C89
#define GL_MAX_TRANSFORM_FEEDBACK_INTERLEAVED_COMPONENTS 0x8C8A
#define GL_MAX_TRANSFORM_FEEDBACK_SEPARATE_ATTRIBS 0x8C8B
#define GL_INTERLEAVED_ATTRIBS            0x8C8C
#define GL_SEPARATE_ATTRIBS               0x8C8D
#define GL_TRANSFORM_FEEDBACK_BUFFER      0x8C8E
#define GL_TRANSFORM_FEEDBACK_BUFFER_BINDING 0x8C8F
#define GL_RGBA32UI                       0x8D70
#define GL_RGB32UI                        0x8D71
#define GL_RGBA16UI                       0x8D76
#define GL_RGB16UI                        0x8D77
#define GL_RGBA8UI                        0x8D7C
#define GL_RGB8UI                         0x8D7D
#define GL_RGBA32I                        0x8D82
#define GL_RGB32I                         0x8D83
#define GL_RGBA16I                        0x8D88
#define GL_RGB16I                         0x8D89
#define GL_RGBA8I                         0x8D8E
#define GL_RGB8I                          0x8D8F
#define GL_RED_INTEGER                    0x8D94
#define GL_GREEN_INTEGER                  0x8D95
#define GL_BLUE_INTEGER                   0x8D96
#define GL_RGB_INTEGER                    0x8D98
#define GL_RGBA_INTEGER                   0x8D99
#define GL_BGR_INTEGER                    0x8D9A
#define GL_BGRA_INTEGER                   0x8D9B
#define GL_SAMPLER_1D_ARRAY               0x8DC0
#define GL_SAMPLER_2D_ARRAY               0x8DC1
#define GL_SAMPLER_1D_ARRAY_SHADOW        0x8DC3
#define GL_SAMPLER_2D_ARRAY_SHADOW        0x8DC4
#define GL_SAMPLER_CUBE_SHADOW            0x8DC5
#define GL_UNSIGNED_INT_VEC2              0x8DC6
#define GL_UNSIGNED_INT_VEC3              0x8DC7
#define GL_UNSIGNED_INT_VEC4              0x8DC8
#define GL_INT_SAMPLER_1D                 0x8DC9
#define GL_INT_SAMPLER_2D                 0x8DCA
#define GL_INT_SAMPLER_3D                 0x8DCB
#define GL_INT_SAMPLER_CUBE               0x8DCC
#define GL_INT_SAMPLER_1D_ARRAY           0x8DCE
#define GL_INT_SAMPLER_2D_ARRAY           0x8DCF
#define GL_UNSIGNED_INT_SAMPLER_1D        0x8DD1
#define GL_UNSIGNED_INT_SAMPLER_2D        0x8DD2
#define GL_UNSIGNED_INT_SAMPLER_3D        0x8DD3
#define GL_UNSIGNED_INT_SAMPLER_CUBE      0x8DD4
#define GL_UNSIGNED_INT_SAMPLER_1D_ARRAY  0x8DD6
#define GL_UNSIGNED_INT_SAMPLER_2D_ARRAY  0x8DD7
#define GL_QUERY_WAIT                     0x8E13
#define GL_QUERY_NO_WAIT                  0x8E14
#define GL_QUERY_BY_REGION_WAIT           0x8E15
#define GL_QUERY_BY_REGION_NO_WAIT        0x8E16
#define GL_BUFFER_ACCESS_FLAGS            0x911F
#define GL_BUFFER_MAP_LENGTH              0x9120
#define GL_BUFFER_MAP_OFFSET              0x9121
/* Reuse tokens from ARB_depth_buffer_float */
/* Reuse tokens from ARB_framebuffer_object */
/* Reuse tokens from ARB_framebuffer_sRGB */
/* Reuse tokens from ARB_half_float_vertex */
/* Reuse tokens from ARB_map_buffer_range */
/* Reuse tokens from ARB_texture_compression_rgtc */
/* Reuse tokens from ARB_texture_rg */
/* Reuse tokens from ARB_vertex_array_object */

/* OpenGL 3.0 also reuses entry points from these extensions: */
/* ARB_framebuffer_object */
/* ARB_map_buffer_range */
/* ARB_vertex_array_object */

extern void (GLApi *GLFuncPtrName(glColorMaski))(GLuint, GLboolean, GLboolean, GLboolean, GLboolean);
extern void (GLApi *GLFuncPtrName(glGetBooleani_v))(GLenum, GLuint, GLboolean *);
extern void (GLApi *GLFuncPtrName(glGetIntegeri_v))(GLenum, GLuint, GLint *);
extern void (GLApi *GLFuncPtrName(glEnablei))(GLenum, GLuint);
extern void (GLApi *GLFuncPtrName(glDisablei))(GLenum, GLuint);
extern GLboolean (GLApi *GLFuncPtrName(glIsEnabledi))(GLenum, GLuint);
extern void (GLApi *GLFuncPtrName(glBeginTransformFeedback))(GLenum);
extern void (GLApi *GLFuncPtrName(glEndTransformFeedback))(void);
extern void (GLApi *GLFuncPtrName(glBindBufferRange))(GLenum, GLuint, GLuint, GLintptr, GLsizeiptr);
extern void (GLApi *GLFuncPtrName(glBindBufferBase))(GLenum, GLuint, GLuint);
extern void (GLApi *GLFuncPtrName(glTransformFeedbackVaryings))(GLuint, GLsizei, const GLchar* *, GLenum);
extern void (GLApi *GLFuncPtrName(glGetTransformFeedbackVarying))(GLuint, GLuint, GLsizei, GLsizei *, GLsizei *, GLenum *, GLchar *);
extern void (GLApi *GLFuncPtrName(glClampColor))(GLenum, GLenum);
extern void (GLApi *GLFuncPtrName(glBeginConditionalRender))(GLuint, GLenum);
extern void (GLApi *GLFuncPtrName(glEndConditionalRender))(void);
extern void (GLApi *GLFuncPtrName(glVertexAttribIPointer))(GLuint, GLint, GLenum, GLsizei, const GLvoid *);
extern void (GLApi *GLFuncPtrName(glGetVertexAttribIiv))(GLuint, GLenum, GLint *);
extern void (GLApi *GLFuncPtrName(glGetVertexAttribIuiv))(GLuint, GLenum, GLuint *);
extern void (GLApi *GLFuncPtrName(glVertexAttribI1i))(GLuint, GLint);
extern void (GLApi *GLFuncPtrName(glVertexAttribI2i))(GLuint, GLint, GLint);
extern void (GLApi *GLFuncPtrName(glVertexAttribI3i))(GLuint, GLint, GLint, GLint);
extern void (GLApi *GLFuncPtrName(glVertexAttribI4i))(GLuint, GLint, GLint, GLint, GLint);
extern void (GLApi *GLFuncPtrName(glVertexAttribI1ui))(GLuint, GLuint);
extern void (GLApi *GLFuncPtrName(glVertexAttribI2ui))(GLuint, GLuint, GLuint);
extern void (GLApi *GLFuncPtrName(glVertexAttribI3ui))(GLuint, GLuint, GLuint, GLuint);
extern void (GLApi *GLFuncPtrName(glVertexAttribI4ui))(GLuint, GLuint, GLuint, GLuint, GLuint);
extern void (GLApi *GLFuncPtrName(glVertexAttribI1iv))(GLuint, const GLint *);
extern void (GLApi *GLFuncPtrName(glVertexAttribI2iv))(GLuint, const GLint *);
extern void (GLApi *GLFuncPtrName(glVertexAttribI3iv))(GLuint, const GLint *);
extern void (GLApi *GLFuncPtrName(glVertexAttribI4iv))(GLuint, const GLint *);
extern void (GLApi *GLFuncPtrName(glVertexAttribI1uiv))(GLuint, const GLuint *);
extern void (GLApi *GLFuncPtrName(glVertexAttribI2uiv))(GLuint, const GLuint *);
extern void (GLApi *GLFuncPtrName(glVertexAttribI3uiv))(GLuint, const GLuint *);
extern void (GLApi *GLFuncPtrName(glVertexAttribI4uiv))(GLuint, const GLuint *);
extern void (GLApi *GLFuncPtrName(glVertexAttribI4bv))(GLuint, const GLbyte *);
extern void (GLApi *GLFuncPtrName(glVertexAttribI4sv))(GLuint, const GLshort *);
extern void (GLApi *GLFuncPtrName(glVertexAttribI4ubv))(GLuint, const GLubyte *);
extern void (GLApi *GLFuncPtrName(glVertexAttribI4usv))(GLuint, const GLushort *);
extern void (GLApi *GLFuncPtrName(glGetUniformuiv))(GLuint, GLint, GLuint *);
extern void (GLApi *GLFuncPtrName(glBindFragDataLocation))(GLuint, GLuint, const GLchar *);
extern GLint (GLApi *GLFuncPtrName(glGetFragDataLocation))(GLuint, const GLchar *);
extern void (GLApi *GLFuncPtrName(glUniform1ui))(GLint, GLuint);
extern void (GLApi *GLFuncPtrName(glUniform2ui))(GLint, GLuint, GLuint);
extern void (GLApi *GLFuncPtrName(glUniform3ui))(GLint, GLuint, GLuint, GLuint);
extern void (GLApi *GLFuncPtrName(glUniform4ui))(GLint, GLuint, GLuint, GLuint, GLuint);
extern void (GLApi *GLFuncPtrName(glUniform1uiv))(GLint, GLsizei, const GLuint *);
extern void (GLApi *GLFuncPtrName(glUniform2uiv))(GLint, GLsizei, const GLuint *);
extern void (GLApi *GLFuncPtrName(glUniform3uiv))(GLint, GLsizei, const GLuint *);
extern void (GLApi *GLFuncPtrName(glUniform4uiv))(GLint, GLsizei, const GLuint *);
extern void (GLApi *GLFuncPtrName(glTexParameterIiv))(GLenum, GLenum, const GLint *);
extern void (GLApi *GLFuncPtrName(glTexParameterIuiv))(GLenum, GLenum, const GLuint *);
extern void (GLApi *GLFuncPtrName(glGetTexParameterIiv))(GLenum, GLenum, GLint *);
extern void (GLApi *GLFuncPtrName(glGetTexParameterIuiv))(GLenum, GLenum, GLuint *);
extern void (GLApi *GLFuncPtrName(glClearBufferiv))(GLenum, GLint, const GLint *);
extern void (GLApi *GLFuncPtrName(glClearBufferuiv))(GLenum, GLint, const GLuint *);
extern void (GLApi *GLFuncPtrName(glClearBufferfv))(GLenum, GLint, const GLfloat *);
extern void (GLApi *GLFuncPtrName(glClearBufferfi))(GLenum, GLint, GLfloat, GLint);
extern const GLubyte *(GLApi *GLFuncPtrName(glGetStringi))(GLenum, GLuint);

#define glColorMaski GLFuncPtrName(glColorMaski)
#define glGetBooleani_v GLFuncPtrName(glGetBooleani_v)
#define glGetIntegeri_v GLFuncPtrName(glGetIntegeri_v)
#define glEnablei GLFuncPtrName(glEnablei)
#define glDisablei GLFuncPtrName(glDisablei)
#define glIsEnabledi GLFuncPtrName(glIsEnabledi)
#define glBeginTransformFeedback GLFuncPtrName(glBeginTransformFeedback)
#define glEndTransformFeedback GLFuncPtrName(glEndTransformFeedback)
#define glBindBufferRange GLFuncPtrName(glBindBufferRange)
#define glBindBufferBase GLFuncPtrName(glBindBufferBase)
#define glTransformFeedbackVaryings GLFuncPtrName(glTransformFeedbackVaryings)
#define glGetTransformFeedbackVarying GLFuncPtrName(glGetTransformFeedbackVarying)
#define glClampColor GLFuncPtrName(glClampColor)
#define glBeginConditionalRender GLFuncPtrName(glBeginConditionalRender)
#define glEndConditionalRender GLFuncPtrName(glEndConditionalRender)
#define glVertexAttribIPointer GLFuncPtrName(glVertexAttribIPointer)
#define glGetVertexAttribIiv GLFuncPtrName(glGetVertexAttribIiv)
#define glGetVertexAttribIuiv GLFuncPtrName(glGetVertexAttribIuiv)
#define glVertexAttribI1i GLFuncPtrName(glVertexAttribI1i)
#define glVertexAttribI2i GLFuncPtrName(glVertexAttribI2i)
#define glVertexAttribI3i GLFuncPtrName(glVertexAttribI3i)
#define glVertexAttribI4i GLFuncPtrName(glVertexAttribI4i)
#define glVertexAttribI1ui GLFuncPtrName(glVertexAttribI1ui)
#define glVertexAttribI2ui GLFuncPtrName(glVertexAttribI2ui)
#define glVertexAttribI3ui GLFuncPtrName(glVertexAttribI3ui)
#define glVertexAttribI4ui GLFuncPtrName(glVertexAttribI4ui)
#define glVertexAttribI1iv GLFuncPtrName(glVertexAttribI1iv)
#define glVertexAttribI2iv GLFuncPtrName(glVertexAttribI2iv)
#define glVertexAttribI3iv GLFuncPtrName(glVertexAttribI3iv)
#define glVertexAttribI4iv GLFuncPtrName(glVertexAttribI4iv)
#define glVertexAttribI1uiv GLFuncPtrName(glVertexAttribI1uiv)
#define glVertexAttribI2uiv GLFuncPtrName(glVertexAttribI2uiv)
#define glVertexAttribI3uiv GLFuncPtrName(glVertexAttribI3uiv)
#define glVertexAttribI4uiv GLFuncPtrName(glVertexAttribI4uiv)
#define glVertexAttribI4bv GLFuncPtrName(glVertexAttribI4bv)
#define glVertexAttribI4sv GLFuncPtrName(glVertexAttribI4sv)
#define glVertexAttribI4ubv GLFuncPtrName(glVertexAttribI4ubv)
#define glVertexAttribI4usv GLFuncPtrName(glVertexAttribI4usv)
#define glGetUniformuiv GLFuncPtrName(glGetUniformuiv)
#define glBindFragDataLocation GLFuncPtrName(glBindFragDataLocation)
#define glGetFragDataLocation GLFuncPtrName(glGetFragDataLocation)
#define glUniform1ui GLFuncPtrName(glUniform1ui)
#define glUniform2ui GLFuncPtrName(glUniform2ui)
#define glUniform3ui GLFuncPtrName(glUniform3ui)
#define glUniform4ui GLFuncPtrName(glUniform4ui)
#define glUniform1uiv GLFuncPtrName(glUniform1uiv)
#define glUniform2uiv GLFuncPtrName(glUniform2uiv)
#define glUniform3uiv GLFuncPtrName(glUniform3uiv)
#define glUniform4uiv GLFuncPtrName(glUniform4uiv)
#define glTexParameterIiv GLFuncPtrName(glTexParameterIiv)
#define glTexParameterIuiv GLFuncPtrName(glTexParameterIuiv)
#define glGetTexParameterIiv GLFuncPtrName(glGetTexParameterIiv)
#define glGetTexParameterIuiv GLFuncPtrName(glGetTexParameterIuiv)
#define glClearBufferiv GLFuncPtrName(glClearBufferiv)
#define glClearBufferuiv GLFuncPtrName(glClearBufferuiv)
#define glClearBufferfv GLFuncPtrName(glClearBufferfv)
#define glClearBufferfi GLFuncPtrName(glClearBufferfi)
#define glGetStringi GLFuncPtrName(glGetStringi)

#endif // GL_VERSION_3_0

//								--- OpenGL 3.1 ---

#ifndef GL_VERSION_3_1
#define GL_VERSION_3_1 1
#define __GENERATE_GL_31__

extern bool GLEH_OPENGL_3_1; /**< set if GL_OPENGL_3_1 is supported */

#define GL_SAMPLER_2D_RECT                0x8B63
#define GL_SAMPLER_2D_RECT_SHADOW         0x8B64
#define GL_SAMPLER_BUFFER                 0x8DC2
#define GL_INT_SAMPLER_2D_RECT            0x8DCD
#define GL_INT_SAMPLER_BUFFER             0x8DD0
#define GL_UNSIGNED_INT_SAMPLER_2D_RECT   0x8DD5
#define GL_UNSIGNED_INT_SAMPLER_BUFFER    0x8DD8
#define GL_TEXTURE_BUFFER                 0x8C2A
#define GL_MAX_TEXTURE_BUFFER_SIZE        0x8C2B
#define GL_TEXTURE_BINDING_BUFFER         0x8C2C
#define GL_TEXTURE_BUFFER_DATA_STORE_BINDING 0x8C2D
#define GL_TEXTURE_BUFFER_FORMAT          0x8C2E
#define GL_TEXTURE_RECTANGLE              0x84F5
#define GL_TEXTURE_BINDING_RECTANGLE      0x84F6
#define GL_PROXY_TEXTURE_RECTANGLE        0x84F7
#define GL_MAX_RECTANGLE_TEXTURE_SIZE     0x84F8
#define GL_RED_SNORM                      0x8F90
#define GL_RG_SNORM                       0x8F91
#define GL_RGB_SNORM                      0x8F92
#define GL_RGBA_SNORM                     0x8F93
#define GL_R8_SNORM                       0x8F94
#define GL_RG8_SNORM                      0x8F95
#define GL_RGB8_SNORM                     0x8F96
#define GL_RGBA8_SNORM                    0x8F97
#define GL_R16_SNORM                      0x8F98
#define GL_RG16_SNORM                     0x8F99
#define GL_RGB16_SNORM                    0x8F9A
#define GL_RGBA16_SNORM                   0x8F9B
#define GL_SIGNED_NORMALIZED              0x8F9C
#define GL_PRIMITIVE_RESTART              0x8F9D
#define GL_PRIMITIVE_RESTART_INDEX        0x8F9E
/* Reuse tokens from ARB_copy_buffer */
/* Would reuse tokens from ARB_draw_instanced, but it has none */
/* Reuse tokens from ARB_uniform_buffer_object */
extern void (GLApi *GLFuncPtrName(glTexBuffer))(GLenum, GLenum, GLuint);
extern void (GLApi *GLFuncPtrName(glPrimitiveRestartIndex))(GLuint);

#define glTexBuffer GLFuncPtrName(glTexBuffer)
#define glPrimitiveRestartIndex GLFuncPtrName(glPrimitiveRestartIndex)

#endif // GL_VERSION_3_1

//								--- OpenGL 3.2 ---

#ifndef GL_VERSION_3_2
#define GL_VERSION_3_2 1
#define __GENERATE_GL_32__

extern bool GLEH_OPENGL_3_2; /**< set if GL_OPENGL_3_2 is supported */

#define GL_CONTEXT_CORE_PROFILE_BIT       0x00000001
#define GL_CONTEXT_COMPATIBILITY_PROFILE_BIT 0x00000002
#define GL_LINES_ADJACENCY                0x000A
#define GL_LINE_STRIP_ADJACENCY           0x000B
#define GL_TRIANGLES_ADJACENCY            0x000C
#define GL_TRIANGLE_STRIP_ADJACENCY       0x000D
#define GL_PROGRAM_POINT_SIZE             0x8642
#define GL_MAX_GEOMETRY_TEXTURE_IMAGE_UNITS 0x8C29
#define GL_FRAMEBUFFER_ATTACHMENT_LAYERED 0x8DA7
#define GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS 0x8DA8
#define GL_GEOMETRY_SHADER                0x8DD9
#define GL_GEOMETRY_VERTICES_OUT          0x8916
#define GL_GEOMETRY_INPUT_TYPE            0x8917
#define GL_GEOMETRY_OUTPUT_TYPE           0x8918
#define GL_MAX_GEOMETRY_UNIFORM_COMPONENTS 0x8DDF
#define GL_MAX_GEOMETRY_OUTPUT_VERTICES   0x8DE0
#define GL_MAX_GEOMETRY_TOTAL_OUTPUT_COMPONENTS 0x8DE1
#define GL_MAX_VERTEX_OUTPUT_COMPONENTS   0x9122
#define GL_MAX_GEOMETRY_INPUT_COMPONENTS  0x9123
#define GL_MAX_GEOMETRY_OUTPUT_COMPONENTS 0x9124
#define GL_MAX_FRAGMENT_INPUT_COMPONENTS  0x9125
#define GL_CONTEXT_PROFILE_MASK           0x9126
/* reuse GL_MAX_VARYING_COMPONENTS (OpenGL 3.0 core) */
/* reuse GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER (GL_ARB_framebuffer_object) */
/* Reuse tokens from ARB_depth_clamp */
/* Would reuse tokens from ARB_draw_elements_base_vertex, but it has none */
/* Would reuse tokens from ARB_fragment_coord_conventions, but it has none */
/* Reuse tokens from ARB_provoking_vertex */
/* Reuse tokens from ARB_seamless_cube_map */
/* Reuse tokens from ARB_sync */
/* Reuse tokens from ARB_texture_multisample */
/* Don't need to reuse tokens from ARB_vertex_array_bgra since they're already in 1.2 core */

extern void (GLApi *GLFuncPtrName(glGetInteger64i_v))(GLenum, GLuint, GLint64 *);
extern void (GLApi *GLFuncPtrName(glGetBufferParameteri64v))(GLenum, GLenum, GLint64 *);
extern void (GLApi *GLFuncPtrName(glProgramParameteri))(GLuint, GLenum, GLint);
extern void (GLApi *GLFuncPtrName(glFramebufferTexture))(GLenum, GLenum, GLuint, GLint);
//extern void (GLApi *GLFuncPtrName(glFramebufferTextureFace))(GLenum, GLenum, GLuint, GLint, GLenum);

#define glGetInteger64i_v GLFuncPtrName(glGetInteger64i_v)
#define glGetBufferParameteri64v GLFuncPtrName(glGetBufferParameteri64v)
#define glProgramParameteri GLFuncPtrName(glProgramParameteri)
#define glFramebufferTexture GLFuncPtrName(glFramebufferTexture)
//#define glFramebufferTextureFace GLFuncPtrName(glFramebufferTextureFace)

#endif // GL_VERSION_3_2


//								--- OpenGL 3.3 ---

#ifndef GL_VERSION_3_3
#define GL_VERSION_3_3 1
#define __GENERATE_GL_33__

extern bool GLEH_OPENGL_3_3; /**< set if GL_OPENGL_3_3 is supported */

#endif // GL_VERSION_3_3

//								--- GL_ARB_depth_buffer_float (part of OpenGL 3.0 core) ---

#ifndef GL_ARB_depth_buffer_float
#define GL_ARB_depth_buffer_float 1

#define GL_DEPTH_COMPONENT32F             0x8CAC
#define GL_DEPTH32F_STENCIL8              0x8CAD
#define GL_FLOAT_32_UNSIGNED_INT_24_8_REV 0x8DAD

// no prototypes here

#endif // GL_ARB_depth_buffer_float

//								--- GL_ARB_framebuffer_object (part of OpenGL 3.0 core) ---

#ifndef GL_ARB_framebuffer_object
#define GL_ARB_framebuffer_object 1
#define __GENERATE_GL_ARB_framebuffer_object__

#define GL_INVALID_FRAMEBUFFER_OPERATION  0x0506
#define GL_FRAMEBUFFER_ATTACHMENT_COLOR_ENCODING 0x8210
#define GL_FRAMEBUFFER_ATTACHMENT_COMPONENT_TYPE 0x8211
#define GL_FRAMEBUFFER_ATTACHMENT_RED_SIZE 0x8212
#define GL_FRAMEBUFFER_ATTACHMENT_GREEN_SIZE 0x8213
#define GL_FRAMEBUFFER_ATTACHMENT_BLUE_SIZE 0x8214
#define GL_FRAMEBUFFER_ATTACHMENT_ALPHA_SIZE 0x8215
#define GL_FRAMEBUFFER_ATTACHMENT_DEPTH_SIZE 0x8216
#define GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE 0x8217
#define GL_FRAMEBUFFER_DEFAULT            0x8218
#define GL_FRAMEBUFFER_UNDEFINED          0x8219
#define GL_DEPTH_STENCIL_ATTACHMENT       0x821A
#define GL_MAX_RENDERBUFFER_SIZE          0x84E8
#define GL_DEPTH_STENCIL                  0x84F9
#define GL_UNSIGNED_INT_24_8              0x84FA
#define GL_DEPTH24_STENCIL8               0x88F0
#define GL_TEXTURE_STENCIL_SIZE           0x88F1
#define GL_TEXTURE_RED_TYPE               0x8C10
#define GL_TEXTURE_GREEN_TYPE             0x8C11
#define GL_TEXTURE_BLUE_TYPE              0x8C12
#define GL_TEXTURE_ALPHA_TYPE             0x8C13
#define GL_TEXTURE_DEPTH_TYPE             0x8C16
#define GL_UNSIGNED_NORMALIZED            0x8C17
#define GL_FRAMEBUFFER_BINDING            0x8CA6
#define GL_DRAW_FRAMEBUFFER_BINDING       GL_FRAMEBUFFER_BINDING
#define GL_RENDERBUFFER_BINDING           0x8CA7
#define GL_READ_FRAMEBUFFER               0x8CA8
#define GL_DRAW_FRAMEBUFFER               0x8CA9
#define GL_READ_FRAMEBUFFER_BINDING       0x8CAA
#define GL_RENDERBUFFER_SAMPLES           0x8CAB
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE 0x8CD0
#define GL_FRAMEBUFFER_ATTACHMENT_OBJECT_NAME 0x8CD1
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL 0x8CD2
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE 0x8CD3
#define GL_FRAMEBUFFER_ATTACHMENT_TEXTURE_LAYER 0x8CD4
#define GL_FRAMEBUFFER_COMPLETE           0x8CD5
#define GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT 0x8CD6
#define GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT 0x8CD7
#define GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER 0x8CDB
#define GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER 0x8CDC
#define GL_FRAMEBUFFER_UNSUPPORTED        0x8CDD
#define GL_MAX_COLOR_ATTACHMENTS          0x8CDF
#define GL_COLOR_ATTACHMENT0              0x8CE0
#define GL_COLOR_ATTACHMENT1              0x8CE1
#define GL_COLOR_ATTACHMENT2              0x8CE2
#define GL_COLOR_ATTACHMENT3              0x8CE3
#define GL_COLOR_ATTACHMENT4              0x8CE4
#define GL_COLOR_ATTACHMENT5              0x8CE5
#define GL_COLOR_ATTACHMENT6              0x8CE6
#define GL_COLOR_ATTACHMENT7              0x8CE7
#define GL_COLOR_ATTACHMENT8              0x8CE8
#define GL_COLOR_ATTACHMENT9              0x8CE9
#define GL_COLOR_ATTACHMENT10             0x8CEA
#define GL_COLOR_ATTACHMENT11             0x8CEB
#define GL_COLOR_ATTACHMENT12             0x8CEC
#define GL_COLOR_ATTACHMENT13             0x8CED
#define GL_COLOR_ATTACHMENT14             0x8CEE
#define GL_COLOR_ATTACHMENT15             0x8CEF
#define GL_DEPTH_ATTACHMENT               0x8D00
#define GL_STENCIL_ATTACHMENT             0x8D20
#define GL_FRAMEBUFFER                    0x8D40
#define GL_RENDERBUFFER                   0x8D41
#define GL_RENDERBUFFER_WIDTH             0x8D42
#define GL_RENDERBUFFER_HEIGHT            0x8D43
#define GL_RENDERBUFFER_INTERNAL_FORMAT   0x8D44
#define GL_STENCIL_INDEX1                 0x8D46
#define GL_STENCIL_INDEX4                 0x8D47
#define GL_STENCIL_INDEX8                 0x8D48
#define GL_STENCIL_INDEX16                0x8D49
#define GL_RENDERBUFFER_RED_SIZE          0x8D50
#define GL_RENDERBUFFER_GREEN_SIZE        0x8D51
#define GL_RENDERBUFFER_BLUE_SIZE         0x8D52
#define GL_RENDERBUFFER_ALPHA_SIZE        0x8D53
#define GL_RENDERBUFFER_DEPTH_SIZE        0x8D54
#define GL_RENDERBUFFER_STENCIL_SIZE      0x8D55
#define GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE 0x8D56
#define GL_MAX_SAMPLES                    0x8D57

#ifndef GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT
#define GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT   0x8CD8
#endif // GL_FRAMEBUFFER_INCOMPLETE_DUPLICATE_ATTACHMENT
// not defined in original extension, nor in GL_ARB_framebuffer_object

extern GLboolean (GLApi *GLFuncPtrName(glIsRenderbuffer))(GLuint);
extern void (GLApi *GLFuncPtrName(glBindRenderbuffer))(GLenum, GLuint);
extern void (GLApi *GLFuncPtrName(glDeleteRenderbuffers))(GLsizei, const GLuint *);
extern void (GLApi *GLFuncPtrName(glGenRenderbuffers))(GLsizei, GLuint *);
extern void (GLApi *GLFuncPtrName(glRenderbufferStorage))(GLenum, GLenum, GLsizei, GLsizei);
extern void (GLApi *GLFuncPtrName(glGetRenderbufferParameteriv))(GLenum, GLenum, GLint *);
extern GLboolean (GLApi *GLFuncPtrName(glIsFramebuffer))(GLuint);
extern void (GLApi *GLFuncPtrName(glBindFramebuffer))(GLenum, GLuint);
extern void (GLApi *GLFuncPtrName(glDeleteFramebuffers))(GLsizei, const GLuint *);
extern void (GLApi *GLFuncPtrName(glGenFramebuffers))(GLsizei, GLuint *);
extern GLenum (GLApi *GLFuncPtrName(glCheckFramebufferStatus))(GLenum);
extern void (GLApi *GLFuncPtrName(glFramebufferTexture1D))(GLenum, GLenum, GLenum, GLuint, GLint);
extern void (GLApi *GLFuncPtrName(glFramebufferTexture2D))(GLenum, GLenum, GLenum, GLuint, GLint);
extern void (GLApi *GLFuncPtrName(glFramebufferTexture3D))(GLenum, GLenum, GLenum, GLuint, GLint, GLint);
extern void (GLApi *GLFuncPtrName(glFramebufferRenderbuffer))(GLenum, GLenum, GLenum, GLuint);
extern void (GLApi *GLFuncPtrName(glGetFramebufferAttachmentParameteriv))(GLenum, GLenum, GLenum, GLint *);
extern void (GLApi *GLFuncPtrName(glGenerateMipmap))(GLenum);
extern void (GLApi *GLFuncPtrName(glBlitFramebuffer))(GLint, GLint, GLint, GLint, GLint, GLint, GLint, GLint, GLbitfield, GLenum);
extern void (GLApi *GLFuncPtrName(glRenderbufferStorageMultisample))(GLenum, GLsizei, GLenum, GLsizei, GLsizei);
extern void (GLApi *GLFuncPtrName(glFramebufferTextureLayer))(GLenum, GLenum, GLuint, GLint, GLint);

#define glIsRenderbuffer GLFuncPtrName(glIsRenderbuffer)
#define glBindRenderbuffer GLFuncPtrName(glBindRenderbuffer)
#define glDeleteRenderbuffers GLFuncPtrName(glDeleteRenderbuffers)
#define glGenRenderbuffers GLFuncPtrName(glGenRenderbuffers)
#define glRenderbufferStorage GLFuncPtrName(glRenderbufferStorage)
#define glGetRenderbufferParameteriv GLFuncPtrName(glGetRenderbufferParameteriv)
#define glIsFramebuffer GLFuncPtrName(glIsFramebuffer)
#define glBindFramebuffer GLFuncPtrName(glBindFramebuffer)
#define glDeleteFramebuffers GLFuncPtrName(glDeleteFramebuffers)
#define glGenFramebuffers GLFuncPtrName(glGenFramebuffers)
#define glCheckFramebufferStatus GLFuncPtrName(glCheckFramebufferStatus)
#define glFramebufferTexture1D GLFuncPtrName(glFramebufferTexture1D)
#define glFramebufferTexture2D GLFuncPtrName(glFramebufferTexture2D)
#define glFramebufferTexture3D GLFuncPtrName(glFramebufferTexture3D)
#define glFramebufferRenderbuffer GLFuncPtrName(glFramebufferRenderbuffer)
#define glGetFramebufferAttachmentParameteriv GLFuncPtrName(glGetFramebufferAttachmentParameteriv)
#define glGenerateMipmap GLFuncPtrName(glGenerateMipmap)
#define glBlitFramebuffer GLFuncPtrName(glBlitFramebuffer)
#define glRenderbufferStorageMultisample GLFuncPtrName(glRenderbufferStorageMultisample)
#define glFramebufferTextureLayer GLFuncPtrName(glFramebufferTextureLayer)

#endif // GL_ARB_framebuffer_object

//								--- GL_ARB_framebuffer_sRGB (part of OpenGL 3.0 core) ---

#ifndef GL_ARB_framebuffer_sRGB
#define GL_ARB_framebuffer_sRGB 1

#define GL_FRAMEBUFFER_SRGB               0x8DB9

// no prototypes here

#endif // GL_ARB_framebuffer_sRGB

//								--- GL_ARB_half_float_vertex (part of OpenGL 3.0 core) ---

#ifndef GL_ARB_half_float_vertex
#define GL_ARB_half_float_vertex 1

#define GL_HALF_FLOAT                     0x140B

// no prototypes here

#endif // GL_ARB_half_float_vertex

//								--- GL_ARB_map_buffer_range (part of OpenGL 3.0 core) ---

#ifndef GL_ARB_map_buffer_range
#define GL_ARB_map_buffer_range 1
#define __GENERATE_GL_ARB_map_buffer_range__

#define GL_MAP_READ_BIT                   0x0001
#define GL_MAP_WRITE_BIT                  0x0002
#define GL_MAP_INVALIDATE_RANGE_BIT       0x0004
#define GL_MAP_INVALIDATE_BUFFER_BIT      0x0008
#define GL_MAP_FLUSH_EXPLICIT_BIT         0x0010
#define GL_MAP_UNSYNCHRONIZED_BIT         0x0020

extern GLvoid* (GLApi *GLFuncPtrName(glMapBufferRange))(GLenum, GLintptr, GLsizeiptr, GLbitfield);
extern void (GLApi *GLFuncPtrName(glFlushMappedBufferRange))(GLenum, GLintptr, GLsizeiptr);

#define glMapBufferRange GLFuncPtrName(glMapBufferRange)
#define glFlushMappedBufferRange GLFuncPtrName(glFlushMappedBufferRange)

#endif // GL_ARB_map_buffer_range

//								--- GL_ARB_texture_compression_rgtc (part of OpenGL 3.0 core) ---

#ifndef GL_ARB_texture_compression_rgtc
#define GL_ARB_texture_compression_rgtc 1

#define GL_COMPRESSED_RED_RGTC1           0x8DBB
#define GL_COMPRESSED_SIGNED_RED_RGTC1    0x8DBC
#define GL_COMPRESSED_RG_RGTC2            0x8DBD
#define GL_COMPRESSED_SIGNED_RG_RGTC2     0x8DBE

// no prototypes here

#endif // GL_ARB_texture_compression_rgtc

//								--- GL_ARB_texture_rg (part of OpenGL 3.0 core) ---

#ifndef GL_ARB_texture_rg
#define GL_ARB_texture_rg 1

#define GL_RG                             0x8227
#define GL_RG_INTEGER                     0x8228
#define GL_R8                             0x8229
#define GL_R16                            0x822A
#define GL_RG8                            0x822B
#define GL_RG16                           0x822C
#define GL_R16F                           0x822D
#define GL_R32F                           0x822E
#define GL_RG16F                          0x822F
#define GL_RG32F                          0x8230
#define GL_R8I                            0x8231
#define GL_R8UI                           0x8232
#define GL_R16I                           0x8233
#define GL_R16UI                          0x8234
#define GL_R32I                           0x8235
#define GL_R32UI                          0x8236
#define GL_RG8I                           0x8237
#define GL_RG8UI                          0x8238
#define GL_RG16I                          0x8239
#define GL_RG16UI                         0x823A
#define GL_RG32I                          0x823B
#define GL_RG32UI                         0x823C

// no prototypes here

#endif // GL_ARB_texture_rg

//								--- GL_ARB_vertex_array_object (part of OpenGL 3.0 core) ---

#ifndef GL_ARB_vertex_array_object
#define GL_ARB_vertex_array_object 1
#define __GENERATE_GL_ARB_vertex_array_object__

#define GL_VERTEX_ARRAY_BINDING           0x85B5

extern void (GLApi *GLFuncPtrName(glBindVertexArray))(GLuint);
extern void (GLApi *GLFuncPtrName(glDeleteVertexArrays))(GLsizei, const GLuint *);
extern void (GLApi *GLFuncPtrName(glGenVertexArrays))(GLsizei, GLuint *);
extern GLboolean (GLApi *GLFuncPtrName(glIsVertexArray))(GLuint);

#define glBindVertexArray GLFuncPtrName(glBindVertexArray)
#define glDeleteVertexArrays GLFuncPtrName(glDeleteVertexArrays)
#define glGenVertexArrays GLFuncPtrName(glGenVertexArrays)
#define glIsVertexArray GLFuncPtrName(glIsVertexArray)

#endif // GL_ARB_vertex_array_object

//								--- GL_ARB_uniform_buffer_object (part of OpenGL 3.1 core) ---

#ifndef GL_ARB_uniform_buffer_object
#define GL_ARB_uniform_buffer_object 1
#define __GENERATE_GL_ARB_uniform_buffer_object__

#define GL_UNIFORM_BUFFER                 0x8A11
#define GL_UNIFORM_BUFFER_BINDING         0x8A28
#define GL_UNIFORM_BUFFER_START           0x8A29
#define GL_UNIFORM_BUFFER_SIZE            0x8A2A
#define GL_MAX_VERTEX_UNIFORM_BLOCKS      0x8A2B
#define GL_MAX_GEOMETRY_UNIFORM_BLOCKS    0x8A2C
#define GL_MAX_FRAGMENT_UNIFORM_BLOCKS    0x8A2D
#define GL_MAX_COMBINED_UNIFORM_BLOCKS    0x8A2E
#define GL_MAX_UNIFORM_BUFFER_BINDINGS    0x8A2F
#define GL_MAX_UNIFORM_BLOCK_SIZE         0x8A30
#define GL_MAX_COMBINED_VERTEX_UNIFORM_COMPONENTS 0x8A31
#define GL_MAX_COMBINED_GEOMETRY_UNIFORM_COMPONENTS 0x8A32
#define GL_MAX_COMBINED_FRAGMENT_UNIFORM_COMPONENTS 0x8A33
#define GL_UNIFORM_BUFFER_OFFSET_ALIGNMENT 0x8A34
#define GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH 0x8A35
#define GL_ACTIVE_UNIFORM_BLOCKS          0x8A36
#define GL_UNIFORM_TYPE                   0x8A37
#define GL_UNIFORM_SIZE                   0x8A38
#define GL_UNIFORM_NAME_LENGTH            0x8A39
#define GL_UNIFORM_BLOCK_INDEX            0x8A3A
#define GL_UNIFORM_OFFSET                 0x8A3B
#define GL_UNIFORM_ARRAY_STRIDE           0x8A3C
#define GL_UNIFORM_MATRIX_STRIDE          0x8A3D
#define GL_UNIFORM_IS_ROW_MAJOR           0x8A3E
#define GL_UNIFORM_BLOCK_BINDING          0x8A3F
#define GL_UNIFORM_BLOCK_DATA_SIZE        0x8A40
#define GL_UNIFORM_BLOCK_NAME_LENGTH      0x8A41
#define GL_UNIFORM_BLOCK_ACTIVE_UNIFORMS  0x8A42
#define GL_UNIFORM_BLOCK_ACTIVE_UNIFORM_INDICES 0x8A43
#define GL_UNIFORM_BLOCK_REFERENCED_BY_VERTEX_SHADER 0x8A44
#define GL_UNIFORM_BLOCK_REFERENCED_BY_GEOMETRY_SHADER 0x8A45
#define GL_UNIFORM_BLOCK_REFERENCED_BY_FRAGMENT_SHADER 0x8A46
#define GL_INVALID_INDEX                  0xFFFFFFFFu

extern void (GLApi *GLFuncPtrName(glGetUniformIndices))(GLuint, GLsizei, const GLchar* *, GLuint *);
extern void (GLApi *GLFuncPtrName(glGetActiveUniformsiv))(GLuint, GLsizei, const GLuint *, GLenum, GLint *);
extern void (GLApi *GLFuncPtrName(glGetActiveUniformName))(GLuint, GLuint, GLsizei, GLsizei *, GLchar *);
extern GLuint (GLApi *GLFuncPtrName(glGetUniformBlockIndex))(GLuint, const GLchar *);
extern void (GLApi *GLFuncPtrName(glGetActiveUniformBlockiv))(GLuint, GLuint, GLenum, GLint *);
extern void (GLApi *GLFuncPtrName(glGetActiveUniformBlockName))(GLuint, GLuint, GLsizei, GLsizei *, GLchar *);
extern void (GLApi *GLFuncPtrName(glUniformBlockBinding))(GLuint, GLuint, GLuint);

#define glGetUniformIndices GLFuncPtrName(glGetUniformIndices)
#define glGetActiveUniformsiv GLFuncPtrName(glGetActiveUniformsiv)
#define glGetActiveUniformName GLFuncPtrName(glGetActiveUniformName)
#define glGetUniformBlockIndex GLFuncPtrName(glGetUniformBlockIndex)
#define glGetActiveUniformBlockiv GLFuncPtrName(glGetActiveUniformBlockiv)
#define glGetActiveUniformBlockName GLFuncPtrName(glGetActiveUniformBlockName)
#define glUniformBlockBinding GLFuncPtrName(glUniformBlockBinding)

#endif // GL_ARB_uniform_buffer_object

//								--- GL_ARB_draw_instanced (part of OpenGL 3.1 core) ---

#ifndef GL_ARB_draw_instanced
#define GL_ARB_draw_instanced 1
#define __GENERATE_GL_ARB_draw_instanced__

extern void (GLApi *GLFuncPtrName(glDrawArraysInstanced))(GLenum, GLint, GLsizei, GLsizei);
extern void (GLApi *GLFuncPtrName(glDrawElementsInstanced))(GLenum, GLsizei, GLenum, const GLvoid *, GLsizei);

#define glDrawArraysInstanced GLFuncPtrName(glDrawArraysInstanced)
#define glDrawElementsInstanced GLFuncPtrName(glDrawElementsInstanced)

#endif // GL_ARB_draw_instanced

//								--- GL_ARB_copy_buffer (part of OpenGL 3.1 core) ---

#ifndef GL_ARB_copy_buffer
#define GL_ARB_copy_buffer 1
#define __GENERATE_GL_ARB_copy_buffer__

#define GL_COPY_READ_BUFFER               0x8F36
#define GL_COPY_WRITE_BUFFER              0x8F37

extern void (GLApi *GLFuncPtrName(glCopyBufferSubData))(GLenum, GLenum, GLintptr, GLintptr, GLsizeiptr);

#define glCopyBufferSubData GLFuncPtrName(glCopyBufferSubData)

#endif // GL_ARB_copy_buffer

//								--- GL_ARB_depth_clamp (part of OpenGL 3.2 core) ---

#ifndef GL_ARB_depth_clamp
#define GL_ARB_depth_clamp 1

#define GL_DEPTH_CLAMP                    0x864F

// no prototypes here

#endif // GL_ARB_depth_clamp

//								--- GL_ARB_draw_elements_base_vertex (part of OpenGL 3.2 core) ---

#ifndef GL_ARB_draw_elements_base_vertex
#define GL_ARB_draw_elements_base_vertex 1
#define __GENERATE_GL_ARB_draw_elements_base_vertex__

extern void (GLApi *GLFuncPtrName(glDrawElementsBaseVertex))(GLenum, GLsizei, GLenum, const GLvoid *, GLint);
extern void (GLApi *GLFuncPtrName(glDrawRangeElementsBaseVertex))(GLenum, GLuint, GLuint, GLsizei, GLenum, const GLvoid *, GLint);
extern void (GLApi *GLFuncPtrName(glDrawElementsInstancedBaseVertex))(GLenum, GLsizei, GLenum, const GLvoid *, GLsizei, GLint);
extern void (GLApi *GLFuncPtrName(glMultiDrawElementsBaseVertex))(GLenum, const GLsizei *, GLenum, const GLvoid* *, GLsizei, const GLint *);

#define glDrawElementsBaseVertex GLFuncPtrName(glDrawElementsBaseVertex)
#define glDrawRangeElementsBaseVertex GLFuncPtrName(glDrawRangeElementsBaseVertex)
#define glDrawElementsInstancedBaseVertex GLFuncPtrName(glDrawElementsInstancedBaseVertex)
#define glMultiDrawElementsBaseVertex GLFuncPtrName(glMultiDrawElementsBaseVertex)

#endif // GL_ARB_draw_elements_base_vertex

//								--- GL_ARB_fragment_coord_conventions (part of OpenGL 3.2 core) ---

#ifndef GL_ARB_fragment_coord_conventions
#define GL_ARB_fragment_coord_conventions 1

// no prototypes here

#endif // GL_ARB_fragment_coord_conventions

//								--- GL_ARB_provoking_vertex (part of OpenGL 3.2 core) ---

#ifndef GL_ARB_provoking_vertex
#define GL_ARB_provoking_vertex 1
#define __GENERATE_GL_ARB_provoking_vertex__

#define GL_QUADS_FOLLOW_PROVOKING_VERTEX_CONVENTION 0x8E4C
#define GL_FIRST_VERTEX_CONVENTION        0x8E4D
#define GL_LAST_VERTEX_CONVENTION         0x8E4E
#define GL_PROVOKING_VERTEX               0x8E4F

extern void (GLApi *GLFuncPtrName(glProvokingVertex))(GLenum);

#define glProvokingVertex GLFuncPtrName(glProvokingVertex)

#endif // GL_ARB_provoking_vertex

//								--- GL_ARB_seamless_cube_map (part of OpenGL 3.2 core) ---

#ifndef GL_ARB_seamless_cube_map
#define GL_ARB_seamless_cube_map 1

#define GL_TEXTURE_CUBE_MAP_SEAMLESS      0x884F

// no prototypes here

#endif // GL_ARB_seamless_cube_map

//								--- GL_ARB_sync (part of OpenGL 3.2 core) ---

#ifndef GL_ARB_sync
#define GL_ARB_sync 1
#define __GENERATE_GL_ARB_sync__

#define GL_MAX_SERVER_WAIT_TIMEOUT        0x9111
#define GL_OBJECT_TYPE                    0x9112
#define GL_SYNC_CONDITION                 0x9113
#define GL_SYNC_STATUS                    0x9114
#define GL_SYNC_FLAGS                     0x9115
#define GL_SYNC_FENCE                     0x9116
#define GL_SYNC_GPU_COMMANDS_COMPLETE     0x9117
#define GL_UNSIGNALED                     0x9118
#define GL_SIGNALED                       0x9119
#define GL_ALREADY_SIGNALED               0x911A
#define GL_TIMEOUT_EXPIRED                0x911B
#define GL_CONDITION_SATISFIED            0x911C
#define GL_WAIT_FAILED                    0x911D
#define GL_SYNC_FLUSH_COMMANDS_BIT        0x00000001
#define GL_TIMEOUT_IGNORED                0xFFFFFFFFFFFFFFFFull

extern GLsync (GLApi *GLFuncPtrName(glFenceSync))(GLenum, GLbitfield);
extern GLboolean (GLApi *GLFuncPtrName(glIsSync))(GLsync);
extern void (GLApi *GLFuncPtrName(glDeleteSync))(GLsync);
extern GLenum (GLApi *GLFuncPtrName(glClientWaitSync))(GLsync, GLbitfield, GLuint64);
extern void (GLApi *GLFuncPtrName(glWaitSync))(GLsync, GLbitfield, GLuint64);
extern void (GLApi *GLFuncPtrName(glGetInteger64v))(GLenum, GLint64 *);
extern void (GLApi *GLFuncPtrName(glGetSynciv))(GLsync, GLenum, GLsizei, GLsizei *, GLint *);

#define glFenceSync GLFuncPtrName(glFenceSync)
#define glIsSync GLFuncPtrName(glIsSync)
#define glDeleteSync GLFuncPtrName(glDeleteSync)
#define glClientWaitSync GLFuncPtrName(glClientWaitSync)
#define glWaitSync GLFuncPtrName(glWaitSync)
#define glGetInteger64v GLFuncPtrName(glGetInteger64v)
#define glGetSynciv GLFuncPtrName(glGetSynciv)

#endif // GL_ARB_sync

//								--- GL_ARB_texture_multisample (part of OpenGL 3.2 core) ---

#ifndef GL_ARB_texture_multisample
#define GL_ARB_texture_multisample 1
#define __GENERATE_GL_ARB_texture_multisample__

#define GL_SAMPLE_POSITION                0x8E50
#define GL_SAMPLE_MASK                    0x8E51
#define GL_SAMPLE_MASK_VALUE              0x8E52
#define GL_MAX_SAMPLE_MASK_WORDS          0x8E59
#define GL_TEXTURE_2D_MULTISAMPLE         0x9100
#define GL_PROXY_TEXTURE_2D_MULTISAMPLE   0x9101
#define GL_TEXTURE_2D_MULTISAMPLE_ARRAY   0x9102
#define GL_PROXY_TEXTURE_2D_MULTISAMPLE_ARRAY 0x9103
#define GL_TEXTURE_BINDING_2D_MULTISAMPLE 0x9104
#define GL_TEXTURE_BINDING_2D_MULTISAMPLE_ARRAY 0x9105
#define GL_TEXTURE_SAMPLES                0x9106
#define GL_TEXTURE_FIXED_SAMPLE_LOCATIONS 0x9107
#define GL_SAMPLER_2D_MULTISAMPLE         0x9108
#define GL_INT_SAMPLER_2D_MULTISAMPLE     0x9109
#define GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE 0x910A
#define GL_SAMPLER_2D_MULTISAMPLE_ARRAY   0x910B
#define GL_INT_SAMPLER_2D_MULTISAMPLE_ARRAY 0x910C
#define GL_UNSIGNED_INT_SAMPLER_2D_MULTISAMPLE_ARRAY 0x910D
#define GL_MAX_COLOR_TEXTURE_SAMPLES      0x910E
#define GL_MAX_DEPTH_TEXTURE_SAMPLES      0x910F
#define GL_MAX_INTEGER_SAMPLES            0x9110

extern void (GLApi *GLFuncPtrName(glTexImage2DMultisample))(GLenum, GLsizei, GLint, GLsizei, GLsizei, GLboolean);
extern void (GLApi *GLFuncPtrName(glTexImage3DMultisample))(GLenum, GLsizei, GLint, GLsizei, GLsizei, GLsizei, GLboolean);
extern void (GLApi *GLFuncPtrName(glGetMultisamplefv))(GLenum, GLuint, GLfloat *);
extern void (GLApi *GLFuncPtrName(glSampleMaski))(GLuint, GLbitfield);

#define glTexImage2DMultisample GLFuncPtrName(glTexImage2DMultisample)
#define glTexImage3DMultisample GLFuncPtrName(glTexImage3DMultisample)
#define glGetMultisamplefv GLFuncPtrName(glGetMultisamplefv)
#define glSampleMaski GLFuncPtrName(glSampleMaski)

#endif // GL_ARB_texture_multisample

//								--- GL_ARB_vertex_array_bgra (part of OpenGL 3.2 core) ---

#ifndef GL_ARB_vertex_array_bgra
#define GL_ARB_vertex_array_bgra 1

/* reuse GL_BGRA */

// no prototypes here

#endif // GL_ARB_vertex_array_bgra

//								--- GL_ARB_draw_buffers_blend ---

#ifndef GL_ARB_draw_buffers_blend
#define GL_ARB_draw_buffers_blend 1
#define __GENERATE_GL_ARB_draw_buffers_blend__

extern bool GLEH_ARB_draw_buffers_blend; /**< set if GL_ARB_draw_buffers_blend is supported */

extern void (GLApi *GLFuncPtrName(glBlendEquationi))(GLuint, GLenum);
extern void (GLApi *GLFuncPtrName(glBlendEquationSeparatei))(GLuint, GLenum, GLenum);
extern void (GLApi *GLFuncPtrName(glBlendFunci))(GLuint, GLenum, GLenum);
extern void (GLApi *GLFuncPtrName(glBlendFuncSeparatei))(GLuint, GLenum, GLenum, GLenum, GLenum);

#define glBlendEquationi GLFuncPtrName(glBlendEquationi)
#define glBlendEquationSeparatei GLFuncPtrName(glBlendEquationSeparatei)
#define glBlendFunci GLFuncPtrName(glBlendFunci)
#define glBlendFuncSeparatei GLFuncPtrName(glBlendFuncSeparatei)

#endif // GL_ARB_draw_buffers_blend

//								--- GL_ARB_sample_shading ---

#ifndef GL_ARB_sample_shading
#define GL_ARB_sample_shading 1
#define __GENERATE_GL_ARB_sample_shading__

extern bool GLEH_ARB_sample_shading; /**< set if GL_ARB_sample_shading is supported */

#define GL_SAMPLE_SHADING                 0x8C36
#define GL_MIN_SAMPLE_SHADING_VALUE       0x8C37

extern void (GLApi *GLFuncPtrName(glMinSampleShading))(GLclampf);

#define glMinSampleShading GLFuncPtrName(glMinSampleShading)

#endif // GL_ARB_sample_shading

//								--- GL_ARB_texture_cube_map_array ---

#ifndef GL_ARB_texture_cube_map_array
#define GL_ARB_texture_cube_map_array 1

extern bool GLEH_ARB_texture_cube_map_array; /**< set if GL_ARB_texture_cube_map_array is supported */

#define GL_TEXTURE_CUBE_MAP_ARRAY         0x9009
#define GL_TEXTURE_BINDING_CUBE_MAP_ARRAY 0x900A
#define GL_PROXY_TEXTURE_CUBE_MAP_ARRAY   0x900B
#define GL_SAMPLER_CUBE_MAP_ARRAY         0x900C
#define GL_SAMPLER_CUBE_MAP_ARRAY_SHADOW  0x900D
#define GL_INT_SAMPLER_CUBE_MAP_ARRAY     0x900E
#define GL_UNSIGNED_INT_SAMPLER_CUBE_MAP_ARRAY 0x900F

// no prototypes here

#endif // GL_ARB_texture_cube_map_array

//								--- GL_ARB_texture_gather ---

#ifndef GL_ARB_texture_gather
#define GL_ARB_texture_gather 1

extern bool GLEH_ARB_texture_gather; /**< set if GL_ARB_texture_gather is supported */

#define GL_MIN_PROGRAM_TEXTURE_GATHER_OFFSET 0x8E5E
#define GL_MAX_PROGRAM_TEXTURE_GATHER_OFFSET 0x8E5F
#define GL_MAX_PROGRAM_TEXTURE_GATHER_COMPONENTS 0x8F9F

// no prototypes here (modifications to GLSL)

#endif // GL_ARB_texture_gather

//								--- GL_EXT_texture_swizzle ---

#ifndef GL_EXT_texture_swizzle
#define GL_EXT_texture_swizzle 1

extern bool GLEH_EXT_texture_swizzle; /**< set if GL_EXT_texture_swizzle is supported */

#define GL_TEXTURE_SWIZZLE_R_EXT               0x8E42
#define GL_TEXTURE_SWIZZLE_G_EXT               0x8E43
#define GL_TEXTURE_SWIZZLE_B_EXT               0x8E44
#define GL_TEXTURE_SWIZZLE_A_EXT               0x8E45
#define GL_TEXTURE_SWIZZLE_RGBA_EXT            0x8E46

// no prototypes here

#endif // GL_EXT_texture_swizzle

//								--- GL_ARB_texture_query_lod ---

#ifndef GL_ARB_texture_query_lod
#define GL_ARB_texture_query_lod 1

extern bool GLEH_ARB_texture_query_lod; /**< set if GL_ARB_texture_query_lod is supported */

// no prototypes here (modifications to GLSL)

#endif // GL_ARB_texture_query_lod

//								--- GL_EXT_timer_query ---

#ifndef GL_EXT_timer_query
#define GL_EXT_timer_query 1
#define __GENERATE_GL_EXT_timer_query__

extern bool GLEH_EXT_timer_query; /**< set if GL_EXT_timer_query is supported */

// Accepted by the <target> parameter of BeginQuery, EndQuery, and GetQueryiv:

#define GL_TIME_ELAPSED_EXT                               0x88BF

// New Procedures and Functions

extern void (GLApi *GLFuncPtrName(glGetQueryObjecti64vEXT))(GLuint id, GLenum pname, GLint64EXT *params);
extern void (GLApi *GLFuncPtrName(glGetQueryObjectui64vEXT))(GLuint id, GLenum pname, GLuint64EXT *params);

#define glGetQueryObjecti64vEXT GLFuncPtrName(glGetQueryObjecti64vEXT)
#define glGetQueryObjectui64vEXT GLFuncPtrName(glGetQueryObjectui64vEXT)

#endif // GL_EXT_timer_query

//								--- GL_EXT_draw_buffers2 ---

#ifndef GL_EXT_draw_buffers2
#define GL_EXT_draw_buffers2 1
#define __GENERATE_GL_EXT_draw_buffers2__

extern bool GLEH_EXT_draw_buffers2; /**< set if GL_EXT_draw_buffers2 is supported */

extern void (GLApi *GLFuncPtrName(glColorMaskIndexedEXT))(GLuint buf, GLboolean r, GLboolean g, GLboolean b, GLboolean a);
extern void (GLApi *GLFuncPtrName(glGetGLbooleanIndexedvEXT))(GLenum value, GLuint index, GLboolean *data);
extern void (GLApi *GLFuncPtrName(glGetIntegerIndexedvEXT))(GLenum value, GLuint index, GLint *data);
extern void (GLApi *GLFuncPtrName(glEnableIndexedEXT))(GLenum target, GLuint index);
extern void (GLApi *GLFuncPtrName(glDisableIndexedEXT))(GLenum target, GLuint index);
extern GLboolean (GLApi *GLFuncPtrName(glIsEnabledIndexedEXT))(GLenum target, GLuint index);

#define glColorMaskIndexedEXT GLFuncPtrName(glColorMaskIndexedEXT)
#define glGetGLbooleanIndexedvEXT GLFuncPtrName(glGetGLbooleanIndexedvEXT)
#define glGetIntegerIndexedvEXT GLFuncPtrName(glGetIntegerIndexedvEXT)
#define glEnableIndexedEXT GLFuncPtrName(glEnableIndexedEXT)
#define glDisableIndexedEXT GLFuncPtrName(glDisableIndexedEXT)
#define glIsEnabledIndexedEXT GLFuncPtrName(glIsEnabledIndexedEXT)

#endif // GL_EXT_draw_buffers2

//								--- GL_EXT_gpu_program_parameters ---

#ifndef GL_EXT_gpu_program_parameters
#define GL_EXT_gpu_program_parameters 1
#define __GENERATE_GL_EXT_gpu_program_parameters__

extern bool GLEH_EXT_gpu_program_parameters; /**< set if GL_EXT_gpu_program_parameters is supported */

extern void (GLApi *GLFuncPtrName(glProgramEnvParameters4fvEXT))(GLenum target, GLuint index, GLsizei count, const GLfloat *params);
extern void (GLApi *GLFuncPtrName(glProgramLocalParameters4fvEXT))(GLenum target, GLuint index, GLsizei count, const GLfloat *params);

#define glProgramEnvParameters4fvEXT GLFuncPtrName(glProgramEnvParameters4fvEXT)
#define glProgramLocalParameters4fvEXT GLFuncPtrName(glProgramLocalParameters4fvEXT)

#endif // GL_EXT_gpu_program_parameters

//								--- GL_EXT_bindable_uniform ---

#ifndef GL_EXT_bindable_uniform
#define GL_EXT_bindable_uniform 1
#define __GENERATE_GL_EXT_bindable_uniform__

extern bool GLEH_EXT_bindable_uniform; /**< set if GL_EXT_bindable_uniform is supported */

#define GL_MAX_VERTEX_BINDABLE_UNIFORMS_EXT                0x8DE2
#define GL_MAX_FRAGMENT_BINDABLE_UNIFORMS_EXT              0x8DE3
#define GL_MAX_GEOMETRY_BINDABLE_UNIFORMS_EXT              0x8DE4
#define GL_MAX_BINDABLE_UNIFORM_SIZE_EXT                   0x8DED
#define GL_UNIFORM_BUFFER_BINDING_EXT                      0x8DEF
#define GL_UNIFORM_BUFFER_EXT                              0x8DEE

extern void (GLApi *GLFuncPtrName(glUniformBufferEXT))(GLuint program, GLint location, GLuint buffer);
extern int (GLApi *GLFuncPtrName(glGetUniformBufferSizeEXT))(GLuint program, GLint location);
extern GLintptr (GLApi *GLFuncPtrName(glGetUniformOffsetEXT))(GLuint program, GLint location);

#define glUniformBufferEXT GLFuncPtrName(glUniformBufferEXT)
#define glGetUniformBufferSizeEXT GLFuncPtrName(glGetUniformBufferSizeEXT)
#define glGetUniformOffsetEXT GLFuncPtrName(glGetUniformOffsetEXT)

#endif // GL_EXT_bindable_uniform

//								--- GL_ARB_instanced_arrays (part of OpenGL 3.3 core) ---

#ifndef GL_ARB_instanced_arrays
#define GL_ARB_instanced_arrays 1
#define __GENERATE_GL_ARB_instanced_arrays

extern bool GLEH_ARB_instanced_arrays; /**< set if GL_ARB_instanced_arrays is supported */

#define GL_VERTEX_ATTRIB_ARRAY_DIVISOR_ARB                           0x88fe

extern void (GLApi *GLFuncPtrName(glVertexAttribDivisorARB))(GLuint, GLuint);

#define glVertexAttribDivisorARB GLFuncPtrName(glVertexAttribDivisorARB)

#endif // GL_ARB_instanced_arrays

//								--- GL_ARB_shader_texture_lod ---

#ifndef GL_ARB_shader_texture_lod
#define GL_ARB_shader_texture_lod 1

extern bool GLEH_ARB_shader_texture_lod; /**< set if GL_ARB_shader_texture_lod is supported */

// no new tokens
// no new procedures or functions

#endif // GL_ARB_shader_texture_lod

//								--- GL_ARB_texture_buffer_object ---

#ifndef GL_ARB_texture_buffer_object
#define GL_ARB_texture_buffer_object 1
#define __GENERATE_GL_ARB_texture_buffer_object

extern bool GLEH_ARB_texture_buffer_object; /**< set if GL_ARB_texture_buffer_object is supported */

#define GL_TEXTURE_BUFFER_ARB                                        0x8c2a
#define GL_MAX_TEXTURE_BUFFER_SIZE_ARB                               0x8c2b
#define GL_TEXTURE_BINDING_BUFFER_ARB                                0x8c2c
#define GL_TEXTURE_BUFFER_DATA_STORE_BINDING_ARB                     0x8c2d
#define GL_TEXTURE_BUFFER_FORMAT_ARB                                 0x8c2e

extern void (GLApi *GLFuncPtrName(glTexBufferARB))(GLenum, GLenum, GLuint);

#define glTexBufferARB GLFuncPtrName(glTexBufferARB)

#endif // GL_ARB_texture_buffer_object

//								--- GL_S3_s3tc ---

#ifndef GL_S3_s3tc
#define GL_S3_s3tc 1

extern bool GLEH_S3_s3tc; /**< set if GL_S3_s3tc is supported */

#define GL_RGB_S3TC                       0x83A0
#define GL_RGB4_S3TC                      0x83A1
#define GL_RGBA_S3TC                      0x83A2
#define GL_RGBA4_S3TC                     0x83A3

#endif // GL_S3_s3tc

//								--- GL_EXT_texture_compression_s3tc ---

#ifndef GL_EXT_texture_compression_s3tc
#define GL_EXT_texture_compression_s3tc 1

extern bool GLEH_EXT_texture_compression_s3tc; /**< set if GL_EXT_texture_compression_s3tc is supported */

#define GL_COMPRESSED_RGB_S3TC_DXT1_EXT   0x83F0
#define GL_COMPRESSED_RGBA_S3TC_DXT1_EXT  0x83F1
#define GL_COMPRESSED_RGBA_S3TC_DXT3_EXT  0x83F2
#define GL_COMPRESSED_RGBA_S3TC_DXT5_EXT  0x83F3

#endif // GL_EXT_texture_compression_s3tc

//								--- GL_EXT_texture_compression_latc ---

#ifndef GL_EXT_texture_compression_latc
#define GL_EXT_texture_compression_latc 1

extern bool GLEH_EXT_texture_compression_latc; /**< set if GL_EXT_texture_compression_latc is supported */

#define GL_COMPRESSED_LUMINANCE_LATC1_EXT 0x8C70
#define GL_COMPRESSED_SIGNED_LUMINANCE_LATC1_EXT 0x8C71
#define GL_COMPRESSED_LUMINANCE_ALPHA_LATC2_EXT 0x8C72
#define GL_COMPRESSED_SIGNED_LUMINANCE_ALPHA_LATC2_EXT 0x8C73

#endif // GL_EXT_texture_compression_latc

//								--- GL_EXT_texture_filter_anisotropic ---

#ifndef GL_EXT_texture_filter_anisotropic
#define GL_EXT_texture_filter_anisotropic 1

extern bool GLEH_EXT_texture_filter_anisotropic; /**< set if GL_EXT_texture_filter_anisotropic is supported */

#define GL_TEXTURE_MAX_ANISOTROPY     0x84FE
#define GL_MAX_TEXTURE_MAX_ANISOTROPY 0x84FF

#endif // GL_EXT_texture_filter_anisotropic

//								--- GL_EXT_abgr ---

#ifndef GL_EXT_abgr
#define GL_EXT_abgr 1

extern bool GLEH_EXT_abgr; /**< set if GL_EXT_abgr is supported */

#define GL_ABGR_EXT                       0x8000

#endif // GL_EXT_abgr

//								--- GL_ARB_sample_shading ---

#ifndef GL_ARB_sample_shading
#define GL_ARB_sample_shading 1
#define __GENERATE_GL_ARB_sample_shading

extern bool GLEH_ARB_sample_shading;

#define GL_SAMPLE_SHADING_ARB                                        0x8c36
#define GL_MIN_SAMPLE_SHADING_VALUE_ARB                              0x8c37

extern void (GLApi *GLFuncPtrName(glMinSampleShadingARB))(GLclampf);

#define glMinSampleShadingARB GLFuncPtrName(glMinSampleShadingARB)

#endif // GL_ARB_sample_shading

//								--- GL_ARB_texture_cube_map_array ---

#ifndef GL_ARB_texture_cube_map_array
#define GL_ARB_texture_cube_map_array 1

extern bool GLEH_ARB_texture_cube_map_array;

#define GL_TEXTURE_CUBE_MAP_ARRAY_ARB                                0x9009
#define GL_TEXTURE_BINDING_CUBE_MAP_ARRAY_ARB                        0x900a
#define GL_PROXY_TEXTURE_CUBE_MAP_ARRAY_ARB                          0x900b
#define GL_SAMPLER_CUBE_MAP_ARRAY_ARB                                0x900c
#define GL_SAMPLER_CUBE_MAP_ARRAY_SHADOW_ARB                         0x900d
#define GL_INT_SAMPLER_CUBE_MAP_ARRAY_ARB                            0x900e
#define GL_UNSIGNED_INT_SAMPLER_CUBE_MAP_ARRAY_ARB                   0x900f

// no new procedures or functions

#endif // GL_ARB_texture_cube_map_array

//								--- GL_ARB_texture_gather ---

#ifndef GL_ARB_texture_gather
#define GL_ARB_texture_gather 1

extern bool GLEH_ARB_texture_gather;

#define GL_MIN_PROGRAM_TEXTURE_GATHER_OFFSET_ARB                     0x8e5e
#define GL_MAX_PROGRAM_TEXTURE_GATHER_OFFSET_ARB                     0x8e5f
#define GL_MAX_PROGRAM_TEXTURE_GATHER_COMPONENTS_ARB                 0x8f9f

// no new procedures or functions

#endif // GL_ARB_texture_gather

//								--- GL_ARB_texture_query_lod ---

#ifndef GL_ARB_texture_query_lod
#define GL_ARB_texture_query_lod 1

extern bool GLEH_ARB_texture_query_lod;

// no new tokens
// no new procedures or functions

#endif // GL_ARB_texture_query_lod

//								--- GL_ARB_shading_language_include ---

#ifndef GL_ARB_shading_language_include
#define GL_ARB_shading_language_include 1
#define __GENERATE_GL_ARB_shading_language_include

extern bool GLEH_ARB_shading_language_include;

#define GL_SHADER_INCLUDE_ARB                                        0x8dae
#define GL_NAMED_STRING_LENGTH_ARB                                   0x8de9
#define GL_NAMED_STRING_TYPE_ARB                                     0x8dea

extern void (GLApi *GLFuncPtrName(glNamedStringARB))(GLenum, int, const GLchar*, int, const GLchar*);
extern void (GLApi *GLFuncPtrName(glDeleteNamedStringARB))(int, const GLchar*);
extern void (GLApi *GLFuncPtrName(glCompileShaderIncludeARB))(GLuint, GLsizei, const GLchar**, const int*);
extern GLboolean (GLApi *GLFuncPtrName(glIsNamedStringARB))(int, const GLchar*);
extern void (GLApi *GLFuncPtrName(glGetNamedStringARB))(int, const GLchar*, GLsizei, int*, GLchar*);
extern void (GLApi *GLFuncPtrName(glGetNamedStringivARB))(int, const GLchar*, GLenum, int*);

#define glNamedStringARB GLFuncPtrName(glNamedStringARB)
#define glDeleteNamedStringARB GLFuncPtrName(glDeleteNamedStringARB)
#define glCompileShaderIncludeARB GLFuncPtrName(glCompileShaderIncludeARB)
#define glIsNamedStringARB GLFuncPtrName(glIsNamedStringARB)
#define glGetNamedStringARB GLFuncPtrName(glGetNamedStringARB)
#define glGetNamedStringivARB GLFuncPtrName(glGetNamedStringivARB)

#endif // GL_ARB_shading_language_include

//								--- GL_ARB_texture_compression_bptc ---

#ifndef GL_ARB_texture_compression_bptc
#define GL_ARB_texture_compression_bptc 1

extern bool GLEH_ARB_texture_compression_bptc;

#define GL_COMPRESSED_RGBA_BPTC_UNORM_ARB                            0x8e8c
#define GL_COMPRESSED_SRGB_ALPHA_BPTC_UNORM_ARB                      0x8e8d
#define GL_COMPRESSED_RGB_BPTC_SIGNED_FLOAT_ARB                      0x8e8e
#define GL_COMPRESSED_RGB_BPTC_UNSIGNED_FLOAT_ARB                    0x8e8f

// no new procedures or functions

#endif // GL_ARB_texture_compression_bptc

//								--- GL_ARB_blend_func_extended (part of OpenGL 3.3 core) ---

#ifndef GL_ARB_blend_func_extended
#define GL_ARB_blend_func_extended 1
#define __GENERATE_GL_ARB_blend_func_extended

extern bool GLEH_ARB_blend_func_extended;

#define GL_SRC1_COLOR                                                0x88f9
#define GL_ONE_MINUS_SRC1_COLOR                                      0x88fa
#define GL_ONE_MINUS_SRC1_ALPHA                                      0x88fb
#define GL_MAX_DUAL_SOURCE_DRAW_BUFFERS                              0x88fc

extern void (GLApi *GLFuncPtrName(glBindFragDataLocationIndexed))(GLuint, GLuint, GLuint, const GLchar*);
extern int (GLApi *GLFuncPtrName(glGetFragDataIndex))(GLuint, const GLchar*);

#define glBindFragDataLocationIndexed GLFuncPtrName(glBindFragDataLocationIndexed)
#define glGetFragDataIndex GLFuncPtrName(glGetFragDataIndex)

#endif // GL_ARB_blend_func_extended

//								--- GL_ARB_explicit_attrib_location (part of OpenGL 3.3 core) ---

#ifndef GL_ARB_explicit_attrib_location
#define GL_ARB_explicit_attrib_location 1

extern bool GLEH_ARB_explicit_attrib_location;

// no new tokens
// no new procedures or functions

#endif // GL_ARB_explicit_attrib_location

//								--- GL_ARB_occlusion_query2 (part of OpenGL 3.3 core) ---

#ifndef GL_ARB_occlusion_query2
#define GL_ARB_occlusion_query2 1

extern bool GLEH_ARB_occlusion_query2;

#define GL_ANY_SAMPLES_PASSED                                        0x8c2f

// no new procedures or functions

#endif // GL_ARB_occlusion_query2

//								--- GL_ARB_sampler_objects (part of OpenGL 3.3 core) ---

#ifndef GL_ARB_sampler_objects
#define GL_ARB_sampler_objects 1
#define __GENERATE_GL_ARB_sampler_objects

extern bool GLEH_ARB_sampler_objects;

#define GL_SAMPLER_BINDING                                           0x8919

extern void (GLApi *GLFuncPtrName(glGenSamplers))(GLsizei, GLuint*);
extern void (GLApi *GLFuncPtrName(glDeleteSamplers))(GLsizei, const GLuint*);
extern GLboolean (GLApi *GLFuncPtrName(glIsSampler))(GLuint);
extern void (GLApi *GLFuncPtrName(glBindSampler))(GLuint, GLuint);
extern void (GLApi *GLFuncPtrName(glSamplerParameteri))(GLuint, GLenum, int);
extern void (GLApi *GLFuncPtrName(glSamplerParameterf))(GLuint, GLenum, float);
extern void (GLApi *GLFuncPtrName(glSamplerParameteriv))(GLuint, GLenum, const int*);
extern void (GLApi *GLFuncPtrName(glSamplerParameterfv))(GLuint, GLenum, const float*);
extern void (GLApi *GLFuncPtrName(glSamplerParameterIiv))(GLuint, GLenum, const int*);
extern void (GLApi *GLFuncPtrName(glSamplerParameterIuiv))(GLuint, GLenum, const GLuint*);
extern void (GLApi *GLFuncPtrName(glGetSamplerParameteriv))(GLuint, GLenum, int*);
extern void (GLApi *GLFuncPtrName(glGetSamplerParameterfv))(GLuint, GLenum, float*);
extern void (GLApi *GLFuncPtrName(glGetSamplerParameterIiv))(GLuint, GLenum, int*);
extern void (GLApi *GLFuncPtrName(glGetSamplerParameterIuiv))(GLuint, GLenum, GLuint*);

#define glGenSamplers GLFuncPtrName(glGenSamplers)
#define glDeleteSamplers GLFuncPtrName(glDeleteSamplers)
#define glIsSampler GLFuncPtrName(glIsSampler)
#define glBindSampler GLFuncPtrName(glBindSampler)
#define glSamplerParameteri GLFuncPtrName(glSamplerParameteri)
#define glSamplerParameterf GLFuncPtrName(glSamplerParameterf)
#define glSamplerParameteriv GLFuncPtrName(glSamplerParameteriv)
#define glSamplerParameterfv GLFuncPtrName(glSamplerParameterfv)
#define glSamplerParameterIiv GLFuncPtrName(glSamplerParameterIiv)
#define glSamplerParameterIuiv GLFuncPtrName(glSamplerParameterIuiv)
#define glGetSamplerParameteriv GLFuncPtrName(glGetSamplerParameteriv)
#define glGetSamplerParameterfv GLFuncPtrName(glGetSamplerParameterfv)
#define glGetSamplerParameterIiv GLFuncPtrName(glGetSamplerParameterIiv)
#define glGetSamplerParameterIuiv GLFuncPtrName(glGetSamplerParameterIuiv)

#endif // GL_ARB_sampler_objects

//								--- GL_ARB_shader_bit_encoding (part of OpenGL 3.3 core) ---

#ifndef GL_ARB_shader_bit_encoding
#define GL_ARB_shader_bit_encoding 1

extern bool GLEH_ARB_shader_bit_encoding;

// no new tokens
// no new procedures or functions

#endif // GL_ARB_shader_bit_encoding

//								--- GL_ARB_texture_rgb10_a2ui (part of OpenGL 3.3 core) ---

#ifndef GL_ARB_texture_rgb10_a2ui
#define GL_ARB_texture_rgb10_a2ui 1

extern bool GLEH_ARB_texture_rgb10_a2ui;

#define GL_RGB10_A2UI                                                0x906f

// no new procedures or functions

#endif // GL_ARB_texture_rgb10_a2ui

//								--- GL_ARB_texture_swizzle (part of OpenGL 3.3 core) ---

#ifndef GL_ARB_texture_swizzle
#define GL_ARB_texture_swizzle 1

extern bool GLEH_ARB_texture_swizzle;

#define GL_TEXTURE_SWIZZLE_R                                         0x8e42
#define GL_TEXTURE_SWIZZLE_G                                         0x8e43
#define GL_TEXTURE_SWIZZLE_B                                         0x8e44
#define GL_TEXTURE_SWIZZLE_A                                         0x8e45
#define GL_TEXTURE_SWIZZLE_RGBA                                      0x8e46

// no new procedures or functions

#endif // GL_ARB_texture_swizzle

//								--- GL_ARB_timer_query (part of OpenGL 3.3 core) ---

#ifndef GL_ARB_timer_query
#define GL_ARB_timer_query 1
#define __GENERATE_GL_ARB_timer_query

extern bool GLEH_ARB_timer_query;

#define GL_TIME_ELAPSED                                              0x88bf
#define GL_TIMESTAMP                                                 0x8e28

extern void (GLApi *GLFuncPtrName(glQueryCounter))(GLuint, GLenum);
extern void (GLApi *GLFuncPtrName(glGetQueryObjecti64v))(GLuint, GLenum, GLint64*);
extern void (GLApi *GLFuncPtrName(glGetQueryObjectui64v))(GLuint, GLenum, GLuint64*);

#define glQueryCounter GLFuncPtrName(glQueryCounter)
#define glGetQueryObjecti64v GLFuncPtrName(glGetQueryObjecti64v)
#define glGetQueryObjectui64v GLFuncPtrName(glGetQueryObjectui64v)

#endif // GL_ARB_timer_query

//								--- GL_ARB_vertex_type_2_10_10_10_rev (part of OpenGL 3.3 core) ---

#ifndef GL_ARB_vertex_type_2_10_10_10_rev
#define GL_ARB_vertex_type_2_10_10_10_rev 1
#define __GENERATE_GL_ARB_vertex_type_2_10_10_10_rev

extern bool GLEH_ARB_vertex_type_2_10_10_10_rev;

#define GL_UNSIGNED_INT_2_10_10_10_REV                               0x8368
#define GL_INT_2_10_10_10_REV                                        0x8d9f

extern void (GLApi *GLFuncPtrName(glVertexP2ui))(GLenum, GLuint);
extern void (GLApi *GLFuncPtrName(glVertexP3ui))(GLenum, GLuint);
extern void (GLApi *GLFuncPtrName(glVertexP4ui))(GLenum, GLuint);
extern void (GLApi *GLFuncPtrName(glVertexP2uiv))(GLenum, const GLuint*);
extern void (GLApi *GLFuncPtrName(glVertexP3uiv))(GLenum, const GLuint*);
extern void (GLApi *GLFuncPtrName(glVertexP4uiv))(GLenum, const GLuint*);
extern void (GLApi *GLFuncPtrName(glTexCoordP1ui))(GLenum, GLuint);
extern void (GLApi *GLFuncPtrName(glTexCoordP2ui))(GLenum, GLuint);
extern void (GLApi *GLFuncPtrName(glTexCoordP3ui))(GLenum, GLuint);
extern void (GLApi *GLFuncPtrName(glTexCoordP4ui))(GLenum, GLuint);
extern void (GLApi *GLFuncPtrName(glTexCoordP1uiv))(GLenum, const GLuint*);
extern void (GLApi *GLFuncPtrName(glTexCoordP2uiv))(GLenum, const GLuint*);
extern void (GLApi *GLFuncPtrName(glTexCoordP3uiv))(GLenum, const GLuint*);
extern void (GLApi *GLFuncPtrName(glTexCoordP4uiv))(GLenum, const GLuint*);
extern void (GLApi *GLFuncPtrName(glMultiTexCoordP1ui))(GLenum, GLenum, GLuint);
extern void (GLApi *GLFuncPtrName(glMultiTexCoordP2ui))(GLenum, GLenum, GLuint);
extern void (GLApi *GLFuncPtrName(glMultiTexCoordP3ui))(GLenum, GLenum, GLuint);
extern void (GLApi *GLFuncPtrName(glMultiTexCoordP4ui))(GLenum, GLenum, GLuint);
extern void (GLApi *GLFuncPtrName(glMultiTexCoordP1uiv))(GLenum, GLenum, const GLuint*);
extern void (GLApi *GLFuncPtrName(glMultiTexCoordP2uiv))(GLenum, GLenum, const GLuint*);
extern void (GLApi *GLFuncPtrName(glMultiTexCoordP3uiv))(GLenum, GLenum, const GLuint*);
extern void (GLApi *GLFuncPtrName(glMultiTexCoordP4uiv))(GLenum, GLenum, const GLuint*);
extern void (GLApi *GLFuncPtrName(glNormalP3ui))(GLenum, GLuint);
extern void (GLApi *GLFuncPtrName(glNormalP3uiv))(GLenum, const GLuint*);
extern void (GLApi *GLFuncPtrName(glColorP3ui))(GLenum, GLuint);
extern void (GLApi *GLFuncPtrName(glColorP4ui))(GLenum, GLuint);
extern void (GLApi *GLFuncPtrName(glColorP3uiv))(GLenum, const GLuint*);
extern void (GLApi *GLFuncPtrName(glColorP4uiv))(GLenum, const GLuint*);
extern void (GLApi *GLFuncPtrName(glSecondaryColorP3ui))(GLenum, GLuint);
extern void (GLApi *GLFuncPtrName(glSecondaryColorP3uiv))(GLenum, const GLuint*);
extern void (GLApi *GLFuncPtrName(glVertexAttribP1ui))(GLuint, GLenum, GLboolean, GLuint);
extern void (GLApi *GLFuncPtrName(glVertexAttribP2ui))(GLuint, GLenum, GLboolean, GLuint);
extern void (GLApi *GLFuncPtrName(glVertexAttribP3ui))(GLuint, GLenum, GLboolean, GLuint);
extern void (GLApi *GLFuncPtrName(glVertexAttribP4ui))(GLuint, GLenum, GLboolean, GLuint);
extern void (GLApi *GLFuncPtrName(glVertexAttribP1uiv))(GLuint, GLenum, GLboolean, const GLuint*);
extern void (GLApi *GLFuncPtrName(glVertexAttribP2uiv))(GLuint, GLenum, GLboolean, const GLuint*);
extern void (GLApi *GLFuncPtrName(glVertexAttribP3uiv))(GLuint, GLenum, GLboolean, const GLuint*);
extern void (GLApi *GLFuncPtrName(glVertexAttribP4uiv))(GLuint, GLenum, GLboolean, const GLuint*);

#define glVertexP2ui GLFuncPtrName(glVertexP2ui)
#define glVertexP3ui GLFuncPtrName(glVertexP3ui)
#define glVertexP4ui GLFuncPtrName(glVertexP4ui)
#define glVertexP2uiv GLFuncPtrName(glVertexP2uiv)
#define glVertexP3uiv GLFuncPtrName(glVertexP3uiv)
#define glVertexP4uiv GLFuncPtrName(glVertexP4uiv)
#define glTexCoordP1ui GLFuncPtrName(glTexCoordP1ui)
#define glTexCoordP2ui GLFuncPtrName(glTexCoordP2ui)
#define glTexCoordP3ui GLFuncPtrName(glTexCoordP3ui)
#define glTexCoordP4ui GLFuncPtrName(glTexCoordP4ui)
#define glTexCoordP1uiv GLFuncPtrName(glTexCoordP1uiv)
#define glTexCoordP2uiv GLFuncPtrName(glTexCoordP2uiv)
#define glTexCoordP3uiv GLFuncPtrName(glTexCoordP3uiv)
#define glTexCoordP4uiv GLFuncPtrName(glTexCoordP4uiv)
#define glMultiTexCoordP1ui GLFuncPtrName(glMultiTexCoordP1ui)
#define glMultiTexCoordP2ui GLFuncPtrName(glMultiTexCoordP2ui)
#define glMultiTexCoordP3ui GLFuncPtrName(glMultiTexCoordP3ui)
#define glMultiTexCoordP4ui GLFuncPtrName(glMultiTexCoordP4ui)
#define glMultiTexCoordP1uiv GLFuncPtrName(glMultiTexCoordP1uiv)
#define glMultiTexCoordP2uiv GLFuncPtrName(glMultiTexCoordP2uiv)
#define glMultiTexCoordP3uiv GLFuncPtrName(glMultiTexCoordP3uiv)
#define glMultiTexCoordP4uiv GLFuncPtrName(glMultiTexCoordP4uiv)
#define glNormalP3ui GLFuncPtrName(glNormalP3ui)
#define glNormalP3uiv GLFuncPtrName(glNormalP3uiv)
#define glColorP3ui GLFuncPtrName(glColorP3ui)
#define glColorP4ui GLFuncPtrName(glColorP4ui)
#define glColorP3uiv GLFuncPtrName(glColorP3uiv)
#define glColorP4uiv GLFuncPtrName(glColorP4uiv)
#define glSecondaryColorP3ui GLFuncPtrName(glSecondaryColorP3ui)
#define glSecondaryColorP3uiv GLFuncPtrName(glSecondaryColorP3uiv)
#define glVertexAttribP1ui GLFuncPtrName(glVertexAttribP1ui)
#define glVertexAttribP2ui GLFuncPtrName(glVertexAttribP2ui)
#define glVertexAttribP3ui GLFuncPtrName(glVertexAttribP3ui)
#define glVertexAttribP4ui GLFuncPtrName(glVertexAttribP4ui)
#define glVertexAttribP1uiv GLFuncPtrName(glVertexAttribP1uiv)
#define glVertexAttribP2uiv GLFuncPtrName(glVertexAttribP2uiv)
#define glVertexAttribP3uiv GLFuncPtrName(glVertexAttribP3uiv)
#define glVertexAttribP4uiv GLFuncPtrName(glVertexAttribP4uiv)

#endif // GL_ARB_vertex_type_2_10_10_10_rev

//								--- GL_ARB_draw_indirect ---

#ifndef GL_ARB_draw_indirect
#define GL_ARB_draw_indirect 1
#define __GENERATE_GL_ARB_draw_indirect

extern bool GLEH_ARB_draw_indirect;

#define GL_DRAW_INDIRECT_BUFFER                                      0x8f3f
#define GL_DRAW_INDIRECT_BUFFER_BINDING                              0x8f43

extern void (GLApi *GLFuncPtrName(glDrawArraysIndirect))(GLenum, const void*);
extern void (GLApi *GLFuncPtrName(glDrawElementsIndirect))(GLenum, GLenum, const void*);

#define glDrawArraysIndirect GLFuncPtrName(glDrawArraysIndirect)
#define glDrawElementsIndirect GLFuncPtrName(glDrawElementsIndirect)

#endif // GL_ARB_draw_indirect

//								--- GL_ARB_gpu_shader5 ---

#ifndef GL_ARB_gpu_shader5
#define GL_ARB_gpu_shader5 1

extern bool GLEH_ARB_gpu_shader5;

#define GL_GEOMETRY_SHADER_INVOCATIONS                               0x887f
#define GL_MAX_GEOMETRY_SHADER_INVOCATIONS                           0x8e5a
#define GL_MIN_FRAGMENT_INTERPOLATION_OFFSET                         0x8e5b
#define GL_MAX_FRAGMENT_INTERPOLATION_OFFSET                         0x8e5c
#define GL_FRAGMENT_INTERPOLATION_OFFSET_BITS                        0x8e5d
#define GL_MAX_VERTEX_STREAMS                                        0x8e71

// no new procedures or functions

#endif // GL_ARB_gpu_shader5

//								--- GL_ARB_gpu_shader_fp64 ---

#ifndef GL_ARB_gpu_shader_fp64
#define GL_ARB_gpu_shader_fp64 1
#define __GENERATE_GL_ARB_gpu_shader_fp64

extern bool GLEH_ARB_gpu_shader_fp64;

#define GL_DOUBLE_VEC2                                               0x8ffc
#define GL_DOUBLE_VEC3                                               0x8ffd
#define GL_DOUBLE_VEC4                                               0x8ffe
#define GL_DOUBLE_MAT2                                               0x8f46
#define GL_DOUBLE_MAT3                                               0x8f47
#define GL_DOUBLE_MAT4                                               0x8f48
#define GL_DOUBLE_MAT2x3                                             0x8f49
#define GL_DOUBLE_MAT2x4                                             0x8f4a
#define GL_DOUBLE_MAT3x2                                             0x8f4b
#define GL_DOUBLE_MAT3x4                                             0x8f4c
#define GL_DOUBLE_MAT4x2                                             0x8f4d
#define GL_DOUBLE_MAT4x3                                             0x8f4e

extern void (GLApi *GLFuncPtrName(glUniform1d))(int, double);
extern void (GLApi *GLFuncPtrName(glUniform2d))(int, double, double);
extern void (GLApi *GLFuncPtrName(glUniform3d))(int, double, double, double);
extern void (GLApi *GLFuncPtrName(glUniform4d))(int, double, double, double, double);
extern void (GLApi *GLFuncPtrName(glUniform1dv))(int, GLsizei, const double*);
extern void (GLApi *GLFuncPtrName(glUniform2dv))(int, GLsizei, const double*);
extern void (GLApi *GLFuncPtrName(glUniform3dv))(int, GLsizei, const double*);
extern void (GLApi *GLFuncPtrName(glUniform4dv))(int, GLsizei, const double*);
extern void (GLApi *GLFuncPtrName(glUniformMatrix2dv))(int, GLsizei, GLboolean, const double*);
extern void (GLApi *GLFuncPtrName(glUniformMatrix3dv))(int, GLsizei, GLboolean, const double*);
extern void (GLApi *GLFuncPtrName(glUniformMatrix4dv))(int, GLsizei, GLboolean, const double*);
extern void (GLApi *GLFuncPtrName(glUniformMatrix2x3dv))(int, GLsizei, GLboolean, const double*);
extern void (GLApi *GLFuncPtrName(glUniformMatrix2x4dv))(int, GLsizei, GLboolean, const double*);
extern void (GLApi *GLFuncPtrName(glUniformMatrix3x2dv))(int, GLsizei, GLboolean, const double*);
extern void (GLApi *GLFuncPtrName(glUniformMatrix3x4dv))(int, GLsizei, GLboolean, const double*);
extern void (GLApi *GLFuncPtrName(glUniformMatrix4x2dv))(int, GLsizei, GLboolean, const double*);
extern void (GLApi *GLFuncPtrName(glUniformMatrix4x3dv))(int, GLsizei, GLboolean, const double*);
extern void (GLApi *GLFuncPtrName(glGetUniformdv))(GLuint, int, double*);
extern void (GLApi *GLFuncPtrName(glProgramUniform1dEXT))(GLuint, int, double);
extern void (GLApi *GLFuncPtrName(glProgramUniform2dEXT))(GLuint, int, double, double);
extern void (GLApi *GLFuncPtrName(glProgramUniform3dEXT))(GLuint, int, double, double, double);
extern void (GLApi *GLFuncPtrName(glProgramUniform4dEXT))(GLuint, int, double, double, double, double);
extern void (GLApi *GLFuncPtrName(glProgramUniform1dvEXT))(GLuint, int, GLsizei, const double*);
extern void (GLApi *GLFuncPtrName(glProgramUniform2dvEXT))(GLuint, int, GLsizei, const double*);
extern void (GLApi *GLFuncPtrName(glProgramUniform3dvEXT))(GLuint, int, GLsizei, const double*);
extern void (GLApi *GLFuncPtrName(glProgramUniform4dvEXT))(GLuint, int, GLsizei, const double*);
extern void (GLApi *GLFuncPtrName(glProgramUniformMatrix2dvEXT))(GLuint, int, GLsizei, GLboolean, const double*);
extern void (GLApi *GLFuncPtrName(glProgramUniformMatrix3dvEXT))(GLuint, int, GLsizei, GLboolean, const double*);
extern void (GLApi *GLFuncPtrName(glProgramUniformMatrix4dvEXT))(GLuint, int, GLsizei, GLboolean, const double*);
extern void (GLApi *GLFuncPtrName(glProgramUniformMatrix2x3dvEXT))(GLuint, int, GLsizei, GLboolean, const double*);
extern void (GLApi *GLFuncPtrName(glProgramUniformMatrix2x4dvEXT))(GLuint, int, GLsizei, GLboolean, const double*);
extern void (GLApi *GLFuncPtrName(glProgramUniformMatrix3x2dvEXT))(GLuint, int, GLsizei, GLboolean, const double*);
extern void (GLApi *GLFuncPtrName(glProgramUniformMatrix3x4dvEXT))(GLuint, int, GLsizei, GLboolean, const double*);
extern void (GLApi *GLFuncPtrName(glProgramUniformMatrix4x2dvEXT))(GLuint, int, GLsizei, GLboolean, const double*);
extern void (GLApi *GLFuncPtrName(glProgramUniformMatrix4x3dvEXT))(GLuint, int, GLsizei, GLboolean, const double*);

#define glUniform1d GLFuncPtrName(glUniform1d)
#define glUniform2d GLFuncPtrName(glUniform2d)
#define glUniform3d GLFuncPtrName(glUniform3d)
#define glUniform4d GLFuncPtrName(glUniform4d)
#define glUniform1dv GLFuncPtrName(glUniform1dv)
#define glUniform2dv GLFuncPtrName(glUniform2dv)
#define glUniform3dv GLFuncPtrName(glUniform3dv)
#define glUniform4dv GLFuncPtrName(glUniform4dv)
#define glUniformMatrix2dv GLFuncPtrName(glUniformMatrix2dv)
#define glUniformMatrix3dv GLFuncPtrName(glUniformMatrix3dv)
#define glUniformMatrix4dv GLFuncPtrName(glUniformMatrix4dv)
#define glUniformMatrix2x3dv GLFuncPtrName(glUniformMatrix2x3dv)
#define glUniformMatrix2x4dv GLFuncPtrName(glUniformMatrix2x4dv)
#define glUniformMatrix3x2dv GLFuncPtrName(glUniformMatrix3x2dv)
#define glUniformMatrix3x4dv GLFuncPtrName(glUniformMatrix3x4dv)
#define glUniformMatrix4x2dv GLFuncPtrName(glUniformMatrix4x2dv)
#define glUniformMatrix4x3dv GLFuncPtrName(glUniformMatrix4x3dv)
#define glGetUniformdv GLFuncPtrName(glGetUniformdv)
#define glProgramUniform1dEXT GLFuncPtrName(glProgramUniform1dEXT)
#define glProgramUniform2dEXT GLFuncPtrName(glProgramUniform2dEXT)
#define glProgramUniform3dEXT GLFuncPtrName(glProgramUniform3dEXT)
#define glProgramUniform4dEXT GLFuncPtrName(glProgramUniform4dEXT)
#define glProgramUniform1dvEXT GLFuncPtrName(glProgramUniform1dvEXT)
#define glProgramUniform2dvEXT GLFuncPtrName(glProgramUniform2dvEXT)
#define glProgramUniform3dvEXT GLFuncPtrName(glProgramUniform3dvEXT)
#define glProgramUniform4dvEXT GLFuncPtrName(glProgramUniform4dvEXT)
#define glProgramUniformMatrix2dvEXT GLFuncPtrName(glProgramUniformMatrix2dvEXT)
#define glProgramUniformMatrix3dvEXT GLFuncPtrName(glProgramUniformMatrix3dvEXT)
#define glProgramUniformMatrix4dvEXT GLFuncPtrName(glProgramUniformMatrix4dvEXT)
#define glProgramUniformMatrix2x3dvEXT GLFuncPtrName(glProgramUniformMatrix2x3dvEXT)
#define glProgramUniformMatrix2x4dvEXT GLFuncPtrName(glProgramUniformMatrix2x4dvEXT)
#define glProgramUniformMatrix3x2dvEXT GLFuncPtrName(glProgramUniformMatrix3x2dvEXT)
#define glProgramUniformMatrix3x4dvEXT GLFuncPtrName(glProgramUniformMatrix3x4dvEXT)
#define glProgramUniformMatrix4x2dvEXT GLFuncPtrName(glProgramUniformMatrix4x2dvEXT)
#define glProgramUniformMatrix4x3dvEXT GLFuncPtrName(glProgramUniformMatrix4x3dvEXT)

#endif // GL_ARB_gpu_shader_fp64

//								--- GL_ARB_shader_subroutine ---

#ifndef GL_ARB_shader_subroutine
#define GL_ARB_shader_subroutine 1
#define __GENERATE_GL_ARB_shader_subroutine

extern bool GLEH_ARB_shader_subroutine;

#define GL_ACTIVE_SUBROUTINES                                        0x8de5
#define GL_ACTIVE_SUBROUTINE_UNIFORMS                                0x8de6
#define GL_ACTIVE_SUBROUTINE_UNIFORM_LOCATIONS                       0x8e47
#define GL_ACTIVE_SUBROUTINE_MAX_LENGTH                              0x8e48
#define GL_ACTIVE_SUBROUTINE_UNIFORM_MAX_LENGTH                      0x8e49
#define GL_MAX_SUBROUTINES                                           0x8de7
#define GL_MAX_SUBROUTINE_UNIFORM_LOCATIONS                          0x8de8
#define GL_NUM_COMPATIBLE_SUBROUTINES                                0x8e4a
#define GL_COMPATIBLE_SUBROUTINES                                    0x8e4b

extern int (GLApi *GLFuncPtrName(glGetSubroutineUniformLocation))(GLuint, GLenum, const GLchar*);
extern GLuint (GLApi *GLFuncPtrName(glGetSubroutineIndex))(GLuint, GLenum, const GLchar*);
extern void (GLApi *GLFuncPtrName(glGetActiveSubroutineUniformiv))(GLuint, GLenum, GLuint, GLenum, int*);
extern void (GLApi *GLFuncPtrName(glGetActiveSubroutineUniformName))(GLuint, GLenum, GLuint, GLsizei, GLsizei*, GLchar*);
extern void (GLApi *GLFuncPtrName(glGetActiveSubroutineName))(GLuint, GLenum, GLuint, GLsizei, GLsizei*, GLchar*);
extern void (GLApi *GLFuncPtrName(glUniformSubroutinesuiv))(GLenum, GLsizei, const GLuint*);
extern void (GLApi *GLFuncPtrName(glGetUniformSubroutineuiv))(GLenum, int, GLuint*);
extern void (GLApi *GLFuncPtrName(glGetProgramStageiv))(GLuint, GLenum, GLenum, int*);

#define glGetSubroutineUniformLocation GLFuncPtrName(glGetSubroutineUniformLocation)
#define glGetSubroutineIndex GLFuncPtrName(glGetSubroutineIndex)
#define glGetActiveSubroutineUniformiv GLFuncPtrName(glGetActiveSubroutineUniformiv)
#define glGetActiveSubroutineUniformName GLFuncPtrName(glGetActiveSubroutineUniformName)
#define glGetActiveSubroutineName GLFuncPtrName(glGetActiveSubroutineName)
#define glUniformSubroutinesuiv GLFuncPtrName(glUniformSubroutinesuiv)
#define glGetUniformSubroutineuiv GLFuncPtrName(glGetUniformSubroutineuiv)
#define glGetProgramStageiv GLFuncPtrName(glGetProgramStageiv)

#endif // GL_ARB_shader_subroutine

//								--- GL_ARB_tessellation_shader ---

#ifndef GL_ARB_tessellation_shader
#define GL_ARB_tessellation_shader 1
#define __GENERATE_GL_ARB_tessellation_shader

extern bool GLEH_ARB_tessellation_shader;

#define GL_PATCHES                                                   0x000e
#define GL_PATCH_VERTICES                                            0x8e72
#define GL_PATCH_DEFAULT_INNER_LEVEL                                 0x8e73
#define GL_PATCH_DEFAULT_OUTER_LEVEL                                 0x8e74
#define GL_TESS_CONTROL_OUTPUT_VERTICES                              0x8e75
#define GL_TESS_GEN_MODE                                             0x8e76
#define GL_TESS_GEN_SPACING                                          0x8e77
#define GL_TESS_GEN_VERTEX_ORDER                                     0x8e78
#define GL_TESS_GEN_POINT_MODE                                       0x8e79
#define GL_ISOLINES                                                  0x8e7a
#define GL_FRACTIONAL_ODD                                            0x8e7b
#define GL_FRACTIONAL_EVEN                                           0x8e7c
#define GL_MAX_PATCH_VERTICES                                        0x8e7d
#define GL_MAX_TESS_GEN_LEVEL                                        0x8e7e
#define GL_MAX_TESS_CONTROL_UNIFORM_COMPONENTS                       0x8e7f
#define GL_MAX_TESS_EVALUATION_UNIFORM_COMPONENTS                    0x8e80
#define GL_MAX_TESS_CONTROL_TEXTURE_IMAGE_UNITS                      0x8e81
#define GL_MAX_TESS_EVALUATION_TEXTURE_IMAGE_UNITS                   0x8e82
#define GL_MAX_TESS_CONTROL_OUTPUT_COMPONENTS                        0x8e83
#define GL_MAX_TESS_PATCH_COMPONENTS                                 0x8e84
#define GL_MAX_TESS_CONTROL_TOTAL_OUTPUT_COMPONENTS                  0x8e85
#define GL_MAX_TESS_EVALUATION_OUTPUT_COMPONENTS                     0x8e86
#define GL_MAX_TESS_CONTROL_UNIFORM_BLOCKS                           0x8e89
#define GL_MAX_TESS_EVALUATION_UNIFORM_BLOCKS                        0x8e8a
#define GL_MAX_TESS_CONTROL_INPUT_COMPONENTS                         0x886c
#define GL_MAX_TESS_EVALUATION_INPUT_COMPONENTS                      0x886d
#define GL_MAX_COMBINED_TESS_CONTROL_UNIFORM_COMPONENTS              0x8e1e
#define GL_MAX_COMBINED_TESS_EVALUATION_UNIFORM_COMPONENTS           0x8e1f
#define GL_UNIFORM_BLOCK_REFERENCED_BY_TESS_CONTROL_SHADER           0x84f0
#define GL_UNIFORM_BLOCK_REFERENCED_BY_TESS_EVALUATION_SHADER        0x84f1
#define GL_TESS_EVALUATION_SHADER                                    0x8e87
#define GL_TESS_CONTROL_SHADER                                       0x8e88

extern void (GLApi *GLFuncPtrName(glPatchParameteri))(GLenum, int);
extern void (GLApi *GLFuncPtrName(glPatchParameterfv))(GLenum, const float*);

#define glPatchParameteri GLFuncPtrName(glPatchParameteri)
#define glPatchParameterfv GLFuncPtrName(glPatchParameterfv)

#endif // GL_ARB_tessellation_shader

//								--- GL_ARB_texture_buffer_object_rgb32 ---

#ifndef GL_ARB_texture_buffer_object_rgb32
#define GL_ARB_texture_buffer_object_rgb32 1

extern bool GLEH_ARB_texture_buffer_object_rgb32;

// no new tokens
// no new procedures or functions

#endif // GL_ARB_texture_buffer_object_rgb32

//								--- GL_ARB_transform_feedback2 ---

#ifndef GL_ARB_transform_feedback2
#define GL_ARB_transform_feedback2 1
#define __GENERATE_GL_ARB_transform_feedback2

extern bool GLEH_ARB_transform_feedback2;

#define GL_TRANSFORM_FEEDBACK                                        0x8e22
#define GL_TRANSFORM_FEEDBACK_BUFFER_PAUSED                          0x8e23
#define GL_TRANSFORM_FEEDBACK_BUFFER_ACTIVE                          0x8e24
#define GL_TRANSFORM_FEEDBACK_BINDING                                0x8e25

extern void (GLApi *GLFuncPtrName(glBindTransformFeedback))(GLenum, GLuint);
extern void (GLApi *GLFuncPtrName(glDeleteTransformFeedbacks))(GLsizei, const GLuint*);
extern void (GLApi *GLFuncPtrName(glGenTransformFeedbacks))(GLsizei, GLuint*);
extern GLboolean (GLApi *GLFuncPtrName(glIsTransformFeedback))(GLuint);
extern void (GLApi *GLFuncPtrName(glPauseTransformFeedback))();
extern void (GLApi *GLFuncPtrName(glResumeTransformFeedback))();
extern void (GLApi *GLFuncPtrName(glDrawTransformFeedback))(GLenum, GLuint);

#define glBindTransformFeedback GLFuncPtrName(glBindTransformFeedback)
#define glDeleteTransformFeedbacks GLFuncPtrName(glDeleteTransformFeedbacks)
#define glGenTransformFeedbacks GLFuncPtrName(glGenTransformFeedbacks)
#define glIsTransformFeedback GLFuncPtrName(glIsTransformFeedback)
#define glPauseTransformFeedback GLFuncPtrName(glPauseTransformFeedback)
#define glResumeTransformFeedback GLFuncPtrName(glResumeTransformFeedback)
#define glDrawTransformFeedback GLFuncPtrName(glDrawTransformFeedback)

#endif // GL_ARB_transform_feedback2

//								--- GL_ARB_transform_feedback3 ---

#ifndef GL_ARB_transform_feedback3
#define GL_ARB_transform_feedback3 1
#define __GENERATE_GL_ARB_transform_feedback3

extern bool GLEH_ARB_transform_feedback3;

#define GL_MAX_TRANSFORM_FEEDBACK_BUFFERS                            0x8e70
#define GL_MAX_VERTEX_STREAMS                                        0x8e71

extern void (GLApi *GLFuncPtrName(glDrawTransformFeedbackStream))(GLenum, GLuint, GLuint);
extern void (GLApi *GLFuncPtrName(glBeginQueryIndexed))(GLenum, GLuint, GLuint);
extern void (GLApi *GLFuncPtrName(glEndQueryIndexed))(GLenum, GLuint);
extern void (GLApi *GLFuncPtrName(glGetQueryIndexediv))(GLenum, GLuint, GLenum, int*);

#define glDrawTransformFeedbackStream GLFuncPtrName(glDrawTransformFeedbackStream)
#define glBeginQueryIndexed GLFuncPtrName(glBeginQueryIndexed)
#define glEndQueryIndexed GLFuncPtrName(glEndQueryIndexed)
#define glGetQueryIndexediv GLFuncPtrName(glGetQueryIndexediv)

#endif // GL_ARB_transform_feedback3

//								--- GL_ARB_ES2_compatibility ---

#ifndef GL_ARB_ES2_compatibility
#define GL_ARB_ES2_compatibility 1
#define __GENERATE_GL_ARB_ES2_compatibility

extern bool GLEH_ARB_ES2_compatibility;

#define GL_SHADER_COMPILER                                           0x8dfa
#define GL_SHADER_BINARY_FORMATS                                     0x8df8
#define GL_NUM_SHADER_BINARY_FORMATS                                 0x8df9
#define GL_MAX_VERTEX_UNIFORM_VECTORS                                0x8dfb
#define GL_MAX_VARYING_VECTORS                                       0x8dfc
#define GL_MAX_FRAGMENT_UNIFORM_VECTORS                              0x8dfd
#define GL_IMPLEMENTATION_COLOR_READ_TYPE                            0x8b9a
#define GL_IMPLEMENTATION_COLOR_READ_FORMAT                          0x8b9b
#define GL_FIXED                                                     0x140c
#define GL_LOW_FLOAT                                                 0x8df0
#define GL_MEDIUM_FLOAT                                              0x8df1
#define GL_HIGH_FLOAT                                                0x8df2
#define GL_LOW_INT                                                   0x8df3
#define GL_MEDIUM_INT                                                0x8df4
#define GL_HIGH_INT                                                  0x8df5

extern void (GLApi *GLFuncPtrName(glReleaseShaderCompiler))();
extern void (GLApi *GLFuncPtrName(glShaderBinary))(GLsizei, const GLuint*, GLenum, const void*, GLsizei);
extern void (GLApi *GLFuncPtrName(glGetShaderPrecisionFormat))(GLenum, GLenum, int*, int*);
extern void (GLApi *GLFuncPtrName(glDepthRangef))(GLclampf, GLclampf);
extern void (GLApi *GLFuncPtrName(glClearDepthf))(GLclampf);

#define glReleaseShaderCompiler GLFuncPtrName(glReleaseShaderCompiler)
#define glShaderBinary GLFuncPtrName(glShaderBinary)
#define glGetShaderPrecisionFormat GLFuncPtrName(glGetShaderPrecisionFormat)
#define glDepthRangef GLFuncPtrName(glDepthRangef)
#define glClearDepthf GLFuncPtrName(glClearDepthf)

#endif // GL_ARB_ES2_compatibility

//								--- GL_ARB_get_program_binary ---

#ifndef GL_ARB_get_program_binary
#define GL_ARB_get_program_binary 1
#define __GENERATE_GL_ARB_get_program_binary

extern bool GLEH_ARB_get_program_binary;

#define GL_PROGRAM_BINARY_RETRIEVABLE_HINT                           0x8257
#define GL_PROGRAM_BINARY_LENGTH                                     0x8741
#define GL_NUM_PROGRAM_BINARY_FORMATS                                0x87fe
#define GL_PROGRAM_BINARY_FORMATS                                    0x87ff

extern void (GLApi *GLFuncPtrName(glGetProgramBinary))(GLuint, GLsizei, GLsizei*, GLenum*, void*);
extern void (GLApi *GLFuncPtrName(glProgramBinary))(GLuint, GLenum, const void*, GLsizei);
extern void (GLApi *GLFuncPtrName(glProgramParameteri))(GLuint, GLenum, int);

#define glGetProgramBinary GLFuncPtrName(glGetProgramBinary)
#define glProgramBinary GLFuncPtrName(glProgramBinary)
#define glProgramParameteri GLFuncPtrName(glProgramParameteri)

#endif // GL_ARB_get_program_binary

//								--- GL_ARB_separate_shader_objects ---

#ifndef GL_ARB_separate_shader_objects
#define GL_ARB_separate_shader_objects 1
#define __GENERATE_GL_ARB_separate_shader_objects

extern bool GLEH_ARB_separate_shader_objects;

#define GL_VERTEX_SHADER_BIT                                         0x00000001
#define GL_FRAGMENT_SHADER_BIT                                       0x00000002
#define GL_GEOMETRY_SHADER_BIT                                       0x00000004
#define GL_TESS_CONTROL_SHADER_BIT                                   0x00000008
#define GL_TESS_EVALUATION_SHADER_BIT                                0x00000010
#define GL_ALL_SHADER_BITS                                           0xffffffffu
#define GL_PROGRAM_SEPARABLE                                         0x8258
#define GL_ACTIVE_PROGRAM                                            0x8259
#define GL_PROGRAM_PIPELINE_BINDING                                  0x825a

extern void (GLApi *GLFuncPtrName(glUseProgramStages))(GLuint, GLbitfield, GLuint);
extern void (GLApi *GLFuncPtrName(glActiveShaderProgram))(GLuint, GLuint);
extern GLuint (GLApi *GLFuncPtrName(glCreateShaderProgramv))(GLenum, GLsizei, const GLchar**);
extern void (GLApi *GLFuncPtrName(glBindProgramPipeline))(GLuint);
extern void (GLApi *GLFuncPtrName(glDeleteProgramPipelines))(GLsizei, const GLuint*);
extern void (GLApi *GLFuncPtrName(glGenProgramPipelines))(GLsizei, GLuint*);
extern GLboolean (GLApi *GLFuncPtrName(glIsProgramPipeline))(GLuint);
extern void (GLApi *GLFuncPtrName(glProgramParameteri))(GLuint, GLenum, int);
extern void (GLApi *GLFuncPtrName(glGetProgramPipelineiv))(GLuint, GLenum, int*);
extern void (GLApi *GLFuncPtrName(glProgramUniform1i))(GLuint, int, int);
extern void (GLApi *GLFuncPtrName(glProgramUniform1f))(GLuint, int, float);
extern void (GLApi *GLFuncPtrName(glProgramUniform1d))(GLuint, int, double);
extern void (GLApi *GLFuncPtrName(glProgramUniform1iv))(GLuint, int, GLsizei, const int);
extern void (GLApi *GLFuncPtrName(glProgramUniform1fv))(GLuint, int, GLsizei, const float);
extern void (GLApi *GLFuncPtrName(glProgramUniform1dv))(GLuint, int, GLsizei, const double);
extern void (GLApi *GLFuncPtrName(glProgramUniform1ui))(GLuint, int, GLuint);
extern void (GLApi *GLFuncPtrName(glProgramUniform1uiv))(GLuint, int, GLsizei, const GLuint*);
extern void (GLApi *GLFuncPtrName(glProgramUniform2i))(GLuint, int, int, int);
extern void (GLApi *GLFuncPtrName(glProgramUniform2f))(GLuint, int, float, float);
extern void (GLApi *GLFuncPtrName(glProgramUniform2d))(GLuint, int, double, double);
extern void (GLApi *GLFuncPtrName(glProgramUniform2iv))(GLuint, int, GLsizei, const int, const int);
extern void (GLApi *GLFuncPtrName(glProgramUniform2fv))(GLuint, int, GLsizei, const float, const float);
extern void (GLApi *GLFuncPtrName(glProgramUniform2dv))(GLuint, int, GLsizei, const double, const double);
extern void (GLApi *GLFuncPtrName(glProgramUniform2ui))(GLuint, int, GLuint, GLuint);
extern void (GLApi *GLFuncPtrName(glProgramUniform2uiv))(GLuint, int, GLsizei, const GLuint*);
extern void (GLApi *GLFuncPtrName(glProgramUniform3i))(GLuint, int, int, int, int);
extern void (GLApi *GLFuncPtrName(glProgramUniform3f))(GLuint, int, float, float, float);
extern void (GLApi *GLFuncPtrName(glProgramUniform3d))(GLuint, int, double, double, double);
extern void (GLApi *GLFuncPtrName(glProgramUniform3iv))(GLuint, int, GLsizei, const int, const int, const int);
extern void (GLApi *GLFuncPtrName(glProgramUniform3fv))(GLuint, int, GLsizei, const float, const float, const float);
extern void (GLApi *GLFuncPtrName(glProgramUniform3dv))(GLuint, int, GLsizei, const double, const double, const double);
extern void (GLApi *GLFuncPtrName(glProgramUniform3ui))(GLuint, int, GLuint, GLuint, GLuint);
extern void (GLApi *GLFuncPtrName(glProgramUniform3uiv))(GLuint, int, GLsizei, const GLuint*);
extern void (GLApi *GLFuncPtrName(glProgramUniform4i))(GLuint, int, int, int, int, int);
extern void (GLApi *GLFuncPtrName(glProgramUniform4f))(GLuint, int, float, float, float, float);
extern void (GLApi *GLFuncPtrName(glProgramUniform4d))(GLuint, int, double, double, double, double);
extern void (GLApi *GLFuncPtrName(glProgramUniform4iv))(GLuint, int, GLsizei, const int, const int, const int, const int);
extern void (GLApi *GLFuncPtrName(glProgramUniform4fv))(GLuint, int, GLsizei, const float, const float, const float, const float);
extern void (GLApi *GLFuncPtrName(glProgramUniform4dv))(GLuint, int, GLsizei, const double, const double, const double, const double);
extern void (GLApi *GLFuncPtrName(glProgramUniform4ui))(GLuint, int, GLuint, GLuint, GLuint, GLuint);
extern void (GLApi *GLFuncPtrName(glProgramUniform4uiv))(GLuint, int, GLsizei, const GLuint*);
extern void (GLApi *GLFuncPtrName(glValidateProgramPipeline))(GLuint);
extern void (GLApi *GLFuncPtrName(glGetProgramPipelineInfoLog))(GLuint, GLsizei, GLsizei*, GLchar*);

#define glUseProgramStages GLFuncPtrName(glUseProgramStages)
#define glActiveShaderProgram GLFuncPtrName(glActiveShaderProgram)
#define glCreateShaderProgramv GLFuncPtrName(glCreateShaderProgramv)
#define glBindProgramPipeline GLFuncPtrName(glBindProgramPipeline)
#define glDeleteProgramPipelines GLFuncPtrName(glDeleteProgramPipelines)
#define glGenProgramPipelines GLFuncPtrName(glGenProgramPipelines)
#define glIsProgramPipeline GLFuncPtrName(glIsProgramPipeline)
#define glProgramParameteri GLFuncPtrName(glProgramParameteri)
#define glGetProgramPipelineiv GLFuncPtrName(glGetProgramPipelineiv)
#define glProgramUniform1i GLFuncPtrName(glProgramUniform1i)
#define glProgramUniform1f GLFuncPtrName(glProgramUniform1f)
#define glProgramUniform1d GLFuncPtrName(glProgramUniform1d)
#define glProgramUniform1iv GLFuncPtrName(glProgramUniform1iv)
#define glProgramUniform1fv GLFuncPtrName(glProgramUniform1fv)
#define glProgramUniform1dv GLFuncPtrName(glProgramUniform1dv)
#define glProgramUniform1ui GLFuncPtrName(glProgramUniform1ui)
#define glProgramUniform1uiv GLFuncPtrName(glProgramUniform1uiv)
#define glProgramUniform2i GLFuncPtrName(glProgramUniform2i)
#define glProgramUniform2f GLFuncPtrName(glProgramUniform2f)
#define glProgramUniform2d GLFuncPtrName(glProgramUniform2d)
#define glProgramUniform2iv GLFuncPtrName(glProgramUniform2iv)
#define glProgramUniform2fv GLFuncPtrName(glProgramUniform2fv)
#define glProgramUniform2dv GLFuncPtrName(glProgramUniform2dv)
#define glProgramUniform2ui GLFuncPtrName(glProgramUniform2ui)
#define glProgramUniform2uiv GLFuncPtrName(glProgramUniform2uiv)
#define glProgramUniform3i GLFuncPtrName(glProgramUniform3i)
#define glProgramUniform3f GLFuncPtrName(glProgramUniform3f)
#define glProgramUniform3d GLFuncPtrName(glProgramUniform3d)
#define glProgramUniform3iv GLFuncPtrName(glProgramUniform3iv)
#define glProgramUniform3fv GLFuncPtrName(glProgramUniform3fv)
#define glProgramUniform3dv GLFuncPtrName(glProgramUniform3dv)
#define glProgramUniform3ui GLFuncPtrName(glProgramUniform3ui)
#define glProgramUniform3uiv GLFuncPtrName(glProgramUniform3uiv)
#define glProgramUniform4i GLFuncPtrName(glProgramUniform4i)
#define glProgramUniform4f GLFuncPtrName(glProgramUniform4f)
#define glProgramUniform4d GLFuncPtrName(glProgramUniform4d)
#define glProgramUniform4iv GLFuncPtrName(glProgramUniform4iv)
#define glProgramUniform4fv GLFuncPtrName(glProgramUniform4fv)
#define glProgramUniform4dv GLFuncPtrName(glProgramUniform4dv)
#define glProgramUniform4ui GLFuncPtrName(glProgramUniform4ui)
#define glProgramUniform4uiv GLFuncPtrName(glProgramUniform4uiv)
#define glValidateProgramPipeline GLFuncPtrName(glValidateProgramPipeline)
#define glGetProgramPipelineInfoLog GLFuncPtrName(glGetProgramPipelineInfoLog)

#endif // GL_ARB_separate_shader_objects

//								--- GL_ARB_shader_precision ---

#ifndef GL_ARB_shader_precision
#define GL_ARB_shader_precision 1

extern bool GLEH_ARB_shader_precision;

// no new tokens
// no new procedures or functions

#endif // GL_ARB_shader_precision

//								--- GL_ARB_vertex_attrib_64bit ---

#ifndef GL_ARB_vertex_attrib_64bit
#define GL_ARB_vertex_attrib_64bit 1
#define __GENERATE_GL_ARB_vertex_attrib_64bit

extern bool GLEH_ARB_vertex_attrib_64bit;

#define GL_DOUBLE_VEC2                                               0x8ffc
#define GL_DOUBLE_VEC3                                               0x8ffd
#define GL_DOUBLE_VEC4                                               0x8ffe
#define GL_DOUBLE_MAT2                                               0x8f46
#define GL_DOUBLE_MAT3                                               0x8f47
#define GL_DOUBLE_MAT4                                               0x8f48
#define GL_DOUBLE_MAT2x3                                             0x8f49
#define GL_DOUBLE_MAT2x4                                             0x8f4a
#define GL_DOUBLE_MAT3x2                                             0x8f4b
#define GL_DOUBLE_MAT3x4                                             0x8f4c
#define GL_DOUBLE_MAT4x2                                             0x8f4d
#define GL_DOUBLE_MAT4x3                                             0x8f4e

extern void (GLApi *GLFuncPtrName(glVertexAttribL1d))(GLuint, double);
extern void (GLApi *GLFuncPtrName(glVertexAttribL2d))(GLuint, double, double);
extern void (GLApi *GLFuncPtrName(glVertexAttribL3d))(GLuint, double, double, double);
extern void (GLApi *GLFuncPtrName(glVertexAttribL4d))(GLuint, double, double, double, double);
extern void (GLApi *GLFuncPtrName(glVertexAttribL1dv))(GLuint, const double*);
extern void (GLApi *GLFuncPtrName(glVertexAttribL2dv))(GLuint, const double*);
extern void (GLApi *GLFuncPtrName(glVertexAttribL3dv))(GLuint, const double*);
extern void (GLApi *GLFuncPtrName(glVertexAttribL4dv))(GLuint, const double*);
extern void (GLApi *GLFuncPtrName(glVertexAttribLPointer))(GLuint, int, GLenum, GLsizei, const void*);
extern void (GLApi *GLFuncPtrName(glGetVertexAttribLdv))(GLuint, GLenum, double*);
extern void (GLApi *GLFuncPtrName(glVertexArrayVertexAttribLOffsetEXT))(GLuint, GLuint, GLuint, int, GLenum, GLsizei, GLintptr);

#define glVertexAttribL1d GLFuncPtrName(glVertexAttribL1d)
#define glVertexAttribL2d GLFuncPtrName(glVertexAttribL2d)
#define glVertexAttribL3d GLFuncPtrName(glVertexAttribL3d)
#define glVertexAttribL4d GLFuncPtrName(glVertexAttribL4d)
#define glVertexAttribL1dv GLFuncPtrName(glVertexAttribL1dv)
#define glVertexAttribL2dv GLFuncPtrName(glVertexAttribL2dv)
#define glVertexAttribL3dv GLFuncPtrName(glVertexAttribL3dv)
#define glVertexAttribL4dv GLFuncPtrName(glVertexAttribL4dv)
#define glVertexAttribLPointer GLFuncPtrName(glVertexAttribLPointer)
#define glGetVertexAttribLdv GLFuncPtrName(glGetVertexAttribLdv)
#define glVertexArrayVertexAttribLOffsetEXT GLFuncPtrName(glVertexArrayVertexAttribLOffsetEXT)

#endif // GL_ARB_vertex_attrib_64bit

//								--- GL_ARB_viewport_array ---

#ifndef GL_ARB_viewport_array
#define GL_ARB_viewport_array 1
#define __GENERATE_GL_ARB_viewport_array

extern bool GLEH_ARB_viewport_array;

#define GL_MAX_VIEWPORTS                                             0x825b
#define GL_VIEWPORT_SUBPIXEL_BITS                                    0x825c
#define GL_VIEWPORT_BOUNDS_RANGE                                     0x825d
#define GL_LAYER_PROVOKING_VERTEX                                    0x825e
#define GL_VIEWPORT_INDEX_PROVOKING_VERTEX                           0x825f
//#define GL_SCISSOR_BOX                                               0x0c10
//#define GL_VIEWPORT                                                  0x0ba2
//#define GL_DEPTH_RANGE                                               0x0b70
//#define GL_SCISSOR_TEST                                              0x0c11
//#define GL_FIRST_VERTEX_CONVENTION                                   0x8e4d
//#define GL_LAST_VERTEX_CONVENTION                                    0x8e4e
//#define GL_PROVOKING_VERTEX                                          0x8e4f
#define GL_UNDEFINED_VERTEX                                          0x8260

extern void (GLApi *GLFuncPtrName(glViewportArrayv))(GLuint, GLsizei, const float*);
extern void (GLApi *GLFuncPtrName(glViewportIndexedf))(GLuint, float, float, float, float);
extern void (GLApi *GLFuncPtrName(glViewportIndexedfv))(GLuint, const float*);
extern void (GLApi *GLFuncPtrName(glScissorArrayv))(GLuint, GLsizei, const int*);
extern void (GLApi *GLFuncPtrName(glScissorIndexed))(GLuint, int, int, GLsizei, GLsizei);
extern void (GLApi *GLFuncPtrName(glScissorIndexedv))(GLuint, const int*);
extern void (GLApi *GLFuncPtrName(glDepthRangeArrayv))(GLuint, GLsizei, const GLclampd*);
extern void (GLApi *GLFuncPtrName(glDepthRangeIndexed))(GLuint, GLclampd, GLclampd);
extern void (GLApi *GLFuncPtrName(glGetFloati_v))(GLenum, GLuint, float*);
extern void (GLApi *GLFuncPtrName(glGetDoublei_v))(GLenum, GLuint, double*);
extern void (GLApi *GLFuncPtrName(glGetIntegerIndexedivEXT))(GLenum, GLuint, int*);
extern void (GLApi *GLFuncPtrName(glEnableIndexedEXT))(GLenum, GLuint);
extern void (GLApi *GLFuncPtrName(glDisableIndexedEXT))(GLenum, GLuint);
extern GLboolean (GLApi *GLFuncPtrName(glIsEnabledIndexedEXT))(GLenum, GLuint);

#define glViewportArrayv GLFuncPtrName(glViewportArrayv)
#define glViewportIndexedf GLFuncPtrName(glViewportIndexedf)
#define glViewportIndexedfv GLFuncPtrName(glViewportIndexedfv)
#define glScissorArrayv GLFuncPtrName(glScissorArrayv)
#define glScissorIndexed GLFuncPtrName(glScissorIndexed)
#define glScissorIndexedv GLFuncPtrName(glScissorIndexedv)
#define glDepthRangeArrayv GLFuncPtrName(glDepthRangeArrayv)
#define glDepthRangeIndexed GLFuncPtrName(glDepthRangeIndexed)
#define glGetFloati_v GLFuncPtrName(glGetFloati_v)
#define glGetDoublei_v GLFuncPtrName(glGetDoublei_v)
#define glGetIntegerIndexedivEXT GLFuncPtrName(glGetIntegerIndexedivEXT)
#define glEnableIndexedEXT GLFuncPtrName(glEnableIndexedEXT)
#define glDisableIndexedEXT GLFuncPtrName(glDisableIndexedEXT)
#define glIsEnabledIndexedEXT GLFuncPtrName(glIsEnabledIndexedEXT)

#endif // GL_ARB_viewport_array

//								--- GL_ARB_cl_event ---

#ifndef GL_ARB_cl_event
#define GL_ARB_cl_event 1
#define __GENERATE_GL_ARB_cl_event

extern bool GLEH_ARB_cl_event;

#define GL_SYNC_CL_EVENT_ARB                                         0x8240
#define GL_SYNC_CL_EVENT_COMPLETE_ARB                                0x8241

typedef void *GLcl_context;
typedef void *GLcl_event;

extern GLsync (GLApi *GLFuncPtrName(glCreateSyncFromCLeventARB))(GLcl_context, GLcl_event, GLbitfield);

#define glCreateSyncFromCLeventARB GLFuncPtrName(glCreateSyncFromCLeventARB)

#endif // GL_ARB_cl_event

//								--- GL_ARB_debug_output ---

#ifndef GL_ARB_debug_output
#define GL_ARB_debug_output 1
#define __GENERATE_GL_ARB_debug_output

extern bool GLEH_ARB_debug_output;

#define GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB                              0x8242
#define GL_MAX_DEBUG_MESSAGE_LENGTH_ARB                              0x9143
#define GL_MAX_DEBUG_LOGGED_MESSAGES_ARB                             0x9144
#define GL_DEBUG_LOGGED_MESSAGES_ARB                                 0x9145
#define GL_DEBUG_NEXT_LOGGED_MESSAGE_LENGTH_ARB                      0x8243
#define GL_DEBUG_CALLBACK_FUNCTION_ARB                               0x8244
#define GL_DEBUG_CALLBACK_USER_PARAM_ARB                             0x8245
#define GL_DEBUG_SOURCE_API_ARB                                      0x8246
#define GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB                            0x8247
#define GL_DEBUG_SOURCE_SHADER_COMPILER_ARB                          0x8248
#define GL_DEBUG_SOURCE_THIRD_PARTY_ARB                              0x8249
#define GL_DEBUG_SOURCE_APPLICATION_ARB                              0x824a
#define GL_DEBUG_SOURCE_OTHER_ARB                                    0x824b
#define GL_DEBUG_TYPE_ERROR_ARB                                      0x824c
#define GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB                        0x824d
#define GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB                         0x824e
#define GL_DEBUG_TYPE_PORTABILITY_ARB                                0x824f
#define GL_DEBUG_TYPE_PERFORMANCE_ARB                                0x8250
#define GL_DEBUG_TYPE_OTHER_ARB                                      0x8251
#define GL_DEBUG_SEVERITY_HIGH_ARB                                   0x9146
#define GL_DEBUG_SEVERITY_MEDIUM_ARB                                 0x9147
#define GL_DEBUG_SEVERITY_LOW_ARB                                    0x9148

typedef void (*GLDEBUGPROCARB)();

extern void (GLApi *GLFuncPtrName(glDebugMessageControlARB))(GLenum, GLenum, GLenum, GLsizei, const GLuint*, GLboolean);
extern void (GLApi *GLFuncPtrName(glDebugMessageInsertARB))(GLenum, GLenum, GLuint, GLenum, GLsizei, const GLchar*);
extern void (GLApi *GLFuncPtrName(glDebugMessageCallbackARB))(GLDEBUGPROCARB, void*);
extern GLuint (GLApi *GLFuncPtrName(glGetDebugMessageLogARB))(GLuint, GLsizei, GLenum*, GLenum*, GLuint*, GLenum*, GLsizei*, GLchar*);
extern void (GLApi *GLFuncPtrName(glGetPointerv))(GLenum, void**);

#define glDebugMessageControlARB GLFuncPtrName(glDebugMessageControlARB)
#define glDebugMessageInsertARB GLFuncPtrName(glDebugMessageInsertARB)
#define glDebugMessageCallbackARB GLFuncPtrName(glDebugMessageCallbackARB)
#define glGetDebugMessageLogARB GLFuncPtrName(glGetDebugMessageLogARB)
#define glGetPointerv GLFuncPtrName(glGetPointerv)

#endif // GL_ARB_debug_output

//								--- GL_ARB_robustness ---

#ifndef GL_ARB_robustness
#define GL_ARB_robustness 1
#define __GENERATE_GL_ARB_robustness

extern bool GLEH_ARB_robustness;

//#define GL_NO_ERROR                                                  0x0000
#define GL_GUILTY_CONTEXT_RESET_ARB                                  0x8253
#define GL_INNOCENT_CONTEXT_RESET_ARB                                0x8254
#define GL_UNKNOWN_CONTEXT_RESET_ARB                                 0x8255
#define GL_RESET_NOTIFICATION_STRATEGY_ARB                           0x8256
#define GL_LOSE_CONTEXT_ON_RESET_ARB                                 0x8252
#define GL_NO_RESET_NOTIFICATION_ARB                                 0x8261
#define GL_CONTEXT_FLAG_ROBUST_ACCESS_BIT_ARB                        0x00000004

extern GLenum (GLApi *GLFuncPtrName(glGetGraphicsResetStatusARB))();
extern void (GLApi *GLFuncPtrName(glGetnMapdvARB))(GLenum, GLenum, GLsizei, double*);
extern void (GLApi *GLFuncPtrName(glGetnMapfvARB))(GLenum, GLenum, GLsizei, float*);
extern void (GLApi *GLFuncPtrName(glGetnMapivARB))(GLenum, GLenum, GLsizei, int*);
extern void (GLApi *GLFuncPtrName(glGetnPixelMapfvARB))(GLenum, GLsizei, float*);
extern void (GLApi *GLFuncPtrName(glGetnPixelMapuivARB))(GLenum, GLsizei, GLuint*);
extern void (GLApi *GLFuncPtrName(glGetnPixelMapusvARB))(GLenum, GLsizei, GLushort*);
extern void (GLApi *GLFuncPtrName(glGetnPolygonStippleARB))(GLsizei, GLubyte*);
extern void (GLApi *GLFuncPtrName(glGetnTexImageARB))(GLenum, int, GLenum, GLenum, GLsizei, void*);
extern void (GLApi *GLFuncPtrName(glReadnPixelsARB))(int, int, GLsizei, GLsizei, GLenum, GLenum, GLsizei, void*);
extern void (GLApi *GLFuncPtrName(glGetnColorTableARB))(GLenum, GLenum, GLenum, GLsizei, void*);
extern void (GLApi *GLFuncPtrName(glGetnConvolutionFilterARB))(GLenum, GLenum, GLenum, GLsizei, void*);
extern void (GLApi *GLFuncPtrName(glGetnSeparableFilterARB))(GLenum, GLenum, GLenum, GLsizei, void*, GLsizei, void*, void*);
extern void (GLApi *GLFuncPtrName(glGetnHistogramARB))(GLenum, GLboolean, GLenum, GLenum, GLsizei, void*);
extern void (GLApi *GLFuncPtrName(glGetnMinmaxARB))(GLenum, GLboolean, GLenum, GLenum, GLsizei, void*);
extern void (GLApi *GLFuncPtrName(glGetnCompressedTexImageARB))(GLenum, int, GLsizei, void*);
extern void (GLApi *GLFuncPtrName(glGetnUniformfvARB))(GLuint, int, GLsizei, float*);
extern void (GLApi *GLFuncPtrName(glGetnUniformivARB))(GLuint, int, GLsizei, int*);
extern void (GLApi *GLFuncPtrName(glGetnUniformuivARB))(GLuint, int, GLsizei, GLuint*);
extern void (GLApi *GLFuncPtrName(glGetnUniformdvARB))(GLuint, int, GLsizei, double*);

#define glGetGraphicsResetStatusARB GLFuncPtrName(glGetGraphicsResetStatusARB)
#define glGetnMapdvARB GLFuncPtrName(glGetnMapdvARB)
#define glGetnMapfvARB GLFuncPtrName(glGetnMapfvARB)
#define glGetnMapivARB GLFuncPtrName(glGetnMapivARB)
#define glGetnPixelMapfvARB GLFuncPtrName(glGetnPixelMapfvARB)
#define glGetnPixelMapuivARB GLFuncPtrName(glGetnPixelMapuivARB)
#define glGetnPixelMapusvARB GLFuncPtrName(glGetnPixelMapusvARB)
#define glGetnPolygonStippleARB GLFuncPtrName(glGetnPolygonStippleARB)
#define glGetnTexImageARB GLFuncPtrName(glGetnTexImageARB)
#define glReadnPixelsARB GLFuncPtrName(glReadnPixelsARB)
#define glGetnColorTableARB GLFuncPtrName(glGetnColorTableARB)
#define glGetnConvolutionFilterARB GLFuncPtrName(glGetnConvolutionFilterARB)
#define glGetnSeparableFilterARB GLFuncPtrName(glGetnSeparableFilterARB)
#define glGetnHistogramARB GLFuncPtrName(glGetnHistogramARB)
#define glGetnMinmaxARB GLFuncPtrName(glGetnMinmaxARB)
#define glGetnCompressedTexImageARB GLFuncPtrName(glGetnCompressedTexImageARB)
#define glGetnUniformfvARB GLFuncPtrName(glGetnUniformfvARB)
#define glGetnUniformivARB GLFuncPtrName(glGetnUniformivARB)
#define glGetnUniformuivARB GLFuncPtrName(glGetnUniformuivARB)
#define glGetnUniformdvARB GLFuncPtrName(glGetnUniformdvARB)

#endif // GL_ARB_robustness

//								--- GL_ARB_shader_stencil_export ---

#ifndef GL_ARB_shader_stencil_export
#define GL_ARB_shader_stencil_export 1

extern bool GLEH_ARB_shader_stencil_export;

// no new tokens
// no new procedures or functions

#endif // GL_ARB_shader_stencil_export

#endif // __OPENGL30_TOOL_INCLUDED
