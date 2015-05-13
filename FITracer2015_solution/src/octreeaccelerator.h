/*
	Name: octreeaccelerator.h
	Desc: Octree accelerated data structure.
	Author: Karel Brezina (xbrezi13)
*/

#ifndef _OCTREE_H_
#define _OCTREE_H_

#include "rayaccelerator.h"
#include "matrix.h"

// Octree has 8 leafs.
#define MAX_CELLS 8
// Max object in octree box.
#define MAX_OBJECTS 3
// Max depth of octree.
#define MAX_DEPTH 7

class OctreeAccelerator : public RayAccelerator {
public:
	OctreeAccelerator() { 
		for (int i = 0; i < MAX_CELLS; i++) 
			child[i] = NULL; 

		level = 0;
		leaf = false;
	}
	~OctreeAccelerator() {}
	void pushObject(Intersectable* obj);
	void setLevel(int lvl) { level = lvl; }
	int getLevel() { return level; }
	void insertObject(Point3D point, Intersectable* obj, std::vector<int>& inserted);

	AABB getBox() { return box; }
	Point3D getMiddlePoint() { return middlePoint; }
	bool isLeaf() { return leaf; }
	OctreeAccelerator* getChild(unsigned int i) {
		if (i < 8) {
			return child[i];
		}
		return NULL;
	}

	virtual void build(const std::vector<Intersectable*>& objects);
	virtual bool intersect(const Ray& ray);
	virtual bool intersect(const Ray& ray, Intersection& is);
	virtual std::vector<Intersectable*> getObjects() { return c_objects; }

private:
	bool ProcessSubNode(const Ray& ray, unsigned char flag,
		float tx0, float ty0, float tz0, float tx1, float ty1, float tz1);
	bool ProcessSubNode(const Ray& ray, Intersection& is, unsigned char flag,
						float tx0, float ty0, float tz0, float tx1, float ty1, float tz1);
	unsigned int GetFirstNode(float tx0, float ty0, float tz0, float txm, float tym, float tzm, unsigned char rayFlags);
	unsigned int GetNextNode(unsigned char currentNode, float tx1, float ty1, float tz1);

	int OctreeAccelerator::getSector(const Point3D& aabb, const Point3D& middlePoint);
	void initBoundaries();
	void recursiveBuild();

	AABB box;
	Point3D middlePoint;
	int level;
	std::vector<Intersectable*> c_objects;
	OctreeAccelerator* child[8];
	bool leaf;
};

#endif // _OCTREE_H_