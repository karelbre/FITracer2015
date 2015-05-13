/*
	Name: kernel_trace_octree.h
	Desc: Trace and intersection functions for OCTREE PT.
	Author: Karel Brezina (xbrezi13)
*/

#ifndef _KERNEL_TRACE_OCTREE_H_
#define _KERNEL_TRACE_OCTREE_H_

#include "kernel_types.h"
#include "kernel_functions.h"

unsigned char GetFirstNode(float tx0, float ty0, float tz0, float txm, float tym, float tzm, unsigned char rayFlags)
{
	unsigned int entryPlane = 0;
	//Get entry plane

	if (tx0 > ty0){
		if (tx0 > tz0)
		{
			if (tym < tx0) entryPlane |= 2;
			if (tzm < tx0) entryPlane |= 1;
			return entryPlane;
		}
	}
	else if (ty0 > tz0){
		if (txm < ty0) entryPlane |= 4;
		if (tzm < ty0) entryPlane |= 1;
		return entryPlane;
	}

	if (txm < tz0) entryPlane |= 4;
	if (tym < tz0) entryPlane |= 2;

	return entryPlane; //returns the first node
}

unsigned char GetNextNode(unsigned char currentNode, float tx1, float ty1, float tz1, unsigned char* nextNodeTable)
{
	//Get exit plane
	float min;
	int minIndex = 0;
	if (tx1 < ty1){
		min = tx1;
		minIndex = 0;
	}
	else {
		minIndex = 1;
		min = ty1;
	}
	if (tz1 < min){
		minIndex = 2;
	}

	//look up next node 
	return nextNodeTable[(currentNode * 3) + minIndex];
}

bool ProcessSubNode(TRay* ray, __global TSphere* sp, unsigned int cnt_sp,
	__global TTriangle* tr, unsigned int cnt_tr, __global TMesh* me,
	__global unsigned int* ra_me, unsigned int cnt_ra_me,
	__global TOctreeBox* octree, unsigned int octree_size, __global TObject* objects,
	unsigned int actualOctree, 
	unsigned char flag, float tx0, float ty0, float tz0, float tx1, float ty1, float tz1,
	unsigned char* nextNodeTable)
{
	unsigned char currentNode = 0;
	float txM, tyM, tzM;
	OctreeState savedState;
	bool isNodeInit = false;
	OctreeState stackStates[100];
	int stackID = 0;

	do
	{
		if (tx1 < 0 || ty1 < 0 || tz1 < 0) {
			stackID -= 1;
			if (stackID < 0) {
				break;
			}
			savedState = stackStates[stackID];
			tx0 = savedState.tx0;
			ty0 = savedState.ty0;
			tz0 = savedState.tz0;
			txM = savedState.txM;
			tyM = savedState.tyM;
			tzM = savedState.tzM;
			tx1 = savedState.tx1;
			ty1 = savedState.ty1;
			tz1 = savedState.tz1;
			currentNode = savedState.currentNode;
			actualOctree = savedState.actualOctree;
			isNodeInit = savedState.isNodeInit;
		}

		if (octree[actualOctree].leaf) {
			TSphere sphere;
			TTriangle triangle;

			for (int i = octree[actualOctree].boxLink.objStartIndex; i < octree[actualOctree].boxLink.objSize; i++) {

				if (objects[i].type == SPHERE_INDEX) {
					sphere = sp[objects[i].index];

					if (sphereIntersect(&sphere, ray)) {
						return true;
					}
				}
				else {
					triangle = tr[objects[i].index];

					if (triangleIntersect(&triangle, ray, me, ra_me, cnt_ra_me)) {
						return true;
					}
				}
			}
			stackID -= 1;
			if (stackID < 0) {
				break;
			}
			savedState = stackStates[stackID];
			tx0 = savedState.tx0;
			ty0 = savedState.ty0;
			tz0 = savedState.tz0;
			txM = savedState.txM;
			tyM = savedState.tyM;
			tzM = savedState.tzM;
			tx1 = savedState.tx1;
			ty1 = savedState.ty1;
			tz1 = savedState.tz1;
			currentNode = savedState.currentNode;
			actualOctree = savedState.actualOctree;
			isNodeInit = savedState.isNodeInit;
		}

		txM = 0.5f * (tx0 + tx1);
		tyM = 0.5f * (ty0 + ty1);
		tzM = 0.5f * (tz0 + tz1);

		if (!isNodeInit) {
			currentNode = GetFirstNode(tx0, ty0, tz0, txM, tyM, tzM, flag);
			isNodeInit = true;
		}

		savedState.tx0 = tx0;
		savedState.ty0 = ty0;
		savedState.tz0 = tz0;
		savedState.txM = txM;
		savedState.tyM = tyM;
		savedState.tzM = tzM;
		savedState.tx1 = tx1;
		savedState.ty1 = ty1;
		savedState.tz1 = tz1;
		savedState.actualOctree = actualOctree;
		savedState.isNodeInit = isNodeInit;
		isNodeInit = false;

		switch (currentNode)
		{
		case 0:
			actualOctree = octree[actualOctree].children[flag];
			savedState.currentNode = GetNextNode(currentNode, txM, tyM, tzM, nextNodeTable);
			tx1 = txM;
			ty1 = tyM;
			tz1 = tzM;
			break;

		case 1:
			actualOctree = octree[actualOctree].children[flag ^ 1];
			savedState.currentNode = GetNextNode(currentNode, txM, tyM, tz1, nextNodeTable);
			tz0 = tzM;
			tx1 = txM;
			ty1 = tyM;
			break;

		case 2:
			actualOctree = octree[actualOctree].children[flag ^ 2];
			savedState.currentNode = GetNextNode(currentNode, txM, ty1, tzM, nextNodeTable);
			ty0 = tyM;
			tx1 = txM;
			tz1 = tzM;
			break;

		case 3:
			actualOctree = octree[actualOctree].children[flag ^ 3];
			savedState.currentNode = GetNextNode(currentNode, txM, ty1, tz1, nextNodeTable);
			ty0 = tyM;
			tz0 = tzM;
			tx1 = txM;
			break;

		case 4:
			actualOctree = octree[actualOctree].children[flag ^ 4];
			savedState.currentNode = GetNextNode(currentNode, tx1, tyM, tzM, nextNodeTable);
			tx0 = txM;
			ty1 = tyM;
			tz1 = tzM;
			break;

		case 5:
			actualOctree = octree[actualOctree].children[flag ^ 5];
			savedState.currentNode = GetNextNode(currentNode, tx1, tyM, tz1, nextNodeTable);
			tx0 = txM;
			tz0 = tzM;
			ty1 = tyM;
			break;

		case 6:
			actualOctree = octree[actualOctree].children[flag ^ 6];
			savedState.currentNode = GetNextNode(currentNode, tx1, ty1, tzM, nextNodeTable);
			tx0 = txM;
			ty0 = tyM;
			tz1 = tzM;
			break;

		case 7:
			actualOctree = octree[actualOctree].children[flag ^ 7];
			savedState.currentNode = 8;        // if we reach the far top right node then there are    
			tx0 = txM;                        // no nodes we can reach from here, given that our ray 
			ty0 = tyM;                       // is always travelling in a positive direction.
			tz0 = tzM;
			break;

		case 8:
			do {
				stackID -= 1;
				if (stackID < 0) {
					return false;
				}
				savedState = stackStates[stackID];
				tx0 = savedState.tx0;
				ty0 = savedState.ty0;
				tz0 = savedState.tz0;
				txM = savedState.txM;
				tyM = savedState.tyM;
				tzM = savedState.tzM;
				tx1 = savedState.tx1;
				ty1 = savedState.ty1;
				tz1 = savedState.tz1;
				currentNode = savedState.currentNode;
				actualOctree = savedState.actualOctree;

				switch (currentNode) {
				case 0:
					actualOctree = octree[actualOctree].children[flag];
					savedState.currentNode = GetNextNode(currentNode, txM, tyM, tzM, nextNodeTable);
					tx1 = txM;
					ty1 = tyM;
					tz1 = tzM;
					break;

				case 1:
					actualOctree = octree[actualOctree].children[flag ^ 1];
					savedState.currentNode = GetNextNode(currentNode, txM, tyM, tz1, nextNodeTable);
					tz0 = tzM;
					tx1 = txM;
					ty1 = tyM;
					break;

				case 2:
					actualOctree = octree[actualOctree].children[flag ^ 2];
					savedState.currentNode = GetNextNode(currentNode, txM, ty1, tzM, nextNodeTable);
					ty0 = tyM;
					tx1 = txM;
					tz1 = tzM;
					break;

				case 3:
					actualOctree = octree[actualOctree].children[flag ^ 3];
					savedState.currentNode = GetNextNode(currentNode, txM, ty1, tz1, nextNodeTable);
					ty0 = tyM;
					tz0 = tzM;
					tx1 = txM;
					break;

				case 4:
					actualOctree = octree[actualOctree].children[flag ^ 4];
					savedState.currentNode = GetNextNode(currentNode, tx1, tyM, tzM, nextNodeTable);
					tx0 = txM;
					ty1 = tyM;
					tz1 = tzM;
					break;

				case 5:
					actualOctree = octree[actualOctree].children[flag ^ 5];
					savedState.currentNode = GetNextNode(currentNode, tx1, tyM, tz1, nextNodeTable);
					tx0 = txM;
					tz0 = tzM;
					ty1 = tyM;
					break;

				case 6:
					actualOctree = octree[actualOctree].children[flag ^ 6];
					savedState.currentNode = GetNextNode(currentNode, tx1, ty1, tzM, nextNodeTable);
					tx0 = txM;
					ty0 = tyM;
					tz1 = tzM;
					break;

				case 7:
					actualOctree = octree[actualOctree].children[flag ^ 7];
					savedState.currentNode = 8;        // if we reach the far top right node then there are    
					tx0 = txM;                        // no nodes we can reach from here, given that our ray 
					ty0 = tyM;                       // is always travelling in a positive direction.
					tz0 = tzM;
					break;
				}
			} while (currentNode == 8);

			break;
		}

		stackStates[stackID] = savedState;
		stackID++;
	} while (stackID >= 0);

	return false;
}

// Intersect object without information about it.
// ray -> information about ray
// sp -> buffer of all spheres
// tr -> buffer of all triangles
// me -> buffer of all meshes
// ra_me -> buffer of size every mesh
// @return -> successful of intersect 
bool intersect_octree(TRay* ray, __global TSphere* sp, unsigned int cnt_sp,
	__global TTriangle* tr, unsigned int cnt_tr, __global TMesh* me,
	__global unsigned int* ra_me, unsigned int cnt_ra_me,
	__global TOctreeBox* octree, unsigned int octree_size, __global TObject* objects,
	unsigned int actualOctree, unsigned char* nextNodeTable)
{
	unsigned char flag = 0;
	TPoint3D halfSize = PointMinusPoint(octree[actualOctree].boxMax, octree[actualOctree].middlePoint);
	TVector3D dir = ray->dir;
	TPoint3D origin = PointPlusPoint(PointMinusPoint(ray->orig, octree[actualOctree].middlePoint), halfSize);
	TPoint3D boxSize = PointMinusPoint(octree[actualOctree].boxMax, octree[actualOctree].boxMin);

	if (dir.x < 0.0f) {
		origin.x = boxSize.x - origin.x;
		dir.x *= -1.0f;
		flag |= 4;
	}

	if (dir.y < 0.0f) {
		origin.y = boxSize.y - origin.y;
		dir.y *= -1.0f;
		flag |= 2;
	}

	if (dir.z < 0.0f) {
		origin.z = boxSize.z - origin.z;
		dir.z *= -1.0f;
	}
	else {
		flag |= 1;
	}

	float invDirx = 1.0f / dir.x;
	float invDiry = 1.0f / dir.y;
	float invDirz = 1.0f / dir.z;

	float tx0 = -origin.x * invDirx;
	float tx1 = (boxSize.x - origin.x) * invDirx;

	float ty0 = -origin.y * invDiry;
	float ty1 = (boxSize.y - origin.y) * invDiry;

	float tz0 = -origin.z * invDirz;
	float tz1 = (boxSize.z - origin.z) * invDirz;

	float maxOf0 = 0.f;
	float minOf1 = 0.f;
	// Compute max for maxOf0
	if (tx0 > ty0) {
		if (tx0 > tz0) {
			maxOf0 = tx0;
		}
		else {
			maxOf0 = tz0;
		}
	}
	else {
		if (ty0 > tz0) {
			maxOf0 = ty0;
		}
		else {
			maxOf0 = tz0;
		}
	}
	// Compute min for minOf1
	if (tx1 < ty1) {
		if (tx1 < tz1) {
			minOf1 = tx1;
		}
		else {
			minOf1 = tz1;
		}
	}
	else {
		if (ty1 < tz1) {
			minOf1 = ty1;
		}
		else {
			minOf1 = tz1;
		}
	}

	if (maxOf0 < minOf1) {
		return ProcessSubNode(ray, sp, cnt_sp, tr, cnt_tr, me, ra_me, cnt_ra_me,
			octree, octree_size, objects, 0,
			flag, tx0, ty0, tz0, tx1, ty1, tz1, nextNodeTable);
	}
	return false;
}

bool ProcessSubNodeIs(TRay* ray, TIntersect* is, __global TSphere* sp, unsigned int cnt_sp,
	__global TTriangle* tr, unsigned int cnt_tr, __global TMesh* me,
	__global unsigned int* ra_me, unsigned int cnt_ra_me,
	__global TOctreeBox* octree, unsigned int octree_size, __global TObject* objects,
	unsigned int actualOctree,
	unsigned char flag, float tx0, float ty0, float tz0, float tx1, float ty1, float tz1,
	unsigned char* nextNodeTable)
{
	unsigned char currentNode = 0;
	float txM, tyM, tzM;
	OctreeState savedState;
	bool isNodeInit = false;
	OctreeState stackStates[100];
	int stackID = 0;
	char znak = 0;

	do
	{
		if (tx1 < 0 || ty1 < 0 || tz1 < 0) {
			stackID -= 1;
			if (stackID < 0) {
				break;
			}
			savedState = stackStates[stackID];
			tx0 = savedState.tx0;
			ty0 = savedState.ty0;
			tz0 = savedState.tz0;
			txM = savedState.txM;
			tyM = savedState.tyM;
			tzM = savedState.tzM;
			tx1 = savedState.tx1;
			ty1 = savedState.ty1;
			tz1 = savedState.tz1;
			currentNode = savedState.currentNode;
			actualOctree = savedState.actualOctree;
			isNodeInit = savedState.isNodeInit;
		}

		if (octree[actualOctree].leaf) {
			TIntersect currentIs;
			TSphere sphere;
			TTriangle triangle;

			for (int i = octree[actualOctree].boxLink.objStartIndex; i < octree[actualOctree].boxLink.objSize; i++) {

				if (objects[i].type == SPHERE_INDEX) {
					sphere = sp[objects[i].index];
					
					if (sphereIntersectIs(&sphere, ray, &currentIs)) {
						// Is object near than previous intersected object.
						if (currentIs.hitTime < is->hitTime) {
							*is = currentIs;
							is->obj_index = objects[i].index;
						}
					}
				}
				else {
					triangle = tr[objects[i].index];

					if (triangleIntersectIs(&triangle, ray, &currentIs, me, ra_me, cnt_ra_me)) {
						// Is object near than previous intersected object.
						if (currentIs.hitTime < is->hitTime) {
							*is = currentIs;
							is->obj_index = objects[i].index;
						}
					}
				}
			}
			stackID -= 1;
			if (stackID < 0) {
				break;
			}
			savedState = stackStates[stackID];
			tx0 = savedState.tx0;
			ty0 = savedState.ty0;
			tz0 = savedState.tz0;
			txM = savedState.txM;
			tyM = savedState.tyM;
			tzM = savedState.tzM;
			tx1 = savedState.tx1;
			ty1 = savedState.ty1;
			tz1 = savedState.tz1;
			currentNode = savedState.currentNode;
			actualOctree = savedState.actualOctree;
			isNodeInit = savedState.isNodeInit;
		}

		txM = 0.5f * (tx0 + tx1);
		tyM = 0.5f * (ty0 + ty1);
		tzM = 0.5f * (tz0 + tz1);

		if (!isNodeInit) {
			currentNode = GetFirstNode(tx0, ty0, tz0, txM, tyM, tzM, flag);
			isNodeInit = true;
		}

		savedState.tx0 = tx0;
		savedState.ty0 = ty0;
		savedState.tz0 = tz0;
		savedState.txM = txM;
		savedState.tyM = tyM;
		savedState.tzM = tzM;
		savedState.tx1 = tx1;
		savedState.ty1 = ty1;
		savedState.tz1 = tz1;
		savedState.actualOctree = actualOctree;
		savedState.isNodeInit = isNodeInit;
		isNodeInit = false;

		switch (currentNode)
		{
		case 0:
			znak = flag;
			actualOctree = octree[actualOctree].children[flag];
			savedState.currentNode = GetNextNode(currentNode, txM, tyM, tzM, nextNodeTable);
			tx1 = txM;
			ty1 = tyM;
			tz1 = tzM;
			break;

		case 1:
			znak = flag ^ 1;
			actualOctree = octree[actualOctree].children[flag ^ 1];
			savedState.currentNode = GetNextNode(currentNode, txM, tyM, tz1, nextNodeTable);
			tz0 = tzM;
			tx1 = txM;
			ty1 = tyM;
			break;

		case 2:
			znak = flag ^ 2;
			actualOctree = octree[actualOctree].children[flag ^ 2];
			savedState.currentNode = GetNextNode(currentNode, txM, ty1, tzM, nextNodeTable);
			ty0 = tyM;
			tx1 = txM;
			tz1 = tzM;
			break;

		case 3:
			znak = flag ^ 3;
			actualOctree = octree[actualOctree].children[flag ^ 3];
			savedState.currentNode = GetNextNode(currentNode, txM, ty1, tz1, nextNodeTable);
			ty0 = tyM;
			tz0 = tzM;
			tx1 = txM;
			break;

		case 4:
			znak = flag ^ 4;
			actualOctree = octree[actualOctree].children[flag ^ 4];
			savedState.currentNode = GetNextNode(currentNode, tx1, tyM, tzM, nextNodeTable);
			tx0 = txM;
			ty1 = tyM;
			tz1 = tzM;
			break;

		case 5:
			znak = flag ^ 5;
			actualOctree = octree[actualOctree].children[flag ^ 5];
			savedState.currentNode = GetNextNode(currentNode, tx1, tyM, tz1, nextNodeTable);
			tx0 = txM;
			tz0 = tzM;
			ty1 = tyM;
			break;

		case 6:
			znak = flag ^ 6;
			actualOctree = octree[actualOctree].children[flag ^ 6];
			savedState.currentNode = GetNextNode(currentNode, tx1, ty1, tzM, nextNodeTable);
			tx0 = txM;
			ty0 = tyM;
			tz1 = tzM;
			break;

		case 7:
			znak = flag ^ 7;
			actualOctree = octree[actualOctree].children[flag ^ 7];
			savedState.currentNode = 8;        // if we reach the far top right node then there are    
			tx0 = txM;                        // no nodes we can reach from here, given that our ray 
			ty0 = tyM;                       // is always travelling in a positive direction.
			tz0 = tzM;
			break;

		case 8:
			do {
				stackID -= 1;
				if (stackID < 0) {
					return is->hitTime != INFINITY;
				}
				savedState = stackStates[stackID];
				tx0 = savedState.tx0;
				ty0 = savedState.ty0;
				tz0 = savedState.tz0;
				txM = savedState.txM;
				tyM = savedState.tyM;
				tzM = savedState.tzM;
				tx1 = savedState.tx1;
				ty1 = savedState.ty1;
				tz1 = savedState.tz1;
				currentNode = savedState.currentNode;
				actualOctree = savedState.actualOctree;

				switch (currentNode) {
				case 0:
					znak = flag ^ 0;
					actualOctree = octree[actualOctree].children[flag];
					savedState.currentNode = GetNextNode(currentNode, txM, tyM, tzM, nextNodeTable);
					tx1 = txM;
					ty1 = tyM;
					tz1 = tzM;
					break;

				case 1:
					znak = flag ^ 1;
					actualOctree = octree[actualOctree].children[flag ^ 1];
					savedState.currentNode = GetNextNode(currentNode, txM, tyM, tz1, nextNodeTable);
					tz0 = tzM;
					tx1 = txM;
					ty1 = tyM;
					break;

				case 2:
					znak = flag ^ 2;
					actualOctree = octree[actualOctree].children[flag ^ 2];
					savedState.currentNode = GetNextNode(currentNode, txM, ty1, tzM, nextNodeTable);
					ty0 = tyM;
					tx1 = txM;
					tz1 = tzM;
					break;

				case 3:
					znak = flag ^ 3;
					actualOctree = octree[actualOctree].children[flag ^ 3];
					savedState.currentNode = GetNextNode(currentNode, txM, ty1, tz1, nextNodeTable);
					ty0 = tyM;
					tz0 = tzM;
					tx1 = txM;
					break;

				case 4:
					znak = flag ^ 4;
					actualOctree = octree[actualOctree].children[flag ^ 4];
					savedState.currentNode = GetNextNode(currentNode, tx1, tyM, tzM, nextNodeTable);
					tx0 = txM;
					ty1 = tyM;
					tz1 = tzM;
					break;

				case 5:
					znak = flag ^ 5;
					actualOctree = octree[actualOctree].children[flag ^ 5];
					savedState.currentNode = GetNextNode(currentNode, tx1, tyM, tz1, nextNodeTable);
					tx0 = txM;
					tz0 = tzM;
					ty1 = tyM;
					break;

				case 6:
					znak = flag ^ 6;
					actualOctree = octree[actualOctree].children[flag ^ 6];
					savedState.currentNode = GetNextNode(currentNode, tx1, ty1, tzM, nextNodeTable);
					tx0 = txM;
					ty0 = tyM;
					tz1 = tzM;
					break;

				case 7:
					znak = flag ^ 7;
					actualOctree = octree[actualOctree].children[flag ^ 7];
					savedState.currentNode = 8;        // if we reach the far top right node then there are    
					tx0 = txM;                        // no nodes we can reach from here, given that our ray 
					ty0 = tyM;                       // is always travelling in a positive direction.
					tz0 = tzM;
					break;
				}
			} while (currentNode == 8);
			break;
		}

		stackStates[stackID] = savedState;
		stackID++;
	} while (stackID >= 0);

	return is->hitTime != INFINITY;
}

// Intersect object within information about it.
// ray -> information about ray
// is -> information about intersection
// sp -> buffer of all spheres
// tr -> buffer of all triangles
// me -> buffer of all meshes
// ra_me -> buffer of size every mesh
// @return -> successful of intersect 
bool intersectIs_octree(TRay* ray, TIntersect* is, __global TSphere* sp, unsigned int cnt_sp,
	__global TTriangle* tr, unsigned int cnt_tr, __global TMesh* me,
	__global unsigned int* ra_me, unsigned int cnt_ra_me,
	__global TOctreeBox* octree, unsigned int octree_size, __global TObject* objects,
	unsigned int actualOctree, unsigned char* nextNodeTable)
{
	is->hitTime = INFINITY;
	unsigned char flag = 0;
	TPoint3D halfSize = PointMinusPoint(octree[actualOctree].boxMax, octree[actualOctree].middlePoint);
	TVector3D dir = ray->dir;
	TPoint3D origin = PointPlusPoint(PointMinusPoint(ray->orig, octree[actualOctree].middlePoint), halfSize);
	TPoint3D boxSize = PointMinusPoint(octree[actualOctree].boxMax, octree[actualOctree].boxMin);

	if (dir.x < 0.0f) {
		origin.x = boxSize.x - origin.x;
		dir.x *= -1.0f;
		flag |= 4;
	}

	if (dir.y < 0.0f) {
		origin.y = boxSize.y - origin.y;
		dir.y *= -1.0f;
		flag |= 2;
	}

	if (dir.z < 0.0f) {
		origin.z = boxSize.z - origin.z;
		dir.z *= -1.0f;
	}
	else {
		flag |= 1;
	}

	float invDirx = 1.0f / dir.x;
	float invDiry = 1.0f / dir.y;
	float invDirz = 1.0f / dir.z;

	float tx0 = -origin.x * invDirx;
	float tx1 = (boxSize.x - origin.x) * invDirx;

	float ty0 = -origin.y * invDiry;
	float ty1 = (boxSize.y - origin.y) * invDiry;

	float tz0 = -origin.z * invDirz;
	float tz1 = (boxSize.z - origin.z) * invDirz;

	float maxOf0 = 0.f;
	float minOf1 = 0.f;
	// Compute max for maxOf0
	if (tx0 > ty0) {
		if (tx0 > tz0) {
			maxOf0 = tx0;
		}
		else {
			maxOf0 = tz0;
		}
	}
	else {
		if (ty0 > tz0) {
			maxOf0 = ty0;
		}
		else {
			maxOf0 = tz0;
		}
	}
	// Compute min for minOf1
	if (tx1 < ty1) {
		if (tx1 < tz1) {
			minOf1 = tx1;
		}
		else {
			minOf1 = tz1;
		}
	}
	else {
		if (ty1 < tz1) {
			minOf1 = ty1;
		}
		else {
			minOf1 = tz1;
		}
	}

	//printf("Flag je: %d %f %f\n", flag, dir.x, dir.y);
	if (maxOf0 < minOf1) {
		return ProcessSubNodeIs(ray, is, sp, cnt_sp, tr, cnt_tr, me, ra_me, cnt_ra_me,
			octree, octree_size, objects, 0,
			flag, tx0, ty0, tz0, tx1, ty1, tz1, nextNodeTable);
	}
	return false;
}

// Compute color for pixel (start pathtracing).
// ray -> information about ray
// depth -> maximum depth of computation
// sp -> buffer of all spheres
// tr -> buffer of all triangles
// me -> buffer of all meshes
// ra_me -> buffer of size every mesh
// @return -> result color for pixel
TColor trace_octree(TRay ray, uint depth, uint2 seed, __global TSphere* sp, unsigned int cnt_sp,
	__global TTriangle* tr, unsigned int cnt_tr, __global TMesh* me,
	__global unsigned int* ra_me, unsigned int cnt_ra_me,
	__global TLight* lights, unsigned int cnt_li,
	__global TOctreeBox* octree, unsigned int octree_size, __global TObject* objects)
{
	TColor stack[500];
	unsigned int stack_id = 0;
	unsigned char nextNodeTable[24] =
	{
		4, 2, 1,
		5, 3, 8,
		6, 8, 3,
		7, 8, 8,
		8, 6, 5,
		8, 7, 8,
		8, 8, 7,
		8, 8, 8
	};

	bool isEnd = true;
	int MINIMUM_DEPTH = 4;
	float M_PI2 = 3.14159265358979323846f;
	float p_absorption = 0.1f;
	float absorption_factor = 1 / (1 - p_absorption);

	TIntersect is;
	TColor directLight, indirectLight;

	while (true) {
		directLight.x = 0.0f; directLight.y = 0.0f; directLight.z = 0.0f;
		indirectLight.x = 0.0f; indirectLight.y = 0.0f; indirectLight.z = 0.0f;
		isEnd = true;
		// Try to intersect any object.
		if (intersectIs_octree(&ray, &is, sp, cnt_sp, tr, cnt_tr, me, ra_me, cnt_ra_me, 
							   octree, octree_size, objects, 0, 
							   nextNodeTable)) 
		{ 
			// Found intersection -> compute color.
			TMaterial material = is.material;

			// Get info about object.
			float reflectivity = material.reflectivity;
			float transparency = material.transparency;

			// Russian Roulette.
			float light_type = randomFloat(&seed, 1);

			// Next ray will be?
			if (light_type <= reflectivity) {
				// Reflected ray.
				ray = getReflectedRay(&ray, &is, sp, tr, me, ra_me); // OK
			}
			else if ((light_type - reflectivity) <= transparency) {
				// Refracted ray.
				ray = getRefractedRay(&ray, &is, sp, tr, me, ra_me); // OK
			}
			else {
				// Compute color.
				// Research if intersected point is in shadow.
				// Compute direct light.
				for (int i = 0; i < cnt_li; ++i) { // instead 5 fill lights
					TLight light = lights[i]; // OK
					TRay shadowRay = getShadowRay(&light, &is); // OK

					// Is point visible by light?
					if (!intersect_octree(&shadowRay, sp, cnt_sp, tr, cnt_tr, me, ra_me, cnt_ra_me,
										  octree, octree_size, objects, 0,
										  nextNodeTable))
					{ 
						float intensity = pow(shadowRay.maxT, -2);
						TColor incomingRadiance = light.radiance * intensity;
						TVector3D lightVec = light.worldPos - is.position;

						normalizeVector3D(&lightVec);
						TColor brdf = evalBRDFdiffuse(&material); // TO DO
						float incidentAngle = max(dotVector3D(lightVec, is.normal), 0.0f);
						directLight += incomingRadiance * brdf * incidentAngle;
					}
				}

				// Compute indirect light.
				if ((depth <= MINIMUM_DEPTH) || (randomFloat(&seed, 1) > p_absorption)) {
					isEnd = false;
					float theta, phi;
					TVector3D n_x, n_y, n_z;
					float x_b, y_b, z_b;
					TVector3D dir;

					// Generate random ray path.
					theta = acos(sqrt(1.0f - randomFloat(&seed, 1)));
					phi = 2.0f * M_PI2 * randomFloat(&seed, 1);

					TVector3D up;
					up.x = 1.0f; up.y = 0.0f; up.z = 0.0f;
					if (fabs(is.normal.x) > 0.75f) {
						up.x = 0.0f; up.y = 1.0f; up.z = 0.0f;
					}

					n_x = crossProductVector3D(up, is.normal);
					normalizeVector3D(&n_x);
					n_y = crossProductVector3D(n_x, is.normal);
					n_z = is.normal;

					x_b = cos(phi) * sin(theta);
					y_b = sin(phi) * sin(theta);
					z_b = cos(theta);
					dir = x_b * n_x + y_b * n_y + z_b * n_z;

					ray.orig = is.position;
					ray.dir = dir;
					ray.maxT = INFINITY;
					ray.minT = 0.001f;
					ray.dp.dx.x = 0.0f; ray.dp.dx.y = 0.0f; ray.dp.dx.z = 0.0f;
					ray.dp.dy.x = 0.0f; ray.dp.dy.y = 0.0f; ray.dp.dy.z = 0.0f;
					ray.dd.dx.x = 0.0f; ray.dd.dx.y = 0.0f; ray.dd.dx.z = 0.0f;
					ray.dd.dy.x = 0.0f; ray.dd.dy.y = 0.0f; ray.dd.dy.z = 0.0f;

					TColor brdf = evalBRDFdiffuse(&material);

					indirectLight = M_PI2 * brdf; // missing trace atd.
					if (depth > MINIMUM_DEPTH) {
						indirectLight *= absorption_factor;
					}
				}

				if (isEnd) {
					TColor pixel = directLight;
					for (int i = stack_id; i > 0; i -= 2) {
						pixel = stack[i - 2] + stack[i - 1] * pixel;
					}
					return pixel;
				}
				else {
					stack[stack_id] = directLight;
					stack[stack_id + 1] = indirectLight;
					stack_id += 2;
				}

			} // End of else branch
		}
		else {
			if (stack_id != 0) {
				TColor pixel = directLight;
				for (int i = stack_id; i > 0; i -= 2) {
					pixel = stack[i - 2] + stack[i - 1] * pixel;
				}
				return pixel;
			}
			else {
				// Intersection wasn't successfully. Return black color.
				return (TColor)(0.0f, 0.0f, 0.0f);
			}
		}
		depth++;
	} // End of while loop
} // End of function

#endif // _KERNEL_TRACE_OCTREE_H_