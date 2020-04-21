#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fileReps.h"
#include "userInterface.h"
#include "codeshopDefs.h"

union flexPointer{
  lineNode *line;
  char *text;
};

//Not available in .h file, so not linkable.
void logEdit(EFILE *efile,enum editType edit,int spot,int numLines,void *a){
  union flexPointer pt;
  pt.line=a;
  if(edit==DELETION)
	  free(pt.line);
}

EFILE* makeEFILE(FILE *file,char *filename){
  EFILE *out=malloc(sizeof(EFILE)+strlen(filename)+sizeof(char));
  *out=(EFILE){.next=NULL,.prev=NULL,.fd=NULL,.head=NULL,.tail=NULL,.edits=NULL};
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
  fclose(file);
  return out;
}

EFILE* makeEmptyEFILE(char *filename){
  size_t len=filename?strlen(filename)+1:1;
  EFILE *out=malloc(sizeof(EFILE)+len);
  *out=(EFILE){.next=NULL,.prev=NULL,.fd=NULL,.head=NULL,.tail=NULL,.edits=NULL,.numLines=0};
  if(filename)
	strcpy(out->name,filename);
  else
	out->name[0]='\0';
  return out;
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

void writeEFILE(EFILE *efile){
  if(!(efile->fd=fopen(efile->name,"w")))
	if(!resolveFailureToOpenForWrite(efile))
		return;
  lineNode *pt=efile->head;
  while(pt){
	fprintf(efile->fd,"%s\n",pt->line);
	pt=pt->next;
  }
  fclose(efile->fd);
}

void writeAndFreeEFILE(EFILE *efile){
  if(!(efile->fd=fopen(efile->name,"w")))
	if(!resolveFailureToOpenForWrite(efile))
		return;
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

void freeEFILEList(EFILEList *list){
  EFILE *efile=list->head;
  EFILE *run;
  while(efile){
	run=efile;
	efile=efile->next;
	freeEFILE(run);
  }
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
  printf("%s\nContaints %d lines\n",efile->name,efile->numLines);
  lineNode *pt;
  for(pt=efile->head;pt;pt=pt->next)
	printf("%s\n",pt->line);
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

  logEdit(efile,INSERTION,AT_END,1,line);
  return SUCCESS;
}

int EFILEDeleteLine(EFILE *efile,int lineNum){
  register int numLines=efile->numLines;
  register lineNode *pt;
  if(lineNum<1||lineNum>numLines)
	return FAILURE;
  if(lineNum==1){
	pt=efile->head;
	if(numLines==1)
		efile->head=efile->tail=NULL;
	else{
		efile->head=pt->next;
		pt->next->prev=NULL;
	}
  }
  else{
	if(lineNum==numLines){
		pt=efile->tail;
		efile->tail=pt->prev;
		efile->tail->next=NULL;
	}
	else{
		int c;
		if(lineNum<=numLines/2){
			pt=efile->head;
			for(c=1;c<lineNum;++c)
				pt=pt->next;
		}
		else{
			pt=efile->tail;
			for(c=numLines;c>lineNum;--c)
				pt=pt->prev;
		}
		pt->prev->next=pt->next;
		pt->next->prev=pt->prev;
	}
  }
  //at this point pt is pointing to line being deleted, and it is disconnected from list in EFILE
  logEdit(efile,DELETION,lineNum,1,pt);
  --efile->numLines;
  return SUCCESS;
}

