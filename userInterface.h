//Willis A. Hershey
//This file aims to provide user interface functions that the back-end can call when needed

#ifndef __USERINTERFACE_
#define __USERINTERFACE_

#ifdef _MSC_VER

#	define WINDOWS _MSC_VER

#endif

#include <stdio.h>

#ifdef WINDOWS

#	include <windows.h>
//This allows this file to compile on Windows systems
#	pragma comment(linker,"/subsystem:windows /ENTRY:mainCRTStartup")
#	pragma comment(lib,"user32.lib")
//WINDOW_HANDLE is how codeshop code refers to window handles, but Windows calls them HWND
#	define WINDOW_HANDLE HWND

#else

	//Include some linux library for making a window pop up
//This sholuld be something else
#	define WINDOW_HANDLE void*

#endif

static const char windowClassName[] = "Codeshop coding environment";
static const char defaultFileName[] = "<noname>.c";

#include "fileReps.h"

enum messageType{notice,error,yesNoQuestion};

int openMainWindow(EFILEList *userFiles);
int messageWindow(const char*,const char*,enum messageType,WINDOW_HANDLE);
int resolveFailedToOpenForRead(EFILEList*,char*);
int resolveFailedToOpenForWrite(EFILEList*,EFILE*);

#endif
