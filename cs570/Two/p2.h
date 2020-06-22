#include <stdio.h>
#include "getword.h"
#include <unistd.h>
#include <wait.h>
#include <fcntl.h>
#include <stdlib.h>
#include <signal.h>
#define MAXITEM 100     /* max number of words per line */
#define FLAG "-1"
#define MAXARGS MAXITEM*STORAGE*10
#define SIZE_FOR_OUTFILE 1

int fork_test();
int less_than();
int greater_than();
int amber_sign();
int go_ahead_and_execute();
int parse();
int parent_process();
int child_process();
int piping();
void myhandler();
int killpg();
