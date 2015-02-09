
#define GLEW_STATIC

#ifdef WIN32
#include <GL/glew.h>
#else
// einar
#endif

#include <glfw3.h> // GLFW helper library
#include <stdio.h>
#include <math.h>
#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <ctime>
#include <cstdlib>
#include <BulletDynamics/btBulletDynamicsCommon.h>
#include <BulletDynamics/btBulletCollisionCommon.h>
#include <BulletDynamics/Dynamics/btDynamicsWorld.h>
#include <BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>

//B…RJA KIKA P BULLET! typ istŠllet fšr calculateLeafRotation... /einis

struct leaf{
	glm::vec3 pos, speed;
	float size, angle, weight;
	bool life;
};
leaf firstLeaf;

static void error_callback(int error, const char* description);

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

float getVelocity(float tid);

void calculateLeafRotation(float time);

void setPosition();

float getPositionY(float tid);

float getWind(float tid);

void drawTriangle();

void positionLog(float velo, float pos);
//void addWind();

using namespace std;
int main(void)
{
    // Build the broadphase
    btBroadphaseInterface* broadphase = new btDbvtBroadphase();
    
    // Set up the collision configuration and dispatcher
    btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
    btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
    
    // The actual physics solver
    btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
    
    // The world.
    btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
    dynamicsWorld->setGravity(btVector3(0, -10, 0));
    
    
    btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0, 1, 0), 1);
    
    btCollisionShape* fallShape = new btSphereShape(1);
    
    
    btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, -1, 0)));
    btRigidBody::btRigidBodyConstructionInfo
    groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
    btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);
  //  dynamicsWorld->addRigidBody(groundRigidBody);
    
    
    btDefaultMotionState* fallMotionState =
    new btDefaultMotionState(btTransform(btQuaternion(0, 0, 0, 1), btVector3(0, 3, 0)));
    btScalar mass = 0.1;
    btVector3 fallInertia(0, 0, 0);
    fallShape->calculateLocalInertia(mass, fallInertia);
    btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, fallMotionState, fallShape, fallInertia);
    btRigidBody* fallRigidBody = new btRigidBody(fallRigidBodyCI);
    fallRigidBody->setLinearVelocity(btVector3(0,0,0));
    
    dynamicsWorld->addRigidBody(fallRigidBody);
    
    btScalar m[16];

    
    
	float radie = 0.93*0.2;
	
	GLFWwindow* window;
	glfwSetErrorCallback(error_callback);
	
    if (!glfwInit())
		glfwTerminate();
	window = glfwCreateWindow(1280, 960, "Simple example", NULL, NULL);
	
    if (!window)
	{
		glfwTerminate();

	
    }
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glfwSetKeyCallback(window, key_callback);
	bool firstLoop = true;
	srand(time(NULL));
	
    while (!glfwWindowShouldClose(window))
	{
		float tid = (float)glfwGetTime();
		float ratio;
		int width, height;
		float velocity = getVelocity(tid);
        
        //berŠkna luftmostŒnd
        btVector3 velo = fallRigidBody->getLinearVelocity();
        
        double airCoeff = 1.28;
        double dens = 1.2041;
        double area = 0.0025;
        double airRes = pow(velo.getY(), 2)*airCoeff*dens*area;
        cout << airRes << " ";
        
        fallRigidBody->applyCentralImpulse( btVector3( 0.f, airRes, 0.f ) );

		//float velo = dist / tid;
		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		/*if (firstLoop){
			firstLeaf.angle = (rand() % 100); 
			setPosition();
        }*/

		int speedScale = 10000;

		glPushMatrix();
        
		glScalef(0.2f, 0.2f, 0.2f);
        

        dynamicsWorld->stepSimulation(1 / 100.f, 1000);
            
        btTransform trans;
        fallRigidBody->getMotionState()->getWorldTransform(trans);
        trans.getOpenGLMatrix(m);
            
        glMultMatrixf((GLfloat*)m);
		//glTranslatef(0.f, firstLeaf.pos.y / speedScale, 0.f);
	/*	if (firstLoop){
			glRotatef(firstLeaf.angle, 1.f, 0.f, 1.f);
		}
		else
        {
			calculateLeafRotation(tid);
		}*/
		
        //glRotatef(getVelocity(tid)*radie*10*tid, 4.f, 4.f, 10.f);;
		drawTriangle();
		glPopMatrix();
	//	firstLeaf.pos.y += getPositionY(tid);
		glfwSwapBuffers(window);
		glfwPollEvents();
		firstLoop = false;
    }
	glfwDestroyWindow(window);
	glfwTerminate();
    
    dynamicsWorld->removeRigidBody(fallRigidBody);
    delete fallRigidBody->getMotionState();
    delete fallRigidBody;
    
    dynamicsWorld->removeRigidBody(groundRigidBody);
    delete groundRigidBody->getMotionState();
    delete groundRigidBody;
    
    
    delete fallShape;
    
    delete groundShape;
    
    
    delete dynamicsWorld;
    delete solver;
    delete collisionConfiguration;
    delete dispatcher;
    delete broadphase;

}


float getVelocity(float tid){
	float velocity;
	velocity = (float)(19.1893 - 19.1893*exp(-0.5117*tid));
	return velocity;
}

float getPositionY(float tid){

	float position;
	position = tid*getVelocity(tid);
	return -position;
}


float getWind(float tid){
	int randomer = tid;

	float windForce = 78560 / 17 - (78560 * exp(-(17 * tid) / 8000)) / 17;
	if (randomer % 2 == 0){ windForce = -windForce; }
	return windForce;
}

void setPosition(){
	
	float RandomPosX = rand() % 10000 - 5000;
	float RandomPosY = rand() % 10000 - 5000;
	float RandomPosZ = rand() % 10000 - 5000;
	float randomX = RandomPosX / 1000;
	float randomY = RandomPosY / 1000;
	float randomZ = RandomPosZ / 1000;
	firstLeaf.pos.x = randomX;
	firstLeaf.pos.y = randomY;
	firstLeaf.pos.y = randomZ;
}

void drawTriangle(){
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_TRIANGLES);
	glColor3f(0.f, 1.f, 0.f);
	glVertex3f(-0.6f, -0.4f, 0.f);
	glColor3f(0.f, 1.f, 0.f);
	glVertex3f(0.6f, -0.4f, 0.f);
	glColor3f(0.f, 1.f, 0.f);
	glVertex3f(0.f, 1.0f, 0.f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void positionLog(float velo, float pos){
	std::cout << velo << " ";
	std::cout << pos << " " << '\n';
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

void calculateLeafRotation(float time){

	float vinklar[3] = { -45, 90, 225 };
	//0.93 i radie
	//vinklar -45, +90, +225
}



/*
#define GLEW_STATIC
#include <GL/glew.h>
#include <glfw3.h> // GLFW helper library
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <ctime>
#include <cstdlib>


struct leaf{
	glm::vec3 pos, speed;
	float size, angle, weight;
	bool life;
	float cameraDist;
};

const int maxleaf = 100;
leaf leafBuffer[maxleaf];
int lastUsedParticle = 0;


static void error_callback(int error, const char* description);

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

float getVelocity(float tid);


void setPosition(int i);

float getPositionY(float tid);

float getWind(float tid);

void drawTriangle();

void positionLog(float velo, float pos);
//void addWind();
int main(void)
{
	
	GLuint TexID1;
	float windForceX = 0;
	float windForceY = 0;
	float windForceZ = 0;


	float pos[maxleaf] = { 0 };
	GLFWwindow* window;
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		glfwTerminate();
	window = glfwCreateWindow(1280, 960, "Simple example", NULL, NULL);
	if (!window)
	{
		glfwTerminate();

	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glfwSetKeyCallback(window, key_callback);
	bool firstLoop = true;
	srand(time(NULL));
	while (!glfwWindowShouldClose(window))
	{
		float tid = (float)glfwGetTime();
		float ratio;
		int width, height;
		float velocity = getVelocity(tid);
		
		//float velo = dist / tid;
		glfwGetFramebufferSize(window, &width, &height);
		ratio = width / (float)height;
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		
		
		int newparticles = (int)(tid*10000.0);
		if (newparticles > (int)(0.016f*10000.0))			
			newparticles = (int)(0.016f*10000.0);
		int speedScale = 10000;
		for (int i = 0; i < maxleaf; i++){
			
			glPushMatrix();
				glScalef(0.1f, 0.1f, 0.1f);
				if (firstLoop){	
					setPosition(i);
				}
				else{
					int tal = tid;
					if (tal%2  == 0){
						leafBuffer[i].pos.x += getPositionY(tid) / speedScale + getWind(tid)/speedScale;
						leafBuffer[i].pos.y += getPositionY(tid) / speedScale + getWind(tid) / speedScale;;
						leafBuffer[i].pos.y += getPositionY(tid) / speedScale + getWind(tid) / speedScale;;
					}
					leafBuffer[i].pos.y += getPositionY(tid)/speedScale;
					//positionLog(velocity, pos[i]);
				}
				//glRotatef(x, y, z); //lövets rotation med avseende på wind
				glTranslatef(leafBuffer[i].pos.x, leafBuffer[i].pos.y, leafBuffer[i].pos.z);
				drawTriangle();
			glPopMatrix();
		}
		glfwSwapBuffers(window);
		glfwPollEvents();
		firstLoop = false;
	}
	glfwDestroyWindow(window);
	glfwTerminate();

}


float getVelocity(float tid){
	float velocity;
	velocity = (float)(19.1893 - 19.1893*exp(-0.5117*tid));
	return velocity;
}

float getPositionY(float tid){
	
	float position;
	position = tid*getVelocity(tid);
	return -position;
}


float getWind(float tid){
	int randomer = tid;

	float windForce = 78560 / 17 - (78560 * exp(-(17 * tid) / 8000)) / 17;
	if (randomer % 2 == 0){ windForce = -windForce; }
	return windForce;
}

void setPosition(int i){
	float RandomPosX = rand() % 10000 - 5000;
	float RandomPosY = rand() % 10000 - 5000;
	float RandomPosZ = rand() % 10000 - 5000;
	float randomX = RandomPosX / 1000;
	float randomY = RandomPosY / 1000;
	float randomZ = RandomPosZ / 1000;
	std::cout << randomX << ' - ' << randomY << ' - ' << randomZ << '\n';
	leafBuffer[i].pos.x = randomX;
	leafBuffer[i].pos.y = randomY;
	leafBuffer[i].pos.z = randomZ;
	std::cout << leafBuffer[i].pos.y << '\t';
}

void drawTriangle(){
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_TRIANGLES);
	glColor3f(0.f, 1.f, 0.f);
	glVertex3f(-0.6f, -0.4f, 0.f);
	glColor3f(0.f, 1.f, 0.f);
	glVertex3f(0.6f, -0.4f, 0.f);
	glColor3f(0.f, 1.f, 0.f);
	glVertex3f(0.f, 1.0f, 0.f);
	glEnd();
	glDisable(GL_TEXTURE_2D);
}

void positionLog(float velo, float pos){
	std::cout << velo << " ";
	std::cout << pos << " " << '\n';
}


static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}*/