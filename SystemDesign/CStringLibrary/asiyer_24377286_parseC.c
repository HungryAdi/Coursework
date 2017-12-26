#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_STRING 1024
//TODO: Deal with operators (++, -- , *=, /=, ==, %0 , <=, >=)
//TODO: Deal with comment case
//TODO: Change function line lengths

int peekc(char peek[], int index)
{
	return peek[index+1];	
}

/*EXCEEDING LINE LIMIT BY 2 LINES*/
int default_parse(char to_parse[], int index)
{
	int i = 0;
	for (i = index; i < strlen(to_parse); i++)
	{
		if (isalnum(to_parse[i]) == 0)
		{
			printf("\n");
			return i;
		}
		else 
		{
			printf("%c",to_parse[i]);
		}
	}
	return i;
}

int parse_operator_equals(char to_parse[], int index)
{
	printf("%c",to_parse[index]);
	printf("%c\n",to_parse[index+1]);
	return index+1;
}


int parse_comment(char to_parse[], int index)
{
	char word[MAX_STRING];
	int counter = 0;
	int i = index;
	if (peekc(to_parse,index) == '=')
	{
		return parse_operator_equals(to_parse,index);
	}
	else
	{
		while (peekc(to_parse,i) != '\n' && peekc(to_parse,i) != '/')
		{
			word[counter++] = to_parse[i++];
		}
		word[counter] = to_parse[i];
		return i;
	}
}

int parse_string(char string[], int index)
{
	for (int i = index+1; i < strlen(string); ++i)
	{
		if (string[i] == '"' && (peekc(string,i) == ',' || peekc(string,i) == ')'))
		{
				printf("%c\n",string[i]);
				return i;	
		}
		else
		{
			printf("%c",string[i]);
		}
	}
}

int parse_char(char character[], int index)
{
	for (int i = index+1; i < strlen(character); ++i)
	{
		if (character[i] == '\'')
		{
			printf("%c\n",character[i]);
			return i;
		}
		else
		{
			printf("%c",character[i]);
		}
	}
}

void parse(char to_parse[])
{
	for (int i = 0; i < strlen(to_parse); ++i)
	{
		switch (to_parse[i])
		{
			case '\t':
				break;
			case '"':
				printf("%c",to_parse[i]);
				i = parse_string(to_parse,i);
				break;
			case '\'':
				printf("%c",to_parse[i]);
				i = parse_char(to_parse,i);
				break;
			case ' ':
				break;
			case '\n':
				break;
			case '(':
			case ')':
			case '{':
			case '[':
			case ']':
			case '}':
			case ';':
			case ':':
			case '.':
			case ',':
			case '#':
				printf("%c\n",to_parse[i]);
				break;
			case '/':
				if (peekc(to_parse,i) == '/' || peekc(to_parse,i) == '*' || peekc(to_parse,i) == '=')
				{
					i = parse_comment(to_parse,i);
					break;
				}
				else
				{
					printf("%c\n",to_parse[i]);
					break;
				}
			case '<':
				if (peekc(to_parse,i) == '=')
				{
					i = parse_operator_equals(to_parse,i);
					break;
				}
				else
				{
					printf("%c\n",to_parse[i]);
					break;
				}
			case '>':
				if (peekc(to_parse,i) == '=')
				{
					i = parse_operator_equals(to_parse,i);
					break;
				}
				else
				{
					printf("%c\n",to_parse[i]);
					break;
				}
			case '+':
				if (peekc(to_parse,i) == '+' || peekc(to_parse,i) == '=')
				{
					i = parse_operator_equals(to_parse,i);
					break;
				}
				else
				{
					printf("%c\n",to_parse[i]);
					break;
				}
			case '-':
				if (peekc(to_parse,i) == '-' || peekc(to_parse,i) == '=')
				{
					i = parse_operator_equals(to_parse,i);
					break;
				}
				else
				{
					printf("%c\n",to_parse[i]);
					break;
				}
			case '*':
				if (peekc(to_parse,i) == '=')
				{
					i = parse_operator_equals(to_parse,i);
					break;
				}
				else
				{
					printf("%c\n",to_parse[i]);
					break;
				}
			case '%':
				if (peekc(to_parse,i) == '=')
				{
					i = parse_operator_equals(to_parse,i);
					break;
				}
				else
				{
					printf("%c\n",to_parse[i]);
					break;
				}
			case '=':
				if (peekc(to_parse,i) == '=')
				{
					i = parse_operator_equals(to_parse,i);
					break;
				}
				else
				{
					printf("%c\n",to_parse[i]);
					break;
				}
			default:
				i = default_parse(to_parse,i);
				printf("%c\n",to_parse[i]);
				break;
					
		}
	}
}

int main()
{
	char code[MAX_STRING];
	while (fgets(code,MAX_STRING,stdin))
	{
		parse(code);
	}

	return 0;
}




