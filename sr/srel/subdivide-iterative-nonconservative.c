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
   int order[SUBDIVIDE_STACK_SIZE];         // vertex reverse order
   _VERTEXCOORD v[SUBDIVIDE_STACK_SIZE];    // vertices
   _VERTEXCOORD vST[SUBDIVIDE_STACK_SIZE];  // space transformed vertices
   _TEXCOORD tex[SUBDIVIDE_STACK_SIZE];     // texture coordinates
} _SUBDIVIDESTACK;

static _SUBDIVIDESTACK stack;

/* stack helper macros */
#define getVertex(pos)   stack.v[stack.order[pos]]
#define getVertexST(pos) stack.vST[stack.order[pos]]
#define getTex(pos)      stack.tex[stack.order[pos]]

/*
* These methods are NOT conservative.
* The stack always grows.
* The only exception is the order[] stack, which is conservative.
*/

void _SubdivideLine(GLfloat *const *arg)
{
   _VERTEXCOORD va,vast;
   int pos=1,vpos=1;
   _VertexCopy(stack.v[0],arg[0]);
   _VertexCopy(stack.v[1],arg[1]);
   _VertexCopy(stack.vST[0],arg[2]);
   _VertexCopy(stack.vST[1],arg[3]);
   _TexCopy(stack.tex[0],arg[4]);
   _TexCopy(stack.tex[1],arg[5]);
   stack.order[0]=1;
   stack.order[1]=0;
   while(pos>0) {
      _VertexMean(va,getVertex(pos),getVertex(pos-1));
      _VertexCopy(vast,va);
      _SpaceTransform(vast);
      if(_VecCos(vast,getVertexST(pos),getVertexST(pos-1))<srel.detail &&
         (vpos+1)<SUBDIVIDE_STACK_SIZE) {
         pos++;
         vpos++;
         _VertexCopy(stack.v[vpos],va);
         _VertexCopy(stack.vST[vpos],vast);
         if(srel.tex.haveCoord)
            _TexMean(stack.tex[vpos],getTex(pos-1),getTex(pos-2));
         stack.order[pos]=stack.order[pos-1];
         stack.order[pos-1]=vpos;
      } else {
         _DrawVertex(getVertexST(pos),getTex(pos));
         pos--;
      }
   }
   _DrawVertex(getVertexST(0),getTex(0));
}

void _SubdivideTriangles(GLfloat *const *arg)
{
   _VERTEXCOORD va,vb,vc,vast,vbst,vcst;
   int pos=2,vpos=2;
   GLboolean noSub01,noSub12,noSub20;
   _VertexCopy(stack.v[0],arg[0]);
   _VertexCopy(stack.v[1],arg[1]);
   _VertexCopy(stack.v[2],arg[2]);
   _VertexCopy(stack.vST[0],arg[3]);
   _VertexCopy(stack.vST[1],arg[4]);
   _VertexCopy(stack.vST[2],arg[5]);
   _TexCopy(stack.tex[0],arg[6]);
   _TexCopy(stack.tex[1],arg[7]);
   _TexCopy(stack.tex[2],arg[8]);
   stack.order[0]=2;
   stack.order[1]=1;
   stack.order[2]=0;
   while(pos>0) {
      noSub01=GL_TRUE;
      noSub12=GL_TRUE;
      noSub20=GL_TRUE;
      _VertexMean(va,getVertex(pos),getVertex(pos-1));
      _VertexMean(vb,getVertex(pos-1),getVertex(pos-2));
      _VertexMean(vc,getVertex(pos-2),getVertex(pos));
      _VertexCopy(vast,va);
      _VertexCopy(vbst,vb);
      _VertexCopy(vcst,vc);
      _SpaceTransform(vast);
      _SpaceTransform(vbst);
      _SpaceTransform(vcst);
      if(_VecCos(vast,getVertexST(pos),getVertexST(pos-1))<srel.detail)
         noSub01=GL_FALSE;
      if(_VecCos(vbst,getVertexST(pos-1),getVertexST(pos-2))<srel.detail)
         noSub12=GL_FALSE;
      if(_VecCos(vcst,getVertexST(pos-2),getVertexST(pos))<srel.detail)
         noSub20=GL_FALSE;
      if((noSub01 && noSub12 && noSub20) ||
         (pos+9)>=SUBDIVIDE_STACK_SIZE || (vpos+3)>=SUBDIVIDE_STACK_SIZE) {
         _DrawVertex(getVertexST(pos),getTex(pos));
         _DrawVertex(getVertexST(pos-1),getTex(pos-1));
         _DrawVertex(getVertexST(pos-2),getTex(pos-2));
         pos-=3;
      } else if(noSub12 && noSub20) {
         pos+=3;
         vpos++;
         _VertexCopy(stack.v[vpos],va);
         _VertexCopy(stack.vST[vpos],vast);
         if(srel.tex.haveCoord)
            _TexMean(stack.tex[vpos],getTex(pos-3),getTex(pos-4));
         stack.order[pos]=stack.order[pos-3];
         stack.order[pos-1]=vpos;
         stack.order[pos-2]=stack.order[pos-5];
         stack.order[pos-3]=stack.order[pos-4];
         stack.order[pos-4]=stack.order[pos-5];
         stack.order[pos-5]=vpos;
      } else if(noSub01 && noSub20) {
         pos+=3;
         vpos++;
         _VertexCopy(stack.v[vpos],vb);
         _VertexCopy(stack.vST[vpos],vbst);
         if(srel.tex.haveCoord)
            _TexMean(stack.tex[vpos],getTex(pos-4),getTex(pos-5));
         stack.order[pos]=stack.order[pos-3];
         stack.order[pos-1]=stack.order[pos-4];
         stack.order[pos-2]=vpos;
         stack.order[pos-3]=stack.order[pos-5];
         stack.order[pos-4]=stack.order[pos];
         stack.order[pos-5]=vpos;
      } else if(noSub01 && noSub12) {
         pos+=3;
         vpos++;
         _VertexCopy(stack.v[vpos],vc);
         _VertexCopy(stack.vST[vpos],vcst);
         if(srel.tex.haveCoord)
            _TexMean(stack.tex[vpos],getTex(pos-5),getTex(pos-3));
         stack.order[pos]=stack.order[pos-3];
         stack.order[pos-1]=stack.order[pos-4];
         stack.order[pos-2]=vpos;
         stack.order[pos-3]=stack.order[pos-4];
         stack.order[pos-4]=stack.order[pos-5];
         stack.order[pos-5]=vpos;
      } else {
         pos+=9;
         vpos+=3;
         _VertexCopy(stack.v[vpos],va);
         _VertexCopy(stack.v[vpos-1],vb);
         _VertexCopy(stack.v[vpos-2],vc);
         _VertexCopy(stack.vST[vpos],vast);
         _VertexCopy(stack.vST[vpos-1],vbst);
         _VertexCopy(stack.vST[vpos-2],vcst);
         if(srel.tex.haveCoord) {
            _TexMean(stack.tex[vpos],getTex(pos-9),getTex(pos-10));
            _TexMean(stack.tex[vpos-1],getTex(pos-10),getTex(pos-11));
            _TexMean(stack.tex[vpos-2],getTex(pos-11),getTex(pos-9));
         }
         stack.order[pos]=stack.order[pos-9];
         stack.order[pos-1]=vpos;
         stack.order[pos-2]=vpos-2;
         stack.order[pos-3]=stack.order[pos-10];
         stack.order[pos-4]=vpos-1;
         stack.order[pos-5]=vpos;
         stack.order[pos-6]=stack.order[pos-11];
         stack.order[pos-7]=vpos-2;
         stack.order[pos-8]=vpos-1;
         stack.order[pos-9]=vpos;
         stack.order[pos-10]=vpos-1;
         stack.order[pos-11]=vpos-2;
      }
   }
}

void _SubdivideTriangleFan(GLfloat *const *arg)
{
   _VERTEXCOORD va,vast;
   int pos=1,vpos=1;
   _VertexCopy(stack.v[0],arg[1]);
   _VertexCopy(stack.v[1],arg[2]);
   _VertexCopy(stack.vST[0],arg[4]);
   _VertexCopy(stack.vST[1],arg[5]);
   _TexCopy(stack.tex[0],arg[7]);
   _TexCopy(stack.tex[1],arg[8]);
   stack.order[0]=1;
   stack.order[1]=0;
   while(pos>0) {
      _VertexMean(va,getVertex(pos),getVertex(pos-1));
      _VertexCopy(vast,va);
      _SpaceTransform(vast);
      if(_VecCos(vast,getVertexST(pos),getVertexST(pos-1))<srel.detail &&
         (vpos+1)<SUBDIVIDE_STACK_SIZE) {
         pos++;
         vpos++;
         _VertexCopy(stack.v[vpos],va);
         _VertexCopy(stack.vST[vpos],vast);
         if(srel.tex.haveCoord)
            _TexMean(stack.tex[vpos],getTex(pos-1),getTex(pos-2));
         stack.order[pos]=stack.order[pos-1];
         stack.order[pos-1]=vpos;
      } else {
         _DrawVertex(getVertexST(pos),getTex(pos));
         pos--;
      }
   }
   _DrawVertex(getVertexST(0),getTex(0));
}

void _SubdivideQuads(GLfloat *const *arg)
{
   _VERTEXCOORD va,vb,vc,vd,vast,vbst,vcst,vdst;
   int pos=3,vpos=3;
   GLboolean noSub01,noSub12,noSub23,noSub30;
   _VertexCopy(stack.v[0],arg[0]);
   _VertexCopy(stack.v[1],arg[1]);
   _VertexCopy(stack.v[2],arg[2]);
   _VertexCopy(stack.v[3],arg[3]);
   _VertexCopy(stack.vST[0],arg[4]);
   _VertexCopy(stack.vST[1],arg[5]);
   _VertexCopy(stack.vST[2],arg[6]);
   _VertexCopy(stack.vST[3],arg[7]);
   _TexCopy(stack.tex[0],arg[8]);
   _TexCopy(stack.tex[1],arg[9]);
   _TexCopy(stack.tex[2],arg[10]);
   _TexCopy(stack.tex[3],arg[11]);
   stack.order[0]=3;
   stack.order[1]=2;
   stack.order[2]=1;
   stack.order[3]=0;
   while(pos>0) {
      noSub01=GL_TRUE;
      noSub12=GL_TRUE;
      noSub23=GL_TRUE;
      noSub30=GL_TRUE;
      _VertexMean(va,getVertex(pos),getVertex(pos-1));
      _VertexMean(vb,getVertex(pos-1),getVertex(pos-2));
      _VertexMean(vc,getVertex(pos-2),getVertex(pos-3));
      _VertexMean(vd,getVertex(pos-3),getVertex(pos));
      _VertexCopy(vast,va);
      _VertexCopy(vbst,vb);
      _VertexCopy(vcst,vc);
      _VertexCopy(vdst,vd);
      _SpaceTransform(vast);
      _SpaceTransform(vbst);
      _SpaceTransform(vcst);
      _SpaceTransform(vdst);
      if(_VecCos(vast,getVertexST(pos),getVertexST(pos-1))<srel.detail)
         noSub01=GL_FALSE;
      if(_VecCos(vbst,getVertexST(pos-1),getVertexST(pos-2))<srel.detail)
         noSub12=GL_FALSE;
      if(_VecCos(vcst,getVertexST(pos-2),getVertexST(pos-3))<srel.detail)
         noSub23=GL_FALSE;
      if(_VecCos(vdst,getVertexST(pos-3),getVertexST(pos))<srel.detail)
         noSub30=GL_FALSE;
      if((noSub01 && noSub12 && noSub23 && noSub30) ||
         (pos+12)>=SUBDIVIDE_STACK_SIZE || (vpos+5)>=SUBDIVIDE_STACK_SIZE) {
         _DrawVertex(getVertexST(pos),getTex(pos));
         _DrawVertex(getVertexST(pos-1),getTex(pos-1));
         _DrawVertex(getVertexST(pos-2),getTex(pos-2));
         _DrawVertex(getVertexST(pos-3),getTex(pos-3));
         pos-=4;
      } else if(noSub01 && noSub23) {
         pos+=4;
         vpos+=2;
         _VertexCopy(stack.v[vpos],vb);
         _VertexCopy(stack.v[vpos-1],vd);
         _VertexCopy(stack.vST[vpos],vbst);
         _VertexCopy(stack.vST[vpos-1],vdst);
         if(srel.tex.haveCoord) {
            _TexMean(stack.tex[vpos],getTex(pos-5),getTex(pos-6));
            _TexMean(stack.tex[vpos-1],getTex(pos-7),getTex(pos-4));
         }
         stack.order[pos]=stack.order[pos-4];
         stack.order[pos-1]=stack.order[pos-5];
         stack.order[pos-2]=vpos;
         stack.order[pos-3]=vpos-1;
         stack.order[pos-4]=stack.order[pos-6];
         stack.order[pos-5]=stack.order[pos-7];
         stack.order[pos-6]=vpos-1;
         stack.order[pos-7]=vpos;
      } else if(noSub12 && noSub30) {
         pos+=4;
         vpos+=2;
         _VertexCopy(stack.v[vpos],va);
         _VertexCopy(stack.v[vpos-1],vc);
         _VertexCopy(stack.vST[vpos],vast);
         _VertexCopy(stack.vST[vpos-1],vcst);
         if(srel.tex.haveCoord) {
            _TexMean(stack.tex[vpos],getTex(pos-4),getTex(pos-5));
            _TexMean(stack.tex[vpos-1],getTex(pos-6),getTex(pos-7));
         }
         stack.order[pos]=stack.order[pos-5];
         stack.order[pos-1]=stack.order[pos-6];
         stack.order[pos-2]=vpos-1;
         stack.order[pos-3]=vpos;
         stack.order[pos-5]=stack.order[pos-4];
         stack.order[pos-4]=stack.order[pos-7];
         stack.order[pos-6]=vpos;
         stack.order[pos-7]=vpos-1;
      } else {
         _VERTEXCOORD vx,vxst;
         pos+=12;
         vpos+=5;
         _VertexQuadMean(vx,getVertex(pos-12),getVertex(pos-13),getVertex(pos-14),getVertex(pos-15));
         _VertexCopy(vxst,vx);
         _SpaceTransform(vxst);
         _VertexCopy(stack.v[vpos],va);
         _VertexCopy(stack.v[vpos-1],vb);
         _VertexCopy(stack.v[vpos-2],vc);
         _VertexCopy(stack.v[vpos-3],vd);
         _VertexCopy(stack.v[vpos-4],vx);
         _VertexCopy(stack.vST[vpos],vast);
         _VertexCopy(stack.vST[vpos-1],vbst);
         _VertexCopy(stack.vST[vpos-2],vcst);
         _VertexCopy(stack.vST[vpos-3],vdst);
         _VertexCopy(stack.vST[vpos-4],vxst);
         if(srel.tex.haveCoord) {
            _TexMean(stack.tex[vpos],getTex(pos-12),getTex(pos-13));
            _TexMean(stack.tex[vpos-1],getTex(pos-13),getTex(pos-14));
            _TexMean(stack.tex[vpos-2],getTex(pos-14),getTex(pos-15));
            _TexMean(stack.tex[vpos-3],getTex(pos-15),getTex(pos-12));
            _TexQuadMean(stack.tex[vpos-4],getTex(pos-12),getTex(pos-13),getTex(pos-14),getTex(pos-15));
         }
         stack.order[pos]=stack.order[pos-12];
         stack.order[pos-1]=vpos;
         stack.order[pos-2]=vpos-4;
         stack.order[pos-3]=vpos-3;
         stack.order[pos-4]=stack.order[pos-13];
         stack.order[pos-5]=vpos-1;
         stack.order[pos-6]=vpos-4;
         stack.order[pos-7]=vpos;
         stack.order[pos-8]=stack.order[pos-14];
         stack.order[pos-9]=vpos-2;
         stack.order[pos-10]=vpos-4;
         stack.order[pos-11]=vpos-1;
         stack.order[pos-12]=stack.order[pos-15];
         stack.order[pos-13]=vpos-3;
         stack.order[pos-14]=vpos-4;
         stack.order[pos-15]=vpos-2;
      }
   }
}

void _SubdivideQuadStrip(GLfloat *const *arg)
{
   _VERTEXCOORD va,vb,vast,vbst;
   int pos=3,vpos=3;
   _VertexCopy(stack.v[0],arg[0]);
   _VertexCopy(stack.v[1],arg[1]);
   _VertexCopy(stack.v[2],arg[2]);
   _VertexCopy(stack.v[3],arg[3]);
   _VertexCopy(stack.vST[0],arg[4]);
   _VertexCopy(stack.vST[1],arg[5]);
   _VertexCopy(stack.vST[2],arg[6]);
   _VertexCopy(stack.vST[3],arg[7]);
   _TexCopy(stack.tex[0],arg[8]);
   _TexCopy(stack.tex[1],arg[9]);
   _TexCopy(stack.tex[2],arg[10]);
   _TexCopy(stack.tex[3],arg[11]);
   stack.order[0]=3;
   stack.order[1]=2;
   stack.order[2]=1;
   stack.order[3]=0;
   while(pos>2) {
      _VertexMean(va,getVertex(pos),getVertex(pos-2));
      _VertexMean(vb,getVertex(pos-1),getVertex(pos-3));
      _VertexCopy(vast,va);
      _VertexCopy(vbst,vb);
      _SpaceTransform(vast);
      _SpaceTransform(vbst);
      if((_VecCos(vast,getVertexST(pos),getVertexST(pos-2))<srel.detail ||
         _VecCos(vbst,getVertexST(pos-1),getVertexST(pos-3))<srel.detail) &&
         (vpos+2)<SUBDIVIDE_STACK_SIZE) {
         pos+=2;
         vpos+=2;
         _VertexCopy(stack.v[vpos],va);
         _VertexCopy(stack.v[vpos-1],vb);
         _VertexCopy(stack.vST[vpos],vast);
         _VertexCopy(stack.vST[vpos-1],vbst);
         if(srel.tex.haveCoord) {
            _TexMean(stack.tex[vpos],getTex(pos-2),getTex(pos-4));
            _TexMean(stack.tex[vpos-1],getTex(pos-3),getTex(pos-5));
         }
         stack.order[pos]=stack.order[pos-2];
         stack.order[pos-1]=stack.order[pos-3];
         stack.order[pos-2]=vpos;
         stack.order[pos-3]=vpos-1;
      } else {
         _DrawVertex(getVertexST(pos),getTex(pos));
         _DrawVertex(getVertexST(pos-1),getTex(pos-1));
         pos-=2;
      }
   }
   _DrawVertex(getVertexST(1),getTex(1));
   _DrawVertex(getVertexST(0),getTex(0));
}


