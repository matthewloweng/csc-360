/* gopipe.c
 *
 * CSC 360, Summer 2023
 *
 * Execute up to four instructions, piping the output of each into the
 * input of the next.
 *
 * Please change the following before submission:
 *
 * Author:
 */


/* Note: The following are the **ONLY** header files you are
 * permitted to use for this assignment! */

#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <wait.h>

int main() {
    char cmds[MAX_CMDS][MAX_LENGTH];
    char buffer[MAX_LENGTH];
    int cmd_count = 0;
    int pipefd[2];
    int old_fd = 0;

    // Reading commands
    while(cmd_count < MAX_CMDS) {
        if (read(STDIN_FILENO, buffer, MAX_LENGTH) <= 1) {
            break;
        }

        buffer[strcspn(buffer, "\n")] = '\0'; // Remove trailing newline
        strncpy(cmds[cmd_count], buffer, MAX_LENGTH);
        cmd_count++;
    }

    // Executing commands and setting up pipes
    for(int i = 0; i < cmd_count; i++) {
        char *cmd[] = {cmds[i], NULL};

        pipe(pipefd);

        if (fork() == 0) {
            // Child Process
            dup2(old_fd, STDIN_FILENO); // Change the input according to the old one 

            if (i < cmd_count - 1) {
                dup2(pipefd[1], STDOUT_FILENO);
            }

            close(pipefd[0]); // No need of this right now

            execvp(cmd[0], cmd);
            exit(EXIT_FAILURE);
        } else {
            wait(NULL); // Wait for the child process to end
            close(pipefd[1]); // No need of this write end
            old_fd = pipefd[0]; // Save the input for the next command
        }
    }
    return 0

}


// int main() {

//     int fd[2];
//     pipe(fd);

//     // char *message = "Nothing working just yet... Stay tuned.\n";
//     // write(1, message, strlen(message));
// }
