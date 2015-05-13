/*
	Name: uniformaccelerator.cpp
	Desc: Uniform grid accelerated data structure.
	Author: Karel Brezina (xbrezi13)
*/

#ifndef _UNIFORM_GRID_H_
#define _UNIFORM_GRID_H_

#include "rayaccelerator.h"
#include "matrix.h"

#define GRID_SIZE 120

class UniNode {
public:
	UniNode() { obj_index = 0; next = NULL; }
	UniNode(int i) { obj_index = i; next = NULL; }
	inline void setObject(int index) { obj_index = index; }
	inline int getObject() { return obj_index; }
	inline void setNext(int obj_index) { this->next = new UniNode(obj_index); }
	inline UniNode* getNext() { return next; }
	inline int getIndex() { return obj_index; }

private:
	int obj_index;
	UniNode* next;
};

class UniformAccelerator : public RayAccelerator {
public:
	virtual void build(const std::vector<Intersectable*>& objects);
	virtual bool intersect(const Ray& ray);
	virtual bool intersect(const Ray& ray, Intersection& is);

	void insertObject(Point3D point_max, Point3D point_min, Intersectable* obj, int index);

	virtual std::vector<Intersectable*> getObjects() { return c_objects; }

	AABB getBox() { return box; }
	Point3D getWorldSize() { return world_size; }
	Point3D getCellsize() { return cell_size; }
	UniNode** getVoxels() { return voxels; }

private:
	AABB box;
	Point3D world_size;
	Point3D cell_size;
	std::vector<Intersectable*> c_objects;
	UniNode** voxels;
};

#endif // _UNIFORM_GRID_H_