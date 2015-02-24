// Link statically with GLEW
#define GLEW_STATIC
// Headers

#include <windows.h>
#include <GL/glew.h>
#include <GL/glfw3.h>
GLFWwindow* window;
#include <iostream>
#include <ctime>
#include <SOIL.h>
#include <vector>
#include <time.h>
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include "BulletDynamics\Dynamics\btDynamicsWorld.h"
#include "BulletCollision\Gimpact\btGImpactCollisionAlgorithm.h"


#include "Leaf.h"
#include "World.h"
#include <vector>

// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;
#include <common/shader.hpp>
#include <common/controls.hpp>



void loadTexture(const char * imagepath, GLuint shaderProgram, const char * name, int i);

int main()
{
	

	// Initialise GLFW
	if (!glfwInit())
	{
		fprintf(stderr, "Failed to initialize GLFW\n");
		return -1;
	}

	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	

	
	window = glfwCreateWindow(1000, 800, "Tutorial 01", NULL, NULL);
	if (window == NULL){
		fprintf(stderr, "Failed to open GLFW window.");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);
	glfwWindowHint(GLFW_REFRESH_RATE, 60);
	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	
	GLfloat vertices[] = {
		//shape				//color			//uv-coord
		-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,	//topp
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,

		-3.5f, 3.5f, 3.5f, 1.0f, 1.0f, 0.9f, 1.0f, 0.0f,	//golv
		-3.5f, 3.5f, -3.5f, 1.0f, 1.0f, 0.9f, 1.0f, 1.0f,
		-3.5f, -3.5f, -3.5f, 1.0f, 1.0f, 0.9f, 0.0f, 1.0f,
		-3.5f, -3.5f, -3.5f, 1.0f, 1.0f, 0.9f, 0.0f, 1.0f,
		-3.5f, -3.5f, 3.5f, 1.0f, 1.0f, 0.9f, 0.0f, 0.0f,
		-3.5f, 3.5f, 3.5f, 1.0f, 1.0f, 0.9f, 1.0f, 0.0f,
						
		- 3.5f, -3.5f, -3.5f, 1.0f, 1.0f, 0.9f, 1.0f, 1.0f,	//bakgrund
		3.5f, -3.5f, -3.5f, 1.0f, 1.0f, 0.9f, 1.0f, 0.0f,
		3.5f, 3.5f, -3.5f, 1.0f, 1.0f, 0.9f, 0.0f, 0.0f,
		3.5f, 3.5f, -3.5f, 1.0f, 1.0f, 0.9f, 0.0f, 0.0f,
		-3.5f, 3.5f, -3.5f, 1.0f, 1.0f, 0.9f, 0.0f, 1.0f,
		-3.5f, -3.5f, -3.5f, 1.0f, 1.0f, 0.9f, 1.0f, 1.0f


	};
	
	
	// Create Vertex Array Object
	GLuint vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	// Create a Vertex Buffer Object and copy the vertex data to it
	GLuint vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	//load shaders
	GLuint shaderProgram = LoadShaders("vertexShader.glsl", "fragmentShader.glsl");
	
	//useShader
	glUseProgram(shaderProgram);
	//GLFW_REFRESH_RATE(60.f);
	
	GLuint textures[2];
	glGenTextures(2, textures);

	
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textures[0]);
	loadTexture("sampleDog.png", shaderProgram, "texDog", 0);

	
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textures[1]);
	loadTexture("sampleLeaf.png", shaderProgram, "texLeaf", 1);
	

	

	// Set up projection
	glm::mat4 view = glm::lookAt(
		glm::vec3(5.5f, 5.5f, 50.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 50.0f)
		);

	glm::mat4 proj = glm::perspective(45.0f, 800.0f / 600.0f, 10.0f, 100.0f);

	//glm::mat4 MVP = proj * view;

	GLint MatrixID = glGetUniformLocation(shaderProgram, "MVP");

	GLint uniModel = glGetUniformLocation(shaderProgram, "model");

	GLint uniColor = glGetUniformLocation(shaderProgram, "overrideColor");

	
	
	// Specify the layout of the vertex data
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
	
	
	GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(3 * sizeof(GLfloat)));
	

	GLint texAttrib = glGetAttribLocation(shaderProgram, "texcoord");
	glEnableVertexAttribArray(texAttrib);
	glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (void*)(6 * sizeof(GLfloat)));
	// Compute the MVP matrix from keyboard and mouse input
	
	
	//sätter alla variabler
	double airCoeff = 1.28;
	double dens = 1.2041;
	double area = 0.0025;
	double mass = 0.1;
	btVector3 pos(0, 0, 0);
	btVector3 startAngle(0.f, 0.f, 0.f);
	btVector3  flu(0.0f, 0.0f, 0.0f);
	vector <Leaf> theLeaves;
	srand(time(NULL));
	btVector3 wind(0.0f, 1.0f, 0.0f);

	World theWorld;
	btScalar transMatrix[16];

	btVector3 airCurrent = wind + theWorld.getDynamicsWorld()->getGravity();
	airCurrent.normalized();
	
	btRigidBody* body;
	for (int i = 0; i < 600; i++)
	{
		float randNumbX = rand() % 10 - 5;
		float randNumbY = rand() % 10 - 5;
		float randNumbZ = rand() % 10 - 5;
		pos = btVector3(randNumbX, randNumbY, randNumbZ);
		startAngle = btVector3(randNumbX*10, randNumbY*5, randNumbZ*10);
		Leaf newLeaf(mass, area, dens, airCoeff, pos, flu, startAngle);


		theLeaves.push_back(newLeaf);

		theWorld.getDynamicsWorld()->addRigidBody(newLeaf.getBody());

	}
	
	

	do{
		glEnable(GL_DEPTH_TEST);
		//glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		//glDepthMask(GL_TRUE);
		//glDisable(GL_CULL_FACE);
		
		// Dark blue background
		glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
		//glClearDepth(1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
		float time = (float)glfwGetTime();
		
		computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		
		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);


		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		// getOpenGLMatrix();
		glm::mat4 model = glm::rotate(90.0f, 0.0f, 0.0f, 1.0f);
		
		glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

		//draw plane
	//	glDrawArrays(GL_TRIANGLES, 6, 12);

		/*
		model = glm::rotate(
			model,
			(GLfloat)clock() / (GLfloat)CLOCKS_PER_SEC * -180.0f,
			glm::vec3(0.0f, 0.0f, 1.0f)
			);
		glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
		*/

		float ratio;
		int width, height;
		//float velocity = getVelocity(tid);

		theWorld.getDynamicsWorld()->stepSimulation(1 / 100.f, 100000);
		btVector3 velo;
		double airRes = 0;
		btTransform trans;
		btTransform trans_local;
		// Measure speed
	

		for (std::vector<Leaf>::iterator it = theLeaves.begin(); it != theLeaves.end(); ++it)
		{
			

			//glScalef(0.01f, 0.01f, 0.01f);
	
			btVector3 pos = it->getPosition();
			//cout << *it->getFlutter(it->getRotation()) << '\n';
			
			glTranslatef(it->getFlutter(it->getRotation()).getX(), it->getFlutter(it->getRotation()).getY(),it->getFlutter(it->getRotation()).getZ());

			it->getBody()->setAngularVelocity(it->getRotation());

			velo = it->getBody()->getLinearVelocity();

			airRes = it->getAirResistance(velo, area, dens);

		//  getEffectiveArea()
			//it->getBody()->applyCentralForce(btVector3(1, 0, 2));
			it->getBody()->applyTorque(it->getRotation());
			it->getBody()->applyCentralForce(btVector3(0.f, airRes+it->getFlutter(it->getRotation()).getZ(), 0.f));
			it->getBody()->getMotionState()->getWorldTransform(trans);

			trans.getOpenGLMatrix(transMatrix);

			//std::cout<< time<<'\n';

			glUniformMatrix4fv(uniModel, 1, GL_FALSE, transMatrix);

			glMultMatrixf((GLfloat*)transMatrix);

			
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glUniform3f(uniColor, 1.0f, 1.0f, 1.0f);
			
		}
		

		
		
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
	glfwWindowShouldClose(window) == 0);

	glfwDestroyWindow(window);
	glfwTerminate();
	
	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	glDeleteProgram(shaderProgram);
	glDeleteTextures(1, textures);

	glfwTerminate();
	return 0;
}