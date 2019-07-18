//
//  GameMaster.cpp
//  DemoGame
//
//  Created by Maya Dunn on 7/11/19.
//  Copyright © 2019 Maya Dunn. All rights reserved.
//

#include "pch.h"
#include "GameMaster.hpp"
#include <sstream>



GameMaster::GameMaster() {
	objectTree = new ActorTree(3); // actor tree with 3 levels
}

GameMaster::~GameMaster() {
	delete objectTree; // delete object tree
}

void GameMaster::testFunct() {
	MovingActor * testActorA = new MovingActor(XMF4( 0, 8.6, 5, 1), 5.0, XMF4(4, 2, 2, 0)); // 5 kg actor with a 1 by 1 by 1 cube volume (bb not scaled by linear transform)
	MovingActor * testActorB = new MovingActor(XMF4(10, 10, 0, 1), 5.0, XMF4(4, 2, 2, 0));
	testActorA->changeAcceleration({ 1, 0, 0, 1 });
	testActorB->changeAcceleration({-1, 0, 0, 1 });

	for (int i = 0; i < 10; i++) { // run for 10 seconds and see if they collide
		stringstream ss;
		testActorA->calculateNewPosition(i, false);
		testActorB->calculateNewPosition(i, false);
		if (testActorA->detectCollision(testActorB)) {
			ss << "A: " << testActorA->getX() << "; B: " << testActorB->getX() << endl;
			OutputDebugStringA("collision detected\n");

		}
		ss << "Checking time: " << i << endl;
		OutputDebugStringA(ss.str().c_str());
	}

	OutputDebugStringA("done:");
	delete testActorA;
	delete testActorB;
}