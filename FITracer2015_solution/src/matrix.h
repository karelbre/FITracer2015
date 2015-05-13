/*
 *  matrix.h
 *  asrTracer
 *
 *  Created by Petrik Clarberg on 2006-02-22.
 *  Copyright 2006 Lund University. All rights reserved.
 *
 */

#ifndef MATRIX_H
#define MATRIX_H

#include <iostream>
#include <stdexcept>
#include <cmath>

class Vector2D;
class Vector3D;
class Point2D;
class Point3D;
class Matrix;

/**
 * Class representing a 4x4 transformation matrix.
 */
class Matrix
{
public:
	/// Default constructor. The matrix is initialized to identity.
	Matrix() { identity(); }
		
	/// Constructor initializing all elements to the given value.
	explicit Matrix(float s) { for(int i=0; i<16; i++) e[i]=s; }
	
	/// Assignment from a scalar value.
	Matrix& operator=(float s) { for(int i=0; i<16; i++) e[i]=s; return *this; }
	
	/// Returns a copy of the element at position row,column (i,j).
	/// @remark The range of the parameters (i,j) is not checked.
	float operator()(int i, int j) const { return e[4*i+j]; }
	
	/// Returns a reference to the element at position row,column (i,j).
	/// @remark The range of the parameters (i,j) is not checked.
	float& operator()(int i, int j) { return e[4*i+j]; }
	
	/// Clear all elements to zero.
	Matrix& zero() { for(int i=0; i<16; i++) e[i]=0.0f; return *this; }
	
	/// Set matrix to identity.
	Matrix& identity()
	{
		e[0]  = 1.0f;	e[1]  = 0.0f;	e[2]  = 0.0f;	e[3]  = 0.0f;
		e[4]  = 0.0f;	e[5]  = 1.0f;	e[6]  = 0.0f;	e[7]  = 0.0f;
		e[8]  = 0.0f;	e[9]  = 0.0f;	e[10] = 1.0f;	e[11] = 0.0f;
		e[12] = 0.0f;	e[13] = 0.0f;	e[14] = 0.0f;	e[15] = 1.0f;			
		return *this;
	}
	
	/// Setup scaling matrix.
	Matrix& scale(float sx, float sy, float sz) { identity(); e[0]=sx; e[5]=sy; e[10]=sz; return *this; }
	
	/// Setup translation matrix.
	Matrix& translation(float tx, float ty, float tz) { identity(); e[3]=tx; e[7]=ty; e[11]=tz; return *this; }
	
	/// Setup rotation matrix around the x-axis, where the rotation angle is given in degrees.
	Matrix& rotX(float rx);

	/// Setup rotation matrix around the y-axis, where the rotation angle is given in degrees.
	Matrix& rotY(float ry);

	/// Setup rotation matrix around the z-axis, where the rotation angle is given in degrees.
	Matrix& rotZ(float rz);
	
	/// Addition of two matrices.
	Matrix operator+(const Matrix& m) const { Matrix r; for(int i=0;i<16;i++) r.e[i]=e[i]+m.e[i]; return r; }
			
	/// Addition of two matrices in place.
	Matrix& operator+=(const Matrix& m) { for(int i=0;i<16;i++) e[i]+=m.e[i]; return *this; }

	/// Subtraction of two matrices.
	Matrix operator-(const Matrix& m) const { Matrix r; for(int i=0;i<16;i++) r.e[i]=e[i]-m.e[i]; return r; }
	
	/// Subtraction of two matrices in place.
	Matrix& operator-=(const Matrix& m) { for(int i=0;i<16;i++) e[i]-=m.e[i]; return *this; }
	
	/// Multiplication of sclar by matrix.
	friend Matrix operator*(float s, const Matrix& m) { Matrix r; for(int i=0;i<16;i++) r.e[i]=s*m.e[i]; return r; }
	
	/// Multiplication of matrix by scalar.
	Matrix operator*(float s) const { Matrix r; for(int i=0;i<16;i++) r.e[i]=s*e[i]; return r; }
	
	/// Multiplication by scalar in place.
	Matrix& operator*=(float s) { for(int i=0;i<16;i++) e[i]*=s; return *this; }
	
	/// Matrix/point multiplication. The point is translated by the last column.
	Point3D operator*(const Point3D& p) const;

	/// Matrix/vector multiplication. The vector is not translated by the last column.
	Vector3D operator*(const Vector3D& v) const;

	/// Matrix multiplication.
	Matrix operator*(const Matrix& m) const
	{
		Matrix r(0);
		for(int i=0; i<4; i++)
			for(int j=0; j<4; j++)
				for(int k=0; k<4; k++)
					r(i,j) += (*this)(i,k) * m(k,j);
		return r;
	}
	
	/// Matrix multiplication in place.
	Matrix& operator*=(const Matrix& m)
	{
		Matrix r(0);
		for(int i=0; i<4; i++)
			for(int j=0; j<4; j++)
				for(int k=0; k<4; k++)
					r(i,j) += (*this)(i,k) * m(k,j);
		return (*this = r);
	}

	/// Compute the determinant of the matrix.
	float determinant() const;
	
	/// Compute the inverse of the matrix.
	Matrix inverse() const;

	/// Compute the transpose of the matrix.
	Matrix transpose() const;

	inline float getNumIndex(unsigned int index) { return e[index]; }

	/// Write matrix elements to an output stream.
	friend std::ostream& operator<<(std::ostream& os, const Matrix& A);
			
protected:
	float e[16];			///< The matrix elements.

	friend class Vector3D;
	friend class Point3D;
};

/**
 * Class representing a 3D vector.
 */
class Vector3D
{
public:
	float x;		///< First component.
	float y;		///< Second component.
	float z;		///< Third component.
		
public:
	/// Default constructor. Initializes vector to (0,0,0).
	Vector3D() : x(0.0f), y(0.0f), z(0.0f) { }

	/// Constructor initializing vector to (x,y,0).
	Vector3D(Vector2D &v);

	/// Constructor initializing all elements to the given value.
	explicit Vector3D(float s) : x(s), y(s), z(s) { }

	/// Constructor initializing vector to (x,y,z).
	Vector3D(float _x, float _y, float _z) : x(_x), y(_y), z(_z) { }

	/// Constructor initializing the vector from a point.
	Vector3D(const Point3D& p);
	
	/// Returns a copy of the element at position (i).
	/// @remark The range of the parameter is not checked.
	float operator()(int i) const { return (&x)[i]; }
	
	/// Returns a reference to the element at position (i).
	/// @remark The range of the parameter is not checked.
	float& operator()(int i) { return (&x)[i]; }
	
	/// Addition of two vectors.
	Vector3D operator+(const Vector3D& v) const { return Vector3D(x+v.x, y+v.y, z+v.z); }

	/// Addition of two vectors in place.
	Vector3D& operator+=(const Vector3D& v) { x+=v.x; y+=v.y; z+=v.z; return *this; }
	
	/// Negation of vector.
	Vector3D operator-() const { return Vector3D(-x,-y,-z); }
	
	/// Subtraction of two vectors.
	Vector3D operator-(const Vector3D& v) const { return Vector3D(x-v.x, y-v.y, z-v.z); }

	/// Subtraction of two vectors in place.
	Vector3D& operator-=(const Vector3D& v) { x-=v.x; y-=v.y; z-=v.z; return *this; }
	
	/// Multiplication of scalar by vector.
	friend Vector3D operator*(float s, const Vector3D& v) { return Vector3D(s*v.x,s*v.y,s*v.z); }
	
	/// Multiplication of vector by scalar.
	Vector3D operator*(float s) const { return Vector3D(s*x,s*y,s*z); }
	
	/// Multiplication by scalar in place.
	Vector3D& operator*=(float s) { x*=s; y*=s; z*=s; return *this; }

	/// Division of vector by scalar.
	Vector3D operator/(float s) const { s=1.0f/s; return Vector3D(s*x,s*y,s*z); }

	/// Division by scalar in place.
	Vector3D& operator/=(float s) { s=1.0f/s; x*=s; y*=s; z*=s; return *this; }
	
	/// Division of scalar by vector.
	friend Vector3D operator/(float s, const Vector3D& v) { return Vector3D(s/v.x,s/v.y,s/v.z); }
		
	/// Dot product of two vectors.
	float dot(const Vector3D& v) const { return x*v.x+y*v.y+z*v.z; }

	/// Dot product of two vectors (short form).
	float operator*(const Vector3D& v) const { return x*v.x+y*v.y+z*v.z; }
	
	/// Cross product of two vectors.
	Vector3D cross(const Vector3D& v) const { return Vector3D(y*v.z-z*v.y, z*v.x-x*v.z, x*v.y-y*v.x); }
	
	/// Cross product of two vectors (short form).
	Vector3D operator%(const Vector3D& v) const { return Vector3D(y*v.z-z*v.y, z*v.x-x*v.z, x*v.y-y*v.x); }
	
	/// Returns length of vector.
	float length() const { return std::sqrt(x*x+y*y+z*z); }

	/// Returns square length of vector.
	float length2() const { return x*x+y*y+z*z; }

	/// Normalize vector to unit length.
	Vector3D& normalize() { float l=1.0f/length(); x*=l; y*=l; z*=l; return *this; }
	
	/// Write elements to an output stream.
	friend std::ostream& operator<<(std::ostream& os, const Vector3D& A);
};


/**
 * Class representing a 2D vector.
 */
class Vector2D
{
public:
	float x;		///< First component.
	float y;		///< Second component.
		
public:
	/// Default constructor. Initializes vector to (0,0).
	Vector2D() : x(0.0f), y(0.0f) { }

	/// Constructor initializing vector to (x,y).
	Vector2D(const Vector2D &v) : x(v.x), y(v.y) { }

	/// Constructor initializing all elements to the given value.
	explicit Vector2D(float s) : x(s), y(s) { }

	/// Constructor initializing vector to (x,y).
	Vector2D(float _x, float _y) : x(_x), y(_y) { }

	/// Constructor initializing the vector from a point.
//	Vector2D(const Point3D& p);
	
	/// Returns a copy of the element at position (i).
	/// @remark The range of the parameter is not checked.
	float operator()(int i) const { return (&x)[i]; }
	
	/// Returns a reference to the element at position (i).
	/// @remark The range of the parameter is not checked.
	float& operator()(int i) { return (&x)[i]; }
	
	/// Addition of two vectors.
	Vector2D operator+(const Vector2D& v) const { return Vector2D(x+v.x, y+v.y); }

	/// Addition of two vectors in place.
	Vector2D& operator+=(const Vector2D& v) { x+=v.x; y+=v.y; return *this; }
	
	/// Negation of vector.
	Vector2D operator-() const { return Vector2D(-x,-y); }
	
	/// Subtraction of two vectors.
	Vector2D operator-(const Vector2D& v) const { return Vector2D(x-v.x, y-v.y); }

	/// Subtraction of two vectors in place.
	Vector2D& operator-=(const Vector2D& v) { x-=v.x; y-=v.y; return *this; }
	
	/// Subtraction of scalar.
	Vector2D operator-(const float& s) const { return Vector2D(x-s, y-s); }

	/// Subtraction of scalar in place.
	Vector2D& operator-=(const float& s) { x-=s; y-=s; return *this; }

	/// Multiplication of scalar by vector.
	friend Vector2D operator*(float s, const Vector2D& v) { return Vector2D(s*v.x,s*v.y); }
	
	/// Multiplication of vector by scalar.
	Vector2D operator*(float s) const { return Vector2D(s*x,s*y); }
	
	/// Multiplication by scalar in place.
	Vector2D& operator*=(float s) { x*=s; y*=s; return *this; }

	/// Division of vector by scalar.
	Vector2D operator/(float s) const { s=1.0f/s; return Vector2D(s*x,s*y); }

	/// Division by scalar in place.
	Vector2D& operator/=(float s) { s=1.0f/s; x*=s; y*=s; return *this; }
	
	/// Division of scalar by vector.
	friend Vector2D operator/(float s, const Vector2D& v) { return Vector2D(s/v.x,s/v.y); }
		
	/// Dot product of two vectors.
	float dot(const Vector2D& v) const { return x*v.x+y*v.y; }

	/// Dot product of two vectors (short form).
	float operator*(const Vector2D& v) const { return x*v.x+y*v.y; }
	
	/// Returns length of vector.
	float length() const { return std::sqrt(x*x+y*y); }

	/// Returns square length of vector.
	float length2() const { return x*x+y*y; }

	/// Normalize vector to unit length.
	Vector2D& normalize() { float l=1.0f/length(); x*=l; y*=l; return *this; }
	
	/// Write elements to an output stream.
	friend std::ostream& operator<<(std::ostream& os, const Vector2D& A);
};


/**
 * Class representing a 3D point. 
 * The main difference between Point3D and Vector3D is how they are transformed
 * by a transformation matrix. The point is assumed to have the 4th component
 * set to 1, so that it is affected by translations, while the vector just
 * represents a direction and hence is not affected by translations.
 */
class Point3D
{
public:
	float x;		///< First component.
	float y;		///< Second component.
	float z;		///< Third component.
	
public:
	/// Default constructor. Initializes point to (0,0,0).
	Point3D() : x(0.0f), y(0.0f), z(0.0f) { }
	
	/// Constructor initializing all elements to the given value.
	explicit Point3D(float s) : x(s), y(s), z(s) { }

	/// Constructor initializing point to (x,y,z).
	Point3D(float _x, float _y, float _z) : x(_x), y(_y), z(_z) { }
	
	/// Returns a copy of the element at position (i).
	/// @remark The range of the parameter is not checked.
	float operator()(int i) const { return (&x)[i]; }

	/// Returns a reference to the element at position (i).
	/// @remark The range of the parameter is not checked.
	float& operator()(int i) { return (&x)[i]; }
		
	/// Addition of point and vector.
	Point3D operator+(const Vector3D& v) const { return Point3D(x+v.x, y+v.y, z+v.z); }

	/// Addition of two points. Not mathematically sensible but convenient to have.
	Point3D operator+(const Point3D& p) const { return Point3D(x+p.x, y+p.y, z+p.z); }

	/// Addition of scalar in place.
	Point3D& operator+=(float s) { x+=s; y+=s; z+=s; return *this; }

	/// Subtraction of point and vector.
	Point3D operator-(const Vector3D& v) const { return Point3D(x-v.x, y-v.y, z-v.z); }
		
	/// Subtraction of two points. Not mathematically sensible but convenient to have.
	Point3D operator-(const Point3D& p) const { return Point3D(x-p.x, y-p.y, z-p.z); }

	/// Subtraction of scalar in place.
	Point3D& operator-=(float s) { x-=s; y-=s; z-=s; return *this; }

	/// Write elements to an output stream.
	friend std::ostream& operator<<(std::ostream& os, const Point3D& A);	
};

/**
 * Class representing a 2D point. 
 * The main difference between Point2D and Vector2D is how they are transformed
 * by a transformation matrix. The point is assumed to have the 4th component
 * set to 1, so that it is affected by translations, while the vector just
 * represents a direction and hence is not affected by translations.
 */
class Point2D
{
public:
	float x;		///< First component.
	float y;		///< Second component.
	
public:
	/// Default constructor. Initializes point to (0,0,0).
	Point2D() : x(0.0f), y(0.0f) { }
	
	/// Constructor initializing all elements to the given value.
	explicit Point2D(float s) : x(s), y(s) { }

	/// Constructor initializing point to (x,y,z).
	Point2D(float _x, float _y) : x(_x), y(_y) { }
	
	/// Returns a copy of the element at position (i).
	/// @remark The range of the parameter is not checked.
	float operator()(int i) const { return (&x)[i]; }

	/// Returns a reference to the element at position (i).
	/// @remark The range of the parameter is not checked.
	float& operator()(int i) { return (&x)[i]; }
		
	/// Addition of point and vector.
	Point2D operator+(const Vector2D& v) const { return Point2D(x+v.x, y+v.y); }

	/// Addition of two points. Not mathematically sensible but convenient to have.
	Point2D operator+(const Point2D& p) const { return Point2D(x+p.x, y+p.y); }

	/// Addition of scalar in place.
	Point2D& operator+=(float s) { x+=s; y+=s; return *this; }

	/// Subtraction of point and vector.
	Point2D operator-(const Vector2D& v) const { return Point2D(x-v.x, y-v.y); }
		
	/// Subtraction of two points. Not mathematically sensible but convenient to have.
	Point2D operator-(const Point2D& p) const { return Point2D(x-p.x, y-p.y); }

	/// Subtraction of scalar in place.
	Point2D& operator-=(float s) { x-=s; y-=s; return *this; }

	/// Write elements to an output stream.
	friend std::ostream& operator<<(std::ostream& os, const Point2D& A);	
};

/**
 * Class representing a texture coordinate (u,v) pair.
 */
class UV
{
public:
	float u;		///< First texture coordinate (u).
	float v;		///< Second texture coordinate (v).
	
public:
	/// Default constructor. Initializes the coordinate to (0,0).
	UV() : u(0), v(0) { }
	
	/// Constructor initializing all elements to the given value.
	explicit UV(float s) : u(s), v(s) { }

	/// Constructor initializing the coordinate to (u,v).
	UV(float _u, float _v) : u(_u), v(_v) { }
	
	/// Addition of two uv coordinates.
	UV operator+(const UV& c) const { return UV(u+c.u, v+c.v); }

	/// Multiplication of scalar by uv coordinate.
	friend UV operator*(float s, const UV& c) { return UV(s*c.u,s*c.v); }
	
	/// Multiplication of uv coordinate by scalar.
	UV operator*(float s) { return UV(s*u,s*v); }	
};

#endif
