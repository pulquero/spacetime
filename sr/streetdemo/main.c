/* Street
 * Relativistic demonstration
 * Bruno Barberi Gnecco <brunobg@geocities.com>, 2001.
 */

#include "street.h"
#include <GL/glut.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
// #include <dicelib.h>

Data data = { GL_FALSE, GL_FALSE, GL_FALSE };

//GLuint textureID[TOTAL];

struct {
	GLfloat		position[3];
	GLfloat		direction[3];
	GLfloat		eye[3];
} camera;

void Animate (int value) {
	static GLboolean show_intro = GL_TRUE;

	if (show_intro) {
		return;
	}

	/* calculate sun light */
}

void Display (void) {
//	DICE_sync();

#ifdef USE_SHADOWS
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
#else
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
#endif

	glLoadIdentity();
	
	/* transform the camera */
	gluLookAt(camera.position[0], camera.position[1], camera.position[2], 
			camera.eye[0], camera.eye[1], camera.eye[2],
			0.0, 0.0, 1.0);
	render_street();
//	render_bluecar();
//	render_redcar();
//	render_man();

	glutSwapBuffers();
}

void Key (unsigned char key, int x, int y) {
	switch (key) {
	  case 'd':
	  case 'D':
		  data.demo = GL_TRUE;
		  break;
	  case 27:/* escape */
	  case 'q':
	  case 'Q':
//		  DICE_close();
		  exit(0);
	}
	glutPostRedisplay();
}

void Special(int key, int x, int y) {
	switch (key) {
/* positions are raw: they are in world coordinates should be in camera */
	  case GLUT_KEY_LEFT:
		  camera.position[0] += 0.5;
		  break;
	  case GLUT_KEY_RIGHT:
		  camera.position[0] -= 0.5;
		  break;
	  case GLUT_KEY_UP:
		  camera.position[1] += 0.5;
		  break;
	  case GLUT_KEY_DOWN:
		  camera.position[1] -= 0.5;
		  break;
	  case GLUT_KEY_PAGE_UP:
		  camera.position[2] -= 0.5;
		  break;
	  case GLUT_KEY_PAGE_DOWN:
		  camera.position[2] += 0.5;
		  break;
	  case GLUT_KEY_HOME:
		  break;
	  case GLUT_KEY_END:
		  break;
	}
	glutPostRedisplay();
}

static int moving = 0, beginx, beginy;

void Mouse (int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		moving = 1;
		beginx = x;
		beginy = y;
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		moving = 0;
	}
}

/* must be redone */
void Motion (int x, int y) {
	if (moving) {
//		angle += ((double)(x - beginx) * 100 / (double)winwidth);
//		angle2 += ((double)(y - beginy) * 100 / (double)winheight);

		glutPostRedisplay();
		beginx = x;
		beginy = y;
	}
}

void Reshape (int w, int h) {
	if (h == 0)
		h = 1;

	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	/* note we divide our width by our height to get the aspect ratio */
	gluPerspective(60.0, (GLfloat) w / (GLfloat) h, 0.5, 300.0);

	glMatrixMode(GL_MODELVIEW);

//	winheight = h;
//	winwidth = w;
}

/* load textures */
/*int BuildTextures ( void ) {
	int 		i, size;
	GLubyte		*tex;
	const char 	*tex_files[TOTAL] =  { "carpet.raw", "wall.raw" };

	glGenTextures(TOTAL, textureID);
	for (i = 0; i < TOTAL; i++) {
		glBindTexture(GL_TEXTURE_2D, textureID[i]);
		size = loadTexture(tex_files[i], &tex);
		fprintf(stderr, "%s %d\n", tex_files[i], size);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, size, size, 0,
			GL_RGB, GL_UNSIGNED_BYTE, tex);
//		glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);	
		free(tex);
	}
} */

int InitData ( void ) {
	/* materials ? */
	glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);
//	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
	glMaterialf(GL_FRONT, GL_SHININESS, 20.);

	/* Setup gl attributes */
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glEnable(GL_BLEND);
	glEnable(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_ALPHA_TEST);
	glDisable(GL_DITHER);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	camera.position[0] = 0.2;
	camera.position[1] = 2.0;
	camera.position[2] = 1.5;
	camera.eye[0] = 6.5;
	camera.eye[1] = 2.0;
	camera.eye[2] = 1.5;
	return 0;
}

int main (int argc, char **argv) {
#ifdef NOT_YET
	printf("Arguments are:\n"
	       "\t-wX\tWindow width. Default: 400.\n"
	       "\t-hX\tWindow height. Default: 400.\n"
/*	  "\t-s\tStereo mode.\n" */
	       "\t-nX\tNumber of screens. Default: 5.\n"
	       "Anything else show this help.\n");

	while (argc > 1) {
		switch (argv[argc - 1][1]) {
		  case 'h':
			  winheight = atoi(&argv[argc - 1][2]);
			  break;
		  case 'w':
			  winwidth = atoi(&argv[argc - 1][2]);
			  break;
		  case 'n':
			  screens = atoi(&argv[argc - 1][2]);
			  break;
		  case 's':
			  stereo = 1;
			  break;
		  default:
			  exit(0);
			  break;
		}
		argc--;
	}
#endif

/*	DICE_init(&argc, argv);
	DICE_sync_new();
	DICE_sync();
*/
	glutInitWindowSize(500, 500);
	glutInit(&argc, argv);

	if (data.stereo)
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH |
				GLUT_STENCIL | GLUT_STEREO);
	else
		glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH |
				GLUT_STENCIL );

/*	if (data.fullscreen)
		; */
	glutCreateWindow("Street");

	if (InitData())
		exit(1);

	/* callbacks */
	glutReshapeFunc(Reshape);
	glutDisplayFunc(Display);
	glutKeyboardFunc(Key);
	glutSpecialFunc(Special);
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutMainLoop();
	return 0;
}
