//
//  Player.cpp
//  DemoGame
//
//  Created by Maya Dunn on 7/11/19.
//  Copyright © 2019 Maya Dunn. All rights reserved.
//

#include "pch.h"
#include "Actor.hpp"
#include <sstream>

#pragma region Constructors

// construct an empty actor
Actor::Actor() {
    
}

// construct a stationary actor
Actor::Actor(XMF4 _position, XMF4  _boundingBox) {
	centerOfMass = _position;
	boundingBox = _boundingBox;
}

Actor::~Actor() {
    // delete the node we were in too. Not sure if I want this yet.
    //delete myNode;
}


#pragma endregion


//// get the largest bounding box coord
float Actor::getMaxCoordinate() {
    return max(max(boundingBox.x, boundingBox.y), boundingBox.z);
}

// these ones are fairly straightforward:

float MovingActor::getX() {
	return centerOfMass.x;
}

float MovingActor::getY() {
	return centerOfMass.y;
}

float MovingActor::getZ() {
	return centerOfMass.z;
}

void Actor::assignNode(ActorListNode * _myNode) {
    myNode = _myNode;
}

ActorListNode * Actor::getNode() {
    return myNode;
}

// empty for now as it's unlikely stationary actors need to check for collisions
bool Actor::detectCollision(Actor other) {
    return false;
}


// This is a function that calculates the position of the actor after timeStep length of time
// will probably need some metal/directx code here

//void MovingActor::calculatePosition(float timeStep) {
//
//}

//// Calculates and applies the new position of the object based on current acceleration and velocity
void MovingActor::calculateNewPosition(float timeStep, bool changeV) {
	// position calculated via simple kinematics
	if (timeStep > 0) {
		XMF4 position = velocity * timeStep + linearAcceleration * ((float)(0.5) * timeStep * timeStep);
		position.w = 1;
		XMF4x4 transformMatrix = XMF4x4::identity();
		transformMatrix.setCol(4, position);
		centerOfMass = transformMatrix * centerOfMass;
		if (changeV) {
			velocity = linearAcceleration * timeStep;
		}
	}
}

void MovingActor::changeAcceleration(XMF4 force) {
	// F = ma
	linearAcceleration = { force.x / mass, force.y / mass, force.z / mass, force.w /mass};
	stringstream ss;
	ss << "Acceleration is" << linearAcceleration.str();
	OutputDebugStringA(ss.str().c_str());
}

//DirectX::XMFLOAT4 MovingActor::applyTransform(DirectX::XMFLOAT4X4 matrix, DirectX::XMFLOAT4 vect) {
//	DirectX::XMFLOAT4 result = { 0, 0, 0, 0 };
//	result.x = matrix[0, 0] * vect.x + 
//		result.y = 
//		result.z = 
//	return result;
//}

// XMF4x4 * MovingActor::linearTransform(XMF4 pos) {
//	 XMF4x4 posMatrix = XMF4x4::identity();
//	 posMatrix.setCol(4, pos);
//	return &posMatrix;
//}

// note that this only works if we rotate the bounding box every time we rotate the actor
bool MovingActor::detectCollision(Actor * other) {
	bool a = (centerOfMass + (boundingBox / 2)) - (other->getCOM() - (other->getBound() / 2)) <= 0;
	bool b = (centerOfMass + (boundingBox / 2)) - (other->getCOM() - (other->getBound() / 2)) >= (boundingBox + other->getBound()) * -1;
	bool c = (centerOfMass - (boundingBox / 2)) - (other->getCOM() + (other->getBound() / 2)) <= 0;
	bool d = (centerOfMass - (boundingBox / 2)) - (other->getCOM() + (other->getBound() / 2)) >= (boundingBox + other->getBound()) * -1;
	if ((a && b) || (c && d)) {
		// note that right now we're only checking straight on, and we also need to check for rotations and stuff

		return true;
	}

	return false;
}

bool MovingActor::detectCollision(MovingActor * other) {
	stringstream ss;

	bool a = (centerOfMass + (boundingBox / 2)) - (other->getCOM() - (other->getBound() / 2)) <= 0;
	bool b = (centerOfMass + (boundingBox / 2)) - (other->getCOM() - (other->getBound() / 2)) >= (boundingBox + other->getBound()) * -1;
	bool c = (centerOfMass - (boundingBox / 2)) - (other->getCOM() + (other->getBound() / 2)) <= 0;
	bool d = (centerOfMass - (boundingBox / 2)) - (other->getCOM() + (other->getBound() / 2)) >= (boundingBox + other->getBound()) * -1;
	ss << "Checking collision" << a << b<< c<<d<<endl;
	
	// check if we are closer than the radiuses, and also not on the wrong side
	if ((a && b)||( c && d )) {
			// note that right now we're only checking x, and we also need to check for rotations and stuff
			ss << "overlap at pos: " << centerOfMass.str() << " And " << other->centerOfMass.str() << endl;
			
			// some kind of indication we've hit
			ss << "collision detected" << endl;
			OutputDebugStringA(ss.str().c_str());
			return true;
	}
	OutputDebugStringA(ss.str().c_str());
	return false;
}