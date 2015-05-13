/*
 *  node.cpp
 *  asrTracer
 *
 *  Created by Petrik Clarberg on 2006-02-14.
 *  Copyright 2006 __MyCompanyName__. All rights reserved.
 *
 */

#include "node.h"

/**
 * Constructor.
 */
Node::Node() : mTransformSet(false), mS(1)
{
}

/**
 * Destroys the node and de-allocates all children nodes.
 */
Node::~Node()
{
	while (mChildren.size() > 0) {
		Node* n = mChildren.front();
		mChildren.pop_front();
		delete n;
	}
}

/**
 * Adds a node to the list of children.
 * @param child Ptr to child node
 */
void Node::addChild(Node* child)
{
	mChildren.push_back(child);
}

/**
 * Returns true if a specific node is a child of this node.
 * @param child The node to look for
 */
bool Node::hasChild(const Node* child) const
{
	t_constitr itr = mChildren.begin();
	for ( ; itr!=mChildren.end(); ++itr) {
		if ((*itr)==child)
			return true;
		if ((*itr)->hasChild(child))
			return true;
	}
	return false;
}


/**
 * Manually set the local transform matrix to M.
 */
void Node::setTransform(Matrix& M) { mLocalTransform=M; mTransformSet=true;}

/**
 * Sets the translation of the node to T.
 */
void Node::setTranslation(const Vector3D& T) { mT=T; }

/**
 * Sets the translation of the node to (tx,ty,tz).
 */
void Node::setTranslation(float tx, float ty, float tz) { mT=Vector3D(tx,ty,tz); }

/**
 * Sets the scale uniformly to s.
 */
void Node::setScale(float s) { mS=Vector3D(s); }

/**
 * Sets the scale non-uniformly to S.
 */
void Node::setScale(const Vector3D& S) { mS=S; }

/**
 * Sets the scale non-uniformly to (sx,sy,sz).
 */
void Node::setScale(float sx, float sy, float sz) { mS=Vector3D(sx,sy,sz); }

/**
 * Sets the rotation about the x,y,z axes to the components of R. 
 * The rotations are concatenated as Rz*Ry*Rz, thus the object is 
 * first rotated around x, then around y, and last around z.
 */
void Node::setRotation(const Vector3D& R) { mR=R; }

/**
 * Sets the rotation about the x,y,z axes to (rx,ry,rz) respectively.
 * The rotations are concatenated as Rz*Ry*Rz, thus the object is 
 * first rotated around x, then around y, and last around z.
 */
void Node::setRotation(float rx, float ry, float rz) { mR=Vector3D(rx,ry,rz); }

/**
 * Setup the local and world transform of the node.
 * The world transform is computed as the parent transform times
 * the local transform.
 */
void Node::setupTransform(const Matrix& parent)
{
	// Setup local transform matrix from the T,S,R parameters, unless
	// it has already been set by a call to setTransform().
	if (!mTransformSet) {
		// Setup rotation, scaling and translation matrices.
		Matrix Rx, Ry, Rz, S, T;
		Rx.rotX(mR.x);
		Ry.rotY(mR.y);
		Rz.rotZ(mR.z);
		S.scale(mS.x,mS.y,mS.z);
		T.translation(mT.x,mT.y,mT.z);

		// Concatenate matrices, scale first, then rotation, last translation.
		mLocalTransform = T*Rz*Ry*Rx*S;
	}

	// Compute object->world transform as parent*local transform.
	mWorldTransform = parent * mLocalTransform;
}
