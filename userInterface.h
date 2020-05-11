//Willis A. Hershey
//This file aims to provide user interface functions that the back-end can call when needed

#ifndef __USERINTERFACE_
#define __USERINTERFACE_

#ifdef _MSC_VER

#	define WINDOWS _MSC_VER

#endif

#ifdef WINDOWS

#	include <windows.h>

//This allows this file to compile on Windows systems
#	pragma comment(linker,"/subsystem:windows /ENTRY:mainCRTStartup")
#	pragma comment(lib,"user32.lib")
//WINDOW_HANDLE is how codeshop code refers to window handles, but Windows calls them HWND
#	define WINDOW_HANDLE HWND

#elif defined __APPLE__

//MacOs specific stuff

//This will be something else when I get around to writing a Quartz gui
#	define WINDOW_HANDLE void*

#else
//Linux specific stuff

//These are the developer files, so this will only build if you have them installed
#	include <X11/Xlib.h>
#	include <X11/Xutil.h>
#	include <X11/Xos.h>

	typedef struct{
		Display *display;
		Screen *screen;
		Window window;
	}x11Handle;

#	define WINDOW_HANDLE x11Handle*
//WINDOW_HANDLE is how codeshop refers to window handles, but X11 calls them Display pointers

#endif

const static char windowClassName[] = "Codeshop coding environment";
const static char defaultFileName[] = "<noname>.c";

#include "fileReps.h"

enum messageType{notice,error,yesNoQuestion};

int openMainWindow(EFILEList *userFiles);
int messageWindow(const char*,const char*,enum messageType,WINDOW_HANDLE);
int resolveFailedToOpenForRead(EFILEList*,char*);
int resolveFailedToOpenForWrite(EFILEList*,EFILE*);
//int resolveEFILELeftUnwritten(EFILEList*,EFILE*);

#endif
