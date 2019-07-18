#include "pch.h"
#include "ActorTree.hpp"

//
//  ActorTree.cpp
//  DemoGame
//
//  Created by Maya Dunn on 7/11/19.
//  Copyright © 2019 Maya Dunn. All rights reserved.
//


ActorListNode::ActorListNode(Actor * actor) {
    data = actor;
    next = NULL;
}

// proceed with caution -> deleting nodes deletes their data too! and the data of everything linked to them!
// do not delete parent.
ActorListNode::~ActorListNode() {
    delete data;
    delete next;
}

ActorTreeNode::ActorTreeNode() {
	actorList = NULL;
	level = 0;
	position = { 0, 0, 0, 0 };
	size = 0;
}

ActorTreeNode::ActorTreeNode(int _level) {
	actorList = NULL;
	level = _level;
	position = { 0, 0, 0, 0 };
	size = 0;
}

// recursive constructor for tree
ActorTreeNode::ActorTreeNode(int curLevel, int lastLevel, float quadSize, DirectX::XMFLOAT4 _position) {
    // we will define this later
    actorList = NULL;
    level = curLevel;
    // turn this into a for loop later :(
	position = { _position.x,  _position.y, _position.z, _position.w };
    size = quadSize;
    
    // this is going to use a lot of memory. Calculate positions of all children and recursively assign
    // middle quad has same position as parent.
    if (curLevel < lastLevel) {
        for (int x = 0; x < childQuantity; x++) {
            // new position is + if last child, - if first child, same if middle child
            _position.x = _position.x+quadSize*(x-1)/2;
            for (int y = 0; y < childQuantity; y++) {
                _position.y = _position.y+quadSize*(y-1)/2;
                for (int z = 0; z < childQuantity; z++) {
                    _position.z = _position.z+quadSize*(z-1)/2;
                    children[x][y][z] = new ActorTreeNode(curLevel+1, lastLevel, quadSize/childQuantity, _position);
                }
            }
        }
    }
}

// recursively gets rid of all children too
ActorTreeNode::~ActorTreeNode() {
    for (int x = 0; x < childQuantity; x++) {
        for (int y = 0; y < childQuantity; y++) {
            for (int z = 0; z < childQuantity; z++) {
                delete children[x][y][z];
            }
        }
    }
    delete actorList;
}

// add an actor to the beginning of a new quadrant's linkedlist by creating a new node for it.
// note: do not call this method on an actor already in a list. That will lose ties to all linked actors without deleting them
//		call disconnectListNode first
void ActorTreeNode::addActorToList(Actor * actor) {
    ActorListNode * actorNode;
    if (actor->getNode() == NULL) {
        actorNode = new ActorListNode(actor);
	} else {
		actorNode = actor->getNode(); 
	}
    // stick it at the front of the list for this quadrant
    // I don't anticipate it being NULL but I also want to stay safe
    if (actorNode != NULL) {
        actor->assignNode(actorNode);
        actorNode->parent = this;
        actorNode->next = actorList;
        actorList = actorNode;
		// I think we are just swapping around pointers but this desperately needs testing
    }
}

// Safely remove an actor from its quadrant
ActorListNode * ActorTreeNode::disconnectListNode(ActorListNode * listNode) {
    ActorListNode * currentNode = actorList;
    ActorListNode * prevNode = actorList;
    // if it's the first node just grab it
    if (listNode == actorList) {
        actorList = currentNode->next;
        return listNode;
    }
    // otherwise search through all the other nodes
    while (&currentNode != &listNode && currentNode->next != NULL) {
        prevNode = currentNode;
        currentNode = (currentNode->next);
    }
    // if we didn't find it, return null
    if (currentNode != listNode) {
        return NULL;
    } else {
        // if we did find it, get rid of parent, relink and return node
        prevNode->next = currentNode->next;
        currentNode->parent = NULL;
        return currentNode;
    }
}

ActorTree::ActorTree(int _levels) {
   // build a tree _levels_ levels deep
    // this will recursively construct the whole tree
    levels = _levels;
	DirectX::XMFLOAT4 origin = {0, 0, 0, 0};
    root = new ActorTreeNode(0, levels, boundSize, origin);
}

ActorTree::~ActorTree() {
    // note that this will eventually delete all nodes attached to root.
    delete root;
}

void ActorTree::changeActorQuadrant(Actor* actor) {
	ActorTreeNode * oldNode = actor->getNode()->parent;
	ActorTreeNode * newNode = calculateNewActorQuadrant(actor);
	ActorListNode * currentNode = oldNode->disconnectListNode(actor->getNode());
	if (currentNode != NULL) { // if it was successfully disconnected
		newNode->addActorToList(actor);
	}
}

ActorTreeNode * ActorTree::calculateNewActorQuadrant(Actor * actor) {
    // use actor.boundingbox to see how large it is. compare to size
    // I have no idea if this function works like this. Fingers crossed
	
	float actorSize = actor->getMaxCoordinate();
    
    float size = boundSize;
    int curLevel = 0;
	ActorTreeNode *prevNode = actor->getNode()->parent;
	if (prevNode) {
		// basically check to make sure we are actually in the wrong quadrant
		if (((prevNode->position + prevNode->size) >= actor->getCOM() &&  (prevNode->position - prevNode->size) <= actor->getCOM())) {
			return prevNode;
		}
	}

	float * actorPos = actor->getCOM().asArr();

    ActorTreeNode* curNode = root;
    while (size > actorSize || curLevel > levels) {
        size = size / curNode->childQuantity;
        int whichChild[3] = {0, 0, 0};
		float positionArr[3] = { curNode->position.x, curNode->position.y, curNode->position.z };
        for (int i = 0; i < 3; i++) {
            // if the position we want is smaller than the position we are at, jump left. if larger jump right.
            if (actorPos[i] > positionArr[i]+size/2) {
                whichChild[i] = 2;
            } else if (actorPos[i] < positionArr[i]-size/2) {
                whichChild[i] = 0;
            } else {
                // if roughly the same, go to the middle
                whichChild[i] = 1;
            }
        }
        // find the right child to go to and go to it.
        curNode = curNode->children[whichChild[0]][whichChild[1]][whichChild[2]];
        curLevel++;
    }
    // return the node that the actor should be attached to.
    return curNode;
}

void ActorTree::calculateAndAddActor(Actor * actor) {
    ActorTreeNode * rightNode = calculateNewActorQuadrant(actor);
    rightNode->addActorToList(actor);
}

// check if the current actor is colliding with any other actors down the line
void ActorTree::resolveCollision(Actor * actor) {
    ActorTreeNode * currentPlace = actor->getNode()->parent;
    resolveCollision(actor, currentPlace);
}

void ActorTree::resolveCollision(Actor * actor, ActorTreeNode * currentTreeNode) {
    if (currentTreeNode != NULL) {
        ActorListNode * otherNode = currentTreeNode->actorList;
        // see if we are colliding with any other nodes in our area
        while (otherNode != NULL) {
            actor->detectCollision(*(otherNode->data));
            // some sort of code here that decides what to do if they are actually colliding or put that in the detect collision method.
            // probably a momentum transfer or something
            otherNode = otherNode->next;
        }
        // then do this with all the other smaller objects down the line, via recursion
        for (int x = 0; x < currentTreeNode->childQuantity; x++) {
            for (int y = 0; y < currentTreeNode->childQuantity; y++) {
                for (int z = 0; z < currentTreeNode->childQuantity; z++) {
                    resolveCollision(actor, currentTreeNode->children[x][y][z]);
                }
            }
        }
    }
}
