#include <GLFW/glfw3.h>


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
    //double time = glfwGetTime();
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glTranslatef(0, -0.01f, 0.0f);
    
    drawPolygon();
    
    glFlush();
}



int main(void)
{
    GLFWwindow* window;
    
    /* Initialize the library */
    if (!glfwInit())
        return -1;
    
    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }
    
    glEnable(GL_DEPTH_TEST);
    
    /* Make the window's context current */
    glfwMakeContextCurrent(window);
    
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