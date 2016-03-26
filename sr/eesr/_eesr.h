/**
*   EESR OpenGL API
*   Copyright (C) 2000-2001  Mark Hale (mark.hale@physics.org)
*
*   This program is free software; you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*   the Free Software Foundation; either version 2 of the License, or
*   (at your option) any later version.
*
*   This program is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with this program; if not, write to the Free Software
*   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/



#ifndef __IBMC__
   #define _Inline inline
#endif

#define PI       3.14159265358979323846
#define SQRT_1_2 0.70710678118654752440
#define SQRT_1_3 0.57735026918962576449

/* lattice settings */
#define GRID_SIZE    5
#define GRID_SPACING 5.0f
#define ROD_LENGTH   GRID_SIZE*GRID_SPACING

_Inline void LINKAGE _DrawVRod(GLfloat x,GLfloat y,GLfloat z);
_Inline void LINKAGE _DrawHRod(GLfloat x,GLfloat y,GLfloat z);
_Inline void LINKAGE _DrawDRod(GLfloat x,GLfloat y,GLfloat z);

/* sphere settings */
#define SPHERE_RADIUS 10.0f
#define SPHERE_X      0.525731112119133606f*SPHERE_RADIUS
#define SPHERE_Z      0.850650808352039932f*SPHERE_RADIUS

static void LINKAGE _SubdivideSphere(const GLfloat *v1, const GLfloat *v2, const GLfloat *v3, const int depth);
_Inline void LINKAGE _ScaleSphere(GLfloat *v);
_Inline void LINKAGE _DrawTriangle(const GLfloat *v1, const GLfloat *v2, const GLfloat *v3);
