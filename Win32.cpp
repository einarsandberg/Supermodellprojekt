#include "stdafx.h"
#include "glew.h"
#include <glfw3.h> // GLFW helper library
#include <stdio.h>
#include <math.h>
#include <iostream>
#include "glm/glm/glm.hpp"
#include "glm/glm/gtc/matrix_transform.hpp"
#include <ctime>
#include <cstdlib>
#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include <BulletDynamics/Dynamics/btDynamicsWorld.h>
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>

#include "Leaf.h"
#include <vector>

static void error_callback(int error, const char* description);

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

double getAirResistance(const btVector3& velocity);

double getAirResistance(const btVector3& velocity);
float getScalingConst();


using namespace std;
int main(void)
{
	//sätter alla variabler
	double airCoeff = 1.28;
	double dens = 1.2041;
	double area = 0.0025;
	double mass = 0.1;
	btVector3 poss(0, 0, 0);
	btVector3 angularVel(0, 0, 0);
	btVector3  flu(0.0f,0.0f,0.0f);
	vector <Leaf> theLeaves;
	srand(time(NULL));
	btVector3 vind(0.0f, 1.0f, 0.0f);

	//initiera alla bullet funktioner

	// Build the broadphase
	btBroadphaseInterface* broadphase = new btDbvtBroadphase();

	// Set up the collision configuration and dispatcher
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

	// The actual physics solver
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

	// The world.
	btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
	dynamicsWorld->setGravity(btVector3(0, -1, 0));


	btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 0, 0), 1);

	btCollisionShape* fallShape = new btSphereShape(1);

	btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -20, 0)));
	btRigidBody::btRigidBodyConstructionInfo
		groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
	btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
	dynamicsWorld->addRigidBody(groundRigidBody);

	btDefaultMotionState* fallMotionState =
		new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 3, 0)));

	btVector3 fallInertia(0, 0, 0);
	fallShape->calculateLocalInertia(mass, fallInertia);
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, fallShape, fallInertia);
	btRigidBody* fallRigidBody = new btRigidBody(fallRigidBodyCI);
	fallRigidBody->setLinearVelocity(btVector3(0, 0, 0));

	btScalar transMatrix[16];

	//float radie = 0.93*0.2;

	//setValues

	for (int i = 0; i < 20; i++)
	{
		//std::cout << theLeaves[i].getFlutter(0.f);
		float randNumbX = rand() % 10000 / 100 - 50;
		float randNumbY = rand() % 10000 / 100 - 50;
		float randNumbZ = rand() % 10000 / 100 - 50;
		Leaf newLeaf(i);
		poss = btVector3(randNumbX, randNumbY, randNumbZ);
		angularVel = btVector3(randNumbX, randNumbY, randNumbZ);
		//flu = 0;
		
		
		newLeaf.setValues(mass, area, dens, airCoeff, poss, flu, fallRigidBody,angularVel); //i/100=the x translation
		dynamicsWorld->addRigidBody(newLeaf.getFallingBody());
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

		//berŠkna luftmostŒnd
		dynamicsWorld->stepSimulation(1 / 100.f, 100000);
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
		//

		glPushMatrix();
		fallRigidBody->applyCentralForce(vind);
			
			for (std::vector<Leaf>::iterator it = theLeaves.begin(); it != theLeaves.end(); ++it)
			{ 

				glPushMatrix();
					
					glScalef(0.01f, 0.01f, 0.01f);

					btVector3 pos = it->getPosition();
					
					std::cout << *it->getFlutter(it->getRotation()) << '\n';
					
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
		//myLeaf.drawLeaf();
		
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	//destruktorer
	glfwDestroyWindow(window);
	glfwTerminate();

	dynamicsWorld->removeRigidBody(fallRigidBody);
	delete fallRigidBody->getMotionState();
	delete fallRigidBody;

	dynamicsWorld->removeRigidBody(groundRigidBody);
	delete groundRigidBody->getMotionState();
	delete groundRigidBody;


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




