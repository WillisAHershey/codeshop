//Willis Hershey
//This file contains the implementations of the functions in fileReps.h

#include <stdio.h> //fopen fclose fgets
#include <stdlib.h>//malloc free NULL
#include <string.h>//strlen
#include <assert.h>//assert

#include "codeshopDefs.h"
#include "fileReps.h"
#include "userInterface.h"

EFILE* makeEFILE(const char *filename){
  FILE *file=fopen(filename,"r+");
  if(!file)
	return NULL;
  EFILE *out=malloc(sizeof(EFILE)+strlen(filename)+sizeof(NULL_TERMINATOR));
  *out=EMPTY_EFILE_NO_NAME;
  strcpy(out->name,filename);
  int len, overflow;
  lineNode *pt;
  char buf[LINE_BUF_LEN];
  while(fgets(buf,LINE_BUF_LEN,file)){
	pt=malloc(sizeof(lineNode)+(len=strnlen(buf,LINE_BUF_LEN-1))+sizeof(NULL_TERMINATOR));
	*pt=EMPTY_LINENODE_NO_LINE;
	strcpy(pt->line,buf);
	pt->prev=out->tail;
	if(len==LINE_BUF_LEN-1&&buf[LINE_BUF_LEN-2]!='\n'){
		overflow=1;
		while(fgets(buf,LINE_BUF_LEN,file)){
			pt=realloc(pt,sizeof(lineNode)+(len=strnlen(buf,LINE_BUF_LEN-1))+overflow*(LINE_BUF_LEN-1)+sizeof(NULL_TERMINATOR));
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
  }
  fclose(file);
  return out;
}

EFILE* makeEmptyEFILE(const char *filename){
  size_t len=filename?strlen(filename):strlen(defaultFileName);
  EFILE *out=malloc(sizeof(EFILE)+len+sizeof(NULL_TERMINATOR));
  *out=EMPTY_EFILE_NO_NAME;
  if(filename)
	strcpy(out->name,filename);
  else
	strcpy(out->name,defaultFileName);
  return out;
}

void freeEFILE(EFILE *efile){
  lineNode *pt=efile->head;
  while(pt){
	lineNode *run=pt;
	pt=pt->next;
	free(run);
  }
  while(efile->edits){
	editLog *hold=efile->edits->next;
	//This produces a leak in the case editType==DELETION
	free(efile->edits);
	efile->edits=hold;
  }
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
  while(efile->edits){
	editLog *hold=efile->edits->next;
	free(efile->edits);
	efile->edits=hold;
  }
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
  lineNode *pt;
  for(pt=efile->head;pt;pt=pt->next)
	printf("%s\n",pt->line);
}
 
int EFILEInsertLinesBefore(EFILE *efile,lineNode *here,lineNode *beg,lineNode *end){
  assert(efile&&here&&beg&&end);
  int len=1;
  for(lineNode *c=beg;c!=end;c=c->next)
	++len;
  beg->prev=here->prev;
  if(!here->prev)
	efile->head=beg;
  else
	here->prev->next=beg;
  end->next=here;
  here->prev=end;
  lineNode *top=beg->prev;
  lineNode *bottom=beg->next;
  int index=0;
  while(top&&bottom){
	top=top->prev;
	bottom=bottom->next;
	++index;
  }
  if(top&&!bottom)
	index*=-1;
  editLog *edit=malloc(sizeof(editLog));
  *edit=(editLog){.next=efile->edits,.editType=INSERTION,.insertion=(insertionLog){.index=index,.numLines=len}};
  efile->edits=edit;
  return SUCCESS;
}

int EFILEInsertLinesAfter(EFILE *efile,lineNode *here,lineNode *beg,lineNode *end){
  assert(efile&&here&&beg&&end);
  int len=1;
  for(lineNode *c=beg;c!=end;c=c->next)
	++len;
  end->next=here->next;
  if(!here->next)
	efile->tail=end;
  else
	here->next->prev=end;
  beg->prev=here;
  here->next=beg;
  lineNode *top=beg->prev;
  lineNode *bottom=beg->next;
  int index=0;
  while(top&&bottom){
	top=top->prev;
	bottom=bottom->next;
	++index;
  }
  if(top&&!bottom)
	index*=-1;
  editLog *edit=malloc(sizeof(editLog));
  *edit=(editLog){.next=efile->edits,.editType=INSERTION,.insertion=(insertionLog){.index=index,.numLines=len}};
  efile->edits=edit;
  return SUCCESS;
}

int EFILEDeleteLines(EFILE *efile,int numLines,int index){
  assert(efile&&numLines);
  lineNode *beg,*end;
  if(index<0){
	for(end=efile->tail;index;++index)
		end=end->prev;
	for(beg=end;--numLines;--numLines)
		beg=beg->prev;
  }
  else{
	for(beg=efile->head;index;--index)
		beg=beg->next;
	for(end=beg;--numLines;--numLines)
		end=end->next;
  }
  if(beg->prev)
	beg->prev->next=end->next;
  else
	efile->head=end->next;
  if(end->next)
	end->next->prev=beg->prev;
  else
	efile->tail=beg->prev;
  beg->prev=end->next=NULL;
  return SUCCESS;
}

int renameEFILE(EFILEList *efileList,EFILE *efile,const char *newName){
  assert(efileList&&efile&&newName);
  EFILE *prev=efile->prev;
  EFILE *next=efile->next;
  efile=realloc(efile,sizeof(EFILE)+strlen(newName)+sizeof(NULL_TERMINATOR));
  strcpy(efile->name,newName);
  if(prev)
	prev->next=efile;
  else
	efileList->head=efile;
  if(next)
	next->prev=efile;
  else
	efileList->tail=efile;
  return SUCCESS;
}

