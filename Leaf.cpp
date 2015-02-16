//
//  Leaf.cpp
//  mos
//
//  Created by Einar Sandberg on 2015-02-10.
//  Copyright (c) 2015 Einar Sandberg. All rights reserved.
//

/* vertex_array */


#include "stdafx.h"

#include "glew.h"
#include "glm/glm/glm.hpp"
#include "Leaf.h"
#include <math.h>
#include <time.h>
#include "glfw3.h" // GLFW helper library
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include <BulletDynamics/Dynamics/btDynamicsWorld.h>
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>
#include <iostream>


const int antalPos = 3;

Leaf::Leaf(int id)
{
    leafID = id;
}

Leaf::~Leaf()
{
    
}
btVector3 Leaf::getPosition()
{
    return position;
}

void Leaf::setValues(double m, double a, double dens, double air, const btVector3& pos, float flu)
{
    mass = m;
    area = a;
    density = dens;
    airCoeff=air;
    position = pos;
	flutter = flu;
}


float Leaf::getFlutter(float time)
{
	float width = 0.005;
	float length = 0.005;
	float farokskonstant = sqrt(mass / (density*pow(length, 2)*width));


	//fixa med krafter, skaffa Aortogonal, Aparallell och räkna på vinkelpositioner ist för tid.
	flutter = -sinf(time)*cosf(time) + abs(cosf(time))*cosf(time + 3) -   abs(sinf(time + 3))*sinf(time / 2 + 3.14 / 2);
	//std::cout << flutter << '\n';
	return flutter * 100 / (farokskonstant);

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

	glewInit();


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
/*
    glEnable(GL_TEXTURE_2D);
    glBegin(GL_TRIANGLES);
    glColor3f(0.f, 1.f, 0.f);
    glVertex3f(-0.6f, -0.4f, 0.f);
    glColor3f(0.f, 1.f, 0.f);
    glVertex3f(0.6f, -0.4f, 0.f);
    glColor3f(0.f, 1.f, 0.f);
    glVertex3f(0.f, 1.0f, 0.f);
    glEnd();
    glDisable(GL_TEXTURE_2D);*/
}
double Leaf::getAirResistance(const btVector3& velocity, double a, double d)
{
    double airRes=pow(velocity.getY(),2)*a*d;
    return airRes;
}

double Leaf::getRotation(){ return 0; };