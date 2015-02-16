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
	//float getScalingConst();
    void setValues(double m, double a, double dens, double air, const btVector3& pos, float flutter, btRigidBody* leafBody);
    void drawLeaf();
    btVector3 getPosition();
    double getAirResistance(const btVector3& velocity, double a,double d);
   
protected:
    glm::vec3 velocity;
	double mass, angle, airCoeff, density, area;
	float flutter;
	btVector3 position;
    int leafID;
    bool life;
    btRigidBody* leafBody;
};