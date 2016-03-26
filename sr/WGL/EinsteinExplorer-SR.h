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



#include "resource.h"

/*-----------*/
/* Constants */
/*-----------*/
#define WIN_TITLE      "Einstein Explorer - SR"
#define HELP_TITLE     "Help for Einstein Explorer - SR"
#define WIN_CLASS      "EESRClass"
#define HELP_FILE_NAME "EinsteinExplorer-SR.hlp"


/*----------------------*/
/* Identifier constants */
/*----------------------*/

/* Timer IDs */
#define ID_TIMER  1


/*-----------*/
/* Functions */
/*-----------*/
LRESULT CALLBACK ClientWndProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK TranslateDlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK RotateDlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK TimeDlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK SpeedDlgProc(HWND, UINT, WPARAM, LPARAM);
BOOL CALLBACK ProdInfoDlgProc(HWND, UINT, WPARAM, LPARAM);
void SetupPixelFormat(HDC);
void SetupPalette(HDC);
void DisplayError(char *);
