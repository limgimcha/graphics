#include <string>
#include <stdlib.h>
#include <windows.h>
#include <vector>

#include "ObjParser.h"
#include "bmpfuncs.h"
#include "skybox.h"
#include "Airfunc.h"
#include "sound.h"
#include <math.h>

#include <GL/glut.h>
#include <GL/freeglut.h>
#include "GL/glext.h"

using namespace std;

#define M_PI 3.1415926535897
using std::cos;
using std::sin;
using std::sqrt;

// global variable for counting fps
volatile int frame = 0, time, timebase = 0;
volatile float fps;

/* texture mapping set variable */
GLuint textureMonkey[20];

bool antialiase_on = true;
double radius = 40;
double theta = 85, phi = 100;
double cam[3];
double center[3] = { 0, 0, 0 };
double up[3] = { 0, 1, 0 };

double airAxis[3] = { 0, -35, 15 }; // 비행기 좌표
int camMode = 0; // camMode = 0: 조종석, camMode = 1: 비행기

double wheel_angle = 0;
double airplane_angle = 0;
double airplane_speed = 0;
double pole_angle = 0;
double engine_angle = 0;
double airHeight = 0;
double gravity = 0;
double cockAngle = 0;

bool airPlat = false; // 비행기 각도 
bool airGoOn = false; // 비행기 앞으로
bool wheelOn = false; // 바퀴 넣기
bool engineOn = false; // 엔진 작동
bool airStart = false;
bool airGoDown = false;

bool boom = false;
bool build = false;
bool dragOn = false;

bool desertOn = false; // 사막
bool cockBut = false;

bool swch_engine = 0;
bool swch_wheel = 0;

bool light_sw = false;
bool light_sw2 = false;

int bX, bY = 0;
int bZ = 300;
int alpha = 0;

double xx1, xx2, yy1, yy2 = 0;

// Light0
GLfloat ambientLight[] = { 0.3f, 0.3f, 0.3f, 1.0f };
GLfloat diffuseLight[] = { 0.9f, 0.9f, 0.9f, 1.0f };
GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 1.0f };
GLfloat light_position[] = { 0.0, 0.0, 0.0, 1.0 };

// object var
ObjParser *monkey;
ObjParser *engine;
ObjParser* cockpit_front;
ObjParser* cockpit_fs;
ObjParser* aircock;
ObjParser* wheel;
ObjParser* engine_prop;
ObjParser* airboom;
ObjParser* gear;
//ObjParser* wheelpipe;

int objidx = 0;

// user-defined function
void init(void);
void add_menu();
void light_default();
void get_resource();
void mouse(int, int, int, int);
void mouseWheel(int, int, int, int);
void motion(int, int);
void passiveMotion(int, int);
void keyboard(unsigned char, int, int);
void special_keyboard(int, int, int);
void draw(void);
void resize(int, int);
void idle();
void draw_manual(int);
void draw_stringBox(string, float, float, float, float, float, float, float, GLfloat);
void airCrash();
void camView();
void textureBackground();

void DrawBackground();
void DrawSphere();

void Picking(int, int);

int draw_option = 4;

float move1 = 1.0f;

int count1 = 0;
int count2 = 0;

float g_pPosition[6] = { -140.0f, -70.0f, 147.0f, -70.0f};
int g_nX, g_nY;
int g_nSelect = 0;
int g_nGLWidth = 800, g_nGLHeight = 500;

void draw_string(void*, string, float, float, float, float, float);
void fly(int);

/* Main method */
int main(int argc, char** argv)
{
	// glut initialize
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(1920, 1000);
	//glutInitWindowSize(800, 500);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("3D Airplane Catalog OpenGL");	// 위의 순서 무조건 고정

	// 사용자 초기화 함수
	init();

	// pop-up 메뉴 등록 함수
	add_menu();

	// 리소스 로드 함수
	get_resource();

	// 콜백 함수
	glutMouseFunc(&mouse);
	glutMouseWheelFunc(&mouseWheel);
	glutMotionFunc(&motion);
	glutPassiveMotionFunc(&passiveMotion);
	glutKeyboardFunc(&keyboard);
	glutSpecialFunc(&special_keyboard);
	glutDisplayFunc(&draw);
	glutReshapeFunc(&resize);
	glutIdleFunc(&idle);

	glutMainLoop();

	return EXIT_SUCCESS;
}

void light_default() {
	glClearColor(0.f, 0.f, 0.f, 1.0f);

	/* Light0 조명 관련 설정 */
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);

	/********* light point position setting **********/
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	/* Light1 조명 관련 설정 */
	GLfloat ambientLight1[] = { 0.4f, 0.3f, 0.0f, 0.0f };
	GLfloat diffuseLight1[] = { 1.0f, 0.5f, 0.5f, 1.0f };
	GLfloat specularLight1[] = { 1.0f, 0.5f, 0.5f, 1.0f };
	GLfloat light_position1[] = { 0.0, 0.0, 0.0, 1.0 };

	glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLight1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLight1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specularLight1);

	/********* light point position setting **********/
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);

	/* Light2 조명 관련 설정 */
	GLfloat ambientLight2[] = { 1.0f, 1.0f, 1.0f, 0.1f };
	GLfloat diffuseLight2[] = { 1.0f, 1.0f, 1.0f, 0.1f };
	GLfloat specularLight2[] = { 1.0f, 1.0f, 1.0f, 0.1f };

	glLightfv(GL_LIGHT2, GL_AMBIENT* 0.01, ambientLight2);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, diffuseLight2);
	glLightfv(GL_LIGHT2, GL_SPECULAR, specularLight2);

	GLfloat specularMaterial[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	GLfloat diffuseMaterial[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLfloat ambientMaterial[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	GLfloat emissionMaterial[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	/************* Material  setting *************/
	glMaterialfv(GL_FRONT, GL_AMBIENT, ambientMaterial);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuseMaterial);
	glMaterialfv(GL_FRONT, GL_SPECULAR, specularMaterial);
	glMaterialf(GL_FRONT, GL_SHININESS, 30);


	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	/* DEPTH TEST ENABLE */
	glFrontFace(GL_CW);
}

void setTextureMapping(const char *name, int i) {
	int imgWidth, imgHeight, channels;
	uchar* img = readImageData(name, &imgWidth, &imgHeight, &channels);

	int texNum = 1;
	glGenTextures(texNum, &textureMonkey[i]);
	glBindTexture(GL_TEXTURE_2D, textureMonkey[i]);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, imgWidth, imgHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, img);


	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void init()
{
	// clear background color
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

	// set blend
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);

	// set antialiasing
	glHint(GL_POLYGON_SMOOTH_HINT, GL_FASTEST);
	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_FASTEST);
	glEnable(GL_LINE_SMOOTH);
		
	light_default();

	/* TEXTURE MAPPING SET */
	glEnable(GL_TEXTURE_2D);

	cubeTextureMapping(0);
	textureBackground();
	cylinderTextureMapping();

	setTextureMapping("./texture/airplane.bmp", 0);
	setTextureMapping("./texture/cockpit_front.bmp", 1);
	setTextureMapping("./texture/engine_texture.bmp", 2);
	setTextureMapping("./texture/cockpit_fs.bmp", 3);
	setTextureMapping("./texture/aircock_texture.bmp", 4);
	setTextureMapping("./texture/wheel_texture.bmp", 5);
	setTextureMapping("./texture/wpipe_texture.bmp", 6);
	setTextureMapping("./texture/prop_texture.bmp", 7);
	setTextureMapping("./texture/airplane.bmp", 8);


	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE); 
}

void special_keyboard(int key, int x, int y)
{
	if (key == GLUT_KEY_F1) {
		engine_sound(4);
	}
	if (key == GLUT_KEY_F2) {
		if (draw_option == 1) {
			draw_option = 0;
		}
		else {
			draw_option = 1;
		}
	}
	if (key == GLUT_KEY_F3) {
		if (draw_option == 2) {
			draw_option = 0;
		}
		else {
			draw_option = 2;
		}
	}
	if (key == GLUT_KEY_F4) {
		if (draw_option == 3) {
			draw_option = 0;
		}
		else {
			draw_option = 3;
		}
	}
	if (key == GLUT_KEY_F5) {
		if (draw_option == 4) {
			draw_option = 0;
		}
		else {
			draw_option = 4;
		}
	}
	if (key == GLUT_KEY_INSERT) {

		if (build == false) {
			radius = 150;
			build = true;
		}
		else {
			build = false;
		}
	}
	if (camMode == 0) {
		if (key == GLUT_KEY_LEFT) {
			phi -= 5;
			if (phi < 0) phi = 355;
		}
		else if (key == GLUT_KEY_RIGHT) {
			phi += 5;
			if (phi >= 360) phi = 0;
		}
		else if (key == GLUT_KEY_UP) {
			if (theta > 10) theta -= 5;
		}
		else if (key == GLUT_KEY_DOWN) {
			if (theta < 170) theta += 5;
		}
	}
	else {
		if (key == GLUT_KEY_LEFT) {
			if (phi > 0) phi -= 5;
		}
		else if (key == GLUT_KEY_RIGHT) {
			if (phi < 360) phi += 5;
		}
		else if (key == GLUT_KEY_UP) {
			if (theta > 45) theta -= 5;
		}
		else if (key == GLUT_KEY_DOWN) {
			if (theta < 170) theta += 5;
		}

	}

	//std::cout << "theta : " << theta << ", phi : " << phi << "\n";
	glutPostRedisplay();
}

/* Keyboard callback function */
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
			/* Exit on escape key press */
		case '\x1B':
		{
			exit(EXIT_SUCCESS);
			break;
		}
		case 'a':
		{
			if (antialiase_on) {
				antialiase_on = false;
				glDisable(GL_POLYGON_SMOOTH);
			}
			else
			{
				antialiase_on = true;
				glEnable(GL_POLYGON_SMOOTH);
			}
			break;
		}
		case 'b': {
			printf("CamMode: Airplane\n");
			radius = 40;
			theta = 85, phi = 100;
			for (int i = 0; i < 3; i++) {
				center[i] = 0;
			}
			up[0] = 0; up[1] = 1; up[2] = 0;
			camMode = 0;
			break;
		}
		case 'c': {
			if (boom == true || build == true) return;
			printf("CamMode: Cockpit\n");
			radius = 30;
			theta = 45, phi = 45;
			center[1] = 0;
			up[0] = 0; up[1] = 1; up[2] = 0;
			camMode = 1;
			break;
		}
		case 'z': {
			if (camMode == 1) {
				if (cockBut == false) {
					cockBut = true;
				}
				else {
					cockBut = false;
				}
			}
			break;
		}
		case 'v': {
			printf("camMode: Wheel\n");
			for (int i = 0; i < 3; i++) {
				center[i] = 0;
			}
			up[0] = 0; up[1] = 1; up[2] = 0;
			theta = 95, phi = 155;
			radius = 25;
			camMode = 0;
			break;
		}
		case 'x': {
			printf("camMode: Engine\n");
			cam[0] = 8.5; cam[1] = 0, cam[2] = 4.64;
			center[0] = 10; center[1] = 0; center[2] = 0;
			up[0] = 0; up[1] = 1; up[2] = 0;
			camMode = 4;
			break;
		}
		case 'r': {
			printf("Airplane ON");
			if (swch_wheel == false && wheelOn==false) {
				count2 = 2;
				swch_wheel = true;
			}
			else if(swch_wheel == true && wheelOn ==false){
				count2 = 0;
				swch_wheel = false;
			}
			break;

		}
		case 'p': {
			printf("Airplane Up\n");
			airplane_angle -= 0.1;
			break;
		}
		case 'l': {
			printf("Airplane Down\n");
			airplane_angle += 0.1;
			break;
		}
		case 'u': {
			printf("Airplane Go\n");
			if (airGoOn == false) {
				airGoOn = true;
				engineOn = true;
				engine_sound(1);
			}
			else {
				airGoOn = false;
				engineOn = false;
				engine_sound(2);
			}
			break;
		}
		case 'm': {
			printf("Airplane Plat\n");
			if (airPlat == false) {
				airPlat = true;
			}
			else {
				airPlat = false;
			}
			break;
		}
		case 't': {
		
			printf("Wheel in\n");
			if (wheelOn == false) {
				count1 = 2;
				wheelOn = true;
				swch_wheel = false;
			}
			else {
				count1 = 0;
				wheelOn = false;
				swch_wheel = false;
			}
			break;
		}
		case 'y': {

			printf("engine on\n");
			if (engineOn == false) {
				engine_sound(1);
				engineOn = true;
			}
			else {
				engineOn = false;
				engine_sound(2);
			}
			break;
		}
		case 'f': {

			printf("start on\n");
			radius = 100;

			if (airStart == false) {
				airStart = true;
				engine_sound(1);
			}
			else {
				airStart = false;
				engine_sound(2);
			}
			break;
		}
		case 'g': {

			printf("down on\n");
			radius = 100;

			if (airGoDown == false) {
				airGoDown = true;
			}
			else {
				airGoDown = false;
			}
			break;
		}
		case ';': {
			bZ += 5;
			break;
		}
		case '\'': {
			bZ -= 5;
			break;
		}
		case '[': {
			bY += 5;
			break;
		}
		case ']': {
			bY -= 5;
			break;
		}
	}
	glutPostRedisplay();
}


void draw_obj(ObjParser *objParser)
{
	glBegin(GL_TRIANGLES);
	for (unsigned int n = 0; n < objParser->getFaceSize(); n+=3) {
		glNormal3f(objParser->normal[objParser->normalIdx[n] - 1].x,
			objParser->normal[objParser->normalIdx[n] - 1].y,
			objParser->normal[objParser->normalIdx[n] - 1].z);
		glVertex3f(objParser->vertices[objParser->vertexIdx[n] - 1].x,
			objParser->vertices[objParser->vertexIdx[n] - 1].y,
			objParser->vertices[objParser->vertexIdx[n] - 1].z);

		glNormal3f(objParser->normal[objParser->normalIdx[n + 1] - 1].x,
			objParser->normal[objParser->normalIdx[n + 1] - 1].y,
			objParser->normal[objParser->normalIdx[n + 1] - 1].z);
		glVertex3f(objParser->vertices[objParser->vertexIdx[n + 1] - 1].x,
			objParser->vertices[objParser->vertexIdx[n + 1] - 1].y,
			objParser->vertices[objParser->vertexIdx[n + 1] - 1].z);

		glNormal3f(objParser->normal[objParser->normalIdx[n + 2] - 1].x,
			objParser->normal[objParser->normalIdx[n + 2] - 1].y,
			objParser->normal[objParser->normalIdx[n + 2] - 1].z);
		glVertex3f(objParser->vertices[objParser->vertexIdx[n + 2] - 1].x,
			objParser->vertices[objParser->vertexIdx[n + 2] - 1].y,
			objParser->vertices[objParser->vertexIdx[n + 2] - 1].z);
	}
	glEnd();
}

void draw_obj_with_texture(ObjParser *objParser, int i)
{
	glDisable(GL_BLEND);
	// glEnable(GL_TEXTURE_2D);	// texture 색 보존을 위한 enable
	glBindTexture(GL_TEXTURE_2D, textureMonkey[i]);
	glBegin(GL_TRIANGLES);
	for (unsigned int n = 0; n < objParser->getFaceSize(); n += 3) {
		glTexCoord2f(objParser->textures[objParser->textureIdx[n] - 1].x,
			objParser->textures[objParser->textureIdx[n] - 1].y);
		glNormal3f(objParser->normal[objParser->normalIdx[n] - 1].x,
			objParser->normal[objParser->normalIdx[n] - 1].y,
			objParser->normal[objParser->normalIdx[n] - 1].z);
		glVertex3f(objParser->vertices[objParser->vertexIdx[n] - 1].x,
			objParser->vertices[objParser->vertexIdx[n] - 1].y,
			objParser->vertices[objParser->vertexIdx[n] - 1].z);

		glTexCoord2f(objParser->textures[objParser->textureIdx[n + 1] - 1].x,
			objParser->textures[objParser->textureIdx[n + 1] - 1].y);
		glNormal3f(objParser->normal[objParser->normalIdx[n + 1] - 1].x,
			objParser->normal[objParser->normalIdx[n + 1] - 1].y,
			objParser->normal[objParser->normalIdx[n + 1] - 1].z);
		glVertex3f(objParser->vertices[objParser->vertexIdx[n + 1] - 1].x,
			objParser->vertices[objParser->vertexIdx[n + 1] - 1].y,
			objParser->vertices[objParser->vertexIdx[n + 1] - 1].z);

		glTexCoord2f(objParser->textures[objParser->textureIdx[n + 2] - 1].x,
			objParser->textures[objParser->textureIdx[n + 2] - 1].y);
		glNormal3f(objParser->normal[objParser->normalIdx[n + 2] - 1].x,
			objParser->normal[objParser->normalIdx[n + 2] - 1].y,
			objParser->normal[objParser->normalIdx[n + 2] - 1].z);
		glVertex3f(objParser->vertices[objParser->vertexIdx[n + 2] - 1].x,
			objParser->vertices[objParser->vertexIdx[n + 2] - 1].y,
			objParser->vertices[objParser->vertexIdx[n + 2] - 1].z);
	}
	glEnd();
	glEnable(GL_BLEND);
}

/* Display callback function */
void draw()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glLoadIdentity();
	glClearDepth(1.0f);
	glEnable(GL_DEPTH_TEST);


	/* Manual 출력 */
	draw_manual(draw_option);
	if (draw_option != 0) {
		draw_stringBox("", xx1, yy1, xx2, yy2, 1.0f, 1.0f, 1.0f, 0.8f);
	}

	if (cockBut == true) {
		glMatrixMode(GL_PROJECTION);


		glLoadIdentity();
		glOrtho(-g_nGLWidth / 2.0f, g_nGLWidth / 2.0f, -g_nGLHeight / 2.0f, g_nGLHeight / 2.0f, -100, 100);

		glMatrixMode(GL_MODELVIEW);

		glLoadIdentity();

		DrawBackground();
		DrawSphere();

		
	}

	if (cockBut == false) {
		resize(g_nGLWidth, g_nGLHeight);
		glMatrixMode(GL_MODELVIEW);


		/* CamMode View */
		camView();

		/* LIGHT 0&1 */

		GLfloat light_position[] = { cam[0],cam[1],cam[2],1.0 };
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);

		GLfloat light_position1[] = { -6, 32, -31 + airplane_speed, 1 };
		glLightfv(GL_LIGHT1, GL_POSITION, light_position1);

		/* LIGHT2 */

		GLfloat light_position2[] = { 0, 0, 50 };
		glLightfv(GL_LIGHT2, GL_POSITION, light_position2);



		glEnable(GL_TEXTURE_2D);
		glEnable(GL_LIGHTING);

		/* 충돌 함수 */
		airCrash();

		if (boom == false) {
			glPushMatrix();


			/* 비행기 전체 */
			glPushMatrix();
			glRotatef(airplane_angle, 1, 0, 0);
			glTranslatef(0, airHeight, 0);

			/* 비행기 몸체 */
			glPushMatrix();
			glTranslatef(airAxis[0], airAxis[1], airAxis[2] + airplane_speed);

			glColor3f(1.0f, 1.0f, 1.0f);

			draw_obj_with_texture(monkey, 0);
			draw_obj_with_texture(engine, 2);
			//draw_obj_with_texture(wheelpipe, 6);
			glPopMatrix();

			////////////////

		/* 바퀴 부분 */
			glPushMatrix();

			glTranslatef(airAxis[0], airAxis[1], airAxis[2]);
			glPushMatrix();

			glTranslatef(5.7, 32 + pole_angle * 0.03, -31 + airplane_speed + pole_angle * 0.03);
			glRotatef(90, 0, 1, 0);
			glRotatef(wheel_angle, 0, 0, 1);
			draw_textureCylinder(0.5f, 1.0f);

			glPopMatrix();

			glPushMatrix();

			glTranslatef(5.45, 35, -31 + airplane_speed);
			glRotatef(90, 1, 0, 0);
			glRotatef(-pole_angle, 1, 0, 0);
			draw_textureCylinder(3.0f, 0.2f);

			glPopMatrix();

			glPushMatrix();

			glTranslatef(4.7, 32 + pole_angle * 0.03, -31 + airplane_speed + pole_angle * 0.03);
			glRotatef(90, 0, 1, 0);
			glRotatef(wheel_angle, 0, 0, 1);
			draw_textureCylinder(0.5f, 1.0f);

			glPopMatrix();

			///////////////////

			glPushMatrix();

			glTranslatef(-6.2, 32 + pole_angle * 0.03, -31 + airplane_speed + pole_angle * 0.03);
			glRotatef(90, 0, 1, 0);
			glRotatef(wheel_angle, 0, 0, 1);
			draw_textureCylinder(0.5f, 1.0f);

			glPopMatrix();

			glPushMatrix();

			glTranslatef(-5.4, 35, -31 + airplane_speed);
			glRotatef(90, 1, 0, 0);
			glRotatef(-pole_angle, 1, 0, 0);
			draw_textureCylinder(3.0f, 0.2f);

			glPopMatrix();

			glPushMatrix();

			glTranslatef(-5.2, 32 + pole_angle * 0.03, -31 + airplane_speed + pole_angle * 0.03);
			glRotatef(90, 0, 1, 0);
			glRotatef(wheel_angle, 0, 0, 1);
			draw_textureCylinder(0.5f, 1.0f);

			glPopMatrix();

			/* 엔진 부분 */


			glPushMatrix();
			glScalef(1.0f, 1.0f, 3.0f);
			glTranslatef(12, 34, -8.5 + airplane_speed / 3);
			glRotatef(7 * engine_angle, 0, 0, 1);
			draw_obj_with_texture(engine_prop, 7);
			glPopMatrix();

			glPushMatrix();
			glScalef(1.0f, 1.0f, 3.0f);
			glTranslatef(21.3, 35, -11 + airplane_speed / 3);
			glRotatef(7 * engine_angle, 0, 0, 1);
			draw_obj_with_texture(engine_prop, 7);
			glPopMatrix();

			glPushMatrix();
			glScalef(1.0f, 1.0f, 3.0f);
			glTranslatef(-12, 34, -8.5 + airplane_speed / 3);
			glRotatef(7 * engine_angle, 0, 0, 1);
			draw_obj_with_texture(engine_prop, 7);
			glPopMatrix();

			glPushMatrix();
			glScalef(1.0f, 1.0f, 3.0f);
			glTranslatef(-21.3, 35, -11 + airplane_speed / 3);
			glRotatef(7 * engine_angle, 0, 0, 1);
			draw_obj_with_texture(engine_prop, 7);
			glPopMatrix();


			glPopMatrix();

			glPopMatrix();

			glPushMatrix();
			glPopMatrix();
			glPopMatrix();

			glPushMatrix();
			glTranslatef(1200, -30 + airHeight, -10 + airplane_speed);
			glRotatef(90, 0, 1, 0);
			draw_obj_with_texture(cockpit_front, 1);
			draw_obj_with_texture(cockpit_fs, 3);
			draw_obj_with_texture(aircock, 4);


			glPopMatrix();

			glPushMatrix();
			glTranslatef(1202, -5+ airHeight, 12 + airplane_speed);
			glRotatef(-90 - cockAngle, 1, 0, 0);
			draw_obj_with_texture(gear, 9);

			glPopMatrix();
		}

		glPushMatrix();
		if (desertOn == true) {
			glTranslatef(0, 415, 300);
		}
		draw_textureCube(light_sw, move1);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(1200, 0, 0);
		if (desertOn == true) {
			glTranslatef(0, 415, 300);
		}
		draw_textureCube(light_sw, move1);
		glPopMatrix();


	}


	glutSwapBuffers();
	glFlush();
}

int c_x, c_y;

void mouse(int button, int state, int x, int y)
{
	y = g_nGLHeight - y;
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && camMode == 1)
	{
		dragOn = true;
		
		c_x = x; c_y = y;

		
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP && camMode == 1)
	{
		dragOn = false;
		g_nSelect = 0;
	}

	if (camMode == 1 && cockBut == true) {
		Picking(x, y);
		g_nX = x;
		g_nY = y;
	
	}
	glutPostRedisplay();
}

void passiveMotion(int x, int y)
{

}

void motion(int x, int y)
{
	if (dragOn == true && camMode == 1 && c_y < 150) {
		if (y * 500 / g_nGLHeight - c_y -320 < -47) {
			engine_sound(1);
			engineOn = true;
			cockAngle = -47;
		}
		else if (y * 500 / g_nGLHeight - c_y -320 > 47) {
			engine_sound(2);
			engineOn = false;
			cockAngle = 47;
		}
		else cockAngle = y * 500 / g_nGLHeight - c_y - 320;
	}

	
	if (g_nSelect > 0) {
		int t_y = g_nGLHeight - y;
		g_pPosition[(g_nSelect - 1) * 2 + 0] += x - g_nX;
		g_pPosition[(g_nSelect - 1) * 2 + 1] += t_y - g_nY;
		g_nX = x;
		g_nY = g_nGLHeight - t_y;
		glutPostRedisplay();
	}
	
	glutPostRedisplay();
}


void mouseWheel(int button, int dir, int x, int y)
{
	//if (camMode == 1 && radius > 40) {
	//	radius = 35;
	//}
	if (dir > 0) {
		if (radius > 0) radius -= 5;
	}
	else {
		if (camMode == 0) {
			if (radius < 2000) radius += 5;
		}
		else {
			if (radius < 40) radius += 5;
		}
	}
	glutPostRedisplay();
}


/* 메뉴 함수 */

void main_menu(int option)
{
	if (option == 99) exit(0);
	else if (option == 1) {
		init();

		// 시점 초기화
		radius = 40; theta = 85; phi = 100;

		for (int i = 0; i < 3; i++) {
			center[i] = 0;
		}
		up[0] = 0; up[1] = 1; up[2] = 0;
		camMode = 0;

		// 스위치 변수들 끄기
		airPlat = false; airGoOn = false; wheelOn = false; engineOn = false; 
		airStart = false; airGoDown = false; boom = false; desertOn = false;
		cockBut = false; swch_engine = false; swch_wheel = false; 
		light_sw = false; light_sw2 = false; build = false;

		// 전역변수 초기화
		wheel_angle = 0; airplane_angle = 0; airplane_speed = 0; pole_angle = 0; engine_angle = 0;
		airHeight = 0; gravity = 0; draw_option = 4;

		// 엔진 소리 끄기 (2)
		engine_sound(2);

		// 스카이박스 초기화
		cubeTextureMapping(0);

		// 텍스쳐 매핑 초기화
		setTextureMapping("./texture/airplane.bmp", 0);

		// 조명 초기화
		ambientLight[0] = 0.3f; diffuseLight[0] = 0.9f; specularLight[0] = 0.5f;
		glEnable(GL_LIGHT0);
		glDisable(GL_LIGHT1);
		light_default();
	}
	glutPostRedisplay();
}

void sub_menu(int option)
{
	printf("Submenu1 %d has been selected\n", option);
	if (option == 2) {
		setTextureMapping("./texture/airplane2.bmp", 0);
		setTextureMapping("./texture/airplane2.bmp", 8);
	}
	else if (option == 1) {
		setTextureMapping("./texture/airplane.bmp", 0);
		setTextureMapping("./texture/airplane.bmp", 8);
	
	}
	glutPostRedisplay();
}

void sub_menu2(int option)
{
	printf("Submenu2 %d has been selected\n", option);

	if (option == 1) {
		cubeTextureMapping(0);
		light_sw = false;
		ambientLight[0] = 0.3f;
		diffuseLight[0] = 0.9f;
		specularLight[0] = 0.5f;
		glEnable(GL_LIGHT0);
		glDisable(GL_LIGHT1);
		light_default();
	
	}
	else if (option == 2) {
		cubeTextureMapping(1);
		ambientLight[0] = 1.0f;
		diffuseLight[0] = 1.0f;
		specularLight[0] = 1.0f;
		//glDisable(GL_LIGHT0);
		glDisable(GL_LIGHT1);
		light_default();
		//light_sw2 = true;
		//glEnable(GL_LIGHT1);
	}
	else if (option == 3) {
		cubeTextureMapping(1);
		ambientLight[0] = 1.0f;
		diffuseLight[0] = 1.0f;
		specularLight[0] = 1.0f;
		
		glEnable(GL_LIGHT1);
		glDisable(GL_LIGHT0);

		light_sw = true; // skybox 조명도 어둡게
		//light_sw2 = false;
	}
	else if (option == 4) {
		cubeTextureMapping(2);
		light_sw = false;
		ambientLight[0] = 0.3f;
		diffuseLight[0] = 0.9f;
		specularLight[0] = 0.5f;
		glEnable(GL_LIGHT0);
		glDisable(GL_LIGHT1);
		light_default();
		desertOn = true;

	}

	glutPostRedisplay();
}

void add_menu()
{
	int mainmenu1;
	int submenu1;
	int submenu2;

	submenu1 = glutCreateMenu(sub_menu);
		glutAddMenuEntry("texture 1: (all white)", 1);
		glutAddMenuEntry("texture 2: (cyan)", 2);

	submenu2 = glutCreateMenu(sub_menu2);
		glutAddMenuEntry("skybox: day", 1);
		glutAddMenuEntry("skybox: sunset", 2);
		glutAddMenuEntry("skybox: sunset ~", 3);
		glutAddMenuEntry("skybox: desert", 4);
	
	mainmenu1 = glutCreateMenu(main_menu);
		glutAddMenuEntry("Init", 1);
		glutAddSubMenu("Change Texture", submenu1);
		glutAddSubMenu("Change skybox", submenu2);
		glutAddMenuEntry("Quit", 99);

	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void idle()
{
	frame++;
	time = glutGet(GLUT_ELAPSED_TIME); // unit : ms

	if (time - timebase > 1000)
	{
		fps = frame * 1000.f / (time - timebase);
		timebase = time;
		frame = 0;
		//printf("fps : %.0f\n", fps);
	}

	if (swch_wheel == true && wheelOn == false) {
		wheel_angle += 1;
	}

	if (airGoOn == true) {
		airplane_speed += 0.2;
	}

	if (airPlat == true) {
		if (airplane_angle > 0) {
			airplane_angle -= 0.01;
			if (airplane_angle < 0.01f) {
				airPlat = false;
			}
		}
		else if (airplane_angle < 0) {
			airplane_angle += 0.01;
			if (airplane_angle > -0.01f) {
				airPlat = false;
			}
		}
		else {
			//airplane_angle = 0;
			airPlat = false;
		}
	}
	else {
		//airplane_angle = 0;
	}

	if (wheelOn == true) {
		if (pole_angle < 90) {
			pole_angle += 0.2;
		}
	}
	else {
		if (pole_angle >= 0) {
			pole_angle -= 0.2;
		}
	}

	if (engineOn == true) {
		engine_angle += 1;
		
	}

	if (airStart == true) {
		fly(1);
	}

	if (airGoDown == true) {
		airStart = false;

		fly(2);
	}

	if (boom == true) {
		gravity += 0.5;
	}

	glutPostRedisplay();
}

void resize(int width, int height)
{
	if (cockBut == false) {
		g_nGLWidth = width;
		g_nGLHeight = height;

		glViewport(0, 0, width, height);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//gluOrtho2D(0, 500, 0, 500);
		gluPerspective(60, (double)width / (double)height, 1, 1000);
		glMatrixMode(GL_MODELVIEW);
	}
	else {
		g_nGLWidth = width;
		g_nGLHeight = height;
		glViewport(0, 0, width, height);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		glOrtho(-width / 2.0f, width / 2.0f, -height / 2.0f, height / 2.0f,
			-100, 100);

		glMatrixMode(GL_MODELVIEW);
	}
}

void get_resource()
{
	monkey = new ObjParser("./obj/airplane.obj");
	engine = new ObjParser("./obj/engine.obj");
	cockpit_front = new ObjParser("./obj/cockpit_front.obj");
	cockpit_fs = new ObjParser("./obj/cockpit_fs.obj");
	aircock = new ObjParser("./obj/aircock.obj");
	wheel = new ObjParser("./obj/wheel.obj");
	engine_prop = new ObjParser("./obj/engine_prop.obj");
	airboom = new ObjParser("./obj/airboom.obj");
	gear = new ObjParser("./obj/gear.obj");
	//wheelpipe = new ObjParser("./obj/wheelpipe.obj");
}

int on = 0;

void fly(int option) {
	if (airHeight < 100 && option == 1) {
		airGoOn = true;
		engineOn = true;

		if (airplane_angle > -10) {
			airplane_angle -= 0.15;
			airHeight += 0.01;
			wheelOn = true;
		}
		if (airplane_angle < -4.5) {

			airHeight += 0.01;
			airPlat = true;
			on = 1;
		}
		if (on==1){
			airHeight += 0.15;
			
		}
	}
	else if (airHeight > -1 && option == 2) {
		if (airHeight > 100) {
			//airplane_angle += 0.15;
			if (airHeight < 100) airPlat = true;
		}
			airHeight -= 0.1;
			wheelOn = false;
			if (airHeight < 1) {
				swch_wheel = true;
			}
	}
}

void draw_manual(int option) {
	string manual = "";
	float ratio = 800.0f / float(g_nGLWidth);
	if (option == 1) {
		xx1 = 10; yy1 = 800; xx2 = 380*ratio; yy2 = 990;
		manual = "<View Control>";
		draw_string(GLUT_BITMAP_9_BY_15, manual, 20, 950, 0, 0, 0);
		manual = "<B: Airplane>";
		draw_string(GLUT_BITMAP_8_BY_13, manual, 20, 930, 0, 0, 0);
		manual = "<C: Cockpit>";
		draw_string(GLUT_BITMAP_8_BY_13, manual, 20, 910, 0, 0, 0);
		manual = "<V: Wheel>";
		draw_string(GLUT_BITMAP_8_BY_13, manual, 20, 890, 0, 0, 0);
		manual = "<X: Engine>";
		draw_string(GLUT_BITMAP_8_BY_13, manual, 20, 870, 0, 0, 0);
		manual = "<Onlu Cam in Cockpit/ Z: Button>";
		draw_string(GLUT_BITMAP_8_BY_13, manual, 20, 850, 0, 0, 0);
	}
	else if (option == 2) {
		xx1 = 10; yy1 = 750; xx2 = 310 * ratio; yy2 = 990;
		manual = "<Airplane Control>";
		draw_string(GLUT_BITMAP_9_BY_15, manual, 20, 950, 0, 0, 0);
		manual = "<R: Wheel On/Off>";
		draw_string(GLUT_BITMAP_8_BY_13, manual, 20, 930, 0, 0, 0);
		manual = "<T: Wheel In/Out>";
		draw_string(GLUT_BITMAP_8_BY_13, manual, 20, 910, 0, 0, 0);
		manual = "<Y: Engine On/Off>";
		draw_string(GLUT_BITMAP_8_BY_13, manual, 20, 890, 0, 0, 0);
		manual = "<P: Angle Up>";
		draw_string(GLUT_BITMAP_8_BY_13, manual, 20, 870, 0, 0, 0);
		manual = "<L: Angle Down>";
		draw_string(GLUT_BITMAP_8_BY_13, manual, 20, 850, 0, 0, 0);
		manual = "<U: Airplane Move>";
		draw_string(GLUT_BITMAP_8_BY_13, manual, 20, 830, 0, 0, 0);
		manual = "<M: Angle Horizon>";
		draw_string(GLUT_BITMAP_8_BY_13, manual, 20, 810, 0, 0, 0);
		manual = "<F: Airplane Take-off>";
		draw_string(GLUT_BITMAP_8_BY_13, manual, 20, 790, 0, 0, 0);
		manual = "<G: Airplane Landing>";
		draw_string(GLUT_BITMAP_8_BY_13, manual, 20, 770, 0, 0, 0);
	}
	else if (option == 3) {
		xx1 = 10; yy1 = 800; xx2 = 460 * ratio; yy2 = 990;
		manual = "<Crashing Mode>";
		draw_string(GLUT_BITMAP_9_BY_15, manual, 20, 950, 0, 0, 0);
		manual = "<INSERT: Create/Delete Object(Only one)>";
		draw_string(GLUT_BITMAP_8_BY_13, manual, 20, 920, 0, 0, 0);
		manual = "<; \': Move Left/Right>";
		draw_string(GLUT_BITMAP_8_BY_13, manual, 20, 890, 0, 0, 0);
		manual = "<[ ]: Move Up/Down>";
		draw_string(GLUT_BITMAP_8_BY_13, manual, 20, 860, 0, 0, 0);
	
	}
	else if (option == 4) {
		xx1 = 10; yy1 = 800; xx2 = 310 * ratio; yy2 = 990;
		manual = "<Manual>";
		draw_string(GLUT_BITMAP_9_BY_15, manual, 20, 950, 0, 0, 0);
		manual = "<F1: Engine Mute>";
		draw_string(GLUT_BITMAP_8_BY_13, manual, 20, 930, 0, 0, 0);
		manual = "<F2: Basic Control Map>";
		draw_string(GLUT_BITMAP_8_BY_13, manual, 20, 910, 0, 0, 0);
		manual = "<F3: Airplane Control Map>";
		draw_string(GLUT_BITMAP_8_BY_13, manual, 20, 890, 0, 0, 0);
		manual = "<F4: View Control Map>";
		draw_string(GLUT_BITMAP_8_BY_13, manual, 20, 870, 0, 0, 0);
		manual = "<F5: Manual>";
		draw_string(GLUT_BITMAP_8_BY_13, manual, 20, 850, 0, 0, 0);
	
	}
	else if (option == 5) {
		//draw_string(GLUT_BITMAP_9_BY_15, "<Cockpit Control> R: Wheel in/ G: Wheel on", 400, 150, 0, 0, 0);
	}

}

void draw_stringBox(string str, float x_pos, float y_pos, float x2_pos, float y2_pos, float red, float green, float blue, GLfloat alpha) {

	double width = 1000; double height = 1000;
	glPushAttrib(GL_LIGHTING_BIT);
	glDisable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, 1000, 0, 1000);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	draw_string(GLUT_BITMAP_9_BY_15, str, 20, 950, 0, 0, 0);

	glDisable(GL_TEXTURE_2D);
	glColor4f(red, green, blue, alpha);
	glRectd(x_pos, y_pos, x2_pos, y2_pos);
	glEnable(GL_TEXTURE_2D);

	/*
	glBegin(GL_QUADS);
	glVertex2i(x_pos, y_pos);
	glVertex2i(x2_pos, y_pos);
	glVertex2i(x_pos, y2_pos);
	glVertex2i(x2_pos, y2_pos);
	glEnd();
	*/

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopAttrib();
}


void draw_string(void* font, string str, float x_position, float y_position, float red, float green, float blue) {
	glPushAttrib(GL_LIGHTING_BIT);
	glDisable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, 1000, 0, 1000);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glColor3f(red, green, blue);
	glRasterPos3f(x_position, y_position, 0);
	for (unsigned int i = 0; i < str.length(); i++) {
		glutBitmapCharacter(font, str[i]);
	}
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopAttrib();
}

void airCrash() {
	if (build == true) {
		glPushMatrix();

		glDisable(GL_LIGHTING);
		if (boom == true) {
			glPushMatrix();
			glTranslatef(0, -25 - gravity, airAxis[2] + airplane_speed);
			glColor3f(0.2f, 0.0f, 0.0f);
			draw_obj_with_texture(airboom, 8);
			glPopMatrix();
		}

		glTranslatef(0, bY, bZ);
		glColor3f(1, 1, 1);
		glutSolidSphere(100, 32, 32);

		double Z = bZ - airAxis[2] - airplane_speed;
		double Yu = bY - airAxis[1] - airHeight - 38;
		double Yd = bY - airAxis[1] - airHeight + 38;
		double distance = sqrt(pow(Z, 2) + pow(Yu, 2));
		double distance2 = sqrt(pow(Z, 2) + pow(Yd, 2));

		if ((distance >= 100 && distance < 104) || (distance2 >= 100 && distance < 104)) {
			airGoOn = false;
			if (boom == false) {
				engine_sound(3);
			}

			boom = true;
		}


		glEnable(GL_LIGHTING);
		glPopMatrix();
	}
}

void draw_text(void* font, string str) {

	double width = 1000;
	double height = 1000;
	glPushAttrib(GL_LIGHTING_BIT);
	glDisable(GL_LIGHTING);
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, 1000, 0, 1000);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	GLfloat alpha1 = 1.0f;

	draw_string(GLUT_BITMAP_9_BY_15, "", 20, 950, 0, 0, 0);

	glDisable(GL_LIGHTING);
	glColor4f(1.0f, 1.0f, 1.0f, alpha1);
	glRectd(10, 800, 310, 990);

	glDisable(GL_LIGHTING);

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glPopAttrib();
}

void camView() {

	if (camMode == 0) {

		cam[0] = radius * sin(theta * M_PI / 180) * sin(phi * M_PI / 180);
		cam[1] = radius * cos(theta * M_PI / 180);
		cam[2] = radius * sin(theta * M_PI / 180) * cos(phi * M_PI / 180);
		gluLookAt(cam[0], cam[1] + airHeight, cam[2] + airplane_speed, center[0], center[1] + airHeight, center[2] + airplane_speed, up[0], up[1], up[2]);
	}
	else if (camMode == 1) {

		center[0] = -80 + phi +1200;
		center[1] = -50 + theta + airHeight;
		center[2] = 300 + phi ;

		cam[0] = 5 + 1200;
		cam[1] = 6;
		cam[2] = -5;

		gluLookAt(cam[0], cam[1] + airHeight, cam[2] + airplane_speed, center[0], center[1], center[2] + airplane_speed, up[0], up[1], up[2]);
	}
	else if (camMode == 4) {
		gluLookAt(cam[0], cam[1] + airHeight, cam[2] + airplane_speed, center[0], center[1] + airHeight, center[2] + airplane_speed, up[0], up[1], up[2]);
	}
}


void DrawSphere() {

	glLoadName(1);
	glPushMatrix();
	glTranslatef(g_pPosition[0], g_pPosition[1], 0.0f);
	if (wheelOn == true) {
		glColor4f(1, 0, 0, 0.5f);
	}
	else {
		glColor4f(1, 0, 0, 1.0f);
	}
	glutSolidCube(200.0f);
	glPopMatrix();

	glLoadName(2);
	glPushMatrix();
	glTranslatef(g_pPosition[2], g_pPosition[3], 0.0f);
	if (swch_wheel == true) {
		glColor4f(0, 1, 0, 0.5f);
	}
	else {
		glColor4f(0, 1, 0, 1.0f);
	}
	glutSolidCube(200.0f);
	glPopMatrix();
}


void Picking(int x, int y) {
	GLuint selectBuf[256];

	// first step
	glSelectBuffer(256, selectBuf);

	// second step
	glRenderMode(GL_SELECT);

	// third step	glInitNames();
	glPushName(0);
	glSelectBuffer(100, selectBuf);

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();

	gluPickMatrix(x, y, 0.1, 0.1, viewport);
	// fourth step
	glOrtho(-g_nGLWidth / 2.0f, g_nGLWidth / 2.0f, -g_nGLHeight / 2.0f,
		g_nGLHeight / 2.0f, -100, 100);
	glMatrixMode(GL_MODELVIEW);

	glPushMatrix();
	DrawSphere();
	glPopMatrix();

	glMatrixMode(GL_PROJECTION);

	glPopMatrix();
	glFlush();

	GLint hits = glRenderMode(GL_RENDER);
	// sixth step

	if (hits <= 0) return;

	g_nSelect = selectBuf[3];

	if (g_nSelect == 1) {
		count1++;
		if (wheelOn == true && count1 == 4) {
			g_nSelect = 0;
			wheelOn = false;
			printf("wheel out\n");
			count1 = 0;
		}
		else {
			g_nSelect = 0;
			wheelOn = true;
			printf("wheel in\n");
		}
	}
	else if (g_nSelect == 2 && wheelOn == false) {
		count2++;
		printf("count2: %d", count2);
		if (swch_wheel == true && count2 == 4) {
			g_nSelect = 0;
			swch_wheel = false;
			count2 = 0;
			printf("wheel Off\n");
		}
		else {
			g_nSelect = 0;
			swch_wheel = true;
			printf("wheel On\n");
		}
	}

	printf("g_nSelect: %d\n", g_nSelect);


}