/*************************************************************
CS320 -Program1
Tom Nguyen
Professor: Dr. John Carroll
username: CSSC0155@edoras.sdsu.edu
Aprile 24, 2017
File: p4.c
Notes:  p4 will read data from standard
input one character at a time and output the results to standard output.
It will delete all non-ASCII characters, change the case
of alphabetic characters, 'fold' long lines, and make browser-friendly
adjustments to the characters _, /, :, ?, &, and space.
**************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#define FOLDING 20  /*length of characters per line*/
#define TOOBIG 2048 //the limited size, 2^11.


/** Global variables **/
int oldMain(bool output,FILE *fout); //old p1.c
char fileArray[0];   //array holding the characters from file and converted.
char* starterPointer;  //when no files are put in. array will grow and resize.
int location=0; //a core variable used for looping. store character into location.
static int case5=0;         /*count characters*/
static int changed=1;       /*1=no-change, 0=changed*/

void case1(int characters);
void oldcase1(int characters); //dealing with no files.
static void caseLast();
static void check();
static void case2(int whatCase);
void oldcase2(int whatCase);
void testprint(char* array, int size); //for debugging for loop prints array[size];
void outprint(char* array, int size, FILE *fout);//prints out to a file.
int finalprintsize(char* array, int size);//go through the array one last time to finally get the true size of printout array.
char* finalarray(char* array,int fileSize, char* newArray,int size); //final array. print as is.
int convert(char* argv); //does all the steps 1-5. coverts everything into a simple array.
int subconvert(int iochar,int fileSize); //the core part of converting. This will be used twice.
void longfile(FILE * openFile,FILE * fout,bool output); //deal with file that are really long.
int toobig; //if file is bigger then this
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
    *(fileArray+location++)='%';
    changed=0; //put in after tester1 april 21, 2017 8:51AM
    switch(characters)
    {
        case 95:
        *(fileArray+location++)='5';
        *(fileArray+location++)='F';
        break;
        case 63:
        *(fileArray+location++)='3';
        *(fileArray+location++)='F';
        break;
        case 58:
        *(fileArray+location++)='3';
        *(fileArray+location++)='A';
        break;
        case 47:
        *(fileArray+location++)='2';
        *(fileArray+location++)='F';
        break;
        case 38:
        *(fileArray+location++)='2';
        *(fileArray+location++)='6';
        break;
        case 32:
        *(fileArray+location++)='2';
        *(fileArray+location++)='0';
        break;
        default:
        *(fileArray+location++)=(char)characters;
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
    if((whatCase>64)&&(whatCase<91))   /*uppercase A-Z decimal value 65-90*/
    *(fileArray+location++)=(whatCase+32); //changed to lowercase
    
    if((whatCase>96)&&(whatCase<123))  /*lowercase a-z decimal value 97-122*/
    *(fileArray+location++)=(whatCase-32); //changed to uppercase
}

/*************************************************************
Main: The concept for this program was taken from "C by
Discovery" by Leslie Foster. In chapter 2 the file inout2.c was used.
Page 52 from CS320 Programming Languages Dr. John Carroll Spring 2017
Command line arguments: arg.c
returns values: //values were picked in the order we programmed it.
0. at lease one change was made and it was successful
1. no change was made
2.
3. argument ended with -o so there was no file to write to
4. there was no file to open, null file.
5. File was not able to close properly.
6. output file was not able to close properly
7. Error 7: File is really long

unexpected behavior:
-If print with "%s",*array. Will print unrecongized characters.
so to prevent this we put it in a for loop. otherwise improper printing.
However, if we were to fix it we would store *string = testprint. and change
testprint to return a string.
-If using -o outputfile1 and outputfile1 already exist. overwrite it. it will
not add on to the file.
-if using -o outfile -o secondOutFile. will make two outfiles and not read anything in.
only handle the case of having one -o outfile. However, if -o outfile infile will work.
-if using -o outfile <input1 <input2. It will only record the last one. input will be deleted
from the output file and only input2 will be passed in.
-if using p4 <input1 <input6. it will only print the lastest one.
ignoring input1 completely.
-if using p4 <input6 input1 it will only readin input1 and ignore
any input from stdin (input6).
-if typed p4 -o outputfile <input -o secondoutput. It see the secondoutput as a file and returns an error.
-if type p4 nosuchfile <input1 nosuchfile2 will not properly run
**************************************************************/
int main(int argc, char *argv[]){
    int i; //a core variable used for looping the args.
    int fileSize,foutFirst;
    int iochar=0;
    bool fileWasOpen = false;
    bool okayToClose = false;
    bool printToScreen = true;
    bool oldmain =false;
    bool error4=false;
    bool error7=false;
    bool oExist=false;
    FILE *openFile, *fout ;
    
    //       argc=1;
    //      argv[1]="-o";
    
    /*
    argc=4;
    argv[1]="C:/Users/tom/Desktop/projects/cs320/Program 4/input1";
    argv[2]="C:/Users/tom/Desktop/projects/cs320/Program 4/input5";
    argv[3]="C:/Users/tom/Desktop/projects/cs320/Program 4/input6";
   // argv[2]="-o";
    //argv[3]="C:/Users/tom/Desktop/projects/cs320/Program 4/outfile1";
    //  argv[4]="-o";
    //  argv[5]="C:/Users/tom/Desktop/projects/cs320/Program 4/outfile2";
    argv[4]="C:/Users/tom/Desktop/projects/cs320/Program 4/input6";
    argv[5]="-o";
    argv[6]="C:/Users/tom/Desktop/projects/cs320/Program 4/outfile2";
    argv[7]="C:/Users/tom/Desktop/projects/cs320/Program 4/input5";
    argv[8]="C:/Users/tom/Desktop/projects/cs320/Program 4/input1";
    argv[9]="C:/Users/tom/Desktop/projects/cs320/Program 4/input6";
    */
//     for(i=0; i<argc; ++i)
//       printf("argv[%d]:%s",i,argv[i]);

    if(argc>0)
    for(i=0;i<argc;i++)
      if(strcmp(argv[i], "-o")==0){
        printToScreen = false;
        oExist=true;
        if(i+1 >= argc){
            fprintf(stderr,"\nError: 3. ending with -o. missing outputfile\n");
            return 3;
        }
        foutFirst = ++i;
        break;
    }
    if( argc>1){
        if(!printToScreen){
            fout = fopen(argv[foutFirst],"w");
            fileWasOpen=true;
        }
        for(i=1; i<argc; ++i){
            if(strcmp(argv[i], "-o")==0){
                if(i+1 >= argc){
                    fprintf(stderr,"\nError: 3. ending with -o. missing outputfile\n");
                    return 3;
                }
                if(fileWasOpen && !(strcmp(argv[i+1], argv[foutFirst])==0)){
                    if(fclose(fout)==EOF){
                        fprintf(stderr,"\nError: 6. File could not be closed\n");
                        return 6;
                    }
                    // fclose(fout); //edoras does not like this command.
                    fout = fopen(argv[++i],"w");//use the next argument and increase the pointer so the loop will not use it.
                    fileWasOpen=true;
                } 
                if(i+1 == argc) //last argument. it is okay to close the file.
                    okayToClose =true;
                else
                    i++;
            }//end if for if(strcmp(argv[i], "-o")==0)
            //                FILE* mystream = stdout;
            
            if(argc==3&& oExist){ //take in input from screen and writing to file.
                oldmain=false;
                oldMain(oldmain,fout); //passing in false
            }
            
            openFile = fopen(argv[i],"r");
            if(openFile==NULL){ //http://stackoverflow.com/questions/4847725/file-read-error-success
                fprintf(stderr,"\nError 4: File Read Error \"%s\"\n",argv[i]);
                error4 = true;
                continue;
            }
            if(openFile){
               toobig=0;
               while( (iochar = fgetc(openFile)) != EOF)
                toobig++;
               fseek(openFile,0,SEEK_SET);
            }
             if(toobig>TOOBIG){
                error7=false;
                longfile(openFile,fout,error7);
                error7 =true;
                continue;
             }
            fileSize=convert(argv[i]);
            int finalSize = finalprintsize(fileArray,  fileSize);
            char finalArray[finalSize];
            char* finalstring = finalArray;
            char* startstring = fileArray;
            finalstring = finalarray(startstring,fileSize, finalstring,finalSize);
            
            if(printToScreen)//if(!fileWasOpen)//
               testprint(finalstring,finalSize);
            else
                outprint(finalstring, finalSize, fout);
            
            if(fclose(openFile)==EOF){
                fprintf(stderr,"\nError: 5. File could not be closed\n");
                return 5; //error:
            }
            //  else //edoras does not like this command
            //                fclose(openFile); //;fprintf(stderr,"");
            
            if(okayToClose)
             if(fileWasOpen){
                if(fclose(fout)==EOF){
                    fprintf(stderr,"\nError: 5. Out File could not be closed\n");
                    return 6;
                }
                //fclose(fout); //edoras does nto like this
              } 
        }//ending loop for for(i=1; i<argc; ++i)

    }//ending for loop if( argc>1)
    
    else{
        oldmain=true;
        oldMain(oldmain,fout); //passing in true;
    }
    if(error4)
      return 4;
    else if(error7)
      return 7;
    return changed;                    //returns 1 if no change otherwise 0.
}

int convert(char* argv){ //does all the steps 1-5. coverts everything into a simple array.
    FILE *openFile;
    int fileSize; //The size of the file. This will be set for array size.
    int iochar;
    location=0;
    openFile = fopen(argv,"r");
    fseek(openFile,0,SEEK_END);   //looking where the end of the file is. pointer will be at end of file.
    fileSize=ftell(openFile);
    fseek(openFile,0,SEEK_SET);   //set the pointer back to the front of the file to be read at.
    while( (iochar = fgetc(openFile)) != EOF)//each of these character will be convert to take up three more spaces.
    if((iochar==95)||(iochar==63)||(iochar==58)||(iochar==47)||(iochar==38)||(iochar==32))
    fileSize+=2;
    
    fseek(openFile,0,SEEK_SET);   //set the pointer back to the front of the file to be read at.
    fileArray[fileSize]; //initialized the array that was at fileArray[0];
    
    //values 95=_, 63=?, 58=:, 47=/, 38=&, and 32= space
    while ( (iochar = fgetc(openFile)) != EOF )   //stops when end of file is reached
        fileSize= subconvert(iochar,fileSize);
    return fileSize;
}
void longfile(FILE * openFile, FILE *fout, bool output){ //deal with file that are really long. deal with 500 characters at a time.
   int iochar,iochar2;
   int i=0;
   int q=0;
   int fileSize;
   char* finalstring;
   char array[500]; //picked a random number. half of 1000. should be 25 lines at a time.

      while ( ( iochar = getchar() ) != EOF ){
         if(i==500 |((iochar2 = getchar()) == EOF)){
             fileSize=i;
         for (q=0; q<i;q++){//each of these character will be convert to take up three more spaces.
            iochar=array[q];
            if((iochar==95)||(iochar==63)||(iochar==58)||(iochar==47)||(iochar==38)||(iochar==32))
            fileSize+=2;
         }
         *(fileArray+fileSize);
         location=0;
         for(q=0;q<i;q++){
            iochar=array[q];
            fileSize= subconvert(iochar,fileSize);
         }
    
         int finalSize = finalprintsize(fileArray,  fileSize);
         char finalArray[finalSize];
         finalstring = finalArray;
         char* startstring = fileArray;
         finalstring = finalarray(startstring,fileSize, finalstring,finalSize);
    
         if(output){
            testprint(finalstring,finalSize);
            fprintf(stderr,"%c",'\n');
            }
         else{
            outprint(finalstring, finalSize,fout);
            putc('\n', fout);
            }
         i=0;
         } else
            ungetc(iochar2,stdin);   
         
         array[i++]=iochar;
      }//end of while loop EOF    


}
int subconvert(int iochar,int fileSize){ //the core part of converting. This will be used twice.
    if((iochar==95)||(iochar==63)||(iochar==58)||(iochar==47)||(iochar==38)||(iochar==32))
        case1(iochar);              //deals with condition 1
    else if (((iochar>64)&&(iochar<91))||((iochar>96)&&(iochar<123)))                            //ascii base-10 for a-z and A-Z
        case2(iochar);             //changes the letter case
    else if ((iochar>126)){       //Deals with condition 3
        fileSize-=1;
        changed=0;     //change was made
    }
    else if(iochar>0 && iochar<128)
      *(fileArray+location++)=iochar;
    return fileSize;
}

int oldMain(bool output,FILE *fout){
    int iochar;         //used for getchar*
    int size=0; //the size of the string
    int q; //variable for for loop.
    int b; //free variable for loops.
    int startsize=25; //default array 256. will change if size grows.
    int fileSize=0; //the modify size
    char startArray[25];
    
    starterPointer =startArray;
    if(stdin){
      toobig=0;
      while ( ( iochar = getchar() ) != EOF )
      printf("%c",iochar);
         toobig++;
      fseek(stdin,0,SEEK_SET);
    }    
    if(toobig>TOOBIG){
               fprintf(stderr,"\nError 7: Long file unable to properly handle.\n");
               longfile(stdin,fout,output); //to screen           
               return 7;
            }
    
    while ( ( iochar = getchar() ) != EOF ){ //a loop that will count the stdin.
        
        size++;
        *(starterPointer+location++)=iochar;
        if(location==startsize){
            startsize*=startsize;
            char temp[startsize];
            for(b=0;b<location;b++)
            *(temp+b) = *(starterPointer+b);
            char currentArray[startsize];
            for(b=0;b<location;b++)
            *(currentArray+b) = *(temp+b);
            starterPointer=currentArray;
        }
    }
    
    fileSize=size;
    for (q=0; q<size;q++){//each of these character will be convert to take up three more spaces.
        iochar=*(starterPointer+q);
        if((iochar==95)||(iochar==63)||(iochar==58)||(iochar==47)||(iochar==38)||(iochar==32))
        fileSize+=2;
    }
    *(fileArray+fileSize);
    location=0;
    for(q=0;q<size;q++){
        iochar=*(starterPointer+q);
        fileSize= subconvert(iochar,fileSize);
    }
    
    int finalSize = finalprintsize(fileArray,  fileSize);
    char finalArray[finalSize];
    char* finalstring = finalArray;
    char* startstring = fileArray;
    finalstring = finalarray(startstring,fileSize, finalstring,finalSize);
    printf("output:%s\n",output? "true" : "false");
    printf("finalstring:%s\n",finalstring);
    printf("finalSize:%d\n",finalSize);
    if(output)
    testprint(finalstring,finalSize);
    else
        outprint(finalstring, finalSize,fout);
    return changed;
}


int finalprintsize(char* array, int size){
    int finalsize = size;
    int foldCounter =0;
    int z; // variable for loops.
    for(z=size-1; z>-1;z--){ //storing the value backwards in fileArray into the front of backprint array
        foldCounter++;
        if(*(fileArray+z)=='\n')
            foldCounter=0;
        if(foldCounter==FOLDING){
            finalsize++;
            foldCounter=0;
        }
    }//end for loop  for(z=size; z>0;z--)
    return finalsize;
}

char* finalarray(char* array,int arraysize, char* newArray,int size){ //final array. print as is.
    int z; //variable for loops.
    int foldCounter=0;
    int position=size-1; //for loading in newArray
    for(z=arraysize-1; z>-1;z--){ //storing the value backwards in fileArray into the front of backprint array
        foldCounter++;
        *(newArray+position--)=*(fileArray+z);
        if(*(fileArray+z)=='\n')
            foldCounter=0;
        if(foldCounter==FOLDING){
            *(newArray+position--)='\n';
            foldCounter=0;
        }
    }//end for loop  for(z=size; z>0;z--)
    return newArray;
}

void testprint(char* array, int size){
    int z;
    for(z=0; z<size;z++)
        printf("%c",*(array+z));
        //fprintf(stderr,"%c",*(array+z));
    return;
}

void outprint(char* array, int size, FILE *fout){//prints out to a file.
    int z;
    for(z=0; z<size; z++)
      putc(*(array+z), fout);
    //if you want to print using string:
    //    char tempArray[size];  char *tempArrays; *(tempArray+z)=*(array+z);
    // tempArrays=tempArray;
    //fprintf(fout,"%s",tempArrays);
    return;
}

