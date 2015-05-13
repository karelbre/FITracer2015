/*
	Name: bvhaccelerator.cpp
	Desc: BVH accelerated data structure.
	Author: FredrikAppelros on GitHub
*/

#ifndef BVHACCELERATOR_H
#define BVHACCELERATOR_H

#include "rayaccelerator.h"
#include <stack>

class BVHAccelerator : public RayAccelerator
{
private:
	class BVHNode {
	private:
		AABB bbox;
		bool leaf;
		unsigned int n_objs;
		unsigned int index;

	public:
		void setAABB(AABB &bbox_) { bbox = bbox_; }
		void makeLeaf(unsigned int index_, unsigned int n_objs_) { 
			leaf = true;
			index = index_; 
			n_objs = n_objs_; 
		}
		void makeNode(unsigned int left_index_, unsigned int n_objs_) { 
			leaf = false;
			index = left_index_; 
			n_objs = n_objs_;
		}

		bool isLeaf() { return leaf; }
		unsigned int getIndex() { return index; }
		unsigned int getNObjs() { return n_objs; }
		AABB& getAABB() { return bbox; }
	};

	std::vector<Intersectable*> c_objects;
	std::vector<BVHNode> nodes;
	void build_recursive(int left_index, int right_index, BVHNode *node, int depth);

public:
	virtual void build(const std::vector<Intersectable*>& objects);
	virtual bool intersect(const Ray& ray);
	virtual bool intersect(const Ray& ray, Intersection& is);

	virtual std::vector<Intersectable*> getObjects() { return c_objects; }
	void getNodes(TBVHNode& node, unsigned int index, unsigned int& leftID, unsigned int& rightID) { 
		if (index < nodes.size()) {
			AABB box = nodes[index].getAABB();
			Point3DtoFloat3(box.mMin, node.boxMin);
			Point3DtoFloat3(box.mMax, node.boxMax);
			node.leaf = nodes[index].isLeaf();
			node.indexNode = nodes[index].getIndex();
			leftID = nodes[index].getIndex();
			rightID = nodes[index].getNObjs();
		}
	}
	unsigned int getNodesCnt() { return nodes.size(); }
};

#endif