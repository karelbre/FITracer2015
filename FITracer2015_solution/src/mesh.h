/*
 *  mesh.h
 *  asrTracer
 *
 *  Created by Petrik Clarberg on 2006-02-14.
 *  Copyright 2006 Lund University. All rights reserved.
 *
 */

#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector>
//#include "triangle.h"
#include "primitive.h"
#include "gpu_types.h"

class Triangle;

struct MaterialProperties {	
	Color ambient;
	bool hasAmbient;

	Color diffuse;
	bool hasDiffuse;

	Color specular;
	bool hasSpecular;

	float shininess;
	bool hasShininess;

	float glossy;
	bool hasGlossy;

	float transparency;
	float refractionIndex;
	float reflectivity;

	void reset() {
		ambient = Color(0.5f, 0.0f, 0.0f);
		hasAmbient = false;
		diffuse = Color(0.5f, 0.5f, 0.5f);
		hasDiffuse = false;
		specular = Color(1.0f, 0.0f, 0.0f);
		hasSpecular = false;
		shininess = 0.25f;
		hasShininess = false;
		glossy = 1.0f;
		hasGlossy = false;
		transparency = 0.0f;
		refractionIndex = 1.0f;
		reflectivity = 0.0f;
	}
};

/** 
 * Class representing a triangle mesh.
 * The mesh can be loaded from an WaveFront (.obj) mesh file.
 * Internally, the mesh's vertex positions, normals, and texture
 * coordinates are stored in separate vectors. Each triangle has
 * three sets of indices into these vectors. If the normals are
 * not specified in the obj-file, these are computed by area-weighting
 * the face normals.
 */
class Mesh : public Primitive
{
public:
	Mesh();
	Mesh(const std::string& filename, Material* m=0);
	void load(const std::string& filename);

	// TEMP TEMP - Should be protected
	void getGeometry(std::vector<Intersectable*>& geometry);
	void getMesh(std::vector<TMesh>* mesh);

	unsigned int setIndexes(unsigned int index);
protected:
	void prepare();
	void clear();
	void loadOBJ(const std::string& filename);

	bool loadMTL(const std::string& filename);
	Material *CreateMaterial(MaterialProperties &mp) const;

protected:
	std::vector<Point3D> mOrigVtxP;		///< Array of original vertex positions.
	std::vector<Vector3D> mOrigVtxN;	///< Array of original vertex normals.

	std::vector<Point3D> mVtxP;			///< Array of vertex positions.
	std::vector<Vector3D> mVtxN;		///< Array of vertex normals.
	std::vector<UV> mVtxUV;				///< Array of vertex UV coordinates.
	std::vector<Triangle> mFaces;		///< Array of triangles.
	std::vector<Material *> mMaterials;	///< Array of materials.
	
	friend class Triangle;				// Triangle is a friend class so it can access protected data.
};

#endif
