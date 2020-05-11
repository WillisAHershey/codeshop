//Willis A. Hershey

//This file is intended to be the backbone of the entire back-end portion of the program
//So it is in my best interest to make this code as portable as humanly possible

#include <stdio.h>  //FILE fopen fclose
#include <stdlib.h> //malloc realloc free size_t
#include <string.h> //strlen strcpy

#include "codeshopDefs.h" //SUCCESS FAILURE

#include "fileReps.h" //EFILE makeEFILE makeEmptyEFILE FreeEFILEList
#include "userInterface.h" //openMainWindow

//USAGE: ./codeshop | ./codeshop fileName

//Once the application is running, more files can be (and most likely will be automatically) opened

//The goal is to create a portable underlying file editing path, so that GUI can be implemented on top after the underyling implementation is working

int main(int args,char *argv[]){
  //I do not anticipate being passed more than one filename but at the moment I do not want to make this an error so I'll just open all non-first arguments
  //For the time being I am going to assume that the mode this program was run in will give me read and write access to filename if it exists
 
  //userFiles will be a continuously updated set of linked lists representing the files being worked on
  auto EFILEList userFiles=EMPTY_EFILE_LIST;
  register int c;
  register EFILE *efile;
  for(c=1;c<args;++c){
	if((efile=makeEFILE(argv[c]))){
		//If file opens successfully add it to EFILE linked list
		if(userFiles.head){
			userFiles.tail->next=efile;
			efile->prev=userFiles.tail;
			userFiles.tail=efile;
		}
		else
			userFiles.head=userFiles.tail=efile;
	}
	else{
		//Otherwise add the name of the file that failed to open to the error queue
		if(userFiles.errorTail){
			userFiles.errorTail->next=malloc(sizeof(fileError));
			userFiles.errorTail=userFiles.errorTail->next;
			*userFiles.errorTail=(fileError){.next=NULL,.filename=argv[c]};
		}
		else{
			userFiles.errorHead=userFiles.errorTail=malloc(sizeof(fileError));
			*userFiles.errorHead=(fileError){.next=NULL,.filename=argv[c]};
		}
	}
  }
  if(!userFiles.head) //If no files successfully opened, create a dummy EFILE
	userFiles.head=userFiles.tail=makeEmptyEFILE(NULL);
  //And pass control to the GUI
  openMainWindow(&userFiles);
  printf("Control made it back to main\n");
  //When GUI is closed, clean up and exit
  freeEFILEList(&userFiles);
}
