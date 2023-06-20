#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_COMMAND_LENGTH 100
#define MAX_ARGUMENTS 10
double eval_expression(const char* expression);
bool execute_command(char* command) {
    char* arguments[MAX_ARGUMENTS];
    int i = 0;

    // Tokenize the command into individual arguments
    char* token = strtok(command, " \t\n");
    while (token != NULL && i < MAX_ARGUMENTS - 1) {
        arguments[i++] = token;
        token = strtok(NULL, " \t\n");
    }
    arguments[i] = NULL;

    // Handle built-in commands
    if (strcmp(arguments[0], "cd") == 0) {
        if (i > 1) {
            if (chdir(arguments[1]) != 0) {
                printf("Directory '%s' not found.\n", arguments[1]);
            }
        }
        return true;
    } else if (strcmp(arguments[0], "exit") == 0) {
        printf("Shell exiting...\n");
        return false;
    } else if (strcmp(arguments[0], "help") == 0) {
        printf("This is a simple shell program.\n");
        printf("Available commands: cd, exit, help, calc, echo, ls, pipe, background\n");
        return true;
    } else if (strcmp(arguments[0], "calc") == 0) {
        if (i > 1) {
            // Evaluate a mathematical expression
            double result = eval_expression(arguments[1]);
            printf("Result: %.2f\n", result);
        } else {
            printf("Usage: calc <expression>\n");
        }
        return true;
    } else if (strcmp(arguments[0], "echo") == 0) {
        // Print the remaining arguments
        for (int j = 1; j < i; j++) {
            printf("%s ", arguments[j]);
        }
        printf("\n");
        return true;
    } else if (strcmp(arguments[0], "ls") == 0) {
        // Execute 'ls' command with options
        execvp("ls", arguments);
        perror("execvp() error");
        return true;
    } else if (strcmp(arguments[0], "pipe") == 0) {
        // Execute command with pipes
        execute_piped_command(arguments);
        return true;
    } else if (strcmp(arguments[0], "background") == 0) {
        // Execute command in the background
        execute_background_command(arguments);
        return true;
    }

    // Execute external command
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork() error");
    } else if (pid == 0) {
        // Child process
        execvp(arguments[0], arguments);
        // execvp() only returns if an error occurs
        perror("execvp() error");
        exit(1);
    } else {
        // Parent process
        wait(NULL);
    }

    return true;
}

double eval_expression(const char* expression) {
    // Evaluate a mathematical expression
    // You can use your own expression evaluation logic or a library like 'exprtk', 'muParser', etc.
    // This is a simple example using the '+', '-', '*', '/' operators
    double result = 0.0;
    char operator;
    double operand;

    sscanf(expression, "%lf%c%lf", &result, &operator, &operand);

    switch (operator) {
        case '+':
            result += operand;
            break;
        case '-':
            result -= operand;
            break;
        case '*':
            result *= operand;
            break;
        case '/':
            if (operand != 0.0) {
                result /= operand;
            } else {
                printf("Error: Division by zero\n");
            }
            break;
        default:
            printf("Error: Invalid operator\n");
            break;
    }

    return result;
}

void execute_piped_command(char** commands) {
    int num_pipes = 0;
    int i = 0;
    while (commands[i] != NULL) {
        if (strcmp(commands[i], "|") == 0) {
            commands[i] = NULL;
            num_pipes++;
        }
        i++;
    }

    int pipefds[2 * num_pipes];
    for (i = 0; i < num_pipes; i++) {
        if (pipe(pipefds + i * 2) < 0) {
            perror("pipe() error");
            return;
        }
    }

    int command_index = 0;
    int pipe_index = 0;
    int input_fd = 0;
    pid_t pid;

    while (commands[command_index] != NULL) {
        pid = fork();
        if (pid == 0) {
            // Child process
            if (command_index != 0) {
                dup2(input_fd, 0);
                close(input_fd);
            }

            if (commands[command_index + 1] != NULL) {
                dup2(pipefds[pipe_index + 1], 1);
                close(pipefds[pipe_index + 1]);
            }

            for (i = 0; i < 2 * num_pipes; i++) {
                close(pipefds[i]);
            }

            execvp(commands[command_index], commands + command_index);
            perror("execvp() error");
            exit(1);
        } else if (pid < 0) {
            perror("fork() error");
            return;
        }

        // Parent process
        if (command_index != 0) {
            close(input_fd);
        }

        if (commands[command_index + 1] != NULL) {
            close(pipefds[pipe_index + 1]);
            input_fd = pipefds[pipe_index];
        }

        command_index++;
        pipe_index += 2;
    }

    for (i = 0; i < 2 * num_pipes; i++) {
        close(pipefds[i]);
    }

    for (i = 0; i < num_pipes + 1; i++) {
        wait(NULL);
    }
}

void execute_background_command(char** commands) {
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork() error");
    } else if (pid == 0) {
        // Child process
        execvp(commands[0], commands);
        // execvp() only returns if an error occurs
        perror("execvp() error");
        exit(1);
    } else {
        // Parent process
        printf("Running command '%s' in the background (PID: %d).\n", commands[0], pid);
    }
}

int main() {
    char command[MAX_COMMAND_LENGTH];

    while (1) {
        printf("$ ");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = '\0'; // Remove trailing newline character

        if (!execute_command(command))
            break;
    }

    return 0;
}

