/*************************************************************
Name: Tom Nguyen 
username: CSSC0155@edoras.sdsu.edu
Project: CS320 Program 1
Date: February 17, 2017
File: p1.c
Notes:  p1 will read data from standard
input one character at a time and output the results to standard output.
It will delete all non-ASCII characters, change the case
of alphabetic characters, 'fold' long lines, and make browser-friendly
adjustments to the characters _, /, :, ?, &, and space.
**************************************************************/
#include <stdio.h>
#include <string.h>

int case5;

void case1(int characters);
/*************************************************************

**************************************************************/
void caseLast(){
   if(case5 ==20){
      printf("\n");
      case5=0;
     }
}
/*************************************************************

**************************************************************/
void case1(int characters){
   printf("%%");
   case5++;
   caseLast();
			switch(characters)
			{
			case '_':
				printf("5");
            case5++;
            caseLast();
            printf("F");
            case5++;
            caseLast();
				break;
			case '?':
				printf("3");
            case5++;
            caseLast();
            printf("F");
            case5++;
            caseLast();
				break;
			case ':':
				printf("3");
            case5++;
            caseLast();
            printf("A");
            case5++;
            caseLast();
				break;
			case '/':
				printf("2");
            case5++;
            caseLast();
            printf("F");
            case5++;
            caseLast();
				break;
			case '&':
				printf("2");
            case5++;
            caseLast();
            printf("6");
            case5++;
            caseLast();
				break;
			case ' ':
				printf("2");
            case5++;
            caseLast();
            printf("0");
            case5++;
            caseLast();
				break;
			default:
            printf("%c",characters);
            case5++;
            caseLast();
				break;
			}
}
/*************************************************************

**************************************************************/
void case2(int whatCase){
   if((whatCase>64)&&(whatCase<91)){
      printf("%c",(whatCase+32));
      case5++;
     }
   if((whatCase>96)&&(whatCase<123)){
      printf("%c",(whatCase-32));
      case5++;
    }
}

/*************************************************************

**************************************************************/
int main( void )
{
    int iochar;
     while ( ( iochar = getchar() ) != EOF ){               /* Note 2 */ 

        if((iochar=='_')||(iochar=='?')||(iochar==':')||(iochar=='/')||(iochar=='&')||(iochar==' ')){
           case1(iochar);
           caseLast();
         }
         else if (((iochar>64)&&(iochar<91))||((iochar>96)&&(iochar<123)))
         {
            case2(iochar);        //unprintable character
            caseLast();
         }       
         else if ((iochar>127)){
            //does nothing. case3.
         }
         else{
            printf("%c",iochar);
            case5++;
            caseLast();
         }
     }
    return 0;
}

//the problem is that someothing is adding up too much
// look at the ending of 2F
