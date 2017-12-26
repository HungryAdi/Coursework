#include "my_ls.h"

char* get_time(struct stat path, char* buf)
{
	time_t t = path.st_mtime;
	struct tm* lt = localtime(&t);
	buf = asctime(lt);
	buf[strlen(buf)-1] = 0;	
	return buf;
}

char* get_user_name(struct stat path)
{
	struct passwd *pw = getpwuid(path.st_uid);
	if (!pw)
	{
		return "";
	}
	return pw->pw_name ? pw->pw_name : "";
}

char* get_group_name(struct stat path)
{
	struct group *grp = getgrgid(path.st_gid);
	if (!grp)
	{
		return "";
	}
	return grp->gr_name ? grp->gr_name : "";
}

void print_user(struct stat path)
{
	printf((S_ISDIR(path.st_mode)) ? "d" : "-");
	printf((path.st_mode & S_IRUSR) ? "r" : "-");
	printf((path.st_mode & S_IWUSR) ? "w" : "-");
	printf((path.st_mode & S_IXUSR) ? "x" : "-");
}

void print_group(struct stat path)
{
	printf((path.st_mode & S_IRGRP) ? "r" : "-");
	printf((path.st_mode & S_IWGRP) ? "w" : "-");
	printf((path.st_mode & S_IXGRP) ? "x" : "-");
}

void print_other(struct stat path)
{
	printf((path.st_mode & S_IROTH) ? "r" : "-");
	printf((path.st_mode & S_IWOTH) ? "w" : "-");
	printf((path.st_mode & S_IXOTH) ? "x" : "-");
}

void print_file_mode(struct stat path)
{
	print_user(path);
	print_group(path);
	print_other(path);	
}

void print_tabs(int level)
{
	for (int i = 0; i < level; ++i)
	{
		printf("    ");
	}
}

void print_file(char input[])
{   
    struct stat file_info;
	char buf[80];
    stat(input,&file_info);
    print_file_mode(file_info);
	printf(" %d %s %s %d %s %s\n ",file_info.st_nlink,get_user_name(file_info),get_group_name(file_info),file_info.st_size,get_time(file_info,buf), input);
}

int is_dir(char name[])
{
	struct stat file_info;
	stat(name,&file_info);
	return S_ISDIR(file_info.st_mode);
}


void print_working_directory()
{
	DIR* cwd = opendir(".");
	struct dirent* file;

	while((file = readdir(cwd)) != NULL)
	{
		if (strcmp(file->d_name,".") == 0 || strcmp(file->d_name,"..") == 0)
		{
			continue;
		}
		else
		{
			print_file(file->d_name);
		}
	}
	closedir(cwd);
}

int perform_ls(char dir_name[], int level)
{
	DIR* dirp;
	struct dirent* direntp;
	char tabs[MAX_SIZE];


	if (!is_dir(dir_name))
	{
		print_tabs(level);
		print_file(dir_name);
		return 0;
	}
	if ((dirp = opendir(dir_name)) == NULL)
	{
		return errno;	
	}

	print_tabs(level);

	while((direntp = readdir(dirp)) != NULL)
	{
		char new_path[MAX_SIZE];
		strcpy(new_path,dir_name);

		if (strcmp(direntp->d_name, ".") == 0 || strcmp(direntp->d_name, "..") == 0)
		{
			continue;
		}
		//printf("%s\n",new_path);
		if (is_dir(new_path))
		{
			strcat(new_path, "/");
			strcat(new_path, direntp->d_name);
			perform_ls(new_path,level+1);
		}
	}
	return 0;
}

void print_command_line(int arguments, char* command[])
{
    for (int i = 1; i < arguments; ++i)
    {   
		char buf[MAX_SIZE];
		if (is_dir(command[i]) == 1)
		{
			char path[MAX_SIZE];
			//getcwd(path,MAX_SIZE);
			//strcat(path, "/");
			//strcat(path,command[i]);
			//if (strcmp(command[i],getcwd(buf,MAX_SIZE)) == 0)
			//{
				strcpy(path,command[i]);
			//}
			perform_ls(path,0);
		}
		else
		{
			print_file(command[i]);
		}
    }
}


void do_ls(int arguments, char* argument_list[])
{
	if (arguments > 1)
	{
		print_command_line(arguments, argument_list);
	}
	else
	{
		print_working_directory();	
	}
}
