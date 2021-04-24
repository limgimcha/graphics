#include <string>
#include <stdlib.h>
#include <windows.h>
#include <vector>

#include "bmpfuncs.h"
#include "skybox.h"
#include <string>

#include <GL/glut.h>
#include <GL/freeglut.h>
#include "GL/glext.h"

using namespace std;

GLuint texName[6];
GLuint texNameC[3];
GLuint texBoard[1];


GLUquadricObj* qobj = gluNewQuadric();


void cubeTextureMapping(int num) {
	glGenTextures(6, texName);
	int imgWidth, imgHeight, channels;
	for (int i = 0; i < 6; i++) {
		glBindTexture(GL_TEXTURE_2D, texName[i]);
		char buf[100];
		sprintf_s(buf, "img/%d.bmp", i+ num*6);
		buf[strlen(buf)] = 0;
		uchar* img = readImageData(buf, &imgWidth, &imgHeight, &channels);
		glTexImage2D(GL_TEXTURE_2D, 0, 3, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	}
};

void draw_textureCube(bool sw, float move) {

	glColor3f(1.0f, 1.0f, 1.0f);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	if (sw == false) {
		glDisable(GL_LIGHTING);
	}
	else if (sw == true) {
		glEnable(GL_LIGHTING);
	}
	int skyboxSize = 600* move;


	// Left
	glBindTexture(GL_TEXTURE_2D, texName[0]);
	glBegin(GL_QUADS);
	glNormal3f(1.0, 0, 0);
	glTexCoord2f(0, 0); glVertex3f(-skyboxSize, 0.7*skyboxSize, -skyboxSize);
	glTexCoord2f(1, 0); glVertex3f(-skyboxSize, -0.7 * skyboxSize, -skyboxSize);
	glTexCoord2f(1, 1); glVertex3f(-skyboxSize, -0.7 * skyboxSize, skyboxSize);
	glTexCoord2f(0, 1); glVertex3f(-skyboxSize, 0.7 * skyboxSize, skyboxSize);
	glEnd();

	// Right
	glBindTexture(GL_TEXTURE_2D, texName[1]);
	glBegin(GL_QUADS);
	glNormal3f(1.0, 0, 0);
	glTexCoord2f(0, 0); glVertex3f(skyboxSize, 0.7 * skyboxSize, skyboxSize);
	glTexCoord2f(1, 0); glVertex3f(skyboxSize, -0.7 * skyboxSize, skyboxSize);
	glTexCoord2f(1, 1); glVertex3f(skyboxSize, -0.7 * skyboxSize, -skyboxSize);
	glTexCoord2f(0, 1); glVertex3f(skyboxSize, 0.7 * skyboxSize, -skyboxSize);
	glEnd();

	// Bottom
	glBindTexture(GL_TEXTURE_2D, texName[2]);
	glBegin(GL_QUADS);
	glNormal3f(1.0, 0, 0);
	glTexCoord2f(0, 0); glVertex3f(-skyboxSize, -0.7 * skyboxSize, -skyboxSize);
	glTexCoord2f(1, 0); glVertex3f(skyboxSize, -0.7 * skyboxSize, -skyboxSize);
	glTexCoord2f(1, 1); glVertex3f(skyboxSize, -0.7 * skyboxSize, skyboxSize);
	glTexCoord2f(0, 1); glVertex3f(-skyboxSize, -0.7 * skyboxSize, skyboxSize);
	glEnd();

	// Top
	glBindTexture(GL_TEXTURE_2D, texName[3]);
	glBegin(GL_QUADS);
	glNormal3f(1.0, 0, 0);
	glTexCoord2f(0, 0); glVertex3f(-skyboxSize, 0.7 * skyboxSize, skyboxSize);
	glTexCoord2f(1, 0); glVertex3f(skyboxSize, 0.7 * skyboxSize, skyboxSize);
	glTexCoord2f(1, 1); glVertex3f(skyboxSize, 0.7 * skyboxSize, -skyboxSize);
	glTexCoord2f(0, 1); glVertex3f(-skyboxSize, 0.7 * skyboxSize, -skyboxSize);
	glEnd();

	// Near
	glBindTexture(GL_TEXTURE_2D, texName[4]);
	glBegin(GL_QUADS);
	glNormal3f(1.0, 0, 0);
	glTexCoord2f(0, 0); glVertex3f(skyboxSize, 0.7 * skyboxSize, skyboxSize);
	glTexCoord2f(1, 0); glVertex3f(-skyboxSize, 0.7 * skyboxSize, skyboxSize);
	glTexCoord2f(1, 1); glVertex3f(-skyboxSize, -0.7 * skyboxSize, skyboxSize);
	glTexCoord2f(0, 1); glVertex3f(skyboxSize, -0.7 * skyboxSize, skyboxSize);
	glEnd();

	
	// Far
	glBindTexture(GL_TEXTURE_2D, texName[5]);
	glBegin(GL_QUADS);
	glNormal3f(1.0, 0, 0);
	glTexCoord2f(0, 0); glVertex3f(skyboxSize, 0.7 * skyboxSize, -skyboxSize);
	glTexCoord2f(1, 0); glVertex3f(-skyboxSize, 0.7 * skyboxSize, -skyboxSize);
	glTexCoord2f(1, 1); glVertex3f(-skyboxSize, -0.7 * skyboxSize, -skyboxSize);
	glTexCoord2f(0, 1); glVertex3f(skyboxSize, -0.7 * skyboxSize, -skyboxSize);
	glEnd();
}


void draw_textureCylinder(float height, float radius) {
	glColor3f(0.5f, 0.5f, 0.5f);
	//glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	//// Top
	glBindTexture(GL_TEXTURE_2D, texNameC[0]);
	gluDisk(qobj, 0.5f, radius, 16, 4);

	// Side
	glBindTexture(GL_TEXTURE_2D, texNameC[1]);
	gluCylinder(qobj, radius, radius, height, 16, 16);

	//// Bottom
	glPushMatrix();
	glTranslatef(0.0, 0.0f, height);
	glBindTexture(GL_TEXTURE_2D, texNameC[2]);
	gluDisk(qobj, 0.5f, radius, 16, 4);
	glPopMatrix();
}


void cylinderTextureMapping() {
	int imgWidth, imgHeight, channels;
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glGenTextures(3, texNameC);

	// TOP
	glBindTexture(GL_TEXTURE_2D, texNameC[0]);
	uchar* img = readImageData("img/CIDER_T.bmp", &imgWidth, &imgHeight, &channels);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// CYLINDER
	glBindTexture(GL_TEXTURE_2D, texNameC[1]);
	img = readImageData("img/wheels.bmp", &imgWidth, &imgHeight, &channels);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// BOTTOM
	glBindTexture(GL_TEXTURE_2D, texNameC[2]);
	img = readImageData("img/CIDER_B.BMP", &imgWidth, &imgHeight, &channels);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
};


void draw_axis(void)
{
	glLineWidth(1.5f);
	glBegin(GL_LINES);

	glColor4f(1.f, 0.f, 0.f, 1.f);
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(4.f, 0.f, 0.f);

	glColor4f(0.f, 1.f, 0.f, 1.f);
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(0.f, 4.f, 0.f);

	glColor4f(0.f, 0.f, 1.f, 1.f);
	glVertex3f(0.f, 0.f, 0.f);
	glVertex3f(0.f, 0.f, 4.f);

	glEnd();
	glLineWidth(1);
}


void textureBackground() {
	glGenTextures(1, texBoard);
	int imgWidth, imgHeight, channels;
	glBindTexture(GL_TEXTURE_2D, texBoard[0]);
	uchar* img = readImageData("img/background.bmp", &imgWidth, &imgHeight, &channels);
	glTexImage2D(GL_TEXTURE_2D, 0, 1, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

void DrawBackground() {

	glPushAttrib(GL_ALL_ATTRIB_BITS);
	glDisable(GL_LIGHTING);
	glColor3f(1.0f, 1.0f, 1.0f);

	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, texBoard[0]);

	float backgroundSize = 800.0f;

	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);  glVertex2i(-backgroundSize, -backgroundSize);
	glTexCoord2f(1, 0); glVertex2i(backgroundSize, -backgroundSize);
	glTexCoord2f(1, 1); glVertex2i(backgroundSize, backgroundSize);
	glTexCoord2f(0, 1); glVertex2i(-backgroundSize, backgroundSize);
	glEnd();

	glPopAttrib();
}