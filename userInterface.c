//Willis A. Hershey
//This file contains platform specific implementations of the functions defined in userInterface.h

#include "codeshopDefs.h"
#include "userInterface.h"
#include "fileReps.h"

#ifdef WINDOWS
//Windows specific code here

//Windows specific function to handle window-events
LPARAM CALLBACK WndProc(HWND windowHandle,unsigned message,WPARAM wparam,LPARAM lparam){
  printf("%u\n",message);
  return DefWindowProc(windowHandle,message,wparam,lparam);
}

//Windows implementation for openMainWindow
int openMainWindow(EFILEList *userFiles){
  HINSTANCE hInstance=(HINSTANCE)GetModuleHandle(NULL);
  WNDCLASSEX windowClass=(WNDCLASSEX){
  	.cbSize=sizeof(WNDCLASSEX),
	.style=0,
	.lpfnWndProc=WndProc,
	.cbClsExtra=0,
	.cbWndExtra=0,
	.hInstance=hInstance,
	.hIcon=LoadIcon(NULL,IDI_APPLICATION),
	.hCursor=LoadCursor(NULL,IDC_ARROW),
	.hbrBackground=(HBRUSH)(COLOR_WINDOW+1),
	.lpszMenuName=NULL,
	.lpszClassName=windowClassName,
	.hIconSm=LoadIcon(NULL,IDI_APPLICATION)
  };
  HWND windowHandle;
  MSG message;
  if(!RegisterClassEx(&windowClass)){
	MessageBox(NULL,"Something went wrong when the program attempted to register the window class\n\nThe program will now terminate", "Error!",MB_ICONEXCLAMATION | MB_OK);
	return FAILURE;
  }
  if(!(windowHandle=CreateWindowEx(WS_EX_APPWINDOW|WS_EX_CLIENTEDGE,windowClassName,efile->name,WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,240,120,NULL,NULL,hInstance,NULL))){
	MessageBox(NULL,"Something went wrong when the program attempted to create a window of the successfully registered class\n\nThe program will now terminate","Error!",MB_ICONEXCLAMATION | MB_OK);
	return FAILURE;
  }
  ShowWindow(windowHandle,SW_MAXIMIZE);
  UpdateWindow(windowHandle);
  while(GetMessage(&message,NULL,0,0)>0){
	TranslateMessage(&message);
	DispatchMessage(&message);
  }
  return SUCCESS;
}

int messageWindow(const char *message,const char *header,enum messageType type,WINDOW_HANDLE windowHandle){
  if(type==notice)
	MessageBox(windowHandle,message,header,MS_OK);
  else if(type==error)
	messageBox(windowHandle,message,header,MS_ICONEXCLAMATION|MS_OK);
  else
	return FAILURE;
  return SUCCESS;
}

#else

//Linux specific code

int openMainWindow(EFILEList *userFiles){
  return FAILURE;
}

int messageWindow(const char *message,const char *header,enum messageType type,WINDOW_HANDLE windowHandle){
  return FAILURE;
}

#endif

int resolveFailedToOpenForRead(EFILEList *filelist,char *filename){
  //This function is called when the program tries and fails to open a file that is supposed to already exist

  //Show user filename that failed and ask if they'd like to provide another name, or abort
  //If they provide another name, it should be passed to fopen with flag r+, and added to filelist if successful

  //Until this is implemented this function implements abort
  return FAILURE;
}

int resolveFailedToOpenFoWrite(EFILEList *filelist,EFILE *efile){
  //This function is called when the program tries and fails to open a file for writing with the filename stored at efile->name
 
  //Show user filename that failed and ask them to either provide another name, or abort and signal the calling function to free the EFILE.
  //If they provide another name, it should be passed to fopen with flag w+, efile should be realloced to have space for the new name,
  //and the new name should be written at efile->name, and the FILE* should be saved at efile->fd, then the surrounding
  //EFILEs should be linked to the new memory

  //Until this is implemented this function implements abort
  removeEFILEListAndFree(filelist,efile);
  return FAILURE;
}
