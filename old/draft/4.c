#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char  magic_number_1[] = {0xBA, 0x5E, 0xBA, 0x11, 0x00};
char  magic_number_2[] = {0xBA, 0x5E, 0xBA, 0x12, 0x00};



/*
 * The encode() function may remain unchanged for A#4.
 */

int encode(FILE *input, FILE *output) {
    return 0;
}

typedef struct node node;
struct node{
	char *word;
	node *next;
};

void *emalloc(size_t n){
        void *p;
	p=malloc(n);
	if(p==NULL){
	  fprintf(stderr,"malloc of %u bytes faied",n);
	  exit(1);
	}
	return p;
}

node *newitem(char *string){
	node *newnode;
	newnode=(node *)emalloc(sizeof(node));

	newnode->word=(char *)emalloc(strlen(string)+1);
	strncpy(newnode->word,string,strlen(string)+1);

	newnode->next=NULL;
	return newnode;
}

node *addfront(node *list,node *newnode){
	newnode->next=list;
	return newnode;
}

node *find_and_move(node *list,int code, FILE *output,int x){
	node *cur, *pre;
	pre=NULL;
	cur=list;
	int i=1;
	for(;i<code;i++){
		pre=cur;
		cur=cur->next;
	}

	fprintf(output,"%s",cur->word);
	if(pre!=NULL){
		pre->next=cur->next;
		return addfront(list,cur);
	}
	else return list;
}

void freeall(node *list){
	node *next;
	for(;list!=NULL;list=next){
		next=list->next;
		free(list->word);
		free(list);
	}
}


int decode(FILE *input, FILE *output) {
    char file_magic_number[5];
    file_magic_number[4]='\0';
    for(int i=0;i<4;i++){
    	file_magic_number[i]=fgetc(input);
    }
    if(strncmp(file_magic_number,magic_number_1,5)!=0 && strncmp(file_magic_number,magic_number_2,5)!=0){
    	return -1;
    }
    node *list=NULL;
    int list_size=0;
    unsigned char ele=fgetc(input);
    while(1){
        if(feof(input)){break;}
    	//printf("%d",ele);
    	if(ele=='\n'){
    		fprintf(output,"\n");
    		ele=fgetc(input);
    	}
    	else if((int)(ele)<249){
    		if((int)(ele)==list_size+129){
		        int size_of_str=10;
		        char *str=(char *)emalloc(sizeof(char)*size_of_str);
		        
    			int i=0;
    			ele=fgetc(input);
    			while(ele<128 && ele!='\n' && ele!='\0'){
    				str[i++]=ele;
				if(i==size_of_str){
				  size_of_str=size_of_str*2;
				  str=(char *)realloc(str,sizeof(char)*size_of_str);
				  if(str==NULL){exit(1);}
				}
    				ele=fgetc(input);
    			}
    			str[i]='\0';
    			fprintf(output,"%s",str);
    			if(ele!='\n' && ele!='\0'){fprintf(output," ");}
    			list=addfront(list,newitem(str));
    			list_size++;
			free(str);
    		}
    		else{
		        list=find_and_move(list,(int)(ele)-128,output,list_size);
    			ele=fgetc(input);
    			if(ele!='\n'){fprintf(output," ");}
    		}
    	}
    	else if((int)(ele)==249){
    		int a=fgetc(input);
    		if(a==list_size-120){
    			ele=getc(input);
    			int size_of_str=10;
		        char *str=(char *)malloc(sizeof(char)*size_of_str);
			
    			int i=0;
    			while(ele<128 && ele!='\n' && ele!='\0'){
    				str[i++]=ele;
				if(i==size_of_str){
				  size_of_str=size_of_str*2;
				  str=(char *)realloc(str,sizeof(char)*size_of_str);
				  if(str==NULL){exit(1);}
				}
    				ele=fgetc(input);
    			}
    			str[i]='\0';
    			fprintf(output,"%s",str);
    			if(ele!='\n' && ele!='\0'){fprintf(output," ");}
    			list=addfront(list,newitem(str));
    			list_size++;
			free(str);
    		}
    		else{
		  list=find_and_move(list,(int)(a)+121,output,list_size);
    			ele=fgetc(input);
    			if(ele!='\n'){fprintf(output," ");}
    		}
    	}
    	else if((int)(ele)==250){
    		int b=fgetc(input);
    		int c=fgetc(input);
	   
    		if(b==(list_size-375)/256 && c==(list_size-375)%256){
    			ele=fgetc(input);
    			int size_of_str=10;
		        char *str=(char *)malloc(sizeof(char)*size_of_str);
		     
    			int i=0;
    			while(ele<128 && ele!='\n' && ele!='\0'){
    				str[i++]=ele;
				if(i==size_of_str){
				  size_of_str=size_of_str*2;
				  str=(char *)realloc(str,sizeof(char)*size_of_str);
				  if(str==NULL){exit(1);}
				}
    				ele=fgetc(input);
    			}
    			str[i]='\0';
    			fprintf(output,"%s",str);
    			if(ele!='\n' && ele!='\0'){fprintf(output," ");}
    			list=addfront(list,newitem(str));
    			list_size++;
			free(str);
    		}
    		else{
		  list=find_and_move(list,b*256+c+376,output,list_size);
    			ele=fgetc(input);
    			if(ele!='\n'){fprintf(output," ");}
    		}
    	}

    }

    freeall(list);
    printf("success\n");
    return 0;
}
