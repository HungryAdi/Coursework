#include "Timer.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

void third_method(char* input_file, char* output_file)
{
	char buf[BUFSIZ];
	double wc, ut, st;
	int n;
	int ifp = open(input_file,O_RDONLY);
	int ofp = open(output_file,O_WRONLY | O_CREAT);
	Timer_start();
	while ((n = read(ifp,&buf,BUFSIZ)) != 0)
	{
		write(ofp,&buf,n);
	}
	Timer_elapsedTime(&wc,&ut,&st);
	close(ifp);
	close(ofp);
	printf("Wallclock Time: %lf, User Time: %lf, System Time: %lf\n",wc,ut,st);
}

void second_method(char* input_file, char* output_file)
{
	int character;
	double wc, ut, st;
	int ifp = open(input_file,O_RDONLY);
	int ofp = open(output_file,O_WRONLY | O_CREAT);
	Timer_start();
	while (read(ifp,&character,1) != 0)
	{
		write(ofp,&character,1);
	}
	Timer_elapsedTime(&wc,&ut,&st);
	close(ifp);
	close(ofp);
	printf("Wallclock Time: %lf, User Time: %lf, System Time: %lf\n",wc,ut,st);
}

void first_method(char* input_file, char* output_file)
{
	int character;
	double wc, ut, st; 
	FILE* ifp = fopen(input_file,"r");
	FILE* ofp = fopen(output_file,"w");
	Timer_start();
	while ((character = fgetc(ifp)) != EOF)
	{
		fputc(character,ofp);
	}
	Timer_elapsedTime(&wc,&ut,&st);
	fclose(ifp);
	fclose(ofp);
	printf("Wallclock Time: %lf, User Time: %lf, System Time: %lf\n",wc,ut,st);

}

int main(int argc, char* argv[])
{
	if (argc == 5)
	{
		switch (argv[1][0])
		{
			case '1':
				for (int i = 0; i < atoi(argv[4]); ++i)
				{
					first_method(argv[2],argv[3]);
				}
				break;
			case '2':
				for (int i = 0; i < atoi(argv[4]); ++i)
				{
					second_method(argv[2],argv[3]);
				}
				break;
			case '3':
				for (int i = 0; i < atoi(argv[4]); ++i)
				{
					third_method(argv[2],argv[3]);
				}
				break;
			default:
				printf("Invalid input, please try again\n");
		}
	}
}
