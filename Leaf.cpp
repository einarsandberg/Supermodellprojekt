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
           const btVector3& pos, const btVector3& flu, const btVector3& startAngle)
{
    mass = m;
    area = a;
    density = dens;
    airCoeff = air;
    position = pos;
    flutter = flu;
    startAng = startAngle;
    
    rotation = btQuaternion(0, 0, 0, 1);
    fallShape = new btSphereShape(1);
    fallMotionState =
    new btDefaultMotionState(btTransform(rotation, btVector3(position)));
    fallInertia = btVector3(0, 0, 0);
    fallShape->calculateLocalInertia(mass, fallInertia);
    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, fallShape, fallInertia);
    leafBody = new btRigidBody(fallRigidBodyCI);
    leafBody->setLinearVelocity(btVector3(0, 0, 0));
    
    
}

/*Leaf::~Leaf()
 {
 delete leafBody->getMotionState();
 delete fallShape;
 //delete leafBody;
 
 
 }*/
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
    //fixa med krafter, skaffa Aortogonal, Aparallell och r�kna p� vinkelpositioner ist f�r tid.
    float flutterX = (-sinf(angularPos.getX())*sinf(angularPos.getX()) + abs(cosf(angularPos.getX()))*cosf(angularPos.getX() + 3) - abs(sinf(angularPos.getX() + 3))*cosf(angularPos.getX() / 2 + 3.14 / 2)) * 1000 / (farokskonstant);
    float flutterY = (-leafBody->getInterpolationLinearVelocity().getY() / farokskonstant * (sinf(angularPos.getY())*cosf(angularPos.getY()) + abs(cosf(angularPos.getY()))*sinf(angularPos.getY() + 3) - abs(sinf(angularPos.getY() + 3))*sinf(angularPos.getY() / 2 + 3.14 / 2)) * 1000 / (farokskonstant));
    float flutterZ = (-sinf(angularPos.getZ())*sinf(angularPos.getZ()) + abs(cosf(angularPos.getZ()))*cosf(angularPos.getZ() + 3) - abs(sinf(angularPos.getZ() + 3))*cosf(angularPos.getZ() / 2 + 3.14 / 2)) * 1000 / (farokskonstant);
    
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
    
    return startAng/5 + btVector3(leafBody->getLinearVelocity().getX() / 0.5 - 1, leafBody->getLinearVelocity().getY() / 5 , leafBody->getLinearVelocity().getZ() / 0.5 - 1);
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