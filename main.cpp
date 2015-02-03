#include <GLFW/glfw3.h>
#include <iostream>
#include <chrono>
#include <ctime>
#include <math.h>
#include <string>
#include <fstream>
#include <vector>

using namespace std;
double pos = 0;
void drawPolygon()
{
    glBegin(GL_POLYGON);
    glColor3f( 1.0, 0.0, 0.0 );
    glVertex3f(  0.5, -0.5, -0.5 );
    glColor3f( 1.0, 0.0, 0.0 );
    glVertex3f(  0.5,  0.5, -0.5 );
    glColor3f( 1.0, 0.0, 0.0 );
    glVertex3f( -0.5,  0.5, -0.5 );
    glColor3f( 1.0, 0.0, 0.0 );
    glVertex3f( -0.5, -0.5, -0.5 );
    glEnd();
}
void display()
{
    
    int m = 0.01;
    int g = 9.82;
    int  k = 1;
    double time = glfwGetTime();
    
    double speed = 19.1893 - 19.1893*exp(-0.5117*time);
    
    pos+=speed*time;
   // cout << speed << "  ";
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    
    glPushMatrix();
    glScalef(0.1f, 0.1f, 0.1f);
    glPushMatrix();
    glTranslatef(0, -pos/100, 0.0f);
    glPushMatrix();
    drawPolygon();
    glPopMatrix();
    glPopMatrix();
    glPopMatrix();
}

int main(void)
{
    GLFWwindow* window;

    
    /* Initialize the library */
    if (!glfwInit())
        return -1;
    
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(1152, 720, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    
    glEnable(GL_DEPTH_TEST);
    
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        
        display();
        glfwSwapBuffers(window);
        
        /* Swap front and back buffers */
        
        /* Poll for and process events */
        glfwPollEvents();
    }
    
    glfwTerminate();
    return 0;
    
    
}