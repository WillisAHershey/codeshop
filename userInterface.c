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
  switch(message){
	case WM_CLOSE:
		DestroyWindow(windowHandle);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
	      return DefWindowProc(windowHandle,message,wparam,lparam);
  }
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
  if(!(windowHandle=CreateWindowEx(WS_EX_APPWINDOW|WS_EX_CLIENTEDGE,windowClassName,userFiles->head->name,WS_OVERLAPPEDWINDOW,CW_USEDEFAULT,CW_USEDEFAULT,240,120,NULL,NULL,hInstance,NULL))){
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
	MessageBox(windowHandle,message,header,MB_OK);
  else if(type==error)
	MessageBox(windowHandle,message,header,MB_ICONEXCLAMATION|MB_OK);
  else
	return FAILURE;
  return SUCCESS;
}

#elif defined __APPLE__

//MacOs specfic code

int openMainWindow(EFILEList *userFiles){
  return FAILURE;
}

int messageWindow(const char *message,const char *header,enum messageType type,WINDOW_HANDLE windowHandle){
  return FAILURE;
}

#else

#include <stdio.h> //For testing

//Linux specific code

void eventLoop(WINDOW_HANDLE handle){
  char buf[10];
  int loop=1;
  Atom deleteWindow=XInternAtom(handle->display,"WM_DELETE_WINDOW",True);
  XSetWMProtocols(handle->display,handle->window,&deleteWindow,1);
  while(loop){
	XEvent event;
	KeySym key;
	XNextEvent(handle->display,&event);
	switch(event.type){
		case KeyPress:
			printf("KeyPress caught:keycode=%u ",event.xkey.keycode);
			if(XLookupString(&event.xkey,buf,10,&key,0)==1)
				printf("%c (%d)\n",buf[0],buf[0]);
			else{
				if(event.xkey.state==ShiftMask)
					printf("Shift key\n");
				else if(event.xkey.state==LockMask)
					printf("Lock key\n");
				else if(event.xkey.state==ControlMask)
					printf("Lock key\n");
				else if(event.xkey.state==Mod1Mask)
					printf("Mod1 key\n");
				else if(event.xkey.state==Mod2Mask)
					printf("Mod2 key\n");
				else if(event.xkey.state==Mod3Mask)
					printf("Mod3 key\n");
				else if(event.xkey.state==Mod4Mask)
					printf("Mod4 key\n");
				else if(event.xkey.state==Mod5Mask)
					printf("Mod5 key\n");
				else
					printf("state=%u Parser error\n",event.xkey.state);
			}
			break;
		case KeyRelease:
			printf("KeyRelease caught:keycode=%u ",event.xkey.keycode);
			if(XLookupString(&event.xkey,buf,10,&key,0)==1)
				printf("%c (%d)\n",buf[0],buf[0]);
			else{
				if(event.xkey.state==ShiftMask)
					printf("Shift key\n");
				else if(event.xkey.state==LockMask)
					printf("Lock key\n");
				else if(event.xkey.state==ControlMask)
					printf("Lock key\n");
				else if(event.xkey.state==Mod1Mask)
					printf("Mod1 key\n");
				else if(event.xkey.state==Mod2Mask)
					printf("Mod2 key\n");
				else if(event.xkey.state==Mod3Mask)
					printf("Mod3 key\n");
				else if(event.xkey.state==Mod4Mask)
					printf("Mod4 key\n");
				else if(event.xkey.state==Mod5Mask)
					printf("Mod5 key\n");
				else
					printf("state=%u Parser error\n",event.xkey.state);
			}
			break;
		case ButtonPress:
			printf("ButtonPress caught:Click by %u at (%d,%d)\n",event.xbutton.button,event.xbutton.x,event.xbutton.y);
			break;
		case ButtonRelease:
			printf("ButtonRelease caught:Release by %u at (%d,%d)\n",event.xbutton.button,event.xbutton.x,event.xbutton.y);
			break;
		case Expose:
			printf("Expose caught:x=%d y=%d width=%d height=%d count=%d\n",event.xexpose.x,event.xexpose.y,event.xexpose.width,event.xexpose.height,event.xexpose.count);
			if(event.xexpose.count==0)
				XClearWindow(handle->display,handle->window);
			break;
		case ClientMessage:
			printf("ClientMessage caught: ");
			if(event.xclient.format==32&&event.xclient.data.l[0]==deleteWindow){
				loop=0;
				printf("Destroy window message. Exiting loop\n");
			}
			else
				printf("Unrecognized client message\n");

			break;
		default:
			printf("Unknown event (%d) caught and ignored\n",event.type);
			break;
	}
  }
}

//X11 specific implementation of openMainWindow
int openMainWindow(EFILEList *userFiles){
  x11Handle handle;
  handle.display=XOpenDisplay(NULL);
  handle.screen=DefaultScreenOfDisplay(handle.display);
  XSetWindowAttributes swa;
  swa.background_pixel=WhitePixelOfScreen(handle.screen);
  swa.bit_gravity=NorthWestGravity;
  swa.border_pixel=BlackPixelOfScreen(handle.screen);
  swa.colormap=DefaultColormapOfScreen(handle.screen);
  swa.cursor=None;
  swa.win_gravity=NorthGravity;
  handle.window=XCreateWindow(handle.display,RootWindowOfScreen(handle.screen),0,0,600,600,0,CopyFromParent,CopyFromParent,CopyFromParent,CWBackPixel|CWBitGravity|CWBorderPixel|CWColormap|CWCursor|CWWinGravity,&swa);
  XSetStandardProperties(handle.display,handle.window,userFiles->head->name,"Name of icon pixmap",None,NULL,0,NULL);
  XSelectInput(handle.display,handle.window,ExposureMask|ButtonPressMask|ButtonReleaseMask|KeyPressMask|KeyReleaseMask);
  GC gc=XCreateGC(handle.display,handle.window,0,0);
  XClearWindow(handle.display,handle.window);
  XMapRaised(handle.display,handle.window);
  eventLoop(&handle);
  XFreeGC(handle.display,gc);
  XDestroyWindow(handle.display,handle.window);
  XCloseDisplay(handle.display);
  return SUCCESS;
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
