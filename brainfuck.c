#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdint.h>
#include <assert.h>

char* get_extension(char* filename){
	while(*filename && *filename++!='.');
	return filename;
}


int main(int argc,char* argv[]){

if(argc==1){
	fprintf(stderr,"Needs at least one file\n");
	exit(1);
} 

uint8_t vec[10000];	//stack
uint8_t* cur=vec;	//stack pointer

uint8_t loop_stack[20];	//tracks location of last square bracket
memset(loop_stack,0,20);
uint8_t* lsp=loop_stack;//loop stack pointer

char* filename=argv[1];

if(strcmp(get_extension(filename),"bf")!=0){
	fprintf(stderr,"Only bf extensions allowed\n");
	exit(0);
}

int i,fd = open(filename,O_RDONLY);
if(fd==-1){
	fprintf(stderr,"File %s does not exist\n",filename);
	exit(1);
}

struct stat st;
stat(filename,&st);

int fsize=st.st_size;

char* buffer=malloc(fsize+1); 
memset(buffer,0,fsize+1);

read(fd,buffer,fsize);
close(fd);

for(i=0;i<fsize;i++){
	switch(*(buffer+i)){	
		case '>':cur++;break;	//move stack pointer up
		case '<':cur--;break;	//move stack pointer down
		case '+':(*cur)++;break;	//increase value at address
		case '-':(*cur)--;break;	//decrease
		case '.':putc(*cur,stdout);break;	//output ascii code value
		case ',':*cur=getchar();break;	//get input
		case '[':*lsp++=i;break;	//save offset of loop
		case ']':			
			if(*cur==0) lsp--;	//if current value is 0 proceed
			else i=*(lsp-1);	//else get last value
			break;
	}
	assert((uint64_t) cur>= (uint64_t) vec);//stack pointers must always be within range
	assert((uint64_t) lsp>= (uint64_t) loop_stack);
}

free(buffer);
return 0;
}












