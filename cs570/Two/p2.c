// CS 570 - program 2
// Name: Tom Nguyen
// Professor: Dr. John Carroll
// Username: cssc0062@edoras.sdsu.edu
// March 07, 2018
// File: p2.c


//=================================p2=================================
// p2.c acts as a simple command line interpreter (a basic shell).
//
// Specifications:
// prompt with "p2: "
// p2 will skip input lines that contains 0 words and loop.
// p2 will terminate when input line first word is EOF.
// special functions for metacharacters '<', '>', '|', '&', '#', and '|&'
// p2 has a cd function to change directory.
// p2 has a MV function to move a file from location to another.
// 
// example: spell < proj/mytext > /tmp/dumberrors
// explaination: func_1 < destination_path_1 > destination_path_2
// '>' is for output a file.
// '<' is for inputing a file.
// 
// example: echo Null&echo Void
// '&' is used for starting a new process to execute the file, will also print child_pid
//
// example: cd /home/cs/carroll/cssc0062/Two/test
// cd will need the full path to work properly.
// use chdir to change directory.
//
// example: # this is a comment
// if '#' is at the start of the input then p2 will ignore the line and reprompt.
//
// example: MV out yeah
// explaination: MV existing-path-and-filename new-path-and-filename
// note: that lowercase will still work but it is not using p2's MV version.
// uses link() and unlink() a build in function.
//=====================================================================================

#include "p2.h"
#include <stdio.h>
#include "getword.h"
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>



//Any thing with ref in front means referencing to gradernotes workcited section.example: <ref 1> means https://www.cs.bu.edu/teaching/cpp/string/array-vs-ptr/.
//<ref 1> Array of strings used for storing the segments from getword.
char *newargv[MAXARGS]; //copied from exec1.c
char s[STORAGE];        //copied from p1.c //Array of character used for getword (breaks the stdin into segments).
int pointer;	//the pointer is the current location of where we stopped at in the stdin.
int is_outfile_being_used=0;	//flag: is '>' already in used?
int is_infile_being_used=0;	//flag: is '<' already in used? 1 is yes, 0 is no.
int is_background_being_used=0;	//flag: is '&' already in used?
int is_pipe_being_used=0;	//flag: is '|' already in used?
int is_comment_being_used=0;	//flag: is '#' already in used?
int newargv_wordcount = 0;	//this is like pointer, keeps track of words inside newargv
char *filename[SIZE_FOR_OUTFILE];	//used for keeping track of outfile name.
char *in_filename[SIZE_FOR_OUTFILE];	//used for keeping track of infile name.
int pipe_counter = 0;		//this is like pointer, keeps track of pipe location
int kidpid;	//used for keeping track of child pid
int pid;	//used for keep track of parent pid


		//===============problem==============
                //What about something that uses '>>'.
                //example: 123>>1234
                //would we need to worry about something like this?
                //====================================
		//What should we do about something > &outfile
		//====================================

//===============================myhandler for sigterm==
void myhandler(){};

//===============================main===================
int main(){
        int value_of_parse; //variable name from notebook cs570 page 4 by Dr.John Carroll
	setpgid(0,0);//give p2 its own, separate process group
	(void)signal (SIGTERM, myhandler);//from notebook cs570 page 10 by Dr.John Carroll
	for(;;){
		newargv_wordcount=0;
		pointer = 0;
		pipe_counter=0;
		is_outfile_being_used=0;
		is_comment_being_used=0;
		is_background_being_used=0;
		is_pipe_being_used=0;
		is_infile_being_used=0;	//resetting values because child fork did the work.
		printf("p2: ");
		value_of_parse=parse();	//deals with values above 0 printf("value=%d\n",value_of_parse);printf("main filename = %s\n",filename[0]);there are zero words so we need to repeat but this is EOF. the repeat will happen i$
                if( newargv_wordcount ==0){//there are no words.
			if(value_of_parse==0)//EOF
				break;}//New line terminates.
                if( value_of_parse == -10){//newline
			if(newargv_wordcount ==0){//no words
				continue;}}
		if (strcmp(newargv[0],"cd")==0){
			go_ahead_and_execute();
			continue;}
		if(strcmp(newargv[0],"MV")==0){
			go_ahead_and_execute();
			continue;}
		if(is_comment_being_used==1){
			continue;}
		fflush(stdout);fflush(stderr);
		kidpid=(int)fork();
		if(kidpid==0){//child
			child_process();
			exit(EXIT_SUCCESS);}
		else{//parent
			if(is_background_being_used==1){
				parent_process();
				continue;}
			for(;;){
				pid=wait(NULL);
				if(pid==kidpid)
					break;}}}//-unrecog
	killpg(getpgrp(),SIGTERM);
	printf("p2 terminated.\n");
	exit(0);
}

//===============================child process====================
//set the stdin to dev/null for '&' background process
int child_process(){//<ref 10>
	if(is_background_being_used==1){
		if(is_infile_being_used==0){//dont have the process fighting over stdin
			int openfile = open("/dev/null",O_RDONLY);
			if (openfile < 0){
				fprintf(stderr, "something went wrong with setting stdin to /dev/null.\n");
				exit(17);}
			dup2(openfile,STDIN_FILENO);
			close(openfile);}}
	go_ahead_and_execute();
	return 0;
}

//===============================parent process=================
//if we see the '&' flag then print the process id
int parent_process(){
	if(is_background_being_used==1){
		printf("%s [%d]\n",newargv[0],kidpid);
		is_background_being_used=0;
		return 14;}
	return 16;
}

//===============================parse=====================
int parse(){//keeps count of how many string is store into the array newargv[].
        int iochar;//every line of input will consist of 0 or more words
	int comment_counter;
	for(;;){
	        while ((iochar=getword(s+pointer)) > 0){//add from the starting point of s until we reach pointer.
         		newargv[newargv_wordcount++]=s+pointer;//pointer will start at 0 and increment by what getword returns to iochar. increase size by 1 for NULL
        		pointer +=iochar+1 ;//we left the while loop which means we have reached a metacharacter. so lets check which metacharacter we got.
		}
		if(*(s+pointer) == '>'){//output to a file
			if(is_outfile_being_used ==0){
				greater_than();
				continue;}}
		else if(*(s+pointer) == '<'){//take in a file
			if(is_infile_being_used ==0){
				less_than();
				continue;}}
		else if(*(s+pointer) == '|'){
			if(is_pipe_being_used==0){
				is_pipe_being_used=1;
				newargv[newargv_wordcount]=NULL;
				newargv_wordcount +=1;
				pipe_counter=newargv_wordcount;
				continue;}}
		else if(*(s+pointer) == '&'){
			amber_sign();
			break;}	//the parsing is not finish yet.
		else if(*(s+pointer) == '#'){
			if(newargv_wordcount==0){
				is_comment_being_used=1;
				comment_counter=0;//printf("pointer= %d\n",pointer);//pointer=0
				while((iochar=getchar()) != '\n');//move through until we see a new line.
				return -10;}
			else{
				newargv[newargv_wordcount++]= s+pointer;//int for '#'
				pointer += 2;
				}}
		else if(*(s+pointer) == EOF)
			ungetc(EOF,stdin);
		else{
			break;}}//end for loop
        {//copied from exec1.c
	/*@unused@*/
        int i;//used for debuging and seeing what is in newargv, should be single words
//        for (i=0; i<newargv_wordcount; i++){printf("newargv[%d]= %s\n",i,newargv[i]);}
	}	// placing the final NULL Into the end position of newargv.
	newargv[newargv_wordcount]=NULL;//return newargv_wordcount; 
	return iochar;
}

//===============================go ahead and execute================
//execvp because it is clear to do so.
int go_ahead_and_execute(){
	if(is_pipe_being_used==1){
		piping();
		return 25;}
	if(strcmp(newargv[0], "cd") ==0){//newargv[0]= cd
		//printf("newargv[1]= %d\n",newargv[1]);//newargv[1]= 0 //for null
		if(newargv[1]== NULL ){//printf("newargv[1]= %s\n",newargv[1]);//newargv[1]= (null)
			char* home_directory= getenv("HOME");//printf("home= %s\n",home_directory);//home= /home/cs/carroll/cssc0062/Two/test
			if(chdir(home_directory) != 0){//getenv is a build in functiont hat will get the home directory.
				fprintf(stderr,"%s: No such file or directory.\n",home_directory);}}
		else if (newargv_wordcount>2){//cd can not handle more than two arguments
			fprintf(stderr, "chdir: Too many arguments.\n");}
		else{
			if(chdir(newargv[1]) != 0){
				fprintf(stderr, "%s: No such file or directory.\n",newargv[1]);}}
		return 13;}
	if(strcmp(newargv[0],"MV") ==0){//printf("newargv_wordcount= %d\n",newargv_wordcount);//newargv_wordcount=3
		if(newargv_wordcount==3){
			if(link(newargv[1],newargv[2]) == 0){//string1 and string2 exist
				if(unlink(newargv[1]) == 0){//can delete string1
					return 24;}
				fprintf(stderr,"unable to remove file: %s\n",newargv[1]);
				return 25;}
			else{//link() returned -1, failed.
				if(strcmp(newargv[1],newargv[2])==0){//same file name
					fprintf(stderr,"MV: '%s' and '%s' are the same file\n",newargv[1],newargv[2]);}
				else{//missing second destination
					fprintf(stderr,"MV: cannot stat '%s': No such file or directory\n",newargv[1]);}}
			return 23;}
		else{//not the right about of arguments.
			if(newargv_wordcount<3){//less than three arguments
				fprintf(stderr,"MV: missing destination file operand after '%s'\n",newargv[1]);}
			else{//newargv_wordcount>3
				fprintf(stderr,"MV: target '%s' is not a directory\n",newargv[newargv_wordcount-1]);}
			return 22;}}
	if(is_outfile_being_used==1){        	//updating pointer, a global variable. pointer+2 is for > + '\0'
	        //time to open this file. reusing a variable from before. shouldn't be called iochar. should be called outfile_name or something like that.
		int iochar;
		int user = S_IRWXU;
		int flags=O_RDWR|O_CREAT|O_EXCL;//O_RDWR=open & write, O_CREAT=create file.use O_EXCL=won't create a file if it already exist.
	        if((iochar=open(filename[0],flags,user))<0){//S_IRWXU=S_IRUSR(only)|S_IWUSR(write)|S_IXUSR(user){
			if(newargv_wordcount<2){//missing an agurment after '>'
				fprintf(stderr,"Missing name for redirect.\n");
				exit(9);}
        	        fprintf(stderr,"%s: File exists.\n",filename[0]);//is_outfile_being_used=0;        //filename already exist so outfile is not being used.
        	        exit(9);}
	        dup2(iochar,STDOUT_FILENO);//<ref 8>
	        close(iochar);}
	if(is_infile_being_used==1){
		int iochar;
		int flags = O_RDONLY;
		if((iochar=open(in_filename[0],flags))<0){
                        if(newargv_wordcount<2){//missing an agurment after '<'
                                fprintf(stderr,"Missing name for redirect.\n");
                                exit(9);}
                	fprintf(stderr,"%s: No such file or directory.\n",in_filename[0]);
			is_outfile_being_used=0;
			exit(10);}
		dup2(iochar,STDIN_FILENO);//<ref 8>
		close(iochar);}
	if(execvp(newargv[0],newargv) < 0){
		fprintf(stderr,"%s: invalid command.\n",newargv[0]);
		exit(9);}
	return 1;
}

//===============================amber sign=============
//set flag for starting a new process in the background
int amber_sign(){
	if(filename[0]==NULL && in_filename[0]==NULL){
		if(is_outfile_being_used ==1)return 14;
		if(is_infile_being_used ==1)return 13;}
	is_background_being_used=1;//	printf("%s [%d]\n",newargv[0],kidpid);
	return 12;	//add 1 to greater_than() return value
}

//===============================piping=====================
//using pipe() to build a link between grandchild and child.
//The grandchild will execute the command, dying, and passing the information to the child. It will pass the information by changing stdout
//The child will take in the information from the grandchild by taking what was send out from stdout(grandchild) and taking it in stdin.
int piping(){
	int file_holder[2];
	pid_t mother;
	pipe(file_holder);//printf("file_holder[0]= %d\n",file_holder[0]);//file_holder[0]=3//printf("file_holder[1]= %d\n",file_holder[1]);//file_holder[1]=4
	mother=fork();
	if(mother==0){//grandchild
		dup2(file_holder[1],STDOUT_FILENO);
		close(file_holder[0]);//child side
		close(file_holder[1]);//grandchild side
		if(is_infile_being_used==1){
			int in_filename2=open(in_filename[0],O_RDONLY);
			if(in_filename2<0){
				fprintf(stderr, "Error in changing stdin\n");}
			dup2(in_filename2,STDIN_FILENO);
			close(in_filename2);}
		if( execvp(newargv[0],newargv) < 0){
			fprintf(stderr,"%s: could not execute.\n",newargv[0]);
			exit(9);}}
	else{//child
		dup2(file_holder[0],STDIN_FILENO);
		close(file_holder[0]);
		close(file_holder[1]);
		if(is_outfile_being_used==1){
			int filename2 = open(filename[0],O_RDWR|O_CREAT|O_EXCL,S_IRWXU);
			if(filename2<0){
				fprintf(stderr,"%s: File exists.\n",filename[0]);}
			dup2(filename2,STDOUT_FILENO);
			close(filename2);}
		if( execvp(newargv[pipe_counter], newargv+pipe_counter)<0){
			fprintf(stderr, "%s: could not execute in newargv[pipe_counter]",newargv[pipe_counter]);
			exit(9);}}//close(file_holder[0]);close(file_holder[1]);//we dont need this.
	return 21;
}

//===============================greather than=======================
//Deals with output files. used page 8 dup2.c from cs570 book by Dr. John Carroll.
//should we return a string instead?
int greater_than(){
	int iochar;
	is_outfile_being_used = 1;	//yes, outfile is now being used.	//updating pointer, a global variable. pointer+2 is for > + '\0'
	iochar = getword(s+ (pointer+2));//printf("greater iochar = %d \n",iochar);
        if(iochar==-10){//new line there were no files enter here.
                return ungetc('\n',stdin);}
	filename[0] = s+pointer+2;//	printf("greater filename=%s\n",filename[0]);//you can delete this
	pointer += iochar + 1+2;//length of filename + Null + 2 for >'\0'
	return 11; //add 1 to the random number to return
}

//===============================less than==========================
//taking in a file and reading from it.
//changing stdin by using dup2.
int less_than(){
	int openfile;
	is_infile_being_used=1;	//yes, infile is now being used.
	openfile = getword(s+(pointer+2));
	if(openfile==-10){//new line there were no files enter here.
		return ungetc('\n',stdin);}
	in_filename[0] = s+pointer+2;//printf("less filename[0]=%s\n",in_filename[0]);
	pointer += openfile +1+2;
	return 10;	//picked a random number to return.
}//Looks for three values to fork: -1(metacharacter '&'), 0(EOF), and -10(newline)

//===============================fork test======================
//seeing how fork() works. just an example I got from them use.
//Do not need for p2.c to work.
int fork_test(){
	int kidpid;
	printf("getpid()=%d\n",(int)getpid());	
	if (-1 == (kidpid = (int) fork())){
		perror("Cannot fork");
		exit(EXIT_FAILURE);}
	else if (0==kidpid){
		printf("kidpid=%d\n",kidpid);
		(void) sleep(1);
		printf("child says: my pid = %d, parent pid =%d\n", (int)getpid(),(int)getppid());
		exit(EXIT_SUCCESS);}
	else{
		printf("how did i get in here also? kidpid=%d\n",kidpid);
		(void)sleep(1);
		printf("parent says: child pid = %d, my pid=%d\n",(int)kidpid,(int)getpid());
		exit(EXIT_SUCCESS);}
	return 0;
}
