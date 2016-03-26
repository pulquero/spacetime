/* Bruno Barberi Gnecco <brunobg@geocities.com>, 2001. */

#ifndef STREET_H
#define STREET_H

#include <stdio.h>
#include <GL/gl.h>

typedef struct {
	GLboolean		fullscreen;
	GLboolean		stereo;
	GLboolean		demo;
} Data;
extern Data data;

/* street geometry */
typedef struct Point2Struct {
	double x, y;
} Point2;

/*typedef struct Point3Struct {
	double x, y, z;
} Point3; */
typedef double Point3 [3];

extern Point3 *street_vertices[];
extern long *street_faces[];
extern void mainstreet(void);
extern void render_street(void);
#endif
