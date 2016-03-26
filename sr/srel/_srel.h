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



#ifndef __IBMC__
   #define _Inline inline
#endif


/* constants */
#define SREL_NO_SUBDIVIDE -1

#define GAUSSIAN_ZEROS_0 -0.861136311594052575223946488893
#define GAUSSIAN_ZEROS_1 -0.339981043584856264802665759103
#define GAUSSIAN_ZEROS_2  0.339981043584856264802665759103
#define GAUSSIAN_ZEROS_3  0.861136311594052575223946488893
#define GAUSSIAN_COEFFS_0 0.347854845137453857373063949222
#define GAUSSIAN_COEFFS_1 0.652145154862546142626936050778
#define GAUSSIAN_COEFFS_2 0.652145154862546142626936050778
#define GAUSSIAN_COEFFS_3 0.347854845137453857373063949222

/* definitions */
typedef GLfloat _VELOCITY[3];
typedef void (* LINKAGE _VELOCITYFUNC)(GLfloat *u,GLfloat t);
typedef GLfloat _VERTEXCOORD[4];
typedef GLfloat _NORMALCOORD[3];
typedef GLfloat _TEXCOORD[2];
typedef GLfloat _RGBA[4];

/* srel state machine */
typedef struct srelMachine {
   GLfloat detail;            // Detail level (controls subdivision)
   GLenum error;              // Current error flag
   GLboolean doppler;         // Doppler on/off
   GLboolean begin;           // Executing srelBegin/srelEnd sequence
   GLfloat model[16];         // Modelview matrix
   GLfloat t;                 // Current time
   _VELOCITY u;               // Current (constant) velocity
   _VELOCITYFUNC uFunc;       // Current velocity function
// vertices
   struct vertex {
      GLenum mode;
      GLboolean isFirst;
      int num;
      _VERTEXCOORD cur[4];    // Read-ahead buffer for vertices
      _VERTEXCOORD curST[4];  // Read-ahead buffer for space transformed vertices
   } vertex;
// textures
   struct tex {
      GLboolean haveCoord;
      _TEXCOORD cur[4];       // Read-ahead buffer for texture coordinates
   } tex;
// materials
   struct mat {
      GLint colorMode;        // ColorMaterial mode
      GLenum face;            // Current face
      _RGBA spec;             // Current specular
      _RGBA amb;              // Current ambient
      _RGBA diff;             // Current diffuse
   } mat;
// cache
   struct cache {
      GLfloat uu;             // u*u
      GLfloat speed;          // sqrt(u*u)
      GLfloat oneMuu;         // 1-u*u
      GLfloat invGamma;       // sqrt(1-u*u)
   } cache;
} _SRELMACHINE;


/* function prototypes */
_Inline void LINKAGE _NormalTransform(const _NORMALCOORD n);
_Inline void LINKAGE _NormalLorentz(_NORMALCOORD n);
_Inline void LINKAGE _Subdivide(const _VERTEXCOORD v);
_Inline void LINKAGE _SubdivideEnd(void);
static void LINKAGE _SubdivideLine(GLfloat *const *arg);
static void LINKAGE _SubdivideTriangles(GLfloat *const *arg);
static void LINKAGE _SubdivideTriangleFan(GLfloat *const *arg);
static void LINKAGE _SubdivideQuads(GLfloat *const *arg);
static void LINKAGE _SubdivideQuadStrip(GLfloat *const *arg);
_Inline void LINKAGE _SpaceTransform(_VERTEXCOORD x);
_Inline void LINKAGE _Modelview(_VERTEXCOORD x);
_Inline void LINKAGE _SpaceLorentz(_VERTEXCOORD x);
_Inline void LINKAGE _IntegrateUdt(GLfloat *I);
_Inline void LINKAGE _SpaceEye(_VERTEXCOORD x);
_Inline GLfloat LINKAGE _TimeTransform(_VERTEXCOORD x);
_Inline GLfloat LINKAGE _TimeLorentz(const _VERTEXCOORD x);
_Inline GLfloat LINKAGE _IntegrateInvGammadt(void);
_Inline GLfloat LINKAGE _TimeEye(const _VERTEXCOORD x);
static GLfloat * LINKAGE _Doppler(const _RGBA rgba,const _VERTEXCOORD x);
_Inline void LINKAGE _RGBtoHSV(const GLfloat *rgb,GLfloat *hsv);
_Inline void LINKAGE _HSVtoRGB(const GLfloat *hsv,GLfloat *rgb);
_Inline void LINKAGE _EvalVelocityFunc(_VELOCITY u,GLfloat t);
_Inline void LINKAGE _DrawVertex(_VERTEXCOORD v,_TEXCOORD t);
_Inline void LINKAGE _UpdateMaterial(GLenum face,GLenum pname,const _RGBA rgba);
_Inline GLfloat LINKAGE _VecScalar(const GLfloat *a,const GLfloat *b);
_Inline GLfloat LINKAGE _VecNorm(const GLfloat *v);
_Inline GLfloat LINKAGE _VecCos(const GLfloat *x,const GLfloat *a,const GLfloat *b);
_Inline void LINKAGE _VertexCopy(_VERTEXCOORD dest,const _VERTEXCOORD src);
_Inline void LINKAGE _VertexMean(_VERTEXCOORD mean,const _VERTEXCOORD a,const _VERTEXCOORD b);
_Inline void LINKAGE _VertexQuadMean(_VERTEXCOORD mean,const _VERTEXCOORD a,const _VERTEXCOORD b,const _VERTEXCOORD c,const _VERTEXCOORD d);
_Inline void LINKAGE _TexCopy(_TEXCOORD dest,const _TEXCOORD src);
_Inline void LINKAGE _TexMean(_TEXCOORD mean,const _TEXCOORD a,const _TEXCOORD b);
_Inline void LINKAGE _TexQuadMean(_TEXCOORD mean,const _TEXCOORD a,const _TEXCOORD b,const _TEXCOORD c,const _TEXCOORD d);
#ifdef GL_VERSION_1_1
_Inline void LINKAGE srelVertexv(GLint size,GLenum type,const GLvoid *v,GLint offset);
_Inline void LINKAGE srelNormalv(GLenum type,const GLvoid *n,GLint offset);
_Inline void LINKAGE srelTexCoordv(GLint size,GLenum type,const GLvoid *v,GLint offset);
#endif
