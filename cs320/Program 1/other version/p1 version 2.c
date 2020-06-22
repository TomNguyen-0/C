/*************************************************************
CS320 -Program1
Tom Nguyen 
Professor: Dr. John Carroll
username: CSSC0155@edoras.sdsu.edu
February 17, 2017
File: p1.c
Notes:  p1 will read data from standard
input one character at a time and output the results to standard output.
It will delete all non-ASCII characters, change the case
of alphabetic characters, 'fold' long lines, and make browser-friendly
adjustments to the characters _, /, :, ?, &, and space.
**************************************************************/
#include <stdio.h>
#include <string.h>
#define FOLDING 20  /*length of characters per line*/

int case5=0;         /*count characters*/
int changed=1;       /*1=no-change, 0=changed*/

void case1(int characters);
void caseLast();
void check();
void case2(int whatCase);

/*************************************************************
Function: CASELAST
Notes:   Step 5, enter a new line after the 20th character.
I/O: input parameters: None but uses a global variable case5.
     output: New line if it is true.
**************************************************************/
void caseLast(){
   if(case5 ==FOLDING){      /*It is the 20th character*/
      printf("\n");     /*new line*/
      case5=0;          /*reset character count*/
     }
}
/*************************************************************
Function:   CHECK
Notes:   Increase character count. Checks if there were any
         changed. If it is the 20th character enter a new line.
**************************************************************/
void check(){
   case5++;             /*increase character count*/
   caseLast();          /*check if it is the 20th character*/
   changed=0;           /*file has been changed*/
}
/*************************************************************
Function:   CASE1
Notes:   Deals with condition 1. Changes: _, /, :, ?, &, and 
         space to %5F, %2F, %3A, %3F, %26, and %20.
I/O: input parameters: An integer represented as an character.
     output: print out the products. 
**************************************************************/
void case1(int characters){
   printf("%%");
   check();
			switch(characters)
			{
			case 95:
				printf("5");
            check();
            printf("F");
            check();
				break;
			case 63:
				printf("3");
            check();
            printf("F");
            check();
				break;
			case 58:
				printf("3");
            check();
            printf("A");
            check();
				break;
			case 47:
				printf("2");
            check();
            printf("F");
            check();
				break;
			case 38:
				printf("2");
            check();
            printf("6");
            check();
				break;
			case 32:
				printf("2");
            check();
            printf("0");
            check();
				break;
			default:
            printf("%c",characters);
            check();
				break;
			}
}
/*************************************************************
Function:   CASE2
Notes:   satisfied condition 2. Changes lower to upper case.
         Changes upper to lower case.
I/O: input parameters: The character that is to be changed. 
     output: Prints out upper case to lower case and vice versa.
             If it is not an alphabet then it does nothing.
**************************************************************/
void case2(int whatCase){
   if((whatCase>64)&&(whatCase<91)){   /*uppercase*/
      printf("%c",(whatCase+32));      /*changed to lowercase*/
      check();
     }
   if((whatCase>96)&&(whatCase<123)){  /*lowercase*/
      printf("%c",(whatCase-32));      /*changed to uppercase*/
      check();
    }
}

/*************************************************************
Main: The concept for this program was taken from "C by 
Discovery" by Leslie Foster. In chapter 2 the file inout2.c was used.
**************************************************************/
int main( void )
{
    int iochar;         /*used for getchar*/

     while ( ( iochar = getchar() ) != EOF ){   /*stops when end of file is reached*/
        if((iochar=='_')||(iochar=='?')||(iochar==':')||(iochar=='/')||(iochar=='&')||(iochar==' ')){
           case1(iochar);              /*deals with condition 1*/
         }
         else if (((iochar>64)&&(iochar<91))||((iochar>96)&&(iochar<123))) 
         {                             /*ascii base-10 for a-z and A-Z*/
            case2(iochar);             /*changes the letter case*/
         }       
         else if ((iochar>127)){       /*Deals with condition 3*/
            changed=0;                 /*change was0123 made*/
         }
         else if(iochar==10){          /*deals with line feed*/
            
         }
         else{                         /*prints the character with no changed*/
            printf("%c",iochar);
            case5++;
            caseLast();
         }
     }
    return changed;                    /*returns 1 if no change otherwise 0.*/
}