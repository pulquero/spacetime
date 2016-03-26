/**
* @mainpage SREL
*
* SREL is an OpenGL extension for rendering relativistic environments.
*
* @section license License
*
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
*
* @section performance Performance Issues
*
* To increase performance:
* - reduce the detail level
* - turn off the Doppler shift
* - orientate srel GL_xxx_STRIP primitives so that the strip direction is
*   transverse to the direction of motion.
* - avoid using the srel OpenGL 1.1 compatible API.
*
* Everything in the rest frame can be drawn using standard OpenGL functions.
*
* @section sideeffects Behavioral Side-effects
*
* The Doppler shift can produce an inconsistent coloring if there is
* a sudden change in the density of srelVertices.
*
* @section trademarks Trademarks
*
* OpenGL is copyright (c) 1992-1999 Silicon Graphics, Inc,
* and is a registered trademark of Silicon Graphics, Inc.
*/



#define _FP_INLINE
#ifdef unix
   #include <GL/gl.h>
#else
   #ifdef WIN32
      #include <windows.h>
   #endif
   #include <gl\gl.h>
#endif
#include <math.h>
#include "srel.h"
#include "_srel.h"


static _SRELMACHINE srel={0.999f,      /* detail level */
                          GL_NO_ERROR, /* error flag */
                          GL_FALSE,    /* doppler */
                          GL_FALSE     /* in begin/end pair */};



/* ------------------------------ Internal Functions ------------------------------ */


/**
* @defgroup internal Internal Functions
*/
//@{

/**
* Transforms and updates the current normal.
*
* @param n A normal vector (input).
*/
void _NormalTransform(const _NORMALCOORD n)
{
   _NORMALCOORD p;
   p[0]=srel.model[0]*n[0]+srel.model[4]*n[1]+srel.model[8] *n[2];
   p[1]=srel.model[1]*n[0]+srel.model[5]*n[1]+srel.model[9] *n[2];
   p[2]=srel.model[2]*n[0]+srel.model[6]*n[1]+srel.model[10]*n[2];
   _NormalLorentz(p);
   glNormal3fv(p);
}
/**
* Lorentz transformation of normal vectors.
*
* Normal vectors are the velocity vectors of light rays.
* So, under a Lorentz transformation, they transform as velocities.
* @note
* This is responsible for the headlight effect.
*
* @see srelVelocityTransform()
*
* @param n A normal vector (input/output).
*/
void _NormalLorentz(_NORMALCOORD n)
{
   if(srel.cache.uu!=0.0f) {
      srelVelocityTransform(n);
   }
}


/**
* Subdividing algorithm.
* @b Supports:
* - GL_POINTS
* - GL_LINES
* - GL_LINE_STRIP
* - GL_LINE_LOOP
* - GL_TRIANGLES
* - GL_TRIANGLE_FAN
* - GL_QUADS
* - GL_QUAD_STRIP
* - GL_POLYGON
*
* @todo
* GL_TRIANGLE_STRIP
*
* @param v A vertex (input).
*/
void _Subdivide(const _VERTEXCOORD v)
{
   const int method=(srel.detail==0.0) ? SREL_NO_SUBDIVIDE : srel.vertex.mode;
   _VertexCopy(srel.vertex.cur[srel.vertex.num],v);
   _VertexCopy(srel.vertex.curST[srel.vertex.num],v);
   _SpaceTransform(srel.vertex.curST[srel.vertex.num]);
   srel.vertex.num++;
   switch(method) {
      case GL_POINTS:
         srel.vertex.num=0;
         _DrawVertex(srel.vertex.curST[0],0);
         break;
      case GL_LINES:
         if(srel.vertex.num==2) {
            GLfloat *arg[6];
            srel.vertex.num=0;
            _DrawVertex(srel.vertex.curST[0],srel.tex.cur[0]);
            arg[0]=srel.vertex.cur[0];
            arg[1]=srel.vertex.cur[1];
            arg[2]=srel.vertex.curST[0];
            arg[3]=srel.vertex.curST[1];
            arg[4]=srel.tex.cur[0];
            arg[5]=srel.tex.cur[1];
            _SubdivideLine(arg);
            _DrawVertex(srel.vertex.curST[1],srel.tex.cur[1]);
         }
         break;
      case GL_LINE_STRIP:
         if(srel.vertex.num==2) {
            GLfloat *arg[6];
            srel.vertex.num=1;
            if(srel.vertex.isFirst) {
               srel.vertex.isFirst=GL_FALSE;
               _DrawVertex(srel.vertex.curST[0],srel.tex.cur[0]);
            }
            arg[0]=srel.vertex.cur[0];
            arg[1]=srel.vertex.cur[1];
            arg[2]=srel.vertex.curST[0];
            arg[3]=srel.vertex.curST[1];
            arg[4]=srel.tex.cur[0];
            arg[5]=srel.tex.cur[1];
            _SubdivideLine(arg);
            _VertexCopy(srel.vertex.cur[0],srel.vertex.cur[1]);
            _VertexCopy(srel.vertex.curST[0],srel.vertex.curST[1]);
            _TexCopy(srel.tex.cur[0],srel.tex.cur[1]);
            _DrawVertex(srel.vertex.curST[1],srel.tex.cur[1]);
         }
         break;
      case GL_TRIANGLES:
         if(srel.vertex.num==3) {
            GLfloat *arg[9];
            srel.vertex.num=0;
            arg[0]=srel.vertex.cur[0];
            arg[1]=srel.vertex.cur[1];
            arg[2]=srel.vertex.cur[2];
            arg[3]=srel.vertex.curST[0];
            arg[4]=srel.vertex.curST[1];
            arg[5]=srel.vertex.curST[2];
            arg[6]=srel.tex.cur[0];
            arg[7]=srel.tex.cur[1];
            arg[8]=srel.tex.cur[2];
            _SubdivideTriangles(arg);
         }
         break;
      case GL_TRIANGLE_FAN:
         if(srel.vertex.num==3) {
            GLfloat *arg[9];
            srel.vertex.num=2;
            if(srel.vertex.isFirst) {
               srel.vertex.isFirst=GL_FALSE;
               _DrawVertex(srel.vertex.curST[0],srel.tex.cur[0]);
               _DrawVertex(srel.vertex.curST[1],srel.tex.cur[1]);
            }
            arg[0]=srel.vertex.cur[0];
            arg[1]=srel.vertex.cur[1];
            arg[2]=srel.vertex.cur[2];
            arg[3]=srel.vertex.curST[0];
            arg[4]=srel.vertex.curST[1];
            arg[5]=srel.vertex.curST[2];
            arg[6]=srel.tex.cur[0];
            arg[7]=srel.tex.cur[1];
            arg[8]=srel.tex.cur[2];
            _SubdivideTriangleFan(arg);
            _VertexCopy(srel.vertex.cur[1],srel.vertex.cur[2]);
            _VertexCopy(srel.vertex.curST[1],srel.vertex.curST[2]);
            _TexCopy(srel.tex.cur[1],srel.tex.cur[2]);
            _DrawVertex(srel.vertex.curST[2],srel.tex.cur[2]);
         }
         break;
      case GL_QUADS:
         if(srel.vertex.num==4) {
            GLfloat *arg[12];
            srel.vertex.num=0;
            arg[ 0]=srel.vertex.cur[0];
            arg[ 1]=srel.vertex.cur[1];
            arg[ 2]=srel.vertex.cur[2];
            arg[ 3]=srel.vertex.cur[3];
            arg[ 4]=srel.vertex.curST[0];
            arg[ 5]=srel.vertex.curST[1];
            arg[ 6]=srel.vertex.curST[2];
            arg[ 7]=srel.vertex.curST[3];
            arg[ 8]=srel.tex.cur[0];
            arg[ 9]=srel.tex.cur[1];
            arg[10]=srel.tex.cur[2];
            arg[11]=srel.tex.cur[3];
            _SubdivideQuads(arg);
         }
         break;
      case GL_QUAD_STRIP:
         if(srel.vertex.num==4) {
            GLfloat *arg[12];
            srel.vertex.num=2;
            if(srel.vertex.isFirst) {
               srel.vertex.isFirst=GL_FALSE;
               _DrawVertex(srel.vertex.curST[0],srel.tex.cur[0]);
               _DrawVertex(srel.vertex.curST[1],srel.tex.cur[1]);
            }
            arg[ 0]=srel.vertex.cur[0];
            arg[ 1]=srel.vertex.cur[1];
            arg[ 2]=srel.vertex.cur[2];
            arg[ 3]=srel.vertex.cur[3];
            arg[ 4]=srel.vertex.curST[0];
            arg[ 5]=srel.vertex.curST[1];
            arg[ 6]=srel.vertex.curST[2];
            arg[ 7]=srel.vertex.curST[3];
            arg[ 8]=srel.tex.cur[0];
            arg[ 9]=srel.tex.cur[1];
            arg[10]=srel.tex.cur[2];
            arg[11]=srel.tex.cur[3];
            _SubdivideQuadStrip(arg);
            _VertexCopy(srel.vertex.cur[0],srel.vertex.cur[2]);
            _VertexCopy(srel.vertex.cur[1],srel.vertex.cur[3]);
            _VertexCopy(srel.vertex.curST[0],srel.vertex.curST[2]);
            _VertexCopy(srel.vertex.curST[1],srel.vertex.curST[3]);
            _TexCopy(srel.tex.cur[0],srel.tex.cur[2]);
            _TexCopy(srel.tex.cur[1],srel.tex.cur[3]);
            _DrawVertex(srel.vertex.curST[2],srel.tex.cur[2]);
            _DrawVertex(srel.vertex.curST[3],srel.tex.cur[3]);
         }
         break;
      case GL_LINE_LOOP:
      case GL_POLYGON:
         if(srel.vertex.num==2) {
            GLfloat *arg[6];
            srel.vertex.num=1;
            if(srel.vertex.isFirst) {
               srel.vertex.isFirst=GL_FALSE;
               _VertexCopy(srel.vertex.cur[2],srel.vertex.cur[0]);
               _VertexCopy(srel.vertex.curST[2],srel.vertex.curST[0]);
               _TexCopy(srel.tex.cur[2],srel.tex.cur[0]);
               _DrawVertex(srel.vertex.curST[0],srel.tex.cur[0]);
            }
            arg[0]=srel.vertex.cur[0];
            arg[1]=srel.vertex.cur[1];
            arg[2]=srel.vertex.curST[0];
            arg[3]=srel.vertex.curST[1];
            arg[4]=srel.tex.cur[0];
            arg[5]=srel.tex.cur[1];
            _SubdivideLine(arg);
            _VertexCopy(srel.vertex.cur[0],srel.vertex.cur[1]);
            _VertexCopy(srel.vertex.curST[0],srel.vertex.curST[1]);
            _TexCopy(srel.tex.cur[0],srel.tex.cur[1]);
            _DrawVertex(srel.vertex.curST[1],srel.tex.cur[1]);
         }
         break;
      case SREL_NO_SUBDIVIDE:
      default:
         srel.vertex.num=0;
         _DrawVertex(srel.vertex.curST[0],srel.tex.cur[0]);
   }
}
void _SubdivideEnd(void)
{
   switch(srel.vertex.mode) {
      case GL_LINE_LOOP:
      case GL_POLYGON:
         {
            GLfloat *arg[6];
            arg[0]=srel.vertex.cur[0];
            arg[1]=srel.vertex.cur[2];
            arg[2]=srel.vertex.curST[0];
            arg[3]=srel.vertex.curST[2];
            arg[4]=srel.tex.cur[0];
            arg[5]=srel.tex.cur[2];
            _SubdivideLine(arg);
            _DrawVertex(srel.vertex.curST[2],srel.tex.cur[2]);
         }
         break;
   }
}



#if defined(SUBDIVIDE_RECURSIVELY)
   #include "subdivide-recursive.c"
#elif defined(SUBDIVIDE_NONCONSERVATIVELY)
   #include "subdivide-iterative-nonconservative.c"
#else
   #include "subdivide-iterative.c"
#endif



/**
* Spatial coordinate transformations.
* @param x A vertex (input/output).
*/
void _SpaceTransform(_VERTEXCOORD x)
{
   _Modelview(x);
   _SpaceLorentz(x);
   _SpaceEye(x);
}
/**
* Modelview transformation.
* @param x A vertex (input/output).
*/
void _Modelview(_VERTEXCOORD x)
{
   const GLfloat y[4]={x[0],x[1],x[2],x[3]};
   x[0]=srel.model[0]*y[0]+srel.model[4]*y[1]+srel.model[8] *y[2]+srel.model[12]*y[3];
   x[1]=srel.model[1]*y[0]+srel.model[5]*y[1]+srel.model[9] *y[2]+srel.model[13]*y[3];
   x[2]=srel.model[2]*y[0]+srel.model[6]*y[1]+srel.model[10]*y[2]+srel.model[14]*y[3];
   x[3]=srel.model[3]*y[0]+srel.model[7]*y[1]+srel.model[11]*y[2]+srel.model[15]*y[3];
}
/**
* Spatial Lorentz transformation.
*
* @f[ z = \frac{z' - u t'}{\sqrt{1-u^2/c^2}} @f]
* Rearranging,
* @f[ z' = z \sqrt{1-u^2/c^2} + u t' @f]
* Use
* @f[ z = \mathbf{U}\cdot\mathbf{X}/u @f]
* and
* @f[ \mathbf{X}_\parallel = (\mathbf{U}\cdot\mathbf{X}/u^2) \mathbf{U} @f]
* @f[ \mathbf{X'}_\perp = \mathbf{X}_\perp = \mathbf{X}-\mathbf{X}_\parallel @f]
* to get
* @f[ \mathbf{X'} = \mathbf{X} + \frac{1-\gamma}{\gamma}(\mathbf{U}\cdot\mathbf{X}/u^2)\mathbf{U} + \mathbf{U}t @f]
*
* @note
* This is responsible for the Lorentz contraction.
*
* @param x A vertex (input/output).
*/
void _SpaceLorentz(_VERTEXCOORD x)
{
   if(srel.uFunc) {
      // update velocity
      _EvalVelocityFunc(srel.u,srel.t);
      srel.cache.uu=_VecScalar(srel.u,srel.u);
      srel.cache.speed=sqrt(srel.cache.uu);
      srel.cache.oneMuu=1.0f-srel.cache.uu;
      srel.cache.invGamma=sqrt(srel.cache.oneMuu);
   }
   if(srel.cache.uu!=0.0f) {
      const GLfloat ux=_VecScalar(srel.u,x);
      const GLfloat k=(srel.cache.invGamma-1.0f)*ux/srel.cache.uu;
      GLfloat intUdt[3];
      if(srel.uFunc) {
         _IntegrateUdt(intUdt);
      } else {
         intUdt[0]=srel.u[0]*srel.t;
         intUdt[1]=srel.u[1]*srel.t;
         intUdt[2]=srel.u[2]*srel.t;
      }
      x[0]+=k*srel.u[0]+intUdt[0];
      x[1]+=k*srel.u[1]+intUdt[1];
      x[2]+=k*srel.u[2]+intUdt[2];
   }
}
/**
* Integrates velocity over time.
* Uses Gaussian quadrature.
*/
void _IntegrateUdt(GLfloat *I)
{
   const int N=100;
   const GLfloat dt=srel.t/N;
   const GLfloat dt_2=dt/2.0;
   _VELOCITY u;
   GLfloat t=0.0;
   int i;
   I[0]=0.0;I[1]=0.0;I[2]=0.0;
   for(i=0;i<N;i++) {
      _EvalVelocityFunc(u,t+(GAUSSIAN_ZEROS_0+1.0)*dt_2);
      I[0]+=GAUSSIAN_COEFFS_0*u[0];
      I[1]+=GAUSSIAN_COEFFS_0*u[1];
      I[2]+=GAUSSIAN_COEFFS_0*u[2];
      _EvalVelocityFunc(u,t+(GAUSSIAN_ZEROS_1+1.0)*dt_2);
      I[0]+=GAUSSIAN_COEFFS_1*u[0];
      I[1]+=GAUSSIAN_COEFFS_1*u[1];
      I[2]+=GAUSSIAN_COEFFS_1*u[2];
      _EvalVelocityFunc(u,t+(GAUSSIAN_ZEROS_2+1.0)*dt_2);
      I[0]+=GAUSSIAN_COEFFS_2*u[0];
      I[1]+=GAUSSIAN_COEFFS_2*u[1];
      I[2]+=GAUSSIAN_COEFFS_2*u[2];
      _EvalVelocityFunc(u,t+(GAUSSIAN_ZEROS_3+1.0)*dt_2);
      I[0]+=GAUSSIAN_COEFFS_3*u[0];
      I[1]+=GAUSSIAN_COEFFS_3*u[1];
      I[2]+=GAUSSIAN_COEFFS_3*u[2];
      t+=dt;
   }
   I[0]*=dt_2;
   I[1]*=dt_2;
   I[2]*=dt_2;
}
/**
* Aberration (time lag for light to reach the eye).
*
* @f[ \mathbf{X} = \mathbf{X'} + \mathbf{U} \Delta t @f]
*
* where
*
* @f$ \mathbf{X} @f$
* is the current position vector,
*
* @f$ \mathbf{X'} @f$
* is the past position vector we see the light from,
*
* @f$ \Delta t = |\mathbf{X'}|/c @f$
* is the time it takes for the light to reach us,
*
* @f$ \mathbf{U} @f$
* is the velocity vector.
*
* Rearranging,
* @f[ \mathbf{X}-\mathbf{X'} = \mathbf{U} \sqrt{\mathbf{X'}\cdot\mathbf{X'}} @f]
* Let @f$ \Delta\mathbf{X} = \mathbf{X}-\mathbf{X'} @f$, then
* @f[ \Delta\mathbf{X} = \mathbf{U} \sqrt{(\mathbf{X}-\Delta\mathbf{X})\cdot(\mathbf{X}-\Delta\mathbf{X})} @f]
* Solving for @f$ \Delta\mathbf{X} @f$ gives
* @f[ \mathbf{X'} = \mathbf{X} + \frac{\mathbf{U}\cdot\mathbf{X}-\sqrt{(\mathbf{U}\cdot\mathbf{X})^2+(1-u^2)|X|^2}}{1-u^2}\mathbf{U} @f]
*
* @note
* This is responsible for the Terrell effect.
*
* @param x A vertex (input/output).
*/
void _SpaceEye(_VERTEXCOORD x)
{
   if(!srel.uFunc) {
      const GLfloat xx=_VecScalar(x,x);
      const GLfloat ux=_VecScalar(srel.u,x);
      const GLfloat k=(ux-sqrt(ux*ux+srel.cache.oneMuu*xx))/srel.cache.oneMuu;
      x[0]+=k*srel.u[0];
      x[1]+=k*srel.u[1];
      x[2]+=k*srel.u[2];
   }
}



/**
* Temporal coordinate transformations.
* @param x A vertex (input/output).
*/
GLfloat _TimeTransform(_VERTEXCOORD x)
{
   _Modelview(x);
   return _TimeLorentz(x)-_TimeEye(x);
}
/**
* Temporal Lorentz transformation.
*
* @f[ t = \frac{t' - \mathbf{U}\cdot\mathbf{X'}/c^2}{\sqrt{1-u^2/c^2}} @f]
* Rearranging,
* @f[ t' = t \sqrt{1-u^2/c^2} + \mathbf{U}\cdot\mathbf{X'}/c^2 @f]
*
* @note
* This is responsible for time dilation.
*
* @param x A vertex (input).
*/
GLfloat _TimeLorentz(const _VERTEXCOORD x)
{
   GLfloat intInvGammadt;
   if(srel.uFunc) {
      intInvGammadt=_IntegrateInvGammadt();
   } else {
      intInvGammadt=srel.cache.invGamma*srel.t;
   }
   return intInvGammadt+_VecScalar(srel.u,x);
}
/**
* Integrates sqrt(1-u*u) over time.
* Uses Gaussian quadrature.
*/
GLfloat _IntegrateInvGammadt(void)
{
   const int N=100;
   const GLfloat dt=srel.t/N;
   const GLfloat dt_2=dt/2.0;
   _VELOCITY u;
   GLfloat t=0.0;
   GLfloat I=0.0;
   int i;
   for(i=0;i<N;i++) {
      _EvalVelocityFunc(u,t+(GAUSSIAN_ZEROS_0+1.0)*dt_2);
      I+=GAUSSIAN_COEFFS_0*sqrt(1.0-_VecScalar(u,u));
      _EvalVelocityFunc(u,t+(GAUSSIAN_ZEROS_1+1.0)*dt_2);
      I+=GAUSSIAN_COEFFS_1*sqrt(1.0-_VecScalar(u,u));
      _EvalVelocityFunc(u,t+(GAUSSIAN_ZEROS_2+1.0)*dt_2);
      I+=GAUSSIAN_COEFFS_2*sqrt(1.0-_VecScalar(u,u));
      _EvalVelocityFunc(u,t+(GAUSSIAN_ZEROS_3+1.0)*dt_2);
      I+=GAUSSIAN_COEFFS_3*sqrt(1.0-_VecScalar(u,u));
      t+=dt;
   }
   return I*dt_2;
}
/**
* Aberration (time lag for light to reach the eye).
*
* @f[ \Delta t = |\mathbf{X}|/c @f]
*
* @param x A vertex (input).
*/
GLfloat _TimeEye(const _VERTEXCOORD x)
{
   return _VecNorm(x);
}



/**
* Doppler effect.
*
* Time dilation effect,
* @f[ T' = \frac{T}{\sqrt{1-u^2/c^2}} @f]
* Arrival rate at eye,
* @f[ T_\mathrm{eye} = (|\mathbf{X}+\mathbf{U} T'|-|\mathbf{X}|)/c + T' @f]
*
* Overall frequency shift,
* @f[ f_\mathrm{eye} = \frac{f'}{1 + f' (|\mathbf{X}+\mathbf{U}/f'|-|\mathbf{X}|)/c} @f]
*/
GLfloat *_Doppler(const _RGBA rgba,const _VERTEXCOORD x)
{
   static _RGBA srelRGBA;
   const GLfloat red=42.857f,blue=75.0f;   // frequencies
   const GLfloat grad=(blue-red)/4.5f;     // gradient
   GLfloat hsv[3];
   _RGBtoHSV(rgba,hsv);
   if(hsv[0]!=-1.0f) {
      GLfloat xprime[3];
      GLfloat f=grad*hsv[0]+red;   // light frequency
   // Doppler shift
      f*=srel.cache.invGamma;
      xprime[0]=x[0]+srel.u[0]/f;
      xprime[1]=x[1]+srel.u[1]/f;
      xprime[2]=x[2]+srel.u[2]/f;
      f/=1.0f+f*(_VecNorm(xprime)-_VecNorm(x));
   // if outside visible range
      if(f<red || f>blue) {
         srelRGBA[0]=srelRGBA[1]=srelRGBA[2]=0.0f;   // then set to black
      } else {
         hsv[0]=(f-red)/grad;
         _HSVtoRGB(hsv,srelRGBA);
      }
   } else {
      srelRGBA[0]=rgba[0];
      srelRGBA[1]=rgba[1];
      srelRGBA[2]=rgba[2];
   }
   srelRGBA[3]=rgba[3];
   return srelRGBA;
}
void _RGBtoHSV(const GLfloat *rgb,GLfloat *hsv)
{
   const GLfloat x=(rgb[0]<=rgb[1] && rgb[0]<=rgb[2]) ? rgb[0] : ((rgb[1]<=rgb[0] && rgb[1]<=rgb[2]) ? rgb[1] : rgb[2]);
   hsv[2]=(rgb[0]>=rgb[1] && rgb[0]>=rgb[2]) ? rgb[0] : ((rgb[1]>=rgb[0] && rgb[1]>=rgb[2]) ? rgb[1] : rgb[2]);
   if(hsv[2]==x) {
      hsv[0]=-1.0f;hsv[1]=0.0f;
   } else {
      const GLfloat f=(rgb[0]==x) ? rgb[1]-rgb[2] : ((rgb[1]==x) ? rgb[2]-rgb[0] : rgb[0]-rgb[1]);
      const int i=(rgb[0]==x) ? 3 : ((rgb[1]==x) ? 5 : 1);
      hsv[0]=i-f/(hsv[2]-x);
      hsv[1]=(hsv[2]-x)/hsv[2];
   }
}
void _HSVtoRGB(const GLfloat *hsv,GLfloat *rgb)
{
   if(hsv[0]==-1.0f) {
      rgb[0]=rgb[1]=rgb[2]=hsv[2];
   } else {
      const int i=floor(hsv[0]);
      GLfloat m,n,f;
      f=hsv[0]-i;
      if(!(i & 1))
         f=1.0f-f;
      m=hsv[2]*(1.0f-hsv[1]);
      n=hsv[2]*(1.0f-hsv[1]*f);
      switch(i) {
         case 6:
         case 0: rgb[0]=hsv[2];rgb[1]=n;rgb[2]=m;
                 break;
         case 1: rgb[0]=n;rgb[1]=hsv[2];rgb[2]=m;
                 break;
         case 2: rgb[0]=m;rgb[1]=hsv[2];rgb[2]=n;
                 break;
         case 3: rgb[0]=m;rgb[1]=n;rgb[2]=hsv[2];
                 break;
         case 4: rgb[0]=n;rgb[1]=m;rgb[2]=hsv[2];
                 break;
         case 5: rgb[0]=hsv[2];rgb[1]=m;rgb[2]=n;
                 break;
      }
   }
}


/**
* Evaluate the velocity function.
*/
void _EvalVelocityFunc(_VELOCITY u,GLfloat t)
{
   _VELOCITY v={u[0],u[1],u[2]};
   (*srel.uFunc)(v,t);
   u[0]=srel.model[0]*v[0]+srel.model[4]*v[1]+srel.model[8] *v[2];
   u[1]=srel.model[1]*v[0]+srel.model[5]*v[1]+srel.model[9] *v[2];
   u[2]=srel.model[2]*v[0]+srel.model[6]*v[1]+srel.model[10]*v[2];
}


/**
* Draws a vertex.
*/
void _DrawVertex(_VERTEXCOORD v,_TEXCOORD t)
{
   if(srel.doppler) {
      glMaterialfv(srel.mat.face,GL_SPECULAR,_Doppler(srel.mat.spec,v));
      glMaterialfv(srel.mat.face,GL_AMBIENT,_Doppler(srel.mat.amb,v));
      glMaterialfv(srel.mat.face,GL_DIFFUSE,_Doppler(srel.mat.diff,v));
   }
   if(srel.tex.haveCoord)
      glTexCoord2fv(t);
   glVertex4fv(v);
}

void _UpdateMaterial(GLenum face,GLenum pname,const _RGBA rgba)
{
   srel.mat.face=face;
   switch(pname) {
      case GL_SPECULAR:
         srel.mat.spec[0]=rgba[0];
         srel.mat.spec[1]=rgba[1];
         srel.mat.spec[2]=rgba[2];
         srel.mat.spec[3]=rgba[3];
         break;
      case GL_AMBIENT:
         srel.mat.amb[0]=rgba[0];
         srel.mat.amb[1]=rgba[1];
         srel.mat.amb[2]=rgba[2];
         srel.mat.amb[3]=rgba[3];
         break;
      case GL_DIFFUSE:
         srel.mat.diff[0]=rgba[0];
         srel.mat.diff[1]=rgba[1];
         srel.mat.diff[2]=rgba[2];
         srel.mat.diff[3]=rgba[3];
         break;
      case GL_AMBIENT_AND_DIFFUSE:
         srel.mat.amb[0]=srel.mat.diff[0]=rgba[0];
         srel.mat.amb[1]=srel.mat.diff[1]=rgba[1];
         srel.mat.amb[2]=srel.mat.diff[2]=rgba[2];
         srel.mat.amb[3]=srel.mat.diff[3]=rgba[3];
         break;
   }
}

/**
* Scalar product of two 3-vectors.
*/
GLfloat _VecScalar(const GLfloat *a,const GLfloat *b)
{
   return a[0]*b[0]+a[1]*b[1]+a[2]*b[2];
}

/**
* Norm of a 3-vector.
*/
GLfloat _VecNorm(const GLfloat *v)
{
   return sqrt(_VecScalar(v,v));
}

/**
* Cosine angle between direction vectors x->a and x->b.
*/
GLfloat _VecCos(const GLfloat *x,const GLfloat *a,const GLfloat *b)
{
   const GLfloat xa[3]={a[0]-x[0],a[1]-x[1],a[2]-x[2]};
   const GLfloat xb[3]={b[0]-x[0],b[1]-x[1],b[2]-x[2]};
   return fabs(_VecScalar(xa,xb)/(_VecNorm(xa)*_VecNorm(xb)));
}

/**
* Copy a vertex.
*/
void _VertexCopy(_VERTEXCOORD dest,const _VERTEXCOORD src)
{
   dest[0]=src[0];
   dest[1]=src[1];
   dest[2]=src[2];
   dest[3]=src[3];
}

/**
* Average of two vertices.
*/
void _VertexMean(_VERTEXCOORD mean,const _VERTEXCOORD a,const _VERTEXCOORD b)
{
   mean[0]=0.5f*(a[0]+b[0]);
   mean[1]=0.5f*(a[1]+b[1]);
   mean[2]=0.5f*(a[2]+b[2]);
   mean[3]=1.0f;
}

/**
* Average of four vertices.
*/
void _VertexQuadMean(_VERTEXCOORD mean,const _VERTEXCOORD a,const _VERTEXCOORD b,const _VERTEXCOORD c,const _VERTEXCOORD d)
{
   mean[0]=0.25f*(a[0]+b[0]+c[0]+d[0]);
   mean[1]=0.25f*(a[1]+b[1]+c[1]+d[1]);
   mean[2]=0.25f*(a[2]+b[2]+c[2]+d[2]);
   mean[3]=1.0f;
}

/**
* Copy texture coordinates.
*/
void _TexCopy(_TEXCOORD dest,const _TEXCOORD src)
{
   dest[0]=src[0];
   dest[1]=src[1];
}

/**
* Average of two texture vertices.
*/
void _TexMean(_TEXCOORD mean,const _TEXCOORD a,const _TEXCOORD b)
{
   mean[0]=0.5f*(a[0]+b[0]);
   mean[1]=0.5f*(a[1]+b[1]);
}

/**
* Average of four texture vertices.
*/
void _TexQuadMean(_TEXCOORD mean,const _TEXCOORD a,const _TEXCOORD b,const _TEXCOORD c,const _TEXCOORD d)
{
   mean[0]=0.25f*(a[0]+b[0]+c[0]+d[0]);
   mean[1]=0.25f*(a[1]+b[1]+c[1]+d[1]);
}

//@}



/* ------------------------------ API Functions ------------------------------ */


/**
* @defgroup api_1_0 API Functions (OpenGL 1.0)
* The SREL OpenGL 1.0 compatible API.
* @note
* The srel equivalent of any gl function should only be executed
* between an srelBegin/srelEnd pair.
*/
//@{

/**
* Sets the level of detail.
* The value is the cosine of the maximum allowable angle
* that a line can deviate from its true position.
* It determines how many line segments should be used
* to represent a curve.
* @param detail A number greater than or equal to 0 and less than 1.
* @exception GL_INVALID_OPERATION If executed between an srelBegin/srelEnd pair.
* @exception GL_INVALID_VALUE If detail is less than 0 or greater than or equal to 1.
*/
void srelDetail(const GLfloat detail)
{
   if(srel.begin) {
      srel.error=GL_INVALID_OPERATION;
      return;
   }
   if(detail<0.0 || detail>=1.0) {
      srel.error=GL_INVALID_VALUE;
      return;
   }
   srel.detail=detail;
}

/**
* Returns error information.
* Error flag is reset after return.
*/
GLenum srelGetError(void)
{
   const GLenum rc=srel.error;
   srel.error=GL_NO_ERROR;
   return rc;
}

/**
* Sets whether the Doppler effect should be included in
* the lighting calculations.
* @exception GL_INVALID_OPERATION If executed between an srelBegin/srelEnd pair.
*/
void srelDoppler(const GLboolean flag)
{
   if(srel.begin) {
      srel.error=GL_INVALID_OPERATION;
      return;
   }
   srel.doppler=flag;
}

/**
* @name srelVelocity
* Sets the current frame velocity.
* All subsequent primitives will be rendered as if they
* are traveling at that velocity.
* @note
* The values of the velocity components are in units of c.
* It is an error to specify a velocity with a speed greater than
* or equal to c.
* It is an error to execute this between an srelBegin/srelEnd pair.
*/
//@{

/**
* Example:
* @code
* srelVelocityf(0.0,0.0,0.0);
* draw_rest_frame();
* srelVelocityf(0.0,0.2,0.5);
* draw_moving_frame1();
* srelVelocityf(0.8,0.0,-0.3);
* draw_moving_frame2();
* @endcode
* @param ux The x component of the velocity.
* @param uy The y component of the velocity.
* @param uz The z component of the velocity.
* @exception GL_INVALID_OPERATION If executed between an srelBegin/srelEnd pair.
* @exception GL_INVALID_VALUE If the speed is greater than or equal to 1.
*/
void srelVelocityf(const GLfloat ux,const GLfloat uy,const GLfloat uz)
{
   const _VELOCITY u={ux,uy,uz};
   srelVelocityfv(u);
}
/**
* @param u A pointer to an array containing the three velocity components (input).
* @exception GL_INVALID_OPERATION If executed between an srelBegin/srelEnd pair.
* @exception GL_INVALID_VALUE If the speed is greater than or equal to 1.
*/
void srelVelocityfv(const GLfloat *u)
{
   const GLfloat uu=_VecScalar(u,u);
   if(srel.begin) {
      srel.error=GL_INVALID_OPERATION;
      return;
   }
   if(uu>=1.0) {
      srel.error=GL_INVALID_VALUE;
      return;
   }
   srel.uFunc=0;
   glGetFloatv(GL_MODELVIEW_MATRIX,srel.model);
   srel.u[0]=srel.model[0]*u[0]+srel.model[4]*u[1]+srel.model[8] *u[2];
   srel.u[1]=srel.model[1]*u[0]+srel.model[5]*u[1]+srel.model[9] *u[2];
   srel.u[2]=srel.model[2]*u[0]+srel.model[6]*u[1]+srel.model[10]*u[2];
   srel.cache.uu=uu;
   srel.cache.speed=sqrt(srel.cache.uu);
   srel.cache.oneMuu=1.0f-srel.cache.uu;
   srel.cache.invGamma=sqrt(srel.cache.oneMuu);
}
/**
* @param ufn A velocity function.
* @exception GL_INVALID_OPERATION If executed between an srelBegin/srelEnd pair.
*/
void srelVelocityFunc(void (* LINKAGE ufn)(GLfloat *u,GLfloat t))
{
   if(srel.begin) {
      srel.error=GL_INVALID_OPERATION;
      return;
   }
   srel.uFunc=ufn;
}
//@}

/**
* Transforms a velocity from the rest frame to the
* current frame specified by srelVelocity.
* Only really useful for moving between frames.
* @param v A pointer to an array containing the three velocity components (input/output).
*
* @f[ \mathbf{V'} = \frac{\mathbf{V}+(\gamma+(\gamma-1)\mathbf{U}\cdot\mathbf{V}/u^2)\mathbf{U}}{\gamma(1+\mathbf{U}\cdot\mathbf{V})} @f]
*/
void srelVelocityTransform(GLfloat *v)
{
   const GLfloat uv=_VecScalar(srel.u,v);
   const GLfloat k=1.0f-(srel.cache.invGamma-1.0f)*uv/srel.cache.uu;
   const GLfloat l=1.0f+uv;
   v[0]=(v[0]*srel.cache.invGamma+k*srel.u[0])/l;
   v[1]=(v[1]*srel.cache.invGamma+k*srel.u[1])/l;
   v[2]=(v[2]*srel.cache.invGamma+k*srel.u[2])/l;
}

/**
* Sets the time at the camera.
* All subsequent primitives will be evolved forward to this time.
*
* Example: (animation)
* @code
* for(float t=0.0;;t+=0.1) {
*    srelLocalTime(t);
*    redraw();
* }
* @endcode
*
* @exception GL_INVALID_OPERATION If executed between an srelBegin/srelEnd pair.
*/
void srelLocalTime(const GLfloat t)
{
   if(srel.begin) {
      srel.error=GL_INVALID_OPERATION;
      return;
   }
   srel.t=t;
}

/**
* @name srelVertexTime
* Returns the observed time at a vertex in the current
* frame specified by srelVelocity.
*/
//@{

/**
* @param x The x component of a vertex.
* @param y The y component of a vertex.
* @param z The z component of a vertex.
*/
GLfloat srelVertexTime3f(GLfloat x,GLfloat y,GLfloat z)
{
   _VERTEXCOORD v={x,y,z,1.0f};
   return _TimeTransform(v);
}
/**
* @param v A pointer to an array containing three components.
*/
GLfloat srelVertexTime3fv(const GLfloat *v)
{
   _VERTEXCOORD u={v[0],v[1],v[2],1.0f};
   return _TimeTransform(u);
}
/**
* @param x The x component of a vertex.
* @param y The y component of a vertex.
* @param z The z component of a vertex.
*/
GLfloat srelVertexTime3d(GLdouble x,GLdouble y,GLdouble z)
{
   _VERTEXCOORD v={x,y,z,1.0f};
   return _TimeTransform(v);
}
/**
* @param v A pointer to an array containing three components.
*/
GLfloat srelVertexTime3dv(const GLdouble *v)
{
   _VERTEXCOORD u={v[0],v[1],v[2],1.0f};
   return _TimeTransform(u);
}
//@}


/**
* @name srelBegin/End
*/
//@{

/**
* Begins a new group of vertices.
* @param mode Specifies the primitive mode (same values as @c glBegin).
*/
void srelBegin(GLenum mode)
{
   srel.begin=GL_TRUE;
   srel.vertex.mode=mode;
   srel.vertex.isFirst=GL_TRUE;
   srel.vertex.num=0;
   srel.tex.haveCoord=GL_FALSE;
   if(srel.doppler) {
      /* get current material parameters */
      srel.mat.face=GL_FRONT;
      glGetMaterialfv(srel.mat.face,GL_SPECULAR,srel.mat.spec);
      glGetMaterialfv(srel.mat.face,GL_AMBIENT,srel.mat.amb);
      glGetMaterialfv(srel.mat.face,GL_DIFFUSE,srel.mat.diff);
      if(glIsEnabled(GL_COLOR_MATERIAL))
         glGetIntegerv(GL_COLOR_MATERIAL_PARAMETER,&srel.mat.colorMode);
      else
         srel.mat.colorMode=0;
   }
   glGetFloatv(GL_MODELVIEW_MATRIX,srel.model);
   glPushMatrix();
   glLoadIdentity();
   glBegin(mode);
}
/**
* Ends the current group of vertices.
*/
void srelEnd(void)
{
   _SubdivideEnd();
   glEnd();
   glPopMatrix();
   if(srel.doppler) {
      /* restore material parameters (altered by DrawVertex) */
      glMaterialfv(GL_FRONT,GL_SPECULAR,srel.mat.spec);
      glMaterialfv(GL_FRONT,GL_AMBIENT,srel.mat.amb);
      glMaterialfv(GL_FRONT,GL_DIFFUSE,srel.mat.diff);
   }
   srel.begin=GL_FALSE;
}
//@}


/**
* @name srelVertex
* Specifies a vertex which will undergo relativistic transformations.
*/
//@{

/**
* @param x The x component of a vertex.
* @param y The y component of a vertex.
*/
void srelVertex2f(GLfloat x,GLfloat y)
{
   _VERTEXCOORD v={x,y,0.0f,1.0f};
   _Subdivide(v);
}
/**
* @param v A pointer to an array containing two components.
*/
void srelVertex2fv(const GLfloat *v)
{
   _VERTEXCOORD u={v[0],v[1],0.0f,1.0f};
   _Subdivide(u);
}
/**
* @param x The x component of a vertex.
* @param y The y component of a vertex.
*/
void srelVertex2d(GLdouble x,GLdouble y)
{
   _VERTEXCOORD v={x,y,0.0f,1.0f};
   _Subdivide(v);
}
/**
* @param v A pointer to an array containing two components.
*/
void srelVertex2dv(const GLdouble *v)
{
   _VERTEXCOORD u={v[0],v[1],0.0f,1.0f};
   _Subdivide(u);
}
/**
* @param x The x component of a vertex.
* @param y The y component of a vertex.
* @param z The z component of a vertex.
*/
void srelVertex3f(GLfloat x,GLfloat y,GLfloat z)
{
   _VERTEXCOORD v={x,y,z,1.0f};
   _Subdivide(v);
}
/**
* @param v A pointer to an array containing three components.
*/
void srelVertex3fv(const GLfloat *v)
{
   _VERTEXCOORD u={v[0],v[1],v[2],1.0f};
   _Subdivide(u);
}
/**
* @param x The x component of a vertex.
* @param y The y component of a vertex.
* @param z The z component of a vertex.
*/
void srelVertex3d(GLdouble x,GLdouble y,GLdouble z)
{
   _VERTEXCOORD v={x,y,z,1.0f};
   _Subdivide(v);
}
/**
* @param v A pointer to an array containing three components.
*/
void srelVertex3dv(const GLdouble *v)
{
   _VERTEXCOORD u={v[0],v[1],v[2],1.0f};
   _Subdivide(u);
}
/**
* @param x The x component of a vertex.
* @param y The y component of a vertex.
* @param z The z component of a vertex.
* @param w The w component of a vertex.
*/
void srelVertex4f(GLfloat x,GLfloat y,GLfloat z,GLfloat w)
{
   _VERTEXCOORD v={x,y,z,w};
   _Subdivide(v);
}
/**
* @param v A pointer to an array containing four components.
*/
void srelVertex4fv(const GLfloat *v)
{
   _Subdivide(v);
}
/**
* @param x The x component of a vertex.
* @param y The y component of a vertex.
* @param z The z component of a vertex.
* @param w The w component of a vertex.
*/
void srelVertex4d(GLdouble x,GLdouble y,GLdouble z,GLdouble w)
{
   _VERTEXCOORD v={x,y,z,w};
   _Subdivide(v);
}
/**
* @param v A pointer to an array containing four components.
*/
void srelVertex4dv(const GLdouble *v)
{
   _VERTEXCOORD u={v[0],v[1],v[2],v[3]};
   _Subdivide(u);
}
//@}


/**
* @name srelNormal
* Specifies a normal.
*/
//@{

/**
* @param x The x component of a normal.
* @param y The y component of a normal.
* @param z The z component of a normal.
*/
void srelNormal3f(GLfloat x,GLfloat y,GLfloat z)
{
   _NORMALCOORD n={x,y,z};
   _NormalTransform(n);
}
/**
* @param n A pointer to an array containing three components.
*/
void srelNormal3fv(const GLfloat *n)
{
   _NormalTransform(n);
}
/**
* @param x The x component of a normal.
* @param y The y component of a normal.
* @param z The z component of a normal.
*/
void srelNormal3d(GLdouble x,GLdouble y,GLdouble z)
{
   _NORMALCOORD n={x,y,z};
   _NormalTransform(n);
}
/**
* @param n A pointer to an array containing three components.
*/
void srelNormal3dv(const GLdouble *n)
{
   _NORMALCOORD m={n[0],n[1],n[2]};
   _NormalTransform(m);
}
//@}


/**
* @name srelTexCoord
* Specifies a texture coordinate.
*/
//@{

/**
* @param s The s component of a texture.
*/
void srelTexCoord1f(GLfloat s)
{
   srel.tex.haveCoord=GL_TRUE;
   srel.tex.cur[srel.vertex.num][0]=s;
   srel.tex.cur[srel.vertex.num][1]=0.0f;
}
/**
* @param v A pointer to an array containing one component.
*/
void srelTexCoord1fv(const GLfloat *v)
{
   srel.tex.haveCoord=GL_TRUE;
   srel.tex.cur[srel.vertex.num][0]=v[0];
   srel.tex.cur[srel.vertex.num][1]=0.0f;
}
/**
* @param s The s component of a texture.
*/
void srelTexCoord1d(GLdouble s)
{
   srel.tex.haveCoord=GL_TRUE;
   srel.tex.cur[srel.vertex.num][0]=s;
   srel.tex.cur[srel.vertex.num][1]=0.0f;
}
/**
* @param v A pointer to an array containing one component.
*/
void srelTexCoord1dv(const GLdouble *v)
{
   srel.tex.haveCoord=GL_TRUE;
   srel.tex.cur[srel.vertex.num][0]=v[0];
   srel.tex.cur[srel.vertex.num][1]=0.0f;
}
/**
* @param s The s component of a texture.
* @param t The t component of a texture.
*/
void srelTexCoord2f(GLfloat s,GLfloat t)
{
   srel.tex.haveCoord=GL_TRUE;
   srel.tex.cur[srel.vertex.num][0]=s;
   srel.tex.cur[srel.vertex.num][1]=t;
}
/**
* @param v A pointer to an array containing two components.
*/
void srelTexCoord2fv(const GLfloat *v)
{
   srel.tex.haveCoord=GL_TRUE;
   srel.tex.cur[srel.vertex.num][0]=v[0];
   srel.tex.cur[srel.vertex.num][1]=v[1];
}
/**
* @param s The s component of a texture.
* @param t The t component of a texture.
*/
void srelTexCoord2d(GLdouble s,GLdouble t)
{
   srel.tex.haveCoord=GL_TRUE;
   srel.tex.cur[srel.vertex.num][0]=s;
   srel.tex.cur[srel.vertex.num][1]=t;
}
/**
* @param v A pointer to an array containing two components.
*/
void srelTexCoord2dv(const GLdouble *v)
{
   srel.tex.haveCoord=GL_TRUE;
   srel.tex.cur[srel.vertex.num][0]=v[0];
   srel.tex.cur[srel.vertex.num][1]=v[1];
}
//@}


/**
* @name srelColor
* Specifies the material color.
*/
//@{

/**
* @param red   The red component of a color.
* @param green The green component of a color.
* @param blue  The blue component of a color.
*/
void srelColor3f(GLfloat red,GLfloat green,GLfloat blue)
{
   if(srel.doppler) {
      _RGBA rgba={red,green,blue,1.0f};
      _UpdateMaterial(GL_FRONT_AND_BACK,srel.mat.colorMode,rgba);
   } else {
      glColor3f(red,green,blue);
   }
}
/**
* @param v A pointer to an array containing three color components.
*/
void srelColor3fv(const GLfloat *v)
{
   if(srel.doppler) {
      _RGBA rgba={v[0],v[1],v[2],1.0f};
      _UpdateMaterial(GL_FRONT_AND_BACK,srel.mat.colorMode,rgba);
   } else {
      glColor3fv(v);
   }
}
/**
* @param red   The red component of a color.
* @param green The green component of a color.
* @param blue  The blue component of a color.
* @param alpha The alpha component of a color.
*/
void srelColor4f(GLfloat red,GLfloat green,GLfloat blue,GLfloat alpha)
{
   if(srel.doppler) {
      _RGBA rgba={red,green,blue,alpha};
      _UpdateMaterial(GL_FRONT_AND_BACK,srel.mat.colorMode,rgba);
   } else {
      glColor4f(red,green,blue,alpha);
   }
}
/**
* @param v A pointer to an array containing four color components.
*/
void srelColor4fv(const GLfloat *v)
{
   if(srel.doppler) {
      _UpdateMaterial(GL_FRONT_AND_BACK,srel.mat.colorMode,v);
   } else {
      glColor4fv(v);
   }
}
//@}


/**
* Specifies material parameters.
*/
void srelMaterialfv(GLenum face,GLenum pname,const GLfloat *rgba)
{
   if(srel.doppler) {
      _UpdateMaterial(face,pname,rgba);
   } else {
      glMaterialfv(face,pname,rgba);
   }
}

//@}



#ifdef GL_VERSION_1_1

/*
* Polymorphic srelVertex.
*/
void srelVertexv(GLint size,GLenum type,const GLvoid *v,GLint offset)
{
   if(size==2) {
      if(type==GL_FLOAT)
         srelVertex2fv(&((GLfloat*)v)[offset]);
      else if(type==GL_DOUBLE)
         srelVertex2dv(&((GLdouble*)v)[offset]);
   } else if(size==3) {
      if(type==GL_FLOAT)
         srelVertex3fv(&((GLfloat*)v)[offset]);
      else if(type==GL_DOUBLE)
         srelVertex3dv(&((GLdouble*)v)[offset]);
   } else if(size==4) {
      if(type==GL_FLOAT)
         srelVertex4fv(&((GLfloat*)v)[offset]);
      else if(type==GL_DOUBLE)
         srelVertex4dv(&((GLdouble*)v)[offset]);
   }
}
/*
* Polymorphic srelNormal.
*/
void srelNormalv(GLenum type,const GLvoid *n,GLint offset)
{
   if(type==GL_FLOAT)
      srelNormal3fv(&((GLfloat*)n)[offset]);
   else if(type==GL_DOUBLE)
      srelNormal3dv(&((GLdouble*)n)[offset]);
}
/*
* Polymorphic srelTexCoord.
*/
void srelTexCoordv(GLint size,GLenum type,const GLvoid *v,GLint offset)
{
   if(size==1) {
      if(type==GL_FLOAT)
         srelTexCoord1fv(&((GLfloat*)v)[offset]);
      else if(type==GL_DOUBLE)
         srelTexCoord1dv(&((GLdouble*)v)[offset]);
   } else if(size==2) {
      if(type==GL_FLOAT)
         srelTexCoord2fv(&((GLfloat*)v)[offset]);
      else if(type==GL_DOUBLE)
         srelTexCoord2dv(&((GLdouble*)v)[offset]);
   }
}


/**
* @defgroup api_1_1 API Functions (OpenGL 1.1)
* The SREL OpenGL 1.1 compatible API.
*/
//@{

/**
* Renders array elements.
* @version
* OpenGL 1.1
*/
void srelDrawArrays(GLenum mode,GLint first,GLsizei count)
{
   int i;
   GLint vSize,tSize;
   GLint vType,nType,tType;
   GLvoid *vPtr,*nPtr,*tPtr;
   glGetIntegerv(GL_VERTEX_ARRAY_SIZE,&vSize);
   glGetIntegerv(GL_VERTEX_ARRAY_TYPE,&vType);
   glGetPointerv(GL_VERTEX_ARRAY_POINTER,&vPtr);
   glGetIntegerv(GL_NORMAL_ARRAY_TYPE,&nType);
   glGetPointerv(GL_NORMAL_ARRAY_POINTER,&nPtr);
   glGetIntegerv(GL_TEXTURE_COORD_ARRAY_SIZE,&tSize);
   glGetIntegerv(GL_TEXTURE_COORD_ARRAY_TYPE,&tType);
   glGetPointerv(GL_TEXTURE_COORD_ARRAY_POINTER,&tPtr);
   srelBegin(mode);
   for(i=0;i<count;i++) {
      if(nPtr)
         srelNormalv(nType,nPtr,i);
      if(tPtr)
         srelTexCoordv(tSize,tType,tPtr,i);
      srelVertexv(vSize,vType,vPtr,i);
   }
   srelEnd();
}
/**
* Renders a set of array elements.
* @version
* OpenGL 1.1
*/
void srelDrawElements(GLenum mode,GLsizei count,GLenum type,const GLvoid *indices)
{
   int i;
   GLint vSize,tSize;
   GLint vType,nType,tType;
   GLvoid *vPtr,*nPtr,*tPtr;
   glGetIntegerv(GL_VERTEX_ARRAY_SIZE,&vSize);
   glGetIntegerv(GL_VERTEX_ARRAY_TYPE,&vType);
   glGetPointerv(GL_VERTEX_ARRAY_POINTER,&vPtr);
   glGetIntegerv(GL_NORMAL_ARRAY_TYPE,&nType);
   glGetPointerv(GL_NORMAL_ARRAY_POINTER,&nPtr);
   glGetIntegerv(GL_TEXTURE_COORD_ARRAY_SIZE,&tSize);
   glGetIntegerv(GL_TEXTURE_COORD_ARRAY_TYPE,&tType);
   glGetPointerv(GL_TEXTURE_COORD_ARRAY_POINTER,&tPtr);
   srelBegin(mode);
   if(type==GL_UNSIGNED_BYTE) {
      for(i=0;i<count;i++) {
         if(nPtr)
            srelNormalv(nType,nPtr,((GLubyte*)indices)[i]);
         if(tPtr)
            srelTexCoordv(tSize,tType,tPtr,((GLubyte*)indices)[i]);
         srelVertexv(vSize,vType,vPtr,((GLubyte*)indices)[i]);
      }
   } else if(type==GL_UNSIGNED_SHORT) {
      for(i=0;i<count;i++) {
         if(nPtr)
            srelNormalv(nType,nPtr,((GLushort*)indices)[i]);
         if(tPtr)
            srelTexCoordv(tSize,tType,tPtr,((GLushort*)indices)[i]);
         srelVertexv(vSize,vType,vPtr,((GLushort*)indices)[i]);
      }
   } else if(type==GL_UNSIGNED_INT) {
      for(i=0;i<count;i++) {
         if(nPtr)
            srelNormalv(nType,nPtr,((GLuint*)indices)[i]);
         if(tPtr)
            srelTexCoordv(tSize,tType,tPtr,((GLuint*)indices)[i]);
         srelVertexv(vSize,vType,vPtr,((GLuint*)indices)[i]);
      }
   }
   srelEnd();
}

//@}
#endif
