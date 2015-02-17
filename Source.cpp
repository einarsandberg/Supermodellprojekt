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
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include "BulletDynamics\Dynamics\btDynamicsWorld.h"
#include "BulletCollision\Gimpact\btGImpactCollisionAlgorithm.h"

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
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	
	window = glfwCreateWindow(1000, 700, "Tutorial 01", NULL, NULL);
	if (window == NULL){
		fprintf(stderr, "Failed to open GLFW window.");
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSwapInterval(1);

	// Initialize GLEW
	glewExperimental = true; // Needed for core profile
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	


	GLfloat vertices[] = {
		//leaf
		//shape				//color			//uv-coord
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,		//botten
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

		-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,		//topp
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

		-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,	
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,	
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		-0.5f, -0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,

		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, -0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 1.0f,
		
						
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
	glUseProgram(shaderProgram);
	

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
		glm::vec3(2.5f, 2.5f, 2.0f),
		glm::vec3(0.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 1.0f)
		);

	glm::mat4 proj = glm::perspective(45.0f, 800.0f / 600.0f, 1.0f, 10.0f);

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

	for (int i = 0; i < 70; i++)
	{
		float randNumbX = rand() % 10000 / 100 - 50;
		float randNumbY = rand() % 10000 / 100 - 50;
		float randNumbZ = rand() % 10000 / 100 - 50;
		Leaf newLeaf(i);
		poss = btVector3(randNumbX, randNumbY, randNumbZ);
		angularVel = btVector3(randNumbX, randNumbY, randNumbZ);
		//flu = 0;


		newLeaf.setValues(mass, area, dens, airCoeff, poss, flu, angularVel); //i/100=the x translation
		theWorld.getDynamicsWorld()->addRigidBody(newLeaf.getFallingBody());
		theLeaves.push_back(newLeaf);
	}
	
	
	do{
		
		//glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		//glDepthMask(GL_TRUE);
		//glDisable(GL_CULL_FACE);
		
		// Dark blue background
		glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
		//glClearDepth(1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
		

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
		glDrawArrays(GL_TRIANGLES, 36, 6);


		model = glm::rotate(
			model,
			(GLfloat)clock() / (GLfloat)CLOCKS_PER_SEC * -180.0f,
			glm::vec3(0.0f, 0.0f, 1.0f)
			);
		glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

		// Draw cube
		glDrawArrays(GL_TRIANGLES, 0, 36);
		
		//nödvändig för färg
		glUniform3f(uniColor, 1.0f, 1.0f, 1.0f);

		 
		

		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
	glfwWindowShouldClose(window) == 0);

	glDeleteBuffers(1, &vbo);
	glDeleteVertexArrays(1, &vao);
	glDeleteProgram(shaderProgram);
	glDeleteTextures(1, textures);

	glfwTerminate();
	return 0;

}