#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <errno.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/select.h>
#include <ctype.h>
#include <fcntl.h>
#include <signal.h>
#include <limits.h>
#include <sys/stat.h>

#define PORT_NUM 7286

void read_from_server(int socket_d)
{
	char buffer[BUFSIZ];
	int size = read(socket_d, buffer, BUFSIZ);
	printf("SIZE: [%i]\n",size );
	printf("[%s]", buffer);
	fflush(stdout);

}

int main(int argc, char* argv[])
{

	struct sockaddr_in server_address;
	struct hostent* entry;

	int socket_d;
	int connect_check;

	if(argc < 2)
	{
		perror("INCORRECT NUMBER OF ARGS\n");
		exit(-1);
	}

	entry = gethostbyname(argv[1]);

	if (entry == NULL)
	{
		perror("GETHOSTBYNAME() CALL FAIL #\n");
		exit(-1);
	}
	else
	{
		printf("GETHOSTBYNAME() CALL CHECK #\n");
	}

	socket_d = socket(AF_INET, SOCK_STREAM, 0);

	if (socket_d == -1)
	{
		perror("SOCKET() CALL FAIL #\n");
		exit(-1);
	}
	else
	{
		printf("SOCKET() CALL CHECK #\n");
	}

	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(PORT_NUM);
	server_address.sin_addr = *((struct in_addr *)entry->h_addr_list[0]);
	memset(&(server_address.sin_zero), '\0', 8);

	connect_check = connect(socket_d, (struct sockaddr *)&server_address, sizeof(struct sockaddr));

	if (connect_check == -1)
	{
		perror("CONNECT() CALL FAIL #\n");
		exit(1);
	}
	else
	{
		printf("CONNECT() CALL CHECK #\n");
		printf("CONNECTED ............\n\n");
	}

	char buffer[1024] = {'\0'};
	strcpy(buffer,argv[2]);
	int i = 3;
	while (i < argc)
	{
		strcat(buffer, " ");
		strcat(buffer, argv[i]);
		i++;

	}
	
	write(socket_d, buffer, sizeof(buffer));
	
	sleep(1);

	char bufin[2048];
	bzero(bufin, 2047);
	int numRead;
	while((numRead = read(socket_d, bufin, 2048)) > 0)
	{
		printf("%s\n", bufin);
		bzero(bufin, 255);
	}


	printf("CLOSING CONNECTION...\n");
	close(socket_d);

	return 0;
}
