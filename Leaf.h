//
//  Leaf.h
//  mos
//
//  Created by Einar Sandberg on 2015-02-10.
//  Copyright (c) 2015 Einar Sandberg. All rights reserved.
//

#include "GL/glfw3.h"// GLFW helper library
#include "glm/glm.hpp"
#include <stdio.h>
//#include "glew.h"
#include <math.h>
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include <BulletDynamics/Dynamics/btDynamicsWorld.h>
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>



using namespace std;
class Leaf
{
public:
	Leaf(double m, double a, double dens, double air,
		const btVector3& pos, const btVector3& flu, const btVector3& Angle);
	//~Leaf();
	btRigidBody* getBody();
	float setFlutter(float time);
	btVector3 getFlutter(const btVector3& angularPos, float areaMult);
	btVector3 getAngVel();
	btVector3 getRotation();
	double getMass();
	void drawLeaf();
	btVector3 getPosition();
	double getAirResistance(const btVector3& velocity, double a, double d);
	double bulletScalar(const btVector3& vec1, const btVector3& vec2);
	btVector3 normVec(const btVector3& vec1);
	btVector3 noise();
	void setPosition(const btVector3& newPos);
	

protected:
	glm::vec3 velocity;
	double mass, angleVel, airCoeff, density, area;
	btVector3 flutter;
	btVector3 position, rotation, angVel;

	bool life;
	//bullet shit
	btRigidBody* leafBody;
	btCollisionShape* fallShape;
	btDefaultMotionState* fallMotionState;
	btVector3 fallInertia;
};