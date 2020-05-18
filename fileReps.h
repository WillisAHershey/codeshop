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

enum edit{DELETION,INSERTION,LINE_EDIT};

//EFILEs are abstract file representations containing linked lists of lines so that editing can take place without enourmous processing overhead
//they also link together nicely to form a linked list of files being edited

typedef struct{
  int index;
  lineNode *start;
  lineNode *end;
}deletionLog;

typedef struct{
  int index;
  int numLines;
}insertionLog;

typedef struct{
  int index;
  char oldLine[];
}lineEditLog;

typedef struct editLogStruct{
  struct editLogStruct *next;
  enum edit editType;
  union{
	deletionLog deletion;
	insertionLog insertion;
	lineEditLog lineEdit;
  };
}editLog;

typedef struct EFILEStruct{
  struct EFILEStruct *next;
  struct EFILEStruct *prev;
  lineNode *head;
  lineNode *tail;
  editLog *edits;
  editLog *redo;
  char name[];
}EFILE;

#define EMPTY_EFILE_NO_NAME (EFILE){.next=NULL,.prev=NULL,.head=NULL,.tail=NULL,.edits=NULL,.redo=NULL}

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

EFILE* makeEFILE(const char*);
EFILE* makeEmptyEFILE(const char*);
void freeEFILE(EFILE*);
int writeEFILE(EFILE*);
void freeEFILEList(EFILEList*);
void removeEFILEListAndFree(EFILEList*,EFILE*);
void printEFILE(EFILE*);
int EFILEInsertLines(EFILE*,int,lineNode*,lineNode*);
int EFILEDeleteLines(EFILE*,int,int);
int renameEFILE(EFILEList*,EFILE*,const char*);

#endif
