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



#define _FP_INLINE
#ifdef unix
   #include <GL/gl.h>
   #include <GL/glu.h>
#else
   #ifdef WIN32
      #include <windows.h>
   #endif
   #include <gl\gl.h>
   #include <gl\glu.h>
#endif
#include <stdlib.h>
#include <math.h>
#include "../srel/srel.h"
#include "eesr.h"
#include "_eesr.h"
#include "texture.c"

GLfloat eesrSpeed=0.0;
GLboolean eesrLoop=GL_FALSE;
static GLfloat eesrTime=0.0;

static void (* LINKAGE rest_frame_ptr)(void)=eesrDrawWorld;
static void (* LINKAGE moving_frame_ptr)(void)=eesrDrawObject;
static void (* LINKAGE object_ptr)(void)=eesrDrawLattice;
static GLuint lattice_mat,sphere_mat,glass_mat,metal_mat,grass_mat,logo_mat;
static char tex[5][3*64*64];

const static GLfloat object_specular[4]={0.9,0.6,0.6,1.0};
const static GLfloat object_amb_diff[4]={0.8,0.5,0.3,1.0};

/**
* Initialise OpenGL and srel.
*/
void eesrInit(void)
{
   const GLfloat object_shininess[1]={50.0};
   const GLfloat window_amb_diff[4]={0.5,0.5,1.0,1.0};
   const GLfloat grass_shininess[1]={0.0};
   const GLfloat grass_specular[4]={0.0,0.0,0.0,1.0};
   const GLfloat grass_amb_diff[4]={0.2,0.6,0.2,1.0};
   const GLfloat global_amb[4]={0.3,0.3,0.3,1.0};
   const GLfloat local_amb[4]={0.6,0.6,0.6,1.0};
   const GLfloat fog_color[4]={0.6,0.6,0.6,1.0};

// lattice material
   lattice_mat=glGenLists(6);
   glNewList(lattice_mat,GL_COMPILE);
      Load24BitBitmap("object.bmp",tex[0],3*64*64);
      gluBuild2DMipmaps(GL_TEXTURE_2D,3,64,64,GL_RGB,GL_UNSIGNED_BYTE,tex[0]);
      glMaterialfv(GL_FRONT,GL_SHININESS,object_shininess);
      glMaterialfv(GL_FRONT,GL_SPECULAR,object_specular);
      glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,object_amb_diff);
   glEndList();
// sphere material
   sphere_mat=lattice_mat+1;
   glNewList(sphere_mat,GL_COMPILE);
      glMaterialfv(GL_FRONT,GL_SHININESS,object_shininess);
      glMaterialfv(GL_FRONT,GL_SPECULAR,object_specular);
      glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,object_amb_diff);
   glEndList();
// glass material
   glass_mat=lattice_mat+2;
   glNewList(glass_mat,GL_COMPILE);
      Load24BitBitmap("glass.bmp",tex[1],3*64*64);
      gluBuild2DMipmaps(GL_TEXTURE_2D,3,64,64,GL_RGB,GL_UNSIGNED_BYTE,tex[1]);
      glMaterialfv(GL_FRONT,GL_SHININESS,object_shininess);
      glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,window_amb_diff);
   glEndList();
// metal material
   metal_mat=lattice_mat+3;
   glNewList(metal_mat,GL_COMPILE);
      Load24BitBitmap("metal.bmp",tex[2],3*64*64);
      gluBuild2DMipmaps(GL_TEXTURE_2D,3,64,64,GL_RGB,GL_UNSIGNED_BYTE,tex[2]);
      glMaterialfv(GL_FRONT,GL_SHININESS,object_shininess);
   glEndList();
// grass material
   grass_mat=lattice_mat+4;
   glNewList(grass_mat,GL_COMPILE);
      Load24BitBitmap("grass.bmp",tex[3],3*64*64);
      gluBuild2DMipmaps(GL_TEXTURE_2D,3,64,64,GL_RGB,GL_UNSIGNED_BYTE,tex[3]);
      glMaterialfv(GL_FRONT_AND_BACK,GL_SHININESS,grass_shininess);
      glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,grass_specular);
      glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,grass_amb_diff);
   glEndList();
// logo material
   logo_mat=lattice_mat+5;
   glNewList(logo_mat,GL_COMPILE);
      Load24BitBitmap("logo.bmp",tex[4],3*64*64);
      gluBuild2DMipmaps(GL_TEXTURE_2D,3,64,64,GL_RGB,GL_UNSIGNED_BYTE,tex[4]);
      glMaterialfv(GL_FRONT,GL_SHININESS,object_shininess);
   glEndList();

// Lighting
   glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,GL_FALSE);   // FALSE for speed
   glLightModelfv(GL_LIGHT_MODEL_AMBIENT,global_amb);
   glEnable(GL_LIGHTING);
   glLightfv(GL_LIGHT0,GL_AMBIENT,local_amb);
   glEnable(GL_LIGHT0);
// Fog
   glHint(GL_FOG_HINT,GL_FASTEST);
   glFogf(GL_FOG_MODE,GL_EXP);
   glFogf(GL_FOG_DENSITY,0.02);
   glFogfv(GL_FOG_COLOR,fog_color);
// Texture
   glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
   glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST_MIPMAP_NEAREST);
   glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
   glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
   glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);

   glEnable(GL_DEPTH_TEST);
   srelVelocityf(0.0,0.0,0.0);
   srelLocalTime(0.0);
   srelDoppler(GL_TRUE);
}

/**
* Resize function.
*/
void eesrResize(long width,long height)
{
   glViewport(0,0,width,height);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluPerspective(50.0,(GLdouble)(width)/(GLdouble)(height),1.0,1000.0);
   glMatrixMode(GL_MODELVIEW);
}

/**
* Draw the scene (foreground and background objects).
*/
void eesrDrawGeometry(void)
{
   srelVelocityf(0.0,0.0,0.0);
   (*rest_frame_ptr)();
   srelVelocityf(0.0,0.0,eesrSpeed);
   (*moving_frame_ptr)();
   if(eesrLoop && eesrTime>100.0f)
      srelLocalTime(0.0);
}

/**
* Sets the rest frame.
* @param func Should either be eesrDrawWorld() or eesrDrawObject().
*/
void eesrSetRestFrame(void (* LINKAGE func)(void))
{
   rest_frame_ptr=func;
}

/**
* Sets the moving frame.
* @param func Should either be eesrDrawWorld() or eesrDrawObject().
*/
void eesrSetMovingFrame(void (* LINKAGE func)(void))
{
   moving_frame_ptr=func;
}

void eesrSetObject(void (* LINKAGE func)(void))
{
   object_ptr=func;
}

void eesrSetTime(const float t)
{
   eesrTime=t;
   srelLocalTime(eesrTime);
}

float eesrGetTime(void)
{
   return eesrTime;
}


/**
* Draw the world and background objects.
*/
void eesrDrawWorld(void)
{
   const static GLfloat norm_zpz[3]={0.0,1.0,0.0};
   const static GLfloat grass_tex[4][2]={ {0.0,0.0},
                                          {0.0,50.0},
                                          {5.0,0.0},
                                          {5.0,50.0} };
   const static GLfloat grassA_points[4][3]={ {-12.5,0.0,100.0},
                                              {-12.5,0.0,-100.0},
                                              {-25.0,0.0,100.0},
                                              {-25.0,0.0,-100.0} };
   const static GLfloat grassB_points[4][3]={ {12.5,0.0,100.0},
                                              {12.5,0.0,-100.0},
                                              {25.0,0.0,100.0},
                                              {25.0,0.0,-100.0} };

   glCallList(grass_mat);
   srelBegin(GL_QUAD_STRIP);
      srelNormal3fv(norm_zpz);
      srelTexCoord2fv(grass_tex[0]);
      srelVertex3fv(grassA_points[0]);
      srelTexCoord2fv(grass_tex[1]);
      srelVertex3fv(grassA_points[1]);
      srelTexCoord2fv(grass_tex[2]);
      srelVertex3fv(grassA_points[2]);
      srelTexCoord2fv(grass_tex[3]);
      srelVertex3fv(grassA_points[3]);
   srelEnd();
   srelBegin(GL_QUAD_STRIP);
      srelNormal3fv(norm_zpz);
      srelTexCoord2fv(grass_tex[0]);
      srelVertex3fv(grassB_points[0]);
      srelTexCoord2fv(grass_tex[1]);
      srelVertex3fv(grassB_points[1]);
      srelTexCoord2fv(grass_tex[2]);
      srelVertex3fv(grassB_points[2]);
      srelTexCoord2fv(grass_tex[3]);
      srelVertex3fv(grassB_points[3]);
   srelEnd();
}

/**
* Draw the foreground objects.
*/
void eesrDrawObject(void)
{
   (*object_ptr)();
}


void eesrDrawCar(void)
{
   const static GLfloat car_amb_diff[4]={0.8,0.4,0.3,1.0};
   const static GLfloat norm_pzz[3]={1.0,0.0,0.0};
   const static GLfloat norm_mzz[3]={-1.0,0.0,0.0};
   const static GLfloat norm_zpz[3]={0.0,1.0,0.0};
   const static GLfloat norm_zmz[3]={0.0,-1.0,0.0};
   const static GLfloat norm_zzp[3]={0.0,0.0,1.0};
   const static GLfloat norm_zzm[3]={0.0,0.0,-1.0};

   glCallList(metal_mat);
   glMaterialfv(GL_FRONT,GL_SPECULAR,object_specular);
   glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,car_amb_diff);
   srelBegin(GL_QUADS);
// Front
      srelNormal3fv(norm_zzp);
      srelTexCoord2f(0.0,0.0);
      srelVertex3f(-5.0,2.0,0.0);
      srelTexCoord2f(1.0,0.0);
      srelVertex3f(5.0,2.0,0.0);
      srelTexCoord2f(0.0,1.0);
      srelVertex3f(5.0,5.0,0.0);
      srelTexCoord2f(1.0,1.0);
      srelVertex3f(-5.0,5.0,0.0);
// Back
      srelNormal3fv(norm_zzm);
      srelTexCoord2f(0.0,0.0);
      srelVertex3f(-5.0,2.0,-20.0);
      srelTexCoord2f(1.0,0.0);
      srelVertex3f(5.0,2.0,-20.0);
      srelTexCoord2f(0.0,1.0);
      srelVertex3f(5.0,5.0,-20.0);
      srelTexCoord2f(1.0,1.0);
      srelVertex3f(-5.0,5.0,-20.0);
   srelEnd();
// Bonet
   srelBegin(GL_QUAD_STRIP);
      srelNormal3fv(norm_zpz);
      srelTexCoord2f(0.0,0.0);
      srelVertex3f(5.0,5.0,0.0);
      srelTexCoord2f(0.0,3.0);
      srelVertex3f(5.0,5.0,-6.0);
      srelTexCoord2f(1.0,0.0);
      srelVertex3f(-5.0,5.0,0.0);
      srelTexCoord2f(1.0,3.0);
      srelVertex3f(-5.0,5.0,-6.0);
   srelEnd();
// Floor
   srelBegin(GL_QUAD_STRIP);
      srelNormal3fv(norm_zmz);
      srelTexCoord2f(0.0,0.0);
      srelVertex3f(5.0,2.0,0.0);
      srelTexCoord2f(0.0,5.0);
      srelVertex3f(5.0,2.0,-20.0);
      srelTexCoord2f(1.0,0.0);
      srelVertex3f(-5.0,2.0,0.0);
      srelTexCoord2f(1.0,5.0);
      srelVertex3f(-5.0,2.0,-20.0);
   srelEnd();
// Roof
   srelBegin(GL_QUAD_STRIP);
      srelNormal3fv(norm_zpz);
      srelTexCoord2f(0.0,0.0);
      srelVertex3f(-5.0,8.0,-8.0);
      srelTexCoord2f(1.0,0.0);
      srelVertex3f(-5.0,8.0,-18.0);
      srelTexCoord2f(0.0,1.0);
      srelVertex3f(5.0,8.0,-8.0);
      srelTexCoord2f(1.0,1.0);
      srelVertex3f(5.0,8.0,-18.0);
   srelEnd();
   glCallList(glass_mat);
   srelBegin(GL_QUADS);
// Front Windscreen
      srelNormal3f(0.0,0.5547002,0.8320502);
      srelTexCoord2f(0.0,0.0);
      srelVertex3f(-5.0,5.0,-6.0);
      srelTexCoord2f(1.0,0.0);
      srelVertex3f(5.0,5.0,-6.0);
      srelTexCoord2f(0.0,1.0);
      srelVertex3f(5.0,8.0,-8.0);
      srelTexCoord2f(1.0,1.0);
      srelVertex3f(-5.0,8.0,-8.0);
// Back Windscreen
      srelNormal3f(0.0,0.5547002,-0.8320502);
      srelTexCoord2f(0.0,0.0);
      srelVertex3f(-5.0,8.0,-18.0);
      srelTexCoord2f(1.0,0.0);
      srelVertex3f(5.0,8.0,-18.0);
      srelTexCoord2f(0.0,1.0);
      srelVertex3f(5.0,5.0,-20.0);
      srelTexCoord2f(1.0,1.0);
      srelVertex3f(-5.0,5.0,-20.0);
   srelEnd();
// Left Side
   glCallList(metal_mat);
   glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,car_amb_diff);
   srelBegin(GL_QUAD_STRIP);
      srelNormal3fv(norm_mzz);
      srelTexCoord2f(0.0,0.0);
      srelVertex3f(-5.0,5.0,0.0);
      srelTexCoord2f(1.0,0.0);
      srelVertex3f(-5.0,5.0,-20.0);
      srelTexCoord2f(0.0,1.0);
      srelVertex3f(-5.0,2.0,0.0);
      srelTexCoord2f(1.0,1.0);
      srelVertex3f(-5.0,2.0,-20.0);
   srelEnd();
   glCallList(glass_mat);
   srelBegin(GL_QUAD_STRIP);
      srelNormal3fv(norm_mzz);
      srelTexCoord2f(0.0,0.0);
      srelVertex3f(-5.0,8.0,-8.0);
      srelTexCoord2f(0.0,1.0);
      srelVertex3f(-5.0,8.0,-18.0);
      srelTexCoord2f(1.0,0.0);
      srelVertex3f(-5.0,5.0,-6.0);
      srelTexCoord2f(1.0,1.0);
      srelVertex3f(-5.0,5.0,-20.0);
   srelEnd();
// Right Side
   glCallList(metal_mat);
   glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,car_amb_diff);
   srelBegin(GL_QUAD_STRIP);
      srelNormal3fv(norm_pzz);
      srelTexCoord2f(0.0,0.0);
      srelVertex3f(5.0,5.0,0.0);
      srelTexCoord2f(1.0,0.0);
      srelVertex3f(5.0,5.0,-20.0);
      srelTexCoord2f(0.0,1.0);
      srelVertex3f(5.0,2.0,0.0);
      srelTexCoord2f(1.0,1.0);
      srelVertex3f(5.0,2.0,-20.0);
   srelEnd();
   glCallList(glass_mat);
   srelBegin(GL_QUAD_STRIP);
      srelNormal3fv(norm_pzz);
      srelTexCoord2f(0.0,0.0);
      srelVertex3f(5.0,8.0,-8.0);
      srelTexCoord2f(0.0,1.0);
      srelVertex3f(5.0,8.0,-18.0);
      srelTexCoord2f(1.0,0.0);
      srelVertex3f(5.0,5.0,-6.0);
      srelTexCoord2f(1.0,1.0);
      srelVertex3f(5.0,5.0,-20.0);
   srelEnd();
}

void eesrDrawAeroplane(void)
{
   const static GLfloat plane_specular[4]={0.4,0.4,0.4,1.0};
   const static GLfloat norm_pzz[3]={1.0,0.0,0.0};
   const static GLfloat norm_mzz[3]={-1.0,0.0,0.0};
   const static GLfloat norm_zpz[3]={0.0,1.0,0.0};
   const static GLfloat norm_zmz[3]={0.0,-1.0,0.0};
   const static GLfloat norm_zzp[3]={0.0,0.0,1.0};
   const static GLfloat norm_zzm[3]={0.0,0.0,-1.0};
   const static GLfloat norm_ppp[3]={SQRT_1_3,SQRT_1_3,SQRT_1_3};
   const static GLfloat norm_mpp[3]={-SQRT_1_3,SQRT_1_3,SQRT_1_3};
   const static GLfloat norm_pmp[3]={SQRT_1_3,-SQRT_1_3,SQRT_1_3};
   const static GLfloat norm_mmp[3]={-SQRT_1_3,-SQRT_1_3,SQRT_1_3};
   const static GLfloat norm_pzp[3]={SQRT_1_2,0.0,SQRT_1_2};
   const static GLfloat norm_mzp[3]={-SQRT_1_2,0.0,SQRT_1_2};
   const static GLfloat norm_zpp[3]={0.0,SQRT_1_2,SQRT_1_2};
   const static GLfloat norm_zmp[3]={0.0,-SQRT_1_2,SQRT_1_2};
   const static GLfloat norm_ppz[3]={SQRT_1_2,SQRT_1_2,0.0};
   const static GLfloat norm_pmz[3]={SQRT_1_2,-SQRT_1_2,0.0};
   const static GLfloat norm_mpz[3]={-SQRT_1_2,SQRT_1_2,0.0};
   const static GLfloat norm_mmz[3]={-SQRT_1_2,-SQRT_1_2,0.0};

// Cockpit Upper Half (Window)
   glCallList(glass_mat);
   glMaterialfv(GL_FRONT,GL_SPECULAR,plane_specular);
   srelBegin(GL_TRIANGLE_FAN);
      srelNormal3fv(norm_zzp);
      srelTexCoord2f(0.0,0.5);
      srelVertex3f(0.0,10.0,5.0);
      srelNormal3fv(norm_mzp);
      srelTexCoord2f(1.0,0.0);
      srelVertex3f(-5.0,10.0,0.0);
      srelNormal3fv(norm_mpp);
      srelTexCoord2f(1.0,0.2);
      srelVertex3f(-5.0,13.0,0.0);
      srelNormal3fv(norm_zpp);
      srelTexCoord2f(1.0,0.4);
      srelVertex3f(-3.0,15.0,0.0);
      srelTexCoord2f(1.0,0.6);
      srelVertex3f(3.0,15.0,0.0);
      srelNormal3fv(norm_ppp);
      srelTexCoord2f(1.0,0.8);
      srelVertex3f(5.0,13.0,0.0);
      srelNormal3fv(norm_pzp);
      srelTexCoord2f(1.0,1.0);
      srelVertex3f(5.0,10.0,0.0);
   srelEnd();
// Cockpit Lower Half
   glCallList(metal_mat);
   glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,object_amb_diff);
   srelBegin(GL_TRIANGLE_FAN);
      srelNormal3fv(norm_zzp);
      srelTexCoord2f(0.0,0.5);
      srelVertex3f(0.0,10.0,5.0);
      srelNormal3fv(norm_pzp);
      srelTexCoord2f(1.0,1.0);
      srelVertex3f(5.0,10.0,0.0);
      srelNormal3fv(norm_pmp);
      srelTexCoord2f(1.0,0.8);
      srelVertex3f(5.0,7.0,0.0);
      srelNormal3fv(norm_zmp);
      srelTexCoord2f(1.0,0.6);
      srelVertex3f(3.0,5.0,0.0);
      srelTexCoord2f(1.0,0.4);
      srelVertex3f(-3.0,5.0,0.0);
      srelNormal3fv(norm_mmp);
      srelTexCoord2f(1.0,0.2);
      srelVertex3f(-5.0,7.0,0.0);
      srelNormal3fv(norm_mzp);
      srelTexCoord2f(1.0,0.0);
      srelVertex3f(-5.0,10.0,0.0);
   srelEnd();
// Back
   srelBegin(GL_TRIANGLE_FAN);
      srelNormal3fv(norm_zzm);
      srelTexCoord2f(0.0,0.5);
      srelVertex3f(0.0,10.0,-50.0);
      srelTexCoord2f(1.0,0.0);
      srelVertex3f(-5.0,13.0,-50.0);
      srelTexCoord2f(1.0,0.125);
      srelVertex3f(-3.0,15.0,-50.0);
      srelTexCoord2f(1.0,0.25);
      srelVertex3f(3.0,15.0,-50.0);
      srelTexCoord2f(1.0,0.375);
      srelVertex3f(5.0,13.0,-50.0);
      srelTexCoord2f(1.0,0.5);
      srelVertex3f(5.0,7.0,-50.0);
      srelTexCoord2f(1.0,0.625);
      srelVertex3f(3.0,5.0,-50.0);
      srelTexCoord2f(1.0,0.75);
      srelVertex3f(-3.0,5.0,-50.0);
      srelTexCoord2f(1.0,0.875);
      srelVertex3f(-5.0,7.0,-50.0);
      srelTexCoord2f(1.0,0.0);
      srelVertex3f(-5.0,13.0,-50.0);
   srelEnd();
// Fuselage
   srelBegin(GL_QUAD_STRIP);
   // top-half
      srelNormal3fv(norm_mpz);
      srelTexCoord2f(0.0,0.0);
      srelVertex3f(-5.0,13.0,0.0);
      srelTexCoord2f(0.0,10.0);
      srelVertex3f(-5.0,13.0,-50.0);
      srelNormal3fv(norm_zpz);
      srelTexCoord2f(1.0,0.0);
      srelVertex3f(-3.0,15.0,0.0);
      srelTexCoord2f(1.0,10.0);
      srelVertex3f(-3.0,15.0,-50.0);
      srelNormal3fv(norm_ppz);
      srelTexCoord2f(2.0,0.0);
      srelVertex3f(3.0,15.0,0.0);
      srelTexCoord2f(2.0,10.0);
      srelVertex3f(3.0,15.0,-50.0);
      srelNormal3fv(norm_pzz);
      srelTexCoord2f(3.0,0.0);
      srelVertex3f(5.0,13.0,0.0);
      srelTexCoord2f(3.0,10.0);
      srelVertex3f(5.0,13.0,-50.0);
   // bottom-half
      srelNormal3fv(norm_pmz);
      srelTexCoord2f(4.0,0.0);
      srelVertex3f(5.0,7.0,0.0);
      srelTexCoord2f(4.0,10.0);
      srelVertex3f(5.0,7.0,-50.0);
      srelNormal3fv(norm_zmz);
      srelTexCoord2f(5.0,0.0);
      srelVertex3f(3.0,5.0,0.0);
      srelTexCoord2f(5.0,10.0);
      srelVertex3f(3.0,5.0,-50.0);
      srelNormal3fv(norm_mmz);
      srelTexCoord2f(6.0,0.0);
      srelVertex3f(-3.0,5.0,0.0);
      srelTexCoord2f(6.0,10.0);
      srelVertex3f(-3.0,5.0,-50.0);
      srelNormal3fv(norm_mzz);
      srelTexCoord2f(7.0,0.0);
      srelVertex3f(-5.0,7.0,0.0);
      srelTexCoord2f(7.0,10.0);
      srelVertex3f(-5.0,7.0,-50.0);
      srelNormal3fv(norm_zpz);
      srelTexCoord2f(8.0,0.0);
      srelVertex3f(-5.0,13.0,0.0);
      srelTexCoord2f(8.0,10.0);
      srelVertex3f(-5.0,13.0,-50.0);
   srelEnd();
// Wings
   srelBegin(GL_QUAD_STRIP);
      srelNormal3fv(norm_zpz);
      srelTexCoord2f(0.0,0.0);
      srelVertex3f(0.0,11.5,-15.0);
      srelTexCoord2f(1.0,0.0);
      srelVertex3f(0.0,11.5,-30.0);
      srelTexCoord2f(0.0,1.0);
      srelVertex3f(40.0,10.5,-25.0);
      srelTexCoord2f(1.0,1.0);
      srelVertex3f(40.0,10.5,-30.0);
      srelNormal3fv(norm_pzz);
      srelTexCoord2f(0.0,2.0);
      srelVertex3f(40.0,9.5,-25.0);
      srelTexCoord2f(1.0,2.0);
      srelVertex3f(40.0,9.5,-30.0);
      srelNormal3fv(norm_zmz);
      srelTexCoord2f(0.0,3.0);
      srelVertex3f(0.0,8.5,-15.0);
      srelTexCoord2f(1.0,3.0);
      srelVertex3f(0.0,8.5,-30.0);
      srelTexCoord2f(0.0,4.0);
      srelVertex3f(-40.0,9.5,-25.0);
      srelTexCoord2f(1.0,4.0);
      srelVertex3f(-40.0,9.5,-30.0);
      srelNormal3fv(norm_mzz);
      srelTexCoord2f(0.0,5.0);
      srelVertex3f(-40.0,10.5,-25.0);
      srelTexCoord2f(1.0,5.0);
      srelVertex3f(-40.0,10.5,-30.0);
      srelNormal3fv(norm_zpz);
      srelTexCoord2f(0.0,6.0);
      srelVertex3f(0.0,11.5,-15.0);
      srelTexCoord2f(1.0,6.0);
      srelVertex3f(0.0,11.5,-30.0);
   srelEnd();
   srelBegin(GL_QUAD_STRIP);
      srelNormal3fv(norm_zzp);
      srelTexCoord2f(0.0,0.0);
      srelVertex3f(0.0,11.5,-15.0);
      srelTexCoord2f(1.0,0.0);
      srelVertex3f(0.0,8.5,-15.0);
      srelTexCoord2f(0.0,1.0);
      srelVertex3f(40.0,10.5,-25.0);
      srelTexCoord2f(1.0,1.0);
      srelVertex3f(40.0,9.5,-25.0);
      srelNormal3fv(norm_pzz);
      srelTexCoord2f(0.0,2.0);
      srelVertex3f(40.0,10.5,-30.0);
      srelTexCoord2f(1.0,2.0);
      srelVertex3f(40.0,9.5,-30.0);
      srelNormal3fv(norm_zzm);
      srelTexCoord2f(0.0,3.0);
      srelVertex3f(0.0,11.5,-30.0);
      srelTexCoord2f(1.0,3.0);
      srelVertex3f(0.0,8.5,-30.0);
      srelTexCoord2f(0.0,4.0);
      srelVertex3f(-40.0,10.5,-30.0);
      srelTexCoord2f(1.0,4.0);
      srelVertex3f(-40.0,9.5,-30.0);
      srelNormal3fv(norm_mzz);
      srelTexCoord2f(0.0,5.0);
      srelVertex3f(-40.0,10.5,-25.0);
      srelTexCoord2f(1.0,5.0);
      srelVertex3f(-40.0,9.5,-25.0);
      srelNormal3fv(norm_zzp);
      srelTexCoord2f(0.0,6.0);
      srelVertex3f(0.0,11.5,-15.0);
      srelTexCoord2f(1.0,6.0);
      srelVertex3f(0.0,8.5,-15.0);
   srelEnd();
// Tail Plane
   srelBegin(GL_QUAD_STRIP);
      srelNormal3fv(norm_zpz);
      srelTexCoord2f(0.0,0.0);
      srelVertex3f(0.0,11.5,-40.0);
      srelTexCoord2f(1.0,0.0);
      srelVertex3f(0.0,11.5,-50.0);
      srelTexCoord2f(0.0,1.0);
      srelVertex3f(15.0,10.5,-45.0);
      srelTexCoord2f(1.0,1.0);
      srelVertex3f(15.0,10.5,-50.0);
      srelNormal3fv(norm_pzz);
      srelTexCoord2f(0.0,2.0);
      srelVertex3f(15.0,9.5,-45.0);
      srelTexCoord2f(1.0,2.0);
      srelVertex3f(15.0,9.5,-50.0);
      srelNormal3fv(norm_zmz);
      srelTexCoord2f(0.0,3.0);
      srelVertex3f(0.0,8.5,-40.0);
      srelTexCoord2f(1.0,3.0);
      srelVertex3f(0.0,8.5,-50.0);
      srelTexCoord2f(0.0,4.0);
      srelVertex3f(-15.0,9.5,-45.0);
      srelTexCoord2f(1.0,4.0);
      srelVertex3f(-15.0,9.5,-50.0);
      srelNormal3fv(norm_mzz);
      srelTexCoord2f(0.0,5.0);
      srelVertex3f(-15.0,10.5,-45.0);
      srelTexCoord2f(1.0,5.0);
      srelVertex3f(-15.0,10.5,-50.0);
      srelNormal3fv(norm_zpz);
      srelTexCoord2f(0.0,6.0);
      srelVertex3f(0.0,11.5,-40.0);
      srelTexCoord2f(1.0,6.0);
      srelVertex3f(0.0,11.5,-50.0);
   srelEnd();
   srelBegin(GL_QUAD_STRIP);
      srelNormal3fv(norm_zzp);
      srelTexCoord2f(0.0,0.0);
      srelVertex3f(0.0,11.5,-40.0);
      srelTexCoord2f(1.0,0.0);
      srelVertex3f(0.0,8.5,-40.0);
      srelTexCoord2f(0.0,1.0);
      srelVertex3f(15.0,10.5,-45.0);
      srelTexCoord2f(1.0,1.0);
      srelVertex3f(15.0,9.5,-45.0);
      srelNormal3fv(norm_pzz);
      srelTexCoord2f(0.0,2.0);
      srelVertex3f(15.0,10.5,-50.0);
      srelTexCoord2f(1.0,2.0);
      srelVertex3f(15.0,9.5,-50.0);
      srelNormal3fv(norm_zzm);
      srelTexCoord2f(0.0,3.0);
      srelVertex3f(0.0,11.5,-50.0);
      srelTexCoord2f(1.0,3.0);
      srelVertex3f(0.0,8.5,-50.0);
      srelTexCoord2f(0.0,4.0);
      srelVertex3f(-15.0,10.5,-50.0);
      srelTexCoord2f(1.0,4.0);
      srelVertex3f(-15.0,9.5,-50.0);
      srelNormal3fv(norm_mzz);
      srelTexCoord2f(0.0,5.0);
      srelVertex3f(-15.0,10.5,-45.0);
      srelTexCoord2f(1.0,5.0);
      srelVertex3f(-15.0,9.5,-45.0);
      srelNormal3fv(norm_zzp);
      srelTexCoord2f(0.0,6.0);
      srelVertex3f(0.0,11.5,-40.0);
      srelTexCoord2f(1.0,6.0);
      srelVertex3f(0.0,8.5,-40.0);
   srelEnd();
   srelBegin(GL_QUADS);
// Tail Fin Front
      srelNormal3f(0.0,0.3162277,0.9486833);
      srelTexCoord2f(0.0,0.0);
      srelVertex3f(-1.5,15.0,-40.0);
      srelTexCoord2f(1.0,0.0);
      srelVertex3f(1.5,15.0,-40.0);
      srelTexCoord2f(0.0,1.0);
      srelVertex3f(0.5,25.0,-45.0);
      srelTexCoord2f(1.0,1.0);
      srelVertex3f(-0.5,25.0,-45.0);
// Tail Fin Back
      srelNormal3fv(norm_zzm);
      srelTexCoord2f(0.0,0.0);
      srelVertex3f(-0.5,25.0,-50.0);
      srelTexCoord2f(1.0,0.0);
      srelVertex3f(0.5,25.0,-50.0);
      srelTexCoord2f(0.0,1.0);
      srelVertex3f(1.5,15.0,-50.0);
      srelTexCoord2f(1.0,1.0);
      srelVertex3f(-1.5,15.0,-50.0);
   srelEnd();
// Tail Fin Top
   srelBegin(GL_QUAD_STRIP);
      srelNormal3fv(norm_zpz);
      srelTexCoord2f(0.0,0.0);
      srelVertex3f(-0.5,25.0,-45.0);
      srelTexCoord2f(1.0,0.0);
      srelVertex3f(-0.5,25.0,-50.0);
      srelTexCoord2f(0.0,1.0);
      srelVertex3f(0.5,25.0,-45.0);
      srelTexCoord2f(1.0,1.0);
      srelVertex3f(0.5,25.0,-50.0);
   srelEnd();
// Tail Fin Sides
   glCallList(logo_mat);
   srelBegin(GL_QUAD_STRIP);
      srelNormal3fv(norm_pzz);
      srelTexCoord2f(0.0,0.0);
      srelVertex3f(1.5,15.0,-50.0);
      srelTexCoord2f(1.0,0.0);
      srelVertex3f(1.5,15.0,-40.0);
      srelTexCoord2f(0.0,1.0);
      srelVertex3f(0.5,25.0,-50.0);
      srelTexCoord2f(1.0,1.0);
      srelVertex3f(0.5,25.0,-45.0);
   srelEnd();
   srelBegin(GL_QUAD_STRIP);
      srelNormal3fv(norm_mzz);
      srelTexCoord2f(0.0,0.0);
      srelVertex3f(-1.5,15.0,-50.0);
      srelTexCoord2f(1.0,0.0);
      srelVertex3f(-1.5,15.0,-40.0);
      srelTexCoord2f(0.0,1.0);
      srelVertex3f(-0.5,25.0,-50.0);
      srelTexCoord2f(1.0,1.0);
      srelVertex3f(-0.5,25.0,-45.0);
   srelEnd();
}

void eesrDrawLattice(void)
{
   GLfloat v[3]={-12.5,0.0,12.5};
   GLfloat h[3]={-12.5,0.0,12.5};
   GLfloat d[3]={-12.5,0.0,12.5};
   GLfloat a,b,c;
   int i,j;
   glCallList(lattice_mat);
   for(i=0;i<=GRID_SIZE;i++) {
      _DrawVRod(v[0],v[1],v[2]);
      _DrawHRod(h[0],h[1],h[2]);
      _DrawDRod(d[0],d[1],d[2]);
      a=v[0]+GRID_SPACING;
      b=h[1]+GRID_SPACING;
      c=d[0]+GRID_SPACING;
      for(j=1;j<=GRID_SIZE;j++) {
         _DrawVRod(a,v[1],v[2]);
         _DrawHRod(h[0],b,h[2]);
         _DrawDRod(c,d[1],d[2]);
         a+=GRID_SPACING;
         b+=GRID_SPACING;
         c+=GRID_SPACING;
      }
      v[2]-=GRID_SPACING;
      h[2]-=GRID_SPACING;
      d[1]+=GRID_SPACING;
   }
}

/**
* Internal function.
*/
void _DrawVRod(GLfloat x,GLfloat y,GLfloat z)
{
   const static GLfloat norm_pzp[3]={SQRT_1_2,0.0,SQRT_1_2};
   const static GLfloat norm_pzm[3]={SQRT_1_2,0.0,-SQRT_1_2};
   const static GLfloat norm_mzp[3]={-SQRT_1_2,0.0,SQRT_1_2};
   const static GLfloat norm_mzm[3]={-SQRT_1_2,0.0,-SQRT_1_2};

   srelBegin(GL_QUAD_STRIP);
      srelNormal3fv(norm_mzp);
      srelTexCoord2f(0.0,0.0);
      srelVertex3f(x-0.1f,y,z+0.1f);
      srelNormal3fv(norm_pzp);
      srelTexCoord2f(1.0,0.0);
      srelVertex3f(x+0.1f,y,z+0.1f);
      srelNormal3fv(norm_mzp);
      srelTexCoord2f(0.0,ROD_LENGTH);
      srelVertex3f(x-0.1f,y+ROD_LENGTH,z+0.1f);
      srelNormal3fv(norm_pzp);
      srelTexCoord2f(1.0,ROD_LENGTH);
      srelVertex3f(x+0.1f,y+ROD_LENGTH,z+0.1f);
   srelEnd();
   srelBegin(GL_QUAD_STRIP);
      srelNormal3fv(norm_pzp);
      srelTexCoord2f(0.0,0.0);
      srelVertex3f(x+0.1f,y,z+0.1f);
      srelNormal3fv(norm_pzm);
      srelTexCoord2f(1.0,0.0);
      srelVertex3f(x+0.1f,y,z-0.1f);
      srelNormal3fv(norm_pzp);
      srelTexCoord2f(0.0,ROD_LENGTH);
      srelVertex3f(x+0.1f,y+ROD_LENGTH,z+0.1f);
      srelNormal3fv(norm_pzm);
      srelTexCoord2f(1.0,ROD_LENGTH);
      srelVertex3f(x+0.1f,y+ROD_LENGTH,z-0.1f);
   srelEnd();
   srelBegin(GL_QUAD_STRIP);
      srelNormal3fv(norm_pzm);
      srelTexCoord2f(0.0,0.0);
      srelVertex3f(x+0.1f,y,z-0.1f);
      srelNormal3fv(norm_mzm);
      srelTexCoord2f(1.0,0.0);
      srelVertex3f(x-0.1f,y,z-0.1f);
      srelNormal3fv(norm_pzm);
      srelTexCoord2f(0.0,ROD_LENGTH);
      srelVertex3f(x+0.1f,y+ROD_LENGTH,z-0.1f);
      srelNormal3fv(norm_mzm);
      srelTexCoord2f(1.0,ROD_LENGTH);
      srelVertex3f(x-0.1f,y+ROD_LENGTH,z-0.1f);
   srelEnd();
   srelBegin(GL_QUAD_STRIP);
      srelNormal3fv(norm_mzm);
      srelTexCoord2f(0.0,0.0);
      srelVertex3f(x-0.1f,y,z-0.1f);
      srelNormal3fv(norm_mzp);
      srelTexCoord2f(1.0,0.0);
      srelVertex3f(x-0.1f,y,z+0.1f);
      srelNormal3fv(norm_mzm);
      srelTexCoord2f(0.0,ROD_LENGTH);
      srelVertex3f(x-0.1f,y+ROD_LENGTH,z-0.1f);
      srelNormal3fv(norm_mzp);
      srelTexCoord2f(1.0,ROD_LENGTH);
      srelVertex3f(x-0.1f,y+ROD_LENGTH,z+0.1f);
   srelEnd();
}

/**
* Internal function.
*/
void _DrawHRod(GLfloat x,GLfloat y,GLfloat z)
{
   const static GLfloat norm_zpp[3]={0.0,SQRT_1_2,SQRT_1_2};
   const static GLfloat norm_zpm[3]={0.0,SQRT_1_2,-SQRT_1_2};
   const static GLfloat norm_zmp[3]={0.0,-SQRT_1_2,SQRT_1_2};
   const static GLfloat norm_zmm[3]={0.0,-SQRT_1_2,-SQRT_1_2};

   srelBegin(GL_QUAD_STRIP);
      srelNormal3fv(norm_zmp);
      srelTexCoord2f(0.0,0.0);
      srelVertex3f(x,y-0.1f,z+0.1f);
      srelNormal3fv(norm_zpp);
      srelTexCoord2f(0.0,1.0);
      srelVertex3f(x,y+0.1f,z+0.1f);
      srelNormal3fv(norm_zmp);
      srelTexCoord2f(ROD_LENGTH,0.0);
      srelVertex3f(x+ROD_LENGTH,y-0.1f,z+0.1f);
      srelNormal3fv(norm_zpp);
      srelTexCoord2f(ROD_LENGTH,1.0);
      srelVertex3f(x+ROD_LENGTH,y+0.1f,z+0.1f);
   srelEnd();
   srelBegin(GL_QUAD_STRIP);
      srelNormal3fv(norm_zpp);
      srelTexCoord2f(0.0,0.0);
      srelVertex3f(x,y+0.1f,z+0.1f);
      srelNormal3fv(norm_zpm);
      srelTexCoord2f(0.0,1.0);
      srelVertex3f(x,y+0.1f,z-0.1f);
      srelNormal3fv(norm_zpp);
      srelTexCoord2f(ROD_LENGTH,0.0);
      srelVertex3f(x+ROD_LENGTH,y+0.1f,z+0.1f);
      srelNormal3fv(norm_zpm);
      srelTexCoord2f(ROD_LENGTH,1.0);
      srelVertex3f(x+ROD_LENGTH,y+0.1f,z-0.1f);
   srelEnd();
   srelBegin(GL_QUAD_STRIP);
      srelNormal3fv(norm_zpm);
      srelTexCoord2f(0.0,0.0);
      srelVertex3f(x,y+0.1f,z-0.1f);
      srelNormal3fv(norm_zmm);
      srelTexCoord2f(0.0,1.0);
      srelVertex3f(x,y-0.1f,z-0.1f);
      srelNormal3fv(norm_zpm);
      srelTexCoord2f(ROD_LENGTH,0.0);
      srelVertex3f(x+ROD_LENGTH,y+0.1f,z-0.1f);
      srelNormal3fv(norm_zmm);
      srelTexCoord2f(ROD_LENGTH,1.0);
      srelVertex3f(x+ROD_LENGTH,y-0.1f,z-0.1f);
   srelEnd();
   srelBegin(GL_QUAD_STRIP);
      srelNormal3fv(norm_zmm);
      srelTexCoord2f(0.0,0.0);
      srelVertex3f(x,y-0.1f,z-0.1f);
      srelNormal3fv(norm_zmp);
      srelTexCoord2f(0.0,1.0);
      srelVertex3f(x,y-0.1f,z+0.1f);
      srelNormal3fv(norm_zmm);
      srelTexCoord2f(ROD_LENGTH,0.0);
      srelVertex3f(x+ROD_LENGTH,y-0.1f,z-0.1f);
      srelNormal3fv(norm_zmp);
      srelTexCoord2f(ROD_LENGTH,1.0);
      srelVertex3f(x+ROD_LENGTH,y-0.1f,z+0.1f);
   srelEnd();
}

/**
* Internal function.
*/
void _DrawDRod(GLfloat x,GLfloat y,GLfloat z)
{
   const static GLfloat norm_ppz[3]={SQRT_1_2,SQRT_1_2,0.0};
   const static GLfloat norm_pmz[3]={SQRT_1_2,-SQRT_1_2,0.0};
   const static GLfloat norm_mpz[3]={-SQRT_1_2,SQRT_1_2,0.0};
   const static GLfloat norm_mmz[3]={-SQRT_1_2,-SQRT_1_2,0.0};

   srelBegin(GL_QUAD_STRIP);
      srelNormal3fv(norm_mpz);
      srelTexCoord2f(0.0,0.0);
      srelVertex3f(x-0.1f,y+0.1f,z);
      srelNormal3fv(norm_ppz);
      srelTexCoord2f(1.0,0.0);
      srelVertex3f(x+0.1f,y+0.1f,z);
      srelNormal3fv(norm_mpz);
      srelTexCoord2f(0.0,ROD_LENGTH);
      srelVertex3f(x-0.1f,y+0.1f,z-ROD_LENGTH);
      srelNormal3fv(norm_ppz);
      srelTexCoord2f(1.0,ROD_LENGTH);
      srelVertex3f(x+0.1f,y+0.1f,z-ROD_LENGTH);
   srelEnd();
   srelBegin(GL_QUAD_STRIP);
      srelNormal3fv(norm_ppz);
      srelTexCoord2f(0.0,0.0);
      srelVertex3f(x+0.1f,y+0.1f,z);
      srelNormal3fv(norm_pmz);
      srelTexCoord2f(1.0,0.0);
      srelVertex3f(x+0.1f,y-0.1f,z);
      srelNormal3fv(norm_ppz);
      srelTexCoord2f(0.0,ROD_LENGTH);
      srelVertex3f(x+0.1f,y+0.1f,z-ROD_LENGTH);
      srelNormal3fv(norm_pmz);
      srelTexCoord2f(1.0,ROD_LENGTH);
      srelVertex3f(x+0.1f,y-0.1f,z-ROD_LENGTH);
   srelEnd();
   srelBegin(GL_QUAD_STRIP);
      srelNormal3fv(norm_pmz);
      srelTexCoord2f(0.0,0.0);
      srelVertex3f(x+0.1f,y-0.1f,z);
      srelNormal3fv(norm_mmz);
      srelTexCoord2f(1.0,0.0);
      srelVertex3f(x-0.1f,y-0.1f,z);
      srelNormal3fv(norm_pmz);
      srelTexCoord2f(0.0,ROD_LENGTH);
      srelVertex3f(x+0.1f,y-0.1f,z-ROD_LENGTH);
      srelNormal3fv(norm_mmz);
      srelTexCoord2f(1.0,ROD_LENGTH);
      srelVertex3f(x-0.1f,y-0.1f,z-ROD_LENGTH);
   srelEnd();
   srelBegin(GL_QUAD_STRIP);
      srelNormal3fv(norm_mmz);
      srelTexCoord2f(0.0,0.0);
      srelVertex3f(x-0.1f,y-0.1f,z);
      srelNormal3fv(norm_mpz);
      srelTexCoord2f(1.0,0.0);
      srelVertex3f(x-0.1f,y+0.1f,z);
      srelNormal3fv(norm_mmz);
      srelTexCoord2f(0.0,ROD_LENGTH);
      srelVertex3f(x-0.1f,y-0.1f,z-ROD_LENGTH);
      srelNormal3fv(norm_mpz);
      srelTexCoord2f(1.0,ROD_LENGTH);
      srelVertex3f(x-0.1f,y+0.1f,z-ROD_LENGTH);
   srelEnd();
}


void eesrDrawSphere(void)
{
   static const GLfloat vdata[12][3]={ {-SPHERE_X,0.0,SPHERE_Z}, {SPHERE_X,0.0,SPHERE_Z}, {-SPHERE_X,0.0,-SPHERE_Z}, {SPHERE_X,0.0,-SPHERE_Z},
                                       {0.0,SPHERE_Z,SPHERE_X}, {0.0,SPHERE_Z,-SPHERE_X}, {0.0,-SPHERE_Z,SPHERE_X}, {0.0,-SPHERE_Z,-SPHERE_X},
                                       {SPHERE_Z,SPHERE_X,0.0}, {-SPHERE_Z,SPHERE_X,0.0}, {SPHERE_Z,-SPHERE_X,0.0}, {-SPHERE_Z,-SPHERE_X,0.0} };
   static const GLint tindices[20][3]={ {0, 4, 1}, {0,9, 4}, {9, 5,4}, { 4,5,8}, {4,8, 1},
                                        {8,10, 1}, {8,3,10}, {5, 3,8}, { 5,2,3}, {2,7, 3},
                                        {7,10, 3}, {7,6,10}, {7,11,6}, {11,0,6}, {0,1, 6},
                                        {6, 1,10}, {9,0,11}, {9,11,2}, { 9,2,5}, {7,2,11} };
   int i,depth=3;
   glCallList(sphere_mat);
   srelBegin(GL_TRIANGLES);
   for(i=0;i<20;i++)
      _SubdivideSphere(&vdata[tindices[i][0]][0],&vdata[tindices[i][1]][0],&vdata[tindices[i][2]][0],depth);
   srelEnd();
}

/**
* Internal function.
*/
void _SubdivideSphere(const GLfloat *v1,const GLfloat *v2,const GLfloat *v3,const int depth)
{
   GLfloat v12[3],v23[3],v31[3];
   if(depth==0) {
      _DrawTriangle(v1,v2,v3);
   } else {
      v12[0]=v1[0]+v2[0];v12[1]=v1[1]+v2[1];v12[2]=v1[2]+v2[2];
      v23[0]=v2[0]+v3[0];v23[1]=v2[1]+v3[1];v23[2]=v2[2]+v3[2];
      v31[0]=v3[0]+v1[0];v31[1]=v3[1]+v1[1];v31[2]=v3[2]+v1[2];
      _ScaleSphere(v12);
      _ScaleSphere(v23);
      _ScaleSphere(v31);
      _SubdivideSphere(v1,v12,v31,depth-1);
      _SubdivideSphere(v2,v23,v12,depth-1);
      _SubdivideSphere(v3,v31,v23,depth-1);
      _SubdivideSphere(v12,v23,v31,depth-1);
   }
}

/**
* Internal function.
*/
void _ScaleSphere(GLfloat *v)
{
   const GLfloat d=sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
   v[0]*=SPHERE_RADIUS/d;
   v[1]*=SPHERE_RADIUS/d;
   v[2]*=SPHERE_RADIUS/d;
}

/**
* Internal function.
*/
void _DrawTriangle(const GLfloat *v1,const GLfloat *v2,const GLfloat *v3)
{
   GLfloat n1[3]={v1[0]/SPHERE_RADIUS, v1[1]/SPHERE_RADIUS, v1[2]/SPHERE_RADIUS};
   GLfloat n2[3]={v2[0]/SPHERE_RADIUS, v2[1]/SPHERE_RADIUS, v2[2]/SPHERE_RADIUS};
   GLfloat n3[3]={v3[0]/SPHERE_RADIUS, v3[1]/SPHERE_RADIUS, v3[2]/SPHERE_RADIUS};
   srelNormal3fv(n1);srelVertex3fv(v1);
   srelNormal3fv(n2);srelVertex3fv(v2);
   srelNormal3fv(n3);srelVertex3fv(v3);
}
