#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdint.h>
#include <assert.h>

#define TEXT_LIMIT 30000
#define LOOP_LIMIT 20

int get_next_matching_bracket(uint8_t* buffer,int* i){
	int left_count=0;
	while(*(buffer+(*i)) && *(buffer+(*i))!=EOF){
		if(*(buffer+(*i))=='[') left_count++;
		else if(*(buffer+(*i))==']') left_count--;
		if(!left_count && *(buffer+(*i))==']') return 1;
		 (*i)++;
	}
	return 0;
}

char* get_extension(char* filename){
	while(*filename && *filename++!='.');
	return filename;
}


int main(int argc,char* argv[]){

if(argc==1){
	fprintf(stderr,"Needs at least one file\n");
	exit(1);
} 

uint8_t vec[TEXT_LIMIT];	//stack
uint8_t* cur=vec;	//stack pointer

uint8_t loop_stack[LOOP_LIMIT];	//tracks location of last square bracket
memset(loop_stack,-1,LOOP_LIMIT);
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

	//fprintf(stderr,"%c - ",*(buffer+i));

	switch(*(buffer+i)){	
		case '>':cur++;break;	//move stack pointer up
		case '<':cur--;break;	//move stack pointer down
		case '+':(*cur)++;break;	//increase value at address
		case '-':(*cur)--;break;	//decrease
		case '.':putchar(*cur);break;	//output ascii code value
		case ',':(*cur)=getchar();break;	//get input
		case '[':
			*lsp++=i;//push current offset to loop stack
			if(*cur) break;	//if current stack pointer is different than 0, continue
			else if(!get_next_matching_bracket(buffer,&i)) goto end;	//else move on to the next matching bracket
		case ']':			
			--lsp;		//decrement loop stack
			if(*cur)	i=(*lsp)-1;	//pop value from loop stack, if current is not 0
			(*lsp)=-1;
		default:break;
	}

	//fprintf(stderr,"%d - %u\n",*cur,i);

	assert(((uint64_t) cur>= (uint64_t) vec) && ((uint64_t) cur < (uint64_t) vec+TEXT_LIMIT));//stack pointers must always be within range
	assert(((uint64_t) lsp>= (uint64_t) loop_stack) && ((uint64_t) lsp < (uint64_t) loop_stack+LOOP_LIMIT));
}

end:
	free(buffer);
	return 0;
}












