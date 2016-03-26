/* Bruno Barberi Gnecco <brunobg@geocities.com>, 2001. */

#include <gl\gl.h>
#include <srel.h>
#include "street.h"

void render_street ( void ) {
	Point3 *vertices = street_vertices[0];
	long *faces = street_faces[0];
	int j = 0;
	GLfloat colors[3][4] = { { 1.0, 0.0, 0.0, 1.0 },
				 { 0.0, 1.0, 0.0, 1.0 },
				 { 0.0, 0.0, 1.0, 1.0 } };


	while (faces) {
		int i = 0;

		/* suppose that they are all triangles */
		srelLocalTime(0.0f);
		srelVelocityf(0.0f,0.8f,0.0f);
		srelColor4fv(colors[j%3]);
		srelBegin(GL_TRIANGLES);
		while (faces[i] != -2) {
			srelVertex3dv(vertices[faces[i]]);
			srelVertex3dv(vertices[faces[i+1]]);
			srelVertex3dv(vertices[faces[i+2]]);
			i += 4;
		}
		srelEnd();
		faces = street_faces[++j];
		vertices = street_vertices[j];
	}
}
