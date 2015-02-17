//
//  Leaf.h
//  mos
//
//  Created by Einar Sandberg on 2015-02-10.
//  Copyright (c) 2015 Einar Sandberg. All rights reserved.
//

#include "glfw3.h"// GLFW helper library
#include "glm/glm.hpp"
#include <stdio.h>
//#include "glew.h"
#include <math.h>
#include <BulletDynamics/btBulletDynamicsCommon.h>
#include <BulletCollision/btBulletCollisionCommon.h>
#include <BulletDynamics/Dynamics/btDynamicsWorld.h>
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>



using namespace std;
class Leaf
{
public:
    Leaf(double m, double a, double dens, double air,
            const btVector3& pos, const btVector3& flu,const btVector3& angularVel);
	~Leaf();
	void getMass();
	void getAirCoeff();
	void getAirRes();
	void getDensity();
	btRigidBody* getBody();
	float setFlutter(float time);
	btVector3 getFlutter(const btVector3& angularPos);
	btVector3 getRotation();

	void drawLeaf();
	btVector3 getPosition();
	double getAirResistance(const btVector3& velocity, double a, double d);

protected:
	glm::vec3 velocity;
	double mass, angle, airCoeff, density, area;
	btVector3 flutter;
	btVector3 position, angularVelocity;
	
	bool life;
    //bullet shit
	btRigidBody* leafBody;
    btCollisionShape* fallShape;
    btDefaultMotionState* fallMotionState;
    btVector3 fallInertia;
    btQuaternion rotation;
};