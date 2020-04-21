//Willis A. Hershey
//Last updated April 17th 2020

//This file aims to provide user interface functions that the back-end can call when needed
#ifndef __USERINTERFACE_
#define __USERINTERFACE_

#include <stdio.h>

#ifdef _WIN_32
#include <windows.h>
#else
//Include some linux library for msking a window pop up
#endif

#include "fileReps.h"

int openEditWindow(EFILE*);
int resolveFailedToOpenForRead(EFILEList*,char*);
int resolveFailedToOpenForWrite(EFILEList*,EFILE*);

#endif
