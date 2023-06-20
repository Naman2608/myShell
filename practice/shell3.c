#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <unistd.h>
    #include <sys/types.h>
    #include <sys/wait.h>
#endif

#define MAX_COMMAND_LENGTH 100
#define MAX_ARGUMENTS 10

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
#ifdef _WIN32
            if (SetCurrentDirectory(arguments[1]) == 0) {
                printf("Directory '%s' not found.\n", arguments[1]);
            }
#else
            if (chdir(arguments[1]) != 0) {
                printf("Directory '%s' not found.\n", arguments[1]);
            }
#endif
        }
        return true;
    } else if (strcmp(arguments[0], "exit") == 0) {
        printf("Shell exiting...\n");
        return false;
    } else if (strcmp(arguments[0], "help") == 0) {
        printf("This is a simple shell program.\n");
        printf("Available commands: cd, exit, help, calc, echo, ls\n");
        return true;
    } else if (strcmp(arguments[0], "calc") == 0) {
        if (i > 1) {
            // Evaluate a simple mathematical expression
            int result = eval_expression(arguments[1]);
            printf("Result: %d\n", result);
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
#ifdef _WIN32
        system("dir");
#else
        system("ls -al");
#endif
        return true;
    }

#ifdef _WIN32
    // Windows specific code
    // ...

    // Example of executing a command using CreateProcess
    // ...
#else
    // Unix-like systems specific code
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
#endif

    return true;
}

int eval_expression(const char* expression) {
    // Evaluate a simple mathematical expression
    // You can use your own expression evaluation logic or a library like 'exprtk', 'muParser', etc.
    // This is a simple example using the '+' operator
    int result = 0;
    char operator;
    int operand;

    sscanf(expression, "%d%c%d", &result, &operator, &operand);

    if (operator == '+') {
        result += operand;
    }

    return result;
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

