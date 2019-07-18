//
//  GameMaster.hpp
//  DemoGame
//
//  Created by Maya Dunn on 7/11/19.
//  Copyright © 2019 Maya Dunn. All rights reserved.
//

#ifndef GameMaster_hpp
#define GameMaster_hpp

#include <stdio.h>
#include "ActorTree.hpp"

class GameMaster {
private:
	ActorTree * objectTree;
public:
	GameMaster();
	~GameMaster();
	void testFunct(); // make a few actors, try and collide them
};

#endif /* GameMaster_hpp */
