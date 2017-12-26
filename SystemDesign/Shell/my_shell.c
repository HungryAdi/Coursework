#define _GNU_SOURCE
#include <unistd.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_SIZE 1024

int open_file(char redirect[], int flag)
{
	printf("%s\n",redirect);
	int fd = 0;
	if (flag == 1)
	{
		fd = open(redirect,O_RDONLY);	
	}
	if (flag == 0)
	{
		fd = open(redirect,O_WRONLY|O_CREAT|O_TRUNC,S_IWUSR|S_IRUSR);
	}
	return fd;
}

void do_exec_pls(char* name, char* argv[], char* envp[])
{
	int pid = fork();
	if (pid >= 0)
	{
		if (pid == 0)
		{
			execvpe(name,argv,envp);
			perror("execvpe");
			exit(1);
		}
		else
		{
			wait(&pid);
		}
	}
	else
	{
		perror("fork");
	}
}

void exec_array(char* parsed_commands[], char string[])
{
	char* command = string;
	char* delim = " ";
	int index = 0;
	command = strtok(string,delim);
	parsed_commands[index++] = command;
	while ((command = strtok(NULL,delim)) != NULL)
	{
		parsed_commands[index++] = command;
	}
	parsed_commands[index] = (char*)NULL;
	//do_exec_pls(parsed_commands[0],parsed_commands,envp);
}

void do_fork(char* parsed_commands[],int fd_in[],int fd_index,int fd_out[],int fd_outdex,char* envp[])
{
	int pid = fork();

	if (pid >= 0)
	{
		if (pid == 0)
		{
			for (int i = 0; i < fd_index; ++i)
			{
				if (fd_in[0] > -1)
				{
					int fd0 = fd_in[i];
					dup2(fd0,STDIN_FILENO);
					close(fd0);
				}
			}
			for (int j = 0; j < fd_outdex; ++j)
			{
				if (fd_out[0] != -2)
				{
					int fd1 = fd_out[j];
					dup2(fd1,STDOUT_FILENO);
					close(fd1);
				}
			}
			execvpe(parsed_commands[0],parsed_commands,envp);
			perror("execvpe");
			_exit(1);
		}
		else
		{
			wait(&pid);
		}
	}
	else
	{
		perror("fork");
	}
}
	
void parse_no_io(char* command,char* envp[])
{
	char args[MAX_SIZE];
	char* parsed_commands[MAX_SIZE];
	strcpy(args,command);
	args[strlen(args)-1] = 0;
	exec_array(parsed_commands,args);
	do_exec_pls(parsed_commands[0],parsed_commands,envp);
	
}

void do_io(char* parsed_commands[],char* io_string,char* envp[])
{
	//printf("%s\n",io_string);
	char redirect[MAX_SIZE];
	int fd_in[MAX_SIZE] = {-2};
	int fd_index = 0;
	int fd_out[MAX_SIZE] = {-2};
	int fd_outdex = 0;
	int index = 0;
	for (int i = 0; io_string[i]; ++i)
	{
		switch(io_string[i])
		{
			case '<':
				for (int j = i+1; io_string[j]; ++j)
				{
					if (io_string[j] == '>' || io_string[j] == '\n')
					{
						break;
					}
					else if (io_string[j] != ' ')
					{
						redirect[index++] = io_string[j];
					}
				}
				redirect[index] = 0;
				fd_in[fd_index++] =  open_file(redirect,1);
				redirect[0] = 0;
				index = 0;
				break;
			case '>':
				for (int k = i+1; io_string[k]; ++k)
				{
					if (io_string[k] == '<' || io_string[k] == '\n')
					{
						break;
					}
					else if (io_string[k] != ' ')
					{
						redirect[index++] = io_string[k];
					}
				}
				redirect[index] = 0;
				fd_out[fd_outdex++] = open_file(redirect,0);
				redirect[0] = 0;
				index = 0;
				break;
			default:
				break;
		}
	}
	do_fork(parsed_commands,fd_in,fd_index,fd_out,fd_outdex,envp);
}

void parse_command(char* command, char* envp[])
{
	char io1[MAX_SIZE];
	char io2[MAX_SIZE];
	char args[MAX_SIZE];
	char* parsed_commands[MAX_SIZE];
	int index = 0;

	if (strchr(command,'<')==NULL && strchr(command,'>')==NULL)
	{
		parse_no_io(command,envp);
		
	}
	else
	{
		for (int i = 0; command[i]; ++i)
		{
			switch(command[i])
			{
				case '<':
				case '>':
					args[index] = 0;
					exec_array(parsed_commands,args);

					//for (int i = 0; parsed_commands[i]; ++i)
					//{
					//	printf("%s\n",parsed_commands[i]);
					//}
					do_io(parsed_commands, (char*)command+i,envp);
					return;
					break;
				default:
					args[index++] = command[i];
					break;
			}
		}
	}
}

char* prompt_command(char* command)
{
	printf("%% ");
	fgets(command,MAX_SIZE,stdin);
	return command;	
}

int main(int argc,char* argv[],char* envp[])
{
	char command[MAX_SIZE];
	while (1)
	{
		strcpy(command,prompt_command(command));
		parse_command(command,envp);
	}
	return 0;
}
