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

/* Window IDs */
#define IDW_BASE             100
#define IDW_FRAME_WINDOW     101

/* Menu Item IDs */
#define IDM_BASE             1000
#define IDM_CTRL             IDM_BASE+10
#define IDM_CTRL_TRA         IDM_CTRL+1
#define IDM_CTRL_ROT         IDM_CTRL+2
#define IDM_CTRL_TIM         IDM_CTRL+3
#define IDM_CTRL_SPE         IDM_CTRL+4
#define IDM_OPT              IDM_BASE+20
#define IDM_OPT_FRAME        IDM_OPT+1
#define IDM_FR_WORLD         IDM_OPT+2
#define IDM_FR_OBJECT        IDM_OPT+3
#define IDM_OPT_OBJ          IDM_OPT+4
#define IDM_OBJ_LATTICE      IDM_OPT+5
#define IDM_OBJ_CAR          IDM_OPT+6
#define IDM_OBJ_PLANE        IDM_OPT+7
#define IDM_OBJ_SPHERE       IDM_OPT+8
#define IDM_OBJ_3DS          IDM_OPT+9
#define IDM_OPT_DOPPLER      IDM_OPT+20
#define IDM_OPT_FREEZE       IDM_OPT+21
#define IDM_OPT_LOOP         IDM_OPT+22
#define IDM_GRAPH            IDM_BASE+50
#define IDM_GRAPH_DETAIL     IDM_GRAPH+1
#define IDM_GRAPH_FOG        IDM_GRAPH+2
#define IDM_GRAPH_TEX        IDM_GRAPH+3
#define IDM_GRAPH_WIRE       IDM_GRAPH+4
#define IDM_HELP             IDM_BASE+60
#define IDM_HELP_INDEX       IDM_HELP+1
#define IDM_HELP_GEN         IDM_HELP+2
#define IDM_HELP_USING       IDM_HELP+3
#define IDM_HELP_ABOUT       IDM_HELP+4

/* Dialog IDs */
#define IDD_BASE             2000
#define IDD_PRODINFO         2001
#define IDI_PRODINFO_ICON    2002
#define IDT_PRODINFO_TITLE   2003
#define IDT_PRODINFO_VERSION 2004
#define IDT_PRODINFO_AUTHOR  2005
#define IDT_PRODINFO_EMAIL   2006
#define IDD_TRA              2101
#define IDP_TRA_INCX         2102
#define IDP_TRA_DECX         2103
#define IDP_TRA_INCY         2104
#define IDP_TRA_DECY         2105
#define IDP_TRA_INCZ         2106
#define IDP_TRA_DECZ         2107
#define IDC_TRA_SCALE        2108
#define IDD_ROT              2201
#define IDS_ROT_X            2202
#define IDS_ROT_Y            2203
#define IDS_ROT_Z            2204
#define IDT_ROT_TEXTX        2205
#define IDT_ROT_TEXTY        2206
#define IDT_ROT_TEXTZ        2207
#define IDC_ROT_ANGLE        2208
#define IDD_TIM              2301
#define IDP_TIM_INC          2302
#define IDP_TIM_DEC          2303
#define IDC_TIM_SCALE        2305
#define IDD_SPE              2401
#define IDS_SPE_BAR          2402
#define IDD_DETAIL           2501
#define IDT_DETAIL_TEXT      2502
#define IDE_DETAIL_FIELD     2503

/* Help IDs */
#define IDH_MAIN_HELPTABLE   5001
#define IDH_SUBTABLE_FRAME   5002
#define IDH_SUBTABLE_TRA     5003
#define IDH_SUBTABLE_ROT     5004
#define IDH_SUBTABLE_TIM     5005
#define IDH_SUBTABLE_SPE     5006
#define IDH_SUBTABLE_DETAIL  5007


/*-----------*/
/* Functions */
/*-----------*/
MRESULT EXPENTRY ClientWndProc(HWND, ULONG, MPARAM, MPARAM);
MRESULT EXPENTRY TranslateDlgProc(HWND, ULONG, MPARAM, MPARAM);
MRESULT EXPENTRY RotateDlgProc(HWND, ULONG, MPARAM, MPARAM);
MRESULT EXPENTRY TimeDlgProc(HWND, ULONG, MPARAM, MPARAM);
MRESULT EXPENTRY SpeedDlgProc(HWND, ULONG, MPARAM, MPARAM);
MRESULT EXPENTRY DetailDlgProc(HWND, ULONG, MPARAM, MPARAM);
void HelpInit(void);
int DisplayError(PSZ);

#ifdef HAVE_LIB3DS
void LINKAGE draw3DS(void);
void LINKAGE draw3DSNode(Lib3dsNode *);
#endif


