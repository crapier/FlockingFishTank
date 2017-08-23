
#include "globals.h"


// mouse rotation 
int rotateon;

int lastx, lasty;
int xchange, ychange;
float spin = 0.0;
float spinup = 0.0;

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();

	//rotate the view
	glRotatef(spinup, 1.0, 0.0, 0.0);
	glRotatef(spin, 0.0, 1.0, 0.0);

	//Draw the scene
	//set material parameters
	for (size_t i = 0; i < sim.scene.faces.size(); i++) {
		sim.scene.faces[i].draw();
	}

	//draw the particles
	for (size_t i = 0; i < sim.scene.boids.size(); i++) {
		if (sim.scene.boids[i].alive) {
			sim.scene.boids[i].draw();
		}
	}


	// draw waypoints
	for (size_t i = 0; i < sim.scene.waypoints.size(); i++) {
		sim.scene.waypoints[i].draw();
	}

	glPopMatrix();
	glutSwapBuffers();
}

void init(void)
{
	glClearColor(0.12941, 0.384313, 0.890196, 0.0);
	// Enable Z-buffering, backface culling
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glShadeModel(GL_SMOOTH);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, 1.0, 1, 9999);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Set eye point and lookat point
	gluLookAt(0, 225, 300, 0, 0, 0, 0, 1, 0);

	glEnable(GL_POINT_SMOOTH);

}


void reshapeFunc(GLint newWidth, GLint newHeight)
{
	if (newWidth > newHeight) // Keep a square viewport
		glViewport((newWidth - newHeight) / 2, 0, newHeight, newHeight);
	else
		glViewport(0, (newHeight - newWidth) / 2, newWidth, newWidth);
	init();
	glutPostRedisplay();
}

void rotateview(void)
{
	if (rotateon) {
		spin += xchange / 250.0;
		if (spin >= 360.0) spin -= 360.0;
		if (spin < 0.0) spin += 360.0;
		spinup -= ychange / 250.0;
		if (spinup > 89.0) spinup = 89.0;
		if (spinup < -89.0) spinup = -89.0;
	}
	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y)
{
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) {
			lastx = x;
			lasty = y;
			xchange = 0;
			ychange = 0;
			rotateon = 1;
		}
		else if (state == GLUT_UP) {
			xchange = 0;
			ychange = 0;
			rotateon = 0;
		}
		break;

	default:
		break;
	}
}

void motion(int x, int y)
{
	xchange = x - lastx;
	ychange = y - lasty;
}

void keyboardFunc(unsigned char key, int x, int y) {
	switch (key)
	{
	case '1':
		break;
	default:
		break;
	}
}

DWORD WINAPI glut_main(LPVOID lpParam) {
	glut_args* arg = (glut_args*)lpParam;
	glutInit(&arg->argc, arg->argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(800, 800);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Flocking");
	init();
	rotateon = 0;
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);
	glutIdleFunc(rotateview);
	glutReshapeFunc(reshapeFunc);
	glutKeyboardFunc(keyboardFunc);

	glutMainLoop();

	return 0;
}