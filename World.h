//
//  World.h
//  mos
//
//  Created by Einar Sandberg on 2015-02-16.
//  Copyright (c) 2015 Einar Sandberg. All rights reserved.
//

#include <stdio.h>
#include <BulletDynamics/btBulletDynamicsCommon.h>
#include <BulletCollision/btBulletCollisionCommon.h>
#include <BulletDynamics/Dynamics/btDynamicsWorld.h>
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>
class World
{
    public:
        World();
        ~World();
        void initWorld();
        btDiscreteDynamicsWorld* getDynamicsWorld();
    private:
        btBroadphaseInterface* broadphase;
        btDefaultCollisionConfiguration* collisionConfiguration;
        btCollisionDispatcher* dispatcher;
        btSequentialImpulseConstraintSolver* solver;
        btDiscreteDynamicsWorld* dynamicsWorld;
};