#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "shell.h"

int main(int argc, char **argv)
{
	char* cmd; // A variable to store commands
	do{
		print_prompt1();

		cmd = read_cmd();

        // If there's an error reading the command, we exit the shell
		if(!cmd){
			exit(EXIT_SUCCESS);
		}

		// If the command is empty (i.e. the user pressed ENTER without writing anything, we skip this input and continue with the loop.
		if (cmd[0]=='\0' || strcmp(cmd,"\n")==0)
		{
			free(cmd);
			continue;
		}

		//  If the command is exit, we exit the shell.
	    if(strcmp(cmd, "exit\n") == 0)
        {
            free(cmd);
            break;
        }

        //Otherwise, we echo back the command, free the memory we used to store the command, and continue with the loop. 
	    printf("%s\n",cmd);
	    free(cmd);

	}while(1);
	exit(EXIT_SUCCESS);
}

char *read_cmd(void){

	char buf[1024];
	char *ptr =NULL;
	char ptrlen = 0;
    
    // we read input from stdin in 1024-byte chunks and store the input in a buffer(buf).
	while(fgets(buf,1024,stdin)){

		int buflen = strlen(buf);

		if(!ptr){
			//The first time we read input (the first chunk for the current command),
			// we create our buffer using malloc().
			ptr = malloc(buflen+1);
		}
		else{
			// For subsequent chunks, we extend the buffer using realloc(). 
			char *ptr2 = realloc(ptr,ptrlen+buflen+1);	

			if(ptr2){
				ptr = ptr2;
			}
			else{
				free(ptr);
				ptr = NULL;
			}
		}
		if (!ptr)
		{
			// We shouldn’t encounter any memory issues here, but if something wrong happens, 
			// we print an error message and return NULL.
			fprintf(stderr,"error : failled to alloc buffer : %s\n",strerror(errno));
			return NULL;
		}
         
        // If everything goes well, we copy the chunk of input we’ve just read from the user to our buffer,
        // and we adjust our pointers accordingly.
		strcpy(ptr+ptrlen,buf);

		// Here, we check to see if the input we’ve got in the buffer ends with \n and, if so,
		// if the \n is escaped by a backslash character \\. If the last \n is not escaped,
		// the input line is complete and we return it to the main() function. Otherwise,
		// we remove the two characters (\\ and \n), print out PS2, and continue reading input.
		if(buf[buflen-1] == '\n')
        {
            if(buflen == 1 || buf[buflen-2] != '\\')
            {
                return ptr;
            }

            ptr[ptrlen+buflen-2] = '\0';
            buflen -= 2;
            print_prompt2();
        } 

        ptrlen += buflen;
	}

     return ptr;

}
