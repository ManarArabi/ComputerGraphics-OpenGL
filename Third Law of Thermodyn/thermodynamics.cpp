// thermodynamics.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <stdlib.h>
#include <cstdlib>
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#include "color.h"
#include "Molecular.h"
#include <time.h>
#include <chrono>

using namespace std;
/***********************************global variables******************************************/
int degree = 4; //0......--//
const int maxDegree = 5;
const int moleNum = 60;
Color tempColors[maxDegree];
Molecular moleculars[moleNum];
float _angle = -70.;
/********************************prototypes************************************************/

void molecularInit();
void drawMoles();

void termometer();
void termometerColorsInit();

void drawFilledCircle(GLfloat x, GLfloat y, GLfloat z, GLfloat radius);
void drawSphere(GLfloat x, GLfloat y, GLfloat z);

void resetColor();
void processSpecialKeys(int key, int mx, int my);
void drawScene();
void handleKeypress(unsigned char key, int x, int y);
void initRendering();
void handleResize(int w, int h);
void update(int value);


/************************************************************/
int main(int argc, char** argv) {

	//Initialize GLUT
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(400, 400);
	//Create the window
	glutCreateWindow("Thermodynamics");
	initRendering();

	molecularInit();

	//Set handler functions
	glutDisplayFunc(drawScene);
	glutKeyboardFunc(handleKeypress);
	glutReshapeFunc(handleResize);
	glutSpecialFunc(processSpecialKeys);
	glutTimerFunc(25, update, 0); //Add a timer
	glutMainLoop();
	return 0;
}


/********************************************Defination*****************************************/
void molecularInit() {

	for (float i = 0; i < moleNum; i++)
	{
		moleculars[(int)i].randmize();
		//cout << i << "\t" << moleculars[i].posX << "\t" << moleculars[i].posY << "\t" << moleculars[i].posZ << endl;
		//cout << i / moleNum << "\t" << 1 - (i / moleNum) << endl;
		moleculars[(int)i].color.setColors(i / moleNum, 1 - (i / moleNum), 1);
	}

}

void drawMoles() {
	molecularInit();
	resetColor();
	glPushMatrix();
	glTranslatef(0, 0.5, 0);
	glColor3f(0, 0, 0);
	for (int i = 0; i < moleNum; i++) {
		glColor3f(moleculars[i].color.red, moleculars[i].color.green, moleculars[i].color.blue);
		drawSphere(moleculars[i].posX, moleculars[i].posY, moleculars[i].posZ);
		resetColor();
		if (degree != 0) {
			glRotatef(_angle, 1.0f, 1.0f, 1.0f);
			Sleep(maxDegree - degree);
		}
	}
	glPopMatrix();
}



void termometer() {

	termometerColorsInit();
	glPushMatrix();
	//Add ambient light
	GLfloat ambientColor[] = { 1.f, 1.f, 1.f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
	glTranslatef(-6, -1.5, 0.);
	glBegin(GL_QUADS);
	glColor3f(tempColors[degree].red, tempColors[degree].green, tempColors[degree].blue);
	glVertex3f(2.5, 0.2, -5.);
	glVertex3f(2.7, 0.2, -5.);
	glVertex3f(2.7, 3, -5.);
	glVertex3f(2.5, 3, -5.);
	glEnd();
	drawFilledCircle(2.6, .1, -5., .15);
	glPopMatrix();


}

void termometerColorsInit() {
	for (double i = 0; i < maxDegree; i += 1) { // i >> degree
		cout << i / maxDegree << "\t" << 1 - (i / maxDegree) << endl;
		tempColors[(int)i].setColors(i / maxDegree, 0, 1 - (i / maxDegree));
	}
}



void drawFilledCircle(GLfloat x, GLfloat y, GLfloat z, GLfloat radius) {
	int i;
	int triangleAmount = 70; //# of triangles used to draw circle

							 //GLfloat radius = 0.8f; //radius
	GLfloat twicePi = 2.0f * 3.14;

	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(x, y, z); // center of circle
	for (i = 0; i <= triangleAmount; i++) {
		glVertex3f(
			x + (radius * cos(i *  twicePi / triangleAmount)),
			y + (radius * sin(i * twicePi / triangleAmount))
			, z);
	}
	glEnd();
}

void drawSphere(GLfloat x, GLfloat y, GLfloat z) {

	//Add ambient light
	GLfloat ambientColor[] = { 0.5f, 0.5f, 0.5f, 1.0f }; //Color (0.2, 0.2, 0.2)
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
	//
	//Add positioned light
	GLfloat lightColor0[] = { 0.5f, 0.0f, 0.0f, 1.0f }; //Color (0.5, 0.5, 0.5)
	GLfloat lightPos0[] = { 4.0f, 0.0f, 8.0f, 1.0f }; //Positioned at (4, 0, 8)

	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);

	//Add directed light
	GLfloat lightColor1[] = { 0.5f, 0.2f, 0.2f, 1.0f }; //Color (0.5, 0.2, 0.2)
														//Coming from the direction (-1, 0.5, 0.5)
	GLfloat lightPos1[] = { -1.0f, 0.5f, 0.5f, 0.0f };

	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);

	//Specular light
	GLfloat specularColor[] = { 0.8f,0.8f,1.0f,1.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularColor);
	GLfloat shininess[] = { 100 };
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);

	// To add another spot of light   //
	// glLightfv(GL_LIGHT1,GL_SPECULAR, specularColor);
	glTranslatef(x, y, z);
	glRotatef(-70.f, 1.0f, 1.0f, 0.0f);
	//glColor3f(1.0f, 0.0f, 0.0f);
	glutSolidSphere(.5, 20., 20.);

}



void resetColor() {
	glColor3f(1, 1, 1);
}

void processSpecialKeys(int key, int mx, int my) {

	switch (key) {

	case GLUT_KEY_UP: //increase temp
		(degree == maxDegree - 1) ? degree = maxDegree - 1 : degree += 1;
		cout << degree << endl;
		glutPostRedisplay();
		break;
	case GLUT_KEY_DOWN: //decrease temp
		(degree == 0) ? degree = 0 : degree -= 1;
		cout << degree << endl;
		glutPostRedisplay();
		break;

	default:
		break;
	}


}

void handleKeypress(unsigned char key, int x, int y) {
	switch (key) {
	case 27: //Escape key
		exit(0);
	}
}

void drawScene() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_MODELVIEW); //Switch to the drawing perspective
	glLoadIdentity(); //Reset the drawing perspective
	termometer();
	

	glTranslatef(0., 0., -10.);
	if (degree != 0)   glRotatef(_angle, 0.0f, 1.0f, 1.0f);

	drawMoles();
	glutSwapBuffers();
}
//Initializes 3D rendering
void initRendering() {
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);

	glEnable(GL_LIGHTING); //Enable lighting

	glEnable(GL_LIGHT0); //Enable light #0
	glEnable(GL_LIGHT1); //Enable light #1

	glEnable(GL_NORMALIZE); //Automatically normalize normals
	glShadeModel(GL_SMOOTH); //Enable smooth shading

}
//Called when the window is resized
void handleResize(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, (double)w / (double)h, 1.0, 200.0);
}

void update(int value) {
	_angle += 1.5f;
	if (_angle > 360) {
		_angle -= 360;
	}

	glutPostRedisplay();
	glutTimerFunc(25, update, 0);
}
