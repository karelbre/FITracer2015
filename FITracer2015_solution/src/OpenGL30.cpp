/*
								+--------------------------------+
								|                                |
								|     ***   OpenGL 3.0   ***     |
								|                                |
								|  Copyright © -tHE SWINe- 2009  |
								|                                |
								|          OpenGL30.cpp          |
								|                                |
								+--------------------------------+
*/

/**
 *	@file gl3/OpenGL30.cpp
 *	@author -tHE SWINe-
 *	@brief OpenGL 3.0 extension entry pointers
 */

#include "OpenGL30.h"

#if defined(_WIN32) || defined (_WIN64)
#include <windows.h>
#endif // _WIN32, _WIN64
/*#define GL_GLEXT_LEGACY
#include <GL/gl.h>*/
#if defined(_WIN32) || defined (_WIN64)
#include "wglext.h"
#else // _WIN32, _WIN64
//#include "glxext.h"
// my glxext.h doesn't cope nicely with some unices; gotta get another version
#include <GL/glx.h>
#define glXGetProcAddress(x) (*glXGetProcAddressARB)((const GLubyte*)x)
#define wglGetProcAddress(x) (*glXGetProcAddress)((GLubyte*)(x))
#endif // _WIN32, _WIN64

#ifdef NULL_FUNCTION_POINTERS
#define INIT_FUNC_POINTERS = 0
#else
#define INIT_FUNC_POINTERS 
#endif

/*
								+----------------------------------+
								|                                  |
								|     ***  STL utilities  ***      |
								|                                  |
								|   Copyright © -tHE SWINe- 2008   |
								|                                  |
								|            StlUtils.h            |
								|                                  |
								+----------------------------------+
*/

/*
 *
 *	2008-12-22
 *
 *	added typename keyword in Reserve_NMore() so it now compiles with g++
 *
 *	2009-01-13
 *
 *	added Format() function and StlUtils.cpp to keep it's body (it's not template)
 *
 *	2009-03-24
 *
 *	added Assign and Append, accepting std::string as second argument,
 *	should allow assigning and appending strings with different character
 *	types as well (no conversion or error handling though).
 *
 *	(there was just const char* before; it was possible to use these versions
 *	to append std::string to std::string using c_str() function, but it
 *	would expectably yield worse performance)
 *
 *	2009-05-04
 *
 *	stl_ut::Swap() was removed in favor of std::swap
 *
 *	added __STL_UTILS_ENABLE_EXCEPTIONS macro, controlling between exception-
 *	based and null-pointer based errors on operator new()
 *
 *	written documentation comments
 *
 *	2009-10-11
 *
 *	added Resize_To_N(), Resize_Add_1More() and Resize_Add_NMore() functions
 *	to help avoiding unhandled std::bad_alloc in container::resize()
 *
 *	added AssignWCStr(), AppendWCStr() and FormatW() to support wide character
 *	strings, can disable them by defining __STL_UTILS_NO_WIDE_STRINGS macro
 *
 *	added __STL_UT_CATCH_BAD_ALLOC macro, containing catch(std::bad_alloc&)
 *	(in case __STL_UTILS_ENABLE_EXCEPTIONS is not defined)
 *
 *	2009-10-20
 *
 *	fixed some warnings when compiling under VC 2005, implemented "Security
 *	Enhancements in the CRT " for VC 2008. compare against MyProjects_2009-10-19_
 *
 *	2010-02-12
 *
 *	added __stl_ut_try, __stl_ut_catch and __stl_ut_catch_bad_alloc aliases
 *
 *	2010-08-09
 *
 *	fixed error in stl_ut::Resize_Add_NMore(), which didn't resize container
 *	in case it had sufficient capacity (regardless of size)
 *
 *	@date 2011-07-18
 *
 *	Added the stl_ut::Split() function for splitting a string to a list
 *	of substrings by a separator.
 *
 */

#ifndef __STL_UTILS_INCLUDED
#define __STL_UTILS_INCLUDED

/*
 *	__STL_UTILS_NO_WIDE_STRINGS
 *		- if defined, AssignWCStr(), AppendWCStr() and FormatW() won't be
 *		  compiled (this may be needed in environments where wchar_t.h is not
 *		  available and/or wcslen() or vsnwprintf() not defined)
 *	@note This is forced on non-windows platforms.
 */
//#define __STL_UTILS_NO_WIDE_STRINGS

#if !defined(_WIN32) && !defined(_WIN64) && !defined(__STL_UTILS_NO_WIDE_STRINGS)
#define __STL_UTILS_NO_WIDE_STRINGS // unix has no vsnwsprintf 
#endif // !_WIN32 && !_WIN64 && !__STL_UTILS_NO_WIDE_STRINGS

/*
 *	__STL_UTILS_ENABLE_EXCEPTIONS
 *		- if defined, it is expected that std::container::reserve() throws
 *		  std::bad_alloc once it runs out of memory
 *		- the other model would be not increasing storage capacity,
 *		  detected by comparing std::container::capacity() with requested
 *		  number. this happens when operator new() returns 0 when there's
 *		  not enough memory (patched versions of STL for MSVC 6.0 and older)
 */
#define __STL_UTILS_ENABLE_EXCEPTIONS

#ifdef __STL_UTILS_ENABLE_EXCEPTIONS
#define __STL_UT_TRY try
#define __STL_UT_CATCH(_Ty) catch(_Ty)
#else // __STL_UTILS_ENABLE_EXCEPTIONS
#define __STL_UT_TRY if(true)
#define __STL_UT_CATCH(_Ty) if(false)
#endif // __STL_UTILS_ENABLE_EXCEPTIONS

#define __STL_UT_CATCH_BAD_ALLOC __STL_UT_CATCH(std::bad_alloc&)

#define __stl_ut_try __STL_UT_TRY
#define __stl_ut_catch __STL_UT_CATCH
#define __stl_ut_catch_bad_alloc __STL_UT_CATCH_BAD_ALLOC
// allow small letters here

#include <string>
#include <vector>

namespace stl_ut {

/*
 *	template <class CContainer>
 *	inline bool stl_ut::Reserve_N(CContainer &r_vec, size_t n) throw()
 *		- reserves space for n elements in container r_vec
 *		- (note this allocates space just for n elements, not for n more!)
 *		- returns true on success, false in case there's not enough memory
 */
template <class CContainer>
inline bool Reserve_N(CContainer &r_vec, size_t n) throw()
{
	__STL_UT_TRY {
		r_vec.reserve(n);
		return r_vec.capacity() >= n;
		// reserve and check
	} __STL_UT_CATCH_BAD_ALLOC {
		// not enough memory
		return false;
	}
}

/*
 *	template <class CContainer>
 *	inline bool stl_ut::Resize_To_N(CContainer &r_vec, size_t n) throw()
 *		- reserves space for n elements in container r_vec and also changes
 *		  it's size to n
 *		- (note this allocates space just for n elements, not for n more!)
 *		- returns true on success, false in case there's not enough memory
 */
template <class CContainer>
inline bool Resize_To_N(CContainer &r_vec, size_t n) throw()
{
	__STL_UT_TRY {
		r_vec.resize(n);
		return true;
		// just resize
	} __STL_UT_CATCH_BAD_ALLOC {
		// not enough memory
		return false;
	}
}

/*
 *	template <class CContainer, class _Ty>
 *	inline bool stl_ut::Resize_To_N(CContainer &r_vec, size_t n, const _Ty &r_t_initializer) throw()
 *		- reserves space for n elements in container r_vec and also changes
 *		  it's size to n, using r_t_initializer as value for new elements
 *		- (note this allocates space just for n elements, not for n more!)
 *		- returns true on success, false in case there's not enough memory
 */
template <class CContainer, class _Ty>
inline bool Resize_To_N(CContainer &r_vec, size_t n, const _Ty &r_t_initializer) throw()
{
	__STL_UT_TRY {
		r_vec.resize(n, r_t_initializer);
		return true;
		// just resize
	} __STL_UT_CATCH_BAD_ALLOC {
		// not enough memory
		return false;
	}
}

/*
 *	template <class CContainer>
 *	inline bool stl_ut::Reserve_1More(CContainer &r_vec) throw()
 *		- reserves space for at least one more element in container r_vec
 *		  (but in practice, it tries to double capacity everytime
 *		  reallocation is needed to avoid too frequent reallocations)
 *		- returns true on success, false in case there's not enough memory
 */
template <class CContainer>
inline bool Reserve_1More(CContainer &r_vec) throw()
{
	__STL_UT_TRY {
		if(r_vec.capacity() == r_vec.size()) {
			r_vec.reserve((r_vec.size() <= r_vec.max_size() / 2)?
				(r_vec.size() * 2) | 1 : r_vec.max_size());
			// watch out for requested capacity overflow
			return r_vec.capacity() > r_vec.size();
		}
		return true;
	} __STL_UT_CATCH_BAD_ALLOC {
		// not enough memory
		return r_vec.size() < r_vec.max_size() && Reserve_N(r_vec, r_vec.size() + 1);
		// try allocating exactly one more (in case there's some memory left,
		// but not enough for twice as long vector)
	}
}

/*
 *	template <class CContainer>
 *	inline bool stl_ut::Resize_Add_1More(CContainer &r_vec) throw()
 *		- reserves space for at least one more element in container r_vec
 *		  and increases it's size (in practice, it tries to double
 *		  capacity everytime reallocation is needed to avoid too frequent
 *		  reallocations; resize is always by 1)
 *		- returns true on success, false in case there's not enough memory
 */
template <class CContainer>
inline bool Resize_Add_1More(CContainer &r_vec) throw()
{
	__STL_UT_TRY {
		if(r_vec.capacity() == r_vec.size()) {
			r_vec.reserve((r_vec.size() <= r_vec.max_size() / 2)?
				(r_vec.size() * 2) | 1 : r_vec.max_size());
			// watch out for requested capacity overflow
		}
		r_vec.resize(r_vec.size() + 1);
		return true;
	} __STL_UT_CATCH_BAD_ALLOC {
		// not enough memory
		return r_vec.size() < r_vec.max_size() && Resize_To_N(r_vec, r_vec.size() + 1);
		// try allocating exactly one more (in case there's some memory left,
		// but not enough for twice as long vector)
	}
}

/*
 *	template <class CContainer, class _Ty>
 *	inline bool stl_ut::Resize_Add_1More(CContainer &r_vec, const _Ty &r_t_initializer) throw()
 *		- reserves space for at least one more element in container r_vec
 *		  and increases it's size (in practice, it tries to double capacity
 *		  everytime reallocation is needed to avoid too frequent reallocations;
 *		  resize is always by 1)
 *		- r_t_initializer is used as the value of the new element
 *		- returns true on success, false in case there's not enough memory
 */
template <class CContainer, class _Ty>
inline bool Resize_Add_1More(CContainer &r_vec, const _Ty &r_t_initializer) throw()
{
	__STL_UT_TRY {
		/*if(r_vec.capacity() == r_vec.size()) {
			r_vec.reserve((r_vec.size() <= r_vec.max_size() / 2)?
				(r_vec.size() * 2) | 1 : r_vec.max_size());
			// watch out for requested capacity overflow
		}
		r_vec.resize(r_vec.size() + 1, r_t_initializer);*/
		r_vec.push_back(r_t_initializer); // this already contains code above (sgi version of stl)
		return true;
	} __STL_UT_CATCH_BAD_ALLOC {
		// not enough memory
		return r_vec.size() < r_vec.max_size() &&
			Resize_To_N(r_vec, r_vec.size() + 1, r_t_initializer);
		// try allocating exactly one more (in case there's some memory left,
		// but not enough for twice as long vector)
	}
}

/*
 *	template <class CContainer>
 *	inline bool stl_ut::Reserve_NMore(CContainer &r_vec, size_t n) throw()
 *		- reserves space for at least n more elements in container r_vec
 *		  (but in practice, it tries to double capacity everytime
 *		  reallocation is needed to avoid too frequent reallocations)
 *		- returns true on success, false in case there's not enough memory
 */
template <class CContainer>
inline bool Reserve_NMore(CContainer &r_vec, size_t n) throw()
{
	const typename CContainer::size_type n_max = r_vec.max_size();
	// get maximal size of container

	if(r_vec.size() > n_max - n)
		return false;
	// see if n items can be inserted

	typename CContainer::size_type n_min_new_capa = r_vec.size() + n;
	if(r_vec.capacity() < n_min_new_capa) {
#if 0
		typename CContainer::size_type n_new_capa = r_vec.capacity();
		while(n_new_capa < n_min_new_capa) {
			if(n_new_capa >= n_max / 2) {
				n_new_capa = n_max;
				break;
			}
			// watch out for overflow

			n_new_capa *= 2;
			n_new_capa |= 1;
		}
		// calculate new capacity, scale by factor of two
#else
		typename CContainer::size_type n_new_capa = (r_vec.capacity() * 2) | 1;

		if(n_new_capa < n_min_new_capa)
			n_new_capa = n_min_new_capa;
		else if(n_new_capa > n_max)
			n_new_capa = n_max;
		// scale by factor of two, do it more simple (sufficient in most cases)
#endif

		__STL_UT_TRY {
			r_vec.reserve(n_new_capa);
			return r_vec.capacity() >= n_min_new_capa;
			// reserve and check
		} __STL_UT_CATCH_BAD_ALLOC {
			// not enough memory
			return Reserve_N(r_vec, n_min_new_capa);
		}
	}
	return true;
}

/*
 *	template <class CContainer>
 *	inline bool stl_ut::Resize_Add_NMore(CContainer &r_vec, size_t n) throw()
 *		- reserves space for at least n more elements in container r_vec
 *		  and increases size by n (in practice, it tries to double
 *		  capacity everytime reallocation is needed to avoid too frequent
 *		  reallocations)
 *		- returns true on success, false in case there's not enough memory
 */
template <class CContainer>
inline bool Resize_Add_NMore(CContainer &r_vec, size_t n) throw()
{
	const typename CContainer::size_type n_max = r_vec.max_size();
	// get maximal size of container

	if(r_vec.size() > n_max - n)
		return false;
	// see if n items can be inserted

	typename CContainer::size_type n_min_new_capa = r_vec.size() + n;
	if(r_vec.capacity() < n_min_new_capa) {
		typename CContainer::size_type n_new_capa = (r_vec.capacity() * 2) | 1;

		if(n_new_capa < n_min_new_capa)
			n_new_capa = n_min_new_capa;
		else if(n_new_capa > n_max)
			n_new_capa = n_max;
		// scale by factor of two, do it more simple (sufficient in most cases)

		__STL_UT_TRY {
			r_vec.reserve(n_new_capa);
			r_vec.resize(n_min_new_capa);
			return true;
			// reserve and check
		} __STL_UT_CATCH_BAD_ALLOC {
			// not enough memory
			//return Resize_To_N(r_vec, n_min_new_capa); // just fall trough
		}
	}
	return Resize_To_N(r_vec, n_min_new_capa); // just very simple function, safe to call here
}

/*
 *	template <class CContainer, class _Ty>
 *	inline bool stl_ut::Resize_Add_NMore(CContainer &r_vec, size_t n,
 *		const _Ty &r_t_initializer) throw()
 *		- reserves space for at least n more elements in container r_vec
 *		  and increases size by n (in practice, it tries to double
 *		  capacity everytime reallocation is needed to avoid too frequent
 *		  reallocations)
 *		- r_t_initializer is used as the value of the new elements
 *		- returns true on success, false in case there's not enough memory
 */
template <class CContainer, class _Ty>
inline bool Resize_Add_NMore(CContainer &r_vec, size_t n, const _Ty &r_t_initializer) throw()
{
	const typename CContainer::size_type n_max = r_vec.max_size();
	// get maximal size of container

	if(r_vec.size() > n_max - n)
		return false;
	// see if n items can be inserted

	typename CContainer::size_type n_min_new_capa = r_vec.size() + n;
	if(r_vec.capacity() < n_min_new_capa) {
		typename CContainer::size_type n_new_capa = (r_vec.capacity() * 2) | 1;

		if(n_new_capa < n_min_new_capa)
			n_new_capa = n_min_new_capa;
		else if(n_new_capa > n_max)
			n_new_capa = n_max;
		// scale by factor of two, do it more simple (sufficient in most cases)

		__STL_UT_TRY {
			r_vec.reserve(n_new_capa);
			r_vec.resize(n_min_new_capa, r_t_initializer);
			return true;
			// reserve and check
		} __STL_UT_CATCH_BAD_ALLOC {
			// not enough memory
			//return Resize_To_N(r_vec, n_min_new_capa, r_t_initializer); // just fall trough
		}
	}
	return Resize_To_N(r_vec, n_min_new_capa, r_t_initializer); // just very simple function, safe to call here
}

/*
 *	template <class _String, class _InString>
 *	bool stl_ut::Assign(_String &r_s_dest, const _InString &r_s_src) throw()
 *		- assigns std::string r_s_src to std::string r_s_dest,
 *		  while watching out for (re)allocation errors
 *		- returns true on success, false on failure
 */
template <class _String, class _InString>
bool Assign(_String &r_s_dest, const _InString &r_s_src) throw()
{
	if(!Reserve_N(r_s_dest, r_s_src.length()))
		return false;
	// make sure there's space in the string

	r_s_dest.assign(r_s_src);
	// assign

	return true;
}

/*
 *	template <class _String, class _TChar>
 *	bool stl_ut::AssignCStr(_String &r_s_dest, _TChar *p_s_src) throw()
 *		- assigns "c" string p_s_src to std::string r_s_dest,
 *		  while watching out for (re)allocation errors
 *		- returns true on success, false on failure
 */
template <class _String, class _TChar>
bool AssignCStr(_String &r_s_dest, _TChar *p_s_src) throw()
{
	if(!Reserve_N(r_s_dest, strlen(p_s_src)))
		return false;
	// make sure there's space in the string

	r_s_dest.assign(p_s_src);
	// append

	return true;
}

/*
 *	template <class _String, class _InString>
 *	bool stl_ut::Append(_String &r_s_dest, const _InString &r_s_src) throw()
 *		- appends std::string r_s_src to std::string r_s_dest,
 *		  while watching out for (re)allocation errors
 *		- returns true on success, false on failure
 */
template <class _String, class _InString>
bool Append(_String &r_s_dest, const _InString &r_s_src) throw()
{
	if(!Reserve_NMore(r_s_dest, r_s_src.length()))
		return false;
	// make sure there's space in the string

	r_s_dest.append(r_s_src); // might be better optimized
	//r_s_dest.insert(r_s_dest.end(), r_s_src.begin(), r_s_src.end()); // equivalent to the above line
	// append

	return true;
}

/*
 *	template <class _String, class _TChar>
 *	bool stl_ut::AppendCStr(_String &r_s_dest, _TChar *p_s_src) throw()
 *		- appends "c" string p_s_src to std::string r_s_dest,
 *		  while watching out for (re)allocation errors
 *		- returns true on success, false on failure
 */
template <class _String, class _TChar>
bool AppendCStr(_String &r_s_dest, _TChar *p_s_src) throw()
{
	if(!Reserve_NMore(r_s_dest, strlen(p_s_src)))
		return false;
	// make sure there's space in the string

	r_s_dest.append(p_s_src);
	// append

	return true;
}

#include <string>

/*
 *	bool stl_ut::Format(std::string &r_s_result, const char *p_s_fmt, ...)
 *		- equivalent of standard "c" library sprintf function,
 *		  working with std::string output
 *		- returns true on success, false on failure (not enough memory)
 *		- note this is safe against output buffer overrun, but is
 *		  still susceptible to ill-formated format string (p_s_fmt)
 *		  and bad arguments (not enough of them / not corresponding
 *		  to tags in the format string). but as the first one being run-time
 *		  error and the latter one compile-time error, this brings
 *		  enought improvement.
 */
bool Format(std::string &r_s_result, const char *p_s_fmt, ...);

/**
 *	@brief reads line form a file
 *
 *	@param[out] r_s_line is output string, containing one line read from a file
 *	@param[in] p_fr is pointer to a file
 *
 *	@return Returns true on success, false on failure (not enough memory / input error).
 *
 *	@note In case file is at it's end, output lines are empty, but the function still succeeds.
 *	@note Output lines may contain carriage-return character(s), for example if the file
 *		is opened for binary reading. Line-feed character marks end of line and is never
 *		included.
 */
bool ReadLine(std::string &r_s_line, FILE *p_fr);

/**
 *	@brief splits a string by a separator
 *
 *	@param[out] r_s_dest is destination vector for substrings
 *	@param[in] r_s_string is the input string
 *	@param[in] p_s_separator is the separator
 *	@param[in] n_thresh is minimal output string threshold
 *		(only strings longer than threshold are stored in r_s_dest)
 *
 *	@return Returns true on success, false on failure (not enough memory).
 */
bool Split(std::vector<std::string> &r_s_dest, const std::string &r_s_string,
	const char *p_s_separator, int n_thresh = -1);

#ifndef __STL_UTILS_NO_WIDE_STRINGS

#include <wchar.h>

/*
 *	template <class _String, class _TChar>
 *	bool stl_ut::AssignWCStr(_String &r_s_dest, _TChar *p_s_src) throw()
 *		- assigns "c" string p_s_src to std::string r_s_dest,
 *		  while watching out for (re)allocation errors
 *		- returns true on success, false on failure
 */
template <class _String, class _TChar>
bool AssignWCStr(_String &r_s_dest, _TChar *p_s_src) throw()
{
	if(!Reserve_N(r_s_dest, wcslen(p_s_src)))
		return false;
	// make sure there's space in the string

	r_s_dest.assign(p_s_src);
	// append

	return true;
}

/*
 *	template <class _String, class _TChar>
 *	bool stl_ut::AppendWCStr(_String &r_s_dest, _TChar *p_s_src) throw()
 *		- appends "c" string p_s_src to std::string r_s_dest,
 *		  while watching out for (re)allocation errors
 *		- returns true on success, false on failure
 */
template <class _String, class _TChar>
bool AppendWCStr(_String &r_s_dest, _TChar *p_s_src) throw()
{
	if(!Reserve_NMore(r_s_dest, wcslen(p_s_src)))
		return false;
	// make sure there's space in the string

	r_s_dest.append(p_s_src);
	// append

	return true;
}

/*
 *	bool stl_ut::FormatW(std::basic_string<wchar_t> &r_s_result, const wchar_t *p_s_fmt, ...)
 *		- unicode equivalent of standard "c" library sprintf function,
 *		  working with std::string output
 *		- returns true on success, false on failure (not enough memory)
 *		- note this is safe against output buffer overrun, but is
 *		  still susceptible to ill-formated format string (p_s_fmt)
 *		  and bad arguments (not enough of them / not corresponding
 *		  to tags in the format string). but as the first one being run-time
 *		  error and the latter one compile-time error, this brings
 *		  enought improvement.
 */
bool FormatW(std::basic_string<wchar_t> &r_s_result, const wchar_t *p_s_fmt, ...);

#endif // __STL_UTILS_NO_WIDE_STRINGS

} // ~stl_ut

#endif // __STL_UTILS_INCLUDED


/*
 *								=== CGL3ExtensionHandler ===
 */

bool CGL3ExtensionHandler::b_Support_OpenGL(int n_min_major, int n_min_minor)
{
	int n_major = n_OpenGL_Version_Major(),
		n_minor = n_OpenGL_Version_Minor();
	if(n_major > n_min_major)
		return true;
	if(n_major == n_min_major && n_minor >= n_min_minor)
		return true;
	return false;
}

int CGL3ExtensionHandler::n_OpenGL_Version()
{
	int n_major = n_OpenGL_Version_Major(),
		n_minor = n_OpenGL_Version_Minor();
	// new simpler code using OpenGL 3.0 query

	/*const char *p_s_version;
	p_s_version = (const char*)glGetString(GL_VERSION);
	if(!p_s_version) {
#if defined(_WIN32) || defined (_WIN64)
		MessageBoxA(0, "OpenGL extension string is 0!", "CGL3ExtensionHandler", MB_OK);
#endif // _WIN32 || _WIN64
		return -1;
	}

#if defined(_MSC_VER) && !defined(__MWERKS__) && _MSC_VER >= 1400
	if(sscanf_s(p_s_version, "%d.%d", &n_major, &n_minor) != 2) {
#else // _MSC_VER && !__MWERKS__ && _MSC_VER >= 1400
	if(sscanf(p_s_version, "%d.%d", &n_major, &n_minor) != 2) {
#endif // _MSC_VER && !__MWERKS__ && _MSC_VER >= 1400
#if defined(_WIN32) || defined (_WIN64)
		MessageBoxA(0, "OpenGL version string malformed!", "CGL3ExtensionHandler", MB_OK);
#endif // _WIN32 || _WIN64
		//_ASSERTE(0); // error - OpenGL version string malformed!
		return -1;
	}
	//_ASSERTE(n_minor >= 0 && n_minor < 10);*/
	// old code, using the GL_VERSION string (still works)

	return n_major * 10 + n_minor;
}

int CGL3ExtensionHandler::n_OpenGL_Version_Major()
{
	int n_major;
	glGetIntegerv(GL_MAJOR_VERSION, &n_major);
	return n_major;
}

int CGL3ExtensionHandler::n_OpenGL_Version_Minor()
{
	int n_minor;
	glGetIntegerv(GL_MINOR_VERSION, &n_minor);
	return n_minor;
}

bool CGL3ExtensionHandler::b_SupportedExtension(const char *p_s_extension_name)
{
	if(!glGetStringi && n_GetGL30FuncPointers())
		return false;
	// requires OpenGL 3.0 functions

	int n_extension_num;
	glGetIntegerv(GL_NUM_EXTENSIONS, &n_extension_num);
	for(int i = 0; i < n_extension_num; ++ i) {
		const char *p_s_ext_name = (const char*)glGetStringi(GL_EXTENSIONS, i);
		if(!p_s_ext_name)
			continue; // ?
		if(!strcmp(p_s_ext_name, p_s_extension_name))
			return true; // extension was found
	}
	// new OpenGL 3 extensions scheme to avoid buffer overruns in old software

	return false;
}

bool CGL3ExtensionHandler::Get_ExtensionString(std::string &r_s_extension_list)
{
	if(!glGetStringi && n_GetGL30FuncPointers())
		return false;
	// requires OpenGL 3.0 functions

	r_s_extension_list.erase();

	int n_extension_num;
	glGetIntegerv(GL_NUM_EXTENSIONS, &n_extension_num);
	for(int i = 0; i < n_extension_num; ++ i) {
		const char *p_s_ext_name = (const char*)glGetStringi(GL_EXTENSIONS, i);
		if(!p_s_ext_name)
			continue; // ?
		if((i && !stl_ut::AppendCStr(r_s_extension_list, " ")) ||
		   !stl_ut::AppendCStr(r_s_extension_list, p_s_ext_name))
			return false;
	}
	// new OpenGL 3 extensions scheme to avoid buffer overruns in old software

	return true;
}

#if defined(_WIN32) || defined (_WIN64)

bool CGL3ExtensionHandler::b_SupportedWGLExtension(const char *p_s_extension_name)
{
	const char *p_s_extensions, *p_s_search_str;

	PFNWGLGETEXTENSIONSSTRINGARBPROC wglGetExtensionsStringARB;
	if(!(wglGetExtensionsStringARB = (PFNWGLGETEXTENSIONSSTRINGARBPROC)wglGetProcAddress("wglGetExtensionsStringARB")))
		return false;

	p_s_extensions = (const char*)wglGetExtensionsStringARB(wglGetCurrentDC());
	p_s_search_str = p_s_extensions;

	do {
		if((p_s_search_str = strstr(p_s_search_str, p_s_extension_name))) {
			if(p_s_search_str > p_s_extensions && !isspace(*(p_s_search_str - 1)))
				continue;
			if(strlen(p_s_search_str + strlen(p_s_extension_name)) &&
			   !isspace(*(p_s_search_str + strlen(p_s_extension_name))))
				continue;

			return true;
		}
	} while(p_s_search_str && strlen(++ p_s_search_str));

	return false;
}

#endif // _WIN32 || _WIN64

int CGL3ExtensionHandler::n_GetAllOpenGLCoreFuncPointers()
{
	return n_GetGL12FuncPointers() +
		n_GetGL13FuncPointers() +
		n_GetGL14FuncPointers() +
		n_GetGL15FuncPointers() +
		n_GetGL20FuncPointers() +
		n_GetGL21FuncPointers() +
		n_GetGL30FuncPointers() +
		n_GetGL31FuncPointers() +
		n_GetGL32FuncPointers() +
		n_GetGL33FuncPointers();
}

int CGL3ExtensionHandler::n_GetOpenGLCoreFuncPointers(int n_major, int n_minor)
{
	int n_failed_function_num = 0;
	if(n_major > 1 || (n_major == 1 && n_minor >= 2)) n_failed_function_num += n_GetGL12FuncPointers();
	if(n_major > 1 || (n_major == 1 && n_minor >= 3)) n_failed_function_num += n_GetGL13FuncPointers();
	if(n_major > 1 || (n_major == 1 && n_minor >= 4)) n_failed_function_num += n_GetGL14FuncPointers();
	if(n_major > 1 || (n_major == 1 && n_minor >= 5)) n_failed_function_num += n_GetGL15FuncPointers();
	if(n_major > 2 || (n_major == 2 && n_minor >= 0)) n_failed_function_num += n_GetGL20FuncPointers();
	if(n_major > 2 || (n_major == 2 && n_minor >= 1)) n_failed_function_num += n_GetGL21FuncPointers();
	if(n_major > 3 || (n_major == 3 && n_minor >= 0)) n_failed_function_num += n_GetGL30FuncPointers();
	if(n_major > 3 || (n_major == 3 && n_minor >= 1)) n_failed_function_num += n_GetGL31FuncPointers();
	if(n_major > 3 || (n_major == 3 && n_minor >= 2)) n_failed_function_num += n_GetGL32FuncPointers();
	if(n_major > 3 || (n_major == 3 && n_minor >= 3)) n_failed_function_num += n_GetGL33FuncPointers();

	return n_failed_function_num;
}

int CGL3ExtensionHandler::n_GetAllSupportedExtensionsFuncPointers()
{
	return n_GetGL12FuncPointers() +
		n_GetGL13FuncPointers() +
		n_GetGL14FuncPointers() +
		n_GetGL15FuncPointers() +
		n_GetGL20FuncPointers() +
		n_GetGL21FuncPointers() +
		n_GetGL30FuncPointers() +
		n_GetGL31FuncPointers() +
		n_GetGL32FuncPointers() +
		n_GetGL33FuncPointers() +
		n_GetEXTDrawBuffers2FuncPointers() +
		n_GetARBDrawBuffersBlendFuncPointers() +
		n_GetARBSampleShadingFuncPointers() +
		n_GetEXTTimerQueryFuncPointers() +
		n_GetEXTGPUProgramParametersFuncPointers() +
		n_GetEXTBindableUniformFuncPointers() +

		n_GetEXTABGRFuncPointers() +
		n_GetEXTTextureFilterAnisotropicFuncPointers() +
		n_GetEXTTextureCompressionLatcFuncPointers() +
		n_GetEXTTextureCompressionS3tcFuncPointers() +
		n_GetS3S3tcFuncPointers() +
		n_GetARBTextureQueryLodFuncPointers() +
		n_GetEXTTextureSwizzleFuncPointers() +
		n_GetARBTextureGatherFuncPointers() +
		n_GetARBTextureCubeMapArrayFuncPointers() +
		
		//n_GetARBInstancedArraysFuncPointers() + // a part OpenGL 3.3 core
		n_GetARBShaderTextureLodFuncPointers() +
		n_GetARBTextureBufferObjectFuncPointers() +

		//n_GetARBBlendFuncExtendedFuncPointers() + // a part OpenGL 3.3 core
		//n_GetARBExplicitAttribLocationFuncPointers() + // a part OpenGL 3.3 core
		//n_GetARBOcclusionQuery2FuncPointers() + // a part OpenGL 3.3 core
		//n_GetARBSamplerObjectsFuncPointers() + // a part OpenGL 3.3 core
		//n_GetARBShaderBitEncodingFuncPointers() + // a part OpenGL 3.3 core
		//n_GetARBTextureRgb10A2uiFuncPointers() + // a part OpenGL 3.3 core
		//n_GetARBTextureSwizzleFuncPointers() + // a part OpenGL 3.3 core
		//n_GetARBTimerQueryFuncPointers() + // a part OpenGL 3.3 core
		//n_GetARBVertexType2101010RevFuncPointers() + // a part OpenGL 3.3 core

		//n_GetARBSampleShadingFuncPointers() +
		//n_GetARBTextureCubeMapArrayFuncPointers() +
		//n_GetARBTextureGatherFuncPointers() +
		//n_GetARBTextureQueryLodFuncPointers() + // already in the list above
		n_GetARBShadingLanguageIncludeFuncPointers() +
		n_GetARBTextureCompressionBptcFuncPointers() +
		n_GetARBDrawIndirectFuncPointers() +
		n_GetARBGpuShader5FuncPointers() +
		n_GetARBGpuShaderFp64FuncPointers() +
		n_GetARBShaderSubroutineFuncPointers() +
		n_GetARBTessellationShaderFuncPointers() +
		n_GetARBTextureBufferObjectRgb32FuncPointers() +
		n_GetARBTransformFeedback2FuncPointers() +
		n_GetARBTransformFeedback3FuncPointers() +
		n_GetARBES2CompatibilityFuncPointers() +
		n_GetARBGetProgramBinaryFuncPointers() +
		n_GetARBSeparateShaderObjectsFuncPointers() +
		n_GetARBShaderPrecisionFuncPointers() +
		n_GetARBVertexAttrib64bitFuncPointers() +
		n_GetARBViewportArrayFuncPointers() +
		n_GetARBClEventFuncPointers() +
		n_GetARBDebugOutputFuncPointers() +
		n_GetARBRobustnessFuncPointers() +
		n_GetARBShaderStencilExportFuncPointers();
}

/*
 *								--- OpenGL 1.2 ---
 */

#ifdef __GENERATE_GL_12__

void (GLApi *glBlendColor)(GLclampf, GLclampf, GLclampf, GLclampf) INIT_FUNC_POINTERS;
void (GLApi *glBlendEquation)(GLenum) INIT_FUNC_POINTERS;
void (GLApi *glDrawRangeElements)(GLenum, GLuint, GLuint, GLsizei, GLenum, const GLvoid*) INIT_FUNC_POINTERS;

// it is unclear whether this is deprecated (it seems to be)
/*void (GLApi *glColorTable)(GLenum, GLenum, GLsizei, GLenum, GLenum, const GLvoid*) INIT_FUNC_POINTERS;
void (GLApi *glColorTableParameterfv)(GLenum, GLenum, const GLfloat*) INIT_FUNC_POINTERS;
void (GLApi *glColorTableParameteriv)(GLenum, GLenum, const GLint*) INIT_FUNC_POINTERS;
void (GLApi *glCopyColorTable)(GLenum, GLenum, GLint, GLint, GLsizei) INIT_FUNC_POINTERS;
void (GLApi *glGetColorTable)(GLenum, GLenum, GLenum, GLvoid*) INIT_FUNC_POINTERS;
void (GLApi *glGetColorTableParameterfv)(GLenum, GLenum, GLfloat*) INIT_FUNC_POINTERS;
void (GLApi *glGetColorTableParameteriv)(GLenum, GLenum, GLint*) INIT_FUNC_POINTERS;
void (GLApi *glColorSubTable)(GLenum, GLsizei, GLsizei, GLenum, GLenum, const GLvoid*) INIT_FUNC_POINTERS;
void (GLApi *glCopyColorSubTable)(GLenum, GLsizei, GLint, GLint, GLsizei) INIT_FUNC_POINTERS;
void (GLApi *glConvolutionFilter1D)(GLenum, GLenum, GLsizei, GLenum, GLenum, const GLvoid*) INIT_FUNC_POINTERS;
void (GLApi *glConvolutionFilter2D)(GLenum, GLenum, GLsizei, GLsizei, GLenum, GLenum, const GLvoid*) INIT_FUNC_POINTERS;
void (GLApi *glConvolutionParameterf)(GLenum, GLenum, GLfloat) INIT_FUNC_POINTERS;
void (GLApi *glConvolutionParameterfv)(GLenum, GLenum, const GLfloat*) INIT_FUNC_POINTERS;
void (GLApi *glConvolutionParameteri)(GLenum, GLenum, GLint) INIT_FUNC_POINTERS;
void (GLApi *glConvolutionParameteriv)(GLenum, GLenum, const GLint*) INIT_FUNC_POINTERS;
void (GLApi *glCopyConvolutionFilter1D)(GLenum, GLenum, GLint, GLint, GLsizei) INIT_FUNC_POINTERS;
void (GLApi *glCopyConvolutionFilter2D)(GLenum, GLenum, GLint, GLint, GLsizei, GLsizei) INIT_FUNC_POINTERS;
void (GLApi *glGetConvolutionFilter)(GLenum, GLenum, GLenum, GLvoid*) INIT_FUNC_POINTERS;
void (GLApi *glGetConvolutionParameterfv)(GLenum, GLenum, GLfloat*) INIT_FUNC_POINTERS;
void (GLApi *glGetConvolutionParameteriv)(GLenum, GLenum, GLint*) INIT_FUNC_POINTERS;
void (GLApi *glGetSeparableFilter)(GLenum, GLenum, GLenum, GLvoid *, GLvoid *, GLvoid*) INIT_FUNC_POINTERS;
void (GLApi *glSeparableFilter2D)(GLenum, GLenum, GLsizei, GLsizei, GLenum, GLenum, const GLvoid *, const GLvoid*) INIT_FUNC_POINTERS;
void (GLApi *glGetHistogram)(GLenum, GLboolean, GLenum, GLenum, GLvoid*) INIT_FUNC_POINTERS;
void (GLApi *glGetHistogramParameterfv)(GLenum, GLenum, GLfloat*) INIT_FUNC_POINTERS;
void (GLApi *glGetHistogramParameteriv)(GLenum, GLenum, GLint*) INIT_FUNC_POINTERS;
void (GLApi *glGetMinmax)(GLenum, GLboolean, GLenum, GLenum, GLvoid*) INIT_FUNC_POINTERS;
void (GLApi *glGetMinmaxParameterfv)(GLenum, GLenum, GLfloat*) INIT_FUNC_POINTERS;
void (GLApi *glGetMinmaxParameteriv)(GLenum, GLenum, GLint*) INIT_FUNC_POINTERS;
void (GLApi *glHistogram)(GLenum, GLsizei, GLenum, GLboolean) INIT_FUNC_POINTERS;
void (GLApi *glMinmax)(GLenum, GLenum, GLboolean) INIT_FUNC_POINTERS;
void (GLApi *glResetHistogram)(GLenum) INIT_FUNC_POINTERS;
void (GLApi *glResetMinmax)(GLenum) INIT_FUNC_POINTERS;*/

void (GLApi *glTexImage3D)(GLenum, GLint, GLint, GLsizei, GLsizei, GLsizei, GLint, GLenum, GLenum, const GLvoid*) INIT_FUNC_POINTERS;
void (GLApi *glTexSubImage3D)(GLenum, GLint, GLint, GLint, GLint, GLsizei, GLsizei, GLsizei, GLenum, GLenum, const GLvoid*) INIT_FUNC_POINTERS;
void (GLApi *glCopyTexSubImage3D)(GLenum, GLint, GLint, GLint, GLint, GLint, GLint, GLsizei, GLsizei) INIT_FUNC_POINTERS;

#endif // __GENERATE_GL_12__

bool GLEH_OPENGL_1_2 = false;

int CGL3ExtensionHandler::n_GetGL12FuncPointers()
{
	int n_failed_function_num = 0;

#ifdef __GENERATE_GL_12__

	if(!(glBlendColor = (void(GLApi*)(GLclampf, GLclampf, GLclampf, GLclampf))wglGetProcAddress("glBlendColor"))) ++ n_failed_function_num;
	if(!(glBlendEquation = (void(GLApi*)(GLenum))wglGetProcAddress("glBlendEquation"))) ++ n_failed_function_num;
	if(!(glDrawRangeElements = (void(GLApi*)(GLenum, GLuint, GLuint, GLsizei, GLenum, const GLvoid*))wglGetProcAddress("glDrawRangeElements"))) ++ n_failed_function_num;

	// it is unclear whether this is deprecated (it seems to be)
	/*if(!(glColorTable = (void(GLApi*)(GLenum, GLenum, GLsizei, GLenum, GLenum, const GLvoid*))wglGetProcAddress("glColorTable"))) ++ n_failed_function_num;
	if(!(glColorTableParameterfv = (void(GLApi*)(GLenum, GLenum, const GLfloat*))wglGetProcAddress("glColorTableParameterfv"))) ++ n_failed_function_num;
	if(!(glColorTableParameteriv = (void(GLApi*)(GLenum, GLenum, const GLint*))wglGetProcAddress("glColorTableParameteriv"))) ++ n_failed_function_num;
	if(!(glCopyColorTable = (void(GLApi*)(GLenum, GLenum, GLint, GLint, GLsizei))wglGetProcAddress("glCopyColorTable"))) ++ n_failed_function_num;
	if(!(glGetColorTable = (void(GLApi*)(GLenum, GLenum, GLenum, GLvoid*))wglGetProcAddress("glGetColorTable"))) ++ n_failed_function_num;
	if(!(glGetColorTableParameterfv = (void(GLApi*)(GLenum, GLenum, GLfloat*))wglGetProcAddress("glGetColorTableParameterfv"))) ++ n_failed_function_num;
	if(!(glGetColorTableParameteriv = (void(GLApi*)(GLenum, GLenum, GLint*))wglGetProcAddress("glGetColorTableParameteriv"))) ++ n_failed_function_num;
	if(!(glColorSubTable = (void(GLApi*)(GLenum, GLsizei, GLsizei, GLenum, GLenum, const GLvoid*))wglGetProcAddress("glColorSubTable"))) ++ n_failed_function_num;
	if(!(glCopyColorSubTable = (void(GLApi*)(GLenum, GLsizei, GLint, GLint, GLsizei))wglGetProcAddress("glCopyColorSubTable"))) ++ n_failed_function_num;
	if(!(glConvolutionFilter1D = (void(GLApi*)(GLenum, GLenum, GLsizei, GLenum, GLenum, const GLvoid*))wglGetProcAddress("glConvolutionFilter1D"))) ++ n_failed_function_num;
	if(!(glConvolutionFilter2D = (void(GLApi*)(GLenum, GLenum, GLsizei, GLsizei, GLenum, GLenum, const GLvoid*))wglGetProcAddress("glConvolutionFilter2D"))) ++ n_failed_function_num;
	if(!(glConvolutionParameterf = (void(GLApi*)(GLenum, GLenum, GLfloat))wglGetProcAddress("glConvolutionParameterf"))) ++ n_failed_function_num;
	if(!(glConvolutionParameterfv = (void(GLApi*)(GLenum, GLenum, const GLfloat*))wglGetProcAddress("glConvolutionParameterfv"))) ++ n_failed_function_num;
	if(!(glConvolutionParameteri = (void(GLApi*)(GLenum, GLenum, GLint))wglGetProcAddress("glConvolutionParameteri"))) ++ n_failed_function_num;
	if(!(glConvolutionParameteriv = (void(GLApi*)(GLenum, GLenum, const GLint*))wglGetProcAddress("glConvolutionParameteriv"))) ++ n_failed_function_num;
	if(!(glCopyConvolutionFilter1D = (void(GLApi*)(GLenum, GLenum, GLint, GLint, GLsizei))wglGetProcAddress("glCopyConvolutionFilter1D"))) ++ n_failed_function_num;
	if(!(glCopyConvolutionFilter2D = (void(GLApi*)(GLenum, GLenum, GLint, GLint, GLsizei, GLsizei))wglGetProcAddress("glCopyConvolutionFilter2D"))) ++ n_failed_function_num;
	if(!(glGetConvolutionFilter = (void(GLApi*)(GLenum, GLenum, GLenum, GLvoid*))wglGetProcAddress("glGetConvolutionFilter"))) ++ n_failed_function_num;
	if(!(glGetConvolutionParameterfv = (void(GLApi*)(GLenum, GLenum, GLfloat*))wglGetProcAddress("glGetConvolutionParameterfv"))) ++ n_failed_function_num;
	if(!(glGetConvolutionParameteriv = (void(GLApi*)(GLenum, GLenum, GLint*))wglGetProcAddress("glGetConvolutionParameteriv"))) ++ n_failed_function_num;
	if(!(glGetSeparableFilter = (void(GLApi*)(GLenum, GLenum, GLenum, GLvoid *, GLvoid *, GLvoid*))wglGetProcAddress("glGetSeparableFilter"))) ++ n_failed_function_num;
	if(!(glSeparableFilter2D = (void(GLApi*)(GLenum, GLenum, GLsizei, GLsizei, GLenum, GLenum, const GLvoid *, const GLvoid*))wglGetProcAddress("glSeparableFilter2D"))) ++ n_failed_function_num;
	if(!(glGetHistogram = (void(GLApi*)(GLenum, GLboolean, GLenum, GLenum, GLvoid*))wglGetProcAddress("glGetHistogram"))) ++ n_failed_function_num;
	if(!(glGetHistogramParameterfv = (void(GLApi*)(GLenum, GLenum, GLfloat*))wglGetProcAddress("glGetHistogramParameterfv"))) ++ n_failed_function_num;
	if(!(glGetHistogramParameteriv = (void(GLApi*)(GLenum, GLenum, GLint*))wglGetProcAddress("glGetHistogramParameteriv"))) ++ n_failed_function_num;
	if(!(glGetMinmax = (void(GLApi*)(GLenum, GLboolean, GLenum, GLenum, GLvoid*))wglGetProcAddress("glGetMinmax"))) ++ n_failed_function_num;
	if(!(glGetMinmaxParameterfv = (void(GLApi*)(GLenum, GLenum, GLfloat*))wglGetProcAddress("glGetMinmaxParameterfv"))) ++ n_failed_function_num;
	if(!(glGetMinmaxParameteriv = (void(GLApi*)(GLenum, GLenum, GLint*))wglGetProcAddress("glGetMinmaxParameteriv"))) ++ n_failed_function_num;
	if(!(glHistogram = (void(GLApi*)(GLenum, GLsizei, GLenum, GLboolean))wglGetProcAddress("glHistogram"))) ++ n_failed_function_num;
	if(!(glMinmax = (void(GLApi*)(GLenum, GLenum, GLboolean))wglGetProcAddress("glMinmax"))) ++ n_failed_function_num;
	if(!(glResetHistogram = (void(GLApi*)(GLenum))wglGetProcAddress("glResetHistogram"))) ++ n_failed_function_num;
	if(!(glResetMinmax = (void(GLApi*)(GLenum))wglGetProcAddress("glResetMinmax"))) ++ n_failed_function_num;*/

	if(!(glTexImage3D = (void(GLApi*)(GLenum, GLint, GLint, GLsizei, GLsizei, GLsizei, GLint, GLenum, GLenum, const GLvoid*))wglGetProcAddress("glTexImage3D"))) ++ n_failed_function_num;
	if(!(glTexSubImage3D = (void(GLApi*)(GLenum, GLint, GLint, GLint, GLint, GLsizei, GLsizei, GLsizei, GLenum, GLenum, const GLvoid*))wglGetProcAddress("glTexSubImage3D"))) ++ n_failed_function_num;
	if(!(glCopyTexSubImage3D = (void(GLApi*)(GLenum, GLint, GLint, GLint, GLint, GLint, GLint, GLsizei, GLsizei))wglGetProcAddress("glCopyTexSubImage3D"))) ++ n_failed_function_num;

#endif // __GENERATE_GL_12__

	GLEH_OPENGL_1_2 = !n_failed_function_num && b_Support_OpenGL(1, 2);

	return n_failed_function_num;
}

/*
 *								--- OpenGL 1.3 ---
 */

#ifdef __GENERATE_GL_13__

// functions below are deprecated
/*void (GLApi *glLoadTransposeMatrixf)(const GLfloat*) INIT_FUNC_POINTERS;
void (GLApi *glLoadTransposeMatrixd)(const GLdouble*) INIT_FUNC_POINTERS;
void (GLApi *glMultTransposeMatrixf)(const GLfloat*) INIT_FUNC_POINTERS;
void (GLApi *glMultTransposeMatrixd)(const GLdouble*) INIT_FUNC_POINTERS;*/

void (GLApi *glActiveTexture)(GLenum) INIT_FUNC_POINTERS;
void (GLApi *glSampleCoverage)(GLclampf, GLboolean) INIT_FUNC_POINTERS;
void (GLApi *glCompressedTexImage3D)(GLenum, GLint, GLenum, GLsizei, GLsizei, GLsizei, GLint, GLsizei, const GLvoid*) INIT_FUNC_POINTERS;
void (GLApi *glCompressedTexImage2D)(GLenum, GLint, GLenum, GLsizei, GLsizei, GLint, GLsizei, const GLvoid*) INIT_FUNC_POINTERS;
void (GLApi *glCompressedTexImage1D)(GLenum, GLint, GLenum, GLsizei, GLint, GLsizei, const GLvoid*) INIT_FUNC_POINTERS;
void (GLApi *glCompressedTexSubImage3D)(GLenum, GLint, GLint, GLint, GLint, GLsizei, GLsizei, GLsizei, GLenum, GLsizei, const GLvoid*) INIT_FUNC_POINTERS;
void (GLApi *glCompressedTexSubImage2D)(GLenum, GLint, GLint, GLint, GLsizei, GLsizei, GLenum, GLsizei, const GLvoid*) INIT_FUNC_POINTERS;
void (GLApi *glCompressedTexSubImage1D)(GLenum, GLint, GLint, GLsizei, GLenum, GLsizei, const GLvoid*) INIT_FUNC_POINTERS;
void (GLApi *glGetCompressedTexImage)(GLenum, GLint, GLvoid*) INIT_FUNC_POINTERS;

// functions below are deprecated
/*void (GLApi *glClientActiveTexture)(GLenum) INIT_FUNC_POINTERS;
void (GLApi *glMultiTexCoord1d)(GLenum, GLdouble) INIT_FUNC_POINTERS;
void (GLApi *glMultiTexCoord1dv)(GLenum, const GLdouble*) INIT_FUNC_POINTERS;
void (GLApi *glMultiTexCoord1f)(GLenum, GLfloat) INIT_FUNC_POINTERS;
void (GLApi *glMultiTexCoord1fv)(GLenum, const GLfloat*) INIT_FUNC_POINTERS;
void (GLApi *glMultiTexCoord1i)(GLenum, GLint) INIT_FUNC_POINTERS;
void (GLApi *glMultiTexCoord1iv)(GLenum, const GLint *) INIT_FUNC_POINTERS;
void (GLApi *glMultiTexCoord1s)(GLenum, GLshort) INIT_FUNC_POINTERS;
void (GLApi *glMultiTexCoord1sv)(GLenum, const GLshort*) INIT_FUNC_POINTERS;
void (GLApi *glMultiTexCoord2d)(GLenum, GLdouble, GLdouble) INIT_FUNC_POINTERS;
void (GLApi *glMultiTexCoord2dv)(GLenum, const GLdouble*) INIT_FUNC_POINTERS;
void (GLApi *glMultiTexCoord2f)(GLenum, GLfloat, GLfloat) INIT_FUNC_POINTERS;
void (GLApi *glMultiTexCoord2fv)(GLenum, const GLfloat*) INIT_FUNC_POINTERS;
void (GLApi *glMultiTexCoord2i)(GLenum, GLint, GLint) INIT_FUNC_POINTERS;
void (GLApi *glMultiTexCoord2iv)(GLenum, const GLint*) INIT_FUNC_POINTERS;
void (GLApi *glMultiTexCoord2s)(GLenum, GLshort, GLshort) INIT_FUNC_POINTERS;
void (GLApi *glMultiTexCoord2sv)(GLenum, const GLshort*) INIT_FUNC_POINTERS;
void (GLApi *glMultiTexCoord3d)(GLenum, GLdouble, GLdouble, GLdouble) INIT_FUNC_POINTERS;
void (GLApi *glMultiTexCoord3dv)(GLenum, const GLdouble*) INIT_FUNC_POINTERS;
void (GLApi *glMultiTexCoord3f)(GLenum, GLfloat, GLfloat, GLfloat) INIT_FUNC_POINTERS;
void (GLApi *glMultiTexCoord3fv)(GLenum, const GLfloat*) INIT_FUNC_POINTERS;
void (GLApi *glMultiTexCoord3i)(GLenum, GLint, GLint, GLint) INIT_FUNC_POINTERS;
void (GLApi *glMultiTexCoord3iv)(GLenum, const GLint*) INIT_FUNC_POINTERS;
void (GLApi *glMultiTexCoord3s)(GLenum, GLshort, GLshort, GLshort) INIT_FUNC_POINTERS;
void (GLApi *glMultiTexCoord3sv)(GLenum, const GLshort*) INIT_FUNC_POINTERS;
void (GLApi *glMultiTexCoord4d)(GLenum, GLdouble, GLdouble, GLdouble, GLdouble) INIT_FUNC_POINTERS;
void (GLApi *glMultiTexCoord4dv)(GLenum, const GLdouble*) INIT_FUNC_POINTERS;
void (GLApi *glMultiTexCoord4f)(GLenum, GLfloat, GLfloat, GLfloat, GLfloat) INIT_FUNC_POINTERS;
void (GLApi *glMultiTexCoord4fv)(GLenum, const GLfloat*) INIT_FUNC_POINTERS;
void (GLApi *glMultiTexCoord4i)(GLenum, GLint, GLint, GLint, GLint) INIT_FUNC_POINTERS;
void (GLApi *glMultiTexCoord4iv)(GLenum, const GLint*) INIT_FUNC_POINTERS;
void (GLApi *glMultiTexCoord4s)(GLenum, GLshort, GLshort, GLshort, GLshort) INIT_FUNC_POINTERS;
void (GLApi *glMultiTexCoord4sv)(GLenum, const GLshort*) INIT_FUNC_POINTERS;*/

#endif // __GENERATE_GL_13__

bool GLEH_OPENGL_1_3 = false;

int CGL3ExtensionHandler::n_GetGL13FuncPointers()
{
	int n_failed_function_num = 0;

#ifdef __GENERATE_GL_13__

	// functions below are deprecated
	/*if(!(glLoadTransposeMatrixf = (void(GLApi*)(const GLfloat*))wglGetProcAddress("glLoadTransposeMatrixf"))) ++ n_failed_function_num;
	if(!(glLoadTransposeMatrixd = (void(GLApi*)(const GLdouble*))wglGetProcAddress("glLoadTransposeMatrixd"))) ++ n_failed_function_num;
	if(!(glMultTransposeMatrixf = (void(GLApi*)(const GLfloat*))wglGetProcAddress("glMultTransposeMatrixf"))) ++ n_failed_function_num;
	if(!(glMultTransposeMatrixd = (void(GLApi*)(const GLdouble*))wglGetProcAddress("glMultTransposeMatrixd"))) ++ n_failed_function_num;*/

	if(!(glActiveTexture = (void(GLApi*)(GLenum))wglGetProcAddress("glActiveTexture"))) ++ n_failed_function_num;
	if(!(glSampleCoverage = (void(GLApi*)(GLclampf, GLboolean))wglGetProcAddress("glSampleCoverage"))) ++ n_failed_function_num;
	if(!(glCompressedTexImage3D = (void(GLApi*)(GLenum, GLint, GLenum, GLsizei, GLsizei, GLsizei, GLint, GLsizei, const GLvoid*))wglGetProcAddress("glCompressedTexImage3D"))) ++ n_failed_function_num;
	if(!(glCompressedTexImage2D = (void(GLApi*)(GLenum, GLint, GLenum, GLsizei, GLsizei, GLint, GLsizei, const GLvoid*))wglGetProcAddress("glCompressedTexImage2D"))) ++ n_failed_function_num;
	if(!(glCompressedTexImage1D = (void(GLApi*)(GLenum, GLint, GLenum, GLsizei, GLint, GLsizei, const GLvoid*))wglGetProcAddress("glCompressedTexImage1D"))) ++ n_failed_function_num;
	if(!(glCompressedTexSubImage3D = (void(GLApi*)(GLenum, GLint, GLint, GLint, GLint, GLsizei, GLsizei, GLsizei, GLenum, GLsizei, const GLvoid*))wglGetProcAddress("glCompressedTexSubImage3D"))) ++ n_failed_function_num;
	if(!(glCompressedTexSubImage2D = (void(GLApi*)(GLenum, GLint, GLint, GLint, GLsizei, GLsizei, GLenum, GLsizei, const GLvoid*))wglGetProcAddress("glCompressedTexSubImage2D"))) ++ n_failed_function_num;
	if(!(glCompressedTexSubImage1D = (void(GLApi*)(GLenum, GLint, GLint, GLsizei, GLenum, GLsizei, const GLvoid*))wglGetProcAddress("glCompressedTexSubImage1D"))) ++ n_failed_function_num;
	if(!(glGetCompressedTexImage = (void(GLApi*)(GLenum, GLint, GLvoid*))wglGetProcAddress("glGetCompressedTexImage"))) ++ n_failed_function_num;

	// functions below are deprecated
	/*if(!(glClientActiveTexture = (void(GLApi*)(GLenum))wglGetProcAddress("glClientActiveTexture"))) ++ n_failed_function_num;
	if(!(glMultiTexCoord1d = (void(GLApi*)(GLenum, GLdouble))wglGetProcAddress("glMultiTexCoord1d"))) ++ n_failed_function_num;
	if(!(glMultiTexCoord1dv = (void(GLApi*)(GLenum, const GLdouble*))wglGetProcAddress("glMultiTexCoord1dv"))) ++ n_failed_function_num;
	if(!(glMultiTexCoord1f = (void(GLApi*)(GLenum, GLfloat))wglGetProcAddress("glMultiTexCoord1f"))) ++ n_failed_function_num;
	if(!(glMultiTexCoord1fv = (void(GLApi*)(GLenum, const GLfloat*))wglGetProcAddress("glMultiTexCoord1fv"))) ++ n_failed_function_num;
	if(!(glMultiTexCoord1i = (void(GLApi*)(GLenum, GLint))wglGetProcAddress("glMultiTexCoord1i"))) ++ n_failed_function_num;
	if(!(glMultiTexCoord1iv = (void(GLApi*)(GLenum, const GLint *))wglGetProcAddress("glMultiTexCoord1iv"))) ++ n_failed_function_num;
	if(!(glMultiTexCoord1s = (void(GLApi*)(GLenum, GLshort))wglGetProcAddress("glMultiTexCoord1s"))) ++ n_failed_function_num;
	if(!(glMultiTexCoord1sv = (void(GLApi*)(GLenum, const GLshort*))wglGetProcAddress("glMultiTexCoord1sv"))) ++ n_failed_function_num;
	if(!(glMultiTexCoord2d = (void(GLApi*)(GLenum, GLdouble, GLdouble))wglGetProcAddress("glMultiTexCoord2d"))) ++ n_failed_function_num;
	if(!(glMultiTexCoord2dv = (void(GLApi*)(GLenum, const GLdouble*))wglGetProcAddress("glMultiTexCoord2dv"))) ++ n_failed_function_num;
	if(!(glMultiTexCoord2f = (void(GLApi*)(GLenum, GLfloat, GLfloat))wglGetProcAddress("glMultiTexCoord2f"))) ++ n_failed_function_num;
	if(!(glMultiTexCoord2fv = (void(GLApi*)(GLenum, const GLfloat*))wglGetProcAddress("glMultiTexCoord2fv"))) ++ n_failed_function_num;
	if(!(glMultiTexCoord2i = (void(GLApi*)(GLenum, GLint, GLint))wglGetProcAddress("glMultiTexCoord2i"))) ++ n_failed_function_num;
	if(!(glMultiTexCoord2iv = (void(GLApi*)(GLenum, const GLint*))wglGetProcAddress("glMultiTexCoord2iv"))) ++ n_failed_function_num;
	if(!(glMultiTexCoord2s = (void(GLApi*)(GLenum, GLshort, GLshort))wglGetProcAddress("glMultiTexCoord2s"))) ++ n_failed_function_num;
	if(!(glMultiTexCoord2sv = (void(GLApi*)(GLenum, const GLshort*))wglGetProcAddress("glMultiTexCoord2sv"))) ++ n_failed_function_num;
	if(!(glMultiTexCoord3d = (void(GLApi*)(GLenum, GLdouble, GLdouble, GLdouble))wglGetProcAddress("glMultiTexCoord3d"))) ++ n_failed_function_num;
	if(!(glMultiTexCoord3dv = (void(GLApi*)(GLenum, const GLdouble*))wglGetProcAddress("glMultiTexCoord3dv"))) ++ n_failed_function_num;
	if(!(glMultiTexCoord3f = (void(GLApi*)(GLenum, GLfloat, GLfloat, GLfloat))wglGetProcAddress("glMultiTexCoord3f"))) ++ n_failed_function_num;
	if(!(glMultiTexCoord3fv = (void(GLApi*)(GLenum, const GLfloat*))wglGetProcAddress("glMultiTexCoord3fv"))) ++ n_failed_function_num;
	if(!(glMultiTexCoord3i = (void(GLApi*)(GLenum, GLint, GLint, GLint))wglGetProcAddress("glMultiTexCoord3i"))) ++ n_failed_function_num;
	if(!(glMultiTexCoord3iv = (void(GLApi*)(GLenum, const GLint*))wglGetProcAddress("glMultiTexCoord3iv"))) ++ n_failed_function_num;
	if(!(glMultiTexCoord3s = (void(GLApi*)(GLenum, GLshort, GLshort, GLshort))wglGetProcAddress("glMultiTexCoord3s"))) ++ n_failed_function_num;
	if(!(glMultiTexCoord3sv = (void(GLApi*)(GLenum, const GLshort*))wglGetProcAddress("glMultiTexCoord3sv"))) ++ n_failed_function_num;
	if(!(glMultiTexCoord4d = (void(GLApi*)(GLenum, GLdouble, GLdouble, GLdouble, GLdouble))wglGetProcAddress("glMultiTexCoord4d"))) ++ n_failed_function_num;
	if(!(glMultiTexCoord4dv = (void(GLApi*)(GLenum, const GLdouble*))wglGetProcAddress("glMultiTexCoord4dv"))) ++ n_failed_function_num;
	if(!(glMultiTexCoord4f = (void(GLApi*)(GLenum, GLfloat, GLfloat, GLfloat, GLfloat))wglGetProcAddress("glMultiTexCoord4f"))) ++ n_failed_function_num;
	if(!(glMultiTexCoord4fv = (void(GLApi*)(GLenum, const GLfloat*))wglGetProcAddress("glMultiTexCoord4fv"))) ++ n_failed_function_num;
	if(!(glMultiTexCoord4i = (void(GLApi*)(GLenum, GLint, GLint, GLint, GLint))wglGetProcAddress("glMultiTexCoord4i"))) ++ n_failed_function_num;
	if(!(glMultiTexCoord4iv = (void(GLApi*)(GLenum, const GLint*))wglGetProcAddress("glMultiTexCoord4iv"))) ++ n_failed_function_num;
	if(!(glMultiTexCoord4s = (void(GLApi*)(GLenum, GLshort, GLshort, GLshort, GLshort))wglGetProcAddress("glMultiTexCoord4s"))) ++ n_failed_function_num;
	if(!(glMultiTexCoord4sv = (void(GLApi*)(GLenum, const GLshort*))wglGetProcAddress("glMultiTexCoord4sv"))) ++ n_failed_function_num;*/

#endif // __GENERATE_GL_13__

	GLEH_OPENGL_1_3 = !n_failed_function_num && b_Support_OpenGL(1, 3);

	return n_failed_function_num;
}

/*
 *								--- OpenGL 1.4 ---
 */

#ifdef __GENERATE_GL_14__

void (GLApi *glBlendFuncSeparate)(GLenum, GLenum, GLenum, GLenum) INIT_FUNC_POINTERS;

// functions below are deprecated
/*void (GLApi *glFogCoordf)(GLfloat) INIT_FUNC_POINTERS;
void (GLApi *glFogCoordfv)(const GLfloat*) INIT_FUNC_POINTERS;
void (GLApi *glFogCoordd)(GLdouble) INIT_FUNC_POINTERS;
void (GLApi *glFogCoorddv)(const GLdouble*) INIT_FUNC_POINTERS;
void (GLApi *glFogCoordPointer)(GLenum, GLsizei, const GLvoid*) INIT_FUNC_POINTERS;*/

void (GLApi *glMultiDrawArrays)(GLenum, GLint *, GLsizei *, GLsizei) INIT_FUNC_POINTERS;
void (GLApi *glMultiDrawElements)(GLenum, const GLsizei *, GLenum, const GLvoid* *, GLsizei) INIT_FUNC_POINTERS;
void (GLApi *glPointParameterf)(GLenum, GLfloat) INIT_FUNC_POINTERS;
void (GLApi *glPointParameterfv)(GLenum, const GLfloat*) INIT_FUNC_POINTERS;
void (GLApi *glPointParameteri)(GLenum, GLint) INIT_FUNC_POINTERS;
void (GLApi *glPointParameteriv)(GLenum, const GLint*) INIT_FUNC_POINTERS;

// functions below are deprecated
/*void (GLApi *glSecondaryColor3b)(GLbyte, GLbyte, GLbyte) INIT_FUNC_POINTERS;
void (GLApi *glSecondaryColor3bv)(const GLbyte*) INIT_FUNC_POINTERS;
void (GLApi *glSecondaryColor3d)(GLdouble, GLdouble, GLdouble) INIT_FUNC_POINTERS;
void (GLApi *glSecondaryColor3dv)(const GLdouble*) INIT_FUNC_POINTERS;
void (GLApi *glSecondaryColor3f)(GLfloat, GLfloat, GLfloat) INIT_FUNC_POINTERS;
void (GLApi *glSecondaryColor3fv)(const GLfloat*) INIT_FUNC_POINTERS;
void (GLApi *glSecondaryColor3i)(GLint, GLint, GLint) INIT_FUNC_POINTERS;
void (GLApi *glSecondaryColor3iv)(const GLint*) INIT_FUNC_POINTERS;
void (GLApi *glSecondaryColor3s)(GLshort, GLshort, GLshort) INIT_FUNC_POINTERS;
void (GLApi *glSecondaryColor3sv)(const GLshort*) INIT_FUNC_POINTERS;
void (GLApi *glSecondaryColor3ub)(GLubyte, GLubyte, GLubyte) INIT_FUNC_POINTERS;
void (GLApi *glSecondaryColor3ubv)(const GLubyte*) INIT_FUNC_POINTERS;
void (GLApi *glSecondaryColor3ui)(GLuint, GLuint, GLuint) INIT_FUNC_POINTERS;
void (GLApi *glSecondaryColor3uiv)(const GLuint*) INIT_FUNC_POINTERS;
void (GLApi *glSecondaryColor3us)(GLushort, GLushort, GLushort) INIT_FUNC_POINTERS;
void (GLApi *glSecondaryColor3usv)(const GLushort*) INIT_FUNC_POINTERS;
void (GLApi *glSecondaryColorPointer)(GLint, GLenum, GLsizei, const GLvoid*) INIT_FUNC_POINTERS;
void (GLApi *glWindowPos2d)(GLdouble, GLdouble) INIT_FUNC_POINTERS;
void (GLApi *glWindowPos2dv)(const GLdouble*) INIT_FUNC_POINTERS;
void (GLApi *glWindowPos2f)(GLfloat, GLfloat) INIT_FUNC_POINTERS;
void (GLApi *glWindowPos2fv)(const GLfloat*) INIT_FUNC_POINTERS;
void (GLApi *glWindowPos2i)(GLint, GLint) INIT_FUNC_POINTERS;
void (GLApi *glWindowPos2iv)(const GLint*) INIT_FUNC_POINTERS;
void (GLApi *glWindowPos2s)(GLshort, GLshort) INIT_FUNC_POINTERS;
void (GLApi *glWindowPos2sv)(const GLshort*) INIT_FUNC_POINTERS;
void (GLApi *glWindowPos3d)(GLdouble, GLdouble, GLdouble) INIT_FUNC_POINTERS;
void (GLApi *glWindowPos3dv)(const GLdouble*) INIT_FUNC_POINTERS;
void (GLApi *glWindowPos3f)(GLfloat, GLfloat, GLfloat) INIT_FUNC_POINTERS;
void (GLApi *glWindowPos3fv)(const GLfloat*) INIT_FUNC_POINTERS;
void (GLApi *glWindowPos3i)(GLint, GLint, GLint) INIT_FUNC_POINTERS;
void (GLApi *glWindowPos3iv)(const GLint*) INIT_FUNC_POINTERS;
void (GLApi *glWindowPos3s)(GLshort, GLshort, GLshort) INIT_FUNC_POINTERS;
void (GLApi *glWindowPos3sv)(const GLshort*) INIT_FUNC_POINTERS;*/

#endif // __GENERATE_GL_14__

bool GLEH_OPENGL_1_4 = false;

int CGL3ExtensionHandler::n_GetGL14FuncPointers()
{
	int n_failed_function_num = 0;

#ifdef __GENERATE_GL_14__

	if(!(glBlendFuncSeparate = (void(GLApi*)(GLenum, GLenum, GLenum, GLenum))wglGetProcAddress("glBlendFuncSeparate"))) ++ n_failed_function_num;

	// functions below are deprecated
	/*if(!(glFogCoordf = (void(GLApi*)(GLfloat))wglGetProcAddress("glFogCoordf"))) ++ n_failed_function_num;
	if(!(glFogCoordfv = (void(GLApi*)(const GLfloat*))wglGetProcAddress("glFogCoordfv"))) ++ n_failed_function_num;
	if(!(glFogCoordd = (void(GLApi*)(GLdouble))wglGetProcAddress("glFogCoordd"))) ++ n_failed_function_num;
	if(!(glFogCoorddv = (void(GLApi*)(const GLdouble*))wglGetProcAddress("glFogCoorddv"))) ++ n_failed_function_num;
	if(!(glFogCoordPointer = (void(GLApi*)(GLenum, GLsizei, const GLvoid*))wglGetProcAddress("glFogCoordPointer"))) ++ n_failed_function_num;*/

	if(!(glMultiDrawArrays = (void(GLApi*)(GLenum, GLint *, GLsizei *, GLsizei))wglGetProcAddress("glMultiDrawArrays"))) ++ n_failed_function_num;
	if(!(glMultiDrawElements = (void(GLApi*)(GLenum, const GLsizei *, GLenum, const GLvoid* *, GLsizei))wglGetProcAddress("glMultiDrawElements"))) ++ n_failed_function_num;
	if(!(glPointParameterf = (void(GLApi*)(GLenum, GLfloat))wglGetProcAddress("glPointParameterf"))) ++ n_failed_function_num;
	if(!(glPointParameterfv = (void(GLApi*)(GLenum, const GLfloat*))wglGetProcAddress("glPointParameterfv"))) ++ n_failed_function_num;
	if(!(glPointParameteri = (void(GLApi*)(GLenum, GLint))wglGetProcAddress("glPointParameteri"))) ++ n_failed_function_num;
	if(!(glPointParameteriv = (void(GLApi*)(GLenum, const GLint*))wglGetProcAddress("glPointParameteriv"))) ++ n_failed_function_num;

	// functions below are deprecated
	/*if(!(glSecondaryColor3b = (void(GLApi*)(GLbyte, GLbyte, GLbyte))wglGetProcAddress("glSecondaryColor3b"))) ++ n_failed_function_num;
	if(!(glSecondaryColor3bv = (void(GLApi*)(const GLbyte*))wglGetProcAddress("glSecondaryColor3bv"))) ++ n_failed_function_num;
	if(!(glSecondaryColor3d = (void(GLApi*)(GLdouble, GLdouble, GLdouble))wglGetProcAddress("glSecondaryColor3d"))) ++ n_failed_function_num;
	if(!(glSecondaryColor3dv = (void(GLApi*)(const GLdouble*))wglGetProcAddress("glSecondaryColor3dv"))) ++ n_failed_function_num;
	if(!(glSecondaryColor3f = (void(GLApi*)(GLfloat, GLfloat, GLfloat))wglGetProcAddress("glSecondaryColor3f"))) ++ n_failed_function_num;
	if(!(glSecondaryColor3fv = (void(GLApi*)(const GLfloat*))wglGetProcAddress("glSecondaryColor3fv"))) ++ n_failed_function_num;
	if(!(glSecondaryColor3i = (void(GLApi*)(GLint, GLint, GLint))wglGetProcAddress("glSecondaryColor3i"))) ++ n_failed_function_num;
	if(!(glSecondaryColor3iv = (void(GLApi*)(const GLint*))wglGetProcAddress("glSecondaryColor3iv"))) ++ n_failed_function_num;
	if(!(glSecondaryColor3s = (void(GLApi*)(GLshort, GLshort, GLshort))wglGetProcAddress("glSecondaryColor3s"))) ++ n_failed_function_num;
	if(!(glSecondaryColor3sv = (void(GLApi*)(const GLshort*))wglGetProcAddress("glSecondaryColor3sv"))) ++ n_failed_function_num;
	if(!(glSecondaryColor3ub = (void(GLApi*)(GLubyte, GLubyte, GLubyte))wglGetProcAddress("glSecondaryColor3ub"))) ++ n_failed_function_num;
	if(!(glSecondaryColor3ubv = (void(GLApi*)(const GLubyte*))wglGetProcAddress("glSecondaryColor3ubv"))) ++ n_failed_function_num;
	if(!(glSecondaryColor3ui = (void(GLApi*)(GLuint, GLuint, GLuint))wglGetProcAddress("glSecondaryColor3ui"))) ++ n_failed_function_num;
	if(!(glSecondaryColor3uiv = (void(GLApi*)(const GLuint*))wglGetProcAddress("glSecondaryColor3uiv"))) ++ n_failed_function_num;
	if(!(glSecondaryColor3us = (void(GLApi*)(GLushort, GLushort, GLushort))wglGetProcAddress("glSecondaryColor3us"))) ++ n_failed_function_num;
	if(!(glSecondaryColor3usv = (void(GLApi*)(const GLushort*))wglGetProcAddress("glSecondaryColor3usv"))) ++ n_failed_function_num;
	if(!(glSecondaryColorPointer = (void(GLApi*)(GLint, GLenum, GLsizei, const GLvoid*))wglGetProcAddress("glSecondaryColorPointer"))) ++ n_failed_function_num;
	if(!(glWindowPos2d = (void(GLApi*)(GLdouble, GLdouble))wglGetProcAddress("glWindowPos2d"))) ++ n_failed_function_num;
	if(!(glWindowPos2dv = (void(GLApi*)(const GLdouble*))wglGetProcAddress("glWindowPos2dv"))) ++ n_failed_function_num;
	if(!(glWindowPos2f = (void(GLApi*)(GLfloat, GLfloat))wglGetProcAddress("glWindowPos2f"))) ++ n_failed_function_num;
	if(!(glWindowPos2fv = (void(GLApi*)(const GLfloat*))wglGetProcAddress("glWindowPos2fv"))) ++ n_failed_function_num;
	if(!(glWindowPos2i = (void(GLApi*)(GLint, GLint))wglGetProcAddress("glWindowPos2i"))) ++ n_failed_function_num;
	if(!(glWindowPos2iv = (void(GLApi*)(const GLint*))wglGetProcAddress("glWindowPos2iv"))) ++ n_failed_function_num;
	if(!(glWindowPos2s = (void(GLApi*)(GLshort, GLshort))wglGetProcAddress("glWindowPos2s"))) ++ n_failed_function_num;
	if(!(glWindowPos2sv = (void(GLApi*)(const GLshort*))wglGetProcAddress("glWindowPos2sv"))) ++ n_failed_function_num;
	if(!(glWindowPos3d = (void(GLApi*)(GLdouble, GLdouble, GLdouble))wglGetProcAddress("glWindowPos3d"))) ++ n_failed_function_num;
	if(!(glWindowPos3dv = (void(GLApi*)(const GLdouble*))wglGetProcAddress("glWindowPos3dv"))) ++ n_failed_function_num;
	if(!(glWindowPos3f = (void(GLApi*)(GLfloat, GLfloat, GLfloat))wglGetProcAddress("glWindowPos3f"))) ++ n_failed_function_num;
	if(!(glWindowPos3fv = (void(GLApi*)(const GLfloat*))wglGetProcAddress("glWindowPos3fv"))) ++ n_failed_function_num;
	if(!(glWindowPos3i = (void(GLApi*)(GLint, GLint, GLint))wglGetProcAddress("glWindowPos3i"))) ++ n_failed_function_num;
	if(!(glWindowPos3iv = (void(GLApi*)(const GLint*))wglGetProcAddress("glWindowPos3iv"))) ++ n_failed_function_num;
	if(!(glWindowPos3s = (void(GLApi*)(GLshort, GLshort, GLshort))wglGetProcAddress("glWindowPos3s"))) ++ n_failed_function_num;
	if(!(glWindowPos3sv = (void(GLApi*)(const GLshort*))wglGetProcAddress("glWindowPos3sv"))) ++ n_failed_function_num;*/

#endif // __GENERATE_GL_14__

	GLEH_OPENGL_1_4 = !n_failed_function_num && b_Support_OpenGL(1, 4);

	return n_failed_function_num;
}

/*
 *								--- OpenGL 1.5 ---
 */

#ifdef __GENERATE_GL_15__

void (GLApi *glGenQueries)(GLsizei, GLuint*) INIT_FUNC_POINTERS;
void (GLApi *glDeleteQueries)(GLsizei, const GLuint*) INIT_FUNC_POINTERS;
GLboolean (GLApi *glIsQuery)(GLuint) INIT_FUNC_POINTERS;
void (GLApi *glBeginQuery)(GLenum, GLuint) INIT_FUNC_POINTERS;
void (GLApi *glEndQuery)(GLenum) INIT_FUNC_POINTERS;
void (GLApi *glGetQueryiv)(GLenum, GLenum, GLint*) INIT_FUNC_POINTERS;
void (GLApi *glGetQueryObjectiv)(GLuint, GLenum, GLint*) INIT_FUNC_POINTERS;
void (GLApi *glGetQueryObjectuiv)(GLuint, GLenum, GLuint*) INIT_FUNC_POINTERS;
void (GLApi *glBindBuffer)(GLenum, GLuint) INIT_FUNC_POINTERS;
void (GLApi *glDeleteBuffers)(GLsizei, const GLuint*) INIT_FUNC_POINTERS;
void (GLApi *glGenBuffers)(GLsizei, GLuint*) INIT_FUNC_POINTERS;
GLboolean (GLApi *glIsBuffer)(GLuint) INIT_FUNC_POINTERS;
void (GLApi *glBufferData)(GLenum, GLsizeiptr, const GLvoid *, GLenum) INIT_FUNC_POINTERS;
void (GLApi *glBufferSubData)(GLenum, GLintptr, GLsizeiptr, const GLvoid*) INIT_FUNC_POINTERS;
void (GLApi *glGetBufferSubData)(GLenum, GLintptr, GLsizeiptr, GLvoid*) INIT_FUNC_POINTERS;
GLvoid* (GLApi *glMapBuffer)(GLenum, GLenum) INIT_FUNC_POINTERS;
GLboolean (GLApi *glUnmapBuffer)(GLenum) INIT_FUNC_POINTERS;
void (GLApi *glGetBufferParameteriv)(GLenum, GLenum, GLint*) INIT_FUNC_POINTERS;
void (GLApi *glGetBufferPointerv)(GLenum, GLenum, GLvoid**) INIT_FUNC_POINTERS;

#endif // __GENERATE_GL_15__

bool GLEH_OPENGL_1_5 = false;

int CGL3ExtensionHandler::n_GetGL15FuncPointers()
{
	int n_failed_function_num = 0;

#ifdef __GENERATE_GL_15__

	if(!(glGenQueries = (void(GLApi*)(GLsizei, GLuint*))wglGetProcAddress("glGenQueries"))) ++ n_failed_function_num;
	if(!(glDeleteQueries = (void(GLApi*)(GLsizei, const GLuint*))wglGetProcAddress("glDeleteQueries"))) ++ n_failed_function_num;
	if(!(glIsQuery = (GLboolean(GLApi*)(GLuint))wglGetProcAddress("glIsQuery"))) ++ n_failed_function_num;
	if(!(glBeginQuery = (void(GLApi*)(GLenum, GLuint))wglGetProcAddress("glBeginQuery"))) ++ n_failed_function_num;
	if(!(glEndQuery = (void(GLApi*)(GLenum))wglGetProcAddress("glEndQuery"))) ++ n_failed_function_num;
	if(!(glGetQueryiv = (void(GLApi*)(GLenum, GLenum, GLint*))wglGetProcAddress("glGetQueryiv"))) ++ n_failed_function_num;
	if(!(glGetQueryObjectiv = (void(GLApi*)(GLuint, GLenum, GLint*))wglGetProcAddress("glGetQueryObjectiv"))) ++ n_failed_function_num;
	if(!(glGetQueryObjectuiv = (void(GLApi*)(GLuint, GLenum, GLuint*))wglGetProcAddress("glGetQueryObjectuiv"))) ++ n_failed_function_num;
	if(!(glBindBuffer = (void(GLApi*)(GLenum, GLuint))wglGetProcAddress("glBindBuffer"))) ++ n_failed_function_num;
	if(!(glDeleteBuffers = (void(GLApi*)(GLsizei, const GLuint*))wglGetProcAddress("glDeleteBuffers"))) ++ n_failed_function_num;
	if(!(glGenBuffers = (void(GLApi*)(GLsizei, GLuint*))wglGetProcAddress("glGenBuffers"))) ++ n_failed_function_num;
	if(!(glIsBuffer = (GLboolean(GLApi*)(GLuint))wglGetProcAddress("glIsBuffer"))) ++ n_failed_function_num;
	if(!(glBufferData = (void(GLApi*)(GLenum, GLsizeiptr, const GLvoid *, GLenum))wglGetProcAddress("glBufferData"))) ++ n_failed_function_num;
	if(!(glBufferSubData = (void(GLApi*)(GLenum, GLintptr, GLsizeiptr, const GLvoid*))wglGetProcAddress("glBufferSubData"))) ++ n_failed_function_num;
	if(!(glGetBufferSubData = (void(GLApi*)(GLenum, GLintptr, GLsizeiptr, GLvoid*))wglGetProcAddress("glGetBufferSubData"))) ++ n_failed_function_num;
	if(!(glMapBuffer = (GLvoid*(GLApi*)(GLenum, GLenum))wglGetProcAddress("glMapBuffer"))) ++ n_failed_function_num;
	if(!(glUnmapBuffer = (GLboolean(GLApi*)(GLenum))wglGetProcAddress("glUnmapBuffer"))) ++ n_failed_function_num;
	if(!(glGetBufferParameteriv = (void(GLApi*)(GLenum, GLenum, GLint*))wglGetProcAddress("glGetBufferParameteriv"))) ++ n_failed_function_num;
	if(!(glGetBufferPointerv = (void(GLApi*)(GLenum, GLenum, GLvoid**))wglGetProcAddress("glGetBufferPointerv"))) ++ n_failed_function_num;

#endif // __GENERATE_GL_15__

	GLEH_OPENGL_1_5 = !n_failed_function_num && b_Support_OpenGL(1, 5);

	return n_failed_function_num;
}

/*
 *								--- OpenGL 2.0 ---
 */

#ifdef __GENERATE_GL_20__

void (GLApi *glDeleteShader)(GLuint shader) INIT_FUNC_POINTERS;
void (GLApi *glDetachShader)(GLuint program, GLuint shader) INIT_FUNC_POINTERS;
GLuint (GLApi *glCreateShader)(GLenum type) INIT_FUNC_POINTERS;
void (GLApi *glShaderSource)(GLuint shader, GLsizei count, const GLchar* *string, const GLint *length) INIT_FUNC_POINTERS;
void (GLApi *glCompileShader)(GLuint shader) INIT_FUNC_POINTERS;
GLuint (GLApi *glCreateProgram)(void) INIT_FUNC_POINTERS;
void (GLApi *glAttachShader)(GLuint program, GLuint shader) INIT_FUNC_POINTERS;
void (GLApi *glLinkProgram)(GLuint program) INIT_FUNC_POINTERS;
void (GLApi *glUseProgram)(GLuint program) INIT_FUNC_POINTERS;
void (GLApi *glDeleteProgram)(GLuint program) INIT_FUNC_POINTERS;
void (GLApi *glValidateProgram)(GLuint program) INIT_FUNC_POINTERS;
void (GLApi *glUniform1f)(GLint location, GLfloat v0) INIT_FUNC_POINTERS;
void (GLApi *glUniform2f)(GLint location, GLfloat v0, GLfloat v1) INIT_FUNC_POINTERS;
void (GLApi *glUniform3f)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2) INIT_FUNC_POINTERS;
void (GLApi *glUniform4f)(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) INIT_FUNC_POINTERS;
void (GLApi *glUniform1i)(GLint location, GLint v0) INIT_FUNC_POINTERS;
void (GLApi *glUniform2i)(GLint location, GLint v0, GLint v1) INIT_FUNC_POINTERS;
void (GLApi *glUniform3i)(GLint location, GLint v0, GLint v1, GLint v2) INIT_FUNC_POINTERS;
void (GLApi *glUniform4i)(GLint location, GLint v0, GLint v1, GLint v2, GLint v3) INIT_FUNC_POINTERS;
void (GLApi *glUniform1fv)(GLint location, GLsizei count, const GLfloat *value) INIT_FUNC_POINTERS;
void (GLApi *glUniform2fv)(GLint location, GLsizei count, const GLfloat *value) INIT_FUNC_POINTERS;
void (GLApi *glUniform3fv)(GLint location, GLsizei count, const GLfloat *value) INIT_FUNC_POINTERS;
void (GLApi *glUniform4fv)(GLint location, GLsizei count, const GLfloat *value) INIT_FUNC_POINTERS;
void (GLApi *glUniform1iv)(GLint location, GLsizei count, const GLint *value) INIT_FUNC_POINTERS;
void (GLApi *glUniform2iv)(GLint location, GLsizei count, const GLint *value) INIT_FUNC_POINTERS;
void (GLApi *glUniform3iv)(GLint location, GLsizei count, const GLint *value) INIT_FUNC_POINTERS;
void (GLApi *glUniform4iv)(GLint location, GLsizei count, const GLint *value) INIT_FUNC_POINTERS;
void (GLApi *glUniformMatrix2fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) INIT_FUNC_POINTERS;
void (GLApi *glUniformMatrix3fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) INIT_FUNC_POINTERS;
void (GLApi *glUniformMatrix4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat *value) INIT_FUNC_POINTERS;
void (GLApi *glBindAttribLocation)(GLuint program, GLuint index, const GLchar *name) INIT_FUNC_POINTERS;
GLint (GLApi *glGetAttribLocation)(GLuint program, const GLchar *name) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttrib1d)(GLuint index, GLdouble x) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttrib1dv)(GLuint index, const GLdouble *v) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttrib1f)(GLuint index, GLfloat x) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttrib1fv)(GLuint index, const GLfloat *v) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttrib1s)(GLuint index, GLshort x) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttrib1sv)(GLuint index, const GLshort *v) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttrib2d)(GLuint index, GLdouble x, GLdouble y) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttrib2dv)(GLuint index, const GLdouble *v) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttrib2f)(GLuint index, GLfloat x, GLfloat y) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttrib2fv)(GLuint index, const GLfloat *v) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttrib2s)(GLuint index, GLshort x, GLshort y) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttrib2sv)(GLuint index, const GLshort *v) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttrib3d)(GLuint index, GLdouble x, GLdouble y, GLdouble z) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttrib3dv)(GLuint index, const GLdouble *v) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttrib3f)(GLuint index, GLfloat x, GLfloat y, GLfloat z) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttrib3fv)(GLuint index, const GLfloat *v) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttrib3s)(GLuint index, GLshort x, GLshort y, GLshort z) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttrib3sv)(GLuint index, const GLshort *v) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttrib4Nbv)(GLuint index, const GLbyte *v) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttrib4Niv)(GLuint index, const GLint *v) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttrib4Nsv)(GLuint index, const GLshort *v) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttrib4Nub)(GLuint index, GLubyte x, GLubyte y, GLubyte z, GLubyte w) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttrib4Nubv)(GLuint index, const GLubyte *v) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttrib4Nuiv)(GLuint index, const GLuint *v) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttrib4Nusv)(GLuint index, const GLushort *v) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttrib4bv)(GLuint index, const GLbyte *v) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttrib4d)(GLuint index, GLdouble x, GLdouble y, GLdouble z, GLdouble w) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttrib4dv)(GLuint index, const GLdouble *v) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttrib4f)(GLuint index, GLfloat x, GLfloat y, GLfloat z, GLfloat w) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttrib4fv)(GLuint index, const GLfloat *v) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttrib4iv)(GLuint index, const GLint *v) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttrib4s)(GLuint index, GLshort x, GLshort y, GLshort z, GLshort w) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttrib4sv)(GLuint index, const GLshort *v) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttrib4ubv)(GLuint index, const GLubyte *v) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttrib4uiv)(GLuint index, const GLuint *v) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttrib4usv)(GLuint index, const GLushort *v) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttribPointer)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const GLvoid *pointer) INIT_FUNC_POINTERS;
void (GLApi *glEnableVertexAttribArray)(GLuint index) INIT_FUNC_POINTERS;
void (GLApi *glDisableVertexAttribArray)(GLuint index) INIT_FUNC_POINTERS;
void (GLApi *glGetVertexAttribdv)(GLuint index, GLenum pname, GLdouble *params) INIT_FUNC_POINTERS;
void (GLApi *glGetVertexAttribfv)(GLuint index, GLenum pname, GLfloat *params) INIT_FUNC_POINTERS;
void (GLApi *glGetVertexAttribiv)(GLuint index, GLenum pname, GLint *params) INIT_FUNC_POINTERS;
void (GLApi *glGetVertexAttribPointerv)(GLuint index, GLenum pname, GLvoid* *pointer) INIT_FUNC_POINTERS;
GLboolean (GLApi *glIsShader)(GLuint shader) INIT_FUNC_POINTERS;
GLboolean (GLApi *glIsProgram)(GLuint program) INIT_FUNC_POINTERS;
void (GLApi *glGetShaderiv)(GLuint program, GLenum pname, GLint *params) INIT_FUNC_POINTERS;
void (GLApi *glGetProgramiv)(GLuint program, GLenum pname, GLint *params) INIT_FUNC_POINTERS;
void (GLApi *glGetAttachedShaders)(GLuint program, GLsizei maxCount, GLsizei *count, GLuint *shaders) INIT_FUNC_POINTERS;
void (GLApi *glGetShaderInfoLog)(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *infoLog) INIT_FUNC_POINTERS;
void (GLApi *glGetProgramInfoLog)(GLuint program, GLsizei bufSize, GLsizei *length, GLchar *infoLog) INIT_FUNC_POINTERS;
GLint (GLApi *glGetUniformLocation)(GLuint program, const GLchar *name) INIT_FUNC_POINTERS;
void (GLApi *glGetActiveUniform)(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name) INIT_FUNC_POINTERS;
void (GLApi *glGetUniformfv)(GLuint program, GLint location, GLfloat *params) INIT_FUNC_POINTERS;
void (GLApi *glGetUniformiv)(GLuint program, GLint location, GLint *params) INIT_FUNC_POINTERS;
void (GLApi *glGetShaderSource)(GLuint shader, GLsizei bufSize, GLsizei *length, GLchar *source) INIT_FUNC_POINTERS;
void (GLApi *glGetActiveAttrib)(GLuint program, GLuint index, GLsizei bufSize, GLsizei *length, GLsizei *size, GLenum *type, GLchar *name) INIT_FUNC_POINTERS;
void (GLApi *glDrawBuffers)(GLsizei n, const GLenum *bufs) INIT_FUNC_POINTERS;
void (GLApi *glStencilFuncSeparate)(GLenum face, GLenum func, GLint ref, GLuint mask) INIT_FUNC_POINTERS;
void (GLApi *glStencilOpSeparate)(GLenum face, GLenum fail, GLenum zfail, GLenum zpass) INIT_FUNC_POINTERS;
void (GLApi *glStencilMaskSeparate)(GLenum face, GLuint mask) INIT_FUNC_POINTERS;
void (GLApi *glBlendEquationSeparate)(GLenum modeRGB, GLenum modeAlpha) INIT_FUNC_POINTERS;

#endif // __GENERATE_GL_20__

bool GLEH_OPENGL_2_0 = false;

int CGL3ExtensionHandler::n_GetGL20FuncPointers()
{
	int n_failed_function_num = 0;

#ifdef __GENERATE_GL_20__

	if(!(glDeleteShader = (void(GLApi*)(GLuint))wglGetProcAddress("glDeleteShader"))) ++ n_failed_function_num;
	if(!(glDetachShader = (void(GLApi*)(GLuint, GLuint))wglGetProcAddress("glDetachShader"))) ++ n_failed_function_num;
	if(!(glCreateShader = (GLuint(GLApi*)(GLenum))wglGetProcAddress("glCreateShader"))) ++ n_failed_function_num;
	if(!(glShaderSource = (void(GLApi*)(GLuint, GLsizei, const GLchar**, const GLint*))wglGetProcAddress("glShaderSource"))) ++ n_failed_function_num;
	if(!(glCompileShader = (void(GLApi*)(GLuint))wglGetProcAddress("glCompileShader"))) ++ n_failed_function_num;
	if(!(glCreateProgram = (GLuint(GLApi*)(void))wglGetProcAddress("glCreateProgram"))) ++ n_failed_function_num;
	if(!(glAttachShader = (void(GLApi*)(GLuint, GLuint shader))wglGetProcAddress("glAttachShader"))) ++ n_failed_function_num;
	if(!(glLinkProgram = (void(GLApi*)(GLuint))wglGetProcAddress("glLinkProgram"))) ++ n_failed_function_num;
	if(!(glUseProgram = (void(GLApi*)(GLuint))wglGetProcAddress("glUseProgram"))) ++ n_failed_function_num;
	if(!(glDeleteProgram = (void(GLApi*)(GLuint))wglGetProcAddress("glDeleteProgram"))) ++ n_failed_function_num;
	if(!(glValidateProgram = (void(GLApi*)(GLuint))wglGetProcAddress("glValidateProgram"))) ++ n_failed_function_num;
	if(!(glUniform1f = (void(GLApi*)(GLint, GLfloat))wglGetProcAddress("glUniform1f"))) ++ n_failed_function_num;
	if(!(glUniform2f = (void(GLApi*)(GLint, GLfloat, GLfloat))wglGetProcAddress("glUniform2f"))) ++ n_failed_function_num;
	if(!(glUniform3f = (void(GLApi*)(GLint, GLfloat, GLfloat, GLfloat))wglGetProcAddress("glUniform3f"))) ++ n_failed_function_num;
	if(!(glUniform4f = (void(GLApi*)(GLint, GLfloat, GLfloat, GLfloat, GLfloat))wglGetProcAddress("glUniform4f"))) ++ n_failed_function_num;
	if(!(glUniform1i = (void(GLApi*)(GLint, GLint))wglGetProcAddress("glUniform1i"))) ++ n_failed_function_num;
	if(!(glUniform2i = (void(GLApi*)(GLint, GLint, GLint))wglGetProcAddress("glUniform2i"))) ++ n_failed_function_num;
	if(!(glUniform3i = (void(GLApi*)(GLint, GLint, GLint, GLint))wglGetProcAddress("glUniform3i"))) ++ n_failed_function_num;
	if(!(glUniform4i = (void(GLApi*)(GLint, GLint, GLint, GLint, GLint))wglGetProcAddress("glUniform4i"))) ++ n_failed_function_num;
	if(!(glUniform1fv = (void(GLApi*)(GLint, GLsizei, const GLfloat*))wglGetProcAddress("glUniform1fv"))) ++ n_failed_function_num;
	if(!(glUniform2fv = (void(GLApi*)(GLint, GLsizei, const GLfloat*))wglGetProcAddress("glUniform2fv"))) ++ n_failed_function_num;
	if(!(glUniform3fv = (void(GLApi*)(GLint, GLsizei, const GLfloat*))wglGetProcAddress("glUniform3fv"))) ++ n_failed_function_num;
	if(!(glUniform4fv = (void(GLApi*)(GLint, GLsizei, const GLfloat*))wglGetProcAddress("glUniform4fv"))) ++ n_failed_function_num;
	if(!(glUniform1iv = (void(GLApi*)(GLint, GLsizei, const GLint*))wglGetProcAddress("glUniform1iv"))) ++ n_failed_function_num;
	if(!(glUniform2iv = (void(GLApi*)(GLint, GLsizei, const GLint*))wglGetProcAddress("glUniform2iv"))) ++ n_failed_function_num;
	if(!(glUniform3iv = (void(GLApi*)(GLint, GLsizei, const GLint*))wglGetProcAddress("glUniform3iv"))) ++ n_failed_function_num;
	if(!(glUniform4iv = (void(GLApi*)(GLint, GLsizei, const GLint*))wglGetProcAddress("glUniform4iv"))) ++ n_failed_function_num;
	if(!(glUniformMatrix2fv = (void(GLApi*)(GLint, GLsizei, GLboolean transpose, const GLfloat*))wglGetProcAddress("glUniformMatrix2fv"))) ++ n_failed_function_num;
	if(!(glUniformMatrix3fv = (void(GLApi*)(GLint, GLsizei, GLboolean transpose, const GLfloat*))wglGetProcAddress("glUniformMatrix3fv"))) ++ n_failed_function_num;
	if(!(glUniformMatrix4fv = (void(GLApi*)(GLint, GLsizei, GLboolean transpose, const GLfloat*))wglGetProcAddress("glUniformMatrix4fv"))) ++ n_failed_function_num;
	if(!(glBindAttribLocation = (void(GLApi*)(GLuint, GLuint, const GLchar *name))wglGetProcAddress("glBindAttribLocation"))) ++ n_failed_function_num;
	if(!(glGetAttribLocation = (GLint(GLApi*)(GLuint, const GLchar *name))wglGetProcAddress("glGetAttribLocation"))) ++ n_failed_function_num;
	if(!(glVertexAttrib1d = (void(GLApi*)(GLuint, GLdouble))wglGetProcAddress("glVertexAttrib1d"))) ++ n_failed_function_num;
	if(!(glVertexAttrib1dv = (void(GLApi*)(GLuint, const GLdouble*))wglGetProcAddress("glVertexAttrib1dv"))) ++ n_failed_function_num;
	if(!(glVertexAttrib1f = (void(GLApi*)(GLuint, GLfloat))wglGetProcAddress("glVertexAttrib1f"))) ++ n_failed_function_num;
	if(!(glVertexAttrib1fv = (void(GLApi*)(GLuint, const GLfloat*))wglGetProcAddress("glVertexAttrib1fv"))) ++ n_failed_function_num;
	if(!(glVertexAttrib1s = (void(GLApi*)(GLuint, GLshort))wglGetProcAddress("glVertexAttrib1s"))) ++ n_failed_function_num;
	if(!(glVertexAttrib1sv = (void(GLApi*)(GLuint, const GLshort*))wglGetProcAddress("glVertexAttrib1sv"))) ++ n_failed_function_num;
	if(!(glVertexAttrib2d = (void(GLApi*)(GLuint, GLdouble, GLdouble))wglGetProcAddress("glVertexAttrib2d"))) ++ n_failed_function_num;
	if(!(glVertexAttrib2dv = (void(GLApi*)(GLuint, const GLdouble*))wglGetProcAddress("glVertexAttrib2dv"))) ++ n_failed_function_num;
	if(!(glVertexAttrib2f = (void(GLApi*)(GLuint, GLfloat, GLfloat))wglGetProcAddress("glVertexAttrib2f"))) ++ n_failed_function_num;
	if(!(glVertexAttrib2fv = (void(GLApi*)(GLuint, const GLfloat*))wglGetProcAddress("glVertexAttrib2fv"))) ++ n_failed_function_num;
	if(!(glVertexAttrib2s = (void(GLApi*)(GLuint, GLshort, GLshort))wglGetProcAddress("glVertexAttrib2s"))) ++ n_failed_function_num;
	if(!(glVertexAttrib2sv = (void(GLApi*)(GLuint, const GLshort*))wglGetProcAddress("glVertexAttrib2sv"))) ++ n_failed_function_num;
	if(!(glVertexAttrib3d = (void(GLApi*)(GLuint, GLdouble, GLdouble, GLdouble))wglGetProcAddress("glVertexAttrib3d"))) ++ n_failed_function_num;
	if(!(glVertexAttrib3dv = (void(GLApi*)(GLuint, const GLdouble*))wglGetProcAddress("glVertexAttrib3dv"))) ++ n_failed_function_num;
	if(!(glVertexAttrib3f = (void(GLApi*)(GLuint, GLfloat, GLfloat, GLfloat z))wglGetProcAddress("glVertexAttrib3f"))) ++ n_failed_function_num;
	if(!(glVertexAttrib3fv = (void(GLApi*)(GLuint, const GLfloat*))wglGetProcAddress("glVertexAttrib3fv"))) ++ n_failed_function_num;
	if(!(glVertexAttrib3s = (void(GLApi*)(GLuint, GLshort, GLshort, GLshort z))wglGetProcAddress("glVertexAttrib3s"))) ++ n_failed_function_num;
	if(!(glVertexAttrib3sv = (void(GLApi*)(GLuint, const GLshort*))wglGetProcAddress("glVertexAttrib3sv"))) ++ n_failed_function_num;
	if(!(glVertexAttrib4Nbv = (void(GLApi*)(GLuint, const GLbyte*))wglGetProcAddress("glVertexAttrib4Nbv"))) ++ n_failed_function_num;
	if(!(glVertexAttrib4Niv = (void(GLApi*)(GLuint, const GLint*))wglGetProcAddress("glVertexAttrib4Niv"))) ++ n_failed_function_num;
	if(!(glVertexAttrib4Nsv = (void(GLApi*)(GLuint, const GLshort*))wglGetProcAddress("glVertexAttrib4Nsv"))) ++ n_failed_function_num;
	if(!(glVertexAttrib4Nub = (void(GLApi*)(GLuint, GLubyte, GLubyte, GLubyte, GLubyte))wglGetProcAddress("glVertexAttrib4Nub"))) ++ n_failed_function_num;
	if(!(glVertexAttrib4Nubv = (void(GLApi*)(GLuint, const GLubyte*))wglGetProcAddress("glVertexAttrib4Nubv"))) ++ n_failed_function_num;
	if(!(glVertexAttrib4Nuiv = (void(GLApi*)(GLuint, const GLuint*))wglGetProcAddress("glVertexAttrib4Nuiv"))) ++ n_failed_function_num;
	if(!(glVertexAttrib4Nusv = (void(GLApi*)(GLuint, const GLushort*))wglGetProcAddress("glVertexAttrib4Nusv"))) ++ n_failed_function_num;
	if(!(glVertexAttrib4bv = (void(GLApi*)(GLuint, const GLbyte*))wglGetProcAddress("glVertexAttrib4bv"))) ++ n_failed_function_num;
	if(!(glVertexAttrib4d = (void(GLApi*)(GLuint, GLdouble, GLdouble, GLdouble, GLdouble))wglGetProcAddress("glVertexAttrib4d"))) ++ n_failed_function_num;
	if(!(glVertexAttrib4dv = (void(GLApi*)(GLuint, const GLdouble*))wglGetProcAddress("glVertexAttrib4dv"))) ++ n_failed_function_num;
	if(!(glVertexAttrib4f = (void(GLApi*)(GLuint, GLfloat, GLfloat, GLfloat, GLfloat))wglGetProcAddress("glVertexAttrib4f"))) ++ n_failed_function_num;
	if(!(glVertexAttrib4fv = (void(GLApi*)(GLuint, const GLfloat*))wglGetProcAddress("glVertexAttrib4fv"))) ++ n_failed_function_num;
	if(!(glVertexAttrib4iv = (void(GLApi*)(GLuint, const GLint*))wglGetProcAddress("glVertexAttrib4iv"))) ++ n_failed_function_num;
	if(!(glVertexAttrib4s = (void(GLApi*)(GLuint, GLshort, GLshort, GLshort, GLshort))wglGetProcAddress("glVertexAttrib4s"))) ++ n_failed_function_num;
	if(!(glVertexAttrib4sv = (void(GLApi*)(GLuint, const GLshort*))wglGetProcAddress("glVertexAttrib4sv"))) ++ n_failed_function_num;
	if(!(glVertexAttrib4ubv = (void(GLApi*)(GLuint, const GLubyte*))wglGetProcAddress("glVertexAttrib4ubv"))) ++ n_failed_function_num;
	if(!(glVertexAttrib4uiv = (void(GLApi*)(GLuint, const GLuint*))wglGetProcAddress("glVertexAttrib4uiv"))) ++ n_failed_function_num;
	if(!(glVertexAttrib4usv = (void(GLApi*)(GLuint, const GLushort*))wglGetProcAddress("glVertexAttrib4usv"))) ++ n_failed_function_num;
	if(!(glVertexAttribPointer = (void(GLApi*)(GLuint, GLint, GLenum, GLboolean, GLsizei, const GLvoid*))wglGetProcAddress("glVertexAttribPointer"))) ++ n_failed_function_num;
	if(!(glEnableVertexAttribArray = (void(GLApi*)(GLuint index))wglGetProcAddress("glEnableVertexAttribArray"))) ++ n_failed_function_num;
	if(!(glDisableVertexAttribArray = (void(GLApi*)(GLuint index))wglGetProcAddress("glDisableVertexAttribArray"))) ++ n_failed_function_num;
	if(!(glGetVertexAttribdv = (void(GLApi*)(GLuint, GLenum, GLdouble*))wglGetProcAddress("glGetVertexAttribdv"))) ++ n_failed_function_num;
	if(!(glGetVertexAttribfv = (void(GLApi*)(GLuint, GLenum, GLfloat*))wglGetProcAddress("glGetVertexAttribfv"))) ++ n_failed_function_num;
	if(!(glGetVertexAttribiv = (void(GLApi*)(GLuint, GLenum, GLint*))wglGetProcAddress("glGetVertexAttribiv"))) ++ n_failed_function_num;
	if(!(glGetVertexAttribPointerv = (void(GLApi*)(GLuint, GLenum, GLvoid**))wglGetProcAddress("glGetVertexAttribPointerv"))) ++ n_failed_function_num;
	if(!(glIsShader = (GLboolean(GLApi*)(GLuint shader))wglGetProcAddress("glIsShader"))) ++ n_failed_function_num;
	if(!(glIsProgram = (GLboolean(GLApi*)(GLuint program))wglGetProcAddress("glIsProgram"))) ++ n_failed_function_num;
	if(!(glGetShaderiv = (void(GLApi*)(GLuint, GLenum, GLint*))wglGetProcAddress("glGetShaderiv"))) ++ n_failed_function_num;
	if(!(glGetProgramiv = (void(GLApi*)(GLuint, GLenum, GLint*))wglGetProcAddress("glGetProgramiv"))) ++ n_failed_function_num;
	if(!(glGetAttachedShaders = (void(GLApi*)(GLuint, GLsizei maxCount, GLsizei *count, GLuint*))wglGetProcAddress("glGetAttachedShaders"))) ++ n_failed_function_num;
	if(!(glGetShaderInfoLog = (void(GLApi*)(GLuint shader, GLsizei bufSize, GLsizei*, GLchar*))wglGetProcAddress("glGetShaderInfoLog"))) ++ n_failed_function_num;
	if(!(glGetProgramInfoLog = (void(GLApi*)(GLuint, GLsizei bufSize, GLsizei*, GLchar*))wglGetProcAddress("glGetProgramInfoLog"))) ++ n_failed_function_num;
	if(!(glGetUniformLocation = (GLint(GLApi*)(GLuint, const GLchar *name))wglGetProcAddress("glGetUniformLocation"))) ++ n_failed_function_num;
	if(!(glGetActiveUniform = (void(GLApi*)(GLuint, GLuint, GLsizei bufSize, GLsizei*, GLsizei*, GLenum*, GLchar*))wglGetProcAddress("glGetActiveUniform"))) ++ n_failed_function_num;
	if(!(glGetUniformfv = (void(GLApi*)(GLuint, GLint, GLfloat*))wglGetProcAddress("glGetUniformfv"))) ++ n_failed_function_num;
	if(!(glGetUniformiv = (void(GLApi*)(GLuint, GLint, GLint*))wglGetProcAddress("glGetUniformiv"))) ++ n_failed_function_num;
	if(!(glGetShaderSource = (void(GLApi*)(GLuint, GLsizei bufSize, GLsizei*, GLchar*))wglGetProcAddress("glGetShaderSource"))) ++ n_failed_function_num;
	if(!(glGetActiveAttrib = (void(GLApi*)(GLuint, GLuint, GLsizei bufSize, GLsizei*, GLsizei*, GLenum*, GLchar*))wglGetProcAddress("glGetActiveAttrib"))) ++ n_failed_function_num;
	if(!(glDrawBuffers = (void(GLApi*)(GLsizei, const GLenum*))wglGetProcAddress("glDrawBuffers"))) ++ n_failed_function_num;
	if(!(glStencilFuncSeparate = (void(GLApi*)(GLenum, GLenum, GLint, GLuint))wglGetProcAddress("glStencilFuncSeparate"))) ++ n_failed_function_num;
	if(!(glStencilOpSeparate = (void(GLApi*)(GLenum, GLenum, GLenum, GLenum))wglGetProcAddress("glStencilOpSeparate"))) ++ n_failed_function_num;
	if(!(glStencilMaskSeparate = (void(GLApi*)(GLenum, GLuint))wglGetProcAddress("glStencilMaskSeparate"))) ++ n_failed_function_num;
	if(!(glBlendEquationSeparate = (void(GLApi*)(GLenum, GLenum))wglGetProcAddress("glBlendEquationSeparate"))) ++ n_failed_function_num;

#endif // __GENERATE_GL_20__

	GLEH_OPENGL_2_0 = !n_failed_function_num && b_Support_OpenGL(2, 0);

	return n_failed_function_num;
}

/*
 *								--- OpenGL 2.1 ---
 */

#ifdef __GENERATE_GL_21__

void (GLApi *glUniformMatrix2x3fv)(GLint, GLsizei, GLboolean, const GLfloat*) INIT_FUNC_POINTERS;
void (GLApi *glUniformMatrix3x2fv)(GLint, GLsizei, GLboolean, const GLfloat*) INIT_FUNC_POINTERS;
void (GLApi *glUniformMatrix2x4fv)(GLint, GLsizei, GLboolean, const GLfloat*) INIT_FUNC_POINTERS;
void (GLApi *glUniformMatrix4x2fv)(GLint, GLsizei, GLboolean, const GLfloat*) INIT_FUNC_POINTERS;
void (GLApi *glUniformMatrix3x4fv)(GLint, GLsizei, GLboolean, const GLfloat*) INIT_FUNC_POINTERS;
void (GLApi *glUniformMatrix4x3fv)(GLint, GLsizei, GLboolean, const GLfloat*) INIT_FUNC_POINTERS;

#endif // __GENERATE_GL_21__

bool GLEH_OPENGL_2_1 = false;

int CGL3ExtensionHandler::n_GetGL21FuncPointers()
{
	int n_failed_function_num = 0;

#ifdef __GENERATE_GL_21__

	if(!(glUniformMatrix2x3fv = (void(GLApi*)(GLint, GLsizei, GLboolean, const GLfloat*))wglGetProcAddress("glUniformMatrix2x3fv"))) ++ n_failed_function_num;
	if(!(glUniformMatrix3x2fv = (void(GLApi*)(GLint, GLsizei, GLboolean, const GLfloat*))wglGetProcAddress("glUniformMatrix3x2fv"))) ++ n_failed_function_num;
	if(!(glUniformMatrix2x4fv = (void(GLApi*)(GLint, GLsizei, GLboolean, const GLfloat*))wglGetProcAddress("glUniformMatrix2x4fv"))) ++ n_failed_function_num;
	if(!(glUniformMatrix4x2fv = (void(GLApi*)(GLint, GLsizei, GLboolean, const GLfloat*))wglGetProcAddress("glUniformMatrix4x2fv"))) ++ n_failed_function_num;
	if(!(glUniformMatrix3x4fv = (void(GLApi*)(GLint, GLsizei, GLboolean, const GLfloat*))wglGetProcAddress("glUniformMatrix3x4fv"))) ++ n_failed_function_num;
	if(!(glUniformMatrix4x3fv = (void(GLApi*)(GLint, GLsizei, GLboolean, const GLfloat*))wglGetProcAddress("glUniformMatrix4x3fv"))) ++ n_failed_function_num;

#endif // __GENERATE_GL_21__

	GLEH_OPENGL_2_1 = !n_failed_function_num && b_Support_OpenGL(2, 1);

	return n_failed_function_num;
}

/*
 *								--- OpenGL 3.0 ---
 */

#ifdef __GENERATE_GL_30__

void (GLApi *glColorMaski)(GLuint, GLboolean, GLboolean, GLboolean, GLboolean) INIT_FUNC_POINTERS;
void (GLApi *glGetBooleani_v)(GLenum, GLuint, GLboolean *) INIT_FUNC_POINTERS;
void (GLApi *glGetIntegeri_v)(GLenum, GLuint, GLint *) INIT_FUNC_POINTERS;
void (GLApi *glEnablei)(GLenum, GLuint) INIT_FUNC_POINTERS;
void (GLApi *glDisablei)(GLenum, GLuint) INIT_FUNC_POINTERS;
GLboolean (GLApi *glIsEnabledi)(GLenum, GLuint) INIT_FUNC_POINTERS;
void (GLApi *glBeginTransformFeedback)(GLenum) INIT_FUNC_POINTERS;
void (GLApi *glEndTransformFeedback)(void) INIT_FUNC_POINTERS;
void (GLApi *glBindBufferRange)(GLenum, GLuint, GLuint, GLintptr, GLsizeiptr) INIT_FUNC_POINTERS;
void (GLApi *glBindBufferBase)(GLenum, GLuint, GLuint) INIT_FUNC_POINTERS;
void (GLApi *glTransformFeedbackVaryings)(GLuint, GLsizei, const GLchar* *, GLenum) INIT_FUNC_POINTERS;
void (GLApi *glGetTransformFeedbackVarying)(GLuint, GLuint, GLsizei, GLsizei *, GLsizei *, GLenum *, GLchar *) INIT_FUNC_POINTERS;
void (GLApi *glClampColor)(GLenum, GLenum) INIT_FUNC_POINTERS;
void (GLApi *glBeginConditionalRender)(GLuint, GLenum) INIT_FUNC_POINTERS;
void (GLApi *glEndConditionalRender)(void) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttribIPointer)(GLuint, GLint, GLenum, GLsizei, const GLvoid *) INIT_FUNC_POINTERS;
void (GLApi *glGetVertexAttribIiv)(GLuint, GLenum, GLint *) INIT_FUNC_POINTERS;
void (GLApi *glGetVertexAttribIuiv)(GLuint, GLenum, GLuint *) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttribI1i)(GLuint, GLint) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttribI2i)(GLuint, GLint, GLint) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttribI3i)(GLuint, GLint, GLint, GLint) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttribI4i)(GLuint, GLint, GLint, GLint, GLint) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttribI1ui)(GLuint, GLuint) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttribI2ui)(GLuint, GLuint, GLuint) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttribI3ui)(GLuint, GLuint, GLuint, GLuint) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttribI4ui)(GLuint, GLuint, GLuint, GLuint, GLuint) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttribI1iv)(GLuint, const GLint *) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttribI2iv)(GLuint, const GLint *) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttribI3iv)(GLuint, const GLint *) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttribI4iv)(GLuint, const GLint *) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttribI1uiv)(GLuint, const GLuint *) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttribI2uiv)(GLuint, const GLuint *) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttribI3uiv)(GLuint, const GLuint *) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttribI4uiv)(GLuint, const GLuint *) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttribI4bv)(GLuint, const GLbyte *) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttribI4sv)(GLuint, const GLshort *) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttribI4ubv)(GLuint, const GLubyte *) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttribI4usv)(GLuint, const GLushort *) INIT_FUNC_POINTERS;
void (GLApi *glGetUniformuiv)(GLuint, GLint, GLuint *) INIT_FUNC_POINTERS;
void (GLApi *glBindFragDataLocation)(GLuint, GLuint, const GLchar *) INIT_FUNC_POINTERS;
GLint (GLApi *glGetFragDataLocation)(GLuint, const GLchar *) INIT_FUNC_POINTERS;
void (GLApi *glUniform1ui)(GLint, GLuint) INIT_FUNC_POINTERS;
void (GLApi *glUniform2ui)(GLint, GLuint, GLuint) INIT_FUNC_POINTERS;
void (GLApi *glUniform3ui)(GLint, GLuint, GLuint, GLuint) INIT_FUNC_POINTERS;
void (GLApi *glUniform4ui)(GLint, GLuint, GLuint, GLuint, GLuint) INIT_FUNC_POINTERS;
void (GLApi *glUniform1uiv)(GLint, GLsizei, const GLuint *) INIT_FUNC_POINTERS;
void (GLApi *glUniform2uiv)(GLint, GLsizei, const GLuint *) INIT_FUNC_POINTERS;
void (GLApi *glUniform3uiv)(GLint, GLsizei, const GLuint *) INIT_FUNC_POINTERS;
void (GLApi *glUniform4uiv)(GLint, GLsizei, const GLuint *) INIT_FUNC_POINTERS;
void (GLApi *glTexParameterIiv)(GLenum, GLenum, const GLint *) INIT_FUNC_POINTERS;
void (GLApi *glTexParameterIuiv)(GLenum, GLenum, const GLuint *) INIT_FUNC_POINTERS;
void (GLApi *glGetTexParameterIiv)(GLenum, GLenum, GLint *) INIT_FUNC_POINTERS;
void (GLApi *glGetTexParameterIuiv)(GLenum, GLenum, GLuint *) INIT_FUNC_POINTERS;
void (GLApi *glClearBufferiv)(GLenum, GLint, const GLint *) INIT_FUNC_POINTERS;
void (GLApi *glClearBufferuiv)(GLenum, GLint, const GLuint *) INIT_FUNC_POINTERS;
void (GLApi *glClearBufferfv)(GLenum, GLint, const GLfloat *) INIT_FUNC_POINTERS;
void (GLApi *glClearBufferfi)(GLenum, GLint, GLfloat, GLint) INIT_FUNC_POINTERS;
const GLubyte *(GLApi *glGetStringi)(GLenum, GLuint) INIT_FUNC_POINTERS;

#endif // __GENERATE_GL_30__

bool GLEH_OPENGL_3_0 = false;

int CGL3ExtensionHandler::n_GetGL30FuncPointers()
{
	int n_failed_function_num = 0;

#ifdef __GENERATE_GL_30__

	if(!(glColorMaski = (void(GLApi*)(GLuint, GLboolean, GLboolean, GLboolean, GLboolean))wglGetProcAddress("glColorMaski"))) ++ n_failed_function_num;
	if(!(glGetBooleani_v = (void(GLApi*)(GLenum, GLuint, GLboolean*))wglGetProcAddress("glGetBooleani_v"))) ++ n_failed_function_num;
	if(!(glGetIntegeri_v = (void(GLApi*)(GLenum, GLuint, GLint*))wglGetProcAddress("glGetIntegeri_v"))) ++ n_failed_function_num;
	if(!(glEnablei = (void(GLApi*)(GLenum, GLuint))wglGetProcAddress("glEnablei"))) ++ n_failed_function_num;
	if(!(glDisablei = (void(GLApi*)(GLenum, GLuint))wglGetProcAddress("glDisablei"))) ++ n_failed_function_num;
	if(!(glIsEnabledi = (GLboolean(GLApi*)(GLenum, GLuint))wglGetProcAddress("glIsEnabledi"))) ++ n_failed_function_num;
	if(!(glBeginTransformFeedback = (void(GLApi*)(GLenum))wglGetProcAddress("glBeginTransformFeedback"))) ++ n_failed_function_num;
	if(!(glEndTransformFeedback = (void(GLApi*)(void))wglGetProcAddress("glEndTransformFeedback"))) ++ n_failed_function_num;
	if(!(glBindBufferRange = (void(GLApi*)(GLenum, GLuint, GLuint, GLintptr, GLsizeiptr))wglGetProcAddress("glBindBufferRange"))) ++ n_failed_function_num;
	if(!(glBindBufferBase = (void(GLApi*)(GLenum, GLuint, GLuint))wglGetProcAddress("glBindBufferBase"))) ++ n_failed_function_num;
	if(!(glTransformFeedbackVaryings = (void(GLApi*)(GLuint, GLsizei, const GLchar**, GLenum))wglGetProcAddress("glTransformFeedbackVaryings"))) ++ n_failed_function_num;
	if(!(glGetTransformFeedbackVarying = (void(GLApi*)(GLuint, GLuint, GLsizei, GLsizei*, GLsizei*, GLenum*, GLchar*))wglGetProcAddress("glGetTransformFeedbackVarying"))) ++ n_failed_function_num;
	if(!(glClampColor = (void(GLApi*)(GLenum, GLenum))wglGetProcAddress("glClampColor"))) ++ n_failed_function_num;
	if(!(glBeginConditionalRender = (void(GLApi*)(GLuint, GLenum))wglGetProcAddress("glBeginConditionalRender"))) ++ n_failed_function_num;
	if(!(glEndConditionalRender = (void(GLApi*)(void))wglGetProcAddress("glEndConditionalRender"))) ++ n_failed_function_num;
	if(!(glVertexAttribIPointer = (void(GLApi*)(GLuint, GLint, GLenum, GLsizei, const GLvoid*))wglGetProcAddress("glVertexAttribIPointer"))) ++ n_failed_function_num;
	if(!(glGetVertexAttribIiv = (void(GLApi*)(GLuint, GLenum, GLint*))wglGetProcAddress("glGetVertexAttribIiv"))) ++ n_failed_function_num;
	if(!(glGetVertexAttribIuiv = (void(GLApi*)(GLuint, GLenum, GLuint*))wglGetProcAddress("glGetVertexAttribIuiv"))) ++ n_failed_function_num;
	if(!(glVertexAttribI1i = (void(GLApi*)(GLuint, GLint))wglGetProcAddress("glVertexAttribI1i"))) ++ n_failed_function_num;
	if(!(glVertexAttribI2i = (void(GLApi*)(GLuint, GLint, GLint))wglGetProcAddress("glVertexAttribI2i"))) ++ n_failed_function_num;
	if(!(glVertexAttribI3i = (void(GLApi*)(GLuint, GLint, GLint, GLint))wglGetProcAddress("glVertexAttribI3i"))) ++ n_failed_function_num;
	if(!(glVertexAttribI4i = (void(GLApi*)(GLuint, GLint, GLint, GLint, GLint))wglGetProcAddress("glVertexAttribI4i"))) ++ n_failed_function_num;
	if(!(glVertexAttribI1ui = (void(GLApi*)(GLuint, GLuint))wglGetProcAddress("glVertexAttribI1ui"))) ++ n_failed_function_num;
	if(!(glVertexAttribI2ui = (void(GLApi*)(GLuint, GLuint, GLuint))wglGetProcAddress("glVertexAttribI2ui"))) ++ n_failed_function_num;
	if(!(glVertexAttribI3ui = (void(GLApi*)(GLuint, GLuint, GLuint, GLuint))wglGetProcAddress("glVertexAttribI3ui"))) ++ n_failed_function_num;
	if(!(glVertexAttribI4ui = (void(GLApi*)(GLuint, GLuint, GLuint, GLuint, GLuint))wglGetProcAddress("glVertexAttribI4ui"))) ++ n_failed_function_num;
	if(!(glVertexAttribI1iv = (void(GLApi*)(GLuint, const GLint*))wglGetProcAddress("glVertexAttribI1iv"))) ++ n_failed_function_num;
	if(!(glVertexAttribI2iv = (void(GLApi*)(GLuint, const GLint*))wglGetProcAddress("glVertexAttribI2iv"))) ++ n_failed_function_num;
	if(!(glVertexAttribI3iv = (void(GLApi*)(GLuint, const GLint*))wglGetProcAddress("glVertexAttribI3iv"))) ++ n_failed_function_num;
	if(!(glVertexAttribI4iv = (void(GLApi*)(GLuint, const GLint*))wglGetProcAddress("glVertexAttribI4iv"))) ++ n_failed_function_num;
	if(!(glVertexAttribI1uiv = (void(GLApi*)(GLuint, const GLuint*))wglGetProcAddress("glVertexAttribI1uiv"))) ++ n_failed_function_num;
	if(!(glVertexAttribI2uiv = (void(GLApi*)(GLuint, const GLuint*))wglGetProcAddress("glVertexAttribI2uiv"))) ++ n_failed_function_num;
	if(!(glVertexAttribI3uiv = (void(GLApi*)(GLuint, const GLuint*))wglGetProcAddress("glVertexAttribI3uiv"))) ++ n_failed_function_num;
	if(!(glVertexAttribI4uiv = (void(GLApi*)(GLuint, const GLuint*))wglGetProcAddress("glVertexAttribI4uiv"))) ++ n_failed_function_num;
	if(!(glVertexAttribI4bv = (void(GLApi*)(GLuint, const GLbyte*))wglGetProcAddress("glVertexAttribI4bv"))) ++ n_failed_function_num;
	if(!(glVertexAttribI4sv = (void(GLApi*)(GLuint, const GLshort*))wglGetProcAddress("glVertexAttribI4sv"))) ++ n_failed_function_num;
	if(!(glVertexAttribI4ubv = (void(GLApi*)(GLuint, const GLubyte*))wglGetProcAddress("glVertexAttribI4ubv"))) ++ n_failed_function_num;
	if(!(glVertexAttribI4usv = (void(GLApi*)(GLuint, const GLushort*))wglGetProcAddress("glVertexAttribI4usv"))) ++ n_failed_function_num;
	if(!(glGetUniformuiv = (void(GLApi*)(GLuint, GLint, GLuint*))wglGetProcAddress("glGetUniformuiv"))) ++ n_failed_function_num;
	if(!(glBindFragDataLocation = (void(GLApi*)(GLuint, GLuint, const GLchar*))wglGetProcAddress("glBindFragDataLocation"))) ++ n_failed_function_num;
	if(!(glGetFragDataLocation = (GLint(GLApi*)(GLuint, const GLchar*))wglGetProcAddress("glGetFragDataLocation"))) ++ n_failed_function_num;
	if(!(glUniform1ui = (void(GLApi*)(GLint, GLuint))wglGetProcAddress("glUniform1ui"))) ++ n_failed_function_num;
	if(!(glUniform2ui = (void(GLApi*)(GLint, GLuint, GLuint))wglGetProcAddress("glUniform2ui"))) ++ n_failed_function_num;
	if(!(glUniform3ui = (void(GLApi*)(GLint, GLuint, GLuint, GLuint))wglGetProcAddress("glUniform3ui"))) ++ n_failed_function_num;
	if(!(glUniform4ui = (void(GLApi*)(GLint, GLuint, GLuint, GLuint, GLuint))wglGetProcAddress("glUniform4ui"))) ++ n_failed_function_num;
	if(!(glUniform1uiv = (void(GLApi*)(GLint, GLsizei, const GLuint*))wglGetProcAddress("glUniform1uiv"))) ++ n_failed_function_num;
	if(!(glUniform2uiv = (void(GLApi*)(GLint, GLsizei, const GLuint*))wglGetProcAddress("glUniform2uiv"))) ++ n_failed_function_num;
	if(!(glUniform3uiv = (void(GLApi*)(GLint, GLsizei, const GLuint*))wglGetProcAddress("glUniform3uiv"))) ++ n_failed_function_num;
	if(!(glUniform4uiv = (void(GLApi*)(GLint, GLsizei, const GLuint*))wglGetProcAddress("glUniform4uiv"))) ++ n_failed_function_num;
	if(!(glTexParameterIiv = (void(GLApi*)(GLenum, GLenum, const GLint*))wglGetProcAddress("glTexParameterIiv"))) ++ n_failed_function_num;
	if(!(glTexParameterIuiv = (void(GLApi*)(GLenum, GLenum, const GLuint*))wglGetProcAddress("glTexParameterIuiv"))) ++ n_failed_function_num;
	if(!(glGetTexParameterIiv = (void(GLApi*)(GLenum, GLenum, GLint*))wglGetProcAddress("glGetTexParameterIiv"))) ++ n_failed_function_num;
	if(!(glGetTexParameterIuiv = (void(GLApi*)(GLenum, GLenum, GLuint*))wglGetProcAddress("glGetTexParameterIuiv"))) ++ n_failed_function_num;
	if(!(glClearBufferiv = (void(GLApi*)(GLenum, GLint, const GLint*))wglGetProcAddress("glClearBufferiv"))) ++ n_failed_function_num;
	if(!(glClearBufferuiv = (void(GLApi*)(GLenum, GLint, const GLuint*))wglGetProcAddress("glClearBufferuiv"))) ++ n_failed_function_num;
	if(!(glClearBufferfv = (void(GLApi*)(GLenum, GLint, const GLfloat*))wglGetProcAddress("glClearBufferfv"))) ++ n_failed_function_num;
	if(!(glClearBufferfi = (void(GLApi*)(GLenum, GLint, GLfloat, GLint))wglGetProcAddress("glClearBufferfi"))) ++ n_failed_function_num;
	if(!(glGetStringi = (const GLubyte*(GLApi*)(GLenum, GLuint))wglGetProcAddress("glGetStringi"))) ++ n_failed_function_num;

#endif // __GENERATE_GL_30__

	n_failed_function_num += n_GetARBFramebufferObjectFuncPointers() +
		n_GetARBMapBufferRangeFuncPointers() +
		n_GetARBVertexArrayObjectFuncPointers();

	GLEH_OPENGL_3_0 = !n_failed_function_num && b_Support_OpenGL(3, 0) &&
		b_SupportedExtension("GL_ARB_framebuffer_object") &&
		b_SupportedExtension("GL_ARB_map_buffer_range") &&
		b_SupportedExtension("GL_ARB_vertex_array_object");

	return n_failed_function_num;
}

/*
 *								--- OpenGL 3.1 ---
 */

#ifdef __GENERATE_GL_31__

void (GLApi *glTexBuffer)(GLenum, GLenum, GLuint) INIT_FUNC_POINTERS;
void (GLApi *glPrimitiveRestartIndex)(GLuint) INIT_FUNC_POINTERS;

#endif // __GENERATE_GL_31__

bool GLEH_OPENGL_3_1 = false;

int CGL3ExtensionHandler::n_GetGL31FuncPointers()
{
	int n_failed_function_num = 0;

#ifdef __GENERATE_GL_31__

	if(!(glTexBuffer = (void(GLApi*)(GLenum, GLenum, GLuint))wglGetProcAddress("glTexBuffer"))) ++ n_failed_function_num;
	if(!(glPrimitiveRestartIndex = (void(GLApi*)(GLuint))wglGetProcAddress("glPrimitiveRestartIndex"))) ++ n_failed_function_num;

#endif // __GENERATE_GL_31__

	n_failed_function_num += n_GetARBUniformBufferObjectFuncPointers() +
		n_GetARBDrawInstancedFuncPointers() +
		n_GetARBCopyBufferFuncPointers();

	GLEH_OPENGL_3_1 = !n_failed_function_num && b_Support_OpenGL(3, 1) &&
		b_SupportedExtension("GL_ARB_uniform_buffer_object") &&
		b_SupportedExtension("GL_ARB_draw_instanced") &&
		b_SupportedExtension("GL_ARB_copy_buffer");

	return n_failed_function_num;
}

/*
 *								--- OpenGL 3.2 ---
 */

#ifdef __GENERATE_GL_32__

void (GLApi *glGetInteger64i_v)(GLenum, GLuint, GLint64 *) INIT_FUNC_POINTERS;
void (GLApi *glGetBufferParameteri64v)(GLenum, GLenum, GLint64 *) INIT_FUNC_POINTERS;
void (GLApi *glProgramParameteri)(GLuint, GLenum, GLint) INIT_FUNC_POINTERS;
void (GLApi *glFramebufferTexture)(GLenum, GLenum, GLuint, GLint) INIT_FUNC_POINTERS;
//void (GLApi *glFramebufferTextureFace)(GLenum, GLenum, GLuint, GLint, GLenum) INIT_FUNC_POINTERS;

#endif // __GENERATE_GL_32__

bool GLEH_OPENGL_3_2 = false;

int CGL3ExtensionHandler::n_GetGL32FuncPointers()
{
	int n_failed_function_num = 0;

#ifdef __GENERATE_GL_32__

	if(!(glGetInteger64i_v = (void(GLApi*)(GLenum, GLuint, GLint64 *))wglGetProcAddress("glGetInteger64i_v"))) ++ n_failed_function_num;
	if(!(glGetBufferParameteri64v = (void(GLApi*)(GLenum, GLenum, GLint64 *))wglGetProcAddress("glGetBufferParameteri64v"))) ++ n_failed_function_num;
	if(!(glProgramParameteri = (void(GLApi*)(GLuint, GLenum, GLint))wglGetProcAddress("glProgramParameteri"))) ++ n_failed_function_num;
	if(!(glFramebufferTexture = (void(GLApi*)(GLenum, GLenum, GLuint, GLint))wglGetProcAddress("glFramebufferTexture"))) ++ n_failed_function_num;
	//if(!(glFramebufferTextureFace = (void(GLApi*)(GLenum, GLenum, GLuint, GLint, GLenum))wglGetProcAddress("glFramebufferTextureFace"))) ++ n_failed_function_num; // this one doesn't exist (a bug in the documentation)

#endif // __GENERATE_GL_32__

	n_failed_function_num += n_GetARBDrawElementsBaseVertexFuncPointers() +
		n_GetARBProvokingVertexFuncPointers() +
		n_GetARBSyncFuncPointers() +
		n_GetARBTextureMultisampleFuncPointers();

	GLEH_OPENGL_3_2 = !n_failed_function_num && b_Support_OpenGL(3, 2) &&
		b_SupportedExtension("GL_ARB_draw_elements_base_vertex") &&
		b_SupportedExtension("GL_ARB_provoking_vertex") &&
		b_SupportedExtension("GL_ARB_sync") &&
		b_SupportedExtension("GL_ARB_texture_multisample");

	return n_failed_function_num;
}

/*
 *								--- OpenGL 3.3 ---
 */

#ifdef __GENERATE_GL_33__

// no new tokens, except the exts

#endif // __GENERATE_GL_33__

bool GLEH_OPENGL_3_3 = false;

int CGL3ExtensionHandler::n_GetGL33FuncPointers()
{
	int n_failed_function_num = 0;

#ifdef __GENERATE_GL_33__

	// nothing

#endif // __GENERATE_GL_33__

	n_failed_function_num +=
		n_GetARBShaderBitEncodingFuncPointers() +
		n_GetARBBlendFuncExtendedFuncPointers() +
		n_GetARBExplicitAttribLocationFuncPointers() +
		n_GetARBOcclusionQuery2FuncPointers() +
		n_GetARBSamplerObjectsFuncPointers() +
		n_GetARBTextureRgb10A2uiFuncPointers() +
		n_GetARBTextureSwizzleFuncPointers() +
		n_GetARBTimerQueryFuncPointers() +
		n_GetARBInstancedArraysFuncPointers() +
		n_GetARBVertexType2101010RevFuncPointers();

	GLEH_OPENGL_3_3 = !n_failed_function_num && b_Support_OpenGL(3, 2) &&
		b_SupportedExtension("GL_ARB_shader_bit_encoding") &&
		b_SupportedExtension("GL_ARB_blend_func_extended") &&
		b_SupportedExtension("GL_ARB_explicit_attrib_location") &&
		b_SupportedExtension("GL_ARB_occlusion_query2") &&
		b_SupportedExtension("GL_ARB_sampler_objects") &&
		b_SupportedExtension("GL_ARB_texture_rgb10_a2ui") &&
		b_SupportedExtension("GL_ARB_texture_swizzle") &&
		b_SupportedExtension("GL_ARB_timer_query") &&
		b_SupportedExtension("GL_ARB_instanced_arrays") &&
		b_SupportedExtension("GL_ARB_vertex_type_2_10_10_10_rev");

	return n_failed_function_num;
}

/*
 *								--- GL_ARB_framebuffer_object ---
 */

#ifdef __GENERATE_GL_ARB_framebuffer_object__

GLboolean (GLApi *glIsRenderbuffer)(GLuint) INIT_FUNC_POINTERS;
void (GLApi *glBindRenderbuffer)(GLenum, GLuint) INIT_FUNC_POINTERS;
void (GLApi *glDeleteRenderbuffers)(GLsizei, const GLuint *) INIT_FUNC_POINTERS;
void (GLApi *glGenRenderbuffers)(GLsizei, GLuint *) INIT_FUNC_POINTERS;
void (GLApi *glRenderbufferStorage)(GLenum, GLenum, GLsizei, GLsizei) INIT_FUNC_POINTERS;
void (GLApi *glGetRenderbufferParameteriv)(GLenum, GLenum, GLint *) INIT_FUNC_POINTERS;
GLboolean (GLApi *glIsFramebuffer)(GLuint) INIT_FUNC_POINTERS;
void (GLApi *glBindFramebuffer)(GLenum, GLuint) INIT_FUNC_POINTERS;
void (GLApi *glDeleteFramebuffers)(GLsizei, const GLuint *) INIT_FUNC_POINTERS;
void (GLApi *glGenFramebuffers)(GLsizei, GLuint *) INIT_FUNC_POINTERS;
GLenum (GLApi *glCheckFramebufferStatus)(GLenum) INIT_FUNC_POINTERS;
void (GLApi *glFramebufferTexture1D)(GLenum, GLenum, GLenum, GLuint, GLint) INIT_FUNC_POINTERS;
void (GLApi *glFramebufferTexture2D)(GLenum, GLenum, GLenum, GLuint, GLint) INIT_FUNC_POINTERS;
void (GLApi *glFramebufferTexture3D)(GLenum, GLenum, GLenum, GLuint, GLint, GLint) INIT_FUNC_POINTERS;
void (GLApi *glFramebufferRenderbuffer)(GLenum, GLenum, GLenum, GLuint) INIT_FUNC_POINTERS;
void (GLApi *glGetFramebufferAttachmentParameteriv)(GLenum, GLenum, GLenum, GLint *) INIT_FUNC_POINTERS;
void (GLApi *glGenerateMipmap)(GLenum) INIT_FUNC_POINTERS;
void (GLApi *glBlitFramebuffer)(GLint, GLint, GLint, GLint, GLint, GLint, GLint, GLint, GLbitfield, GLenum) INIT_FUNC_POINTERS;
void (GLApi *glRenderbufferStorageMultisample)(GLenum, GLsizei, GLenum, GLsizei, GLsizei) INIT_FUNC_POINTERS;
void (GLApi *glFramebufferTextureLayer)(GLenum, GLenum, GLuint, GLint, GLint) INIT_FUNC_POINTERS;

#endif // __GENERATE_GL_ARB_framebuffer_object__

int CGL3ExtensionHandler::n_GetARBFramebufferObjectFuncPointers()
{
	int n_failed_function_num = 0;

#ifdef __GENERATE_GL_ARB_framebuffer_object__

	if(!(glIsRenderbuffer = (GLboolean(GLApi*)(GLuint))wglGetProcAddress("glIsRenderbuffer"))) ++ n_failed_function_num;
	if(!(glBindRenderbuffer = (void(GLApi*)(GLenum, GLuint))wglGetProcAddress("glBindRenderbuffer"))) ++ n_failed_function_num;
	if(!(glDeleteRenderbuffers = (void(GLApi*)(GLsizei, const GLuint*))wglGetProcAddress("glDeleteRenderbuffers"))) ++ n_failed_function_num;
	if(!(glGenRenderbuffers = (void(GLApi*)(GLsizei, GLuint *))wglGetProcAddress("glGenRenderbuffers"))) ++ n_failed_function_num;
	if(!(glRenderbufferStorage = (void(GLApi*)(GLenum, GLenum, GLsizei, GLsizei))wglGetProcAddress("glRenderbufferStorage"))) ++ n_failed_function_num;
	if(!(glGetRenderbufferParameteriv = (void(GLApi*)(GLenum, GLenum, GLint*))wglGetProcAddress("glGetRenderbufferParameteriv"))) ++ n_failed_function_num;
	if(!(glIsFramebuffer = (GLboolean(GLApi*)(GLuint))wglGetProcAddress("glIsFramebuffer"))) ++ n_failed_function_num;
	if(!(glBindFramebuffer = (void(GLApi*)(GLenum, GLuint))wglGetProcAddress("glBindFramebuffer"))) ++ n_failed_function_num;
	if(!(glDeleteFramebuffers = (void(GLApi*)(GLsizei, const GLuint*))wglGetProcAddress("glDeleteFramebuffers"))) ++ n_failed_function_num;
	if(!(glGenFramebuffers = (void(GLApi*)(GLsizei, GLuint*))wglGetProcAddress("glGenFramebuffers"))) ++ n_failed_function_num;
	if(!(glCheckFramebufferStatus = (GLenum(GLApi*)(GLenum))wglGetProcAddress("glCheckFramebufferStatus"))) ++ n_failed_function_num;
	if(!(glFramebufferTexture1D = (void(GLApi*)(GLenum, GLenum, GLenum, GLuint, GLint))wglGetProcAddress("glFramebufferTexture1D"))) ++ n_failed_function_num;
	if(!(glFramebufferTexture2D = (void(GLApi*)(GLenum, GLenum, GLenum, GLuint, GLint))wglGetProcAddress("glFramebufferTexture2D"))) ++ n_failed_function_num;
	if(!(glFramebufferTexture3D = (void(GLApi*)(GLenum, GLenum, GLenum, GLuint, GLint, GLint))wglGetProcAddress("glFramebufferTexture3D"))) ++ n_failed_function_num;
	if(!(glFramebufferRenderbuffer = (void(GLApi*)(GLenum, GLenum, GLenum, GLuint))wglGetProcAddress("glFramebufferRenderbuffer"))) ++ n_failed_function_num;
	if(!(glGetFramebufferAttachmentParameteriv = (void(GLApi*)(GLenum, GLenum, GLenum, GLint*))wglGetProcAddress("glGetFramebufferAttachmentParameteriv"))) ++ n_failed_function_num;
	if(!(glGenerateMipmap = (void(GLApi*)(GLenum))wglGetProcAddress("glGenerateMipmap"))) ++ n_failed_function_num;
	if(!(glBlitFramebuffer = (void(GLApi*)(GLint, GLint, GLint, GLint, GLint, GLint, GLint, GLint, GLbitfield, GLenum))wglGetProcAddress("glBlitFramebuffer"))) ++ n_failed_function_num;
	if(!(glRenderbufferStorageMultisample = (void(GLApi*)(GLenum, GLsizei, GLenum, GLsizei, GLsizei))wglGetProcAddress("glRenderbufferStorageMultisample"))) ++ n_failed_function_num;
	if(!(glFramebufferTextureLayer = (void(GLApi*)(GLenum, GLenum, GLuint, GLint, GLint))wglGetProcAddress("glFramebufferTextureLayer"))) ++ n_failed_function_num;

#endif // __GENERATE_GL_ARB_framebuffer_object__

	return n_failed_function_num;
}

/*
 *								--- GL_ARB_map_buffer_range ---
 */

#ifdef __GENERATE_GL_ARB_map_buffer_range__

GLvoid *(GLApi *glMapBufferRange)(GLenum, GLintptr, GLsizeiptr, GLbitfield) INIT_FUNC_POINTERS;
void (GLApi *glFlushMappedBufferRange)(GLenum, GLintptr, GLsizeiptr) INIT_FUNC_POINTERS;

#endif // __GENERATE_GL_ARB_map_buffer_range__

int CGL3ExtensionHandler::n_GetARBMapBufferRangeFuncPointers()
{
	int n_failed_function_num = 0;

#ifdef __GENERATE_GL_ARB_map_buffer_range__

	if(!(glMapBufferRange = (GLvoid*(GLApi*)(GLenum, GLintptr, GLsizeiptr, GLbitfield))wglGetProcAddress("glMapBufferRange"))) ++ n_failed_function_num;
	if(!(glFlushMappedBufferRange = (void(GLApi*)(GLenum, GLintptr, GLsizeiptr))wglGetProcAddress("glFlushMappedBufferRange"))) ++ n_failed_function_num;

#endif // __GENERATE_GL_ARB_map_buffer_range__

	return n_failed_function_num;
}

/*
 *								--- GL_ARB_vertex_array_object ---
 */

#ifdef __GENERATE_GL_ARB_vertex_array_object__

void (GLApi *glBindVertexArray)(GLuint) INIT_FUNC_POINTERS;
void (GLApi *glDeleteVertexArrays)(GLsizei, const GLuint *) INIT_FUNC_POINTERS;
void (GLApi *glGenVertexArrays)(GLsizei, GLuint *) INIT_FUNC_POINTERS;
GLboolean (GLApi *glIsVertexArray)(GLuint) INIT_FUNC_POINTERS;

#endif // __GENERATE_GL_ARB_vertex_array_object__

int CGL3ExtensionHandler::n_GetARBVertexArrayObjectFuncPointers()
{
	int n_failed_function_num = 0;

#ifdef __GENERATE_GL_ARB_vertex_array_object__

	if(!(glBindVertexArray = (void(GLApi*)(GLuint))wglGetProcAddress("glBindVertexArray"))) ++ n_failed_function_num;
	if(!(glDeleteVertexArrays = (void(GLApi*)(GLsizei, const GLuint *))wglGetProcAddress("glDeleteVertexArrays"))) ++ n_failed_function_num;
	if(!(glGenVertexArrays = (void(GLApi*)(GLsizei, GLuint *))wglGetProcAddress("glGenVertexArrays"))) ++ n_failed_function_num;
	if(!(glIsVertexArray = (GLboolean(GLApi*)(GLuint))wglGetProcAddress("glIsVertexArray"))) ++ n_failed_function_num;

#endif // __GENERATE_GL_ARB_vertex_array_object__

	return n_failed_function_num;
}

/*
 *								--- GL_ARB_uniform_buffer_object ---
 */

#ifdef __GENERATE_GL_ARB_uniform_buffer_object__

void (GLApi *glGetUniformIndices)(GLuint, GLsizei, const GLchar* *, GLuint *) INIT_FUNC_POINTERS;
void (GLApi *glGetActiveUniformsiv)(GLuint, GLsizei, const GLuint *, GLenum, GLint *) INIT_FUNC_POINTERS;
void (GLApi *glGetActiveUniformName)(GLuint, GLuint, GLsizei, GLsizei *, GLchar *) INIT_FUNC_POINTERS;
GLuint (GLApi *glGetUniformBlockIndex)(GLuint, const GLchar *) INIT_FUNC_POINTERS;
void (GLApi *glGetActiveUniformBlockiv)(GLuint, GLuint, GLenum, GLint *) INIT_FUNC_POINTERS;
void (GLApi *glGetActiveUniformBlockName)(GLuint, GLuint, GLsizei, GLsizei *, GLchar *) INIT_FUNC_POINTERS;
void (GLApi *glUniformBlockBinding)(GLuint, GLuint, GLuint) INIT_FUNC_POINTERS;

#endif // __GENERATE_GL_ARB_uniform_buffer_object__

int CGL3ExtensionHandler::n_GetARBUniformBufferObjectFuncPointers()
{
	int n_failed_function_num = 0;

#ifdef __GENERATE_GL_ARB_uniform_buffer_object__

	if(!(glGetUniformIndices = (void(GLApi*)(GLuint, GLsizei, const GLchar* *, GLuint *))wglGetProcAddress("glGetUniformIndices"))) ++ n_failed_function_num;
	if(!(glGetActiveUniformsiv = (void(GLApi*)(GLuint, GLsizei, const GLuint *, GLenum, GLint *))wglGetProcAddress("glGetActiveUniformsiv"))) ++ n_failed_function_num;
	if(!(glGetActiveUniformName = (void(GLApi*)(GLuint, GLuint, GLsizei, GLsizei *, GLchar *))wglGetProcAddress("glGetActiveUniformName"))) ++ n_failed_function_num;
	if(!(glGetUniformBlockIndex = (GLuint(GLApi*)(GLuint, const GLchar *))wglGetProcAddress("glGetUniformBlockIndex"))) ++ n_failed_function_num;
	if(!(glGetActiveUniformBlockiv = (void(GLApi*)(GLuint, GLuint, GLenum, GLint *))wglGetProcAddress("glGetActiveUniformBlockiv"))) ++ n_failed_function_num;
	if(!(glGetActiveUniformBlockName = (void(GLApi*)(GLuint, GLuint, GLsizei, GLsizei *, GLchar *))wglGetProcAddress("glGetActiveUniformBlockName"))) ++ n_failed_function_num;
	if(!(glUniformBlockBinding = (void(GLApi*)(GLuint, GLuint, GLuint))wglGetProcAddress("glUniformBlockBinding"))) ++ n_failed_function_num;

#endif // __GENERATE_GL_ARB_uniform_buffer_object__

	return n_failed_function_num;
}

/*
 *								--- GL_ARB_draw_instanced ---
 */

#ifdef __GENERATE_GL_ARB_draw_instanced__

void (GLApi *glDrawArraysInstanced)(GLenum, GLint, GLsizei, GLsizei) INIT_FUNC_POINTERS;
void (GLApi *glDrawElementsInstanced)(GLenum, GLsizei, GLenum, const GLvoid *, GLsizei) INIT_FUNC_POINTERS;

#endif // __GENERATE_GL_ARB_draw_instanced__

int CGL3ExtensionHandler::n_GetARBDrawInstancedFuncPointers()
{
	int n_failed_function_num = 0;

#ifdef __GENERATE_GL_ARB_draw_instanced__

	if(!(glDrawArraysInstanced = (void(GLApi*)(GLenum, GLint, GLsizei, GLsizei))wglGetProcAddress("glDrawArraysInstanced"))) ++ n_failed_function_num;
	if(!(glDrawElementsInstanced = (void(GLApi*)(GLenum, GLsizei, GLenum, const GLvoid *, GLsizei))wglGetProcAddress("glDrawElementsInstanced"))) ++ n_failed_function_num;

#endif // __GENERATE_GL_ARB_draw_instanced__

	return n_failed_function_num;
}

/*
 *								--- GL_ARB_copy_buffer ---
 */

#ifdef __GENERATE_GL_ARB_copy_buffer__

void (GLApi *glCopyBufferSubData)(GLenum, GLenum, GLintptr, GLintptr, GLsizeiptr) INIT_FUNC_POINTERS;

#endif // __GENERATE_GL_ARB_copy_buffer__

int CGL3ExtensionHandler::n_GetARBCopyBufferFuncPointers()
{
	int n_failed_function_num = 0;

#ifdef __GENERATE_GL_ARB_copy_buffer__

	if(!(glCopyBufferSubData = (void(GLApi*)(GLenum, GLenum, GLintptr, GLintptr, GLsizeiptr))wglGetProcAddress("glCopyBufferSubData"))) ++ n_failed_function_num;

#endif // __GENERATE_GL_ARB_copy_buffer__

	return n_failed_function_num;
}

/*
 *								--- GL_ARB_draw_elements_base_vertex ---
 */

#ifdef __GENERATE_GL_ARB_draw_elements_base_vertex__

void (GLApi *glDrawElementsBaseVertex)(GLenum, GLsizei, GLenum, const GLvoid *, GLint) INIT_FUNC_POINTERS;
void (GLApi *glDrawRangeElementsBaseVertex)(GLenum, GLuint, GLuint, GLsizei, GLenum, const GLvoid *, GLint) INIT_FUNC_POINTERS;
void (GLApi *glDrawElementsInstancedBaseVertex)(GLenum, GLsizei, GLenum, const GLvoid *, GLsizei, GLint) INIT_FUNC_POINTERS;
void (GLApi *glMultiDrawElementsBaseVertex)(GLenum, const GLsizei *, GLenum, const GLvoid* *, GLsizei, const GLint *) INIT_FUNC_POINTERS;

#endif // __GENERATE_GL_ARB_draw_elements_base_vertex__

int CGL3ExtensionHandler::n_GetARBDrawElementsBaseVertexFuncPointers()
{
	int n_failed_function_num = 0;

#ifdef __GENERATE_GL_ARB_draw_elements_base_vertex__

	if(!(glDrawElementsBaseVertex = (void(GLApi*)(GLenum, GLsizei, GLenum, const GLvoid *, GLint))wglGetProcAddress("glDrawElementsBaseVertex"))) ++ n_failed_function_num;
	if(!(glDrawRangeElementsBaseVertex = (void(GLApi*)(GLenum, GLuint, GLuint, GLsizei, GLenum, const GLvoid *, GLint))wglGetProcAddress("glDrawRangeElementsBaseVertex"))) ++ n_failed_function_num;
	if(!(glDrawElementsInstancedBaseVertex = (void(GLApi*)(GLenum, GLsizei, GLenum, const GLvoid *, GLsizei, GLint))wglGetProcAddress("glDrawElementsInstancedBaseVertex"))) ++ n_failed_function_num;
	if(!(glMultiDrawElementsBaseVertex = (void(GLApi*)(GLenum, const GLsizei *, GLenum, const GLvoid* *, GLsizei, const GLint *))wglGetProcAddress("glMultiDrawElementsBaseVertex"))) ++ n_failed_function_num;

#endif // __GENERATE_GL_ARB_draw_elements_base_vertex__

	return n_failed_function_num;
}

/*
 *								--- GL_ARB_provoking_vertex ---
 */

#ifdef __GENERATE_GL_ARB_provoking_vertex__

void (GLApi *glProvokingVertex)(GLenum) INIT_FUNC_POINTERS;

#endif // __GENERATE_GL_ARB_provoking_vertex__

int CGL3ExtensionHandler::n_GetARBProvokingVertexFuncPointers()
{
	int n_failed_function_num = 0;

#ifdef __GENERATE_GL_ARB_provoking_vertex__

	if(!(glProvokingVertex = (void(GLApi*)(GLenum))wglGetProcAddress("glProvokingVertex"))) ++ n_failed_function_num;

#endif // __GENERATE_GL_ARB_provoking_vertex__

	return n_failed_function_num;
}

/*
 *								--- GL_ARB_sync ---
 */

#ifdef __GENERATE_GL_ARB_sync__

GLsync (GLApi *glFenceSync)(GLenum, GLbitfield) INIT_FUNC_POINTERS;
GLboolean (GLApi *glIsSync)(GLsync) INIT_FUNC_POINTERS;
void (GLApi *glDeleteSync)(GLsync) INIT_FUNC_POINTERS;
GLenum (GLApi *glClientWaitSync)(GLsync, GLbitfield, GLuint64) INIT_FUNC_POINTERS;
void (GLApi *glWaitSync)(GLsync, GLbitfield, GLuint64) INIT_FUNC_POINTERS;
void (GLApi *glGetInteger64v)(GLenum, GLint64 *) INIT_FUNC_POINTERS;
void (GLApi *glGetSynciv)(GLsync, GLenum, GLsizei, GLsizei *, GLint *) INIT_FUNC_POINTERS;

#endif // __GENERATE_GL_ARB_sync__

int CGL3ExtensionHandler::n_GetARBSyncFuncPointers()
{
	int n_failed_function_num = 0;

#ifdef __GENERATE_GL_ARB_sync__

	if(!(glFenceSync = (GLsync(GLApi*)(GLenum, GLbitfield))wglGetProcAddress("glFenceSync"))) ++ n_failed_function_num;
	if(!(glIsSync = (GLboolean(GLApi*)(GLsync))wglGetProcAddress("glIsSync"))) ++ n_failed_function_num;
	if(!(glDeleteSync = (void(GLApi*)(GLsync))wglGetProcAddress("glDeleteSync"))) ++ n_failed_function_num;
	if(!(glClientWaitSync = (GLenum(GLApi*)(GLsync, GLbitfield, GLuint64))wglGetProcAddress("glClientWaitSync"))) ++ n_failed_function_num;
	if(!(glWaitSync = (void(GLApi*)(GLsync, GLbitfield, GLuint64))wglGetProcAddress("glWaitSync"))) ++ n_failed_function_num;
	if(!(glGetInteger64v = (void(GLApi*)(GLenum, GLint64 *))wglGetProcAddress("glGetInteger64v"))) ++ n_failed_function_num;
	if(!(glGetSynciv = (void(GLApi*)(GLsync, GLenum, GLsizei, GLsizei *, GLint *))wglGetProcAddress("glGetSynciv"))) ++ n_failed_function_num;

#endif // __GENERATE_GL_ARB_sync__

	return n_failed_function_num;
}

/*
 *								--- GL_ARB_texture_multisample ---
 */

#ifdef __GENERATE_GL_ARB_texture_multisample__

void (GLApi *glTexImage2DMultisample)(GLenum, GLsizei, GLint, GLsizei, GLsizei, GLboolean) INIT_FUNC_POINTERS;
void (GLApi *glTexImage3DMultisample)(GLenum, GLsizei, GLint, GLsizei, GLsizei, GLsizei, GLboolean) INIT_FUNC_POINTERS;
void (GLApi *glGetMultisamplefv)(GLenum, GLuint, GLfloat *) INIT_FUNC_POINTERS;
void (GLApi *glSampleMaski)(GLuint, GLbitfield) INIT_FUNC_POINTERS;

#endif // __GENERATE_GL_ARB_texture_multisample__

int CGL3ExtensionHandler::n_GetARBTextureMultisampleFuncPointers()
{
	int n_failed_function_num = 0;

#ifdef __GENERATE_GL_ARB_texture_multisample__

	if(!(glTexImage2DMultisample = (void(GLApi*)(GLenum, GLsizei, GLint, GLsizei, GLsizei, GLboolean))wglGetProcAddress("glTexImage2DMultisample"))) ++ n_failed_function_num;
	if(!(glTexImage3DMultisample = (void(GLApi*)(GLenum, GLsizei, GLint, GLsizei, GLsizei, GLsizei, GLboolean))wglGetProcAddress("glTexImage3DMultisample"))) ++ n_failed_function_num;
	if(!(glGetMultisamplefv = (void(GLApi*)(GLenum, GLuint, GLfloat *))wglGetProcAddress("glGetMultisamplefv"))) ++ n_failed_function_num;
	if(!(glSampleMaski = (void(GLApi*)(GLuint, GLbitfield))wglGetProcAddress("glSampleMaski"))) ++ n_failed_function_num;

#endif // __GENERATE_GL_ARB_texture_multisample__

	return n_failed_function_num;
}

/*
 *								--- GL_ARB_draw_buffers_blend ---
 */

#ifdef __GENERATE_GL_ARB_draw_buffers_blend__

void (GLApi *glBlendEquationi)(GLuint, GLenum) INIT_FUNC_POINTERS;
void (GLApi *glBlendEquationSeparatei)(GLuint, GLenum, GLenum) INIT_FUNC_POINTERS;
void (GLApi *glBlendFunci)(GLuint, GLenum, GLenum) INIT_FUNC_POINTERS;
void (GLApi *glBlendFuncSeparatei)(GLuint, GLenum, GLenum, GLenum, GLenum) INIT_FUNC_POINTERS;

#endif // __GENERATE_GL_ARB_draw_buffers_blend__

bool GLEH_ARB_draw_buffers_blend = false;

int CGL3ExtensionHandler::n_GetARBDrawBuffersBlendFuncPointers()
{
	int n_failed_function_num = 0;

#ifdef __GENERATE_GL_ARB_draw_buffers_blend__

	if(!(glBlendEquationi = (void(GLApi*)(GLuint, GLenum))wglGetProcAddress("glBlendEquationi"))) ++ n_failed_function_num;
	if(!(glBlendEquationSeparatei = (void(GLApi*)(GLuint, GLenum, GLenum))wglGetProcAddress("glBlendEquationSeparatei"))) ++ n_failed_function_num;
	if(!(glBlendFunci = (void(GLApi*)(GLuint, GLenum, GLenum))wglGetProcAddress("glBlendFunci"))) ++ n_failed_function_num;
	if(!(glBlendFuncSeparatei = (void(GLApi*)(GLuint, GLenum, GLenum, GLenum, GLenum))wglGetProcAddress("glBlendFuncSeparatei"))) ++ n_failed_function_num;

#endif // __GENERATE_GL_ARB_draw_buffers_blend__

	GLEH_ARB_draw_buffers_blend = !n_failed_function_num && b_SupportedExtension("GL_ARB_draw_buffers_blend");

	return n_failed_function_num;
}

/*
 *								--- GL_ARB_sample_shading ---
 */

#ifdef __GENERATE_GL_ARB_sample_shading__

void (GLApi *glMinSampleShading)(GLclampf) INIT_FUNC_POINTERS;

#endif // __GENERATE_GL_ARB_sample_shading__

bool GLEH_ARB_sample_shading = false;

int CGL3ExtensionHandler::n_GetARBSampleShadingFuncPointers()
{
	int n_failed_function_num = 0;

#ifdef __GENERATE_GL_ARB_sample_shading__

	if(!(glMinSampleShading = (void(GLApi*)(GLclampf))wglGetProcAddress("glMinSampleShading"))) ++ n_failed_function_num;

#endif // __GENERATE_GL_ARB_sample_shading__

	GLEH_ARB_sample_shading = !n_failed_function_num && b_SupportedExtension("GL_ARB_sample_shading");

	return n_failed_function_num;
}

/*
 *								--- GL_EXT_timer_query ---
 */

#ifdef __GENERATE_GL_EXT_timer_query__

void (GLApi *glGetQueryObjecti64vEXT)(GLuint id, GLenum pname, GLint64EXT *params) INIT_FUNC_POINTERS;
void (GLApi *glGetQueryObjectui64vEXT)(GLuint id, GLenum pname, GLuint64EXT *params) INIT_FUNC_POINTERS;

#endif // __GENERATE_GL_EXT_timer_query__

bool GLEH_EXT_timer_query = false;

int CGL3ExtensionHandler::n_GetEXTTimerQueryFuncPointers()
{
	int n_failed_function_num = 0;

#ifdef __GENERATE_GL_EXT_timer_query__

	if(!(glGetQueryObjecti64vEXT = (void(GLApi*)(GLuint id, GLenum pname, GLint64EXT *params))wglGetProcAddress("glGetQueryObjecti64vEXT"))) ++ n_failed_function_num;
	if(!(glGetQueryObjectui64vEXT = (void(GLApi*)(GLuint id, GLenum pname, GLuint64EXT *params))wglGetProcAddress("glGetQueryObjectui64vEXT"))) ++ n_failed_function_num;

#endif // __GENERATE_GL_EXT_timer_query__

	GLEH_EXT_timer_query = !n_failed_function_num && b_SupportedExtension("GL_EXT_timer_query");

	return n_failed_function_num;
}

/*
 *								--- GL_EXT_draw_buffers2 ---
 */

#ifdef __GENERATE_GL_EXT_draw_buffers2__

void (GLApi *glColorMaskIndexedEXT)(GLuint buf, GLboolean r, GLboolean g, GLboolean b, GLboolean a) INIT_FUNC_POINTERS;
void (GLApi *glGetGLbooleanIndexedvEXT)(GLenum value, GLuint index, GLboolean *data) INIT_FUNC_POINTERS;
void (GLApi *glGetIntegerIndexedvEXT)(GLenum value, GLuint index, GLint *data) INIT_FUNC_POINTERS;
void (GLApi *glEnableIndexedEXT)(GLenum target, GLuint index) INIT_FUNC_POINTERS;
void (GLApi *glDisableIndexedEXT)(GLenum target, GLuint index) INIT_FUNC_POINTERS;
GLboolean (GLApi *glIsEnabledIndexedEXT)(GLenum target, GLuint index) INIT_FUNC_POINTERS;

#endif // __GENERATE_GL_EXT_draw_buffers2__

bool GLEH_EXT_draw_buffers2 = false;

int CGL3ExtensionHandler::n_GetEXTDrawBuffers2FuncPointers()
{
	int n_failed_function_num = 0;

#ifdef __GENERATE_GL_EXT_draw_buffers2__

	if(!(glColorMaskIndexedEXT = (void(GLApi*)(GLuint buf, GLboolean r, GLboolean g, GLboolean b, GLboolean a))wglGetProcAddress("glColorMaskIndexedEXT"))) ++ n_failed_function_num;
	if(!(glGetGLbooleanIndexedvEXT = (void(GLApi*)(GLenum value, GLuint index, GLboolean *data))wglGetProcAddress("glGetGLbooleanIndexedvEXT"))) ++ n_failed_function_num;
	if(!(glGetIntegerIndexedvEXT = (void(GLApi*)(GLenum value, GLuint index, GLint *data))wglGetProcAddress("glGetIntegerIndexedvEXT"))) ++ n_failed_function_num;
	if(!(glEnableIndexedEXT = (void(GLApi*)(GLenum target, GLuint index))wglGetProcAddress("glEnableIndexedEXT"))) ++ n_failed_function_num;
	if(!(glDisableIndexedEXT = (void(GLApi*)(GLenum target, GLuint index))wglGetProcAddress("glDisableIndexedEXT"))) ++ n_failed_function_num;
	if(!(glIsEnabledIndexedEXT = (GLboolean(GLApi*)(GLenum target, GLuint index))wglGetProcAddress("glIsEnabledIndexedEXT"))) ++ n_failed_function_num;

#endif // __GENERATE_GL_EXT_draw_buffers2__

	GLEH_EXT_draw_buffers2 = !n_failed_function_num && b_SupportedExtension("GL_EXT_draw_buffers2");

	return n_failed_function_num;
}

/*
 *								--- GL_EXT_gpu_program_parameters ---
 */

#ifdef __GENERATE_GL_EXT_gpu_program_parameters__

void (GLApi *glProgramEnvParameters4fvEXT)(GLenum target, GLuint index, GLsizei count, const GLfloat *params) INIT_FUNC_POINTERS;
void (GLApi *glProgramLocalParameters4fvEXT)(GLenum target, GLuint index, GLsizei count, const GLfloat *params) INIT_FUNC_POINTERS;

#endif // __GENERATE_GL_EXT_gpu_program_parameters__

bool GLEH_EXT_gpu_program_parameters = false;

int CGL3ExtensionHandler::n_GetEXTGPUProgramParametersFuncPointers()
{
	int n_failed_function_num = 0;

#ifdef __GENERATE_GL_EXT_gpu_program_parameters__

	if(!(glProgramEnvParameters4fvEXT = (void(GLApi*)(GLenum target, GLuint index, GLsizei count, const GLfloat *params))wglGetProcAddress("glProgramEnvParameters4fvEXT"))) ++ n_failed_function_num;
	if(!(glProgramLocalParameters4fvEXT = (void(GLApi*)(GLenum target, GLuint index, GLsizei count, const GLfloat *params))wglGetProcAddress("glProgramLocalParameters4fvEXT"))) ++ n_failed_function_num;

#endif // __GENERATE_GL_EXT_gpu_program_parameters__

	GLEH_EXT_gpu_program_parameters = !n_failed_function_num && b_SupportedExtension("GL_EXT_gpu_program_parameters");

	return n_failed_function_num;
}

/*
 *								--- GL_EXT_bindable_uniform ---
 */

#ifdef __GENERATE_GL_EXT_bindable_uniform__

void (GLApi *glUniformBufferEXT)(GLuint program, GLint location, GLuint buffer) INIT_FUNC_POINTERS;
int (GLApi *glGetUniformBufferSizeEXT)(GLuint program, GLint location) INIT_FUNC_POINTERS;
GLintptr (GLApi *glGetUniformOffsetEXT)(GLuint program, GLint location) INIT_FUNC_POINTERS;

#endif // __GENERATE_GL_EXT_bindable_uniform__

bool GLEH_EXT_bindable_uniform = false;

int CGL3ExtensionHandler::n_GetEXTBindableUniformFuncPointers()
{
	int n_failed_function_num = 0;

#ifdef __GENERATE_GL_EXT_bindable_uniform__

	if(!(glUniformBufferEXT = (void(GLApi*)(GLuint program, GLint location, GLuint buffer))wglGetProcAddress("glUniformBufferEXT"))) ++ n_failed_function_num;
	if(!(glGetUniformBufferSizeEXT = (int(GLApi*)(GLuint program, GLint location))wglGetProcAddress("glGetUniformBufferSizeEXT"))) ++ n_failed_function_num;
	if(!(glGetUniformOffsetEXT = (GLintptr(GLApi*)(GLuint program, GLint location))wglGetProcAddress("glGetUniformOffsetEXT"))) ++ n_failed_function_num;

#endif // __GENERATE_GL_EXT_bindable_uniform__

	GLEH_EXT_bindable_uniform = !n_failed_function_num && b_SupportedExtension("GL_EXT_bindable_uniform");

	return n_failed_function_num;
}

/*
 *								--- GL_ARB_texture_cube_map_array ---
 */

bool GLEH_ARB_texture_cube_map_array = false;

int CGL3ExtensionHandler::n_GetARBTextureCubeMapArrayFuncPointers()
{
	GLEH_ARB_texture_cube_map_array = b_SupportedExtension("GL_ARB_texture_cube_map_array");
	// no function pointers

	return 0;
}

/*
 *								--- GL_ARB_texture_gather ---
 */

bool GLEH_ARB_texture_gather = false;

int CGL3ExtensionHandler::n_GetARBTextureGatherFuncPointers()
{
	GLEH_ARB_texture_gather = b_SupportedExtension("GL_ARB_texture_gather");
	// no function pointers

	return 0;
}

/*
 *								--- GL_EXT_texture_swizzle ---
 */

bool GLEH_EXT_texture_swizzle = false;

int CGL3ExtensionHandler::n_GetEXTTextureSwizzleFuncPointers()
{
	GLEH_EXT_texture_swizzle = b_SupportedExtension("GL_EXT_texture_swizzle");
	// no function pointers

	return 0;
}

/*
 *								--- GL_ARB_texture_query_lod ---
 */

bool GLEH_ARB_texture_query_lod = false;

int CGL3ExtensionHandler::n_GetARBTextureQueryLodFuncPointers()
{
	GLEH_ARB_texture_query_lod = b_SupportedExtension("GL_ARB_texture_query_lod");
	// no function pointers

	return 0;
}

/*
 *								--- GL_S3_s3tc ---
 */

bool GLEH_S3_s3tc = false;

int CGL3ExtensionHandler::n_GetS3S3tcFuncPointers()
{
	GLEH_S3_s3tc = b_SupportedExtension("GL_S3_s3tc");
	// no function pointers

	return 0;
}

/*
 *								--- GL_EXT_texture_compression_s3tc ---
 */

bool GLEH_EXT_texture_compression_s3tc = false;

int CGL3ExtensionHandler::n_GetEXTTextureCompressionS3tcFuncPointers()
{
	GLEH_EXT_texture_compression_s3tc = b_SupportedExtension("GL_EXT_texture_compression_s3tc");
	// no function pointers

	return 0;
}

/*
 *								--- GL_EXT_texture_compression_latc ---
 */

bool GLEH_EXT_texture_compression_latc = false;

int CGL3ExtensionHandler::n_GetEXTTextureCompressionLatcFuncPointers()
{
	GLEH_EXT_texture_compression_latc = b_SupportedExtension("GL_EXT_texture_compression_latc");
	// no function pointers

	return 0;
}

/*
 *								--- GL_EXT_texture_filter_anisotropic ---
 */

bool GLEH_EXT_texture_filter_anisotropic = false;

int CGL3ExtensionHandler::n_GetEXTTextureFilterAnisotropicFuncPointers()
{
	GLEH_EXT_texture_filter_anisotropic = b_SupportedExtension("GL_EXT_texture_filter_anisotropic");
	// no function pointers

	return 0;
}

/*
 *								--- GL_EXT_abgr ---
 */

bool GLEH_EXT_abgr = false;

int CGL3ExtensionHandler::n_GetEXTABGRFuncPointers()
{
	GLEH_EXT_abgr = b_SupportedExtension("GL_EXT_abgr");
	// no function pointers

	return 0;
}

/*
 *								--- GL_ARB_instanced_arrays ---
 */

bool GLEH_ARB_instanced_arrays = false;

#ifdef __GENERATE_GL_ARB_instanced_arrays

void (GLApi *glVertexAttribDivisorARB)(GLuint, GLuint) INIT_FUNC_POINTERS;

#endif // __GENERATE_GL_ARB_instanced_arrays

int CGL3ExtensionHandler::n_GetARBInstancedArraysFuncPointers()
{
	int n_failed_function_num = 0;

#ifdef __GENERATE_GL_ARB_instanced_arrays

	if(!(glVertexAttribDivisorARB = (void(GLApi*)(GLuint, GLuint))wglGetProcAddress("glVertexAttribDivisorARB"))) ++ n_failed_function_num;

#endif // __GENERATE_GL_ARB_instanced_arrays

	GLEH_ARB_instanced_arrays = !n_failed_function_num && b_SupportedExtension("GL_ARB_instanced_arrays");

	return n_failed_function_num;
}

/*
 *								--- ~GL_ARB_instanced_arrays ---
 */

/*
 *								--- GL_ARB_shader_texture_lod ---
 */

bool GLEH_ARB_shader_texture_lod = false;

int CGL3ExtensionHandler::n_GetARBShaderTextureLodFuncPointers()
{
	int n_failed_function_num = 0;

	// no new functions or procedures here

	GLEH_ARB_shader_texture_lod = !n_failed_function_num && b_SupportedExtension("GL_ARB_shader_texture_lod");

	return n_failed_function_num;
}

/*
 *								--- ~GL_ARB_shader_texture_lod ---
 */

/*
 *								--- GL_ARB_texture_buffer_object ---
 */

bool GLEH_ARB_texture_buffer_object = false;

#ifdef __GENERATE_GL_ARB_texture_buffer_object

void (GLApi *glTexBufferARB)(GLenum, GLenum, GLuint) INIT_FUNC_POINTERS;

#endif // __GENERATE_GL_ARB_texture_buffer_object

int CGL3ExtensionHandler::n_GetARBTextureBufferObjectFuncPointers()
{
	int n_failed_function_num = 0;

#ifdef __GENERATE_GL_ARB_texture_buffer_object

	if(!(glTexBufferARB = (void(GLApi*)(GLenum, GLenum, GLuint))wglGetProcAddress("glTexBufferARB"))) ++ n_failed_function_num;

#endif // __GENERATE_GL_ARB_texture_buffer_object

	GLEH_ARB_texture_buffer_object = !n_failed_function_num && b_SupportedExtension("GL_ARB_texture_buffer_object");

	return n_failed_function_num;
}

/*
 *								--- ~GL_ARB_texture_buffer_object ---
 */

/*
 *								--- GL_ARB_shading_language_include ---
 */

bool GLEH_ARB_shading_language_include = false;

#ifdef __GENERATE_GL_ARB_shading_language_include

void (GLApi *glNamedStringARB)(GLenum, int, const GLchar*, int, const GLchar*) INIT_FUNC_POINTERS;
void (GLApi *glDeleteNamedStringARB)(int, const GLchar*) INIT_FUNC_POINTERS;
void (GLApi *glCompileShaderIncludeARB)(GLuint, GLsizei, const GLchar**, const int*) INIT_FUNC_POINTERS;
GLboolean (GLApi *glIsNamedStringARB)(int, const GLchar*) INIT_FUNC_POINTERS;
void (GLApi *glGetNamedStringARB)(int, const GLchar*, GLsizei, int*, GLchar*) INIT_FUNC_POINTERS;
void (GLApi *glGetNamedStringivARB)(int, const GLchar*, GLenum, int*) INIT_FUNC_POINTERS;

#endif // __GENERATE_GL_ARB_shading_language_include

int CGL3ExtensionHandler::n_GetARBShadingLanguageIncludeFuncPointers()
{
	int n_failed_function_num = 0;

#ifdef __GENERATE_GL_ARB_shading_language_include

	if(!(glNamedStringARB = (void(GLApi*)(GLenum, int, const GLchar*, int, const GLchar*))wglGetProcAddress("glNamedStringARB"))) ++ n_failed_function_num;
	if(!(glDeleteNamedStringARB = (void(GLApi*)(int, const GLchar*))wglGetProcAddress("glDeleteNamedStringARB"))) ++ n_failed_function_num;
	if(!(glCompileShaderIncludeARB = (void(GLApi*)(GLuint, GLsizei, const GLchar**, const int*))wglGetProcAddress("glCompileShaderIncludeARB"))) ++ n_failed_function_num;
	if(!(glIsNamedStringARB = (GLboolean(GLApi*)(int, const GLchar*))wglGetProcAddress("glIsNamedStringARB"))) ++ n_failed_function_num;
	if(!(glGetNamedStringARB = (void(GLApi*)(int, const GLchar*, GLsizei, int*, GLchar*))wglGetProcAddress("glGetNamedStringARB"))) ++ n_failed_function_num;
	if(!(glGetNamedStringivARB = (void(GLApi*)(int, const GLchar*, GLenum, int*))wglGetProcAddress("glGetNamedStringivARB"))) ++ n_failed_function_num;

#endif // __GENERATE_GL_ARB_shading_language_include

	GLEH_ARB_shading_language_include = !n_failed_function_num && b_SupportedExtension("GL_ARB_shading_language_include");

	return n_failed_function_num;
}

/*
 *								--- ~GL_ARB_shading_language_include ---
 */

/*
 *								--- GL_ARB_texture_compression_bptc ---
 */

bool GLEH_ARB_texture_compression_bptc = false;

int CGL3ExtensionHandler::n_GetARBTextureCompressionBptcFuncPointers()
{
	int n_failed_function_num = 0;

	// no new functions or procedures here

	GLEH_ARB_texture_compression_bptc = !n_failed_function_num && b_SupportedExtension("GL_ARB_texture_compression_bptc");

	return n_failed_function_num;
}

/*
 *								--- ~GL_ARB_texture_compression_bptc ---
 */

/*
 *								--- GL_ARB_blend_func_extended ---
 */

bool GLEH_ARB_blend_func_extended = false;

#ifdef __GENERATE_GL_ARB_blend_func_extended

void (GLApi *glBindFragDataLocationIndexed)(GLuint, GLuint, GLuint, const GLchar*) INIT_FUNC_POINTERS;
int (GLApi *glGetFragDataIndex)(GLuint, const GLchar*) INIT_FUNC_POINTERS;

#endif // __GENERATE_GL_ARB_blend_func_extended

int CGL3ExtensionHandler::n_GetARBBlendFuncExtendedFuncPointers()
{
	int n_failed_function_num = 0;

#ifdef __GENERATE_GL_ARB_blend_func_extended

	if(!(glBindFragDataLocationIndexed = (void(GLApi*)(GLuint, GLuint, GLuint, const GLchar*))wglGetProcAddress("glBindFragDataLocationIndexed"))) ++ n_failed_function_num;
	if(!(glGetFragDataIndex = (int(GLApi*)(GLuint, const GLchar*))wglGetProcAddress("glGetFragDataIndex"))) ++ n_failed_function_num;

#endif // __GENERATE_GL_ARB_blend_func_extended

	GLEH_ARB_blend_func_extended = !n_failed_function_num && b_SupportedExtension("GL_ARB_blend_func_extended");

	return n_failed_function_num;
}

/*
 *								--- ~GL_ARB_blend_func_extended ---
 */

/*
 *								--- GL_ARB_explicit_attrib_location ---
 */

bool GLEH_ARB_explicit_attrib_location = false;

int CGL3ExtensionHandler::n_GetARBExplicitAttribLocationFuncPointers()
{
	int n_failed_function_num = 0;

	// no new functions or procedures here

	GLEH_ARB_explicit_attrib_location = !n_failed_function_num && b_SupportedExtension("GL_ARB_explicit_attrib_location");

	return n_failed_function_num;
}

/*
 *								--- ~GL_ARB_explicit_attrib_location ---
 */

/*
 *								--- GL_ARB_occlusion_query2 ---
 */

bool GLEH_ARB_occlusion_query2 = false;

int CGL3ExtensionHandler::n_GetARBOcclusionQuery2FuncPointers()
{
	int n_failed_function_num = 0;

	// no new functions or procedures here

	GLEH_ARB_occlusion_query2 = !n_failed_function_num && b_SupportedExtension("GL_ARB_occlusion_query2");

	return n_failed_function_num;
}

/*
 *								--- ~GL_ARB_occlusion_query2 ---
 */

/*
 *								--- GL_ARB_sampler_objects ---
 */

bool GLEH_ARB_sampler_objects = false;

#ifdef __GENERATE_GL_ARB_sampler_objects

void (GLApi *glGenSamplers)(GLsizei, GLuint*) INIT_FUNC_POINTERS;
void (GLApi *glDeleteSamplers)(GLsizei, const GLuint*) INIT_FUNC_POINTERS;
GLboolean (GLApi *glIsSampler)(GLuint) INIT_FUNC_POINTERS;
void (GLApi *glBindSampler)(GLuint, GLuint) INIT_FUNC_POINTERS;
void (GLApi *glSamplerParameteri)(GLuint, GLenum, int) INIT_FUNC_POINTERS;
void (GLApi *glSamplerParameterf)(GLuint, GLenum, float) INIT_FUNC_POINTERS;
void (GLApi *glSamplerParameteriv)(GLuint, GLenum, const int*) INIT_FUNC_POINTERS;
void (GLApi *glSamplerParameterfv)(GLuint, GLenum, const float*) INIT_FUNC_POINTERS;
void (GLApi *glSamplerParameterIiv)(GLuint, GLenum, const int*) INIT_FUNC_POINTERS;
void (GLApi *glSamplerParameterIuiv)(GLuint, GLenum, const GLuint*) INIT_FUNC_POINTERS;
void (GLApi *glGetSamplerParameteriv)(GLuint, GLenum, int*) INIT_FUNC_POINTERS;
void (GLApi *glGetSamplerParameterfv)(GLuint, GLenum, float*) INIT_FUNC_POINTERS;
void (GLApi *glGetSamplerParameterIiv)(GLuint, GLenum, int*) INIT_FUNC_POINTERS;
void (GLApi *glGetSamplerParameterIuiv)(GLuint, GLenum, GLuint*) INIT_FUNC_POINTERS;

#endif // __GENERATE_GL_ARB_sampler_objects

int CGL3ExtensionHandler::n_GetARBSamplerObjectsFuncPointers()
{
	int n_failed_function_num = 0;

#ifdef __GENERATE_GL_ARB_sampler_objects

	if(!(glGenSamplers = (void(GLApi*)(GLsizei, GLuint*))wglGetProcAddress("glGenSamplers"))) ++ n_failed_function_num;
	if(!(glDeleteSamplers = (void(GLApi*)(GLsizei, const GLuint*))wglGetProcAddress("glDeleteSamplers"))) ++ n_failed_function_num;
	if(!(glIsSampler = (GLboolean(GLApi*)(GLuint))wglGetProcAddress("glIsSampler"))) ++ n_failed_function_num;
	if(!(glBindSampler = (void(GLApi*)(GLuint, GLuint))wglGetProcAddress("glBindSampler"))) ++ n_failed_function_num;
	if(!(glSamplerParameteri = (void(GLApi*)(GLuint, GLenum, int))wglGetProcAddress("glSamplerParameteri"))) ++ n_failed_function_num;
	if(!(glSamplerParameterf = (void(GLApi*)(GLuint, GLenum, float))wglGetProcAddress("glSamplerParameterf"))) ++ n_failed_function_num;
	if(!(glSamplerParameteriv = (void(GLApi*)(GLuint, GLenum, const int*))wglGetProcAddress("glSamplerParameteriv"))) ++ n_failed_function_num;
	if(!(glSamplerParameterfv = (void(GLApi*)(GLuint, GLenum, const float*))wglGetProcAddress("glSamplerParameterfv"))) ++ n_failed_function_num;
	if(!(glSamplerParameterIiv = (void(GLApi*)(GLuint, GLenum, const int*))wglGetProcAddress("glSamplerParameterIiv"))) ++ n_failed_function_num;
	if(!(glSamplerParameterIuiv = (void(GLApi*)(GLuint, GLenum, const GLuint*))wglGetProcAddress("glSamplerParameterIuiv"))) ++ n_failed_function_num;
	if(!(glGetSamplerParameteriv = (void(GLApi*)(GLuint, GLenum, int*))wglGetProcAddress("glGetSamplerParameteriv"))) ++ n_failed_function_num;
	if(!(glGetSamplerParameterfv = (void(GLApi*)(GLuint, GLenum, float*))wglGetProcAddress("glGetSamplerParameterfv"))) ++ n_failed_function_num;
	if(!(glGetSamplerParameterIiv = (void(GLApi*)(GLuint, GLenum, int*))wglGetProcAddress("glGetSamplerParameterIiv"))) ++ n_failed_function_num;
	if(!(glGetSamplerParameterIuiv = (void(GLApi*)(GLuint, GLenum, GLuint*))wglGetProcAddress("glGetSamplerParameterIuiv"))) ++ n_failed_function_num;

#endif // __GENERATE_GL_ARB_sampler_objects

	GLEH_ARB_sampler_objects = !n_failed_function_num && b_SupportedExtension("GL_ARB_sampler_objects");

	return n_failed_function_num;
}

/*
 *								--- ~GL_ARB_sampler_objects ---
 */

/*
 *								--- GL_ARB_shader_bit_encoding ---
 */

bool GLEH_ARB_shader_bit_encoding = false;

int CGL3ExtensionHandler::n_GetARBShaderBitEncodingFuncPointers()
{
	int n_failed_function_num = 0;

	// no new functions or procedures here

	GLEH_ARB_shader_bit_encoding = !n_failed_function_num && b_SupportedExtension("GL_ARB_shader_bit_encoding");

	return n_failed_function_num;
}

/*
 *								--- ~GL_ARB_shader_bit_encoding ---
 */

/*
 *								--- GL_ARB_texture_rgb10_a2ui ---
 */

bool GLEH_ARB_texture_rgb10_a2ui = false;

int CGL3ExtensionHandler::n_GetARBTextureRgb10A2uiFuncPointers()
{
	int n_failed_function_num = 0;

	// no new functions or procedures here

	GLEH_ARB_texture_rgb10_a2ui = !n_failed_function_num && b_SupportedExtension("GL_ARB_texture_rgb10_a2ui");

	return n_failed_function_num;
}

/*
 *								--- ~GL_ARB_texture_rgb10_a2ui ---
 */

/*
 *								--- GL_ARB_texture_swizzle ---
 */

bool GLEH_ARB_texture_swizzle = false;

int CGL3ExtensionHandler::n_GetARBTextureSwizzleFuncPointers()
{
	int n_failed_function_num = 0;

	// no new functions or procedures here

	GLEH_ARB_texture_swizzle = !n_failed_function_num && b_SupportedExtension("GL_ARB_texture_swizzle");

	return n_failed_function_num;
}

/*
 *								--- ~GL_ARB_texture_swizzle ---
 */

/*
 *								--- GL_ARB_timer_query ---
 */

bool GLEH_ARB_timer_query = false;

#ifdef __GENERATE_GL_ARB_timer_query

void (GLApi *glQueryCounter)(GLuint, GLenum) INIT_FUNC_POINTERS;
void (GLApi *glGetQueryObjecti64v)(GLuint, GLenum, GLint64*) INIT_FUNC_POINTERS;
void (GLApi *glGetQueryObjectui64v)(GLuint, GLenum, GLuint64*) INIT_FUNC_POINTERS;

#endif // __GENERATE_GL_ARB_timer_query

int CGL3ExtensionHandler::n_GetARBTimerQueryFuncPointers()
{
	int n_failed_function_num = 0;

#ifdef __GENERATE_GL_ARB_timer_query

	if(!(glQueryCounter = (void(GLApi*)(GLuint, GLenum))wglGetProcAddress("glQueryCounter"))) ++ n_failed_function_num;
	if(!(glGetQueryObjecti64v = (void(GLApi*)(GLuint, GLenum, GLint64*))wglGetProcAddress("glGetQueryObjecti64v"))) ++ n_failed_function_num;
	if(!(glGetQueryObjectui64v = (void(GLApi*)(GLuint, GLenum, GLuint64*))wglGetProcAddress("glGetQueryObjectui64v"))) ++ n_failed_function_num;

#endif // __GENERATE_GL_ARB_timer_query

	GLEH_ARB_timer_query = !n_failed_function_num && b_SupportedExtension("GL_ARB_timer_query");

	return n_failed_function_num;
}

/*
 *								--- ~GL_ARB_timer_query ---
 */

/*
 *								--- GL_ARB_vertex_type_2_10_10_10_rev ---
 */

bool GLEH_ARB_vertex_type_2_10_10_10_rev = false;

#ifdef __GENERATE_GL_ARB_vertex_type_2_10_10_10_rev

void (GLApi *glVertexP2ui)(GLenum, GLuint) INIT_FUNC_POINTERS;
void (GLApi *glVertexP3ui)(GLenum, GLuint) INIT_FUNC_POINTERS;
void (GLApi *glVertexP4ui)(GLenum, GLuint) INIT_FUNC_POINTERS;
void (GLApi *glVertexP2uiv)(GLenum, const GLuint*) INIT_FUNC_POINTERS;
void (GLApi *glVertexP3uiv)(GLenum, const GLuint*) INIT_FUNC_POINTERS;
void (GLApi *glVertexP4uiv)(GLenum, const GLuint*) INIT_FUNC_POINTERS;
void (GLApi *glTexCoordP1ui)(GLenum, GLuint) INIT_FUNC_POINTERS;
void (GLApi *glTexCoordP2ui)(GLenum, GLuint) INIT_FUNC_POINTERS;
void (GLApi *glTexCoordP3ui)(GLenum, GLuint) INIT_FUNC_POINTERS;
void (GLApi *glTexCoordP4ui)(GLenum, GLuint) INIT_FUNC_POINTERS;
void (GLApi *glTexCoordP1uiv)(GLenum, const GLuint*) INIT_FUNC_POINTERS;
void (GLApi *glTexCoordP2uiv)(GLenum, const GLuint*) INIT_FUNC_POINTERS;
void (GLApi *glTexCoordP3uiv)(GLenum, const GLuint*) INIT_FUNC_POINTERS;
void (GLApi *glTexCoordP4uiv)(GLenum, const GLuint*) INIT_FUNC_POINTERS;
void (GLApi *glMultiTexCoordP1ui)(GLenum, GLenum, GLuint) INIT_FUNC_POINTERS;
void (GLApi *glMultiTexCoordP2ui)(GLenum, GLenum, GLuint) INIT_FUNC_POINTERS;
void (GLApi *glMultiTexCoordP3ui)(GLenum, GLenum, GLuint) INIT_FUNC_POINTERS;
void (GLApi *glMultiTexCoordP4ui)(GLenum, GLenum, GLuint) INIT_FUNC_POINTERS;
void (GLApi *glMultiTexCoordP1uiv)(GLenum, GLenum, const GLuint*) INIT_FUNC_POINTERS;
void (GLApi *glMultiTexCoordP2uiv)(GLenum, GLenum, const GLuint*) INIT_FUNC_POINTERS;
void (GLApi *glMultiTexCoordP3uiv)(GLenum, GLenum, const GLuint*) INIT_FUNC_POINTERS;
void (GLApi *glMultiTexCoordP4uiv)(GLenum, GLenum, const GLuint*) INIT_FUNC_POINTERS;
void (GLApi *glNormalP3ui)(GLenum, GLuint) INIT_FUNC_POINTERS;
void (GLApi *glNormalP3uiv)(GLenum, const GLuint*) INIT_FUNC_POINTERS;
void (GLApi *glColorP3ui)(GLenum, GLuint) INIT_FUNC_POINTERS;
void (GLApi *glColorP4ui)(GLenum, GLuint) INIT_FUNC_POINTERS;
void (GLApi *glColorP3uiv)(GLenum, const GLuint*) INIT_FUNC_POINTERS;
void (GLApi *glColorP4uiv)(GLenum, const GLuint*) INIT_FUNC_POINTERS;
void (GLApi *glSecondaryColorP3ui)(GLenum, GLuint) INIT_FUNC_POINTERS;
void (GLApi *glSecondaryColorP3uiv)(GLenum, const GLuint*) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttribP1ui)(GLuint, GLenum, GLboolean, GLuint) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttribP2ui)(GLuint, GLenum, GLboolean, GLuint) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttribP3ui)(GLuint, GLenum, GLboolean, GLuint) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttribP4ui)(GLuint, GLenum, GLboolean, GLuint) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttribP1uiv)(GLuint, GLenum, GLboolean, const GLuint*) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttribP2uiv)(GLuint, GLenum, GLboolean, const GLuint*) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttribP3uiv)(GLuint, GLenum, GLboolean, const GLuint*) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttribP4uiv)(GLuint, GLenum, GLboolean, const GLuint*) INIT_FUNC_POINTERS;

#endif // __GENERATE_GL_ARB_vertex_type_2_10_10_10_rev

int CGL3ExtensionHandler::n_GetARBVertexType2101010RevFuncPointers()
{
	int n_failed_function_num = 0;

#ifdef __GENERATE_GL_ARB_vertex_type_2_10_10_10_rev

	if(!(glVertexP2ui = (void(GLApi*)(GLenum, GLuint))wglGetProcAddress("glVertexP2ui"))) ++ n_failed_function_num;
	if(!(glVertexP3ui = (void(GLApi*)(GLenum, GLuint))wglGetProcAddress("glVertexP3ui"))) ++ n_failed_function_num;
	if(!(glVertexP4ui = (void(GLApi*)(GLenum, GLuint))wglGetProcAddress("glVertexP4ui"))) ++ n_failed_function_num;
	if(!(glVertexP2uiv = (void(GLApi*)(GLenum, const GLuint*))wglGetProcAddress("glVertexP2uiv"))) ++ n_failed_function_num;
	if(!(glVertexP3uiv = (void(GLApi*)(GLenum, const GLuint*))wglGetProcAddress("glVertexP3uiv"))) ++ n_failed_function_num;
	if(!(glVertexP4uiv = (void(GLApi*)(GLenum, const GLuint*))wglGetProcAddress("glVertexP4uiv"))) ++ n_failed_function_num;
	if(!(glTexCoordP1ui = (void(GLApi*)(GLenum, GLuint))wglGetProcAddress("glTexCoordP1ui"))) ++ n_failed_function_num;
	if(!(glTexCoordP2ui = (void(GLApi*)(GLenum, GLuint))wglGetProcAddress("glTexCoordP2ui"))) ++ n_failed_function_num;
	if(!(glTexCoordP3ui = (void(GLApi*)(GLenum, GLuint))wglGetProcAddress("glTexCoordP3ui"))) ++ n_failed_function_num;
	if(!(glTexCoordP4ui = (void(GLApi*)(GLenum, GLuint))wglGetProcAddress("glTexCoordP4ui"))) ++ n_failed_function_num;
	if(!(glTexCoordP1uiv = (void(GLApi*)(GLenum, const GLuint*))wglGetProcAddress("glTexCoordP1uiv"))) ++ n_failed_function_num;
	if(!(glTexCoordP2uiv = (void(GLApi*)(GLenum, const GLuint*))wglGetProcAddress("glTexCoordP2uiv"))) ++ n_failed_function_num;
	if(!(glTexCoordP3uiv = (void(GLApi*)(GLenum, const GLuint*))wglGetProcAddress("glTexCoordP3uiv"))) ++ n_failed_function_num;
	if(!(glTexCoordP4uiv = (void(GLApi*)(GLenum, const GLuint*))wglGetProcAddress("glTexCoordP4uiv"))) ++ n_failed_function_num;
	if(!(glMultiTexCoordP1ui = (void(GLApi*)(GLenum, GLenum, GLuint))wglGetProcAddress("glMultiTexCoordP1ui"))) ++ n_failed_function_num;
	if(!(glMultiTexCoordP2ui = (void(GLApi*)(GLenum, GLenum, GLuint))wglGetProcAddress("glMultiTexCoordP2ui"))) ++ n_failed_function_num;
	if(!(glMultiTexCoordP3ui = (void(GLApi*)(GLenum, GLenum, GLuint))wglGetProcAddress("glMultiTexCoordP3ui"))) ++ n_failed_function_num;
	if(!(glMultiTexCoordP4ui = (void(GLApi*)(GLenum, GLenum, GLuint))wglGetProcAddress("glMultiTexCoordP4ui"))) ++ n_failed_function_num;
	if(!(glMultiTexCoordP1uiv = (void(GLApi*)(GLenum, GLenum, const GLuint*))wglGetProcAddress("glMultiTexCoordP1uiv"))) ++ n_failed_function_num;
	if(!(glMultiTexCoordP2uiv = (void(GLApi*)(GLenum, GLenum, const GLuint*))wglGetProcAddress("glMultiTexCoordP2uiv"))) ++ n_failed_function_num;
	if(!(glMultiTexCoordP3uiv = (void(GLApi*)(GLenum, GLenum, const GLuint*))wglGetProcAddress("glMultiTexCoordP3uiv"))) ++ n_failed_function_num;
	if(!(glMultiTexCoordP4uiv = (void(GLApi*)(GLenum, GLenum, const GLuint*))wglGetProcAddress("glMultiTexCoordP4uiv"))) ++ n_failed_function_num;
	if(!(glNormalP3ui = (void(GLApi*)(GLenum, GLuint))wglGetProcAddress("glNormalP3ui"))) ++ n_failed_function_num;
	if(!(glNormalP3uiv = (void(GLApi*)(GLenum, const GLuint*))wglGetProcAddress("glNormalP3uiv"))) ++ n_failed_function_num;
	if(!(glColorP3ui = (void(GLApi*)(GLenum, GLuint))wglGetProcAddress("glColorP3ui"))) ++ n_failed_function_num;
	if(!(glColorP4ui = (void(GLApi*)(GLenum, GLuint))wglGetProcAddress("glColorP4ui"))) ++ n_failed_function_num;
	if(!(glColorP3uiv = (void(GLApi*)(GLenum, const GLuint*))wglGetProcAddress("glColorP3uiv"))) ++ n_failed_function_num;
	if(!(glColorP4uiv = (void(GLApi*)(GLenum, const GLuint*))wglGetProcAddress("glColorP4uiv"))) ++ n_failed_function_num;
	if(!(glSecondaryColorP3ui = (void(GLApi*)(GLenum, GLuint))wglGetProcAddress("glSecondaryColorP3ui"))) ++ n_failed_function_num;
	if(!(glSecondaryColorP3uiv = (void(GLApi*)(GLenum, const GLuint*))wglGetProcAddress("glSecondaryColorP3uiv"))) ++ n_failed_function_num;
	if(!(glVertexAttribP1ui = (void(GLApi*)(GLuint, GLenum, GLboolean, GLuint))wglGetProcAddress("glVertexAttribP1ui"))) ++ n_failed_function_num;
	if(!(glVertexAttribP2ui = (void(GLApi*)(GLuint, GLenum, GLboolean, GLuint))wglGetProcAddress("glVertexAttribP2ui"))) ++ n_failed_function_num;
	if(!(glVertexAttribP3ui = (void(GLApi*)(GLuint, GLenum, GLboolean, GLuint))wglGetProcAddress("glVertexAttribP3ui"))) ++ n_failed_function_num;
	if(!(glVertexAttribP4ui = (void(GLApi*)(GLuint, GLenum, GLboolean, GLuint))wglGetProcAddress("glVertexAttribP4ui"))) ++ n_failed_function_num;
	if(!(glVertexAttribP1uiv = (void(GLApi*)(GLuint, GLenum, GLboolean, const GLuint*))wglGetProcAddress("glVertexAttribP1uiv"))) ++ n_failed_function_num;
	if(!(glVertexAttribP2uiv = (void(GLApi*)(GLuint, GLenum, GLboolean, const GLuint*))wglGetProcAddress("glVertexAttribP2uiv"))) ++ n_failed_function_num;
	if(!(glVertexAttribP3uiv = (void(GLApi*)(GLuint, GLenum, GLboolean, const GLuint*))wglGetProcAddress("glVertexAttribP3uiv"))) ++ n_failed_function_num;
	if(!(glVertexAttribP4uiv = (void(GLApi*)(GLuint, GLenum, GLboolean, const GLuint*))wglGetProcAddress("glVertexAttribP4uiv"))) ++ n_failed_function_num;

#endif // __GENERATE_GL_ARB_vertex_type_2_10_10_10_rev

	GLEH_ARB_vertex_type_2_10_10_10_rev = !n_failed_function_num && b_SupportedExtension("GL_ARB_vertex_type_2_10_10_10_rev");

	return n_failed_function_num;
}

/*
 *								--- ~GL_ARB_vertex_type_2_10_10_10_rev ---
 */

/*
 *								--- GL_ARB_draw_indirect ---
 */

bool GLEH_ARB_draw_indirect = false;

#ifdef __GENERATE_GL_ARB_draw_indirect

void (GLApi *glDrawArraysIndirect)(GLenum, const void*) INIT_FUNC_POINTERS;
void (GLApi *glDrawElementsIndirect)(GLenum, GLenum, const void*) INIT_FUNC_POINTERS;

#endif // __GENERATE_GL_ARB_draw_indirect

int CGL3ExtensionHandler::n_GetARBDrawIndirectFuncPointers()
{
	int n_failed_function_num = 0;

#ifdef __GENERATE_GL_ARB_draw_indirect

	if(!(glDrawArraysIndirect = (void(GLApi*)(GLenum, const void*))wglGetProcAddress("glDrawArraysIndirect"))) ++ n_failed_function_num;
	if(!(glDrawElementsIndirect = (void(GLApi*)(GLenum, GLenum, const void*))wglGetProcAddress("glDrawElementsIndirect"))) ++ n_failed_function_num;

#endif // __GENERATE_GL_ARB_draw_indirect

	GLEH_ARB_draw_indirect = !n_failed_function_num && b_SupportedExtension("GL_ARB_draw_indirect");

	return n_failed_function_num;
}

/*
 *								--- ~GL_ARB_draw_indirect ---
 */

/*
 *								--- GL_ARB_gpu_shader5 ---
 */

bool GLEH_ARB_gpu_shader5 = false;

int CGL3ExtensionHandler::n_GetARBGpuShader5FuncPointers()
{
	int n_failed_function_num = 0;

	// no new functions or procedures here

	GLEH_ARB_gpu_shader5 = !n_failed_function_num && b_SupportedExtension("GL_ARB_gpu_shader5");

	return n_failed_function_num;
}

/*
 *								--- ~GL_ARB_gpu_shader5 ---
 */

/*
 *								--- GL_ARB_gpu_shader_fp64 ---
 */

bool GLEH_ARB_gpu_shader_fp64 = false;

#ifdef __GENERATE_GL_ARB_gpu_shader_fp64

void (GLApi *glUniform1d)(int, double) INIT_FUNC_POINTERS;
void (GLApi *glUniform2d)(int, double, double) INIT_FUNC_POINTERS;
void (GLApi *glUniform3d)(int, double, double, double) INIT_FUNC_POINTERS;
void (GLApi *glUniform4d)(int, double, double, double, double) INIT_FUNC_POINTERS;
void (GLApi *glUniform1dv)(int, GLsizei, const double*) INIT_FUNC_POINTERS;
void (GLApi *glUniform2dv)(int, GLsizei, const double*) INIT_FUNC_POINTERS;
void (GLApi *glUniform3dv)(int, GLsizei, const double*) INIT_FUNC_POINTERS;
void (GLApi *glUniform4dv)(int, GLsizei, const double*) INIT_FUNC_POINTERS;
void (GLApi *glUniformMatrix2dv)(int, GLsizei, GLboolean, const double*) INIT_FUNC_POINTERS;
void (GLApi *glUniformMatrix3dv)(int, GLsizei, GLboolean, const double*) INIT_FUNC_POINTERS;
void (GLApi *glUniformMatrix4dv)(int, GLsizei, GLboolean, const double*) INIT_FUNC_POINTERS;
void (GLApi *glUniformMatrix2x3dv)(int, GLsizei, GLboolean, const double*) INIT_FUNC_POINTERS;
void (GLApi *glUniformMatrix2x4dv)(int, GLsizei, GLboolean, const double*) INIT_FUNC_POINTERS;
void (GLApi *glUniformMatrix3x2dv)(int, GLsizei, GLboolean, const double*) INIT_FUNC_POINTERS;
void (GLApi *glUniformMatrix3x4dv)(int, GLsizei, GLboolean, const double*) INIT_FUNC_POINTERS;
void (GLApi *glUniformMatrix4x2dv)(int, GLsizei, GLboolean, const double*) INIT_FUNC_POINTERS;
void (GLApi *glUniformMatrix4x3dv)(int, GLsizei, GLboolean, const double*) INIT_FUNC_POINTERS;
void (GLApi *glGetUniformdv)(GLuint, int, double*) INIT_FUNC_POINTERS;
void (GLApi *glProgramUniform1dEXT)(GLuint, int, double) INIT_FUNC_POINTERS;
void (GLApi *glProgramUniform2dEXT)(GLuint, int, double, double) INIT_FUNC_POINTERS;
void (GLApi *glProgramUniform3dEXT)(GLuint, int, double, double, double) INIT_FUNC_POINTERS;
void (GLApi *glProgramUniform4dEXT)(GLuint, int, double, double, double, double) INIT_FUNC_POINTERS;
void (GLApi *glProgramUniform1dvEXT)(GLuint, int, GLsizei, const double*) INIT_FUNC_POINTERS;
void (GLApi *glProgramUniform2dvEXT)(GLuint, int, GLsizei, const double*) INIT_FUNC_POINTERS;
void (GLApi *glProgramUniform3dvEXT)(GLuint, int, GLsizei, const double*) INIT_FUNC_POINTERS;
void (GLApi *glProgramUniform4dvEXT)(GLuint, int, GLsizei, const double*) INIT_FUNC_POINTERS;
void (GLApi *glProgramUniformMatrix2dvEXT)(GLuint, int, GLsizei, GLboolean, const double*) INIT_FUNC_POINTERS;
void (GLApi *glProgramUniformMatrix3dvEXT)(GLuint, int, GLsizei, GLboolean, const double*) INIT_FUNC_POINTERS;
void (GLApi *glProgramUniformMatrix4dvEXT)(GLuint, int, GLsizei, GLboolean, const double*) INIT_FUNC_POINTERS;
void (GLApi *glProgramUniformMatrix2x3dvEXT)(GLuint, int, GLsizei, GLboolean, const double*) INIT_FUNC_POINTERS;
void (GLApi *glProgramUniformMatrix2x4dvEXT)(GLuint, int, GLsizei, GLboolean, const double*) INIT_FUNC_POINTERS;
void (GLApi *glProgramUniformMatrix3x2dvEXT)(GLuint, int, GLsizei, GLboolean, const double*) INIT_FUNC_POINTERS;
void (GLApi *glProgramUniformMatrix3x4dvEXT)(GLuint, int, GLsizei, GLboolean, const double*) INIT_FUNC_POINTERS;
void (GLApi *glProgramUniformMatrix4x2dvEXT)(GLuint, int, GLsizei, GLboolean, const double*) INIT_FUNC_POINTERS;
void (GLApi *glProgramUniformMatrix4x3dvEXT)(GLuint, int, GLsizei, GLboolean, const double*) INIT_FUNC_POINTERS;

#endif // __GENERATE_GL_ARB_gpu_shader_fp64

int CGL3ExtensionHandler::n_GetARBGpuShaderFp64FuncPointers()
{
	int n_failed_function_num = 0;

#ifdef __GENERATE_GL_ARB_gpu_shader_fp64

	if(!(glUniform1d = (void(GLApi*)(int, double))wglGetProcAddress("glUniform1d"))) ++ n_failed_function_num;
	if(!(glUniform2d = (void(GLApi*)(int, double, double))wglGetProcAddress("glUniform2d"))) ++ n_failed_function_num;
	if(!(glUniform3d = (void(GLApi*)(int, double, double, double))wglGetProcAddress("glUniform3d"))) ++ n_failed_function_num;
	if(!(glUniform4d = (void(GLApi*)(int, double, double, double, double))wglGetProcAddress("glUniform4d"))) ++ n_failed_function_num;
	if(!(glUniform1dv = (void(GLApi*)(int, GLsizei, const double*))wglGetProcAddress("glUniform1dv"))) ++ n_failed_function_num;
	if(!(glUniform2dv = (void(GLApi*)(int, GLsizei, const double*))wglGetProcAddress("glUniform2dv"))) ++ n_failed_function_num;
	if(!(glUniform3dv = (void(GLApi*)(int, GLsizei, const double*))wglGetProcAddress("glUniform3dv"))) ++ n_failed_function_num;
	if(!(glUniform4dv = (void(GLApi*)(int, GLsizei, const double*))wglGetProcAddress("glUniform4dv"))) ++ n_failed_function_num;
	if(!(glUniformMatrix2dv = (void(GLApi*)(int, GLsizei, GLboolean, const double*))wglGetProcAddress("glUniformMatrix2dv"))) ++ n_failed_function_num;
	if(!(glUniformMatrix3dv = (void(GLApi*)(int, GLsizei, GLboolean, const double*))wglGetProcAddress("glUniformMatrix3dv"))) ++ n_failed_function_num;
	if(!(glUniformMatrix4dv = (void(GLApi*)(int, GLsizei, GLboolean, const double*))wglGetProcAddress("glUniformMatrix4dv"))) ++ n_failed_function_num;
	if(!(glUniformMatrix2x3dv = (void(GLApi*)(int, GLsizei, GLboolean, const double*))wglGetProcAddress("glUniformMatrix2x3dv"))) ++ n_failed_function_num;
	if(!(glUniformMatrix2x4dv = (void(GLApi*)(int, GLsizei, GLboolean, const double*))wglGetProcAddress("glUniformMatrix2x4dv"))) ++ n_failed_function_num;
	if(!(glUniformMatrix3x2dv = (void(GLApi*)(int, GLsizei, GLboolean, const double*))wglGetProcAddress("glUniformMatrix3x2dv"))) ++ n_failed_function_num;
	if(!(glUniformMatrix3x4dv = (void(GLApi*)(int, GLsizei, GLboolean, const double*))wglGetProcAddress("glUniformMatrix3x4dv"))) ++ n_failed_function_num;
	if(!(glUniformMatrix4x2dv = (void(GLApi*)(int, GLsizei, GLboolean, const double*))wglGetProcAddress("glUniformMatrix4x2dv"))) ++ n_failed_function_num;
	if(!(glUniformMatrix4x3dv = (void(GLApi*)(int, GLsizei, GLboolean, const double*))wglGetProcAddress("glUniformMatrix4x3dv"))) ++ n_failed_function_num;
	if(!(glGetUniformdv = (void(GLApi*)(GLuint, int, double*))wglGetProcAddress("glGetUniformdv"))) ++ n_failed_function_num;
	if(!(glProgramUniform1dEXT = (void(GLApi*)(GLuint, int, double))wglGetProcAddress("glProgramUniform1dEXT"))) ++ n_failed_function_num;
	if(!(glProgramUniform2dEXT = (void(GLApi*)(GLuint, int, double, double))wglGetProcAddress("glProgramUniform2dEXT"))) ++ n_failed_function_num;
	if(!(glProgramUniform3dEXT = (void(GLApi*)(GLuint, int, double, double, double))wglGetProcAddress("glProgramUniform3dEXT"))) ++ n_failed_function_num;
	if(!(glProgramUniform4dEXT = (void(GLApi*)(GLuint, int, double, double, double, double))wglGetProcAddress("glProgramUniform4dEXT"))) ++ n_failed_function_num;
	if(!(glProgramUniform1dvEXT = (void(GLApi*)(GLuint, int, GLsizei, const double*))wglGetProcAddress("glProgramUniform1dvEXT"))) ++ n_failed_function_num;
	if(!(glProgramUniform2dvEXT = (void(GLApi*)(GLuint, int, GLsizei, const double*))wglGetProcAddress("glProgramUniform2dvEXT"))) ++ n_failed_function_num;
	if(!(glProgramUniform3dvEXT = (void(GLApi*)(GLuint, int, GLsizei, const double*))wglGetProcAddress("glProgramUniform3dvEXT"))) ++ n_failed_function_num;
	if(!(glProgramUniform4dvEXT = (void(GLApi*)(GLuint, int, GLsizei, const double*))wglGetProcAddress("glProgramUniform4dvEXT"))) ++ n_failed_function_num;
	if(!(glProgramUniformMatrix2dvEXT = (void(GLApi*)(GLuint, int, GLsizei, GLboolean, const double*))wglGetProcAddress("glProgramUniformMatrix2dvEXT"))) ++ n_failed_function_num;
	if(!(glProgramUniformMatrix3dvEXT = (void(GLApi*)(GLuint, int, GLsizei, GLboolean, const double*))wglGetProcAddress("glProgramUniformMatrix3dvEXT"))) ++ n_failed_function_num;
	if(!(glProgramUniformMatrix4dvEXT = (void(GLApi*)(GLuint, int, GLsizei, GLboolean, const double*))wglGetProcAddress("glProgramUniformMatrix4dvEXT"))) ++ n_failed_function_num;
	if(!(glProgramUniformMatrix2x3dvEXT = (void(GLApi*)(GLuint, int, GLsizei, GLboolean, const double*))wglGetProcAddress("glProgramUniformMatrix2x3dvEXT"))) ++ n_failed_function_num;
	if(!(glProgramUniformMatrix2x4dvEXT = (void(GLApi*)(GLuint, int, GLsizei, GLboolean, const double*))wglGetProcAddress("glProgramUniformMatrix2x4dvEXT"))) ++ n_failed_function_num;
	if(!(glProgramUniformMatrix3x2dvEXT = (void(GLApi*)(GLuint, int, GLsizei, GLboolean, const double*))wglGetProcAddress("glProgramUniformMatrix3x2dvEXT"))) ++ n_failed_function_num;
	if(!(glProgramUniformMatrix3x4dvEXT = (void(GLApi*)(GLuint, int, GLsizei, GLboolean, const double*))wglGetProcAddress("glProgramUniformMatrix3x4dvEXT"))) ++ n_failed_function_num;
	if(!(glProgramUniformMatrix4x2dvEXT = (void(GLApi*)(GLuint, int, GLsizei, GLboolean, const double*))wglGetProcAddress("glProgramUniformMatrix4x2dvEXT"))) ++ n_failed_function_num;
	if(!(glProgramUniformMatrix4x3dvEXT = (void(GLApi*)(GLuint, int, GLsizei, GLboolean, const double*))wglGetProcAddress("glProgramUniformMatrix4x3dvEXT"))) ++ n_failed_function_num;

#endif // __GENERATE_GL_ARB_gpu_shader_fp64

	GLEH_ARB_gpu_shader_fp64 = !n_failed_function_num && b_SupportedExtension("GL_ARB_gpu_shader_fp64");

	return n_failed_function_num;
}

/*
 *								--- ~GL_ARB_gpu_shader_fp64 ---
 */

/*
 *								--- GL_ARB_shader_subroutine ---
 */

bool GLEH_ARB_shader_subroutine = false;

#ifdef __GENERATE_GL_ARB_shader_subroutine

int (GLApi *glGetSubroutineUniformLocation)(GLuint, GLenum, const GLchar*) INIT_FUNC_POINTERS;
GLuint (GLApi *glGetSubroutineIndex)(GLuint, GLenum, const GLchar*) INIT_FUNC_POINTERS;
void (GLApi *glGetActiveSubroutineUniformiv)(GLuint, GLenum, GLuint, GLenum, int*) INIT_FUNC_POINTERS;
void (GLApi *glGetActiveSubroutineUniformName)(GLuint, GLenum, GLuint, GLsizei, GLsizei*, GLchar*) INIT_FUNC_POINTERS;
void (GLApi *glGetActiveSubroutineName)(GLuint, GLenum, GLuint, GLsizei, GLsizei*, GLchar*) INIT_FUNC_POINTERS;
void (GLApi *glUniformSubroutinesuiv)(GLenum, GLsizei, const GLuint*) INIT_FUNC_POINTERS;
void (GLApi *glGetUniformSubroutineuiv)(GLenum, int, GLuint*) INIT_FUNC_POINTERS;
void (GLApi *glGetProgramStageiv)(GLuint, GLenum, GLenum, int*) INIT_FUNC_POINTERS;

#endif // __GENERATE_GL_ARB_shader_subroutine

int CGL3ExtensionHandler::n_GetARBShaderSubroutineFuncPointers()
{
	int n_failed_function_num = 0;

#ifdef __GENERATE_GL_ARB_shader_subroutine

	if(!(glGetSubroutineUniformLocation = (int(GLApi*)(GLuint, GLenum, const GLchar*))wglGetProcAddress("glGetSubroutineUniformLocation"))) ++ n_failed_function_num;
	if(!(glGetSubroutineIndex = (GLuint(GLApi*)(GLuint, GLenum, const GLchar*))wglGetProcAddress("glGetSubroutineIndex"))) ++ n_failed_function_num;
	if(!(glGetActiveSubroutineUniformiv = (void(GLApi*)(GLuint, GLenum, GLuint, GLenum, int*))wglGetProcAddress("glGetActiveSubroutineUniformiv"))) ++ n_failed_function_num;
	if(!(glGetActiveSubroutineUniformName = (void(GLApi*)(GLuint, GLenum, GLuint, GLsizei, GLsizei*, GLchar*))wglGetProcAddress("glGetActiveSubroutineUniformName"))) ++ n_failed_function_num;
	if(!(glGetActiveSubroutineName = (void(GLApi*)(GLuint, GLenum, GLuint, GLsizei, GLsizei*, GLchar*))wglGetProcAddress("glGetActiveSubroutineName"))) ++ n_failed_function_num;
	if(!(glUniformSubroutinesuiv = (void(GLApi*)(GLenum, GLsizei, const GLuint*))wglGetProcAddress("glUniformSubroutinesuiv"))) ++ n_failed_function_num;
	if(!(glGetUniformSubroutineuiv = (void(GLApi*)(GLenum, int, GLuint*))wglGetProcAddress("glGetUniformSubroutineuiv"))) ++ n_failed_function_num;
	if(!(glGetProgramStageiv = (void(GLApi*)(GLuint, GLenum, GLenum, int*))wglGetProcAddress("glGetProgramStageiv"))) ++ n_failed_function_num;

#endif // __GENERATE_GL_ARB_shader_subroutine

	GLEH_ARB_shader_subroutine = !n_failed_function_num && b_SupportedExtension("GL_ARB_shader_subroutine");

	return n_failed_function_num;
}

/*
 *								--- ~GL_ARB_shader_subroutine ---
 */

/*
 *								--- GL_ARB_tessellation_shader ---
 */

bool GLEH_ARB_tessellation_shader = false;

#ifdef __GENERATE_GL_ARB_tessellation_shader

void (GLApi *glPatchParameteri)(GLenum, int) INIT_FUNC_POINTERS;
void (GLApi *glPatchParameterfv)(GLenum, const float*) INIT_FUNC_POINTERS;

#endif // __GENERATE_GL_ARB_tessellation_shader

int CGL3ExtensionHandler::n_GetARBTessellationShaderFuncPointers()
{
	int n_failed_function_num = 0;

#ifdef __GENERATE_GL_ARB_tessellation_shader

	if(!(glPatchParameteri = (void(GLApi*)(GLenum, int))wglGetProcAddress("glPatchParameteri"))) ++ n_failed_function_num;
	if(!(glPatchParameterfv = (void(GLApi*)(GLenum, const float*))wglGetProcAddress("glPatchParameterfv"))) ++ n_failed_function_num;

#endif // __GENERATE_GL_ARB_tessellation_shader

	GLEH_ARB_tessellation_shader = !n_failed_function_num && b_SupportedExtension("GL_ARB_tessellation_shader");

	return n_failed_function_num;
}

/*
 *								--- ~GL_ARB_tessellation_shader ---
 */

/*
 *								--- GL_ARB_texture_buffer_object_rgb32 ---
 */

bool GLEH_ARB_texture_buffer_object_rgb32 = false;

int CGL3ExtensionHandler::n_GetARBTextureBufferObjectRgb32FuncPointers()
{
	int n_failed_function_num = 0;

	// no new functions or procedures here

	GLEH_ARB_texture_buffer_object_rgb32 = !n_failed_function_num && b_SupportedExtension("GL_ARB_texture_buffer_object_rgb32");

	return n_failed_function_num;
}

/*
 *								--- ~GL_ARB_texture_buffer_object_rgb32 ---
 */

/*
 *								--- GL_ARB_transform_feedback2 ---
 */

bool GLEH_ARB_transform_feedback2 = false;

#ifdef __GENERATE_GL_ARB_transform_feedback2

void (GLApi *glBindTransformFeedback)(GLenum, GLuint) INIT_FUNC_POINTERS;
void (GLApi *glDeleteTransformFeedbacks)(GLsizei, const GLuint*) INIT_FUNC_POINTERS;
void (GLApi *glGenTransformFeedbacks)(GLsizei, GLuint*) INIT_FUNC_POINTERS;
GLboolean (GLApi *glIsTransformFeedback)(GLuint) INIT_FUNC_POINTERS;
void (GLApi *glPauseTransformFeedback)() INIT_FUNC_POINTERS;
void (GLApi *glResumeTransformFeedback)() INIT_FUNC_POINTERS;
void (GLApi *glDrawTransformFeedback)(GLenum, GLuint) INIT_FUNC_POINTERS;

#endif // __GENERATE_GL_ARB_transform_feedback2

int CGL3ExtensionHandler::n_GetARBTransformFeedback2FuncPointers()
{
	int n_failed_function_num = 0;

#ifdef __GENERATE_GL_ARB_transform_feedback2

	if(!(glBindTransformFeedback = (void(GLApi*)(GLenum, GLuint))wglGetProcAddress("glBindTransformFeedback"))) ++ n_failed_function_num;
	if(!(glDeleteTransformFeedbacks = (void(GLApi*)(GLsizei, const GLuint*))wglGetProcAddress("glDeleteTransformFeedbacks"))) ++ n_failed_function_num;
	if(!(glGenTransformFeedbacks = (void(GLApi*)(GLsizei, GLuint*))wglGetProcAddress("glGenTransformFeedbacks"))) ++ n_failed_function_num;
	if(!(glIsTransformFeedback = (GLboolean(GLApi*)(GLuint))wglGetProcAddress("glIsTransformFeedback"))) ++ n_failed_function_num;
	if(!(glPauseTransformFeedback = (void(GLApi*)())wglGetProcAddress("glPauseTransformFeedback"))) ++ n_failed_function_num;
	if(!(glResumeTransformFeedback = (void(GLApi*)())wglGetProcAddress("glResumeTransformFeedback"))) ++ n_failed_function_num;
	if(!(glDrawTransformFeedback = (void(GLApi*)(GLenum, GLuint))wglGetProcAddress("glDrawTransformFeedback"))) ++ n_failed_function_num;

#endif // __GENERATE_GL_ARB_transform_feedback2

	GLEH_ARB_transform_feedback2 = !n_failed_function_num && b_SupportedExtension("GL_ARB_transform_feedback2");

	return n_failed_function_num;
}

/*
 *								--- ~GL_ARB_transform_feedback2 ---
 */

/*
 *								--- GL_ARB_transform_feedback3 ---
 */

bool GLEH_ARB_transform_feedback3 = false;

#ifdef __GENERATE_GL_ARB_transform_feedback3

void (GLApi *glDrawTransformFeedbackStream)(GLenum, GLuint, GLuint) INIT_FUNC_POINTERS;
void (GLApi *glBeginQueryIndexed)(GLenum, GLuint, GLuint) INIT_FUNC_POINTERS;
void (GLApi *glEndQueryIndexed)(GLenum, GLuint) INIT_FUNC_POINTERS;
void (GLApi *glGetQueryIndexediv)(GLenum, GLuint, GLenum, int*) INIT_FUNC_POINTERS;

#endif // __GENERATE_GL_ARB_transform_feedback3

int CGL3ExtensionHandler::n_GetARBTransformFeedback3FuncPointers()
{
	int n_failed_function_num = 0;

#ifdef __GENERATE_GL_ARB_transform_feedback3

	if(!(glDrawTransformFeedbackStream = (void(GLApi*)(GLenum, GLuint, GLuint))wglGetProcAddress("glDrawTransformFeedbackStream"))) ++ n_failed_function_num;
	if(!(glBeginQueryIndexed = (void(GLApi*)(GLenum, GLuint, GLuint))wglGetProcAddress("glBeginQueryIndexed"))) ++ n_failed_function_num;
	if(!(glEndQueryIndexed = (void(GLApi*)(GLenum, GLuint))wglGetProcAddress("glEndQueryIndexed"))) ++ n_failed_function_num;
	if(!(glGetQueryIndexediv = (void(GLApi*)(GLenum, GLuint, GLenum, int*))wglGetProcAddress("glGetQueryIndexediv"))) ++ n_failed_function_num;

#endif // __GENERATE_GL_ARB_transform_feedback3

	GLEH_ARB_transform_feedback3 = !n_failed_function_num && b_SupportedExtension("GL_ARB_transform_feedback3");

	return n_failed_function_num;
}

/*
 *								--- ~GL_ARB_transform_feedback3 ---
 */

/*
 *								--- GL_ARB_ES2_compatibility ---
 */

bool GLEH_ARB_ES2_compatibility = false;

#ifdef __GENERATE_GL_ARB_ES2_compatibility

void (GLApi *glReleaseShaderCompiler)() INIT_FUNC_POINTERS;
void (GLApi *glShaderBinary)(GLsizei, const GLuint*, GLenum, const void*, GLsizei) INIT_FUNC_POINTERS;
void (GLApi *glGetShaderPrecisionFormat)(GLenum, GLenum, int*, int*) INIT_FUNC_POINTERS;
void (GLApi *glDepthRangef)(GLclampf, GLclampf) INIT_FUNC_POINTERS;
void (GLApi *glClearDepthf)(GLclampf) INIT_FUNC_POINTERS;

#endif // __GENERATE_GL_ARB_ES2_compatibility

int CGL3ExtensionHandler::n_GetARBES2CompatibilityFuncPointers()
{
	int n_failed_function_num = 0;

#ifdef __GENERATE_GL_ARB_ES2_compatibility

	if(!(glReleaseShaderCompiler = (void(GLApi*)())wglGetProcAddress("glReleaseShaderCompiler"))) ++ n_failed_function_num;
	if(!(glShaderBinary = (void(GLApi*)(GLsizei, const GLuint*, GLenum, const void*, GLsizei))wglGetProcAddress("glShaderBinary"))) ++ n_failed_function_num;
	if(!(glGetShaderPrecisionFormat = (void(GLApi*)(GLenum, GLenum, int*, int*))wglGetProcAddress("glGetShaderPrecisionFormat"))) ++ n_failed_function_num;
	if(!(glDepthRangef = (void(GLApi*)(GLclampf, GLclampf))wglGetProcAddress("glDepthRangef"))) ++ n_failed_function_num;
	if(!(glClearDepthf = (void(GLApi*)(GLclampf))wglGetProcAddress("glClearDepthf"))) ++ n_failed_function_num;

#endif // __GENERATE_GL_ARB_ES2_compatibility

	GLEH_ARB_ES2_compatibility = !n_failed_function_num && b_SupportedExtension("GL_ARB_ES2_compatibility");

	return n_failed_function_num;
}

/*
 *								--- ~GL_ARB_ES2_compatibility ---
 */

/*
 *								--- GL_ARB_get_program_binary ---
 */

bool GLEH_ARB_get_program_binary = false;

#ifdef __GENERATE_GL_ARB_get_program_binary

void (GLApi *glGetProgramBinary)(GLuint, GLsizei, GLsizei*, GLenum*, void*) INIT_FUNC_POINTERS;
void (GLApi *glProgramBinary)(GLuint, GLenum, const void*, GLsizei) INIT_FUNC_POINTERS;
//void (GLApi *glProgramParameteri)(GLuint, GLenum, int) INIT_FUNC_POINTERS;

#endif // __GENERATE_GL_ARB_get_program_binary

int CGL3ExtensionHandler::n_GetARBGetProgramBinaryFuncPointers()
{
	int n_failed_function_num = 0;

#ifdef __GENERATE_GL_ARB_get_program_binary

	if(!(glGetProgramBinary = (void(GLApi*)(GLuint, GLsizei, GLsizei*, GLenum*, void*))wglGetProcAddress("glGetProgramBinary"))) ++ n_failed_function_num;
	if(!(glProgramBinary = (void(GLApi*)(GLuint, GLenum, const void*, GLsizei))wglGetProcAddress("glProgramBinary"))) ++ n_failed_function_num;
	if(!(glProgramParameteri = (void(GLApi*)(GLuint, GLenum, int))wglGetProcAddress("glProgramParameteri"))) ++ n_failed_function_num;

#endif // __GENERATE_GL_ARB_get_program_binary

	GLEH_ARB_get_program_binary = !n_failed_function_num && b_SupportedExtension("GL_ARB_get_program_binary");

	return n_failed_function_num;
}

/*
 *								--- ~GL_ARB_get_program_binary ---
 */

/*
 *								--- GL_ARB_separate_shader_objects ---
 */

bool GLEH_ARB_separate_shader_objects = false;

#ifdef __GENERATE_GL_ARB_separate_shader_objects

void (GLApi *glUseProgramStages)(GLuint, GLbitfield, GLuint) INIT_FUNC_POINTERS;
void (GLApi *glActiveShaderProgram)(GLuint, GLuint) INIT_FUNC_POINTERS;
GLuint (GLApi *glCreateShaderProgramv)(GLenum, GLsizei, const GLchar**) INIT_FUNC_POINTERS;
void (GLApi *glBindProgramPipeline)(GLuint) INIT_FUNC_POINTERS;
void (GLApi *glDeleteProgramPipelines)(GLsizei, const GLuint*) INIT_FUNC_POINTERS;
void (GLApi *glGenProgramPipelines)(GLsizei, GLuint*) INIT_FUNC_POINTERS;
GLboolean (GLApi *glIsProgramPipeline)(GLuint) INIT_FUNC_POINTERS;
//void (GLApi *glProgramParameteri)(GLuint, GLenum, int) INIT_FUNC_POINTERS;
void (GLApi *glGetProgramPipelineiv)(GLuint, GLenum, int*) INIT_FUNC_POINTERS;
void (GLApi *glProgramUniform1i)(GLuint, int, int) INIT_FUNC_POINTERS;
void (GLApi *glProgramUniform1f)(GLuint, int, float) INIT_FUNC_POINTERS;
void (GLApi *glProgramUniform1d)(GLuint, int, double) INIT_FUNC_POINTERS;
void (GLApi *glProgramUniform1iv)(GLuint, int, GLsizei, const int) INIT_FUNC_POINTERS;
void (GLApi *glProgramUniform1fv)(GLuint, int, GLsizei, const float) INIT_FUNC_POINTERS;
void (GLApi *glProgramUniform1dv)(GLuint, int, GLsizei, const double) INIT_FUNC_POINTERS;
void (GLApi *glProgramUniform1ui)(GLuint, int, GLuint) INIT_FUNC_POINTERS;
void (GLApi *glProgramUniform1uiv)(GLuint, int, GLsizei, const GLuint*) INIT_FUNC_POINTERS;
void (GLApi *glProgramUniform2i)(GLuint, int, int, int) INIT_FUNC_POINTERS;
void (GLApi *glProgramUniform2f)(GLuint, int, float, float) INIT_FUNC_POINTERS;
void (GLApi *glProgramUniform2d)(GLuint, int, double, double) INIT_FUNC_POINTERS;
void (GLApi *glProgramUniform2iv)(GLuint, int, GLsizei, const int, const int) INIT_FUNC_POINTERS;
void (GLApi *glProgramUniform2fv)(GLuint, int, GLsizei, const float, const float) INIT_FUNC_POINTERS;
void (GLApi *glProgramUniform2dv)(GLuint, int, GLsizei, const double, const double) INIT_FUNC_POINTERS;
void (GLApi *glProgramUniform2ui)(GLuint, int, GLuint, GLuint) INIT_FUNC_POINTERS;
void (GLApi *glProgramUniform2uiv)(GLuint, int, GLsizei, const GLuint*) INIT_FUNC_POINTERS;
void (GLApi *glProgramUniform3i)(GLuint, int, int, int, int) INIT_FUNC_POINTERS;
void (GLApi *glProgramUniform3f)(GLuint, int, float, float, float) INIT_FUNC_POINTERS;
void (GLApi *glProgramUniform3d)(GLuint, int, double, double, double) INIT_FUNC_POINTERS;
void (GLApi *glProgramUniform3iv)(GLuint, int, GLsizei, const int, const int, const int) INIT_FUNC_POINTERS;
void (GLApi *glProgramUniform3fv)(GLuint, int, GLsizei, const float, const float, const float) INIT_FUNC_POINTERS;
void (GLApi *glProgramUniform3dv)(GLuint, int, GLsizei, const double, const double, const double) INIT_FUNC_POINTERS;
void (GLApi *glProgramUniform3ui)(GLuint, int, GLuint, GLuint, GLuint) INIT_FUNC_POINTERS;
void (GLApi *glProgramUniform3uiv)(GLuint, int, GLsizei, const GLuint*) INIT_FUNC_POINTERS;
void (GLApi *glProgramUniform4i)(GLuint, int, int, int, int, int) INIT_FUNC_POINTERS;
void (GLApi *glProgramUniform4f)(GLuint, int, float, float, float, float) INIT_FUNC_POINTERS;
void (GLApi *glProgramUniform4d)(GLuint, int, double, double, double, double) INIT_FUNC_POINTERS;
void (GLApi *glProgramUniform4iv)(GLuint, int, GLsizei, const int, const int, const int, const int) INIT_FUNC_POINTERS;
void (GLApi *glProgramUniform4fv)(GLuint, int, GLsizei, const float, const float, const float, const float) INIT_FUNC_POINTERS;
void (GLApi *glProgramUniform4dv)(GLuint, int, GLsizei, const double, const double, const double, const double) INIT_FUNC_POINTERS;
void (GLApi *glProgramUniform4ui)(GLuint, int, GLuint, GLuint, GLuint, GLuint) INIT_FUNC_POINTERS;
void (GLApi *glProgramUniform4uiv)(GLuint, int, GLsizei, const GLuint*) INIT_FUNC_POINTERS;
void (GLApi *glValidateProgramPipeline)(GLuint) INIT_FUNC_POINTERS;
void (GLApi *glGetProgramPipelineInfoLog)(GLuint, GLsizei, GLsizei*, GLchar*) INIT_FUNC_POINTERS;

#endif // __GENERATE_GL_ARB_separate_shader_objects

int CGL3ExtensionHandler::n_GetARBSeparateShaderObjectsFuncPointers()
{
	int n_failed_function_num = 0;

#ifdef __GENERATE_GL_ARB_separate_shader_objects

	if(!(glUseProgramStages = (void(GLApi*)(GLuint, GLbitfield, GLuint))wglGetProcAddress("glUseProgramStages"))) ++ n_failed_function_num;
	if(!(glActiveShaderProgram = (void(GLApi*)(GLuint, GLuint))wglGetProcAddress("glActiveShaderProgram"))) ++ n_failed_function_num;
	if(!(glCreateShaderProgramv = (GLuint(GLApi*)(GLenum, GLsizei, const GLchar**))wglGetProcAddress("glCreateShaderProgramv"))) ++ n_failed_function_num;
	if(!(glBindProgramPipeline = (void(GLApi*)(GLuint))wglGetProcAddress("glBindProgramPipeline"))) ++ n_failed_function_num;
	if(!(glDeleteProgramPipelines = (void(GLApi*)(GLsizei, const GLuint*))wglGetProcAddress("glDeleteProgramPipelines"))) ++ n_failed_function_num;
	if(!(glGenProgramPipelines = (void(GLApi*)(GLsizei, GLuint*))wglGetProcAddress("glGenProgramPipelines"))) ++ n_failed_function_num;
	if(!(glIsProgramPipeline = (GLboolean(GLApi*)(GLuint))wglGetProcAddress("glIsProgramPipeline"))) ++ n_failed_function_num;
	if(!(glProgramParameteri = (void(GLApi*)(GLuint, GLenum, int))wglGetProcAddress("glProgramParameteri"))) ++ n_failed_function_num;
	if(!(glGetProgramPipelineiv = (void(GLApi*)(GLuint, GLenum, int*))wglGetProcAddress("glGetProgramPipelineiv"))) ++ n_failed_function_num;
	if(!(glProgramUniform1i = (void(GLApi*)(GLuint, int, int))wglGetProcAddress("glProgramUniform1i"))) ++ n_failed_function_num;
	if(!(glProgramUniform1f = (void(GLApi*)(GLuint, int, float))wglGetProcAddress("glProgramUniform1f"))) ++ n_failed_function_num;
	if(!(glProgramUniform1d = (void(GLApi*)(GLuint, int, double))wglGetProcAddress("glProgramUniform1d"))) ++ n_failed_function_num;
	if(!(glProgramUniform1iv = (void(GLApi*)(GLuint, int, GLsizei, const int))wglGetProcAddress("glProgramUniform1iv"))) ++ n_failed_function_num;
	if(!(glProgramUniform1fv = (void(GLApi*)(GLuint, int, GLsizei, const float))wglGetProcAddress("glProgramUniform1fv"))) ++ n_failed_function_num;
	if(!(glProgramUniform1dv = (void(GLApi*)(GLuint, int, GLsizei, const double))wglGetProcAddress("glProgramUniform1dv"))) ++ n_failed_function_num;
	if(!(glProgramUniform1ui = (void(GLApi*)(GLuint, int, GLuint))wglGetProcAddress("glProgramUniform1ui"))) ++ n_failed_function_num;
	if(!(glProgramUniform1uiv = (void(GLApi*)(GLuint, int, GLsizei, const GLuint*))wglGetProcAddress("glProgramUniform1uiv"))) ++ n_failed_function_num;
	if(!(glProgramUniform2i = (void(GLApi*)(GLuint, int, int, int))wglGetProcAddress("glProgramUniform2i"))) ++ n_failed_function_num;
	if(!(glProgramUniform2f = (void(GLApi*)(GLuint, int, float, float))wglGetProcAddress("glProgramUniform2f"))) ++ n_failed_function_num;
	if(!(glProgramUniform2d = (void(GLApi*)(GLuint, int, double, double))wglGetProcAddress("glProgramUniform2d"))) ++ n_failed_function_num;
	if(!(glProgramUniform2iv = (void(GLApi*)(GLuint, int, GLsizei, const int, const int))wglGetProcAddress("glProgramUniform2iv"))) ++ n_failed_function_num;
	if(!(glProgramUniform2fv = (void(GLApi*)(GLuint, int, GLsizei, const float, const float))wglGetProcAddress("glProgramUniform2fv"))) ++ n_failed_function_num;
	if(!(glProgramUniform2dv = (void(GLApi*)(GLuint, int, GLsizei, const double, const double))wglGetProcAddress("glProgramUniform2dv"))) ++ n_failed_function_num;
	if(!(glProgramUniform2ui = (void(GLApi*)(GLuint, int, GLuint, GLuint))wglGetProcAddress("glProgramUniform2ui"))) ++ n_failed_function_num;
	if(!(glProgramUniform2uiv = (void(GLApi*)(GLuint, int, GLsizei, const GLuint*))wglGetProcAddress("glProgramUniform2uiv"))) ++ n_failed_function_num;
	if(!(glProgramUniform3i = (void(GLApi*)(GLuint, int, int, int, int))wglGetProcAddress("glProgramUniform3i"))) ++ n_failed_function_num;
	if(!(glProgramUniform3f = (void(GLApi*)(GLuint, int, float, float, float))wglGetProcAddress("glProgramUniform3f"))) ++ n_failed_function_num;
	if(!(glProgramUniform3d = (void(GLApi*)(GLuint, int, double, double, double))wglGetProcAddress("glProgramUniform3d"))) ++ n_failed_function_num;
	if(!(glProgramUniform3iv = (void(GLApi*)(GLuint, int, GLsizei, const int, const int, const int))wglGetProcAddress("glProgramUniform3iv"))) ++ n_failed_function_num;
	if(!(glProgramUniform3fv = (void(GLApi*)(GLuint, int, GLsizei, const float, const float, const float))wglGetProcAddress("glProgramUniform3fv"))) ++ n_failed_function_num;
	if(!(glProgramUniform3dv = (void(GLApi*)(GLuint, int, GLsizei, const double, const double, const double))wglGetProcAddress("glProgramUniform3dv"))) ++ n_failed_function_num;
	if(!(glProgramUniform3ui = (void(GLApi*)(GLuint, int, GLuint, GLuint, GLuint))wglGetProcAddress("glProgramUniform3ui"))) ++ n_failed_function_num;
	if(!(glProgramUniform3uiv = (void(GLApi*)(GLuint, int, GLsizei, const GLuint*))wglGetProcAddress("glProgramUniform3uiv"))) ++ n_failed_function_num;
	if(!(glProgramUniform4i = (void(GLApi*)(GLuint, int, int, int, int, int))wglGetProcAddress("glProgramUniform4i"))) ++ n_failed_function_num;
	if(!(glProgramUniform4f = (void(GLApi*)(GLuint, int, float, float, float, float))wglGetProcAddress("glProgramUniform4f"))) ++ n_failed_function_num;
	if(!(glProgramUniform4d = (void(GLApi*)(GLuint, int, double, double, double, double))wglGetProcAddress("glProgramUniform4d"))) ++ n_failed_function_num;
	if(!(glProgramUniform4iv = (void(GLApi*)(GLuint, int, GLsizei, const int, const int, const int, const int))wglGetProcAddress("glProgramUniform4iv"))) ++ n_failed_function_num;
	if(!(glProgramUniform4fv = (void(GLApi*)(GLuint, int, GLsizei, const float, const float, const float, const float))wglGetProcAddress("glProgramUniform4fv"))) ++ n_failed_function_num;
	if(!(glProgramUniform4dv = (void(GLApi*)(GLuint, int, GLsizei, const double, const double, const double, const double))wglGetProcAddress("glProgramUniform4dv"))) ++ n_failed_function_num;
	if(!(glProgramUniform4ui = (void(GLApi*)(GLuint, int, GLuint, GLuint, GLuint, GLuint))wglGetProcAddress("glProgramUniform4ui"))) ++ n_failed_function_num;
	if(!(glProgramUniform4uiv = (void(GLApi*)(GLuint, int, GLsizei, const GLuint*))wglGetProcAddress("glProgramUniform4uiv"))) ++ n_failed_function_num;
	if(!(glValidateProgramPipeline = (void(GLApi*)(GLuint))wglGetProcAddress("glValidateProgramPipeline"))) ++ n_failed_function_num;
	if(!(glGetProgramPipelineInfoLog = (void(GLApi*)(GLuint, GLsizei, GLsizei*, GLchar*))wglGetProcAddress("glGetProgramPipelineInfoLog"))) ++ n_failed_function_num;

#endif // __GENERATE_GL_ARB_separate_shader_objects

	GLEH_ARB_separate_shader_objects = !n_failed_function_num && b_SupportedExtension("GL_ARB_separate_shader_objects");

	return n_failed_function_num;
}

/*
 *								--- ~GL_ARB_separate_shader_objects ---
 */

/*
 *								--- GL_ARB_shader_precision ---
 */

bool GLEH_ARB_shader_precision = false;

int CGL3ExtensionHandler::n_GetARBShaderPrecisionFuncPointers()
{
	int n_failed_function_num = 0;

	// no new functions or procedures here

	GLEH_ARB_shader_precision = !n_failed_function_num && b_SupportedExtension("GL_ARB_shader_precision");

	return n_failed_function_num;
}

/*
 *								--- ~GL_ARB_shader_precision ---
 */

/*
 *								--- GL_ARB_vertex_attrib_64bit ---
 */

bool GLEH_ARB_vertex_attrib_64bit = false;

#ifdef __GENERATE_GL_ARB_vertex_attrib_64bit

void (GLApi *glVertexAttribL1d)(GLuint, double) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttribL2d)(GLuint, double, double) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttribL3d)(GLuint, double, double, double) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttribL4d)(GLuint, double, double, double, double) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttribL1dv)(GLuint, const double*) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttribL2dv)(GLuint, const double*) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttribL3dv)(GLuint, const double*) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttribL4dv)(GLuint, const double*) INIT_FUNC_POINTERS;
void (GLApi *glVertexAttribLPointer)(GLuint, int, GLenum, GLsizei, const void*) INIT_FUNC_POINTERS;
void (GLApi *glGetVertexAttribLdv)(GLuint, GLenum, double*) INIT_FUNC_POINTERS;
void (GLApi *glVertexArrayVertexAttribLOffsetEXT)(GLuint, GLuint, GLuint, int, GLenum, GLsizei, GLintptr) INIT_FUNC_POINTERS;

#endif // __GENERATE_GL_ARB_vertex_attrib_64bit

int CGL3ExtensionHandler::n_GetARBVertexAttrib64bitFuncPointers()
{
	int n_failed_function_num = 0;

#ifdef __GENERATE_GL_ARB_vertex_attrib_64bit

	if(!(glVertexAttribL1d = (void(GLApi*)(GLuint, double))wglGetProcAddress("glVertexAttribL1d"))) ++ n_failed_function_num;
	if(!(glVertexAttribL2d = (void(GLApi*)(GLuint, double, double))wglGetProcAddress("glVertexAttribL2d"))) ++ n_failed_function_num;
	if(!(glVertexAttribL3d = (void(GLApi*)(GLuint, double, double, double))wglGetProcAddress("glVertexAttribL3d"))) ++ n_failed_function_num;
	if(!(glVertexAttribL4d = (void(GLApi*)(GLuint, double, double, double, double))wglGetProcAddress("glVertexAttribL4d"))) ++ n_failed_function_num;
	if(!(glVertexAttribL1dv = (void(GLApi*)(GLuint, const double*))wglGetProcAddress("glVertexAttribL1dv"))) ++ n_failed_function_num;
	if(!(glVertexAttribL2dv = (void(GLApi*)(GLuint, const double*))wglGetProcAddress("glVertexAttribL2dv"))) ++ n_failed_function_num;
	if(!(glVertexAttribL3dv = (void(GLApi*)(GLuint, const double*))wglGetProcAddress("glVertexAttribL3dv"))) ++ n_failed_function_num;
	if(!(glVertexAttribL4dv = (void(GLApi*)(GLuint, const double*))wglGetProcAddress("glVertexAttribL4dv"))) ++ n_failed_function_num;
	if(!(glVertexAttribLPointer = (void(GLApi*)(GLuint, int, GLenum, GLsizei, const void*))wglGetProcAddress("glVertexAttribLPointer"))) ++ n_failed_function_num;
	if(!(glGetVertexAttribLdv = (void(GLApi*)(GLuint, GLenum, double*))wglGetProcAddress("glGetVertexAttribLdv"))) ++ n_failed_function_num;
	if(!(glVertexArrayVertexAttribLOffsetEXT = (void(GLApi*)(GLuint, GLuint, GLuint, int, GLenum, GLsizei, GLintptr))wglGetProcAddress("glVertexArrayVertexAttribLOffsetEXT"))) ++ n_failed_function_num;

#endif // __GENERATE_GL_ARB_vertex_attrib_64bit

	GLEH_ARB_vertex_attrib_64bit = !n_failed_function_num && b_SupportedExtension("GL_ARB_vertex_attrib_64bit");

	return n_failed_function_num;
}

/*
 *								--- ~GL_ARB_vertex_attrib_64bit ---
 */

/*
 *								--- GL_ARB_viewport_array ---
 */

bool GLEH_ARB_viewport_array = false;

#ifdef __GENERATE_GL_ARB_viewport_array

void (GLApi *glViewportArrayv)(GLuint, GLsizei, const float*) INIT_FUNC_POINTERS;
void (GLApi *glViewportIndexedf)(GLuint, float, float, float, float) INIT_FUNC_POINTERS;
void (GLApi *glViewportIndexedfv)(GLuint, const float*) INIT_FUNC_POINTERS;
void (GLApi *glScissorArrayv)(GLuint, GLsizei, const int*) INIT_FUNC_POINTERS;
void (GLApi *glScissorIndexed)(GLuint, int, int, GLsizei, GLsizei) INIT_FUNC_POINTERS;
void (GLApi *glScissorIndexedv)(GLuint, const int*) INIT_FUNC_POINTERS;
void (GLApi *glDepthRangeArrayv)(GLuint, GLsizei, const GLclampd*) INIT_FUNC_POINTERS;
void (GLApi *glDepthRangeIndexed)(GLuint, GLclampd, GLclampd) INIT_FUNC_POINTERS;
void (GLApi *glGetFloati_v)(GLenum, GLuint, float*) INIT_FUNC_POINTERS;
void (GLApi *glGetDoublei_v)(GLenum, GLuint, double*) INIT_FUNC_POINTERS;
void (GLApi *glGetIntegerIndexedivEXT)(GLenum, GLuint, int*) INIT_FUNC_POINTERS;
//void (GLApi *glEnableIndexedEXT)(GLenum, GLuint) INIT_FUNC_POINTERS;
//void (GLApi *glDisableIndexedEXT)(GLenum, GLuint) INIT_FUNC_POINTERS;
//GLboolean (GLApi *glIsEnabledIndexedEXT)(GLenum, GLuint) INIT_FUNC_POINTERS;

#endif // __GENERATE_GL_ARB_viewport_array

int CGL3ExtensionHandler::n_GetARBViewportArrayFuncPointers()
{
	int n_failed_function_num = 0;

#ifdef __GENERATE_GL_ARB_viewport_array

	if(!(glViewportArrayv = (void(GLApi*)(GLuint, GLsizei, const float*))wglGetProcAddress("glViewportArrayv"))) ++ n_failed_function_num;
	if(!(glViewportIndexedf = (void(GLApi*)(GLuint, float, float, float, float))wglGetProcAddress("glViewportIndexedf"))) ++ n_failed_function_num;
	if(!(glViewportIndexedfv = (void(GLApi*)(GLuint, const float*))wglGetProcAddress("glViewportIndexedfv"))) ++ n_failed_function_num;
	if(!(glScissorArrayv = (void(GLApi*)(GLuint, GLsizei, const int*))wglGetProcAddress("glScissorArrayv"))) ++ n_failed_function_num;
	if(!(glScissorIndexed = (void(GLApi*)(GLuint, int, int, GLsizei, GLsizei))wglGetProcAddress("glScissorIndexed"))) ++ n_failed_function_num;
	if(!(glScissorIndexedv = (void(GLApi*)(GLuint, const int*))wglGetProcAddress("glScissorIndexedv"))) ++ n_failed_function_num;
	if(!(glDepthRangeArrayv = (void(GLApi*)(GLuint, GLsizei, const GLclampd*))wglGetProcAddress("glDepthRangeArrayv"))) ++ n_failed_function_num;
	if(!(glDepthRangeIndexed = (void(GLApi*)(GLuint, GLclampd, GLclampd))wglGetProcAddress("glDepthRangeIndexed"))) ++ n_failed_function_num;
	if(!(glGetFloati_v = (void(GLApi*)(GLenum, GLuint, float*))wglGetProcAddress("glGetFloati_v"))) ++ n_failed_function_num;
	if(!(glGetDoublei_v = (void(GLApi*)(GLenum, GLuint, double*))wglGetProcAddress("glGetDoublei_v"))) ++ n_failed_function_num;
	if(!(glGetIntegerIndexedivEXT = (void(GLApi*)(GLenum, GLuint, int*))wglGetProcAddress("glGetIntegerIndexedivEXT"))) ++ n_failed_function_num;
	if(!(glEnableIndexedEXT = (void(GLApi*)(GLenum, GLuint))wglGetProcAddress("glEnableIndexedEXT"))) ++ n_failed_function_num;
	if(!(glDisableIndexedEXT = (void(GLApi*)(GLenum, GLuint))wglGetProcAddress("glDisableIndexedEXT"))) ++ n_failed_function_num;
	if(!(glIsEnabledIndexedEXT = (GLboolean(GLApi*)(GLenum, GLuint))wglGetProcAddress("glIsEnabledIndexedEXT"))) ++ n_failed_function_num;

#endif // __GENERATE_GL_ARB_viewport_array

	GLEH_ARB_viewport_array = !n_failed_function_num && b_SupportedExtension("GL_ARB_viewport_array");

	return n_failed_function_num;
}

/*
 *								--- ~GL_ARB_viewport_array ---
 */

/*
 *								--- GL_ARB_cl_event ---
 */

bool GLEH_ARB_cl_event = false;

#ifdef __GENERATE_GL_ARB_cl_event

GLsync (GLApi *glCreateSyncFromCLeventARB)(GLcl_context, GLcl_event, GLbitfield) INIT_FUNC_POINTERS;

#endif // __GENERATE_GL_ARB_cl_event

int CGL3ExtensionHandler::n_GetARBClEventFuncPointers()
{
	int n_failed_function_num = 0;

#ifdef __GENERATE_GL_ARB_cl_event

	if(!(glCreateSyncFromCLeventARB = (GLsync(GLApi*)(GLcl_context, GLcl_event, GLbitfield))wglGetProcAddress("glCreateSyncFromCLeventARB"))) ++ n_failed_function_num;

#endif // __GENERATE_GL_ARB_cl_event

	GLEH_ARB_cl_event = !n_failed_function_num && b_SupportedExtension("GL_ARB_cl_event");

	return n_failed_function_num;
}

/*
 *								--- ~GL_ARB_cl_event ---
 */

/*
 *								--- GL_ARB_debug_output ---
 */

bool GLEH_ARB_debug_output = false;

#ifdef __GENERATE_GL_ARB_debug_output

void (GLApi *glDebugMessageControlARB)(GLenum, GLenum, GLenum, GLsizei, const GLuint*, GLboolean) INIT_FUNC_POINTERS;
void (GLApi *glDebugMessageInsertARB)(GLenum, GLenum, GLuint, GLenum, GLsizei, const GLchar*) INIT_FUNC_POINTERS;
void (GLApi *glDebugMessageCallbackARB)(GLDEBUGPROCARB, void*) INIT_FUNC_POINTERS;
GLuint (GLApi *glGetDebugMessageLogARB)(GLuint, GLsizei, GLenum*, GLenum*, GLuint*, GLenum*, GLsizei*, GLchar*) INIT_FUNC_POINTERS;
void (GLApi *glGetPointerv)(GLenum, void**) INIT_FUNC_POINTERS;

#endif // __GENERATE_GL_ARB_debug_output

int CGL3ExtensionHandler::n_GetARBDebugOutputFuncPointers()
{
	int n_failed_function_num = 0;

#ifdef __GENERATE_GL_ARB_debug_output

	if(!(glDebugMessageControlARB = (void(GLApi*)(GLenum, GLenum, GLenum, GLsizei, const GLuint*, GLboolean))wglGetProcAddress("glDebugMessageControlARB"))) ++ n_failed_function_num;
	if(!(glDebugMessageInsertARB = (void(GLApi*)(GLenum, GLenum, GLuint, GLenum, GLsizei, const GLchar*))wglGetProcAddress("glDebugMessageInsertARB"))) ++ n_failed_function_num;
	if(!(glDebugMessageCallbackARB = (void(GLApi*)(GLDEBUGPROCARB, void*))wglGetProcAddress("glDebugMessageCallbackARB"))) ++ n_failed_function_num;
	if(!(glGetDebugMessageLogARB = (GLuint(GLApi*)(GLuint, GLsizei, GLenum*, GLenum*, GLuint*, GLenum*, GLsizei*, GLchar*))wglGetProcAddress("glGetDebugMessageLogARB"))) ++ n_failed_function_num;
	if(!(glGetPointerv = (void(GLApi*)(GLenum, void**))wglGetProcAddress("glGetPointerv"))) ++ n_failed_function_num;

#endif // __GENERATE_GL_ARB_debug_output

	GLEH_ARB_debug_output = !n_failed_function_num && b_SupportedExtension("GL_ARB_debug_output");

	return n_failed_function_num;
}

/*
 *								--- ~GL_ARB_debug_output ---
 */

/*
 *								--- GL_ARB_robustness ---
 */

bool GLEH_ARB_robustness = false;

#ifdef __GENERATE_GL_ARB_robustness

GLenum (GLApi *glGetGraphicsResetStatusARB)() INIT_FUNC_POINTERS;
void (GLApi *glGetnMapdvARB)(GLenum, GLenum, GLsizei, double*) INIT_FUNC_POINTERS;
void (GLApi *glGetnMapfvARB)(GLenum, GLenum, GLsizei, float*) INIT_FUNC_POINTERS;
void (GLApi *glGetnMapivARB)(GLenum, GLenum, GLsizei, int*) INIT_FUNC_POINTERS;
void (GLApi *glGetnPixelMapfvARB)(GLenum, GLsizei, float*) INIT_FUNC_POINTERS;
void (GLApi *glGetnPixelMapuivARB)(GLenum, GLsizei, GLuint*) INIT_FUNC_POINTERS;
void (GLApi *glGetnPixelMapusvARB)(GLenum, GLsizei, GLushort*) INIT_FUNC_POINTERS;
void (GLApi *glGetnPolygonStippleARB)(GLsizei, GLubyte*) INIT_FUNC_POINTERS;
void (GLApi *glGetnTexImageARB)(GLenum, int, GLenum, GLenum, GLsizei, void*) INIT_FUNC_POINTERS;
void (GLApi *glReadnPixelsARB)(int, int, GLsizei, GLsizei, GLenum, GLenum, GLsizei, void*) INIT_FUNC_POINTERS;
void (GLApi *glGetnColorTableARB)(GLenum, GLenum, GLenum, GLsizei, void*) INIT_FUNC_POINTERS;
void (GLApi *glGetnConvolutionFilterARB)(GLenum, GLenum, GLenum, GLsizei, void*) INIT_FUNC_POINTERS;
void (GLApi *glGetnSeparableFilterARB)(GLenum, GLenum, GLenum, GLsizei, void*, GLsizei, void*, void*) INIT_FUNC_POINTERS;
void (GLApi *glGetnHistogramARB)(GLenum, GLboolean, GLenum, GLenum, GLsizei, void*) INIT_FUNC_POINTERS;
void (GLApi *glGetnMinmaxARB)(GLenum, GLboolean, GLenum, GLenum, GLsizei, void*) INIT_FUNC_POINTERS;
void (GLApi *glGetnCompressedTexImageARB)(GLenum, int, GLsizei, void*) INIT_FUNC_POINTERS;
void (GLApi *glGetnUniformfvARB)(GLuint, int, GLsizei, float*) INIT_FUNC_POINTERS;
void (GLApi *glGetnUniformivARB)(GLuint, int, GLsizei, int*) INIT_FUNC_POINTERS;
void (GLApi *glGetnUniformuivARB)(GLuint, int, GLsizei, GLuint*) INIT_FUNC_POINTERS;
void (GLApi *glGetnUniformdvARB)(GLuint, int, GLsizei, double*) INIT_FUNC_POINTERS;

#endif // __GENERATE_GL_ARB_robustness

int CGL3ExtensionHandler::n_GetARBRobustnessFuncPointers()
{
	int n_failed_function_num = 0;

#ifdef __GENERATE_GL_ARB_robustness

	if(!(glGetGraphicsResetStatusARB = (GLenum(GLApi*)())wglGetProcAddress("glGetGraphicsResetStatusARB"))) ++ n_failed_function_num;
	if(!(glGetnMapdvARB = (void(GLApi*)(GLenum, GLenum, GLsizei, double*))wglGetProcAddress("glGetnMapdvARB"))) ++ n_failed_function_num;
	if(!(glGetnMapfvARB = (void(GLApi*)(GLenum, GLenum, GLsizei, float*))wglGetProcAddress("glGetnMapfvARB"))) ++ n_failed_function_num;
	if(!(glGetnMapivARB = (void(GLApi*)(GLenum, GLenum, GLsizei, int*))wglGetProcAddress("glGetnMapivARB"))) ++ n_failed_function_num;
	if(!(glGetnPixelMapfvARB = (void(GLApi*)(GLenum, GLsizei, float*))wglGetProcAddress("glGetnPixelMapfvARB"))) ++ n_failed_function_num;
	if(!(glGetnPixelMapuivARB = (void(GLApi*)(GLenum, GLsizei, GLuint*))wglGetProcAddress("glGetnPixelMapuivARB"))) ++ n_failed_function_num;
	if(!(glGetnPixelMapusvARB = (void(GLApi*)(GLenum, GLsizei, GLushort*))wglGetProcAddress("glGetnPixelMapusvARB"))) ++ n_failed_function_num;
	if(!(glGetnPolygonStippleARB = (void(GLApi*)(GLsizei, GLubyte*))wglGetProcAddress("glGetnPolygonStippleARB"))) ++ n_failed_function_num;
	if(!(glGetnTexImageARB = (void(GLApi*)(GLenum, int, GLenum, GLenum, GLsizei, void*))wglGetProcAddress("glGetnTexImageARB"))) ++ n_failed_function_num;
	if(!(glReadnPixelsARB = (void(GLApi*)(int, int, GLsizei, GLsizei, GLenum, GLenum, GLsizei, void*))wglGetProcAddress("glReadnPixelsARB"))) ++ n_failed_function_num;
	if(!(glGetnColorTableARB = (void(GLApi*)(GLenum, GLenum, GLenum, GLsizei, void*))wglGetProcAddress("glGetnColorTableARB"))) ++ n_failed_function_num;
	if(!(glGetnConvolutionFilterARB = (void(GLApi*)(GLenum, GLenum, GLenum, GLsizei, void*))wglGetProcAddress("glGetnConvolutionFilterARB"))) ++ n_failed_function_num;
	if(!(glGetnSeparableFilterARB = (void(GLApi*)(GLenum, GLenum, GLenum, GLsizei, void*, GLsizei, void*, void*))wglGetProcAddress("glGetnSeparableFilterARB"))) ++ n_failed_function_num;
	if(!(glGetnHistogramARB = (void(GLApi*)(GLenum, GLboolean, GLenum, GLenum, GLsizei, void*))wglGetProcAddress("glGetnHistogramARB"))) ++ n_failed_function_num;
	if(!(glGetnMinmaxARB = (void(GLApi*)(GLenum, GLboolean, GLenum, GLenum, GLsizei, void*))wglGetProcAddress("glGetnMinmaxARB"))) ++ n_failed_function_num;
	if(!(glGetnCompressedTexImageARB = (void(GLApi*)(GLenum, int, GLsizei, void*))wglGetProcAddress("glGetnCompressedTexImageARB"))) ++ n_failed_function_num;
	if(!(glGetnUniformfvARB = (void(GLApi*)(GLuint, int, GLsizei, float*))wglGetProcAddress("glGetnUniformfvARB"))) ++ n_failed_function_num;
	if(!(glGetnUniformivARB = (void(GLApi*)(GLuint, int, GLsizei, int*))wglGetProcAddress("glGetnUniformivARB"))) ++ n_failed_function_num;
	if(!(glGetnUniformuivARB = (void(GLApi*)(GLuint, int, GLsizei, GLuint*))wglGetProcAddress("glGetnUniformuivARB"))) ++ n_failed_function_num;
	if(!(glGetnUniformdvARB = (void(GLApi*)(GLuint, int, GLsizei, double*))wglGetProcAddress("glGetnUniformdvARB"))) ++ n_failed_function_num;

#endif // __GENERATE_GL_ARB_robustness

	GLEH_ARB_robustness = !n_failed_function_num && b_SupportedExtension("GL_ARB_robustness");

	return n_failed_function_num;
}

/*
 *								--- ~GL_ARB_robustness ---
 */

/*
 *								--- GL_ARB_shader_stencil_export ---
 */

bool GLEH_ARB_shader_stencil_export = false;

int CGL3ExtensionHandler::n_GetARBShaderStencilExportFuncPointers()
{
	int n_failed_function_num = 0;

	// no new functions or procedures here

	GLEH_ARB_shader_stencil_export = !n_failed_function_num && b_SupportedExtension("GL_ARB_shader_stencil_export");

	return n_failed_function_num;
}

/*
 *								--- ~GL_ARB_shader_stencil_export ---
 */

// empty template for Get_FuncPointers() functions
#if 0

/*
 *								---  ---
 */

#ifdef 

#endif //

bool GLEH_ARB_ = false;

int CGL3ExtensionHandler::n_GetARBFuncPointers()
{
	int n_failed_function_num = 0;

#ifdef 

	if(!( = (void(GLApi*)())wglGetProcAddress(""))) ++ n_failed_function_num;

#endif //

	GLEH_ARB_ = !n_failed_function_num && b_SupportedExtension("GL_ARB_");

	return n_failed_function_num;
}

#endif // 0

/*
 *								=== ~CGL3ExtensionHandler ===
 */
