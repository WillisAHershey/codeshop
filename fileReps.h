//Willis A. Hershey
//This file contains definitions of structures and related functions used in the codeshop project to represent files as they are being read and edited

#ifndef __FILEREPS_
#define __FILEREPS_

#define LINE_BUF_LEN 257

typedef struct lineNodeStruct{
  struct lineNodeStruct *next;
  struct lineNodeStruct *prev;
  char line[];
}lineNode;

#define EMPTY_LINENODE_NO_LINE (lineNode){.next=NULL,.prev=NULL}

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
  lineNode *cursor;
  editLog *edits;
  int numLines;
  char name[];
}EFILE;

#define EMPTY_EFILE_NO_NAME (EFILE){.next=NULL,.prev=NULL,.head=NULL,.tail=NULL,.cursor=NULL,.edits=NULL,.numLines=0}

typedef struct fileErrorStruct{
  struct fileErrorStruct *next;
  const char *filename;
}fileError;

typedef struct{
  EFILE *head;
  EFILE *tail;
  fileError *errorHead;
  fileError *errorTail;
}EFILEList;

#define EMPTY_EFILE_LIST (EFILEList){.head=NULL,.tail=NULL,.errorHead=NULL,.errorTail=NULL}

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
