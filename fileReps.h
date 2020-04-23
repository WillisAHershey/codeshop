//Willis A. Hershey
//Last updated April 17th 2020
//
//This file contains definitions of structures and related functions used in the codeshop project to represent files as they are being read and edited
#ifndef __FILEREPS_
#define __FILEREPS_

#define LINE_BUF_LEN 257

typedef struct lineNodeStruct{
  struct lineNodeStruct *next;
  struct lineNodeStruct *prev;
  char line[0];
}lineNode;

enum editType{DELETION,INSERTION,LINEEDIT};

//EFILEs are abstract file representations containing linked lists of lines so that editing can take place without enourmous processing overhead
//they also link together nicely to form a linked list of files being edited

//this is a temporary hush-error
#define editLog void

typedef struct EFILEStruct{
  struct EFILEStruct *next;
  struct EFILEStruct *prev;
  lineNode *head;
  lineNode *tail;
  editLog *edits;
  int numLines;
  char name[0];
}EFILE;

typedef struct{
  EFILE *head;
  EFILE *tail;
}EFILEList;

EFILE* makeEFILE(char*);
EFILE* makeEmptyEFILE(char*);
void freeEFILE(EFILE*);
int writeEFILE(EFILE*);
int writeAndFreeEFILE(EFILE*);
void freeEFILEList(EFILEList*);
void removeEFILEListAndFree(EFILEList*,EFILE*);
void printEFILE(EFILE*);
int EFILEAppendLine(EFILE*,char*);
int EFILEDeleteLine(EFILE*,int);

#endif
