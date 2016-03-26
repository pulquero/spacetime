/**
*   SREL OpenGL API
*   Copyright (C) 2000-2001  Mark Hale (mark.hale@physics.org)
*
*   This library is free software; you can redistribute it and/or
*   modify it under the terms of the GNU Lesser General Public
*   License as published by the Free Software Foundation; either
*   version 2.1 of the License, or (at your option) any later version.
*
*   This library is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
*   Lesser General Public License for more details.
*
*   You should have received a copy of the GNU Lesser General Public
*   License along with this library; if not, write to the Free Software
*   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/



#ifdef __IBMC__
   #define LINKAGE _Optlink
#else
   #define LINKAGE
#endif


/* function prototypes */
extern void LINKAGE srelDetail(const GLfloat detail);
extern GLenum LINKAGE srelGetError(void);
extern void LINKAGE srelDoppler(const GLboolean flag);
extern void LINKAGE srelVelocityf(const GLfloat ux,const GLfloat uy,const GLfloat uz);
extern void LINKAGE srelVelocityfv(const GLfloat *u);
extern void LINKAGE srelVelocityFunc(void (* LINKAGE ufn)(GLfloat *u,GLfloat t));
extern void LINKAGE srelVelocityTransform(GLfloat *v);
extern void LINKAGE srelLocalTime(const GLfloat t);

extern GLfloat LINKAGE srelVertexTime3f(GLfloat x,GLfloat y,GLfloat z);
extern GLfloat LINKAGE srelVertexTime3fv(const GLfloat *v);
extern GLfloat LINKAGE srelVertexTime3d(GLdouble x,GLdouble y,GLdouble z);
extern GLfloat LINKAGE srelVertexTime3dv(const GLdouble *v);

extern void LINKAGE srelBegin(GLenum mode);
extern void LINKAGE srelEnd(void);

extern void LINKAGE srelVertex2f(GLfloat x,GLfloat y);
extern void LINKAGE srelVertex2fv(const GLfloat *v);
extern void LINKAGE srelVertex2d(GLdouble x,GLdouble y);
extern void LINKAGE srelVertex2dv(const GLdouble *v);
extern void LINKAGE srelVertex3f(GLfloat x,GLfloat y,GLfloat z);
extern void LINKAGE srelVertex3fv(const GLfloat *v);
extern void LINKAGE srelVertex3d(GLdouble x,GLdouble y,GLdouble z);
extern void LINKAGE srelVertex3dv(const GLdouble *v);
extern void LINKAGE srelVertex4f(GLfloat x,GLfloat y,GLfloat z,GLfloat w);
extern void LINKAGE srelVertex4fv(const GLfloat *v);
extern void LINKAGE srelVertex4d(GLdouble x,GLdouble y,GLdouble z,GLdouble w);
extern void LINKAGE srelVertex4dv(const GLdouble *v);

extern void LINKAGE srelNormal3f(GLfloat x,GLfloat y,GLfloat z);
extern void LINKAGE srelNormal3fv(const GLfloat *n);
extern void LINKAGE srelNormal3d(GLdouble x,GLdouble y,GLdouble z);
extern void LINKAGE srelNormal3dv(const GLdouble *n);

extern void LINKAGE srelTexCoord1f(GLfloat s);
extern void LINKAGE srelTexCoord1fv(const GLfloat *v);
extern void LINKAGE srelTexCoord1d(GLdouble s);
extern void LINKAGE srelTexCoord1dv(const GLdouble *v);
extern void LINKAGE srelTexCoord2f(GLfloat s,GLfloat t);
extern void LINKAGE srelTexCoord2fv(const GLfloat *v);
extern void LINKAGE srelTexCoord2d(GLdouble s,GLdouble t);
extern void LINKAGE srelTexCoord2dv(const GLdouble *v);

extern void LINKAGE srelColor3f(GLfloat red,GLfloat green,GLfloat blue);
extern void LINKAGE srelColor3fv(const GLfloat *v);
extern void LINKAGE srelColor4f(GLfloat red,GLfloat green,GLfloat blue,GLfloat alpha);
extern void LINKAGE srelColor4fv(const GLfloat *v);

extern void LINKAGE srelMaterialfv(GLenum face,GLenum pname,const GLfloat *rgba);

#ifdef GL_VERSION_1_1
extern void LINKAGE srelDrawArrays(GLenum mode,GLint first,GLsizei count);
extern void LINKAGE srelDrawElements(GLenum mode,GLsizei count,GLenum type,const GLvoid *indices);
#endif
