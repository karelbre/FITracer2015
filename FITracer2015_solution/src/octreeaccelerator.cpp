/*
	Name: octreeaccelerator.cpp
	Desc: Octree accelerated data structure.
	Author: Karel Brezina (xbrezi13)
*/

#include "octreeaccelerator.h"

void OctreeAccelerator::build(const std::vector<Intersectable*>& objects) 
{
	c_objects = objects;

	std::for_each(c_objects.begin(), c_objects.end(), [&](Intersectable* obj) {
		AABB aabb;
		obj->getAABB(aabb);
		box.include(aabb);
	});

	middlePoint.x = ((box.mMax.x - box.mMin.x) / 2) + box.mMin.x;
	middlePoint.y = ((box.mMax.y - box.mMin.y) / 2) + box.mMin.y;
	middlePoint.z = (((box.mMin.z - box.mMax.z) / 2) + box.mMax.z) * -1;

	if (c_objects.size() > MAX_OBJECTS) {
		leaf = false;
		for (int i = 0; i < MAX_CELLS; i++) {
			child[i] = new OctreeAccelerator();
			child[i]->setLevel( getLevel() + 1 );
		}

		initBoundaries();

		for (std::vector<Intersectable*>::iterator it = c_objects.begin(); it != c_objects.end(); it++) {
			AABB aabb;
			(*it)->getAABB(aabb);
			std::vector<int> inserted;
			Point3D diff = aabb.mMax - aabb.mMin;

			Point3D point = aabb.mMin;
			insertObject(point, (*it), inserted); // -x-y-z
			point.z += diff.z;
			insertObject(point, (*it), inserted); // -x-y+z
			point.y += diff.y;
			insertObject(point, (*it), inserted); // -x+y+z
			point.z -= diff.z;
			insertObject(point, (*it), inserted); // -x+y-z
			point.x += diff.x;
			insertObject(point, (*it), inserted); // +x+y-z
			point.z += diff.z;
			insertObject(point, (*it), inserted); // +x+y+z
			point.y -= diff.y;
			insertObject(point, (*it), inserted); // +x-y+z
			point.z -= diff.z;
			insertObject(point, (*it), inserted); // +x-y-z
		}

		for (int i = 0; i < MAX_CELLS; i++) {
			child[i]->recursiveBuild();
			std::cout << child[i]->c_objects.size() << std::endl;
		}
	}
	else {
		leaf = true;
	}
	middlePoint.z *= -1;
}

void OctreeAccelerator::recursiveBuild() 
{
	if (level > MAX_DEPTH) {
		leaf = true;
		return;
	}

	middlePoint.x = ((box.mMax.x - box.mMin.x) / 2) + box.mMin.x;
	middlePoint.y = ((box.mMax.y - box.mMin.y) / 2) + box.mMin.y;
	middlePoint.z = (((box.mMin.z - box.mMax.z) / 2) + box.mMax.z) * -1;

	if (c_objects.size() > MAX_OBJECTS) {
		leaf = false;
		for (int i = 0; i < MAX_CELLS; i++) {
			child[i] = new OctreeAccelerator();
			child[i]->setLevel(getLevel() + 1);
		}

		initBoundaries();

		for (std::vector<Intersectable*>::iterator it = c_objects.begin(); it != c_objects.end(); it++) {
			AABB aabb;
			(*it)->getAABB(aabb);
			std::vector<int> inserted;
			Point3D diff = aabb.mMax - aabb.mMin;

			Point3D point = aabb.mMin;
			insertObject(point, (*it), inserted); // -x-y-z
			point.z += diff.z;
			insertObject(point, (*it), inserted); // -x-y+z
			point.y += diff.y;
			insertObject(point, (*it), inserted); // -x+y+z
			point.z -= diff.z;
			insertObject(point, (*it), inserted); // -x+y-z
			point.x += diff.x;
			insertObject(point, (*it), inserted); // +x+y-z
			point.z += diff.z;
			insertObject(point, (*it), inserted); // +x+y+z
			point.y -= diff.y;
			insertObject(point, (*it), inserted); // +x-y+z
			point.z -= diff.z;
			insertObject(point, (*it), inserted); // +x-y-z
		}

		for (int i = 0; i < MAX_CELLS; i++) {
			child[i]->recursiveBuild();
		}
	}
	else {
		leaf = true;
	}
	middlePoint.z *= -1;
}

void OctreeAccelerator::insertObject(Point3D point, Intersectable* obj, std::vector<int>& inserted) 
{
	int index = getSector(point, middlePoint);

	for (std::vector<int>::iterator it = inserted.begin(); it != inserted.end(); it++) {
		if ((*it) == index) return;
	}

	inserted.push_back(index);

	child[index]->c_objects.push_back(obj);
}

void OctreeAccelerator::initBoundaries() 
{
	child[0]->box.mMin.x = box.mMin.x;
	child[0]->box.mMin.y = box.mMin.y;
	child[0]->box.mMin.z = box.mMin.z;

	child[0]->box.mMax.x = middlePoint.x;
	child[0]->box.mMax.y = middlePoint.y;
	child[0]->box.mMax.z = -middlePoint.z;

	child[1]->box.mMin.x = box.mMin.x;
	child[1]->box.mMin.y = box.mMin.y;
	child[1]->box.mMin.z = -middlePoint.z;

	child[1]->box.mMax.x = middlePoint.x;
	child[1]->box.mMax.y = middlePoint.y;
	child[1]->box.mMax.z = box.mMax.z;

	child[2]->box.mMin.x = box.mMin.x;
	child[2]->box.mMin.y = middlePoint.y;
	child[2]->box.mMin.z = box.mMin.z;

	child[2]->box.mMax.x = middlePoint.x;
	child[2]->box.mMax.y = box.mMax.y;
	child[2]->box.mMax.z = -middlePoint.z;

	child[3]->box.mMin.x = box.mMin.x;
	child[3]->box.mMin.y = middlePoint.y;
	child[3]->box.mMin.z = -middlePoint.z;

	child[3]->box.mMax.x = middlePoint.x;
	child[3]->box.mMax.y = box.mMax.y;
	child[3]->box.mMax.z = box.mMax.z;

	child[4]->box.mMin.x = middlePoint.x;
	child[4]->box.mMin.y = box.mMin.y;
	child[4]->box.mMin.z = box.mMin.z;

	child[4]->box.mMax.x = box.mMax.x;
	child[4]->box.mMax.y = middlePoint.y;
	child[4]->box.mMax.z = -middlePoint.z;

	child[5]->box.mMin.x = middlePoint.x;
	child[5]->box.mMin.y = box.mMin.y;
	child[5]->box.mMin.z = -middlePoint.z;

	child[5]->box.mMax.x = box.mMax.x;
	child[5]->box.mMax.y = middlePoint.y;
	child[5]->box.mMax.z = box.mMax.z;

	child[6]->box.mMin.x = middlePoint.x;
	child[6]->box.mMin.y = middlePoint.y;
	child[6]->box.mMin.z = box.mMin.z;

	child[6]->box.mMax.x = box.mMax.x;
	child[6]->box.mMax.y = box.mMax.y;
	child[6]->box.mMax.z = -middlePoint.z;

	child[7]->box.mMin.x = middlePoint.x;
	child[7]->box.mMin.y = middlePoint.y;
	child[7]->box.mMin.z = -middlePoint.z;

	child[7]->box.mMax.x = box.mMax.x;
	child[7]->box.mMax.y = box.mMax.y;
	child[7]->box.mMax.z = box.mMax.z;
}

void OctreeAccelerator::pushObject(Intersectable* obj) 
{
	c_objects.push_back(obj);
}

int OctreeAccelerator::getSector(const Point3D& point, const Point3D& origin) 
{
	int oct = 0;
	if (point.z < origin.z) oct |= 1;
	if (point.y >= origin.y) oct |= 2;
	if (point.x >= origin.x) oct |= 4;
	return oct;
}

bool OctreeAccelerator::intersect(const Ray& ray) 
{
	//flags for the negative direction, used to transform the octree nodes using an XOR operation.
	unsigned char flag = 0;
	AABB bounds;
	Point3D halfSize = box.mMax - middlePoint;

	Vector3D dir = ray.dir;
	Point3D origin = ray.orig - middlePoint + halfSize;
	bounds.mMin = { 0, 0, 0 };
	bounds.mMax = box.mMax - box.mMin;
	Point3D boxSize = bounds.mMax;

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

	float tx0 = (bounds.mMin.x - origin.x) * invDirx;
	float tx1 = (bounds.mMax.x - origin.x) * invDirx;

	float ty0 = (bounds.mMin.y - origin.y) * invDiry;
	float ty1 = (bounds.mMax.y - origin.y) * invDiry;

	float tz0 = (bounds.mMin.z - origin.z) * invDirz;
	float tz1 = (bounds.mMax.z - origin.z) * invDirz;

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
		return ProcessSubNode(ray, flag, tx0, ty0, tz0, tx1, ty1, tz1);
	}
	return false;
}

bool OctreeAccelerator::ProcessSubNode(const Ray& ray, unsigned char flag,
	float tx0, float ty0, float tz0, float tx1, float ty1, float tz1)
{
	if (tx1 < 0 || ty1 < 0 || tz1 < 0) {
		return false;
	}

	if (this->isLeaf())
	{
		std::vector<Intersectable*>::iterator i;
		for (i = c_objects.begin(); i != c_objects.end(); ++i) {
			Intersection currentIs;

			if ((*i)->rayID == ray.ID) {
				continue;
			}
			(*i)->rayID = ray.ID;
			if ((*i)->intersect(ray, currentIs)) {
				return true;
			}
		}
		return false;
	}

	float txM = 0.5f * (tx0 + tx1);
	float tyM = 0.5f * (ty0 + ty1);
	float tzM = 0.5f * (tz0 + tz1);

	unsigned char currentNode = GetFirstNode(tx0, ty0, tz0, txM, tyM, tzM, flag);
	bool success = false;
	do
	{
		switch (currentNode)
		{
		case 0: success = child[flag]->ProcessSubNode(ray, flag, tx0, ty0, tz0, txM, tyM, tzM);
			currentNode = GetNextNode(currentNode, txM, tyM, tzM);
			break;

		case 1: success = child[flag ^ 1]->ProcessSubNode(ray, flag, tx0, ty0, tzM, txM, tyM, tz1);
			currentNode = GetNextNode(currentNode, txM, tyM, tz1);
			break;

		case 2: success = child[flag ^ 2]->ProcessSubNode(ray, flag, tx0, tyM, tz0, txM, ty1, tzM);
			currentNode = GetNextNode(currentNode, txM, ty1, tzM);
			break;

		case 3: success = child[flag ^ 3]->ProcessSubNode(ray, flag, tx0, tyM, tzM, txM, ty1, tz1);
			currentNode = GetNextNode(currentNode, txM, ty1, tz1);
			break;

		case 4: success = child[flag ^ 4]->ProcessSubNode(ray, flag, txM, ty0, tz0, tx1, tyM, tzM);
			currentNode = GetNextNode(currentNode, tx1, tyM, tzM);
			break;

		case 5: success = child[flag ^ 5]->ProcessSubNode(ray, flag, txM, ty0, tzM, tx1, tyM, tz1);
			currentNode = GetNextNode(currentNode, tx1, tyM, tz1);
			break;

		case 6: success = child[flag ^ 6]->ProcessSubNode(ray, flag, txM, tyM, tz0, tx1, ty1, tzM);
			currentNode = GetNextNode(currentNode, tx1, ty1, tzM);
			break;

		case 7: success = child[flag ^ 7]->ProcessSubNode(ray, flag, txM, tyM, tzM, tx1, ty1, tz1);
			currentNode = 8;
			break;
		}

		if (success) {
			return true;
		}
	} while (currentNode < 8);

	return false;
}

bool OctreeAccelerator::intersect(const Ray& ray, Intersection& is)
{
	//flags for the negative direction, used to transform the octree nodes using an XOR operation.
	unsigned char flag = 0;
	AABB bounds;
	Point3D halfSize = box.mMax - middlePoint;

	Vector3D dir = ray.dir;
	Point3D origin = ray.orig - middlePoint + halfSize;
	bounds.mMin = { 0, 0, 0 };
	bounds.mMax = box.mMax - box.mMin;
	Point3D boxSize = bounds.mMax;

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

	float tx0 = (bounds.mMin.x - origin.x) * invDirx;
	float tx1 = (bounds.mMax.x - origin.x) * invDirx;

	float ty0 = (bounds.mMin.y - origin.y) * invDiry;
	float ty1 = (bounds.mMax.y - origin.y) * invDiry;

	float tz0 = (bounds.mMin.z - origin.z) * invDirz;
	float tz1 = (bounds.mMax.z - origin.z) * invDirz;

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
		return ProcessSubNode(ray, is, flag, tx0, ty0, tz0, tx1, ty1, tz1);
	}
	return false;
}

bool OctreeAccelerator::ProcessSubNode(const Ray& ray, Intersection& is, unsigned char flag,
	float tx0, float ty0, float tz0, float tx1, float ty1, float tz1)
{
	char znak = 0;
	if (tx1 < 0 || ty1 < 0 || tz1 < 0) {
		return false;
	}

	if (this->isLeaf())
	{
		std::vector<Intersectable*>::iterator i;
		for (i = c_objects.begin(); i != c_objects.end(); ++i) {
			Intersection currentIs;
			unsigned int iid = (*i)->getIndex();
			if ((*i)->rayID == ray.ID) {
				continue;
			}
			(*i)->rayID = ray.ID;
			if ((*i)->intersect(ray, currentIs)) {
				if (currentIs.mHitTime < is.mHitTime) {
					is = currentIs;
				}
			}
		}
		return is.mHitTime != INF;
	}

	float txM = 0.5f * (tx0 + tx1);
	float tyM = 0.5f * (ty0 + ty1);
	float tzM = 0.5f * (tz0 + tz1);

	unsigned char currentNode = GetFirstNode(tx0, ty0, tz0, txM, tyM, tzM, flag);
	bool success = false;
	do
	{
		switch (currentNode)
		{
		case 0: 
			znak = flag;
			success = child[flag]->ProcessSubNode(ray, is, flag, tx0, ty0, tz0, txM, tyM, tzM);
			currentNode = GetNextNode(currentNode, txM, tyM, tzM);
			break;

		case 1:
			znak = flag ^ 1;
			success = child[flag ^ 1]->ProcessSubNode(ray, is, flag, tx0, ty0, tzM, txM, tyM, tz1);
			currentNode = GetNextNode(currentNode, txM, tyM, tz1);
			break;

		case 2: 
			znak = flag ^ 2;
			success = child[flag ^ 2]->ProcessSubNode(ray, is, flag, tx0, tyM, tz0, txM, ty1, tzM);
			currentNode = GetNextNode(currentNode, txM, ty1, tzM);
			break;

		case 3: 
			znak = flag ^ 3;
			success = child[flag ^ 3]->ProcessSubNode(ray, is, flag, tx0, tyM, tzM, txM, ty1, tz1);
			currentNode = GetNextNode(currentNode, txM, ty1, tz1);
			break;

		case 4: 
			znak = flag ^ 4;
			success = child[flag ^ 4]->ProcessSubNode(ray, is, flag, txM, ty0, tz0, tx1, tyM, tzM);
			currentNode = GetNextNode(currentNode, tx1, tyM, tzM);
			break;

		case 5: 
			znak = flag ^ 5;
			success = child[flag ^ 5]->ProcessSubNode(ray, is, flag, txM, ty0, tzM, tx1, tyM, tz1);
			currentNode = GetNextNode(currentNode, tx1, tyM, tz1);
			break;

		case 6: 
			znak = flag ^ 6;
			success = child[flag ^ 6]->ProcessSubNode(ray, is, flag, txM, tyM, tz0, tx1, ty1, tzM);
			currentNode = GetNextNode(currentNode, tx1, ty1, tzM);
			break;

		case 7: 
			znak = flag ^ 7;
			success = child[flag ^ 7]->ProcessSubNode(ray, is, flag, txM, tyM, tzM, tx1, ty1, tz1);
			currentNode = 8;       
			break;
		}
		
	} while (currentNode < 8);

	return success;
}

unsigned int OctreeAccelerator::GetFirstNode(float tx0, float ty0, float tz0, float txm, float tym, float tzm, unsigned char rayFlags)
{
	unsigned int entryPlane = 0;
	//Get entry plane

	if (tx0 > ty0){
		if (tx0 > tz0)
		{
			if (tym < tx0)entryPlane |= 2;
			if (tzm < tx0)entryPlane |= 1;
			return entryPlane;
		}
	}
	else if (ty0 > tz0){
		if (txm < ty0)entryPlane |= 4;
		if (tzm < ty0)entryPlane |= 1;
		return entryPlane;
	}

	if (txm < tz0) entryPlane |= 4;
	if (tym < tz0) entryPlane |= 2;

	return entryPlane; //returns the first node
}

unsigned int OctreeAccelerator::GetNextNode(unsigned char currentNode, float tx1, float ty1, float tz1)
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

	unsigned char exitPlane = minIndex;

	// Static memory access is slower. 
	// the paper suggested passing the possible exist nodes as function arguments on the stack which is faster
	static unsigned char nextNodeTable[8][3] =
	{
		{ 4, 2, 1 },
		{ 5, 3, 8 },
		{ 6, 8, 3 },
		{ 7, 8, 8 },
		{ 8, 6, 5 },
		{ 8, 7, 8 },
		{ 8, 8, 7 },
		{ 8, 8, 8 }
	};

	//look up next node 
	unsigned char next = nextNodeTable[currentNode][exitPlane];
	return next;
}