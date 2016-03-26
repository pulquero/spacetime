/**
*   Einstein Explorer - SR (Win32 version)
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



#include <windows.h>
#include <commctrl.h>
#include <gl\gl.h>
#include <math.h>
#include <stdio.h>
#include "..\srel\srel.h"
#include "..\eesr\eesr.h"
#include "EinsteinExplorer-SR.h"

static const GLfloat light_pos[4]={0.0,0.0,0.0,1.0};

static GLfloat angle=0.0;
static GLfloat rotAxis[3]={0.0,10.0,0.0};
static GLfloat pos[3]={0.0,-12.5,0.0};

static BOOL menuDoppler=TRUE,menuFreeze=TRUE,menuLoop=FALSE;
static BOOL menuFog=FALSE,menuTex=FALSE,menuWire=FALSE;
static int menuFrame=1,menuObject=1;

static HINSTANCE hinst;
static HWND hwndFrame,hwndClient,hwndHelp;
static HMENU hmenuMenu;
static HDC hDC;
static HGLRC hGLRC;
static HPALETTE hPalette;

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nShowCmd)
{
	MSG msg;
	WNDCLASS wc;
	HACCEL hAccelTable;

	wc.style=CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc=ClientWndProc;
	wc.cbClsExtra=0;
	wc.cbWndExtra=0;
	wc.hInstance=hInstance;
	wc.hIcon=LoadIcon(hInstance,"Icon");
	wc.hCursor=LoadCursor(NULL,IDC_ARROW);
	wc.hbrBackground=NULL;
	wc.lpszMenuName="MainMenu";
	wc.lpszClassName=WIN_CLASS;

	if(!RegisterClass(&wc))
		DisplayError("Unable to register class.");

	hwndFrame=CreateWindow(WIN_CLASS,WIN_TITLE,WS_OVERLAPPEDWINDOW,
						   CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,CW_USEDEFAULT,
						   NULL,NULL,hInstance,NULL);
	if(!hwndFrame)
		DisplayError("Unable to create window.");

	hinst=hInstance;
	hmenuMenu=GetMenu(hwndFrame);
	ShowWindow(hwndFrame,nShowCmd);
	UpdateWindow(hwndFrame);
	
	InitCommonControls();
	hAccelTable=LoadAccelerators(hInstance,"Accelerator");

	while(GetMessage(&msg,NULL,0,0)) {
		if(!TranslateAccelerator(msg.hwnd,hAccelTable,&msg)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	return msg.wParam;
}


LRESULT CALLBACK ClientWndProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
   LRESULT lRet=0;
   RECT rect;
   PAINTSTRUCT ps;
   switch(msg) {
		case WM_CREATE:
			hwndClient=hwnd;
			hDC=GetDC(hwnd);
			SetupPixelFormat(hDC);
			SetupPalette(hDC);
			hGLRC=wglCreateContext(hDC);
			wglMakeCurrent(hDC,hGLRC);
			GetClientRect(hwnd,&rect);
			eesrInit();
			eesrResize(rect.right,rect.bottom);
			break;
		case WM_SIZE:
			eesrResize(LOWORD(lParam),HIWORD(lParam));
			break;
		case WM_PALETTECHANGED:
			if(hGLRC && hPalette && (HWND)wParam!=hwnd) {
				UnrealizeObject(hPalette);
				SelectPalette(hDC,hPalette,FALSE);
				RealizePalette(hDC);
				RedrawWindow(hwnd,NULL,NULL,RDW_INTERNALPAINT);
			}
			break;
		case WM_QUERYNEWPALETTE:
			if(hGLRC && hPalette) {
				UnrealizeObject(hPalette);
				SelectPalette(hDC,hPalette,FALSE);
				RealizePalette(hDC);
				RedrawWindow(hwnd,NULL,NULL,RDW_INTERNALPAINT);
			}
			break;
		case WM_PAINT:
			BeginPaint(hwnd,&ps);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glLoadIdentity();
                                glLightfv(GL_LIGHT0,GL_POSITION,light_pos);
			glRotated(angle,rotAxis[0],rotAxis[1],rotAxis[2]);
			glTranslated(pos[0],pos[1],pos[2]);
			eesrDrawGeometry();
			SwapBuffers(hDC);
			EndPaint(hwnd,&ps);
			break;
		case WM_COMMAND:
			switch(LOWORD(wParam)) {
				case IDM_CTRL_TRA:
					CreateDialog(hinst,"TranslateDlg",hwnd,TranslateDlgProc);
					break;
				case IDM_CTRL_ROT:
					CreateDialog(hinst,"RotateDlg",hwnd,RotateDlgProc);
					break;
				case IDM_CTRL_TIM:
					CreateDialog(hinst,"TimeDlg",hwnd,TimeDlgProc);
					break;
				case IDM_CTRL_SPE:
					CreateDialog(hinst,"SpeedDlg",hwnd,SpeedDlgProc);
					break;
				case IDM_FR_WORLD:
					if(menuFrame!=1) {
						menuFrame=1;
						eesrSetRestFrame(eesrDrawWorld);
						eesrSetMovingFrame(eesrDrawObject);
						eesrSpeed=-eesrSpeed;
						CheckMenuItem(hmenuMenu,IDM_FR_WORLD,MF_CHECKED);
						CheckMenuItem(hmenuMenu,IDM_FR_OBJECT,MF_UNCHECKED);
						SendMessage(hwnd,WM_PAINT,0,0);
					}
					break;
				case IDM_FR_OBJECT:
					if(menuFrame!=2) {
						menuFrame=2;
						eesrSetRestFrame(eesrDrawObject);
						eesrSetMovingFrame(eesrDrawWorld);
						eesrSpeed=-eesrSpeed;
						CheckMenuItem(hmenuMenu,IDM_FR_WORLD,MF_UNCHECKED);
						CheckMenuItem(hmenuMenu,IDM_FR_OBJECT,MF_CHECKED);
						SendMessage(hwnd,WM_PAINT,0,0);
					}
					break;
				case IDM_OBJ_LATTICE:
					if(menuObject!=1) {
						menuObject=1;
						eesrSetObject(eesrDrawLattice);
						CheckMenuItem(hmenuMenu,IDM_OBJ_LATTICE,MF_CHECKED);
						CheckMenuItem(hmenuMenu,IDM_OBJ_CAR,MF_UNCHECKED);
						CheckMenuItem(hmenuMenu,IDM_OBJ_PLANE,MF_UNCHECKED);
						CheckMenuItem(hmenuMenu,IDM_OBJ_SPHERE,MF_UNCHECKED);
						SendMessage(hwnd,WM_PAINT,0,0);
					}
					break;
				case IDM_OBJ_CAR:
					if(menuObject!=2) {
						menuObject=2;
						eesrSetObject(eesrDrawCar);
						CheckMenuItem(hmenuMenu,IDM_OBJ_LATTICE,MF_UNCHECKED);
						CheckMenuItem(hmenuMenu,IDM_OBJ_CAR,MF_CHECKED);
						CheckMenuItem(hmenuMenu,IDM_OBJ_PLANE,MF_UNCHECKED);
						CheckMenuItem(hmenuMenu,IDM_OBJ_SPHERE,MF_UNCHECKED);
						SendMessage(hwnd,WM_PAINT,0,0);
					}
					break;
				case IDM_OBJ_PLANE:
					if(menuObject!=3) {
						menuObject=3;
						eesrSetObject(eesrDrawAeroplane);
						CheckMenuItem(hmenuMenu,IDM_OBJ_LATTICE,MF_UNCHECKED);
						CheckMenuItem(hmenuMenu,IDM_OBJ_CAR,MF_UNCHECKED);
						CheckMenuItem(hmenuMenu,IDM_OBJ_PLANE,MF_CHECKED);
						CheckMenuItem(hmenuMenu,IDM_OBJ_SPHERE,MF_UNCHECKED);
						SendMessage(hwnd,WM_PAINT,0,0);
					}
					break;
				case IDM_OBJ_SPHERE:
					if(menuObject!=4) {
						menuObject=4;
						eesrSetObject(eesrDrawSphere);
						CheckMenuItem(hmenuMenu,IDM_OBJ_LATTICE,MF_UNCHECKED);
						CheckMenuItem(hmenuMenu,IDM_OBJ_CAR,MF_UNCHECKED);
						CheckMenuItem(hmenuMenu,IDM_OBJ_PLANE,MF_UNCHECKED);
						CheckMenuItem(hmenuMenu,IDM_OBJ_SPHERE,MF_CHECKED);
						SendMessage(hwnd,WM_PAINT,0,0);
					}
					break;
				case IDM_OPT_DOPPLER:
					if(menuDoppler==FALSE) {
						menuDoppler=TRUE;
						srelDoppler(GL_TRUE);
						CheckMenuItem(hmenuMenu,IDM_OPT_DOPPLER,MF_CHECKED);
					} else {
						menuDoppler=FALSE;
						srelDoppler(GL_FALSE);
						CheckMenuItem(hmenuMenu,IDM_OPT_DOPPLER,MF_UNCHECKED);
					}
					SendMessage(hwnd,WM_PAINT,0,0);
					break;
				case IDM_OPT_FREEZE:
					if(menuFreeze==FALSE) {
						menuFreeze=TRUE;
						KillTimer(hwnd,ID_TIMER);
						CheckMenuItem(hmenuMenu,IDM_OPT_FREEZE,MF_CHECKED);
					} else {
						menuFreeze=FALSE;
						SetTimer(hwnd,ID_TIMER,100,NULL);
						CheckMenuItem(hmenuMenu,IDM_OPT_FREEZE,MF_UNCHECKED);
					}
					break;
				case IDM_OPT_LOOP:
					if(menuLoop==FALSE) {
						menuLoop=TRUE;
						eesrLoop=1;
						CheckMenuItem(hmenuMenu,IDM_OPT_LOOP,MF_CHECKED);
					} else {
						menuLoop=FALSE;
						eesrLoop=0;
						CheckMenuItem(hmenuMenu,IDM_OPT_LOOP,MF_UNCHECKED);
					}
					break;
				case IDM_GRAPH_FOG:
					if(menuFog==FALSE) {
						menuFog=TRUE;
						glEnable(GL_FOG);
						CheckMenuItem(hmenuMenu,IDM_GRAPH_FOG,MF_CHECKED);
					} else {
						menuFog=FALSE;
						glDisable(GL_FOG);
						CheckMenuItem(hmenuMenu,IDM_GRAPH_FOG,MF_UNCHECKED);
					}
					SendMessage(hwnd,WM_PAINT,0,0);
					break;
				case IDM_GRAPH_TEX:
					if(menuTex==FALSE) {
						menuTex=TRUE;
						glEnable(GL_TEXTURE_2D);
						CheckMenuItem(hmenuMenu,IDM_GRAPH_TEX,MF_CHECKED);
					} else {
						menuTex=FALSE;
						glDisable(GL_TEXTURE_2D);
						CheckMenuItem(hmenuMenu,IDM_GRAPH_TEX,MF_UNCHECKED);
					}
					SendMessage(hwnd,WM_PAINT,0,0);
					break;
				case IDM_GRAPH_WIRE:
					if(menuWire==FALSE) {
						menuWire=TRUE;
						glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
						CheckMenuItem(hmenuMenu,IDM_GRAPH_WIRE,MF_CHECKED);
					} else {
						menuWire=FALSE;
						glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
						CheckMenuItem(hmenuMenu,IDM_GRAPH_WIRE,MF_UNCHECKED);
					}
					SendMessage(hwnd,WM_PAINT,0,0);
					break;
				case IDM_HELP_CONTENTS:
					WinHelp(hwnd,HELP_FILE_NAME,HELP_FINDER,0);
					break;
				case IDM_HELP_ABOUT:
					DialogBox(hinst,"ProdInfoDlg",hwnd,ProdInfoDlgProc);
					break;
         }
         break;
      case WM_TIMER:
         eesrSetTime(eesrGetTime()+1.0);
	 SendMessage(hwnd,WM_PAINT,0,0);
         break;
	  case WM_CLOSE:
		  if(hGLRC)
			  wglDeleteContext(hGLRC);
		  hGLRC=0;
		  if(hDC)
			  ReleaseDC(hwnd,hDC);
		  hDC=0;
		  DestroyWindow(hwnd);
		  break;
      case WM_DESTROY:
		  if(hGLRC)
			  wglDeleteContext(hGLRC);
		  if(hDC)
			  ReleaseDC(hwnd,hDC);
		  PostQuitMessage(0);
         break;
      default:
         lRet=DefWindowProc(hwnd,msg,wParam,lParam);
         break;
   }
   return lRet;
}


BOOL CALLBACK TranslateDlgProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
   static GLdouble scale;
   BOOL bRet=FALSE;
   switch(msg) {
      case WM_INITDIALOG:
         scale=1.0;
         break;
      case WM_COMMAND:
         switch(LOWORD(wParam)) {
            case IDP_TRA_INCX:
               pos[0]-=scale;
			   SendMessage(hwndClient,WM_PAINT,0,0);
               break;
            case IDP_TRA_DECX:
               pos[0]+=scale;
			   SendMessage(hwndClient,WM_PAINT,0,0);
               break;
            case IDP_TRA_INCY:
               pos[1]-=scale;
			   SendMessage(hwndClient,WM_PAINT,0,0);
               break;
            case IDP_TRA_DECY:
               pos[1]+=scale;
			   SendMessage(hwndClient,WM_PAINT,0,0);
               break;
            case IDP_TRA_INCZ:
               pos[2]+=scale;
			   SendMessage(hwndClient,WM_PAINT,0,0);
               break;
            case IDP_TRA_DECZ:
               pos[2]-=scale;
			   SendMessage(hwndClient,WM_PAINT,0,0);
               break;
            case IDC_TRA_SCALE:
               if(HIWORD(wParam)==BN_CLICKED) {
                  if(scale==1.0)
                     scale=10.0;
                  else
                     scale=1.0;
               }
			   break;
			case IDOK:
				EndDialog(hwnd,TRUE);
				break;
         }
         break;
   }
   return bRet;
}


BOOL CALLBACK RotateDlgProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	static HWND hwndAngle,hwndX,hwndY,hwndZ;
	BOOL bRet=FALSE;
	switch(msg) {
		case WM_INITDIALOG:
			hwndAngle=GetDlgItem(hwnd,IDC_ROT_ANGLE);
			hwndX=GetDlgItem(hwnd,IDS_ROT_X);
			hwndY=GetDlgItem(hwnd,IDS_ROT_Y);
			hwndZ=GetDlgItem(hwnd,IDS_ROT_Z);
			SendDlgItemMessage(hwnd,IDC_ROT_ANGLE,TBM_SETRANGE,(WPARAM)TRUE,(LPARAM)MAKELONG(0,360));
			SendDlgItemMessage(hwnd,IDS_ROT_X,TBM_SETRANGE,(WPARAM)TRUE,(LPARAM)MAKELONG(0,10));
			SendDlgItemMessage(hwnd,IDS_ROT_Y,TBM_SETRANGE,(WPARAM)TRUE,(LPARAM)MAKELONG(0,10));
			SendDlgItemMessage(hwnd,IDS_ROT_Z,TBM_SETRANGE,(WPARAM)TRUE,(LPARAM)MAKELONG(0,10));
			SendDlgItemMessage(hwnd,IDC_ROT_ANGLE,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)(angle+180));
			SendDlgItemMessage(hwnd,IDS_ROT_X,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)(10.0-rotAxis[0]));
			SendDlgItemMessage(hwnd,IDS_ROT_Y,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)(10.0-rotAxis[1]));
			SendDlgItemMessage(hwnd,IDS_ROT_Z,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)(10.0-rotAxis[2]));
			break;
		case WM_HSCROLL:
			if((HWND)lParam==hwndAngle) {
				angle=SendDlgItemMessage(hwnd,IDC_ROT_ANGLE,TBM_GETPOS,0,0)-180;
				SendMessage(hwndClient,WM_PAINT,0,0);
			}
			break;
		case WM_VSCROLL:
			if((HWND)lParam==hwndX) {
				rotAxis[0]=10.0-SendDlgItemMessage(hwnd,IDS_ROT_X,TBM_GETPOS,0,0);
				SendMessage(hwndClient,WM_PAINT,0,0);
			} else if((HWND)lParam==hwndY) {
				rotAxis[1]=10.0-SendDlgItemMessage(hwnd,IDS_ROT_Y,TBM_GETPOS,0,0);
				SendMessage(hwndClient,WM_PAINT,0,0);
			} else if((HWND)lParam==hwndZ) {
	            rotAxis[2]=10.0-SendDlgItemMessage(hwnd,IDS_ROT_Z,TBM_GETPOS,0,0);
				SendMessage(hwndClient,WM_PAINT,0,0);
			}
			break;
		case WM_COMMAND:
			switch(LOWORD(wParam)) {
	            case IDOK:
					EndDialog(hwnd,TRUE);
					break;
			}
			break;
	}
	return bRet;
}


BOOL CALLBACK TimeDlgProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
   static GLdouble scale;
   BOOL bRet=FALSE;
   switch(msg) {
      case WM_INITDIALOG:
         scale=1.0;
         break;
      case WM_COMMAND:
         switch(LOWORD(wParam)) {
            case IDP_TIM_INC:
               eesrSetTime(eesrGetTime()+scale);
               SendMessage(hwndClient,WM_PAINT,0,0);
               break;
            case IDP_TIM_DEC:
               eesrSetTime(eesrGetTime()-scale);
               SendMessage(hwndClient,WM_PAINT,0,0);
               break;
            case IDC_TIM_SCALE:
               if(HIWORD(wParam)==BN_CLICKED) {
                  if(scale==1.0)
                     scale=10.0;
                  else
                     scale=1.0;
               }
               break;
            case IDOK:
			   EndDialog(hwnd,TRUE);
               break;
         }
         break;
   }
   return bRet;
}


BOOL CALLBACK SpeedDlgProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
	static HWND hwndBar;
	double mag;
	BOOL bRet=FALSE;
	switch(msg) {
		case WM_INITDIALOG:
			mag=fabs(eesrSpeed);
			hwndBar=GetDlgItem(hwnd,IDS_SPE_BAR);
			SendDlgItemMessage(hwnd,IDS_SPE_BAR,TBM_SETRANGE,(WPARAM)TRUE,(LPARAM)MAKELONG(0,50));
			SendDlgItemMessage(hwnd,IDS_SPE_BAR,TBM_SETPOS,(WPARAM)TRUE,(LPARAM)(mag*51.1));
			break;
		case WM_HSCROLL:
			if((HWND)lParam==hwndBar) {
				mag=SendDlgItemMessage(hwnd,IDS_SPE_BAR,TBM_GETPOS,0,0)/51.0;
				if(eesrSpeed>=0)
					eesrSpeed=mag;
				else
					eesrSpeed=-mag;
				SendMessage(hwndClient,WM_PAINT,0,0);
			}
			break;
		case WM_COMMAND:
			switch(LOWORD(wParam)) {
				case IDOK:
					EndDialog(hwnd,TRUE);
					break;
			}
			break;
	}
	return bRet;
}


BOOL CALLBACK ProdInfoDlgProc(HWND hwnd,UINT msg,WPARAM wParam,LPARAM lParam)
{
   BOOL bRet=FALSE;
   switch(msg) {
      case WM_COMMAND:
         switch(LOWORD(wParam)) {
            case IDOK:
			   EndDialog(hwnd,TRUE);
               break;
         }
         break;
   }
   return bRet;
}


void SetupPixelFormat(HDC hdc)
{
	int pixelformat;
	PIXELFORMATDESCRIPTOR pfd;

	pfd.nSize=sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion=1;
	pfd.dwFlags=PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.dwLayerMask=PFD_MAIN_PLANE;
	pfd.iPixelType=PFD_TYPE_RGBA;
	pfd.cColorBits=8;
	pfd.cDepthBits=8;
	pfd.cAccumBits=0;
	pfd.cStencilBits=0;

	pixelformat=ChoosePixelFormat(hdc,&pfd);
	if(!pixelformat)
		DisplayError("No OpenGL visual config.");
	if(SetPixelFormat(hdc,pixelformat,&pfd)==FALSE)
		DisplayError("Unable to use OpenGL visual config.");
}


void SetupPalette(HDC hdc)
{
	int pixelformat,paletteSize;
	PIXELFORMATDESCRIPTOR pfd;
	LOGPALETTE *pPal;
	int i,redMask,greenMask,blueMask;

	pixelformat=GetPixelFormat(hdc);
	DescribePixelFormat(hdc,pixelformat,sizeof(PIXELFORMATDESCRIPTOR),&pfd);
	if(pfd.dwFlags & PFD_NEED_PALETTE)
		paletteSize=1<<pfd.cColorBits;
	else
		return;

	pPal=(LOGPALETTE*)malloc(sizeof(LOGPALETTE)+paletteSize*sizeof(PALETTEENTRY));
	pPal->palVersion=0x300;
	pPal->palNumEntries=paletteSize;

	redMask=(1<<pfd.cRedBits)-1;
	greenMask=(1<<pfd.cGreenBits)-1;
	blueMask=(1<<pfd.cBlueBits)-1;
	
	for(i=0;i<paletteSize;i++) {
		pPal->palPalEntry[i].peRed=
			(((i>>pfd.cRedShift)&redMask)*255)/redMask;
		pPal->palPalEntry[i].peGreen=
			(((i>>pfd.cGreenShift)&greenMask)*255)/greenMask;
		pPal->palPalEntry[i].peBlue=
			(((i>>pfd.cBlueShift)&blueMask)*255)/blueMask;
		pPal->palPalEntry[i].peFlags=0;
	}

	hPalette=CreatePalette(pPal);
	free(pPal);
	if(hPalette) {
		SelectPalette(hdc,hPalette,FALSE);
		RealizePalette(hdc);
	}
}

void DisplayError(char *msg)
{
	MessageBox(NULL,msg,WIN_TITLE,MB_OK | MB_ICONHAND);
	exit(0);
}
