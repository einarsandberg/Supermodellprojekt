#include <glfw3.h> // GLFW helper library
#include <stdio.h>
#include <math.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}
float getVelocity(float time);

float getPosition(float time);

void drawTriangle();

void positionLog(float velo, float pos);
//void addWind();
int main(void)
{
	glm::mat4 mat;

	float pos = 0;
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
	while (!glfwWindowShouldClose(window))
	{
		float time = (float)glfwGetTime();
		float ratio;
		int width, height;
		float wind = 0.2f;
		
		float velocity = getVelocity(time);
		pos += getPosition(time);
		//float velo = dist / time;

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
			glScaled(0.2f, 0.2f, 0.2f);
			glTranslatef(pos / 100, -pos / 100, 0.0f);
			drawTriangle();
		glPopMatrix();

		glfwSwapBuffers(window);
		glfwPollEvents();

	}
	glfwDestroyWindow(window);
	glfwTerminate();

}

float getVelocity(float time){
	float velocity;
	velocity = (float)(491.0f / 500.0f)*(1 - exp(-time));
	return velocity;
}
float getPosition(float time){
	
	float position;
	position = time*getVelocity(time);
	return position;
}

void drawTriangle(){
	glBegin(GL_TRIANGLES);
	glColor3f(1.f, 0.f, 0.f);
	glVertex3f(-0.6f, -0.4f, 0.f);
	glColor3f(0.f, 1.f, 0.f);
	glVertex3f(0.6f, -0.4f, 0.f);
	glColor3f(0.f, 0.f, 1.f);
	glVertex3f(0.f, 0.6f, 0.f);
	glEnd();
}

void positionLog(float velo, float pos){
	std::cout << velo << " ";
	std::cout << pos << " " << '\n';
}