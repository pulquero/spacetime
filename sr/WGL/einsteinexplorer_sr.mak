# Microsoft Developer Studio Generated NMAKE File, Format Version 40001
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

!IF "$(CFG)" == ""
CFG=EinsteinExplorer_SR - Win32 Release
!MESSAGE No configuration specified.  Defaulting to EinsteinExplorer_SR - Win32\
 Release.
!ENDIF 

!IF "$(CFG)" != "EinsteinExplorer_SR - Win32 Release"
!MESSAGE Invalid configuration "$(CFG)" specified.
!MESSAGE You can specify a configuration when running NMAKE on this makefile
!MESSAGE by defining the macro CFG on the command line.  For example:
!MESSAGE 
!MESSAGE NMAKE /f "einsteinexplorer_sr.mak"\
 CFG="EinsteinExplorer_SR - Win32 Release"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "EinsteinExplorer_SR - Win32 Release" (based on\
 "Win32 (x86) Application")
!MESSAGE 
!ERROR An invalid configuration is specified.
!ENDIF 

!IF "$(OS)" == "Windows_NT"
NULL=
!ELSE 
NULL=nul
!ENDIF 
################################################################################
# Begin Project
# PROP Target_Last_Scanned "EinsteinExplorer_SR - Win32 Release"
CPP=cl.exe
RSC=rc.exe
MTL=mktyplib.exe
# PROP BASE Use_MFC 0
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 0
# PROP Use_Debug_Libraries 0
# PROP Output_Dir ""
# PROP Intermediate_Dir ""
# PROP Target_Dir ""
OUTDIR=.
INTDIR=.

ALL : "$(OUTDIR)\einsteinexplorer_sr.exe"

CLEAN : 
	-@erase ".\einsteinexplorer_sr.exe"
	-@erase ".\eesr.obj"
	-@erase ".\srel.obj"
	-@erase ".\einsteinexplorer-sr.obj"
	-@erase ".\einsteinexplorer-sr.res"

# ADD BASE CPP /nologo /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /YX /c
# ADD CPP /nologo /G5 /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D _Inline=_inline /c
# SUBTRACT CPP /YX
# ADD BASE MTL /nologo /D "NDEBUG" /win32
# ADD MTL /nologo /D "NDEBUG" /win32
# ADD BASE RSC /l 0x809 /d "NDEBUG"
# ADD RSC /l 0x809 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
BSC32_FLAGS=/nologo /o"$(OUTDIR)/einsteinexplorer_sr.bsc" 
BSC32_SBRS=
LINK32=link.exe
# ADD BASE LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib ole32.lib oleaut32.lib uuid.lib odbc32.lib odbccp32.lib /nologo /subsystem:windows /machine:I386
# ADD LINK32 kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib advapi32.lib shell32.lib uuid.lib comctl32.lib opengl32.lib glu32.lib /nologo /stack:0x989680,0x1e8480 /subsystem:windows /machine:I386
LINK32_FLAGS=kernel32.lib user32.lib gdi32.lib winspool.lib comdlg32.lib\
 advapi32.lib shell32.lib uuid.lib comctl32.lib opengl32.lib glu32.lib /nologo\
 /stack:0x989680,0x1e8480 /subsystem:windows /incremental:no\
 /pdb:"$(OUTDIR)/einsteinexplorer_sr.pdb" /machine:I386\
 /out:"$(OUTDIR)/einsteinexplorer_sr.exe" 
LINK32_OBJS= \
	"$(INTDIR)/eesr.obj" \
	"$(INTDIR)/srel.obj" \
	"$(INTDIR)/einsteinexplorer-sr.obj" \
	"$(INTDIR)/einsteinexplorer-sr.res"

"$(OUTDIR)\einsteinexplorer_sr.exe" : "$(OUTDIR)" $(DEF_FILE) $(LINK32_OBJS)
    $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<

CPP_PROJ=/nologo /G5 /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D\
 _Inline=_inline /c 

.c.obj:
   $(CPP) $(CPP_PROJ) $<  

.cpp.obj:
   $(CPP) $(CPP_PROJ) $<  

.cxx.obj:
   $(CPP) $(CPP_PROJ) $<  

.c.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cpp.sbr:
   $(CPP) $(CPP_PROJ) $<  

.cxx.sbr:
   $(CPP) $(CPP_PROJ) $<  

RSC_PROJ=/l 0x809 /fo"$(INTDIR)/einsteinexplorer-sr.res" /d "NDEBUG" 
MTL_PROJ=/nologo /D "NDEBUG" /win32 
################################################################################
# Begin Target

# Name "EinsteinExplorer_SR - Win32 Release"
################################################################################
# Begin Source File

SOURCE=.\srel.c
DEP_CPP_SREL_=\
	".\srel.h"\
	{$(INCLUDE)}"\gl\GL.H"\
	

"$(INTDIR)\srel.obj" : $(SOURCE) $(DEP_CPP_SREL_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=.\eesr.c
DEP_CPP_EESR_=\
	".\eesr.h"\
	
NODEP_CPP_EESR_=\
	"..\srel\srel.h"\
	

"$(INTDIR)\eesr.obj" : $(SOURCE) $(DEP_CPP_EESR_) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=".\einsteinexplorer-sr.c"
DEP_CPP_EINST=\
	".\srel.h"\
	".\eesr.h"\
	".\EinsteinExplorer-SR.h"\
	".\..\glos.h"\
	{$(INCLUDE)}"\gl\GL.H"\
	

"$(INTDIR)\einsteinexplorer-sr.obj" : $(SOURCE) $(DEP_CPP_EINST) "$(INTDIR)"


# End Source File
################################################################################
# Begin Source File

SOURCE=".\einsteinexplorer-sr.rc"

"$(INTDIR)\einsteinexplorer-sr.res" : $(SOURCE) "$(INTDIR)"
   $(RSC) $(RSC_PROJ) $(SOURCE)


# End Source File
# End Target
# End Project
################################################################################
