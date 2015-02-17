//#include "stdafx.h"
//#include "glew.h"
#include <glfw3.h> // GLFW helper library
#include <stdio.h>
#include <math.h>
#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <ctime>
#include <cstdlib>
#include <BulletDynamics/btBulletDynamicsCommon.h>
#include <BulletCollision/btBulletCollisionCommon.h>
#include <BulletDynamics/Dynamics/btDynamicsWorld.h>
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>

#include "Leaf.h"
#include "World.h"
#include <vector>

static void error_callback(int error, const char* description);

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

float getScalingConst();

using namespace std;
int main(void)
{
	//sätter alla variabler
	double airCoeff = 1.28;
	double dens = 1.2041;
	double area = 0.0025;
	double mass = 0.1;
	btVector3 pos(0, 0, 0);
	btVector3 angularVel(0, 0, 0);
	btVector3  flu(0.0f,0.0f,0.0f);
	vector <Leaf> theLeaves;
	srand(time(NULL));
	btVector3 wind(0.0f, 1.0f, 0.0f);

    World theWorld;

	btScalar transMatrix[16];

	for (int i = 0; i < 70; i++)
	{
		float randNumbX = rand() % 10000 / 100 - 50;
		float randNumbY = rand() % 10000 / 100 - 50;
		float randNumbZ = rand() % 10000 / 100 - 50;
		Leaf newLeaf(i);
		pos = btVector3(randNumbX, randNumbY, randNumbZ);
		angularVel = btVector3(randNumbX, randNumbY, randNumbZ);
		//flu = 0;
		
		
		newLeaf.setValues(mass, area, dens, airCoeff, pos, flu, angularVel); //i/100=the x translation
		theWorld.getDynamicsWorld()->addRigidBody(newLeaf.getFallingBody());
		theLeaves.push_back(newLeaf);
	}
	//skapa fönster o lite sånt
    
	GLFWwindow* window;
	glfwSetErrorCallback(error_callback);

	if (!glfwInit())
	{
		glfwTerminate();
	}
	window = glfwCreateWindow(1280, 960, "Simple example", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
	}

	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glfwSetKeyCallback(window, key_callback);

	//renderingsloop
	while (!glfwWindowShouldClose(window))
	{

		//nŒn ful vindgrej
		float time = (float)glfwGetTime();
		/*		for (i = dynamicsWorld->getNumCollisionObjects() - 1; i >= 0; i--)
		{
		btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (!body->isStaticObject())
		body->applyCentralForce(btVector3(0.f, 0.0f, 0.0f));
		body->applyTorque(btVector3(0.f, 0.0f, 0.0f));
		}*/

		float ratio;
		int width, height;
		//float velocity = getVelocity(tid);

		theWorld.getDynamicsWorld()->stepSimulation(1 / 100.f, 100000);
		btVector3 velo;
		double airRes = 0;
		btTransform trans;
		btTransform trans_local;

		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		

		glPushMatrix();
			
			for (std::vector<Leaf>::iterator it = theLeaves.begin(); it != theLeaves.end(); ++it)
			{
				glPushMatrix();
					
					glScalef(0.01f, 0.01f, 0.01f);
					btVector3 pos = it->getPosition();
					cout << *it->getFlutter(it->getRotation()) << '\n';
            
					glTranslatef(pos.getX() + it->getFlutter(it->getRotation()).getX(), pos.getY() + it->getFlutter(it->getRotation()).getY(), pos.getZ());
					
					it->getFallingBody()->setAngularVelocity(it->getRotation());
		
					velo = it->getFallingBody()->getLinearVelocity();

					airRes = it->getAirResistance(velo, area, dens);
					
					it->getFallingBody()->applyCentralForce(btVector3(0.f, airRes, 0.f));
					it->getFallingBody()->getMotionState()->getWorldTransform(trans);

					trans.getOpenGLMatrix(transMatrix);
					glMultMatrixf((GLfloat*)transMatrix);
					it->drawLeaf();
				
				glPopMatrix();
			}
		glPopMatrix();
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//destruktorer
	glfwDestroyWindow(window);
	glfwTerminate();

	//theWorld.getDynamicsWorld()->removeRigidBody(fallRigidBody);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}