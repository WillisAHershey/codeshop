#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fileReps.h"
#include "userInterface.h"
#include "codeshopDefs.h"

EFILE* makeEFILE(FILE *file,char *filename){
  EFILE *out=malloc(sizeof(EFILE)+strlen(filename)+sizeof(char));
  *out=(EFILE){.next=NULL,.prev=NULL,.fd=file,.head=NULL,.tail=NULL};
  strcpy(out->name,filename);
  int len, overflow;
  lineNode *pt;
  char buf[LINE_BUF_LEN];
  int numLines=0;
  while(fgets(buf,LINE_BUF_LEN,file)){
	pt=malloc(sizeof(lineNode)+(len=strnlen(buf,LINE_BUF_LEN-1))+sizeof(char));
	*pt=(lineNode){.next=NULL,.prev=out->tail};
	strcpy(pt->line,buf);
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
  return out;
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

EFILE* makeEmptyEFILE(char *filename){
  size_t len=filename?strlen(filename)+1:1;
  EFILE *out=malloc(sizeof(EFILE)+len);
  *out=(EFILE){.next=NULL,.prev=NULL,.fd=NULL,.head=NULL,.tail=NULL,.numLines=0};
  if(filename)
	strcpy(out->name,filename);
  else
	out->name[0]='\0';
  return out;
}

int EFILEAppendLine(EFILE *efile,char *line){
  lineNode *pt=malloc(sizeof(lineNode)+strlen(line)+1);
  pt->next=NULL;
  strcpy(pt->line,line);
  if(!efile->tail){
	efile->head=efile->tail=pt;
	pt->prev=NULL;
	efile->numLines=1;
  }
  else{
	efile->tail->next=pt;
	pt->prev=efile->tail;
	efile->tail=pt;
	++efile->numLines;
  }
  return SUCCESS;
}

void freeEFILE(EFILE *efile){
  fclose(efile->fd);
  lineNode *pt=efile->head;
  lineNode *run;
  while(pt){
	run=pt;
	pt=pt->next;
	free(run);
  }
  free(efile);
}

void freeEFILEList(EFILEList *list){
  EFILE *efile=list->head;
  EFILE *run;
  while(efile){
	run=efile;
	efile=efile->next;
	freeEFILE(run);
  }
}

void printEFILE(EFILE *efile){
  printf("%s\nContaints %d lines\n",efile->name,efile->numLines);
  lineNode *pt;
  for(pt=efile->head;pt;pt=pt->next)
	printf("%s\n",pt->line);
}

//efile must have a valid FILE* before this function is called
void writeEFILE(EFILE *efile){
  fseek(efile->fd,0,SEEK_SET);
  lineNode *pt=efile->head;
  while(pt){
	fprintf(efile->fd,"%s\n",pt->line);
	pt=pt->next;
  }
}

//efile must have a valid FILE* before this function is called
void writeAndFreeEFILE(EFILE *efile){
  fseek(efile->fd,0,SEEK_SET);
  lineNode *pt=efile->head;
  lineNode *run;
  while(pt){
	run=pt;
	fprintf(efile->fd,"%s\n",pt->line);
	pt=pt->next;
	free(run);
  }
  fclose(efile->fd);
  free(efile);
}
