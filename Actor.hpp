//
//  Player.hpp
//  DemoGame
//
//  Created by Maya Dunn on 7/11/19.
//  Copyright © 2019 Maya Dunn. All rights reserved.
//

// This game is just 3D snake (with some obstacles)

#ifndef Player_hpp
#define Player_hpp

#include <stdio.h>
#include <cmath>
#include <cstdlib>
#include <algorithm>   // just for max



class Actor;

#include "ActorTree.hpp"
#include "XMF4.h"

using namespace std;

class Actor {
    
    protected:
		//XMF4 position = { 0, 0, 0, 1 };
		XMF4 centerOfMass = { 0, 0, 0, 1 };
		XMF4 boundingBox;
        ActorListNode * myNode;
    
    public:
        Actor();
        ~Actor();
        Actor(XMF4 _position, XMF4 _boundingBox);
        void assignNode(ActorListNode * _myNode);
        ActorListNode * getNode();
        bool detectCollision(Actor other);
        float getMaxCoordinate();
        
		// this should probably both be a deep copy
		XMF4 getBound() {
			return boundingBox;
		};
		XMF4 getCOM() {
			return centerOfMass;
		};
};

class MovingActor : Actor {
    private:
		XMF4 linearAcceleration;
		XMF4 velocity;
		float mass;
		XMF4x4* linearTransform(XMF4 pos);
		XMF4 applyTransform(XMF4x4 matrix, XMF4 vect);

    public:
		float getX();
		float getY();
		float getZ();
		MovingActor(XMF4 _position, float _mass, XMF4 _boundingBox) : Actor(_position, _boundingBox) {
			mass = _mass;
		};
		void changeAcceleration(XMF4 force);
		void calculateNewPosition(float timeStep, bool changeV);
		bool detectCollision(Actor * other);
		bool detectCollision(MovingActor * other);
};

#endif
