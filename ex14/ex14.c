#include <stdio.h>
#include <ctype.h>
#include <string.h>

void print_letters(int len, char arg[])
{
	int i = 0;

	for(i = 0; i < len; i++)
	{
		char ch = arg[i];

		if(isalpha(ch) || isblank(ch) || ispunct(ch))
		{
			printf("'%c' == %d ", ch, ch);
		}
	}

	printf("\n");
}

int main(int argc, char* argv[])
{
	int i = 0;

	for(i = 0; i < argc; i++)
	{
		int len = strlen(argv[i]);
		print_letters(len, argv[i]);
	}

	return 0;
}
