/*
	Name: uniformaccelerator.cpp
	Desc: Uniform grid accelerated data structure.
	Author: Karel Brezina (xbrezi13)
*/

#include "uniformaccelerator.h"

void UniformAccelerator::build(const std::vector<Intersectable*>& objects) {
	c_objects = objects;

	std::for_each(c_objects.begin(), c_objects.end(), [&](Intersectable* obj) {
		AABB aabb;
		obj->getAABB(aabb);
		box.include(aabb);
	});

	world_size = box.mMax - box.mMin;
	float inv_grid_size = 1.0f / GRID_SIZE;
	cell_size = world_size;
	cell_size.x *= inv_grid_size; cell_size.y *= inv_grid_size; cell_size.z *= inv_grid_size;
	int size = GRID_SIZE * GRID_SIZE * GRID_SIZE;

	voxels = new UniNode*[size];

	for (int i = 0; i < size; i++)
		voxels[i] = NULL;

	int index = 0;
	float temp;

	for (std::vector<Intersectable*>::iterator it = c_objects.begin(); it != c_objects.end(); it++) {
		AABB aabb;
		(*it)->getAABB(aabb);
		temp = -aabb.mMax.z;
		aabb.mMax.z = -aabb.mMin.z;
		aabb.mMin.z = temp;

		Point3D point_max = aabb.mMax - box.mMin;
		Point3D point_min = aabb.mMin - box.mMin;

		insertObject(point_max, point_min, (*it), index); 
		index++;
	}
}

void UniformAccelerator::insertObject(Point3D point_max, Point3D point_min, Intersectable* obj, int obj_index) {

	int x_max = int(point_max.x / cell_size.x);
	if (x_max == GRID_SIZE) x_max--;
	int y_max = int(point_max.y / cell_size.y);
	if (y_max == GRID_SIZE) y_max--;
	int z_max = int(point_max.z / cell_size.z);
	if (z_max == GRID_SIZE) z_max--;

	int x_min = int(point_min.x / cell_size.x);
	if (x_min == GRID_SIZE) x_min--;
	int y_min = int(point_min.y / cell_size.y);
	if (y_min == GRID_SIZE) y_min--;
	int z_min = int(point_min.z / cell_size.z);
	if (z_min == GRID_SIZE) z_min--;

	for (int x = x_min; x <= x_max; x++) {
		for (int y = y_min; y <= y_max; y++) {
			for (int z = z_min; z <= z_max; z++) {

				int index = x + (y * GRID_SIZE) + (z * GRID_SIZE * GRID_SIZE);

				if (voxels[index] == NULL)
					voxels[index] = new UniNode(obj_index);
				else {
					UniNode* actual = NULL;
					UniNode* next = voxels[index];
					do {
						actual = next;
						next = actual->getNext();
						if (actual->getObject() == obj_index)
							return;
					} while (next != NULL); // neposunuji se v øetìzci

					actual->setNext(obj_index);
				}
			}
		}
	}
}

bool UniformAccelerator::intersect(const Ray& ray) 
{
	float xAxis, yAxis, zAxis;
	Vector3D dir = ray.dir;
	dir.normalize(); // size of step

	Vector3D tDelta;
	Point3D tMax;
	Point3D step;

	tMax.x = ray.orig.x + abs(box.mMin.x);
	tMax.y = ray.orig.y + abs(box.mMin.y);
	tMax.z = abs(box.mMin.z) - ray.orig.z;

	int X, Y, Z, id;
	X = (int(tMax.x / cell_size.x));
	Y = (int(tMax.y / cell_size.y));
	Z = (int(tMax.z / cell_size.z)); // index to cells

	if (dir.x < 0.f) {
		step.x = -1;
		tMax.x -= X*cell_size.x;
	}
	else {
		step.x = 1;
		tMax.x = ((X + 1)*cell_size.x) - tMax.x;
	}
	if (dir.y < 0.f) {
		step.y = -1;
		tMax.y -= (Y*cell_size.y);
	}
	else {
		step.y = 1;
		tMax.y = ((Y + 1)*cell_size.y) - tMax.y;
	}
	if (dir.z < 0.f) {
		step.z = 1;
		tMax.z = ((Z + 1)*cell_size.z) - tMax.z;
	}
	else {
		step.z = -1;
		tMax.z -= (Z*cell_size.z);
	}

	tDelta.x = cell_size.x;
	tDelta.y = cell_size.y;
	tDelta.z = cell_size.z;

	while ((X < GRID_SIZE) && (X >= 0) &&
		(Y < GRID_SIZE) && (Y >= 0) &&
		(Z < GRID_SIZE) && (Z >= 0)) {

		id = int(X + Y * GRID_SIZE + Z * GRID_SIZE * GRID_SIZE);
		UniNode* oct = voxels[id];
		while (oct != NULL) {
			if (c_objects[oct->getObject()]->rayID == ray.ID) {
				oct = oct->getNext();
				continue;
			}
			c_objects[oct->getObject()]->rayID = ray.ID;

			if (c_objects[oct->getObject()]->intersect(ray))
				return true;
			oct = oct->getNext();
		}

		xAxis = (1 / abs(dir.x) * tMax.x);
		yAxis = (1 / abs(dir.y) * tMax.y);
		zAxis = (1 / abs(dir.z) * tMax.z);

		if (xAxis < yAxis) {
			if (xAxis < zAxis) {
				tMax.x += tDelta.x;
				X += (int)step.x;
			}
			else {
				tMax.z += tDelta.z;
				Z += (int)step.z;
			}
		}
		else {
			if (yAxis < zAxis) {
				tMax.y += tDelta.y;
				Y += (int)step.y;
			}
			else {
				tMax.z += tDelta.z;
				Z += (int)step.z;
			}
		}
	}

	return false;
}

bool UniformAccelerator::intersect(const Ray& ray, Intersection& is) 
{
	float xAxis, yAxis, zAxis;
	Vector3D dir = ray.dir;
	dir.normalize(); // size of step

	Vector3D tDelta;
	Point3D tMax;
	Point3D step;

	tMax.x = ray.orig.x + abs(box.mMin.x);
	tMax.y = ray.orig.y + abs(box.mMin.y);
	tMax.z = abs(box.mMin.z) - ray.orig.z;

	int X, Y, Z, id;
	X = (int(tMax.x / cell_size.x));
	Y = (int(tMax.y / cell_size.y));
	Z = (int(tMax.z / cell_size.z)); // index to cells

	if (dir.x < 0.f) {
		step.x = -1;
		tMax.x -= X*cell_size.x;
	}
	else {
		step.x = 1;
		tMax.x = ((X + 1)*cell_size.x) - tMax.x;
	}
	if (dir.y < 0.f) {
		step.y = -1;
		tMax.y -= (Y*cell_size.y);
	}
	else {
		step.y = 1;
		tMax.y = ((Y+1)*cell_size.y) - tMax.y;
	}
	if (dir.z < 0.f) {
		step.z = 1;
		tMax.z = ((Z+1)*cell_size.z) - tMax.z;
	}
	else {
		step.z = -1;
		tMax.z -= (Z*cell_size.z);
	}

	tDelta.x = cell_size.x;
	tDelta.y = cell_size.y;
	tDelta.z = cell_size.z;

	while ((X < GRID_SIZE) && (X >= 0) &&
		(Y < GRID_SIZE) && (Y >= 0) &&
		(Z < GRID_SIZE) && (Z >= 0)) {

		id = int(X + Y * GRID_SIZE + Z * GRID_SIZE * GRID_SIZE);
		UniNode* oct = voxels[id];
		Intersection currentIs;

		while (oct != NULL) {
			if (c_objects[oct->getObject()]->rayID == ray.ID) {
				oct = oct->getNext();
				continue;
			}
			c_objects[oct->getObject()]->rayID = ray.ID;

			if (c_objects[oct->getObject()]->intersect(ray, currentIs)) {
				if (currentIs.mHitTime < is.mHitTime) {
					is = currentIs;
				}
			}
			oct = oct->getNext();
		}

		xAxis = (1 / abs(dir.x) * tMax.x);
		yAxis = (1 / abs(dir.y) * tMax.y);
		zAxis = (1 / abs(dir.z) * tMax.z);

		if (xAxis < yAxis) {
			if (xAxis < zAxis) {
				tMax.x += tDelta.x;
				X += (int)step.x;
			}
			else {
				tMax.z += tDelta.z;
				Z += (int)step.z;
			}
		}
		else {
			if (yAxis < zAxis) {
				tMax.y += tDelta.y;
				Y += (int)step.y;
			}
			else {
				tMax.z += tDelta.z;
				Z += (int)step.z;
			}
		}
	}

	return is.mHitTime != INF;
}

