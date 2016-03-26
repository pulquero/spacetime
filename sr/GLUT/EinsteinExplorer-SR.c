/**
*   Einstein Explorer - SR (GLUT version)
*   Copyright (C) 2000  Mark Hale (mark.hale@physics.org)
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



#if defined(unix)
   #include <GL/glut.h>
#else
   #if defined(WIN32)
      #include <windows.h>
      #if defined(__MINGW32__)
         #define GLUT_DISABLE_ATEXIT_HACK
      #endif
   #endif
   #include <gl\glut.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#ifdef HAVE_LIB3DS
#include <lib3ds/file.h>
#include <lib3ds/mesh.h>
#include <lib3ds/node.h>
#include <lib3ds/matrix.h>
#include <lib3ds/material.h>
#endif
#ifdef HAVE_GLUTCAVE
#include <glutcave.h>
#endif
#include "../srel/srel.h"
#include "../eesr/eesr.h"
#include "EinsteinExplorer-SR.h"


#define FALSE 0
#define TRUE  1

#define X_AXIS 0
#define Y_AXIS 1
#define Z_AXIS 2

#define TRANSLATE_DELTA 2.0
#define ROTATE_DELTA    5.0
#define AXIS_DELTA      0.1

#define MOUSE_SCALE     0.2

#ifdef HAVE_LIB3DS
static Lib3dsFile *file=NULL;
#endif
#ifdef HAVE_GLUTCAVE
GLUTCAVE_DATA;
#endif

#define EYESEP 0.1
static int stereo=FALSE;

/* keyboard rotation */
static int axis=Y_AXIS;
static GLfloat angle=0.0;
static GLfloat rotAxis[3]={0.0,1.0,0.0};
/* mouse rotation */
static GLfloat xAngle,yAngle;
/* translation */
static GLfloat pos[3]={0.0,-12.5,0.0};

/* menus/mouse movement */
static int menusActive=TRUE;
/* left mouse button menu IDs */
static int controlMID,helpMID;
/* right mouse button menu IDs */
static int optionMID,frameMID,objectMID,graphicsMID;

int main(int argc,char **argv)
{
   char *title="Einstein Explorer - SR v1.36 (Mark Hale)";
   int width=400, height=400;
#ifdef HAVE_LIB3DS
   if(argc==2) {
      file=lib3ds_file_load(argv[1]);
      eesrSetObject(draw3DS);
   }
#endif
/*
   printf("Special Relativity CAVE(tm) demo. By Bruno Barberi Gnecco and Mark Hale.\n");
   printf("Arguments are:\n"
	  "\t-wX\tWindow width. Default: 400.\n"
	  "\t-hX\tWindow height. Default: 400.\n"
	  "\t-s\tStereo mode.\n"
	  "\t-nX\tNumber of screens. Default: 5.\n"
	  "Anything else show this help.\n");

   while ( argc > 1 ) {
      switch ( argv[argc-1][1] ) {
      	 case 'h':
	    height = atoi(&argv[argc-1][2]);
	    break;
      	 case 'w':
	    width = atoi(&argv[argc-1][2]);
	    break;
      	 case 'n':
	    screens = atoi(&argv[argc-1][2]);
	    break;
      	 case 's':
	    stereo = TRUE;
	    break;
	 default:
	    exit(0);
	    break;
      }	  
      argc--;
   }
   GLUTCAVE_SET_SCREENS(screens);
*/
   glutInitWindowSize(width, height);
   glutInit(&argc, argv);
   if(stereo)
      glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH | GLUT_STEREO);
   else
      glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

#ifdef HAVE_GLUTCAVE
   GLUTCAVE_CREATE_SUBWINDOWS_BEGIN(title, width, height);
#else
   glutCreateWindow(title);
#endif
   eesrInit();
   glutReshapeFunc(Reshape);
   glutDisplayFunc(Display);
   glutKeyboardFunc(Key);
   glutSpecialFunc(Special);
   glutMouseFunc(Mouse);
   glutMotionFunc(Motion);
   /* help menu */
   helpMID=glutCreateMenu(HelpMenu);
   glutAddMenuEntry("Arrow keys  :  left/right/forward/backward movement",1);
   glutAddMenuEntry("Page up/down  :  up/down movement",2);
   glutAddMenuEntry("<,>  :  rotate about the rotation axis",3);
   glutAddMenuEntry("1,2,3  :  select x/y/z axis component",4);
   glutAddMenuEntry("Home,End  :  increase/decrease current axis component",5);
   glutAddMenuEntry("+,-  :  increase/decrease speed",6);
   glutAddMenuEntry("t  :  increase time",7);
   glutAddMenuEntry("m  :  toggle menus/mouse movement",8);
   /* control menu */
   controlMID=glutCreateMenu(ControlMenu);
   glutAddMenuEntry("(right click for options)",1);
   glutAddMenuEntry("Pos:   +0.0,-12.5,+0.0",2);
   glutAddMenuEntry("Angle: +0.0",3);
   glutAddMenuEntry("Axis:  +0.0,+1.0,+0.0",4);
   glutAddMenuEntry("Speed: +0.00",5);
   glutAddSubMenu("Help",helpMID);
   glutAttachMenu(GLUT_LEFT_BUTTON);
   /* frame menu */
   frameMID=glutCreateMenu(FrameMenu);
   glutAddMenuEntry("World",1);
   glutAddMenuEntry("Object",2);
   /* object menu */
   objectMID=glutCreateMenu(ObjectMenu);
   glutAddMenuEntry("Lattice",1);
   glutAddMenuEntry("Car",2);
   glutAddMenuEntry("Aeroplane",3);
   glutAddMenuEntry("Sphere",4);
   glutAddMenuEntry("Rotating wheel",5);
   /* graphics menu */
   graphicsMID=glutCreateMenu(GraphicsMenu);
   glutAddMenuEntry("Wireframe on/off",1);
   glutAddMenuEntry("Texture on/off",2);
   glutAddMenuEntry("Fog on/off",3);
   /* options menu */
   optionMID=glutCreateMenu(OptionMenu);
   glutAddMenuEntry("(left click for controls)",1);
   glutAddSubMenu("Frame",frameMID);
   glutAddSubMenu("Object",objectMID);
   glutAddMenuEntry("Doppler on/off",2);
   glutAddSubMenu("Graphics",graphicsMID);
   glutAddMenuEntry("Exit",3);
   glutAttachMenu(GLUT_RIGHT_BUTTON);
#ifdef HAVE_GLUTCAVE
   GLUTCAVE_CREATE_SUBWINDOWS_END();
#endif
   glutMainLoop();
   return 0;
}

void GLUTCALLBACK Reshape(int w,int h)
{
#ifdef HAVE_GLUTCAVE
   glViewport(0,0,w,h);
   glMatrixMode(GL_PROJECTION);
   GLUTCAVE_RESHAPE(w, h, 1.0, 1000.0);
   glMatrixMode(GL_MODELVIEW);
#else
   eesrResize(w,h);
#endif
}

void GLUTCALLBACK Display(void)
{
   static const GLfloat light_pos[4]={0.0,0.0,0.0,1.0};

   if(stereo) {
      /* left buffer */
      glDrawBuffer(GL_BACK_LEFT);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glLoadIdentity();
      glTranslatef(EYESEP/2,0.0,0.0);
      /* viewpoint light */
      glLightfv(GL_LIGHT0,GL_POSITION,light_pos);
      /* viewing transformations */
#ifdef HAVE_GLUTCAVE
      GLUTCAVE_ROTATE();
#endif
      if(menusActive) {
         glRotatef(angle,rotAxis[0],rotAxis[1],rotAxis[2]);
      } else {
         glRotatef(xAngle,1.0,0.0,0.0);
         glRotatef(yAngle,0.0,1.0,0.0);
      }
      glTranslatef(pos[0],pos[1],pos[2]);
      eesrDrawGeometry();

      /* right buffer */
      glDrawBuffer(GL_BACK_RIGHT);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glLoadIdentity();
      glTranslatef(-EYESEP/2,0.0,0.0);
      /* viewpoint light */
      glLightfv(GL_LIGHT0,GL_POSITION,light_pos);
      /* viewing transformations */
#ifdef HAVE_GLUTCAVE
      GLUTCAVE_ROTATE();
#endif
      if(menusActive) {
         glRotatef(angle,rotAxis[0],rotAxis[1],rotAxis[2]);
      } else {
         glRotatef(xAngle,1.0,0.0,0.0);
         glRotatef(yAngle,0.0,1.0,0.0);
      }
      glTranslatef(pos[0],pos[1],pos[2]);
      eesrDrawGeometry();
   } else {
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glLoadIdentity();
      /* viewpoint light */
      glLightfv(GL_LIGHT0,GL_POSITION,light_pos);
      /* viewing transformations */
#ifdef HAVE_GLUTCAVE
      GLUTCAVE_ROTATE();
#endif
      if(menusActive) {
         glRotatef(angle,rotAxis[0],rotAxis[1],rotAxis[2]);
      } else {
         glRotatef(xAngle,1.0,0.0,0.0);
         glRotatef(yAngle,0.0,1.0,0.0);
      }
      glTranslatef(pos[0],pos[1],pos[2]);
      eesrDrawGeometry();
   }
   glutSwapBuffers();
}

void GLUTCALLBACK Key(unsigned char key,int x,int y)
{
   char buf[30];
   glutSetMenu(controlMID);
   switch(key) {
      case '1':
         axis=X_AXIS;
         break;
      case '2':
         axis=Y_AXIS;
         break;
      case '3':
         axis=Z_AXIS;
         break;
      case '<':
      case ',':
         angle-=ROTATE_DELTA;
         sprintf(buf,"Angle: %+.1f",angle);
         glutChangeToMenuEntry(3,buf,3);
         break;
      case '>':
      case '.':
         angle+=ROTATE_DELTA;
         sprintf(buf,"Angle: %+.1f",angle);
         glutChangeToMenuEntry(3,buf,3);
         break;
      case '+':
      case '=':
         if(eesrSpeed<0.9)
            eesrSpeed+=0.08;
         sprintf(buf,"Speed: %+.2f",eesrSpeed);
         glutChangeToMenuEntry(5,buf,5);
         break;
      case '_':
      case '-':
         if(eesrSpeed>-0.9)
            eesrSpeed-=0.08;
         sprintf(buf,"Speed: %+.2f",eesrSpeed);
         glutChangeToMenuEntry(5,buf,5);
         break;
      case 'T':
         eesrSetTime(eesrGetTime()-1.0);
         break;
      case 't':
         eesrSetTime(eesrGetTime()+1.0);
         break;
      case 'm':
         if(menusActive) {
            glutDetachMenu(GLUT_LEFT_BUTTON);
            glutDetachMenu(GLUT_RIGHT_BUTTON);
            menusActive=FALSE;
         } else {
            glutSetMenu(controlMID);
            glutAttachMenu(GLUT_LEFT_BUTTON);
            glutSetMenu(optionMID);
            glutAttachMenu(GLUT_RIGHT_BUTTON);
            menusActive=TRUE;
         }
         break;
      case 27: /* escape */
      case 'q':
      case 'Q':
	 exit(0);
   }
#ifdef HAVE_GLUTCAVE
   GLUTCAVE_REDISPLAY();
#else
   glutPostRedisplay();
#endif
}

void GLUTCALLBACK Special(int key,int x,int y)
{
   char buf[40];
   glutSetMenu(controlMID);
   switch(key) {
      case GLUT_KEY_LEFT:
         pos[X_AXIS]+=TRANSLATE_DELTA;
         sprintf(buf,"Pos:   %+.1f,%+.1f,%+.1f",pos[0],pos[1],pos[2]);
         glutChangeToMenuEntry(2,buf,2);
         break;
      case GLUT_KEY_RIGHT:
         pos[X_AXIS]-=TRANSLATE_DELTA;
         sprintf(buf,"Pos:   %+.1f,%+.1f,%+.1f",pos[0],pos[1],pos[2]);
         glutChangeToMenuEntry(2,buf,2);
         break;
      case GLUT_KEY_UP:
         pos[Z_AXIS]+=TRANSLATE_DELTA;
         sprintf(buf,"Pos:   %+.1f,%+.1f,%+.1f",pos[0],pos[1],pos[2]);
         glutChangeToMenuEntry(2,buf,2);
         break;
      case GLUT_KEY_DOWN:
         pos[Z_AXIS]-=TRANSLATE_DELTA;
         sprintf(buf,"Pos:   %+.1f,%+.1f,%+.1f",pos[0],pos[1],pos[2]);
         glutChangeToMenuEntry(2,buf,2);
         break;
      case GLUT_KEY_PAGE_UP:
         pos[Y_AXIS]-=TRANSLATE_DELTA;
         sprintf(buf,"Pos:   %+.1f,%+.1f,%+.1f",pos[0],pos[1],pos[2]);
         glutChangeToMenuEntry(2,buf,2);
         break;
      case GLUT_KEY_PAGE_DOWN:
         pos[Y_AXIS]+=TRANSLATE_DELTA;
         sprintf(buf,"Pos:   %+.1f,%+.1f,%+.1f",pos[0],pos[1],pos[2]);
         glutChangeToMenuEntry(2,buf,2);
         break;
      case GLUT_KEY_HOME:
         rotAxis[axis]+=AXIS_DELTA;
         sprintf(buf,"Axis:  %+.1f,%+.1f,%+.1f",rotAxis[0],rotAxis[1],rotAxis[2]);
         glutChangeToMenuEntry(4,buf,4);
         break;
      case GLUT_KEY_END:
         rotAxis[axis]-=AXIS_DELTA;
         sprintf(buf,"Axis:  %+.1f,%+.1f,%+.1f",rotAxis[0],rotAxis[1],rotAxis[2]);
         glutChangeToMenuEntry(4,buf,4);
         break;
   }
#ifdef HAVE_GLUTCAVE
   GLUTCAVE_REDISPLAY();
#else
   glutPostRedisplay();
#endif
}

static int buttonDown;
static int beginX,beginY;

void GLUTCALLBACK Mouse(int button,int state,int x,int y)
{
   if(state==GLUT_DOWN) {
      buttonDown=button;
      beginX=x;
      beginY=y;
   }
}

void GLUTCALLBACK Motion(int x,int y)
{
   switch(buttonDown) {
      case GLUT_LEFT_BUTTON:
         pos[X_AXIS]-=(x-beginX)*MOUSE_SCALE;
         pos[Z_AXIS]-=(y-beginY)*MOUSE_SCALE;
         break;
      case GLUT_RIGHT_BUTTON:
         yAngle+=(x-beginX)*MOUSE_SCALE;
         xAngle+=(y-beginY)*MOUSE_SCALE;
         {
            char buf[40];
            sprintf(buf,"Angle: %+.1f",yAngle);
            glutChangeToMenuEntry(3,buf,3);
         }
         break;
   }
   beginX=x;
   beginY=y;
#ifdef HAVE_GLUTCAVE
   GLUTCAVE_REDISPLAY();
#else
   glutPostRedisplay();
#endif
}

void GLUTCALLBACK ControlMenu(int value)
{
}

void GLUTCALLBACK HelpMenu(int value)
{
}

void GLUTCALLBACK OptionMenu(int value)
{
   static GLboolean doppler=GL_TRUE;
   switch(value) {
      case 2:
         doppler=!doppler;
         srelDoppler(doppler);
#ifdef HAVE_GLUTCAVE
         GLUTCAVE_REDISPLAY();
#else
         glutPostRedisplay();
#endif
         break;
      case 3:
#ifdef HAVE_LIB3DS
         if(file)
            lib3ds_file_free(file);
#endif
         exit(0);
         break;
   }
}

void GLUTCALLBACK FrameMenu(int value)
{
   switch(value) {
      case 1:
         eesrSetRestFrame(eesrDrawWorld);
         eesrSetMovingFrame(eesrDrawObject);
         break;
      case 2:
         eesrSetRestFrame(eesrDrawObject);
         eesrSetMovingFrame(eesrDrawWorld);
         break;
   }
#ifdef HAVE_GLUTCAVE
   GLUTCAVE_REDISPLAY();
#else
   glutPostRedisplay();
#endif
}

void GLUTCALLBACK ObjectMenu(int value)
{
   switch(value) {
      case 1:
         eesrSetObject(eesrDrawLattice);
         break;
      case 2:
         eesrSetObject(eesrDrawCar);
         break;
      case 3:
         eesrSetObject(eesrDrawAeroplane);
         break;
      case 4:
         eesrSetObject(eesrDrawSphere);
         break;
      case 5:
         eesrSetObject(drawRotatingWheel);
         break;
   }
#ifdef HAVE_GLUTCAVE
   GLUTCAVE_REDISPLAY();
#else
   glutPostRedisplay();
#endif
}

void GLUTCALLBACK GraphicsMenu(int value)
{
   static GLboolean wireframe=GL_FALSE;
   switch(value) {
      case 1:
         if(wireframe) {
            glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
         } else {
            glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
         }
         wireframe=!wireframe;
         break;
      case 2:
         if(glIsEnabled(GL_TEXTURE_2D)) {
            glDisable(GL_TEXTURE_2D);
         } else {
            glEnable(GL_TEXTURE_2D);
         }
         break;
      case 3:
         if(glIsEnabled(GL_FOG)) {
            glDisable(GL_FOG);
         } else {
            glEnable(GL_FOG);
         }
         break;
   }
#ifdef HAVE_GLUTCAVE
   GLUTCAVE_REDISPLAY();
#else
   glutPostRedisplay();
#endif
}

#define TWO_PI 6.28318530717958647692
#define RADIUS 10.0
#define DELTA_ANGLE TWO_PI/20
#define DELTA_RADIUS RADIUS/15

float wheelAngle=0.0;
float wheelRadius=0.0;
void LINKAGE rotateVertex(GLfloat x,GLfloat y,GLfloat z);
void LINKAGE rimU(GLfloat *u,GLfloat t);
void LINKAGE spokesU(GLfloat *u,GLfloat t);

void drawRotatingWheel(void)
{
   const GLfloat amb_diff[4]={0.2,0.6,0.7,1.0};
   const GLfloat amb_diff2[4]={0.8,0.4,0.5,1.0};
   float cosAng,sinAng;
   srelDetail(0.0);   // no subdivision - we'll do it ourselves via DELTA_ANGLE
   glPushMatrix();
   glTranslatef(0.0,RADIUS,-10.0-RADIUS);
   glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,amb_diff);
   srelVelocityFunc(rimU);
   wheelAngle=0.0;
   srelBegin(GL_QUADS);
   srelNormal3f(0.0,0.0,1.0);
   rotateVertex(1.0,RADIUS-1.0,0.0);
   rotateVertex(-1.0,RADIUS-1.0,0.0);
   rotateVertex(-1.0,RADIUS+1.0,0.0);
   rotateVertex(1.0,RADIUS+1.0,0.0);
   srelEnd();
   // Rim
   srelBegin(GL_QUAD_STRIP);
   srelNormal3f(0.0,1.0,0.0);
   srelVertex3f(1.0,RADIUS,0.0);
   srelVertex3f(-1.0,RADIUS,0.0);
   for(wheelAngle=DELTA_ANGLE;wheelAngle<TWO_PI;wheelAngle+=DELTA_ANGLE) {
      cosAng=cos(wheelAngle);
      sinAng=sin(wheelAngle);
      srelNormal3f(0.0,cosAng,sinAng);
      srelVertex3f(1.0,RADIUS*cosAng,RADIUS*sinAng);
      srelVertex3f(-1.0,RADIUS*cosAng,RADIUS*sinAng);
   }
   wheelAngle=TWO_PI;
   glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,amb_diff2);
   srelNormal3f(0.0,1.0,0.0);
   srelVertex3f(1.0,RADIUS,0.0);
   srelVertex3f(-1.0,RADIUS,0.0);
   srelEnd();
   // Spokes
   glMaterialfv(GL_FRONT,GL_AMBIENT_AND_DIFFUSE,amb_diff);
   srelVelocityFunc(spokesU);
   for(wheelAngle=DELTA_ANGLE;wheelAngle<TWO_PI;wheelAngle+=DELTA_ANGLE) {
      srelBegin(GL_QUAD_STRIP);
      cosAng=cos(wheelAngle);
      sinAng=sin(wheelAngle);
      for(wheelRadius=0.0;wheelRadius<=RADIUS;wheelRadius+=DELTA_RADIUS) {
         srelNormal3f(0.0,-sinAng,cosAng);
         srelVertex3f(1.0,wheelRadius*cosAng,wheelRadius*sinAng);
         srelVertex3f(-1.0,wheelRadius*cosAng,wheelRadius*sinAng);
      }
      srelEnd();
   }
   glPopMatrix();
}
void rotateVertex(GLfloat x,GLfloat y,GLfloat z)
{
   const GLfloat t=eesrGetTime();
   const GLfloat w=eesrSpeed/RADIUS;
   GLfloat v[3];
   // translate to origin
   y-=RADIUS*cos(wheelAngle);
   z-=RADIUS*sin(wheelAngle);
   // rotate frame
   v[0]=x;
   v[1]=cos(w*t)*y-sin(w*t)*z;
   v[2]=sin(w*t)*y+cos(w*t)*z;
   // translate back
   v[1]+=RADIUS*cos(wheelAngle);
   v[2]+=RADIUS*sin(wheelAngle);
   srelVertex3fv(v);
}
void rimU(GLfloat *u,GLfloat t)
{
   GLfloat w=eesrSpeed/RADIUS;
   u[0]=0.0;
   u[1]=-eesrSpeed*sin(w*t+wheelAngle);
   u[2]=eesrSpeed*cos(w*t+wheelAngle);
}
void spokesU(GLfloat *u,GLfloat t)
{
   GLfloat w=eesrSpeed/RADIUS;
   u[0]=0.0;
   u[1]=-w*wheelRadius*sin(w*t+wheelAngle);
   u[2]=w*wheelRadius*cos(w*t+wheelAngle);
}

#ifdef HAVE_LIB3DS

void draw3DS(void)
{
   Lib3dsNode *n;
   glPushMatrix();
   glTranslatef(0.0,5.0,-50.0);
   for(n=file->nodes;n!=NULL;n=n->next)
      draw3DSNode(n);
   glPopMatrix();
}

void draw3DSNode(Lib3dsNode *node)
{
   Lib3dsNode *n;
   for(n=node->childs;n!=NULL;n=n->next)
      draw3DSNode(n);
   if(node->type==LIB3DS_OBJECT_NODE) {
      int i;
      Lib3dsMesh *mesh=NULL;
      Lib3dsVector *normals=NULL;
      Lib3dsMatrix M;
      if(strcmp(node->name,"$$$DUMMY")==0)
         return;
      mesh=lib3ds_file_mesh_by_name(file,node->name);
      if(!mesh)
         return;
      /* apply mesh transformation */
      lib3ds_matrix_copy(M,mesh->matrix);
      lib3ds_matrix_inv(M);
      glMultMatrixf(&M[0][0]);
      /* calculate normals */
      normals=malloc(3*sizeof(Lib3dsVector)*mesh->faces);
      lib3ds_mesh_calculate_normals(mesh,normals);
      for(i=0;i<mesh->faces;i++) {
         Lib3dsFace *face=&mesh->faceL[i];
         Lib3dsMaterial *mat=NULL;
         /* materials */
         if(face->material[0])
            mat=lib3ds_file_material_by_name(file,face->material);
         if(mat) {
            float shine;
            glMaterialfv(GL_FRONT,GL_AMBIENT,mat->ambient);
            glMaterialfv(GL_FRONT,GL_DIFFUSE,mat->diffuse);
            glMaterialfv(GL_FRONT,GL_SPECULAR,mat->specular);
            shine=pow(2.0,10.0*mat->shininess);
            if(shine>128.0)
               shine=128.0;
            glMaterialf(GL_FRONT,GL_SHININESS,shine);
         } else {
            static GLfloat amb[4]={0.2,0.2,0.2,1.0};
            static GLfloat diff[4]={0.8,0.8,0.8,1.0};
            static GLfloat spec[4]={0.0,0.0,0.0,1.0};
            glMaterialfv(GL_FRONT,GL_AMBIENT,amb);
            glMaterialfv(GL_FRONT,GL_DIFFUSE,diff);
            glMaterialfv(GL_FRONT,GL_SPECULAR,spec);
         }
         /* vertices */
         srelBegin(GL_TRIANGLES);
         srelNormal3fv(normals[3*i]);
         srelVertex3fv(mesh->pointL[face->points[0]].pos);
         srelNormal3fv(normals[3*i+1]);
         srelVertex3fv(mesh->pointL[face->points[1]].pos);
         srelNormal3fv(normals[3*i+2]);
         srelVertex3fv(mesh->pointL[face->points[2]].pos);
         srelEnd();
      }
      free(normals);
   }
}

#endif

