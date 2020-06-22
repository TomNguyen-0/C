/*
 * CS 570 - Program 1
 * Name: Tom Nguyen
 * Professor: Dr. John Carroll
 * Username: cssc0062@edroas.sdsu.edu
 * January 26, 2018
 * File: getword.c
 *
 * Function: Lexical analyzer
 * Examines successive words until getword() returns with a zero.
 *
 * Conditions:
 * Anything that is separated by a space, newline, and EOF will be consider a word.
 * Sidenotes: Tabs will not be consider part of the words.
 * If a word starts with '#' will be consider a one-character word and returns -1.
 * If the word size is zero and it encounters a EOF then returns 0.
 * If the word size is zero and it encounters a newline then returns -10. 
 * getword() will ignore leading spaces in the string.
 * whenever getword() encounters a meta character (<,>,|,&, or #) returns a -1 unless,
 *  that meta character is a '|' then it will, instead, return a -2.
 * When getword() encounters '\' it will treat a meta character or space  as if it is a normal character.
 * As noted above if getword() encounters '\\' it will consider this as a normal character. 
 * If '#' is not the start of the word then it will be consider as a normal character.
 * If we reach our char size limit of 255 break the string into 254 sizes.
 *
 * Notes:
 * The *input position that is going to be passed into should be the beginning of the string.
 * The array size limit is 255.
 * 
 * Use ungetc to return a character back to stdin in order to look at it again for comparisons.
 */

// Include Files
#include <stdio.h>
#include "getword.h"
#include <stdbool.h>

bool checkMeta(int meta){	// it checks to see if it is one of the five meta characters
	if( (meta == '<') || (meta == '>') || (meta == '|') || (meta == '&') || (meta == '#') )
		return true;
	return false;
}
//function lookFor(char sym, int returnThisValue)
int getword(char *input ){
	int iochar;                                         
	int wordCount=0;
	fflush(stdin);
//	for (start=input; *start;start++){wordCount++;}
        if( wordCount==0){
                while ( ((iochar = getchar() ) == 32) ); // getting rid of any leading spaces
		if( iochar == 10){ 	// case: newline
			input[wordCount]='\0';
			return -10;
		}
		ungetc(iochar,stdin);//return the character because it is not a space or a newline
        }
        if( (iochar=getchar()) == EOF){ // check if the next character is EOF
                input[0]='\0';
                return 0;
        }
        else
                ungetc(iochar,stdin);//returning the character back into stdin because it was not EOF
	while ( (iochar=getchar()) != 32 ){               // space in Dec = 32
		if(wordCount==254){     /* the char array is at the limit*/
                        ungetc(iochar,stdin);// returning the character back into stdin because we reach our limit
                        input[wordCount+1]='\0';
                        return wordCount;
                }
		if(iochar == '\\'){
			iochar=getchar();
			if(iochar<0 || iochar>127){  // Case: special character
                        	if(iochar==EOF)
                                	break;
                       		 else
                                	continue;
               		 }
			switch(iochar){		// switch any meta characters and space  
				case '<':
					input[wordCount++]=iochar;
					continue;
				case '>':
					input[wordCount++]=iochar;
					continue;
				case '|':
					input[wordCount++]=iochar;
					continue;
				case '&':
					input[wordCount++]=iochar;
					continue;
				case '#':
					input[wordCount++]=iochar;
					continue;
				case ' ':
					input[wordCount++]=iochar;
					continue;
				case '\\':
					input[wordCount++]=iochar;
					continue;
				case '\n':	// put new line back and return current wordCount.
					ungetc(iochar,stdin);//return the character back to stdin becuase it is not a metachacter
					return wordCount;
				default:	// if the blackslash is not followed by a meta or space ignore
					input[wordCount++]=iochar;
					continue;
			}
		}
		input[wordCount++]=iochar;
		if(checkMeta(iochar) && wordCount==1){  // checking case starts with '#'=35.
			if(iochar=='|'){	// Got a pipe character
				iochar=getchar();	// look at the next character to see if it is a meta character
				if(checkMeta(iochar)){	// found a second meta character
					input[wordCount++]=iochar;
					input[wordCount]='\0';
					return -wordCount;
				}
				else{
					ungetc(iochar,stdin);	// put the character back because this is not a meta character
				}
			}
			input[wordCount]='\0';
			return -1;
		}
		else if(checkMeta(iochar)){	
			if(iochar == '#' ){	// excluding # for its special rules
				continue;
			}
			ungetc(iochar,stdin);
			input[--wordCount]='\0';
			return wordCount;
		}
		else if(iochar == 10){	// if we encounter a newline  
		        input[--wordCount]='\0';       // stopping the array here
       			ungetc(iochar,stdin);
//        		wordCount--;            //reducing the return value of wordCount
			return wordCount;
		}
		else if( (iochar = getchar()) ==EOF ){ // checking if the next getchar() is EOF
			input[wordCount]='\0';
			return wordCount;
		}
		else	ungetc(iochar,stdin);
	}	
	input[wordCount]='\0'; // clears the any left overs from previous uses 
	return wordCount;       // stops after space and returns number of letters in the word 
}
