#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>

int int_counter = 0;
int quit_counter = 0;
int stop_counter = 0;

void print_status()
{
	printf("\nInterrupt: %d\n",int_counter);
	printf("Stop: %d\n",stop_counter);
	printf("Quit: %d\n",quit_counter);
}

void init_signal_handlers(int signum)
{
	switch (signum)
	{
		case SIGINT:
			int_counter++;
			printf("L ");
			fflush(stdout);
			break;
		case SIGQUIT:
			quit_counter++;
			printf("Q ");
			fflush(stdout);
			break;
		case SIGTSTP:
			stop_counter++;
			printf("S ");
			fflush(stdout);
			
			if (stop_counter == 3)
			{
				print_status();
				exit(0);
			}
			
			raise(SIGSTOP);
		default:
			break;
	}			
}

void signal_check()
{
	if (signal(SIGINT, init_signal_handlers) == SIG_ERR)
	{
		printf("\ncan't catch SIGINT\n");
	}
	if (signal(SIGQUIT, init_signal_handlers) == SIG_ERR)
	{
		printf("\ncan't catch SIGQUIT\n");
	}
	if (signal(SIGTSTP, init_signal_handlers) == SIG_ERR)
	{
		printf("\ncan't catch SIGTSTP\n");
	}	
}


int main()
{
	while(1)
	{
		sleep(1);
		printf("X ");
		fflush(stdout);
		signal_check();
	}

	return 0;
}
