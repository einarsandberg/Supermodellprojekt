//
//  Leaf.cpp
//  mos
//  Created by Einar Sandberg on 2015-02-10.
//  Copyright (c) 2015 Einar Sandberg. All rights reserved.
//

/* vertex_array */
//#include "stdafx.h"
#include "GL/glew.h"

#include "glm/glm.hpp"
#include "Leaf.h"
#include <math.h>
#include <time.h>
#include "GL/glfw3.h" // GLFW helper library
#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"
#include <BulletDynamics\Dynamics\btDynamicsWorld.h>
#include <BulletCollision\Gimpact\btGImpactCollisionAlgorithm.h>
#include <BulletCollision\CollisionShapes\btSphereShape.h>
#include <iostream>
#include <common/shader.hpp>



Leaf::Leaf(double m, double a, double dens, double air, const btVector3& pos, const btVector3& flu, const btVector3& angleVel)
{
	mass = m;
	area = a;
	density = dens;
	airCoeff = air;
	position = pos;
	flutter = flu;
	angVel = angleVel;

	rotation = btVector3(rand() % 100 - 50, rand() % 100 - 50, rand() % 100 - 50);
	rotation = normVec(rotation);
	fallShape = new btSphereShape(1);
	fallMotionState =
		new btDefaultMotionState(btTransform(btQuaternion(rotation, 1), btVector3(position)));
	fallInertia = btVector3(0, 0, 0);
	fallShape->calculateLocalInertia(mass, fallInertia);
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, fallShape, fallInertia);
	leafBody = new btRigidBody(fallRigidBodyCI);
	leafBody->setLinearVelocity(btVector3(0, 0, 0));
}

btVector3 Leaf::getFlutter(const btVector3& angularPos, float effectiveArea)
{
	btVector3 angularPos2 = btVector3(angularPos.getX() / (float)glfwGetTime(), angularPos.getY() / (float)glfwGetTime(), angularPos.getZ() / (float)glfwGetTime());
	
	const float width = 0.005;
	const float length = 0.005;

	float farokskonstant = sqrt(mass / (density*(pow(length, 2)*width)));

	float flutterX = (-effectiveArea*sinf(angularPos2.getX())*sinf(angularPos2.getX()) +
		(1 - effectiveArea)*abs(cosf(angularPos2.getX()))*cosf(angularPos2.getX() + 3.14 / 2) -
		abs(sinf(angularPos2.getX() + 3))*cosf(angularPos2.getX() / 2 + 3.14 / 2)) / (farokskonstant);

	float flutterY = (-effectiveArea*pow(leafBody->getAngularVelocity().getY(), 2) / farokskonstant * 
		(sinf(angularPos2.getY())*cosf(angularPos2.getY()) + abs(cosf(angularPos2.getY()))*sinf(angularPos2.getY() + 3.14) 
		- abs(sinf(angularPos2.getY() + 3))*sinf(angularPos.getY() / 2 + 3.14 / 2)) / (farokskonstant));
	
	float flutterZ = ((1 - effectiveArea )- sinf(angularPos2.getZ())*sinf(angularPos2.getZ()) + 
		effectiveArea* abs(cosf(angularPos2.getZ()))*cosf(angularPos2.getZ() + 3.14/2) - abs(sinf(angularPos2.getZ() + 3.14))
		*cosf(angularPos2.getZ() / 2 + 3.14 / 2)) / (farokskonstant);

	//std::cout << flutter << '\n';*/
	flutter = btVector3(flutterX, flutterY, flutterZ);
	return flutter;
}


double Leaf::getAirResistance(const btVector3& velocity, double a, double d)
{
    double airRes = pow(velocity.getY(), 2)*a*d;
    return airRes;
}
btRigidBody* Leaf::getBody()
{
    return leafBody;
}

btVector3 Leaf::getRotation()
{
	return rotation;
}

btVector3 Leaf::getAngVel()
{	
	btVector3 AngVel = btVector3(
		leafBody->getLinearVelocity().getX()/0.035 ,
		leafBody->getLinearVelocity().getY(),
		leafBody->getLinearVelocity().getZ()/0.035 );
	rotation = rotation + AngVel;

	return AngVel;
}
double Leaf::getMass()
{
	return mass;
}

double Leaf::bulletScalar(const btVector3& vec1, const btVector3& vec2)
{
	double X1 = vec1.getX();
	double X2 = vec2.getX();
	double Y1 = vec1.getY();
	double Y2 = vec2.getY();
	double Z1 = vec1.getZ();
	double Z2 = vec2.getZ();
	double ans = (X1*X2) + (Y1*Y2) + (Z1*Z2);
	ans = abs(ans);
	return ans;
}

btVector3 Leaf::normVec(const btVector3& vec1)
{
	float div = sqrt(pow(vec1.getX(), 2) + pow(vec1.getY(), 2) + pow(vec1.getZ(), 2));
	btVector3 ans = btVector3((vec1.getX() / div), (vec1.getY() / div), (vec1.getZ() / div));
	return ans;
}
btVector3 Leaf::noise(){

	float noiseX = (rand() % 100 -50);
	float noiseY = (rand() % 100 -50);
	float noiseZ = (rand() % 100 -50);
	//std::cout << noiseX << '\n';
	//skickar tillbaka en vector med impulskrafter för att bryta upp
	//"molnet" med löv, anpassade storleken efter hur stora resten av
	//krafterna är,detta är inte förankrat i verkligheten.
	return btVector3(noiseX/5000, noiseY/5000, noiseZ/5000);
}

btVector3 Leaf::getPosition()
{
	btVector3 positionFall = getBody()->getCenterOfMassPosition();
	return positionFall;
}


void Leaf::setPosition(const btVector3& newPos){
	position = newPos;
	
	getBody()->translate(btVector3(position));

}