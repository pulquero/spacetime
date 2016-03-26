
#if defined(__OS2__)
#define INCL_DOSFILEMGR
#include <os2.h>
#elif defined(__WIN32__)
#include <windows.h>
#endif

void Load24BitBitmap(char *filename,char *buffer,int length);

