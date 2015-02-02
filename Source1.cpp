#include <GLFW/glfw3.h> // GLFW helper library
#include <stdio.h>
#include <math.h>
#include <iostream>
void drawCube();
static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}


int main(void)
{	
	float pos = 0;
	GLFWwindow* window;
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
		glfwTerminate();
	window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
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
		
		float velo = (float)(491.0f / 500.0f)*(1 - exp(-time));
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
		std::cout << velo << " ";
		std::cout << pos << " " << '\n';
		
		pos += velo*time;
		glPushMatrix();
		glScaled(0.2f, 0.2f, 0.2f);
		glTranslatef(0.0, -pos/5000, 0.0);
		
		//glRotatef(velo*4, 0.f, 0.f, 1.f);
			//drawCube();

			glBegin(GL_TRIANGLES);
			glColor3f(1.f, 0.f, 0.f);
			glVertex3f(-0.6f, -0.4f, 0.f);
			glColor3f(0.f, 1.f, 0.f);
			glVertex3f(0.6f, -0.4f, 0.f);
			glColor3f(0.f, 0.f, 1.f);
			glVertex3f(0.f, 0.6f, 0.f);
			glEnd();
		glPopMatrix();

		glfwSwapBuffers(window);
		drawCube();
		glfwPollEvents();
		
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	
}


void drawCube()
{
	GLfloat vertices[] =
	{
		-1, -1, -1, -1, -1, 1, -1, 1, 1, -1, 1, -1,
		1, -1, -1, 1, -1, 1, 1, 1, 1, 1, 1, -1,
		-1, -1, -1, -1, -1, 1, 1, -1, 1, 1, -1, -1,
		-1, 1, -1, -1, 1, 1, 1, 1, 1, 1, 1, -1,
		-1, -1, -1, -1, 1, -1, 1, 1, -1, 1, -1, -1,
		-1, -1, 1, -1, 1, 1, 1, 1, 1, 1, -1, 1
	};

	GLfloat colors[] =
	{
		0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0,
		1, 0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0,
		0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 0,
		0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0,
		0, 0, 0, 0, 1, 0, 1, 1, 0, 1, 0, 0,
		0, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1
	};

	static float alpha = 0;
	//attempt to rotate cube
	//glRotatef(alpha, 0, 1, 0);

	/* We have a color array and a vertex array */
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glColorPointer(3, GL_FLOAT, 0, colors);

	/* Send data : 24 vertices */
	glDrawArrays(GL_QUADS, 0, 24);

	/* Cleanup states */
	glDisableClientState(GL_COLOR_ARRAY);
	glDisableClientState(GL_VERTEX_ARRAY);
	alpha += 1;
}