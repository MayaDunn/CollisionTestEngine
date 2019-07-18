# CollisionTestEngine
Work in progress collision detecting engine in C++

This project currently has an (untested) ternary tree that controls "Actors" to reduce the complexity of collision tests. 
These actors can currently detect if their bounding boxes overlap.

Project also contains a DirectX XMFLOAT4 wrapper class to handle vector operations.

GameMaster contains a simple test function that gives two objects a 'push' and sees where they collide.

Future goals include 
- detecting overlap of more complex shapes
- running tests to make sure the tree is able to transfer Actors to the right branches
- Creating visual representations of objects
- Adding basic game style character control

Project is largely awaiting comments and testing.
