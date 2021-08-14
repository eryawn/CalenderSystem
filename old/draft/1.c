#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define MAX_LINE_NUMBER 100
#define MAX_LINE_LENGTH 81
#define MAX_WORD_NUMBER 120
#define MAX_WORD_LENGTH 20

char words[MAX_WORD_NUMBER][MAX_WORD_LENGTH];
int num_word=0;

int find(char *w){
	int i;
	for(i=0;i<num_word;i++){
		if(strcmp(w,words[i])==0){
			return i;
		}
	}
	return -1;
}

char* outputname(char* input){
	int len=strlen(input);
	static char output[50];
	strncpy(output,input,50);
	output[len-3]='m',output[len-2]='t',output[len-1]='f';
	return output;
}



main(int argc,char **argv){
	
  if (argc<2){
	  fprintf(stderr,"usage: %s filename\n", argv[0]);
	  exit(1);
  }
  FILE *file=fopen(argv[1],"r");
  if(file==NULL){
	  fprintf(stderr,"%s: cannot open %s", argv[0],argv[1]);
	  exit(1);
  }
  
  char lines[MAX_LINE_LENGTH];
  int num_char=0, i=0;
  char *t;

  FILE *output=fopen(outputname(argv[1]),"w");
  
  fprintf(output,"%c%c%c%c",0xba,0x5e,0xba,0x11);
  

  while(fgets(lines,MAX_LINE_LENGTH,file)!=NULL){
	lines[strlen(lines)-1]='\0';
    t=strtok(lines," ");
    while(t&&num_word<MAX_WORD_NUMBER){
	  i=find(t);
	  if(i!=-1){
		  fprintf(output,"%c",num_word-i+128);
		  for(int j=i;j<num_word-1;j++){strncpy(words[j],words[j+1],MAX_WORD_LENGTH);}
		  num_word--;
	  }	
      strncpy(words[num_word++],t,MAX_WORD_LENGTH);
	  if(i==-1){
		  fprintf(output,"%c",num_word+128);
          num_char=0;
          while(words[num_word-1][num_char]){
            fprintf(output,"%c",(words[num_word-1][num_char]));
            num_char++;
          }
	  }
      t=strtok(NULL," ");  
	  
	  
    }
    fprintf(output,"%c",('\n'));
  }
  fclose(output);
  fclose(file);

  
}








