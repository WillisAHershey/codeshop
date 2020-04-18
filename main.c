//Willis A. Hershey
//Last edited April 17th 2020

//This file is intended to be the backbone of the entire back-end portion of the program
//So it is in my best interest to make this code as portable as humanly possible
//This code is written to work in Linux, but I plan to add Macros to translate POSIX standard functions to WIN_32 functions sometime later

#include <stdio.h>  //FILE fopen fseek fclose
#include <stdlib.h> //malloc realloc free size_t
#include <string.h> //strlen strcpy

#include "codeshopDefs.h"

#include "fileReps.h"
#include "userInterface.h"

//USAGE: ./codeshop | ./codeshop fileName

//Once the application is running, more files can be (and most likely will be automatically) opened

//The goal is to create a portable underlying file editing path, so that GUI can be implemented on top after the underyling implementation is working


int main(int args,char *argv[]){
  //I do not anticipate being passed more than one filename but at the moment I do not want to make this an error so I'll just open all non-first arguments
  //For the time being I am going to assume that the mode this program was run in will give me read access to filename if it exists
 
  //userFiles will be a continuously updated linked list of open EFILES
  EFILEList userFiles=(EFILEList){.head=NULL,.tail=NULL};
  FILE *fp;
  EFILE *efile;
  int c;
  for(c=1;c<args;++c){
	fp=fopen(argv[c],"r");
	if(fp){
		efile=makeEFILE(fp,argv[c]);
		if(userFiles.head){
			userFiles.tail->next=efile;
			userFiles.tail=efile;
		}
		else
			userFiles.head=userFiles.tail=efile;
	}
	else
		resolveFailedToOpenForRead(&userFiles,argv[c]);
  }
  printEFILE(userFiles.head);
  freeEFILEList(&userFiles);
}
