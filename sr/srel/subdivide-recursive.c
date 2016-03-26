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
* Recursive algorithms are considered to be slower than
* iterative algorithms since they involve the computer
* building a stack frame.
*/

void _SubdivideLine(GLfloat *const *arg)
{
   _VERTEXCOORD va,vast;
   _VertexMean(va,arg[0],arg[1]);
   _VertexCopy(vast,va);                          // interpolated middle vertex
   _SpaceTransform(vast);
   if(_VecCos(vast,arg[2],arg[3])<srel.detail) {
      GLfloat *subarg[6];
      _TEXCOORD ta;
      if(srel.tex.haveCoord)
         _TexMean(ta,arg[4],arg[5]);
      subarg[0]=arg[0];
      subarg[1]=va;
      subarg[2]=arg[2];
      subarg[3]=vast;
      subarg[4]=arg[4];
      subarg[5]=ta;
      _SubdivideLine(subarg);
      _DrawVertex(vast,ta);
      subarg[0]=va;
      subarg[1]=arg[1];
      subarg[2]=vast;
      subarg[3]=arg[3];
      subarg[4]=ta;
      subarg[5]=arg[5];
      _SubdivideLine(subarg);
   }
}

void _SubdivideTriangles(GLfloat *const *arg)
{
   _VERTEXCOORD va,vb,vc,vast,vbst,vcst;
   GLboolean noSub01=GL_TRUE,noSub12=GL_TRUE,noSub20=GL_TRUE;
   _VertexMean(va,arg[0],arg[1]);
   _VertexMean(vb,arg[1],arg[2]);
   _VertexMean(vc,arg[2],arg[0]);
   _VertexCopy(vast,va);
   _VertexCopy(vbst,vb);
   _VertexCopy(vcst,vc);
   _SpaceTransform(vast);
   _SpaceTransform(vbst);
   _SpaceTransform(vcst);
   if(_VecCos(vast,arg[3],arg[4])<srel.detail)
      noSub01=GL_FALSE;
   if(_VecCos(vbst,arg[4],arg[5])<srel.detail)
      noSub12=GL_FALSE;
   if(_VecCos(vcst,arg[5],arg[3])<srel.detail)
      noSub20=GL_FALSE;
   if(noSub01 && noSub12 && noSub20) {
      _DrawVertex(arg[3],arg[6]);
      _DrawVertex(arg[4],arg[7]);
      _DrawVertex(arg[5],arg[8]);
   } else if(noSub12 && noSub20) {
      GLfloat *subarg[9];
      _TEXCOORD ta;
      if(srel.tex.haveCoord)
         _TexMean(ta,arg[6],arg[7]);
      subarg[0]=arg[0];
      subarg[1]=va;
      subarg[2]=arg[2];
      subarg[3]=arg[3];
      subarg[4]=vast;
      subarg[5]=arg[5];
      subarg[6]=arg[6];
      subarg[7]=ta;
      subarg[8]=arg[8];
      _SubdivideTriangles(subarg);
      subarg[0]=arg[1];
      subarg[1]=arg[2];
      subarg[2]=va;
      subarg[3]=arg[4];
      subarg[4]=arg[5];
      subarg[5]=vast;
      subarg[6]=arg[7];
      subarg[7]=arg[8];
      subarg[8]=ta;
      _SubdivideTriangles(subarg);
   } else if(noSub01 && noSub20) {
      GLfloat *subarg[9];
      _TEXCOORD tb;
      if(srel.tex.haveCoord)
         _TexMean(tb,arg[7],arg[8]);
      subarg[0]=arg[0];
      subarg[1]=arg[1];
      subarg[2]=vb;
      subarg[3]=arg[3];
      subarg[4]=arg[4];
      subarg[5]=vbst;
      subarg[6]=arg[6];
      subarg[7]=arg[7];
      subarg[8]=tb;
      _SubdivideTriangles(subarg);
      subarg[0]=arg[2];
      subarg[1]=arg[0];
      subarg[2]=vb;
      subarg[3]=arg[5];
      subarg[4]=arg[3];
      subarg[5]=vbst;
      subarg[6]=arg[8];
      subarg[7]=arg[6];
      subarg[8]=tb;
      _SubdivideTriangles(subarg);
   } else if(noSub01 && noSub12) {
      GLfloat *subarg[9];
      _TEXCOORD tc;
      if(srel.tex.haveCoord)
         _TexMean(tc,arg[8],arg[6]);
      subarg[0]=arg[0];
      subarg[1]=arg[1];
      subarg[2]=vc;
      subarg[3]=arg[3];
      subarg[4]=arg[4];
      subarg[5]=vcst;
      subarg[6]=arg[6];
      subarg[7]=arg[7];
      subarg[8]=tc;
      _SubdivideTriangles(subarg);
      subarg[0]=arg[1];
      subarg[1]=arg[2];
      subarg[2]=vc;
      subarg[3]=arg[4];
      subarg[4]=arg[5];
      subarg[5]=vcst;
      subarg[6]=arg[7];
      subarg[7]=arg[8];
      subarg[8]=tc;
      _SubdivideTriangles(subarg);
   } else {
      GLfloat *subarg[9];
      _TEXCOORD ta,tb,tc;
      if(srel.tex.haveCoord) {
         _TexMean(ta,arg[6],arg[7]);
         _TexMean(tb,arg[7],arg[8]);
         _TexMean(tc,arg[8],arg[6]);
      }
      subarg[0]=arg[0];
      subarg[1]=va;
      subarg[2]=vc;
      subarg[3]=arg[3];
      subarg[4]=vast;
      subarg[5]=vcst;
      subarg[6]=arg[6];
      subarg[7]=ta;
      subarg[8]=tc;
      _SubdivideTriangles(subarg);
      subarg[0]=arg[1];
      subarg[1]=vb;
      subarg[2]=va;
      subarg[3]=arg[4];
      subarg[4]=vbst;
      subarg[5]=vast;
      subarg[6]=arg[7];
      subarg[7]=tb;
      subarg[8]=ta;
      _SubdivideTriangles(subarg);
      subarg[0]=arg[2];
      subarg[1]=vc;
      subarg[2]=vb;
      subarg[3]=arg[5];
      subarg[4]=vcst;
      subarg[5]=vbst;
      subarg[6]=arg[8];
      subarg[7]=tc;
      subarg[8]=tb;
      _SubdivideTriangles(subarg);
      subarg[0]=va;
      subarg[1]=vb;
      subarg[2]=vc;
      subarg[3]=vast;
      subarg[4]=vbst;
      subarg[5]=vcst;
      subarg[6]=ta;
      subarg[7]=tb;
      subarg[8]=tc;
      _SubdivideTriangles(subarg);
   }
}

void _SubdivideTriangleFan(GLfloat *const *arg)
{
   _VERTEXCOORD va,vast;
   _VertexMean(va,arg[1],arg[2]);
   _VertexCopy(vast,va);
   _SpaceTransform(vast);
   if(_VecCos(vast,arg[4],arg[5])<srel.detail) {
      GLfloat *subarg[9];
      _TEXCOORD ta;
      if(srel.tex.haveCoord)
         _TexMean(ta,arg[7],arg[8]);
      subarg[0]=arg[0];
      subarg[1]=arg[1];
      subarg[2]=va;
      subarg[3]=arg[3];
      subarg[4]=arg[4];
      subarg[5]=vast;
      subarg[6]=arg[6];
      subarg[7]=arg[7];
      subarg[8]=ta;
      _SubdivideTriangleFan(subarg);
      _DrawVertex(vast,ta);
      subarg[0]=arg[0];
      subarg[1]=va;
      subarg[2]=arg[2];
      subarg[3]=arg[3];
      subarg[4]=vast;
      subarg[5]=arg[5];
      subarg[6]=arg[6];
      subarg[7]=ta;
      subarg[8]=arg[8];
      _SubdivideTriangleFan(subarg);
   }
}

void _SubdivideQuads(GLfloat *const *arg)
{
   _VERTEXCOORD va,vb,vc,vd,vx,vast,vbst,vcst,vdst;
   GLboolean noSub01=GL_TRUE,noSub12=GL_TRUE,noSub23=GL_TRUE,noSub30=GL_TRUE;
   _VertexMean(va,arg[0],arg[1]);
   _VertexMean(vb,arg[1],arg[2]);
   _VertexMean(vc,arg[2],arg[3]);
   _VertexMean(vd,arg[3],arg[0]);
   _VertexQuadMean(vx,arg[0],arg[1],arg[2],arg[3]);
   _VertexCopy(vast,va);
   _VertexCopy(vbst,vb);
   _VertexCopy(vcst,vc);
   _VertexCopy(vdst,vd);
   _SpaceTransform(vast);
   _SpaceTransform(vbst);
   _SpaceTransform(vcst);
   _SpaceTransform(vdst);
   if(_VecCos(vast,arg[4],arg[5])<srel.detail)
      noSub01=GL_FALSE;
   if(_VecCos(vbst,arg[5],arg[6])<srel.detail)
      noSub12=GL_FALSE;
   if(_VecCos(vcst,arg[6],arg[7])<srel.detail)
      noSub23=GL_FALSE;
   if(_VecCos(vdst,arg[7],arg[4])<srel.detail)
      noSub30=GL_FALSE;
   if(noSub01 && noSub12 && noSub23 && noSub30) {
      _DrawVertex(arg[4],arg[8]);
      _DrawVertex(arg[5],arg[9]);
      _DrawVertex(arg[6],arg[10]);
      _DrawVertex(arg[7],arg[11]);
   } else if(noSub01 && noSub23) {
      GLfloat *subarg[12];
      _TEXCOORD tb,td;
      if(srel.tex.haveCoord) {
         _TexMean(tb,arg[9],arg[10]);
         _TexMean(td,arg[11],arg[8]);
      }
      subarg[0]=arg[0];
      subarg[1]=arg[1];
      subarg[2]=vb;
      subarg[3]=vd;
      subarg[4]=arg[4];
      subarg[5]=arg[5];
      subarg[6]=vbst;
      subarg[7]=vdst;
      subarg[8]=arg[8];
      subarg[9]=arg[9];
      subarg[10]=tb;
      subarg[11]=td;
      _SubdivideQuads(subarg);
      subarg[0]=arg[2];
      subarg[1]=arg[3];
      subarg[2]=vd;
      subarg[3]=vb;
      subarg[4]=arg[6];
      subarg[5]=arg[7];
      subarg[6]=vdst;
      subarg[7]=vbst;
      subarg[8]=arg[10];
      subarg[9]=arg[11];
      subarg[10]=td;
      subarg[11]=tb;
      _SubdivideQuads(subarg);
   } else if(noSub12 && noSub30) {
      GLfloat *subarg[12];
      _TEXCOORD ta,tc;
      if(srel.tex.haveCoord) {
         _TexMean(ta,arg[8],arg[9]);
         _TexMean(tc,arg[10],arg[11]);
      }
      subarg[0]=arg[1];
      subarg[1]=arg[2];
      subarg[2]=vc;
      subarg[3]=va;
      subarg[4]=arg[5];
      subarg[5]=arg[6];
      subarg[6]=vcst;
      subarg[7]=vast;
      subarg[8]=arg[9];
      subarg[9]=arg[10];
      subarg[10]=tc;
      subarg[11]=ta;
      _SubdivideQuads(subarg);
      subarg[0]=arg[3];
      subarg[1]=arg[0];
      subarg[2]=va;
      subarg[3]=vc;
      subarg[4]=arg[7];
      subarg[5]=arg[4];
      subarg[6]=vast;
      subarg[7]=vcst;
      subarg[8]=arg[11];
      subarg[9]=arg[8];
      subarg[10]=ta;
      subarg[11]=tc;
      _SubdivideQuads(subarg);
   } else {
      GLfloat *subarg[12];
      _VERTEXCOORD vxst;
      _TEXCOORD ta,tb,tc,td,tx;
      _VertexCopy(vxst,vx);
      _SpaceTransform(vxst);
      if(srel.tex.haveCoord) {
         _TexMean(ta,arg[8],arg[9]);
         _TexMean(tb,arg[9],arg[10]);
         _TexMean(tc,arg[10],arg[11]);
         _TexMean(td,arg[11],arg[8]);
         _TexQuadMean(tx,arg[8],arg[9],arg[10],arg[11]);
      }
      subarg[0]=arg[0];
      subarg[1]=va;
      subarg[2]=vx;
      subarg[3]=vd;
      subarg[4]=arg[4];
      subarg[5]=vast;
      subarg[6]=vxst;
      subarg[7]=vdst;
      subarg[8]=arg[8];
      subarg[9]=ta;
      subarg[10]=tx;
      subarg[11]=td;
      _SubdivideQuads(subarg);
      subarg[0]=arg[1];
      subarg[1]=vb;
      subarg[2]=vx;
      subarg[3]=va;
      subarg[4]=arg[5];
      subarg[5]=vbst;
      subarg[6]=vxst;
      subarg[7]=vast;
      subarg[8]=arg[9];
      subarg[9]=tb;
      subarg[10]=tx;
      subarg[11]=ta;
      _SubdivideQuads(subarg);
      subarg[0]=arg[2];
      subarg[1]=vc;
      subarg[2]=vx;
      subarg[3]=vb;
      subarg[4]=arg[6];
      subarg[5]=vcst;
      subarg[6]=vxst;
      subarg[7]=vbst;
      subarg[8]=arg[10];
      subarg[9]=tc;
      subarg[10]=tx;
      subarg[11]=tb;
      _SubdivideQuads(subarg);
      subarg[0]=arg[3];
      subarg[1]=vd;
      subarg[2]=vx;
      subarg[3]=vc;
      subarg[4]=arg[7];
      subarg[5]=vdst;
      subarg[6]=vxst;
      subarg[7]=vcst;
      subarg[8]=arg[11];
      subarg[9]=td;
      subarg[10]=tx;
      subarg[11]=tc;
      _SubdivideQuads(subarg);
   }
}

void _SubdivideQuadStrip(GLfloat *const *arg)
{
   _VERTEXCOORD va,vb,vast,vbst;
   _VertexMean(va,arg[0],arg[2]);
   _VertexMean(vb,arg[1],arg[3]);
   _VertexCopy(vast,va);
   _VertexCopy(vbst,vb);
   _SpaceTransform(vast);
   _SpaceTransform(vbst);
   if(_VecCos(vast,arg[4],arg[6])<srel.detail || _VecCos(vbst,arg[5],arg[7])<srel.detail) {
      GLfloat *subarg[12];
      _TEXCOORD ta,tb;
      if(srel.tex.haveCoord) {
         _TexMean(ta,arg[8],arg[10]);
         _TexMean(tb,arg[9],arg[11]);
      }
      subarg[0]=arg[0];
      subarg[1]=arg[1];
      subarg[2]=va;
      subarg[3]=vb;
      subarg[4]=arg[4];
      subarg[5]=arg[5];
      subarg[6]=vast;
      subarg[7]=vbst;
      subarg[8]=arg[8];
      subarg[9]=arg[9];
      subarg[10]=ta;
      subarg[11]=tb;
      _SubdivideQuadStrip(subarg);
      _DrawVertex(vast,ta);
      _DrawVertex(vbst,tb);
      subarg[0]=va;
      subarg[1]=vb;
      subarg[2]=arg[2];
      subarg[3]=arg[3];
      subarg[4]=vast;
      subarg[5]=vbst;
      subarg[6]=arg[6];
      subarg[7]=arg[7];
      subarg[8]=ta;
      subarg[9]=tb;
      subarg[10]=arg[10];
      subarg[11]=arg[11];
      _SubdivideQuadStrip(subarg);
   }
}


