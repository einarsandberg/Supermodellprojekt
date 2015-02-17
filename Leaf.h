//
//  Leaf.h
//  mos
//
//  Created by Einar Sandberg on 2015-02-10.
//  Copyright (c) 2015 Einar Sandberg. All rights reserved.
//

#include "glfw3.h"// GLFW helper library
#include "glm/glm/glm.hpp"
#include <stdio.h>
//#include "glew.h"
#include <math.h>
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include <BulletDynamics/Dynamics/btDynamicsWorld.h>
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>
#include <vector>


using namespace std;
class Leaf
{
public:
    Leaf(int leafID);
    ~Leaf();
    void getMass();
    void getAirCoeff();
    void getAirRes();
    void getDensity();
    btRigidBody* getFallingBody();
	float setFlutter(float time);
	float getFlutter(float time);
	double getRotation();
	btVector3 getNormal();
	//float getScalingConst();
    void setValues(double m, double a, double dens, double air, const btVector3& pos, float flutter, btRigidBody* leafBody, const btVector3& normal);
    void drawLeaf();
    btVector3 getPosition();
    double getAirResistance(const btVector3& velocity, double a,double d);
   
protected:
    glm::vec3 velocity;
	btVector3 normal;
	double mass, angle, airCoeff, density, area;
	float flutter;
	btVector3 position;
    int leafID;
    bool life;
    btRigidBody* leafBody;
};