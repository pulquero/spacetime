/* GLUTCAVE (c) Bruno Barberi Gnecco <brunobg@geocities.com>, 2001.
   Version 1.0B1

   This header is under public domain. Use it at your own risk.

You can get the latest version, along with complete documentation, at:
http://www../

HOW TO USE
----------
GLUTCAVE is a simple library (so good that fits in a C header;) that makes easy
the task of porting an existing GLUT program to run in a CAVE(tm) system. The
idea is to open either several windows, one for each projector, or one big 
window with the images inside it (easier to map in a SGI).

Here's a simple step-by-step list of what you have to do to port an application:

1. Insert '#include <glutcave.h>' in your file.
2. Insert 'GLUTCAVE_DATA' somewhere in your file, just as if you were declaring
  a global variable.
3. Replace the glutCreateWindow() call by 'GLUTCAVE_CREATE_WINDOWS_BEGIN(width,
  height)', or 'GLUTCAVE_CREATE_SUBWINDOWS_BEGIN(width, height)'; the first 
  argument is the window width, the other the window height (subwindow, if 
  using SUBWINDOWS_BEGIN()).
  GLUTCAVE_CREATE_WINDOWS_BEGIN() creates one window for each view.
  GLUTCAVE_CREATE_SUBWINDOWS_BEGIN() creates a big window, and each view is
  a subwindow.
4. Insert a GLUTCAVE_CREATE_[SUB]WINDOWS_END() after all code that is applied to 
  this window. This code includes glut callbacks, OpenGL initialization, etc. 
5. Replace all occurrences of glutPostRedisplay in your code by 
  GLUTCAVE_REDISPLAY().
6. Add a GLUTCAVE_ROTATE() just after glLoadIdentity() in your display function.
7. Add a GLUTCAVE_RESHAPE(width, height) in your reshape function, after 
  the glLoadIdentity() of GL_PROJECTION. This function does nothing if you
  are used GLUTCAVE_CREATE_WINDOWS_BEGIN, but is important in SUBWINDOWS. 
  Here's a sample of what your reshape callback should look like:
  
	void GLUTCALLBACK Reshape(int w,int h) {
		glViewport(0,0,w,h);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		GLUTCAVE_RESHAPE(w, h);
	}

  You may want to change GLUTCAVE_RESHAPE() to suit your physical setup,
  correcting fov, aspect ratio, etc. The default will work for cubic caves.

		  
SETUP
----- 
The current header supposes that you are using a 5-side cave, with 4 vertical
walls and the floor, that you look towards the X axis, and "up" is the Y axis.
To change this setup, do the following:

1. Change GLUTCAVE_TOTALSCREENS to reflect the number of screens. You can 
  change this in realtime using GLUTCAVE_SET_SCREENS(), if for some reason
  you want to use less screens than defined.
2. Change GLUTCAVE_data to reflect the screen names and transformations. The
  structure is the following: {screen name, {angle to rotate in degrees, 
  x, y, and z coordinates of the rotation vector}, -1}. 


FAQ & TROUBLESHOOTING
---------------------
Q: My views are all scrambled! Right is on top, top is bottom, etc!
A: Note that this header supposes that you look towards the X axis, and "up" is
  the Y axis. You can change this behaviour tunning GLUTCAVE_data, as explained
  in the SETUP section.

Q: Lighting is not correct: there are abrupt changes from view to view.
A: This has to do with the way that OpenGL deals with light. In short, light 
  position is transformed by the ModelView matrix, when you call
  glLightfv(GL_LIGHT_POSITION, ...). When you change ModelView (and that's what
  happens when you call GLUTCAVE_ROTATE()), lights are not automatically
  retransformed. To updated light position and make sure lighting is correct,
  call glLightfv after calling GLUTCAVE_ROTATE().

Q: I use gluLookAt, where should GLUTCAVE_ROTATE() go?
A: GLUTCAVE_ROTATE() should go before gluLookAt. In general, use GLUTCAVE_ROTATE
  just after glLoadIdentity.

Q: Only one of the screens is updated. What's wrong?
A: You are not calling GLUTCAVE_REDISPLAY(). It's *very* important to call 
  GLUTCAVE_REDISPLAY every time you want to redraw the scene. I saw several 
  programs that did not call glutPostRedisplay(), assuming that GLUT would take
  care of this. If you are using more than one window, only the *current* one
  gets updated. So insert some calls to GLUTCAVE_REDISPLAY() and it will work.

CHANGELOG
---------
v1.0	September XX, 2001
	+ Added the option of using subwindows.
	+ Added GLUTCAVE_SET_SCREENS().
	+ Added GLUTCAVE_RESHAPE().
	+ Deleted the unnecessary GLUTCAVE_INIT.
v0.9	April 9, 2001
	+ Initial release.


TODO
----
Improve RESHAPE, since it's kind of limiting by now.
*/

#ifndef GLUTCAVE_H
#define GLUTCAVE_H

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Default values.
 */ 
#define __GLUTCAVE_TOTALSCREENS					5

/*
 * Internal data.
 */
struct _glutcave_data {
	const char	*name;
	int		rot[4];
	int		win;
};

enum _glutcave_type { 
	GLUTCAVE_NONE, GLUTCAVE_WINDOWS, GLUTCAVE_SUBWINDOWS 
};

#define GLUTCAVE_DATA 						\
 struct _glutcave_data GLUTCAVE_data[__GLUTCAVE_TOTALSCREENS] = { \
	{ "Left",   {  90.0, 0.0, -1.0, 0.0 }, -1},		      \
	{ "Middle", {   0.0, 0.0,  0.0, 0.0 }, -1},		      \
	{ "Right",  {  90.0, 0.0,  1.0, 0.0 }, -1},		      \
	{ "Door",   { 180.0, 0.0,  1.0, 0.0 }, -1},		      \
	{ "Floor",  {  90.0, 1.0,  0.0, 0.0 }, -1} };		      \
 int GLUTCAVE_screens = 5;  					\
 int GLUTCAVE_mainwin;						\
 int GLUTCAVE_winwidth, GLUTCAVE_winheight;			\
 enum _glutcave_type GLUTCAVE_type = GLUTCAVE_NONE;

extern struct _glutcave_data GLUTCAVE_data[__GLUTCAVE_TOTALSCREENS];
extern int GLUTCAVE_screens;
extern int GLUTCAVE_mainwin;
extern int GLUTCAVE_winwidth, GLUTCAVE_winheight;
extern enum _glutcave_type GLUTCAVE_type;

/*
 * Initialize internal data 
 */
#define GLUTCAVE_INIT()						\
 /* left */							\
 GLUTCAVE_data[0].rot[0] = 90;	GLUTCAVE_data[0].rot[1] = 0;	\
 GLUTCAVE_data[0].rot[2] = -1;	GLUTCAVE_data[0].rot[3] = 0;	\
 /* middle */							\
 GLUTCAVE_data[1].rot[0] = 0;	GLUTCAVE_data[1].rot[1] = 0;	\
 GLUTCAVE_data[1].rot[2] = 0;	GLUTCAVE_data[1].rot[3] = 0;	\
 /* right */							\
 GLUTCAVE_data[2].rot[0] = 90;	GLUTCAVE_data[2].rot[1] = 0;	\
 GLUTCAVE_data[2].rot[2] = 1;	GLUTCAVE_data[2].rot[3] = 0;	\
 /* door */							\
 GLUTCAVE_data[3].rot[0] = 180;	GLUTCAVE_data[3].rot[1] = 0;	\
 GLUTCAVE_data[3].rot[2] = 1;	GLUTCAVE_data[3].rot[3] = 0;	\
 /* floor */							\
 GLUTCAVE_data[4].rot[0] = 90;	GLUTCAVE_data[4].rot[1] = 1;	\
 GLUTCAVE_data[4].rot[2] = 0;	GLUTCAVE_data[4].rot[3] = 0;


#define GLUTCAVE_SET_SCREENS(screens)				\
 if (screens <= __GLUTCAVE_TOTALSCREENS)			\
 	GLUTCAVE_screens = screens;
/*
 * Create windows code. All commands that should be applied to windows should
 * be placed between the GLUTCAVE_CREATE_WINDOWS_BEGIN() and 
 * GLUTCAVE_CREATE_WINDOWS_END() calls.
 * Parameters:
 *   width: window width.
 *   height: window height.
 */
#define GLUTCAVE_CREATE_WINDOWS_BEGIN(width,height)		\
{ int i;							\
 GLUTCAVE_type = GLUTCAVE_WINDOWS;				\
 GLUTCAVE_winwidth = width;					\
 GLUTCAVE_winheight = height;					\
 if ( width != 0 && height != 0 )				\
 	glutInitWindowSize(width,height);			\
 for ( i = 0; i < GLUTCAVE_screens; i++ ) {			\
 	if ( width != 0 && height != 0 )					\
		glutInitWindowPosition( (i%3)*(width+10),	\
					(i/3)*(height+40));	\
   	GLUTCAVE_data[i].win = glutCreateWindow(GLUTCAVE_data[i].name);

#define GLUTCAVE_CREATE_WINDOWS_END()				\
} }

 
/*
 * Create windows code. All commands that should be applied to windows should
 * be placed between the GLUTCAVE_CREATE_WINDOWS_BEGIN() and 
 * GLUTCAVE_CREATE_WINDOWS_END() calls.
 * Parameters:
 *   title: main window title.
 *   width: subwindow width.
 *   height: subwindow height.
 */
#define GLUTCAVE_CREATE_SUBWINDOWS_BEGIN(title,width,height)		\
{ int i;							\
 GLUTCAVE_type = GLUTCAVE_SUBWINDOWS;				\
 GLUTCAVE_winwidth = width;					\
 GLUTCAVE_winheight = height;					\
 if ( width != 0 && height != 0 )				\
 	glutInitWindowSize(width*3, height*(1+GLUTCAVE_screens/3));	\
 for ( i = -1; i < GLUTCAVE_screens; i++ ) {			\
	if (i == -1) {						\
		GLUTCAVE_mainwin = glutCreateWindow(title);	\
	}							\
	else {							\
	   	GLUTCAVE_data[i].win = 				\
			glutCreateSubWindow(GLUTCAVE_mainwin,	\
			(i%3)*width, (i/3)*height, 		\
			((i%3)+1)*width-1, ((i/3)+1)*height-1);	\
	}

#define GLUTCAVE_CREATE_SUBWINDOWS_END()			\
} }

/*
 * Replacement of glutPostRedisplay().
 */
#define GLUTCAVE_REDISPLAY() 					\
{ int i;							\
  for ( i = 0; i < GLUTCAVE_screens; i++ ) { 			\
  	glutSetWindow(GLUTCAVE_data[i].win);			\
  	glutPostRedisplay();  					\
  } 								\
}

/*
 * Set up rotation. Use just after glLoadIdentity().
 */
#define GLUTCAVE_ROTATE()					\
{ int i, j; 							\
  i = glutGetWindow(); 						\
  for ( j = 0; j < GLUTCAVE_screens; j++ ) { 			\
	if ( i == GLUTCAVE_data[j].win) { 			\
		glRotated(GLUTCAVE_data[j].rot[0],		\
			  GLUTCAVE_data[j].rot[1],		\
			  GLUTCAVE_data[j].rot[2],		\
			  GLUTCAVE_data[j].rot[3]);		\
		break; 						\
	} 							\
  } 								\
}

/*
 * Set up . Use just after glLoadIdentity().
 */
#define GLUTCAVE_RESHAPE(width, height, znear, zfar)		\
if (GLUTCAVE_type == GLUTCAVE_SUBWINDOWS) {			\
 if (glutGetWindow() == GLUTCAVE_mainwin) {			\
	int i;							\
	/* adjust main window size */				\
	GLUTCAVE_winwidth = width/3;				\
	GLUTCAVE_winheight = height/(1+GLUTCAVE_screens/3);	\
	/* adjust other subwindows */				\
	for ( i = 0; i < GLUTCAVE_screens; i++ ) {		\
		glutSetWindow(GLUTCAVE_data[i].win);  	\
		glutPositionWindow((i%3)*GLUTCAVE_winwidth, 	\
				(i/3)*GLUTCAVE_winheight);	\
		glutReshapeWindow(GLUTCAVE_winwidth, 		\
				GLUTCAVE_winheight); 		\
		glMatrixMode(GL_PROJECTION);			\
		glLoadIdentity();				\
		gluPerspective(90.0,1.0,znear,zfar);		\
		glMatrixMode(GL_MODELVIEW);			\
	}							\
 }								\
}


#ifdef __cplusplus
}
#endif
 
#endif /* GLUTCAVE_H */
