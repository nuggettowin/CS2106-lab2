#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>

// program that pipes the output of one program to the input of another, and redirect final result to results.out
// ./slow 5 | .talk > results.out
// execlp don't specify full path to run

int main() {
    int pipe_fds[2]; // 0 = read, 1 = write
    int pipe_res = pipe(pipe_fds);
    if (pipe_res == -1) {
        perror("Error when pipe");
        return EXIT_FAILURE;
    }

    pid_t pid = fork();
    switch (pid) {
    case -1:
        perror("Failed first fork");
        return EXIT_FAILURE;
    case 0: // child
        printf("Child 1 forked\n");

        // redirect output 
        close(pipe_fds[0]);
        int change_slow_out_fd_res = dup2(pipe_fds[1], STDOUT_FILENO);
        if (change_slow_out_fd_res == -1) {
            perror("Error converting stdout to talk file");
            exit(EXIT_FAILURE);
        }

        int res = execl("./slow", "slow", "5", (char *) NULL);
        if (res == -1) {
            perror("Error caling exec on slow");
        }
        exit(EXIT_FAILURE);
    default: // parent
        pid_t wait1_res = wait(NULL);
        if  (wait1_res == -1) {
            perror("Error waiting 1 fork");
            return EXIT_FAILURE;
        }
        printf("Child exited\n");
    }

    close(pipe_fds[1]);
    pid_t pid2 = fork();
    switch (pid2) {
    case -1:
        perror("Failed second fork");
        return EXIT_FAILURE;
    case 0: // child
        printf("Child 2 forked\n");

        // redirect stdin
        int change_talk_in_fd_res = dup2(pipe_fds[0], STDIN_FILENO);
        if (change_talk_in_fd_res == -1) {
            perror("Error converting stdin to talk file");
            exit(EXIT_FAILURE);
        }

        // redirect stdout
        int res_fd = open("./results.out", 
                O_WRONLY | O_CREAT,
                0644
            ); 
        if (res_fd == -1) {
            perror("Cannot open results file");
            return EXIT_FAILURE;
        }
        int change_talk_out_fd_res = dup2(res_fd, STDOUT_FILENO);
        if (change_talk_out_fd_res == -1) {
            perror("Error converting stdout to talk file");
            exit(EXIT_FAILURE);
        }

        int res = execl("./talk", "talk", (char *) NULL);
        if (res == -1) {
            perror("Error caling exec on talk");
            exit(EXIT_FAILURE);
        }
    default: // parent
        pid_t wait2_res = wait(NULL);

        close(pipe_fds[0]);
    }

    return EXIT_SUCCESS;

    //
    // Add code here to pipe from ./slow 5 to ./talk and redirect
    // output of ./talk to results.out
    // I.e. your program should do the equivalent of ./slow 5 | talk > results.out
    // WITHOUT using | and > from the shell.
    // Look at how we did < and > and | in the previous parts of this lab, and do the same!

}

