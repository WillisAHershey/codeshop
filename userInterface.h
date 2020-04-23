//Willis A. Hershey
//Last updated April 17th 2020

//This file aims to provide user interface functions that the back-end can call when needed
#ifndef __USERINTERFACE_
#define __USERINTERFACE_

#ifdef _MSC_VER
#define WINDOWS _MSC_VER
#endif

#include <stdio.h>

#ifdef WINDOWS

#include <windows.h>
#pragma comment(linker,"/subsystem:windows /ENTRY:mainCRTStartup")
#pragma comment(lib,"user32.lib")

#else
//Include some linux library for making a window pop up
#endif

#include "fileReps.h"

int openEditWindow(EFILE*);
int resolveFailedToOpenForRead(EFILEList*,char*);
int resolveFailedToOpenForWrite(EFILEList*,EFILE*);

#endif
