#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
 
#define MAX_SIZE 1024

void do_ls(int arguments, char* argument_list[]);
void print_command_line(int arguments, char* command[]);
int perform_ls(char dir_name[], int level);
void print_working_directory();
int is_dir(char name[]);
void print_file(char input[]);
void print_tabs(int level);
void print_file_mode(struct stat path);
void print_other(struct stat path);
void print_group(struct stat path);
void print_user(struct stat path);
char* get_group_name(struct stat path);
char* get_user_name(struct stat path);
char* get_time(struct stat path, char* buf);
