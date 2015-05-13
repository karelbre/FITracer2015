/*
	Name: bvhaccelerator.cpp
	Desc: BVH accelerated data structure.
	Author: FredrikAppelros on GitHub
*/

#include "bvhaccelerator.h"
#include <algorithm>
#include <iomanip>

void BVHAccelerator::build(const std::vector<Intersectable*>& objects) {

	c_objects = objects;
	nodes.reserve(2 * objects.size() - 1);
	BVHNode root;
	AABB& worldBox = root.getAABB();

	std::for_each(c_objects.begin(), c_objects.end(), [&](Intersectable* obj) {
		AABB aabb;
		obj->getAABB(aabb);
		worldBox.include(aabb);
	});
	nodes.push_back(root);
	build_recursive(0, c_objects.size(), &nodes[0], 0);

}

void BVHAccelerator::build_recursive(int left_index, int right_index, BVHNode *node, int depth) {
	static int MAX_NUM_PRIMITIVES_IN_LEAF = 4;
	static int MAX_DEPTH = 20;

	int n_objs = right_index - left_index;
	if (n_objs <= MAX_NUM_PRIMITIVES_IN_LEAF || depth == MAX_DEPTH) {
		node->makeLeaf(left_index, n_objs);
	}
	else {
		// Sort the objects along the largest axis
		AABB& node_box = node->getAABB();
		int sort_dim = node_box.getLargestAxis();
		std::sort(c_objects.begin() + left_index, c_objects.begin() + right_index, [&](Intersectable* a, Intersectable* b) -> bool {
			AABB box;
			a->getAABB(box);
			float ca = (box.mMax(sort_dim) + box.mMin(sort_dim)) * 0.5f;
			b->getAABB(box);
			float cb = (box.mMax(sort_dim) + box.mMin(sort_dim)) * 0.5f;
			return ca < cb;
		});

		// Find split index
		float mid = (node_box.mMax(sort_dim) + node_box.mMin(sort_dim)) * 0.5f;
		int split_index = left_index;
		AABB box;
		float cb;
		do {
			c_objects[split_index++]->getAABB(box);
			cb = (box.mMax(sort_dim) + box.mMin(sort_dim)) * 0.5f;
		} while (cb <= mid && split_index < right_index);
		split_index--;
		if (split_index == left_index || split_index == right_index) {
			split_index = static_cast<int>((left_index + right_index - 1) * 0.5f + 0.5f);
		}

		// Create left and right nodes with AABB
		BVHNode left_node;
		BVHNode right_node;
		AABB& left_box = left_node.getAABB();
		AABB& right_box = right_node.getAABB();
		std::for_each(c_objects.begin() + left_index, c_objects.begin() + split_index, [&](Intersectable* obj) {
			AABB aabb;
			obj->getAABB(aabb);
			left_box.include(aabb);
		});
		std::for_each(c_objects.begin() + split_index, c_objects.begin() + right_index, [&](Intersectable* obj) {
			AABB aabb;
			obj->getAABB(aabb);
			right_box.include(aabb);
		});
		int n_nodes = nodes.size();
		nodes.push_back(left_node);
		nodes.push_back(right_node);

		// Initiate current node as interior node
		node->makeNode(n_nodes, right_index - left_index);

		// Recurse
		build_recursive(left_index, split_index, &nodes[n_nodes], depth + 1);
		build_recursive(split_index, right_index, &nodes[n_nodes + 1], depth + 1);
	}
}

bool BVHAccelerator::intersect(const Ray& ray)
{
	BVHNode* currentNode = &nodes[0];
	AABB& box = currentNode->getAABB();
	float minT, maxT;
	if (!box.intersect(ray, minT, maxT))
		return false;
	std::stack<BVHNode> intersect_stack;
	while (true) {
		if (currentNode->isLeaf()) {
			for (unsigned int i = currentNode->getIndex(); i < currentNode->getIndex() + currentNode->getNObjs(); ++i) {
				if (c_objects[i]->intersect(ray)) {
					return true;
				}
			}
		}
		else {
			BVHNode& left_node = nodes[currentNode->getIndex()];
			BVHNode& right_node = nodes[currentNode->getIndex() + 1];
			AABB& left_box = left_node.getAABB();
			AABB& right_box = right_node.getAABB();

			bool leftHit, rightHit;
			leftHit = left_box.intersect(ray, minT, maxT);
			rightHit = right_box.intersect(ray, minT, maxT);
			if (leftHit && rightHit) {
				currentNode = &left_node;
				intersect_stack.push(right_node);
				continue;
			}
			else if (leftHit) {
				currentNode = &left_node;
				continue;
			}
			else if (rightHit) {
				currentNode = &right_node;
				continue;
			}
		}
		if (intersect_stack.empty())
			return false;
		currentNode = &intersect_stack.top();
		intersect_stack.pop();
	}
}

bool BVHAccelerator::intersect(const Ray& ray, Intersection& is)
{
	BVHNode* currentNode = &nodes[0];
	AABB& box = currentNode->getAABB();
	float minT, maxT;
	if (!box.intersect(ray, minT, maxT))
		return false;
	bool hit = false;
	Ray localRay = ray;
	std::stack<std::pair<float, BVHNode*>> intersect_stack;
	for (;;) {
		if (currentNode->isLeaf()) {
			for (unsigned int i = currentNode->getIndex(); i < currentNode->getIndex() + currentNode->getNObjs(); ++i) {
				if (c_objects[i]->intersect(localRay, is)) {
					localRay.maxT = is.mHitTime;
					hit = true;
				}
			}
		}
		else {
			BVHNode& left_node = nodes[currentNode->getIndex()];
			BVHNode& right_node = nodes[currentNode->getIndex() + 1];
			AABB& left_box = left_node.getAABB();
			AABB& right_box = right_node.getAABB();

			bool leftHit, rightHit;
			float leftT, rightT;
			leftHit = left_box.intersect(ray, minT, maxT);
			leftT = minT;
			rightHit = right_box.intersect(ray, minT, maxT);
			rightT = minT;
			if (leftHit && rightHit) {
				if (leftT < rightT) {
					currentNode = &left_node;
					intersect_stack.push(std::make_pair(rightT, &right_node));
				}
				else {
					currentNode = &right_node;
					intersect_stack.push(std::make_pair(leftT, &left_node));
				}
				continue;
			}
			else if (leftHit) {
				currentNode = &left_node;
				continue;
			}
			else if (rightHit) {
				currentNode = &right_node;
				continue;
			}
		}

		currentNode = 0;
		std::pair<float, BVHNode*> p;
		while (!intersect_stack.empty()) {
			p = intersect_stack.top();
			intersect_stack.pop();
			if (p.first < localRay.maxT) {
				currentNode = p.second;
				break;
			}
		}
		if (currentNode == 0)
			return hit;
	}
}