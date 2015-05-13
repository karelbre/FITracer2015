/*
 *  triangle.h
 *  asrTracer
 *
 *  Created by Petrik Clarberg on 2006-02-22.
 *  Copyright 2006 Lund University. All rights reserved.
 *
 */

#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "intersectable.h"
#include "gpu_types.h"
#include "mesh.h"

//class Mesh;

/**
 * Class representing a single triangle. 
 * This class is used by Mesh to represent the triangles in the mesh.
 * It has functions for computing the triangle's face normal, area,
 * and for intersection testing with a ray.
 */
class Triangle : public Intersectable
{	
public:
	/// \cond INTERNAL_CLASS
	
	/// Internal class representing the vertex indices.
	struct vertex
	{
		int p;		///< Position index.
		int n;		///< Normal index.
		int t;		///< Auxiliary index (texture coordinate).
	};
	
	/// \endcond // INTERNAL_CLASS

	Triangle();
	Triangle(Mesh* owner, const vertex& vtx1, const vertex& vtx2, const vertex& vtx3, Material *mtl);
	
	Vector3D getFaceNormal() const;
	float getArea() const;
	void prepare();

	// Implementation of the Intersectable interface:
	bool intersect(const Ray& ray) const;
	bool intersect(const Ray& ray, Intersection& isect) const;
	void getAABB(AABB& bb) const;
	UV calculateTextureDifferential(const Point3D& p, const Vector3D& dp) const;
	Vector3D calculateNormalDifferential(const Point3D& p, const Vector3D& dp, bool isFrontFacing) const;

	const Point3D& getVtxPosition(int i) const;
	const Vector3D& getVtxNormal(int i) const;
	const UV& getVtxTexture(int i) const;
	Material *getMaterial() const {return mMaterial;}

	bool isSphere() const { return false; }

	void setObj(void* obj) {
		setMaterial(((TTriangle*)obj)->material);
		Vector3DToFloat3(mPlaneOffsets, ((TTriangle*)obj)->planeOffsets);
		Vector3DToFloat3(mPlanes[0], ((TTriangle*)obj)->planes[0]);
		Vector3DToFloat3(mPlanes[1], ((TTriangle*)obj)->planes[1]);
		Vector3DToFloat3(mPlanes[2], ((TTriangle*)obj)->planes[2]);
		setVertex(((TTriangle*)obj)->vtx);
	}
	
	void getMesh(std::vector<TMesh>* meshes) {
		mMesh->getMesh(meshes);
	}

	Mesh* getMesh() { return mMesh; }

	void getTriangle(TTriangle& triangle) {
		setMaterial(triangle.material);
		Vector3DToFloat3(mPlaneOffsets, triangle.planeOffsets);
		Vector3DToFloat3(mPlanes[0], triangle.planes[0]);
		Vector3DToFloat3(mPlanes[1], triangle.planes[1]);
		Vector3DToFloat3(mPlanes[2], triangle.planes[2]);
		setVertex(triangle.vtx);
	}

	unsigned int getIndex() { return index; }

	unsigned int setIndex(unsigned int _index)
	{
		index = _index;
		return index + 1;
	}

protected:

	void setMaterial(TMaterial& mat) {
		if (mMaterial != NULL) {
			Color col = ((Diffuse*)this)->getColor();
			setColor(mat.color, col.r, col.g, col.b);
			mMaterial->getMaterial(mat);
			mat.isMat = true;
		}
		else {
			Diffuse* prim = (Diffuse*)mMesh->mMaterial;
			prim->getColor(mat.color);
			prim->getMaterial(mat);
			mat.isMat = true;
		}
	}

	void setVertex(TVertex* ver) {
		for (int i = 0; i < 3; i++) {
			ver[i].n = mVtx[i].n;
			ver[i].p = mVtx[i].p;
			ver[i].t = mVtx[i].t;
		}
	}

	Vector3D mPlanes[3];
	Vector3D mPlaneOffsets;
	Mesh* mMesh;				///< Ptr to the mesh this triangle belongs to.
	vertex mVtx[3];				///< The three vertices of the triangle.	
	Material *mMaterial;		///< Material of the triangle.

	friend class Mesh;
};

#endif
