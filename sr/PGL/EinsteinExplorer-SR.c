/**
*   Einstein Explorer - SR (OS/2 version)
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



#define INCL_PM
#include <os2.h>
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
#include <gl\gl.h>
#include <gl\pgl.h>
#include "..\srel\srel.h"
#include "..\eesr\eesr.h"
#include "EinsteinExplorer-SR.h"

#define MOUSE_SCALE 0.2

static const GLfloat light_pos[4]={0.0,0.0,0.0,1.0};

#ifdef HAVE_LIB3DS
static Lib3dsFile *file=NULL;
#endif

static GLfloat detail=0.999;
/* GUI rotation */
static GLfloat angle=0.0;
static GLfloat rotAxis[3]={0.0,9.0,0.0};
/* mouse rotation */
static GLfloat xAngle,yAngle;
/* translation */
static GLfloat pos[3]={0.0,-12.5,0.0};

static HAB hab;
static HWND hwndFrame=HWND_DESKTOP,hwndClient,hwndMenu,hwndHelp;
static HWND hwndTra,hwndRot,hwndTim,hwndSpe;

int main(int argc,char *argv[])
{
   HMQ hmq;
   ULONG flCreateFlags=FCF_STANDARD;
   QMSG qmsg;
   SWP swp;
   PVISUALCONFIG pVisualConfig;
   HGC hgc;
   int pglAttrib[]={PGL_DOUBLEBUFFER,PGL_RGBA,PGL_None};

   if(!(hab=WinInitialize(0)))
      return DisplayError("Unable to initialize PM.");
   if(!(hmq=WinCreateMsgQueue(hab,0)))
      return DisplayError("Unable to create a message queue.");
   if(!pglQueryCapability(hab))
      return DisplayError("OpenGL not found.");
   if(!(pVisualConfig=pglChooseConfig(hab,pglAttrib)))
      return DisplayError("No PGL visual config.");
   if(!WinRegisterClass(hab,WIN_CLASS,ClientWndProc,CS_SIZEREDRAW | CS_MOVENOTIFY,0))
      return DisplayError("Failed to register window class.");
   if(!(hwndFrame=WinCreateStdWindow(HWND_DESKTOP,WS_VISIBLE,&flCreateFlags,
                                     WIN_CLASS,WIN_TITLE,WS_VISIBLE,NULLHANDLE,
                                     IDW_FRAME_WINDOW,&hwndClient)))
      return DisplayError("Failed to create a window.");
   if(!(hgc=pglCreateContext(hab,pVisualConfig,NULLHANDLE,TRUE)))
      return DisplayError("Unable to create an OpenGL context.");
   if(!pglMakeCurrent(hab,hgc,hwndClient))
      return DisplayError("Failed to bind to OpenGL context.");

   hwndMenu=WinWindowFromID(hwndFrame,FID_MENU);
   WinQueryWindowPos(hwndClient,&swp);
   HelpInit();
   eesrInit();
   eesrResize(swp.cx,swp.cy);

   while(WinGetMsg(hab,&qmsg,0,0,0))
      WinDispatchMsg(hab,&qmsg);

   WinDestroyWindow(hwndFrame);
   WinDestroyMsgQueue(hmq);
   WinTerminate(hab);
   return 0;
}


MRESULT EXPENTRY ClientWndProc(HWND hwnd,ULONG msg,MPARAM mp1,MPARAM mp2)
{
   static int objectID=IDM_OBJ_LATTICE;
   static ULONG buttonDown;
   static SHORT beginX,beginY;

   MRESULT mr=(MRESULT)FALSE;
   switch(msg) {
      case WM_SIZE:
         eesrResize(SHORT1FROMMP(mp2),SHORT2FROMMP(mp2));
         break;
      case WM_PAINT:
         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
         glLoadIdentity();
         glLightfv(GL_LIGHT0,GL_POSITION,light_pos);
         if(hwndRot) {
            glRotatef(angle,rotAxis[0],rotAxis[1],rotAxis[2]);
         } else {
            glRotatef(xAngle,1.0,0.0,0.0);
            glRotatef(yAngle,0.0,1.0,0.0);
         }
         glTranslatef(pos[0],pos[1],pos[2]);
         eesrDrawGeometry();
         pglSwapBuffers(hab,hwnd);
         break;
      case WM_COMMAND:
         switch(SHORT1FROMMP(mp1)) {
            case IDM_CTRL_TRA:
               if(!hwndTra)
                  hwndTra=WinLoadDlg(HWND_DESKTOP,hwnd,TranslateDlgProc,NULLHANDLE,IDD_TRA,NULL);
               else
                  WinSetFocus(HWND_DESKTOP,hwndTra);
               break;
            case IDM_CTRL_ROT:
               if(!hwndRot)
                  hwndRot=WinLoadDlg(HWND_DESKTOP,hwnd,RotateDlgProc,NULLHANDLE,IDD_ROT,NULL);
               else
                  WinSetFocus(HWND_DESKTOP,hwndRot);
               break;
            case IDM_CTRL_TIM:
               if(!hwndTim)
                  hwndTim=WinLoadDlg(HWND_DESKTOP,hwnd,TimeDlgProc,NULLHANDLE,IDD_TIM,NULL);
               else
                  WinSetFocus(HWND_DESKTOP,hwndTim);
               break;
            case IDM_CTRL_SPE:
               if(!hwndSpe)
                  hwndSpe=WinLoadDlg(HWND_DESKTOP,hwnd,SpeedDlgProc,NULLHANDLE,IDD_SPE,NULL);
               else
                  WinSetFocus(HWND_DESKTOP,hwndSpe);
               break;
            case IDM_FR_WORLD:
               if(WinIsMenuItemChecked(hwndMenu,IDM_FR_WORLD)==FALSE) {
                  eesrSetRestFrame(eesrDrawWorld);
                  eesrSetMovingFrame(eesrDrawObject);
                  eesrSpeed=-eesrSpeed;
                  WinCheckMenuItem(hwndMenu,IDM_FR_WORLD,TRUE);
                  WinCheckMenuItem(hwndMenu,IDM_FR_OBJECT,FALSE);
               }
               break;
            case IDM_FR_OBJECT:
               if(WinIsMenuItemChecked(hwndMenu,IDM_FR_OBJECT)==FALSE) {
                  eesrSetRestFrame(eesrDrawObject);
                  eesrSetMovingFrame(eesrDrawWorld);
                  eesrSpeed=-eesrSpeed;
                  WinCheckMenuItem(hwndMenu,IDM_FR_WORLD,FALSE);
                  WinCheckMenuItem(hwndMenu,IDM_FR_OBJECT,TRUE);
               }
               break;
            case IDM_OBJ_LATTICE:
               if(objectID!=IDM_OBJ_LATTICE) {
                  eesrSetObject(eesrDrawLattice);
                  WinCheckMenuItem(hwndMenu,objectID,FALSE);
                  WinCheckMenuItem(hwndMenu,IDM_OBJ_LATTICE,TRUE);
                  objectID=IDM_OBJ_LATTICE;
               }
               break;
            case IDM_OBJ_CAR:
               if(objectID!=IDM_OBJ_CAR) {
                  eesrSetObject(eesrDrawCar);
                  WinCheckMenuItem(hwndMenu,objectID,FALSE);
                  WinCheckMenuItem(hwndMenu,IDM_OBJ_CAR,TRUE);
                  objectID=IDM_OBJ_CAR;
               }
               break;
            case IDM_OBJ_PLANE:
               if(objectID!=IDM_OBJ_PLANE) {
                  eesrSetObject(eesrDrawAeroplane);
                  WinCheckMenuItem(hwndMenu,objectID,FALSE);
                  WinCheckMenuItem(hwndMenu,IDM_OBJ_PLANE,TRUE);
                  objectID=IDM_OBJ_PLANE;
               }
               break;
            case IDM_OBJ_SPHERE:
               if(objectID!=IDM_OBJ_SPHERE) {
                  eesrSetObject(eesrDrawSphere);
                  WinCheckMenuItem(hwndMenu,objectID,FALSE);
                  WinCheckMenuItem(hwndMenu,IDM_OBJ_SPHERE,TRUE);
                  objectID=IDM_OBJ_SPHERE;
               }
               break;
#ifdef HAVE_LIB3DS
            case IDM_OBJ_3DS:
               {
               FILEDLG fileDlg;
               HWND hwndDlg;
               fileDlg.cbSize=sizeof(FILEDLG);
               fileDlg.fl=FDS_CENTER | FDS_OPEN_DIALOG;
               fileDlg.ulUser=0;
               fileDlg.lReturn=0;
               fileDlg.lSRC=0;
               fileDlg.pszTitle="Open 3DS File";
               fileDlg.pszOKButton="Open";
               fileDlg.pfnDlgProc=NULL;
               fileDlg.pszIType=NULL;
               fileDlg.papszITypeList=NULL;
               fileDlg.pszIDrive=NULL;
               fileDlg.papszIDriveList=NULL;
               fileDlg.hMod=NULLHANDLE;
               strcpy(fileDlg.szFullFile,"*.3ds");
               fileDlg.papszFQFilename=NULL;
               fileDlg.ulFQFCount=0;
               fileDlg.usDlgId=0;
               fileDlg.x=FDS_CENTER;
               fileDlg.y=FDS_CENTER;
               fileDlg.sEAType=0;
               hwndDlg=WinFileDlg(HWND_DESKTOP,hwnd,&fileDlg);
               if(hwndDlg && fileDlg.lReturn==DID_OK) {
                  if(file)
                     lib3ds_file_free(file);
                  file=lib3ds_file_load(fileDlg.szFullFile);
                  eesrSetObject(draw3DS);
                  WinCheckMenuItem(hwndMenu,objectID,FALSE);
                  WinCheckMenuItem(hwndMenu,IDM_OBJ_3DS,TRUE);
                  objectID=IDM_OBJ_3DS;
               }
               }
               break;
#endif
            case IDM_OPT_DOPPLER:
               if(WinIsMenuItemChecked(hwndMenu,IDM_OPT_DOPPLER)==FALSE) {
                  srelDoppler(GL_TRUE);
                  WinCheckMenuItem(hwndMenu,IDM_OPT_DOPPLER,TRUE);
               } else {
                  srelDoppler(GL_FALSE);
                  WinCheckMenuItem(hwndMenu,IDM_OPT_DOPPLER,FALSE);
               }
               WinPostMsg(hwnd,WM_PAINT,NULL,NULL);
               break;
            case IDM_OPT_FREEZE:
               if(WinIsMenuItemChecked(hwndMenu,IDM_OPT_FREEZE)==FALSE) {
                  WinStopTimer(hab,hwnd,ID_TIMER);
                  WinCheckMenuItem(hwndMenu,IDM_OPT_FREEZE,TRUE);
               } else {
                  WinStartTimer(hab,hwnd,ID_TIMER,100);
                  WinCheckMenuItem(hwndMenu,IDM_OPT_FREEZE,FALSE);
               }
               break;
            case IDM_OPT_LOOP:
               if(WinIsMenuItemChecked(hwndMenu,IDM_OPT_LOOP)==FALSE) {
                  eesrLoop=GL_TRUE;
                  WinCheckMenuItem(hwndMenu,IDM_OPT_LOOP,TRUE);
               } else {
                  eesrLoop=GL_FALSE;
                  WinCheckMenuItem(hwndMenu,IDM_OPT_LOOP,FALSE);
               }
               break;
            case IDM_GRAPH_DETAIL:
               WinDlgBox(HWND_DESKTOP,hwnd,DetailDlgProc,NULLHANDLE,IDD_DETAIL,NULL);
               WinPostMsg(hwnd,WM_PAINT,NULL,NULL);
               break;
            case IDM_GRAPH_FOG:
               if(WinIsMenuItemChecked(hwndMenu,IDM_GRAPH_FOG)==FALSE) {
                  glEnable(GL_FOG);
                  WinCheckMenuItem(hwndMenu,IDM_GRAPH_FOG,TRUE);
               } else {
                  glDisable(GL_FOG);
                  WinCheckMenuItem(hwndMenu,IDM_GRAPH_FOG,FALSE);
               }
               WinPostMsg(hwnd,WM_PAINT,NULL,NULL);
               break;
            case IDM_GRAPH_TEX:
               if(WinIsMenuItemChecked(hwndMenu,IDM_GRAPH_TEX)==FALSE) {
                  glEnable(GL_TEXTURE_2D);
                  WinCheckMenuItem(hwndMenu,IDM_GRAPH_TEX,TRUE);
               } else {
                  glDisable(GL_TEXTURE_2D);
                  WinCheckMenuItem(hwndMenu,IDM_GRAPH_TEX,FALSE);
               }
               WinPostMsg(hwnd,WM_PAINT,NULL,NULL);
               break;
            case IDM_GRAPH_WIRE:
               if(WinIsMenuItemChecked(hwndMenu,IDM_GRAPH_WIRE)==FALSE) {
                  glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
                  WinCheckMenuItem(hwndMenu,IDM_GRAPH_WIRE,TRUE);
               } else {
                  glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
                  WinCheckMenuItem(hwndMenu,IDM_GRAPH_WIRE,FALSE);
               }
               WinPostMsg(hwnd,WM_PAINT,NULL,NULL);
               break;
            case IDM_HELP_INDEX:
               WinSendMsg(hwndHelp,HM_HELP_INDEX,MPVOID,MPVOID);
               break;
            case IDM_HELP_GEN:
               WinSendMsg(hwndHelp,HM_DISPLAY_HELP,
                          MPFROMSHORT(100),
                          MPFROMSHORT(HM_RESOURCEID));
               break;
            case IDM_HELP_USING:
               WinSendMsg(hwndHelp,HM_DISPLAY_HELP,MPVOID,MPVOID);
               break;
            case IDM_HELP_ABOUT:
               WinDlgBox(HWND_DESKTOP,hwnd,WinDefDlgProc,NULLHANDLE,IDD_PRODINFO,NULL);
               break;
         }
         break;
      case WM_CHAR:
         if(!(SHORT1FROMMP(mp1) & KC_KEYUP) && (SHORT1FROMMP(mp1) & KC_VIRTUALKEY)) {
            switch(SHORT2FROMMP(mp2)) {
               case VK_LEFT:
                  pos[0]+=CHAR3FROMMP(mp1);
                  WinPostMsg(hwnd,WM_PAINT,NULL,NULL);
                  break;
               case VK_RIGHT:
                  pos[0]-=CHAR3FROMMP(mp1);
                  WinPostMsg(hwnd,WM_PAINT,NULL,NULL);
                  break;
               case VK_UP:
                  pos[2]+=CHAR3FROMMP(mp1);
                  WinPostMsg(hwnd,WM_PAINT,NULL,NULL);
                  break;
               case VK_DOWN:
                  pos[2]-=CHAR3FROMMP(mp1);
                  WinPostMsg(hwnd,WM_PAINT,NULL,NULL);
                  break;
            }
         } else if(!(SHORT1FROMMP(mp1) & KC_KEYUP) && (SHORT1FROMMP(mp1) & KC_CHAR)) {
            switch(SHORT1FROMMP(mp2)) {
               case '<':
               case ',':
                  yAngle-=CHAR3FROMMP(mp1);
                  WinPostMsg(hwnd,WM_PAINT,NULL,NULL);
                  break;
               case '>':
               case '.':
                  yAngle+=CHAR3FROMMP(mp1);
                  WinPostMsg(hwnd,WM_PAINT,NULL,NULL);
                  break;
            }
         }
         break;
      case WM_BUTTON1DOWN:
      case WM_BUTTON2DOWN:
      case WM_BUTTON3DOWN:
         buttonDown=msg;
         beginX=SHORT1FROMMP(mp1);
         beginY=SHORT2FROMMP(mp1);
         break;
      case WM_BUTTON1UP:
      case WM_BUTTON2UP:
      case WM_BUTTON3UP:
         buttonDown=0;
         break;
      case WM_MOUSEMOVE:
         if(buttonDown==WM_BUTTON1DOWN) {
            pos[0]-=(SHORT1FROMMP(mp1)-beginX)*MOUSE_SCALE;
            pos[2]+=(SHORT2FROMMP(mp1)-beginY)*MOUSE_SCALE;
            WinPostMsg(hwnd,WM_PAINT,NULL,NULL);
         } else if(buttonDown==WM_BUTTON2DOWN) {
            yAngle+=(SHORT1FROMMP(mp1)-beginX)*MOUSE_SCALE;
            xAngle-=(SHORT2FROMMP(mp1)-beginY)*MOUSE_SCALE;
            WinPostMsg(hwnd,WM_PAINT,NULL,NULL);
         }
         beginX=SHORT1FROMMP(mp1);
         beginY=SHORT2FROMMP(mp1);
         break;
      case WM_TIMER:
         eesrSetTime(eesrGetTime()+1.0);
         WinPostMsg(hwnd,WM_PAINT,NULL,NULL);
         break;
      case WM_DESTROY:
#ifdef HAVE_LIB3DS
         if(file)
            lib3ds_file_free(file);
#endif
         WinAssociateHelpInstance(NULLHANDLE,hwnd);
         WinDestroyHelpInstance(hwndHelp);
         break;
      default:
         mr=WinDefWindowProc(hwnd,msg,mp1,mp2);
         break;
   }
   return mr;
}


MRESULT EXPENTRY TranslateDlgProc(HWND hwnd,ULONG msg,MPARAM mp1,MPARAM mp2)
{
   static GLfloat scale;

   MRESULT mr=(MRESULT)FALSE;
   switch(msg) {
      case WM_INITDLG:
         scale=1.0;
         break;
      case WM_CONTROL:
         switch(SHORT1FROMMP(mp1)) {
            case IDC_TRA_SCALE:
               if(SHORT2FROMMP(mp1)==BN_CLICKED) {
                  if(scale==1.0)
                     scale=10.0;
                  else
                     scale=1.0;
               }
               break;
         }
         break;
      case WM_COMMAND:
         switch(SHORT1FROMMP(mp1)) {
            case IDP_TRA_INCX:
               pos[0]-=scale;
               WinPostMsg(hwndClient,WM_PAINT,NULL,NULL);
               break;
            case IDP_TRA_DECX:
               pos[0]+=scale;
               WinPostMsg(hwndClient,WM_PAINT,NULL,NULL);
               break;
            case IDP_TRA_INCY:
               pos[1]-=scale;
               WinPostMsg(hwndClient,WM_PAINT,NULL,NULL);
               break;
            case IDP_TRA_DECY:
               pos[1]+=scale;
               WinPostMsg(hwndClient,WM_PAINT,NULL,NULL);
               break;
            case IDP_TRA_INCZ:
               pos[2]+=scale;
               WinPostMsg(hwndClient,WM_PAINT,NULL,NULL);
               break;
            case IDP_TRA_DECZ:
               pos[2]-=scale;
               WinPostMsg(hwndClient,WM_PAINT,NULL,NULL);
               break;
            case DID_OK:
               WinDestroyWindow(hwnd);
               hwndTra=NULLHANDLE;
               break;
         }
         break;
      default:
         mr=WinDefDlgProc(hwnd,msg,mp1,mp2);
         break;
   }
   return mr;
}


MRESULT EXPENTRY RotateDlgProc(HWND hwnd,ULONG msg,MPARAM mp1,MPARAM mp2)
{
   static HWND hwndX,hwndY,hwndZ,hwndAngle;
   SHORT sAng;

   MRESULT mr=(MRESULT)FALSE;
   switch(msg) {
      case WM_INITDLG:
         hwndAngle=WinWindowFromID(hwnd,IDC_ROT_ANGLE);
         hwndX=WinWindowFromID(hwnd,IDS_ROT_X);
         hwndY=WinWindowFromID(hwnd,IDS_ROT_Y);
         hwndZ=WinWindowFromID(hwnd,IDS_ROT_Z);
         WinSendMsg(hwndAngle,CSM_SETRANGE,MPFROMSHORT(0),MPFROMSHORT(360));
         sAng=(SHORT)(angle);
         sAng=sAng>180 ? sAng-180 : sAng+180;
         WinSendMsg(hwndAngle,CSM_SETVALUE,MPFROMSHORT(sAng),NULL);
         WinSendMsg(hwndX,SLM_SETSLIDERINFO,
                    MPFROM2SHORT(SMA_SLIDERARMPOSITION,SMA_INCREMENTVALUE),
                    MPFROMSHORT(rotAxis[0]));
         WinSendMsg(hwndY,SLM_SETSLIDERINFO,
                    MPFROM2SHORT(SMA_SLIDERARMPOSITION,SMA_INCREMENTVALUE),
                    MPFROMSHORT(rotAxis[1]));
         WinSendMsg(hwndZ,SLM_SETSLIDERINFO,
                    MPFROM2SHORT(SMA_SLIDERARMPOSITION,SMA_INCREMENTVALUE),
                    MPFROMSHORT(rotAxis[2]));
         break;
      case WM_CONTROL:
         switch(SHORT1FROMMP(mp1)) {
            case IDC_ROT_ANGLE:
               if(SHORT2FROMMP(mp1)==CSN_CHANGED) {
                  WinSendMsg(hwndAngle,CSM_QUERYVALUE,MPFROMP(&sAng),NULL);
                  angle=sAng>180 ? sAng-180 : sAng+180;
                  WinPostMsg(hwndClient,WM_PAINT,NULL,NULL);
               }
               break;
            case IDS_ROT_X:
               if(SHORT2FROMMP(mp1)==SLN_CHANGE) {
                  rotAxis[0]=(USHORT)WinSendMsg(hwndX,SLM_QUERYSLIDERINFO,
                                                MPFROM2SHORT(SMA_SLIDERARMPOSITION,SMA_INCREMENTVALUE),
                                                NULL);
                  WinPostMsg(hwndClient,WM_PAINT,NULL,NULL);
               }
               break;
            case IDS_ROT_Y:
               if(SHORT2FROMMP(mp1)==SLN_CHANGE) {
                  rotAxis[1]=(USHORT)WinSendMsg(hwndY,SLM_QUERYSLIDERINFO,
                                                MPFROM2SHORT(SMA_SLIDERARMPOSITION,SMA_INCREMENTVALUE),
                                                NULL);
                  WinPostMsg(hwndClient,WM_PAINT,NULL,NULL);
               }
               break;
            case IDS_ROT_Z:
               if(SHORT2FROMMP(mp1)==SLN_CHANGE) {
                  rotAxis[2]=(USHORT)WinSendMsg(hwndZ,SLM_QUERYSLIDERINFO,
                                                MPFROM2SHORT(SMA_SLIDERARMPOSITION,SMA_INCREMENTVALUE),
                                                NULL);
                  WinPostMsg(hwndClient,WM_PAINT,NULL,NULL);
               }
               break;
         }
         break;
      case WM_COMMAND:
         switch(SHORT1FROMMP(mp1)) {
            case DID_OK:
               WinDestroyWindow(hwnd);
               hwndRot=NULLHANDLE;
               break;
         }
         break;
      default:
         mr=WinDefDlgProc(hwnd,msg,mp1,mp2);
         break;
   }
   return mr;
}


MRESULT EXPENTRY TimeDlgProc(HWND hwnd,ULONG msg,MPARAM mp1,MPARAM mp2)
{
   static GLfloat scale;

   MRESULT mr=(MRESULT)FALSE;
   switch(msg) {
      case WM_INITDLG:
         scale=1.0;
         break;
      case WM_CONTROL:
         switch(SHORT1FROMMP(mp1)) {
            case IDC_TIM_SCALE:
               if(SHORT2FROMMP(mp1)==BN_CLICKED) {
                  if(scale==1.0)
                     scale=10.0;
                  else
                     scale=1.0;
               }
               break;
         }
         break;
      case WM_COMMAND:
         switch(SHORT1FROMMP(mp1)) {
            case IDP_TIM_INC:
               eesrSetTime(eesrGetTime()+scale);
               WinPostMsg(hwndClient,WM_PAINT,NULL,NULL);
               break;
            case IDP_TIM_DEC:
               eesrSetTime(eesrGetTime()-scale);
               WinPostMsg(hwndClient,WM_PAINT,NULL,NULL);
               break;
            case DID_OK:
               WinDestroyWindow(hwnd);
               hwndTim=NULLHANDLE;
               break;
         }
         break;
      default:
         mr=WinDefDlgProc(hwnd,msg,mp1,mp2);
         break;
   }
   return mr;
}


MRESULT EXPENTRY SpeedDlgProc(HWND hwnd,ULONG msg,MPARAM mp1,MPARAM mp2)
{
   static HWND hwndBar;
   double mag;

   MRESULT mr=(MRESULT)FALSE;
   switch(msg) {
      case WM_INITDLG:
         hwndBar=WinWindowFromID(hwnd,IDS_SPE_BAR);
         mag=fabs(eesrSpeed);
         WinSendMsg(hwndBar,SLM_SETSLIDERINFO,
                    MPFROM2SHORT(SMA_SLIDERARMPOSITION,SMA_INCREMENTVALUE),
                    MPFROMSHORT(mag*50.1));
         break;
      case WM_CONTROL:
         switch(SHORT1FROMMP(mp1)) {
            case IDS_SPE_BAR:
               if(SHORT2FROMMP(mp1)==SLN_CHANGE) {
                  mag=(USHORT)WinSendMsg(hwndBar,SLM_QUERYSLIDERINFO,
                                             MPFROM2SHORT(SMA_SLIDERARMPOSITION,SMA_INCREMENTVALUE),
                                             NULL)/50.0;
                  if(eesrSpeed>=0)
                     eesrSpeed=mag;
                  else
                     eesrSpeed=-mag;
                  WinPostMsg(hwndClient,WM_PAINT,NULL,NULL);
               }
               break;
         }
         break;
      case WM_COMMAND:
         switch(SHORT1FROMMP(mp1)) {
            case DID_OK:
               WinDestroyWindow(hwnd);
               hwndSpe=NULLHANDLE;
               break;
         }
         break;
      default:
         mr=WinDefDlgProc(hwnd,msg,mp1,mp2);
         break;
   }
   return mr;
}


MRESULT EXPENTRY DetailDlgProc(HWND hwnd,ULONG msg,MPARAM mp1,MPARAM mp2)
{
   static HWND hwndField;
   char detailStr[10];
   float newValue;

   MRESULT mr=(MRESULT)FALSE;
   switch(msg) {
      case WM_INITDLG:
         hwndField=WinWindowFromID(hwnd,IDE_DETAIL_FIELD);
         sprintf(detailStr,"%.5f",detail);
         WinSetWindowText(hwndField,detailStr);
         break;
      case WM_COMMAND:
         switch(SHORT1FROMMP(mp1)) {
            case DID_OK:
               WinQueryWindowText(hwndField,10,detailStr);
               sscanf(detailStr,"%f",&newValue);
               if(newValue>=0.0 && newValue<1.0) {
                  detail=newValue;
                  srelDetail(detail);
                  WinDismissDlg(hwnd,TRUE);
               } else
                  WinMessageBox(HWND_DESKTOP,hwndFrame,"Value must be between 0 and 1.","Detail Control",0,MB_OK | MB_ERROR);
               break;
            case DID_CANCEL:
               WinDismissDlg(hwnd,FALSE);
               break;
         }
         break;
      default:
         mr=WinDefDlgProc(hwnd,msg,mp1,mp2);
         break;
   }
   return mr;
}


void HelpInit(void)
{
   HELPINIT hini;
   hini.cb=sizeof(HELPINIT);
   hini.ulReturnCode=0;
   hini.pszTutorialName=NULL;
   hini.phtHelpTable=(PHELPTABLE)MAKEULONG(IDH_MAIN_HELPTABLE,0xFFFF);
   hini.hmodHelpTableModule=0;
   hini.hmodAccelActionBarModule=0;
   hini.idAccelTable=0;
   hini.idActionBar=0;
   hini.pszHelpWindowTitle=HELP_TITLE;
   hini.fShowPanelId=CMIC_HIDE_PANEL_ID;
   hini.pszHelpLibraryName=HELP_FILE_NAME;
   if(!(hwndHelp=WinCreateHelpInstance(hab,&hini)))
      DisplayError("Unable to initialize help.");
   WinAssociateHelpInstance(hwndHelp,hwndFrame);
}

int DisplayError(PSZ msg)
{
   WinMessageBox(HWND_DESKTOP,hwndFrame,msg,WIN_TITLE,0,MB_OK | MB_ERROR);
   WinTerminate(hab);
   return -1;
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


