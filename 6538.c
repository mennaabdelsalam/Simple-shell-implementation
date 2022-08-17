#include <stdio.h>
#include<time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>
#include<signal.h>
#include<stdlib.h>
#define MAX_LINE 100 /* The maximum length command */

 
int main(void){
char *args[MAX_LINE/2 + 1]; /* command line arguments */
int running = 1; /* flag to determine when to exit program */
char input[MAX_LINE];
int i = 0;
 

 
void  mychildhandler(int sig)
{
    
    int status;
    waitpid(-1, &status, WNOHANG); //i hope to not have zombies in the moniter but i have failed to handle it :(
    //pid_t pid;*/
    //int status;
   FILE *fptr;    
   fptr = fopen("log.txt","a");
    time_t timer;
    char buffer[26], line[50]; // puisque j'ai utiliser append to my file each time i haven't creadted a file every time 
    struct tm* mytime;      //so i decide to print the time (just have a clean code :) 
    time(&timer);
    mytime = localtime(&timer); //it's a built in fuction to get the time by including <time>
    strftime(buffer, 26, "%d-%m-%y %H:%M:%S", mytime); //day-month-year hour:minute:second 
    fputs(buffer,fptr);
     fprintf(fptr,"the child process has terminated \n");
     fclose(fptr);   
    //append to the log file "the child process has terminated"
}
  signal(SIGCHLD,mychildhandler); //it's a signal to send to the parent that his child had finished and then to print in the file
//"the child has terminated".
 
 while(1){ 
 
  int flagforbackground = 0;
    printf("shell>> ");
    fflush(stdout);
    scanf ("%[^\n]%*c", input); //take the input line
    i = 0;
    args[i] = strtok(input," "); //tokenize and save in the array of tokens which is (args)
    while (args[i] != NULL){
      i++; //increment the size by one
      args[i] = strtok(NULL, " "); //delete the space
      //printf(%s,args);
    }
    if(strcmp(args[0], "exit") == 0) /* exit command */
      break;
    if(strcmp(args[i-1], "&") == 0){ /* if & is included process running in background*/
        args[i-1] = NULL; //delete the last element of the array &
        flagforbackground = 1;
    }
    printf("%d",flagforbackground);
   

     
    pid_t pid;
    pid = fork(); /* fork child process */
    if(pid < 0){  //if fork failed due to some reason
         printf("somthing went wrong!");
        return 1;

    }

    if (pid == 0){
        execvp(args[0],args); // execute the command  ,this is the child
        if (execvp(args[0],args) < 0) {    
           printf("ERROR: Invalid command\n");
           break;
        }
    } else{               //this is the parent
          
    if (!flagforbackground){         //to wait the flag should be zero 

    //  wait (NULL );
        waitpid(pid,NULL,0 ); 
        //on a 3 chois  
        //--> whonhang:kill preocess process
        //-->whontrace:kill group of precesses
        //-->0 idk why we have choosen it instead of whontraced (tho whonhang do the same that i need) but it looks    easier :)
        //background=0;
        

    }
    }
  
  }

  return 0;

}
//i have that my code was well formated (it's funcional tho) but using file.c sucks,i will use vs editer next times :)
