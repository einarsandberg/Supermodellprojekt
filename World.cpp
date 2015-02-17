//
//  World.cpp
//  mos
//
//  Created by Einar Sandberg on 2015-02-16.
//  Copyright (c) 2015 Einar Sandberg. All rights reserved.
//

#include "World.h"
#include "glm/glm.hpp"
#include "Leaf.h"
#include <math.h>
#include <time.h>
#include "glfw3.h" // GLFW helper library
#include <BulletDynamics/btBulletDynamicsCommon.h>
#include <BulletCollision/btBulletCollisionCommon.h>
#include <BulletDynamics/Dynamics/btDynamicsWorld.h>
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>
#include <iostream>

World::World()
{
    // Build the broadphase
    broadphase = new btDbvtBroadphase();
    
    // Set up the collision configuration and dispatcher
    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
    
    // The actual physics solver
    solver = new btSequentialImpulseConstraintSolver;
    
    // The world.
    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
    dynamicsWorld->setGravity(btVector3(0, -9.82, 0));
}
void World::initWorld()
{

}
btDiscreteDynamicsWorld* World::getDynamicsWorld()
{
    return dynamicsWorld;
}
World::~World()
{
    delete dynamicsWorld;
    delete solver;
    delete dispatcher;
    delete collisionConfiguration;
  
    delete broadphase;
    
}
