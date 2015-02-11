//
//  Leaf.h
//  mos
//
//  Created by Einar Sandberg on 2015-02-10.
//  Copyright (c) 2015 Einar Sandberg. All rights reserved.
//


#include "glm/glm.hpp"
#include <stdio.h>
#include <math.h>
#include <BulletDynamics/btBulletDynamicsCommon.h>
#include <BulletDynamics/btBulletCollisionCommon.h>
#include <BulletDynamics/Dynamics/btDynamicsWorld.h>
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>
#include <glfw3.h> // GLFW helper library

class Leaf
{
public:
    Leaf(int leafID);
    ~Leaf();
    void getMass();
    void getAirCoeff();
    void getAirRes();
    void getDensity();
    void setValues(double m, double a, double dens, double air, double position);
    void drawLeaf();
    double getXPosition();
    double getAirResistance(const btVector3& velocity, double a, double d);
protected:
    glm::vec3 velocity;
    double mass, angle, airCoeff, density, area, position;
    int leafID;
    bool life;
};