//String library function implementations abd
#include <string.h>
#include <stdio.h>

#define require(e) if (!(e)) fprintf(stderr, "Failed line %d        %s: %s       %s\n",__LINE__,__FILE__,__func__,#e)

static char* strtok_string;

// 1) strlen
int ai_strlen(const char* str)
{
	return *str ? 1 + ai_strlen(str+1) : 0;
}

void test_strlen()
{
	require(ai_strlen("") == 0);
	require(ai_strlen("hello") == 5);
	require(ai_strlen("pugnacious") == 10);
}
// end 1) strlen

// 2) strncmp
//EXCEEDING LINE LIMIT BY 1 LINE
int iterate_stringncmp(const char* str1, const char* str, size_t n)
{
	int i = 0;
	for (*str; i < n; i++)
	{
	    if (*str1 != *str)
		{
			return (*str1 - *str);
		}
		str1++, str++;
	}
	return 0;
}

int ai_strncmp(const char* str1, const char* str, size_t n)
{
	if (ai_strlen(str1) >= n && ai_strlen(str) >= n)
	{
		return iterate_stringncmp(str1,str,n);
	}
	else if (ai_strlen(str1) >= n)
	{
		return 1;
	}
	else 
	{
		return -1;
	}
}

void test_strncmp()
{
	char *r = "hello", *s = "hellcat", *t = "he";
	require(ai_strncmp(r,s,3) == 0);
	require(ai_strncmp(r,s,ai_strlen(s)) < 0);
	require(ai_strncmp(t,r,4) < 0);
	require(ai_strncmp(s,t,ai_strlen(s)) > 0);
}
// end 2) strncmp

// 3) strcmp
int ai_strcmp(const char* str1, const char* str)
{
	if (ai_strlen(str1) - ai_strlen(str) > 0 || ai_strlen(str1) - ai_strlen(str) == 0)
	{
		return ai_strncmp(str1,str,ai_strlen(str1));
	}
	else
	{
		return ai_strncmp(str1,str,ai_strlen(str));
	}
}

void test_strcmp()
{
	char *r = "hello", *s = "hello", *t = "he";
	require(ai_strcmp(r,s) == 0 );
	require(ai_strcmp(t,r) < 0);
	require(ai_strcmp(s,t) > 0);
}
// end 3) strcmp


// 4) strncpy

void ai_strncpy_addnull(int counter, char* dest,int n)
{
	for (;counter < n; ++counter)
	{
		dest[counter] = '\0';
	}
}

char* ai_strncpy(char* dest, const char* src, int n)
{
	int i;
	for ( i = 0; i < n; ++i)
	{
		dest[i] = src[i];
	}
	ai_strncpy_addnull(i,dest,n);
	return dest;
}

void test_strncpy()
{
	char r[64] = "hello", *s = "world";
	require(ai_strcmp(ai_strncpy(r,s,5),s) == 0);
	char charr[64] = "hello";
	require(ai_strcmp(ai_strncpy(charr,s,3),"worlo") == 0);
}
// end 4) strncpy

// 5) strcpy
char* ai_strcpy(char* dest, const char* src)
{
	return ai_strncpy(dest,src,ai_strlen(src));
}

void test_strcpy()
{
	char r[64] = "hello", *s = "world";
	require(ai_strcmp(ai_strcpy(r,s),s) == 0);
	
}
// end 5) strcpy

// 6) strncat
char* ai_strncat(char * dest, const char* src, size_t n)
{
	int j = ai_strlen(dest);
	for (int i = 0; i < n; ++i)
	{
		dest[j++] = src[i];
	}
	return dest;
}

void test_strncat()
{
	char r[64] = "hello", *s = "world";
	require(ai_strcmp(ai_strncat(r,s,5),"helloworld") == 0);
}
// end 6) strncat

// 7) strcat
char* ai_strcat(char* dest, const char* src)
{
	return ai_strncat(dest,src,ai_strlen(src));
}

void test_strcat()
{
	char r[64] = "hello", *s = "world";
	require(ai_strcmp(ai_strcat(r,s),"helloworld") == 0);
}
// end 7) strcat

// 8) strchr
char* ai_strchr(const char* str, int c)
{
	while (*str)
	{
		if (*str == c)
		{
			return (char *)str;
		}
		str++;
	}
	return NULL;
}

void test_strchr()
{
	char r[64] = "Hello";
	int c = 'c', h = 'H';
	require(ai_strchr(r,c) == NULL);
	require(*ai_strchr(r,h) == 'H');
}
// end 8) strchr

// 9) strcspn
size_t ai_strcspn(const char* str1, const char* str2)
{
	for (int i = 0; i < ai_strlen(str1); ++i)
	{
		for (int j = 0; j < ai_strlen(str2); ++j)
		{
			if (ai_strchr(str2,str1[i]) != NULL)
			{
				return i;
			}
		}
	}
	return ai_strlen(str1);	
}

void test_strcspn()
{
	char r[64] = "hello", *s = "hellcat";
	require (ai_strcspn(r,s) == 0);
}
// end 9) strcspn

// 10) strpbrk
char* ai_strpbrk(const char* str1, const char* str2)
{
	for (int i = 0; i < ai_strlen(str1); ++i)
	{
		for (int j = 0; j < ai_strlen(str2); ++j)
		{
			if (str1[i] == str2[j])
			{
				return (char *)(str1+i);
			}
		}
	}
	return NULL;
}

void test_strpbrk()
{
	char r[64] = "abscde2fghi3jk4l";
	char* s = "34";
	char* t = "z";
	require(*ai_strpbrk(r,s) == '3');
	require(ai_strpbrk(r,t) == NULL);	
}
// end 10) strpbrk

// 11) strspn
size_t ai_strspn(const char* str1, const char* str2)
{
	for (int i = 0; i < ai_strlen(str1); ++i)
	{
		for (int j = 0; j < ai_strlen(str2); ++j)
		{
			if (ai_strchr(str2,str1[i]) == NULL)
			{
				return i;
			}
		}		
	}
	return ai_strlen(str1);
}

void test_strspn()
{
	char r[64] = "hello", *s = "hellcat";
	require(ai_strspn(r,s) == 4);
}
// end 11) strspn

// 12) strrchr
// EXCEEDING 5 LINE LIMIT BY 1
char* ai_strrchr(const char* str, int c)
{
	str = str+(ai_strlen(str)-1);
	while (*str)
	{
		if (*str == c)
		{
			return (char *)str;
		}
		str--;
	}
	return NULL;
}

void test_strrchr()
{
	char r[64] = "halal";
	int l = 'l', c = 'c';
	require(*ai_strrchr(r,l) == 'l');
	require(ai_strrchr(r,c) == NULL);
}
// end 12) strrchar

// 13) strstr
char* ai_strstr(const char* haystack, const char* needle)
{
	for (int i = 0; i <= (ai_strlen(haystack)-ai_strlen(needle)); ++i)
	{
		if (ai_strspn((haystack+i),needle) == ai_strlen(needle))
		{
			if (ai_strncmp(haystack+i,needle,ai_strlen(needle)) == 0)
			{
				return (char *)(haystack+i);
			}
		}
	}
	return NULL;
}

void test_strstr()
{
	char *r = "HollowPoint", *s = "Point", *t = "Hello";
	require(ai_strcmp(ai_strstr(r,s),"Point") == 0);
	require(ai_strstr(r,t) == NULL);
}
// end 13) strstr

// 14) strtok
int is_delim(char* str, const char* delim)
{
	if (ai_strspn(str,delim) == ai_strlen(delim))
	{
		return 1;
	}
	return 0;
}

//EXCEEDING LINE LIMIT BY 2 LINES
char* ai_strtok_loop(char* str, const char* delim)
{
	while (*strtok_string++)
	{
		if (is_delim(strtok_string,delim))
		{
			*strtok_string = '\0';
			strtok_string += ai_strlen(delim);
			return str;
		}
		else if (*(strtok_string+1) == '\0')
		{
			return str;
		}
	}
}

char* ai_strtok(char* str, const char* delim)
{
	if (str != NULL)
	{
		strtok_string = str;
	}
	str = strtok_string;

	return ai_strtok_loop(str,delim);
	
	return NULL;
}

void test_strtok()
{
	char str[64] = "This is / youtube.com / Tim Westwood TV";
	char* delim = " / ";
	char tok[64];

	ai_strcpy(tok,ai_strtok(str,delim));
	require(ai_strcmp(tok,"This is") == 0);

	ai_strcpy(tok,ai_strtok(NULL,delim));
	require(ai_strcmp(tok,"youtube.com") == 0);

	ai_strcpy(tok,ai_strtok(NULL,delim));
	require(ai_strcmp(tok,"Tim Westwood TV") == 0);	
	 
}
// end 14) strtok

int main()
{
	test_strlen();
	
	test_strncmp();
	test_strcmp();
	
	test_strncpy();
	test_strcpy();
	
	test_strncat();
	test_strcat();
	
	test_strrchr();
	test_strchr();
	
	test_strcspn();
	test_strpbrk();
	test_strspn();

	test_strstr();

	test_strtok();
	
	return 0;
}
