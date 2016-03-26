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



#ifdef __IBMC__
   #define LINKAGE _Optlink
#else
   #define LINKAGE
#endif


/* global variables */
extern GLfloat eesrSpeed;
extern GLboolean eesrLoop;

/* function prototypes */
extern void LINKAGE eesrInit(void);
extern void LINKAGE eesrResize(long width,long height);
extern void LINKAGE eesrDrawGeometry(void);
extern void LINKAGE eesrSetRestFrame(void (* LINKAGE func)(void));
extern void LINKAGE eesrSetMovingFrame(void (* LINKAGE func)(void));
extern void LINKAGE eesrSetTime(const float t);
extern float LINKAGE eesrGetTime(void);
extern void LINKAGE eesrDrawWorld(void);
extern void LINKAGE eesrDrawObject(void);
extern void LINKAGE eesrSetObject(void (* LINKAGE func)(void));
extern void LINKAGE eesrDrawCar(void);
extern void LINKAGE eesrDrawLattice(void);
extern void LINKAGE eesrDrawAeroplane(void);
extern void LINKAGE eesrDrawSphere(void);
