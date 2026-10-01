#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <fcntl.h>

// ./slow 5 | .talk > results.out

int main() {
    int pipe_fds[2]; // 0 = read, 1 = write
    if (pipe(pipe_fds) == -1) {
        perror("Error when pipe setup");
        return EXIT_FAILURE;
    }

    pid_t pid = fork();
    switch (pid) {
    case -1:
        perror("Failed first fork");
        return EXIT_FAILURE;
    case 0: // child
        // redirect stdout to pipe write end
        if (close(pipe_fds[0]) == -1) {
            perror("Error closing read for slow");
            exit(EXIT_FAILURE);
        }
        if (dup2(pipe_fds[1], STDOUT_FILENO) == -1) {
            perror("Error converting stdout to talk file");
            exit(EXIT_FAILURE);
        }

        if (execl("./slow", "slow", "5", (char *)NULL)== -1) {
            perror("Error caling exec on slow");
        }
        exit(EXIT_FAILURE);
    default: // parent
        // wait for child
        if (wait(NULL) == -1) {
            perror("Error waiting 1 fork");
            return EXIT_FAILURE;
        }

        if (close(pipe_fds[1]) == -1) {
            perror("Error closing write after slow");
        }

        // redirect stdin to pipe read en
        if (dup2(pipe_fds[0], STDIN_FILENO) == -1) {
            perror("Error converting stdin to talk file");
            exit(EXIT_FAILURE);
        }

        // redirect stdout to external file
        int res_fd = open("./results.out",
                          O_WRONLY | O_CREAT,
                          0644);
        if (res_fd == -1) {
            perror("Cannot open results file");
            return EXIT_FAILURE;
        }
        if (dup2(res_fd, STDOUT_FILENO) == -1) {
            perror("Error converting stdout to talk file");
            exit(EXIT_FAILURE);
        }

        if (execl("./talk", "talk", (char *)NULL) == -1) {
            perror("Error caling exec on talk");
            exit(EXIT_FAILURE);
        }
    }
}