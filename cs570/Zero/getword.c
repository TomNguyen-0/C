/*
 * CS 570 - Program 0
 * Name: Tom Nguyen
 * Professor: Dr. John Carroll
 * Username: cssc0062@edroas.sdsu.edu
 * January 19, 2018
 * File: getword.c
 *
 * Function: Lexical analyzer
 * Examines successive words until getword() returns with a negative number.
 *
 * Conditions:
 * Anything that is separated by a space, newline, and EOF will be consider a word.
 * Sidenotes: Tabs will not be consider part of the words.
 * If a word starts with '#' will be consider a one-character word and returns -1.
 * If the word size is zero and it encounters a EOF then returns 0.
 * If the word size is zero and it encounters a newline then returns -10. 
 * getword() will ignore leading spaces in the string.
 *
 * Notes:
 * The *input position that is going to be passed into should be the beginning of the string.
 */

/* Include Files */
#include <stdio.h>
#include "getword.h"

int getword(char *input ){
	int iochar;                                           /* Note 1 */
	int wordCount=0;
//	for (start=input; *start;start++){
//		wordCount++;
//	}
        if( wordCount==0){
                while ( ((iochar = getchar() ) == 32) ); /* getting rid of any leading spaces */
		if( iochar == 10){ 	/* case: newline  */
			input[wordCount]='\0';
			return -10;
		}
		ungetc(iochar,stdin);
        }
        if( (iochar=getchar()) == EOF){ /* check if the next character is EOF */
                input[0]='\0';
                return 0;
        }
        else
                ungetc(iochar,stdin);
	while ( (iochar=getchar()) != 32 ){               /* space in Dec = 32 */
		input[wordCount++]=iochar;
		if(iochar == 35 && wordCount==1){  /* checking case starts with '#'=35. */
			input[wordCount]='\0';
			return -1;
		}
		else if(iochar == 10){	/* if we encounter a newline  */ 
		        input[--wordCount]='\0';       /* stopping the array here */
       			ungetc(iochar,stdin);
//        		wordCount--;            /* reducing the return value of wordCount */
			return wordCount;
		}
		else if( (iochar = getchar()) ==EOF ){ /* checking if the next getchar() is EOF */
			input[wordCount]='\0';
			return wordCount;
		}
		else	ungetc(iochar,stdin);
	}	
	input[wordCount]='\0'; /* clears the any left overs from previous uses */
	return wordCount;       /* stops after space and returns number of letters in the word */
}
