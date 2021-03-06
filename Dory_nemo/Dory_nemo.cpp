#include "stdafx.h"
#include <iostream>
#include <stdlib.h>
#include <iostream>
#include <GL/glut.h>
#include <GL/GLU.h>
#include <GL/GL.h>
#include "imageloader.h"
using namespace System;

void handleKeypress(unsigned char key, int x, int y) {
	switch (key) {
	case 27: //Escape key
		exit(0);
	}
}

//Makes the image into a texture, and returns the id of the texture
GLuint loadTexture(Image* image) {
	GLuint textureId;
	glGenTextures(1, &textureId); //Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
	//Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
		0,                            //0 for now
		GL_RGB,                       //Format OpenGL uses for image
		image->width, image->height,  //Width and height
		0,                            //The border of the image
		GL_RGB,				//GL_RGB, because pixels are stored in RGB format
		GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
						  //as unsigned numbers
		image->pixels);               //The actual pixel data
	return textureId; //Returns the id of the texture
}

GLuint _textureSea; //The id of the texture
GLuint _textureNemo; //The id of the texture
GLuint _textureDory; //The id of the texture

Image* image;
void initRendering() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-20.0, 20.0, -20.0, 20.0, 10, -10);
	
	image = loadBMP("nemo.bmp");
	_textureNemo = loadTexture(image);
	
	image = loadBMP("Dory_FD.bmp");
	_textureDory = loadTexture(image);
	
	image = loadBMP("sea.bmp");
	_textureSea = loadTexture(image);
	
	delete image;

}

void handleResize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (float)w / (float)h, 1.0, 200.0);
}

float x_axis = 0.0;
float a = 0.0;
float z = 0.0;
void mySpecialFunc(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_RIGHT:
		x_axis += 0.05;
		z += 0.05;
		if (x_axis > 5.0 || z > 5.0) {
			x_axis = 0.0;
			z = 0.0;
		}
		break;
	case GLUT_KEY_LEFT:
		x_axis -= 0.05;
		z -= 0.05;
		if (x_axis < -5.0|| z < -5.0) {
			x_axis = 0.0;
			z = 0.0;
		}
		break;
	case GLUT_KEY_DOWN:
		a += 1.0;
		if(a > 360)
			a = 0.0;
		break;
	case GLUT_KEY_UP:
		a -= 1;
		if(a > 360)
			a = 0.0;
		break;
	}
	glutPostRedisplay();
}

void drawScene() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, 1.0f, -6.0f);

	GLfloat ambientLight[] = { 0.6f, 0.6f, 0.6f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);

	GLfloat directedLight[] = { 1.0f, 0.0f, 0.0f, 1.0f };
	GLfloat directedLightPos[] = { -16.0f, 0.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, directedLight);
	glLightfv(GL_LIGHT0, GL_POSITION, directedLightPos);



	glEnable(GL_TEXTURE_2D);
	
	//Dory
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, _textureDory);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTranslatef(0, 0, -6);
	glTranslatef(x_axis, 0.0, z);
	glRotatef(a, 0.0, 1.0, 0.0);
	glBegin(GL_QUADS);
	glTexCoord3f(0.0, 1.0, 0.1);  glVertex3f(0, 0, 3);
	glTexCoord3f(1.0, 1.0, 0.1);  glVertex3f(2, 0, 3);
	glTexCoord3f(1.0, 0.0, 0.1);  glVertex3f(2, -1.5, 3);
	glTexCoord3f(0.0, 0.0, 0.1);  glVertex3f(0, -1.5, 3);
	glEnd();
	glPopMatrix();
	
	// Nemo
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, _textureNemo);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTranslatef(0, 0, -3);
	glTranslatef(x_axis, 0.0, z);
	glRotatef(a, 0.0, 1.0, 0.0);
	glBegin(GL_QUADS);  
	glNormal3f(-16, 0, 0);
	glTexCoord3f(0.0, 1.0, 0.1);  glVertex3f(-3, 0, 1);
	glTexCoord3f(1.0, 1.0, 0.1);  glVertex3f(-1, 0, 1);
	glTexCoord3f(1.0, 0.0, 0.1);  glVertex3f(-1, -1.5, 1);
	glTexCoord3f(0.0, 0.0, 0.1);  glVertex3f(-3, -1.5, 1);
	glEnd();
	glPopMatrix();
	
	// Sea
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, _textureSea);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTranslatef(0, 0, -12);
	glBegin(GL_QUADS);
	glTexCoord3f(0.0, 1.0, 0.1);  glVertex3f(-12, 10, -10);
	glTexCoord3f(1.0, 1.0, 0.1);  glVertex3f(12, 10, -10);
	glTexCoord3f(1.0, 0.0, 0.1);  glVertex3f(12, -10, -10);
	glTexCoord3f(0.0, 0.0, 0.1);  glVertex3f(-12, -10, -10);
	glEnd();
	glPopMatrix();

	glutSwapBuffers();

	glFlush();
	glDisable(GL_TEXTURE_2D);
}

int main(int argc, char** argv) {


	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(600, 600);
	glutCreateWindow("Nemo&Dory");
	initRendering();

	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutReshapeFunc(handleResize);
	glutSpecialFunc(mySpecialFunc);

	glutMainLoop();
	return 0;
}