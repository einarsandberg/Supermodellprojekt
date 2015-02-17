#define GLEW_STATIC
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>

#include <GL/glew.h>

#include <GL/glfw3.h>
#include <SOIL.h>
#pragma warning (disable : 4996)

void loadTexture(const char * imagepath, GLuint shaderProgram, const char * name, int i){

	printf("Reading image %s\n", imagepath);


	int width, height;
	unsigned char* image;

	image = SOIL_load_image(imagepath, &width, &height, 0, SOIL_LOAD_RGB);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
	SOIL_free_image_data(image);
	glUniform1i(glGetUniformLocation(shaderProgram, name), i);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);



}