//Willis A. Hershey
//Last edited April 17th 2020

#include <stdio.h>
#include <stdarg.h>

#include "userInterface.h"
#include "fileReps.h"
#include "codeshopDefs.h"

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
