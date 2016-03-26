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



/*
* This should be large enough for the stack to be considered
* infinite for all practical purposes.
*/
#define SUBDIVIDE_STACK_SIZE 4096

typedef struct subdivideStack {
   _VERTEXCOORD v[SUBDIVIDE_STACK_SIZE];
   _VERTEXCOORD vST[SUBDIVIDE_STACK_SIZE];
   _TEXCOORD tex[SUBDIVIDE_STACK_SIZE];
} _SUBDIVIDESTACK;

static _SUBDIVIDESTACK stack;

/*
* These methods are conservative.
* That is, the stack shrinks as much as it grows
* and is always empty on exit.
*
* Stack layout:
*
* stack[pos] = first vertex
*    .
*    .
*    .
* stack[0]   = last vertex
*/

void _SubdivideLine(GLfloat *const *arg)
{
   _VERTEXCOORD va,vast;
   int pos=1;
   _VertexCopy(stack.v[0],arg[1]);
   _VertexCopy(stack.v[1],arg[0]);
   _VertexCopy(stack.vST[0],arg[3]);
   _VertexCopy(stack.vST[1],arg[2]);
   _TexCopy(stack.tex[0],arg[5]);
   _TexCopy(stack.tex[1],arg[4]);
   while(pos>0) {
      _VertexMean(va,stack.v[pos],stack.v[pos-1]);
      _VertexCopy(vast,va);
      _SpaceTransform(vast);
      if(_VecCos(vast,stack.vST[pos],stack.vST[pos-1])<srel.detail &&
         (pos+1)<SUBDIVIDE_STACK_SIZE) {
         pos++;
         _VertexCopy(stack.v[pos],stack.v[pos-1]);
         _VertexCopy(stack.v[pos-1],va);
         _VertexCopy(stack.vST[pos],stack.vST[pos-1]);
         _VertexCopy(stack.vST[pos-1],vast);
         if(srel.tex.haveCoord) {
            _TexCopy(stack.tex[pos],stack.tex[pos-1]);
            _TexMean(stack.tex[pos-1],stack.tex[pos],stack.tex[pos-2]);
         }
      } else {
         _DrawVertex(stack.vST[pos],stack.tex[pos]);
         pos--;
      }
   }
   _DrawVertex(stack.vST[0],stack.tex[0]);
}

void _SubdivideTriangles(GLfloat *const *arg)
{
   _VERTEXCOORD va,vb,vc,vast,vbst,vcst;
   int pos=2;
   GLboolean noSub01,noSub12,noSub20;
   _VertexCopy(stack.v[0],arg[2]);
   _VertexCopy(stack.v[1],arg[1]);
   _VertexCopy(stack.v[2],arg[0]);
   _VertexCopy(stack.vST[0],arg[5]);
   _VertexCopy(stack.vST[1],arg[4]);
   _VertexCopy(stack.vST[2],arg[3]);
   _TexCopy(stack.tex[0],arg[8]);
   _TexCopy(stack.tex[1],arg[7]);
   _TexCopy(stack.tex[2],arg[6]);
   while(pos>0) {
      noSub01=GL_TRUE;
      noSub12=GL_TRUE;
      noSub20=GL_TRUE;
      _VertexMean(va,stack.v[pos],stack.v[pos-1]);
      _VertexMean(vb,stack.v[pos-1],stack.v[pos-2]);
      _VertexMean(vc,stack.v[pos-2],stack.v[pos]);
      _VertexCopy(vast,va);
      _VertexCopy(vbst,vb);
      _VertexCopy(vcst,vc);
      _SpaceTransform(vast);
      _SpaceTransform(vbst);
      _SpaceTransform(vcst);
      if(_VecCos(vast,stack.vST[pos],stack.vST[pos-1])<srel.detail)
         noSub01=GL_FALSE;
      if(_VecCos(vbst,stack.vST[pos-1],stack.vST[pos-2])<srel.detail)
         noSub12=GL_FALSE;
      if(_VecCos(vcst,stack.vST[pos-2],stack.vST[pos])<srel.detail)
         noSub20=GL_FALSE;
      if((noSub01 && noSub12 && noSub20) || (pos+9)>=SUBDIVIDE_STACK_SIZE) {
         _DrawVertex(stack.vST[pos],stack.tex[pos]);
         _DrawVertex(stack.vST[pos-1],stack.tex[pos-1]);
         _DrawVertex(stack.vST[pos-2],stack.tex[pos-2]);
         pos-=3;
      } else if(noSub12 && noSub20) {
         pos+=3;
         _VertexCopy(stack.v[pos],stack.v[pos-3]);
         _VertexCopy(stack.v[pos-1],va);
         _VertexCopy(stack.v[pos-2],stack.v[pos-5]);
         _VertexCopy(stack.v[pos-3],stack.v[pos-4]);
         _VertexCopy(stack.v[pos-4],stack.v[pos-5]);
         _VertexCopy(stack.v[pos-5],va);
         _VertexCopy(stack.vST[pos],stack.vST[pos-3]);
         _VertexCopy(stack.vST[pos-1],vast);
         _VertexCopy(stack.vST[pos-2],stack.vST[pos-5]);
         _VertexCopy(stack.vST[pos-3],stack.vST[pos-4]);
         _VertexCopy(stack.vST[pos-4],stack.vST[pos-5]);
         _VertexCopy(stack.vST[pos-5],vast);
         if(srel.tex.haveCoord) {
            _TEXCOORD ta;
            _TexMean(ta,stack.tex[pos-3],stack.tex[pos-4]);
            _TexCopy(stack.tex[pos],stack.tex[pos-3]);
            _TexCopy(stack.tex[pos-1],ta);
            _TexCopy(stack.tex[pos-2],stack.tex[pos-5]);
            _TexCopy(stack.tex[pos-3],stack.tex[pos-4]);
            _TexCopy(stack.tex[pos-4],stack.tex[pos-5]);
            _TexCopy(stack.tex[pos-5],ta);
         }
      } else if(noSub01 && noSub20) {
         pos+=3;
         _VertexCopy(stack.v[pos],stack.v[pos-3]);
         _VertexCopy(stack.v[pos-1],stack.v[pos-4]);
         _VertexCopy(stack.v[pos-2],vb);
         _VertexCopy(stack.v[pos-3],stack.v[pos-5]);
         _VertexCopy(stack.v[pos-4],stack.v[pos]);
         _VertexCopy(stack.v[pos-5],vb);
         _VertexCopy(stack.vST[pos],stack.vST[pos-3]);
         _VertexCopy(stack.vST[pos-1],stack.vST[pos-4]);
         _VertexCopy(stack.vST[pos-2],vbst);
         _VertexCopy(stack.vST[pos-3],stack.vST[pos-5]);
         _VertexCopy(stack.vST[pos-4],stack.vST[pos]);
         _VertexCopy(stack.vST[pos-5],vbst);
         if(srel.tex.haveCoord) {
            _TEXCOORD tb;
            _TexMean(tb,stack.tex[pos-4],stack.tex[pos-5]);
            _TexCopy(stack.tex[pos],stack.tex[pos-3]);
            _TexCopy(stack.tex[pos-1],stack.tex[pos-4]);
            _TexCopy(stack.tex[pos-2],tb);
            _TexCopy(stack.tex[pos-3],stack.tex[pos-5]);
            _TexCopy(stack.tex[pos-4],stack.tex[pos]);
            _TexCopy(stack.tex[pos-5],tb);
         }
      } else if(noSub01 && noSub12) {
         pos+=3;
         _VertexCopy(stack.v[pos],stack.v[pos-3]);
         _VertexCopy(stack.v[pos-1],stack.v[pos-4]);
         _VertexCopy(stack.v[pos-2],vc);
         _VertexCopy(stack.v[pos-3],stack.v[pos-4]);
         _VertexCopy(stack.v[pos-4],stack.v[pos-5]);
         _VertexCopy(stack.v[pos-5],vc);
         _VertexCopy(stack.vST[pos],stack.vST[pos-3]);
         _VertexCopy(stack.vST[pos-1],stack.vST[pos-4]);
         _VertexCopy(stack.vST[pos-2],vcst);
         _VertexCopy(stack.vST[pos-3],stack.vST[pos-4]);
         _VertexCopy(stack.vST[pos-4],stack.vST[pos-5]);
         _VertexCopy(stack.vST[pos-5],vcst);
         if(srel.tex.haveCoord) {
            _TEXCOORD tc;
            _TexMean(tc,stack.tex[pos-5],stack.tex[pos-3]);
            _TexCopy(stack.tex[pos],stack.tex[pos-3]);
            _TexCopy(stack.tex[pos-1],stack.tex[pos-4]);
            _TexCopy(stack.tex[pos-2],tc);
            _TexCopy(stack.tex[pos-3],stack.tex[pos-4]);
            _TexCopy(stack.tex[pos-4],stack.tex[pos-5]);
            _TexCopy(stack.tex[pos-5],tc);
         }
      } else {
         pos+=9;
         _VertexCopy(stack.v[pos],stack.v[pos-9]);
         _VertexCopy(stack.v[pos-1],va);
         _VertexCopy(stack.v[pos-2],vc);
         _VertexCopy(stack.v[pos-3],stack.v[pos-10]);
         _VertexCopy(stack.v[pos-4],vb);
         _VertexCopy(stack.v[pos-5],va);
         _VertexCopy(stack.v[pos-6],stack.v[pos-11]);
         _VertexCopy(stack.v[pos-7],vc);
         _VertexCopy(stack.v[pos-8],vb);
         _VertexCopy(stack.v[pos-9],va);
         _VertexCopy(stack.v[pos-10],vb);
         _VertexCopy(stack.v[pos-11],vc);
         _VertexCopy(stack.vST[pos],stack.vST[pos-9]);
         _VertexCopy(stack.vST[pos-1],vast);
         _VertexCopy(stack.vST[pos-2],vcst);
         _VertexCopy(stack.vST[pos-3],stack.vST[pos-10]);
         _VertexCopy(stack.vST[pos-4],vbst);
         _VertexCopy(stack.vST[pos-5],vast);
         _VertexCopy(stack.vST[pos-6],stack.vST[pos-11]);
         _VertexCopy(stack.vST[pos-7],vcst);
         _VertexCopy(stack.vST[pos-8],vbst);
         _VertexCopy(stack.vST[pos-9],vast);
         _VertexCopy(stack.vST[pos-10],vbst);
         _VertexCopy(stack.vST[pos-11],vcst);
         if(srel.tex.haveCoord) {
            _TEXCOORD ta,tb,tc;
            _TexMean(ta,stack.tex[pos-9],stack.tex[pos-10]);
            _TexMean(tb,stack.tex[pos-10],stack.tex[pos-11]);
            _TexMean(tc,stack.tex[pos-11],stack.tex[pos-9]);
            _TexCopy(stack.tex[pos],stack.tex[pos-9]);
            _TexCopy(stack.tex[pos-1],ta);
            _TexCopy(stack.tex[pos-2],tc);
            _TexCopy(stack.tex[pos-3],stack.tex[pos-10]);
            _TexCopy(stack.tex[pos-4],tb);
            _TexCopy(stack.tex[pos-5],ta);
            _TexCopy(stack.tex[pos-6],stack.tex[pos-11]);
            _TexCopy(stack.tex[pos-7],tc);
            _TexCopy(stack.tex[pos-8],tb);
            _TexCopy(stack.tex[pos-9],ta);
            _TexCopy(stack.tex[pos-10],tb);
            _TexCopy(stack.tex[pos-11],tc);
         }
      }
   }
}

void _SubdivideTriangleFan(GLfloat *const *arg)
{
   _VERTEXCOORD va,vast;
   int pos=1;
   _VertexCopy(stack.v[0],arg[2]);
   _VertexCopy(stack.v[1],arg[1]);
   _VertexCopy(stack.vST[0],arg[5]);
   _VertexCopy(stack.vST[1],arg[4]);
   _TexCopy(stack.tex[0],arg[8]);
   _TexCopy(stack.tex[1],arg[7]);
   while(pos>0) {
      _VertexMean(va,stack.v[pos],stack.v[pos-1]);
      _VertexCopy(vast,va);
      _SpaceTransform(vast);
      if(_VecCos(vast,stack.vST[pos],stack.vST[pos-1])<srel.detail &&
         (pos+1)<SUBDIVIDE_STACK_SIZE) {
         pos++;
         _VertexCopy(stack.v[pos],stack.v[pos-1]);
         _VertexCopy(stack.v[pos-1],va);
         _VertexCopy(stack.vST[pos],stack.vST[pos-1]);
         _VertexCopy(stack.vST[pos-1],vast);
         if(srel.tex.haveCoord) {
            _TexCopy(stack.tex[pos],stack.tex[pos-1]);
            _TexMean(stack.tex[pos-1],stack.tex[pos],stack.tex[pos-2]);
         }
      } else {
         _DrawVertex(stack.vST[pos],stack.tex[pos]);
         pos--;
      }
   }
   _DrawVertex(stack.vST[0],stack.tex[0]);
}

void _SubdivideQuads(GLfloat *const *arg)
{
   _VERTEXCOORD va,vb,vc,vd,vast,vbst,vcst,vdst;
   int pos=3;
   GLboolean noSub01,noSub12,noSub23,noSub30;
   _VertexCopy(stack.v[0],arg[3]);
   _VertexCopy(stack.v[1],arg[2]);
   _VertexCopy(stack.v[2],arg[1]);
   _VertexCopy(stack.v[3],arg[0]);
   _VertexCopy(stack.vST[0],arg[7]);
   _VertexCopy(stack.vST[1],arg[6]);
   _VertexCopy(stack.vST[2],arg[5]);
   _VertexCopy(stack.vST[3],arg[4]);
   _TexCopy(stack.tex[0],arg[11]);
   _TexCopy(stack.tex[1],arg[10]);
   _TexCopy(stack.tex[2],arg[9]);
   _TexCopy(stack.tex[3],arg[8]);
   while(pos>0) {
      noSub01=GL_TRUE;
      noSub12=GL_TRUE;
      noSub23=GL_TRUE;
      noSub30=GL_TRUE;
      _VertexMean(va,stack.v[pos],stack.v[pos-1]);
      _VertexMean(vb,stack.v[pos-1],stack.v[pos-2]);
      _VertexMean(vc,stack.v[pos-2],stack.v[pos-3]);
      _VertexMean(vd,stack.v[pos-3],stack.v[pos]);
      _VertexCopy(vast,va);
      _VertexCopy(vbst,vb);
      _VertexCopy(vcst,vc);
      _VertexCopy(vdst,vd);
      _SpaceTransform(vast);
      _SpaceTransform(vbst);
      _SpaceTransform(vcst);
      _SpaceTransform(vdst);
      if(_VecCos(vast,stack.vST[pos],stack.vST[pos-1])<srel.detail)
         noSub01=GL_FALSE;
      if(_VecCos(vbst,stack.vST[pos-1],stack.vST[pos-2])<srel.detail)
         noSub12=GL_FALSE;
      if(_VecCos(vcst,stack.vST[pos-2],stack.vST[pos-3])<srel.detail)
         noSub23=GL_FALSE;
      if(_VecCos(vdst,stack.vST[pos-3],stack.vST[pos])<srel.detail)
         noSub30=GL_FALSE;
      if((noSub01 && noSub12 && noSub23 && noSub30) ||
         (pos+12)>=SUBDIVIDE_STACK_SIZE) {
         _DrawVertex(stack.vST[pos],stack.tex[pos]);
         _DrawVertex(stack.vST[pos-1],stack.tex[pos-1]);
         _DrawVertex(stack.vST[pos-2],stack.tex[pos-2]);
         _DrawVertex(stack.vST[pos-3],stack.tex[pos-3]);
         pos-=4;
      } else if(noSub01 && noSub23) {
         pos+=4;
         _VertexCopy(stack.v[pos],stack.v[pos-4]);
         _VertexCopy(stack.v[pos-1],stack.v[pos-5]);
         _VertexCopy(stack.v[pos-2],vb);
         _VertexCopy(stack.v[pos-3],vd);
         _VertexCopy(stack.v[pos-4],stack.v[pos-6]);
         _VertexCopy(stack.v[pos-5],stack.v[pos-7]);
         _VertexCopy(stack.v[pos-6],vd);
         _VertexCopy(stack.v[pos-7],vb);
         _VertexCopy(stack.vST[pos],stack.vST[pos-4]);
         _VertexCopy(stack.vST[pos-1],stack.vST[pos-5]);
         _VertexCopy(stack.vST[pos-2],vbst);
         _VertexCopy(stack.vST[pos-3],vdst);
         _VertexCopy(stack.vST[pos-4],stack.vST[pos-6]);
         _VertexCopy(stack.vST[pos-5],stack.vST[pos-7]);
         _VertexCopy(stack.vST[pos-6],vdst);
         _VertexCopy(stack.vST[pos-7],vbst);
         if(srel.tex.haveCoord) {
            _TEXCOORD tb,td;
            _TexMean(tb,stack.tex[pos-5],stack.tex[pos-6]);
            _TexMean(td,stack.tex[pos-7],stack.tex[pos-4]);
            _TexCopy(stack.tex[pos],stack.tex[pos-4]);
            _TexCopy(stack.tex[pos-1],stack.tex[pos-5]);
            _TexCopy(stack.tex[pos-2],tb);
            _TexCopy(stack.tex[pos-3],td);
            _TexCopy(stack.tex[pos-4],stack.tex[pos-6]);
            _TexCopy(stack.tex[pos-5],stack.tex[pos-7]);
            _TexCopy(stack.tex[pos-6],td);
            _TexCopy(stack.tex[pos-7],tb);
         }
      } else if(noSub12 && noSub30) {
         pos+=4;
         _VertexCopy(stack.v[pos],stack.v[pos-5]);
         _VertexCopy(stack.v[pos-1],stack.v[pos-6]);
         _VertexCopy(stack.v[pos-2],vc);
         _VertexCopy(stack.v[pos-3],va);
         _VertexCopy(stack.v[pos-5],stack.v[pos-4]);
         _VertexCopy(stack.v[pos-4],stack.v[pos-7]);
         _VertexCopy(stack.v[pos-6],va);
         _VertexCopy(stack.v[pos-7],vc);
         _VertexCopy(stack.vST[pos],stack.vST[pos-5]);
         _VertexCopy(stack.vST[pos-1],stack.vST[pos-6]);
         _VertexCopy(stack.vST[pos-2],vcst);
         _VertexCopy(stack.vST[pos-3],vast);
         _VertexCopy(stack.vST[pos-5],stack.vST[pos-4]);
         _VertexCopy(stack.vST[pos-4],stack.vST[pos-7]);
         _VertexCopy(stack.vST[pos-6],vast);
         _VertexCopy(stack.vST[pos-7],vcst);
         if(srel.tex.haveCoord) {
            _TEXCOORD ta,tc;
            _TexMean(ta,stack.tex[pos-4],stack.tex[pos-5]);
            _TexMean(tc,stack.tex[pos-6],stack.tex[pos-7]);
            _TexCopy(stack.tex[pos],stack.tex[pos-5]);
            _TexCopy(stack.tex[pos-1],stack.tex[pos-6]);
            _TexCopy(stack.tex[pos-2],tc);
            _TexCopy(stack.tex[pos-3],ta);
            _TexCopy(stack.tex[pos-5],stack.tex[pos-4]);
            _TexCopy(stack.tex[pos-4],stack.tex[pos-7]);
            _TexCopy(stack.tex[pos-6],ta);
            _TexCopy(stack.tex[pos-7],tc);
         }
      } else {
         _VERTEXCOORD vx,vxst;
         pos+=12;
         _VertexQuadMean(vx,stack.v[pos-12],stack.v[pos-13],stack.v[pos-14],stack.v[pos-15]);
         _VertexCopy(vxst,vx);
         _SpaceTransform(vxst);
         _VertexCopy(stack.v[pos],stack.v[pos-12]);
         _VertexCopy(stack.v[pos-1],va);
         _VertexCopy(stack.v[pos-2],vx);
         _VertexCopy(stack.v[pos-3],vd);
         _VertexCopy(stack.v[pos-4],stack.v[pos-13]);
         _VertexCopy(stack.v[pos-5],vb);
         _VertexCopy(stack.v[pos-6],vx);
         _VertexCopy(stack.v[pos-7],va);
         _VertexCopy(stack.v[pos-8],stack.v[pos-14]);
         _VertexCopy(stack.v[pos-9],vc);
         _VertexCopy(stack.v[pos-10],vx);
         _VertexCopy(stack.v[pos-11],vb);
         _VertexCopy(stack.v[pos-12],stack.v[pos-15]);
         _VertexCopy(stack.v[pos-13],vd);
         _VertexCopy(stack.v[pos-14],vx);
         _VertexCopy(stack.v[pos-15],vc);
         _VertexCopy(stack.vST[pos],stack.vST[pos-12]);
         _VertexCopy(stack.vST[pos-1],vast);
         _VertexCopy(stack.vST[pos-2],vxst);
         _VertexCopy(stack.vST[pos-3],vdst);
         _VertexCopy(stack.vST[pos-4],stack.vST[pos-13]);
         _VertexCopy(stack.vST[pos-5],vbst);
         _VertexCopy(stack.vST[pos-6],vxst);
         _VertexCopy(stack.vST[pos-7],vast);
         _VertexCopy(stack.vST[pos-8],stack.vST[pos-14]);
         _VertexCopy(stack.vST[pos-9],vcst);
         _VertexCopy(stack.vST[pos-10],vxst);
         _VertexCopy(stack.vST[pos-11],vbst);
         _VertexCopy(stack.vST[pos-12],stack.vST[pos-15]);
         _VertexCopy(stack.vST[pos-13],vdst);
         _VertexCopy(stack.vST[pos-14],vxst);
         _VertexCopy(stack.vST[pos-15],vcst);
         if(srel.tex.haveCoord) {
            _TEXCOORD ta,tb,tc,td,tx;
            _TexMean(ta,stack.tex[pos-12],stack.tex[pos-13]);
            _TexMean(tb,stack.tex[pos-13],stack.tex[pos-14]);
            _TexMean(tc,stack.tex[pos-14],stack.tex[pos-15]);
            _TexMean(td,stack.tex[pos-15],stack.tex[pos-12]);
            _TexQuadMean(tx,stack.tex[pos-12],stack.tex[pos-13],stack.tex[pos-14],stack.tex[pos-15]);
            _TexCopy(stack.tex[pos],stack.tex[pos-12]);
            _TexCopy(stack.tex[pos-1],ta);
            _TexCopy(stack.tex[pos-2],tx);
            _TexCopy(stack.tex[pos-3],td);
            _TexCopy(stack.tex[pos-4],stack.tex[pos-13]);
            _TexCopy(stack.tex[pos-5],tb);
            _TexCopy(stack.tex[pos-6],tx);
            _TexCopy(stack.tex[pos-7],ta);
            _TexCopy(stack.tex[pos-8],stack.tex[pos-14]);
            _TexCopy(stack.tex[pos-9],tc);
            _TexCopy(stack.tex[pos-10],tx);
            _TexCopy(stack.tex[pos-11],tb);
            _TexCopy(stack.tex[pos-12],stack.tex[pos-15]);
            _TexCopy(stack.tex[pos-13],td);
            _TexCopy(stack.tex[pos-14],tx);
            _TexCopy(stack.tex[pos-15],tc);
         }
      }
   }
}

void _SubdivideQuadStrip(GLfloat *const *arg)
{
   _VERTEXCOORD va,vb,vast,vbst;
   int pos=3;
   _VertexCopy(stack.v[0],arg[3]);
   _VertexCopy(stack.v[1],arg[2]);
   _VertexCopy(stack.v[2],arg[1]);
   _VertexCopy(stack.v[3],arg[0]);
   _VertexCopy(stack.vST[0],arg[7]);
   _VertexCopy(stack.vST[1],arg[6]);
   _VertexCopy(stack.vST[2],arg[5]);
   _VertexCopy(stack.vST[3],arg[4]);
   _TexCopy(stack.tex[0],arg[11]);
   _TexCopy(stack.tex[1],arg[10]);
   _TexCopy(stack.tex[2],arg[9]);
   _TexCopy(stack.tex[3],arg[8]);
   while(pos>2) {
      _VertexMean(va,stack.v[pos],stack.v[pos-2]);
      _VertexMean(vb,stack.v[pos-1],stack.v[pos-3]);
      _VertexCopy(vast,va);
      _VertexCopy(vbst,vb);
      _SpaceTransform(vast);
      _SpaceTransform(vbst);
      if((_VecCos(vast,stack.vST[pos],stack.vST[pos-2])<srel.detail ||
         _VecCos(vbst,stack.vST[pos-1],stack.vST[pos-3])<srel.detail) &&
         (pos+2)<SUBDIVIDE_STACK_SIZE) {
         pos+=2;
         _VertexCopy(stack.v[pos],stack.v[pos-2]);
         _VertexCopy(stack.v[pos-1],stack.v[pos-3]);
         _VertexCopy(stack.v[pos-2],va);
         _VertexCopy(stack.v[pos-3],vb);
         _VertexCopy(stack.vST[pos],stack.vST[pos-2]);
         _VertexCopy(stack.vST[pos-1],stack.vST[pos-3]);
         _VertexCopy(stack.vST[pos-2],vast);
         _VertexCopy(stack.vST[pos-3],vbst);
         if(srel.tex.haveCoord) {
            _TexCopy(stack.tex[pos],stack.tex[pos-2]);
            _TexCopy(stack.tex[pos-1],stack.tex[pos-3]);
            _TexMean(stack.tex[pos-2],stack.tex[pos],stack.tex[pos-4]);
            _TexMean(stack.tex[pos-3],stack.tex[pos-1],stack.tex[pos-5]);
         }
      } else {
         _DrawVertex(stack.vST[pos],stack.tex[pos]);
         _DrawVertex(stack.vST[pos-1],stack.tex[pos-1]);
         pos-=2;
      }
   }
   _DrawVertex(stack.vST[1],stack.tex[1]);
   _DrawVertex(stack.vST[0],stack.tex[0]);
}
