//
//  ActorTree.hpp
//  DemoGame
//
//  Created by Maya Dunn on 7/11/19.
//  Copyright © 2019 Maya Dunn. All rights reserved.
//

#ifndef ActorTree_hpp
#define ActorTree_hpp

#include <stdio.h>
class ActorTree;
class ActorTreeNode;
struct ActorListNode;

#include "Actor.hpp"
#include "XMF4.h"

// I am on the fence about adding the player to the actor tree.

class ActorTree;
class ActorTreeNode;

struct ActorListNode {
    Actor * data;
    ActorListNode * next;
    ActorListNode(Actor * actor);
    ~ActorListNode();
    ActorTreeNode * parent;
};

class ActorTreeNode {
    friend class ActorTree;
    private:
    // some constants to determine tree size
    const static int dimensions = 3;
    const static int childQuantity = 3;
    
    // store a pointer to the list of objects in the current location
    ActorListNode * actorList;
    // 3 children for each level, 3 coordinates to have children in
    ActorTreeNode * children[childQuantity][childQuantity][childQuantity];
    float size;
    void addActorToList(Actor * actor);
    int level;
    // roughly where we are

    // directX variables
	XMF4 position;
    
    public:
    ActorListNode * disconnectListNode(ActorListNode * listNode);
    
    ~ActorTreeNode();
    ActorTreeNode();
    ActorTreeNode(int curLevel);
    // recursively make children
    ActorTreeNode(int curLevel, int lastLevel, float quadSize, DirectX::XMFLOAT4 _position);
};

class ActorTree {
    private:
    // pointer to the root of the tree
    ActorTreeNode * root;
    int levels;
    // Size of each cube. set default to 99 units
    float boundSize = 99;
    
    public:
    void changeActorQuadrant(Actor* actor);
    void resolveCollision(Actor * actor);
    void resolveCollision(Actor * actor, ActorTreeNode * currentPlace);
    ActorTreeNode * calculateNewActorQuadrant(Actor * actor);
    void calculateAndAddActor(Actor * actor);
    
    ActorTree(int _levels);
    ~ActorTree();
};

#endif /* ActorTree_hpp */
