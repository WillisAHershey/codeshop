//Willis Hershey
//This file contains the implementations of the functions in fileReps.h

#include <stdio.h> //fopen fclose fgets
#include <stdlib.h>//malloc free NULL
#include <string.h>//strlen

#include "codeshopDefs.h"
#include "fileReps.h"
#include "userInterface.h"

//Not available in .h file, so not linkable.
void logEdit(EFILE *efile,enum editType edit,int spot,int numLines,void *a){
  return;
}

EFILE* makeEFILE(char *filename){
  FILE *file=fopen(filename,"r+");
  if(!file)
	return NULL;
  EFILE *out=malloc(sizeof(EFILE)+strlen(filename)+sizeof(NULL_TERMINATOR));
  *out=EMPTY_EFILE_NO_NAME;
  strcpy(out->name,filename);
  int len, overflow;
  lineNode *pt;
  char buf[LINE_BUF_LEN];
  int numLines=0;
  while(fgets(buf,LINE_BUF_LEN,file)){
	pt=malloc(sizeof(lineNode)+(len=strnlen(buf,LINE_BUF_LEN-1))+sizeof(NULL_TERMINATOR));
	*pt=EMPTY_LINENODE_NO_LINE;
	strcpy(pt->line,buf);
	pt->prev=out->tail;
	if(len==LINE_BUF_LEN-1&&buf[LINE_BUF_LEN-2]!='\n'){
		overflow=1;
		while(fgets(buf,LINE_BUF_LEN,file)){
			pt=realloc(pt,sizeof(lineNode)+(len=strnlen(buf,LINE_BUF_LEN-1))+overflow*(LINE_BUF_LEN-1)+1);
			strcpy(pt->line+(LINE_BUF_LEN-1)*overflow,buf);
			if(len<LINE_BUF_LEN-1||buf[LINE_BUF_LEN-2]=='\n'){
				pt->line[(LINE_BUF_LEN-1)*overflow+len-1]='\0';
				break;
			}
			else
				++overflow;
		}
	}
	else
		pt->line[len-1]='\0';
	if(out->head){
		out->tail->next=pt;
		out->tail=pt;
	}
	else
		out->head=out->tail=pt;
	++numLines;
  }
  out->numLines=numLines;
  fclose(file);
  return out;
}

EFILE* makeEmptyEFILE(char *filename){
  size_t len=filename?strlen(filename):0;
  EFILE *out=malloc(sizeof(EFILE)+len+sizeof(NULL_TERMINATOR));
  *out=EMPTY_EFILE_NO_NAME;
  if(filename)
	strcpy(out->name,filename);
  else
	out->name[0]='\0';
  return out;
}

void freeEFILE(EFILE *efile){
  lineNode *pt=efile->head;
  lineNode *run;
  while(pt){
	run=pt;
	pt=pt->next;
	free(run);
  }
  //free edits
  free(efile);
}

int writeEFILE(EFILE *efile){
  FILE *fd=fopen(efile->name,"w+");
  if(!fd)
	return FAILURE;
  lineNode *pt=efile->head;
  while(pt){
	fprintf(fd,"%s\n",pt->line);
	pt=pt->next;
  }
  fclose(fd);
  //free and clear edits
  return SUCCESS;
}

void freeEFILEList(EFILEList *list){
  EFILE *efile=list->head;
  EFILE *run;
  while(efile){
	run=efile;
	efile=efile->next;
	freeEFILE(run);
  }
  //handle error list
}

void removeEFILEListAndFree(EFILEList *efilelist,EFILE *efile){
  if(efile==efilelist->head){
	if(efile==efilelist->tail)
		efilelist->head=efilelist->tail=NULL;
	else{
		efilelist->head=efile->next;
		efile->next->prev=NULL;
	}
  }
  else if(efile==efilelist->tail){
	efilelist->tail=efile->prev;
	efile->prev->next=NULL;
  }
  else{
	efile->prev->next=efile->next;
	efile->next->prev=efile->prev;
  }
  freeEFILE(efile);
}

void printEFILE(EFILE *efile){
  printf("%s\nContains %d lines\n",efile->name,efile->numLines);
  lineNode *pt;
  for(pt=efile->head;pt;pt=pt->next)
	printf("%s\n",pt->line);
}

int EFILEInsertLines(EFILE *efile,int numLines,lineNode *beg,lineNode *end){
  return FAILURE;
}

int EFILEDeleteLines(EFILE *efile,int numLines,lineNode *end){
  return FAILURE;
}

