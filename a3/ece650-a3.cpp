#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <vector>
#include <sys/wait.h>
using namespace std;
// https://stackoverflow.com/questions/8860895/how-do-you-use-dup2-and-fork-together
// https://stackoverflow.com/questions/7867014/which-end-of-a-pipe-is-for-input-and-which-for-output
// https://www.reddit.com/r/linux/comments/19884er/best_way_to_effectively_kill_a_process/

int main(int argc, char **argv){
  pid_t child1,child2,child3;

  // Generate Pipe
  int rgenToa1[2]; //Pipe from rgen and ece650-a1
  int a1Toa2[2]; // Pipe from ece650-a1 and ece650-a2
  pipe(rgenToa1);
  pipe(a1Toa2);
  pid_t childProcessId;

  // Forking first child process
  childProcessId=fork();
  if(childProcessId == 0){
    // Redirecting the stdout to the write end of the pipe rgenToa1.
    dup2(rgenToa1[1],STDOUT_FILENO);  
    close(rgenToa1[0]);
    close(rgenToa1[1]);

    // Executing the rgen program
    int random = execv("rgen",argv);
    return random; // Returning the state of the rgen
  }

  child1 = childProcessId;
  

  //Creating a new child process for interaction between a1 and a2.
  childProcessId = fork();
  if(childProcessId == 0){
     // redirecting stdin to the read end of pipe rgenToa1. This will send the input to our ece650-a1.py from the read end of the first pipe.
    dup2(rgenToa1[0], STDIN_FILENO);
    close(rgenToa1[0]);
    close(rgenToa1[1]);

    // Redirecting stdout to the write end of the pipe a1Toa2 as we have to get the output of a1.
    dup2(a1Toa2[1],STDOUT_FILENO); 
    close(a1Toa2[0]);
    close(a1Toa2[1]);

     char* argv0[3];

     //We have to write the name of the command that is going to execute the pyhton file.
     argv0[0] = (char *)"python3";
     argv0[1] = (char *)"ece650-a1.py";
     argv0[2] = nullptr;

     return execvp(argv0[0],argv0); // Return the status of the execution.
  }

  child2 = childProcessId;

  childProcessId = fork();
  if(childProcessId == 0){
    char* argv1[2];
    argv1[0] = (char *)"ece650-a2";
    argv1[1] = nullptr;
    // Redirect stdin to the read end of a1Toa2 as we have to read the output of the a1 as an input to a2.
    dup2(a1Toa2[0],STDIN_FILENO);
    close(a1Toa2[0]);
    close(a1Toa2[1]);

    int executea2 = execv(argv1[0],argv);
    return executea2;
  }
  child3 = childProcessId;

  dup2(a1Toa2[1],STDOUT_FILENO);
  close(a1Toa2[1]);
  while(!cin.eof()){
    string line;
    getline(cin, line);
    cout<<line<<endl;
  }

    // Killing the child process gracefully nand only then the parent process will be killed.
    int status;
    kill(child1,SIGTERM);
    waitpid(child1, &status, 0);

    kill(child2,SIGTERM);
    waitpid(child2, &status, 0);

    kill(child3,SIGTERM);
    waitpid(child3, &status, 0);
    
    return 0;
}