#include "Airfunc.h"
#include <GL/glut.h>
#include <GL/freeglut.h>
#include "GL/glext.h"


void wheelFunc(int swch, int Rangle) {
	if (swch == 1) {
		glRotatef(Rangle, 1, 0, 0);
		Rangle++;
	}
}


void engineFunc(int swch) {
	if (swch == 1) {
	}

};