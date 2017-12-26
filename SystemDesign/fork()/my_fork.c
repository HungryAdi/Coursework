#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

#define NUM_FORKS 4
const char letter[NUM_FORKS] = {'A','B','C','D'};

void print_characters(int num_print, int index)
{
	for(int i = 0; i < num_print; ++i)
	{ 
		printf("%c",letter[index]);
		fflush(stdout); 
	}		
}

/*EXCEEDING LINE LIMIT BY 4*/
int call_forks(int run)
{
	int pids[NUM_FORKS];
	int fork_counter = 0;
	for (int i = 0; i < NUM_FORKS; ++i)
	{
		pids[i] = fork();
		if (pids[i]>= 0)
		{
			fork_counter++;
			if (pids[i] == 0)
			{
				print_characters(run,i);
				exit(0);
			}
		}
	}
	return fork_counter;
}


/*EXCEEDING LINE LIMIT BY 1 */
int main(int argc, char* argv[])
{
	int num_forks = 0;
	int pids[NUM_FORKS];
	int pid;
	int k = 10000;
	int return_status;
	if (argc == 2)
	{
		k = atoi(argv[1]);
	}

	num_forks = call_forks(k);
		
	while (num_forks > 0)
	{
		 wait(&pid);
		--num_forks;
	}
	printf("\n");
	return 0;
}
