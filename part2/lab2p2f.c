#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>

// program that pipes the output of one program to the input of another, and redirect final result to results.out
// ./slow 5 | .talk > results.out
// execlp don't specify full path to run

int main() {

    printf("Be patient, the program will take around 7 seconds to run.\n");
    printf("At the end you can do \"cat results.out\" to see the result.\n");

    pid_t pid = fork();

    switch (pid) {
    case -1:
        perror("Failed first fork");
        exit(EXIT_FAILURE);
    case 0: // child
        printf("Child forked\n");
        int res = execl("./slow", "slow", "5", (char *) NULL);
        if (res == -1) {
            perror("Error caling exec on slow");
        }
        exit(EXIT_FAILURE);
    default: // parent
        pid_t wait1_res = wait(NULL);
        if  (wait1_res == -1) {
            perror("Error waiting 1 fork");
            exit(EXIT_FAILURE);
        }
        printf("Child exited\n");
    }

    pid_t pid2 = fork();

    switch (pid2) {
    case -1:
        perror("Failed second fork");
        exit(EXIT_FAILURE);
    case 0: // child
        printf("Child forked\n");
        int res = execl("./talk", "talk", (char *) NULL);
        if (res == -1) {
            perror("Error caling exec on talk");
        }
        exit(EXIT_FAILURE);
    default: // parent
        pid_t wait2_res = wait(NULL);
        if  (wait2_res == -1) {
            perror("Error waiting 2 fork");
            exit(EXIT_FAILURE);
        }
        printf("Child exited\n");
    }

    //
    // Add code here to pipe from ./slow 5 to ./talk and redirect
    // output of ./talk to results.out
    // I.e. your program should do the equivalent of ./slow 5 | talk > results.out
    // WITHOUT using | and > from the shell.
    // Look at how we did < and > and | in the previous parts of this lab, and do the same!

}

