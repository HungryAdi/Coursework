#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define __USE_GNU
#include <unistd.h>

#define MAX_QTY 20
#define STRING_LENGTH 1024

void do_exec(char* name, char* argv[], char* envp[])
{
	execvpe(name,argv,envp);
}

/*EXCEEDING LINE LIMIT BY 1*/
void parse_argv(char input[], char* envp[])
{
	int index = 0;
	char* arguments[STRING_LENGTH];
	char* temp = input;

	arguments[index++] = strtok(input," ");	
	
	while ((temp = strtok(NULL," "))!= NULL)
	{
		arguments[index++] = temp;
	}

	arguments[index] = (char*)NULL;

	
	do_exec(arguments[0], arguments, envp);	
}

int main(int argc, char* argv[], char* envp[])
{
	char input[STRING_LENGTH];
	fgets(input,STRING_LENGTH,stdin);
	input[strlen(input)-1] = 0;
	parse_argv(input,envp);
	
	return 0;	
}
