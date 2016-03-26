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



#ifndef GLUTCALLBACK
   #define GLUTCALLBACK
#endif

void GLUTCALLBACK Reshape(int w,int h);
void GLUTCALLBACK Display(void);
void GLUTCALLBACK Key(unsigned char key,int x,int y);
void GLUTCALLBACK Special(int key,int x,int y);
void GLUTCALLBACK Mouse(int button,int state,int x,int y);
void GLUTCALLBACK Motion(int x,int y);
void GLUTCALLBACK ControlMenu(int value);
void GLUTCALLBACK HelpMenu(int value);
void GLUTCALLBACK OptionMenu(int value);
void GLUTCALLBACK FrameMenu(int value);
void GLUTCALLBACK ObjectMenu(int value);
void GLUTCALLBACK GraphicsMenu(int value);

void LINKAGE drawRotatingWheel(void);
#ifdef HAVE_LIB3DS
void LINKAGE draw3DS(void);
void LINKAGE draw3DSNode(Lib3dsNode *node);
#endif


