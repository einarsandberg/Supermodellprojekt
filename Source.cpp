// Link statically with GLEW
#define GLEW_STATIC
// Headers
//#ifndef GLM_GTX_string_cast
//#define GLM_GTX_string_cast

#include <windows.h>
#include <GL/glew.h>
#include <GL/glfw3.h>
GLFWwindow* window;
#include <iostream>
#include <ctime>
#include <SOIL.h>
#include <vector>
#include <string>

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include "BulletDynamics\Dynamics\btDynamicsWorld.h"
#include "BulletCollision\Gimpact\btGImpactCollisionAlgorithm.h"


#include "Leaf.h"
#include "World.h"


// Include GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/norm.hpp>
using namespace glm;

// Include AntTweakBar
#include <common/AntTweakBar.h>

using namespace glm;
#include <common/shader.hpp>
#include <common/controls.hpp>
#include <common/objloader.hpp>
#include <common/vboindexer.hpp>
#include <common/quaternion_utils.hpp> // See quaternion_utils.cpp for RotationBetweenVectors, LookAt and RotateTowards

vec3 gPosition1(-1.5f, 0.0f, 0.0f);
vec3 gOrientation1;

vec3 gPosition2(1.5f, 0.0f, 0.0f);
quat gOrientation2;

bool gLookAtOther = true;


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

	// Initialize the GUI
	TwInit(TW_OPENGL_CORE, NULL);
	TwWindowSize(1024, 768);
	TwBar * EulerGUI = TwNewBar("Euler settings");
	TwBar * QuaternionGUI = TwNewBar("Quaternion settings");
	TwSetParam(EulerGUI, NULL, "refresh", TW_PARAM_CSTRING, 1, "0.1");
	TwSetParam(QuaternionGUI, NULL, "position", TW_PARAM_CSTRING, 1, "808 16");

	TwAddVarRW(EulerGUI, "Euler X", TW_TYPE_FLOAT, &gOrientation1.x, "step=0.01");
	TwAddVarRW(EulerGUI, "Euler Y", TW_TYPE_FLOAT, &gOrientation1.y, "step=0.01");
	TwAddVarRW(EulerGUI, "Euler Z", TW_TYPE_FLOAT, &gOrientation1.z, "step=0.01");
	TwAddVarRW(EulerGUI, "Pos X", TW_TYPE_FLOAT, &gPosition1.x, "step=0.1");
	TwAddVarRW(EulerGUI, "Pos Y", TW_TYPE_FLOAT, &gPosition1.y, "step=0.1");
	TwAddVarRW(EulerGUI, "Pos Z", TW_TYPE_FLOAT, &gPosition1.z, "step=0.1");

	TwAddVarRW(QuaternionGUI, "Quaternion", TW_TYPE_QUAT4F, &gOrientation2, "showval=true open=true ");
	TwAddVarRW(QuaternionGUI, "Use LookAt", TW_TYPE_BOOL8, &gLookAtOther, "help='Look at the other monkey ?'");

	

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	
	GLfloat vertices[] = {
		//shape				//color			//uv-coord
		-0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,	//löv
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
	
	
	//sätter alla variabler
	double airCoeff = 1.28;
	double dens = 1.2041;
	double area = 0.0025;
	double mass = 0.1;
	double life = 3.0;
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
	for (int i = 0; i < 100; i++)
	{
		float randNumbX = rand() % 10 - 5;
		float randNumbY = rand() % 10 - 5;
		float randNumbZ = rand() % 10 - 5;
		pos = btVector3(randNumbX, randNumbY, randNumbZ);
		startAngle = btVector3(randNumbX*10, randNumbY*5, randNumbZ*10);
		Leaf newLeaf(mass, area, dens, airCoeff, pos, flu, startAngle, life);


		theLeaves.push_back(newLeaf);

		theWorld.getDynamicsWorld()->addRigidBody(newLeaf.getBody());

	}
	
	double lastTime = glfwGetTime();
	//double lastFrameTime = lastTime;
	
	do{
		double currentTime = glfwGetTime();
		double deltaTime = currentTime - lastTime;
		lastTime = currentTime;

		glEnable(GL_DEPTH_TEST);
		//glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		//glDepthMask(GL_TRUE);
		//glDisable(GL_CULL_FACE);
		
		// Dark blue background
		glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
		//glClearDepth(1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
		//float time = (float)glfwGetTime();
		
		computeMatricesFromInputs();
		glm::mat4 ProjectionMatrix = getProjectionMatrix();
		glm::mat4 ViewMatrix = getViewMatrix();
		glm::mat4 ModelMatrix = glm::mat4(1.0);
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		/*
		for (int i = 0; i < 4; i++){
			std::cout << ViewMatrix[i].x << " ";
			std::cout << ViewMatrix[i].y << " ";
			std::cout << ViewMatrix[i].z << " ";
			std::cout << ViewMatrix[1].w << " "; 
			std::cout << std::endl;
		}*/

		std::cout << endl;
		//std::cout << glm::to_string(ViewMatrix) << std::endl << endl << endl;


		glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);


		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glm::mat4 skal = glm::scale( 2.0f, 2.0f, 2.0f);
		

		glm::mat4 rot = glm::rotate(90.0f, 0.0f, 0.0f, 1.0f);

		glm::mat4 model = skal*rot;

		glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

		//draw plane
		glDrawArrays(GL_TRIANGLES, 6, 18);



		float ratio;
		int width, height;
		//float velocity = getVelocity(tid);

		theWorld.getDynamicsWorld()->stepSimulation(1 / 100.f, 100000);
		btVector3 velo;
		double airRes = 0;
		btTransform trans;
		btTransform trans_local;

		//sätter krafter på varje löv
		for (std::vector<Leaf>::iterator it = theLeaves.begin(); it != theLeaves.end(); ++it)
		{
			
			if (it->getBody()->getCenterOfMassPosition().getY()> -5.0f){


					glTranslatef(it->getFlutter(it->getRotation()).getX(), it->getFlutter(it->getRotation()).getY(), 0);

					it->getBody()->setAngularVelocity(it->getRotation());

					velo = it->getBody()->getLinearVelocity();

					airRes = it->getAirResistance(velo, area, dens);

					it->getBody()->applyCentralForce(btVector3(0.f, airRes, 0.f));
					it->getBody()->getMotionState()->getWorldTransform(trans);

					trans.getOpenGLMatrix(transMatrix);




					glUniformMatrix4fv(uniModel, 1, GL_FALSE, transMatrix);

					glMultMatrixf((GLfloat*)transMatrix);

					//löv
					glDrawArrays(GL_TRIANGLES, 0, 6);
					glUniform3f(uniColor, 1.0f, 1.0f, 1.0f);


					std::cout << "Y-pos: " << theLeaves.at(2).getPosition().getY() << endl;

				}
				else{

					it->setPosition(btVector3(0, 10, 0));
					//it->life = 3.0;
				}
			
		}
		

		
		if (gLookAtOther){
			vec3 desiredDir = gPosition1 - gPosition2;
			vec3 desiredUp = vec3(0.0f, 1.0f, 0.0f); // +Y

			// Compute the desired orientation
			quat targetOrientation = normalize(LookAt(desiredDir, desiredUp));

			// And interpolate
			gOrientation2 = RotateTowards(gOrientation2, targetOrientation, 1.0f*deltaTime);
		}
		

		//Draw GUI
		//TwDraw();
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


	// Close GUI and OpenGL window, and terminate GLFW
	TwTerminate();
	glfwTerminate();
	return 0;

}