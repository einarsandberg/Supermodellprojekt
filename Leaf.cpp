//
//  Leaf.cpp
//  mos
//  Created by Einar Sandberg on 2015-02-10.
//  Copyright (c) 2015 Einar Sandberg. All rights reserved.
//

/* vertex_array */
//#include "stdafx.h"
//#include "glew.h"
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


const int antalPos = 3;

Leaf::Leaf(double m, double a, double dens, double air,
           const btVector3& pos, const btVector3& flu,const btVector3& angularVel)
{
    mass = m;
    area = a;
    density = dens;
    airCoeff = air;
    position = pos;
    flutter = flu;
    angularVelocity = angularVel;
    
    rotation=btQuaternion(0,0,0,1);
    fallShape = new btSphereShape(1);
    fallMotionState =
    new btDefaultMotionState(btTransform(rotation, btVector3(0, 3, 0)));
    fallInertia=btVector3(0,0,0);
    fallShape->calculateLocalInertia(mass, fallInertia);
    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, fallShape, fallInertia);
    leafBody = new btRigidBody(fallRigidBodyCI);
    leafBody->setLinearVelocity(btVector3(0, 0, 0));
    
    
}

Leaf::~Leaf()
{/*
    delete leafBody->getMotionState();
    //delete leafBody;
    delete fallShape;*/
    
}
btVector3 Leaf::getPosition()
{
	return position;
}

btVector3 Leaf::getFlutter(const btVector3& angularPos)
{
	float width = 0.005;
	float length = 0.005;
	float farokskonstant = sqrt(mass / (density*pow(length, 2)*width));

	/**/
	//fixa med krafter, skaffa Aortogonal, Aparallell och räkna på vinkelpositioner ist för tid.
	float flutterX = (-sinf(angularPos.getX())*cosf(angularPos.getX()) + abs(cosf(angularPos.getX()))*cosf(angularPos.getX() + 3) - abs(sinf(angularPos.getX() + 3))*sinf(angularPos.getX() / 2 + 3.14 / 2)) * 1000 / (farokskonstant);
	float flutterY = (-sinf(angularPos.getY())*cosf(angularPos.getY()) + abs(cosf(angularPos.getY()))*cosf(angularPos.getY() + 3) - abs(sinf(angularPos.getY() + 3))*sinf(angularPos.getY() / 2 + 3.14 / 2)) * 1000 / (farokskonstant);
	float flutterZ = (-sinf(angularPos.getZ())*cosf(angularPos.getZ()) + abs(cosf(angularPos.getZ()))*cosf(angularPos.getZ() + 3) - abs(sinf(angularPos.getZ() + 3))*sinf(angularPos.getZ() / 2 + 3.14 / 2)) * 1000 / (farokskonstant);
	
	//std::cout << flutter << '\n';*/
	flutter = btVector3(flutterX, flutterY, flutterZ);
	return flutter;

}

void Leaf::drawLeaf()
{

	GLuint triangleVBO;

	//Vertices of a triangle (counter-clockwise winding)
	GLfloat leafVertices[] =
	{
		0.0, 1.0, 0.0,
		-1.0, -1.0, 0.0,
		1.0, -1.0, 0.0
	};

   // glewInit();

	//Create a new VBO and use the variable id to store the VBO id
	glGenBuffers(1, &triangleVBO);

	//Make the new VBO active
	glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);

	//Upload vertex data to the video device

	glBufferData(GL_ARRAY_BUFFER, sizeof(leafVertices), leafVertices, GL_STATIC_DRAW);

	//Make the new VBO active. Repeat here incase changed since initialisation
	glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);

	//Draw Triangle from VBO - do each time window, view point or data changes
	//Establish its 3 coordinates per vertex with zero stride in this array; necessary here
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	//Establish array contains vertices (not normals, colours, texture coords etc)
	glEnableClientState(GL_VERTEX_ARRAY);

	//Actually draw the triangle, giving the number of vertices provided
	glDrawArrays(GL_TRIANGLES, 0, sizeof(leafVertices) / sizeof(float) / 3);

	//Force display to be drawn now
	glFlush();

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
	return angularVelocity;
}