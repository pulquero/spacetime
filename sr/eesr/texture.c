
#include "texture.h"
#include <stdio.h>

/*
* Loads a 24bit bitmap file into an RGB buffer.
* buffer should have a minimum size of length
* length should be set to 3*width*height
*/
void Load24BitBitmap(char *filename,char *buffer,int length)
{
   int i;
#if defined(__OS2__)
   HFILE hFile = NULL;
   ULONG ulAction = 0;
   ULONG ulBytesRead = 0;
   APIRET rc;

   rc = DosOpen(filename,&hFile,&ulAction,0,FILE_ARCHIVED | FILE_NORMAL,
           OPEN_ACTION_FAIL_IF_NEW | OPEN_ACTION_OPEN_IF_EXISTS,
           OPEN_FLAGS_NOINHERIT | OPEN_SHARE_DENYNONE | OPEN_ACCESS_READONLY,0);
   if(rc != NO_ERROR) {
      fprintf(stderr, "Error opening file %s.\n", filename);
      return;
   }
   DosRead(hFile,buffer,length,&ulBytesRead);
   DosClose(hFile);
#elif defined(__WIN32__)
   HANDLE hFile = NULL;
   DWORD bytesRead = 0;

   hFile = CreateFile(filename,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,
                    FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,NULL);
   if(hFile == INVALID_HANDLE_VALUE) {
      fprintf(stderr, "Error opening file %s.\n", filename);
      return;
   }
   ReadFile(hFile,buffer,length,&bytesRead,NULL);
   CloseHandle(hFile);
#else
   FILE *fp = NULL;

   fp = fopen(filename,"rb");
   if(fp == NULL) {
      fprintf(stderr, "Error opening file %s.\n", filename);
      return;
   }
   fread(buffer,sizeof(char),length,fp);
   fclose(fp);
#endif

   for(i=0; i<length; i+=3) {
      buffer[i] = buffer[22+i];
      buffer[i+1] = buffer[21+i];
      buffer[i+2] = buffer[20+i];
   }
}
