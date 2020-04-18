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

//EFILEs are abstract file representations containing linked lists of lines so that editing can take place without enourmous processing overhead
//they also link together nicely to form a linked list of files being edited
typedef struct EFILEStruct{
  struct EFILEStruct *next;
  struct EFILEStruct *prev;
  FILE *fd;
  lineNode *head;
  lineNode *tail;
  int numLines;
  char name[0];
}EFILE;

typedef struct{
  EFILE *head;
  EFILE *tail;
}EFILEList;

EFILE* makeEFILE(FILE*,char*);
EFILE* makeEmptyEFILE(char*);
void freeEFILE(EFILE*);
int EFILEAppendLine(EFILE*,char*);
void writeEFILE(EFILE*);
void writeAndFreeEFILE(EFILE*);
void removeEFILEListAndFree(EFILEList*,EFILE*);
void freeEFILEList(EFILEList*);
void printEFILE(EFILE*);

#endif
