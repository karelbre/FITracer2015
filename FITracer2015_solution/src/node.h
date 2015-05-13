/*
 *  node.h
 *  asrTracer
 *
 *  Created by Petrik Clarberg on 2006-02-13.
 *  Copyright 2006 Lund University. All rights reserved.
 *
 */
#ifndef NODE_H
#define NODE_H

#include <list>
#include <vector>
#include "matrix.h"
#include "color.h"

class Intersectable;

/**
 * Base class for all scene hierarchy nodes (cameras, lights, primitives, etc). 
 * This class has a number of functions for setting up the transform matrix of
 * the node. The matrix can either be setup by directly giving a 4x4 matrix,
 * or through calls to the setTranslation(), setScale(), setRotation() functions.
 */
class Node
{
public:
	Node();
	virtual ~Node();
	
	void setTransform(Matrix& M);
	void setTranslation(const Vector3D& T);
	void setTranslation(float tx, float ty, float tz);
	void setScale(float s);
	void setScale(const Vector3D& S);
	void setScale(float sx, float sy, float sz);
	void setRotation(const Vector3D& R);
	void setRotation(float rx, float ry, float rz);
		
protected:
	virtual void prepare() { }
	virtual void getGeometry(std::vector<Intersectable*>& geometry) { }
	
	void addChild(Node* child);	
	bool hasChild(const Node* child) const;
	void setupTransform(const Matrix& world);
	
protected:
	Vector3D mT;						///< Translation.
	Vector3D mS;						///< Scale.
	Vector3D mR;						///< Rotation.
	bool mTransformSet;				///< True if the local transform has been setup.
	Matrix mLocalTransform;			///< Local transform, created from the T,S,R parameters.
	Matrix mWorldTransform;			///< Object->World transform (local + inherited transform).

	std::list<Node*> mChildren;		///< List of children attached to the Node.
	typedef std::list<Node*>::iterator t_itr;
	typedef std::list<Node*>::const_iterator t_constitr;
	
	friend class Scene;				// Scene is a friend class
};

#endif
