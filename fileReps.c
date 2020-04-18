#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fileReps.h"
#include "codeshopDefs.h"

EFILE* makeEFILE(FILE *file,char *filename){
  EFILE *out=malloc(sizeof(EFILE)+strlen(filename)+sizeof(char));
  *out=(EFILE){.next=NULL,.head=NULL,.tail=NULL};
  strcpy(out->name,filename);
  int len, overflow;
  lineNode *pt;
  char buf[LINE_BUF_LEN];
  while(fgets(buf,LINE_BUF_LEN,file)){
	pt=malloc(sizeof(lineNode)+(len=strnlen(buf,LINE_BUF_LEN-1))+sizeof(char));
	*pt=(lineNode){.next=NULL,.prev=out->tail};
	strcpy(pt->line,buf);
	if(len==LINE_BUF_LEN-1&&buf[LINE_BUF_LEN-2]!='\n'){
		overflow=1;
		while(fgets(buf,LINE_BUF_LEN,file)){
			printf("Had to loop\n");
			pt=realloc(pt,sizeof(lineNode)+(len=strnlen(buf,LINE_BUF_LEN-1))+overflow*(LINE_BUF_LEN-1)+1);
			strcpy(pt->line+(LINE_BUF_LEN-1)*overflow,buf);
			if(len<LINE_BUF_LEN-1||buf[LINE_BUF_LEN-2]=='\n')
				break;
			else
				++overflow;
		}
	}
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

void printEFILE(EFILE *efile){
  printf("%s\n",efile->name);
  lineNode *pt;
  for(pt=efile->head;pt;pt=pt->next)
	printf("%s",pt->line);
}
