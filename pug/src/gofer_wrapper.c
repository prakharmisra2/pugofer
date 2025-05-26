#include <stdio.h>
#include "thread_stream.h"



int initializeGoferLoop(int  argc, char *argv[], OutputCallback fnCallback);

void writeStdout(char* message, int length) {
    // This function will be called from the Gofer thread
    // and will write to the standard output.
    printf("%s", message);
}

int main(int argc, char *argv[]) {
    int ret = initializeGoferLoop(argc, argv, &writeStdout);
    if (ret != 0) {
        printf("Error initializing Gofer loop: %d\n", ret);
        return ret;
    }
    // Main loop or other logic can go here
    // Start a REPL where we read from input, call writeInputStream with input and then
    // readOutputStream, all in a loop

    // Use scanf to read a line
    char input[BUFFER_SIZE];
    while (1) {
        //printf("Enter a command: ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            break; // EOF or error
        }
        writeInputStream(input);
        fflush(stdout);
    }

}