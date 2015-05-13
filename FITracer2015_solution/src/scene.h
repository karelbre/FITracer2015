/*
 *  scene.h
 *  asrTracer
 *
 *  Created by Petrik Clarberg on 2006-02-13.
 *  Copyright 2006 Lund University. All rights reserved.
 *
 */

#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include "rayaccelerator.h"
#include "pointlight.h"
#include "triangle.h"
#include "gpu_types.h"

class Node;
class Dummy;
class Camera;
class LightProbe;
class Ray;
class Intersection;
class Intersectable;

/**
 * Class representing all scene data.
 * Scene objects (primitives, light sources, cameras) are added to
 * the scene with the add() function, and internally stored in a scene graph.
 * Before any operations can be performed on the scene, the prepare()
 * function should be called. This function prepares the scene hierarchy
 * for rendering by computing all transform matrices, setting up an
 * acceleration data structure, and caching necessary data.
 * A ray can be intersected tested against the scene by calling the
 * intersect() functions.
 */
class Scene
{
public:
	Scene(RayAccelerator* accelerator);
	~Scene();
	
	void add(Node* node, Node* parent=0);
	void prepare();
	
	void setBackground(const Color& c);
	void setBackground(LightProbe* lp);
	Color getBackground(const Vector3D& d) const;

	// Ray-scene intersection tests
	bool intersect(const Ray& ray);
	bool intersect(const Ray& ray, Intersection& is);

	/// Returns the number of cameras in the scene.
	int getNumberOfCameras() const { return (int)mCameras.size(); }
	
	/// Returns a pointer to camera number i (starting at 0).
	Camera* getCamera(int i) const { return mCameras.at(i); }
	
	/// Returns the number of lights (PointLight) in the scene.
	int getNumberOfLights() const { return (int)mPLights.size(); }
	
	/// Returns a pointer to light number i (starting at 0).
	PointLight* getLight(int i) const { return mPLights.at(i); }

	//Set new accelerate data structure
	void setAccelerator(RayAccelerator* accelerator) { 
		if (mAccelerator != NULL)
			delete mAccelerator;
		mAccelerator = accelerator; 
	}

	RayAccelerator* getAccelerator() { return mAccelerator; }

	void rebuild();

	void getObjects(std::vector<TSphere>* sp_obj, std::vector<TTriangle>* tr_obj, 
		std::vector<TMesh>* meshes, std::vector<unsigned int>* size_meshes);

	void getLights(std::vector<TLight>* lights);

private:
	void setupTransform(Node* node, const Matrix& parent);
	void prepareNode(Node* node);
	void extractData(Node* node, std::vector<Intersectable*>& geometry);

private:
	Node* mRoot;							///< Ptr to root node in the scene hierarchy.
	std::vector<Camera*> mCameras;			///< Array of ptrs to cameras in the scene.
	std::vector<PointLight*> mPLights;		///< Array of ptrs to lights in the scene.
	Color mBackgroundColor;					///< Background color to use if not using light probe.
	LightProbe* mBackgroundProbe;			///< Ptr to light probe or 0 if none.
	RayAccelerator* mAccelerator;		///< kD-tree accelerator structure.
};

#endif
