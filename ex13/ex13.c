#include <stdio.h>

char to_lower(char c);

char to_lower(char c)
{
	const int magic_number = 32;
	return c >= 'A' && c <= 'Z' ? c + magic_number : c;
}

int main(int argc, char* argv[])
{
	if(argc < 2)
	{
		printf("ERROR: You need at least one argument.\n");
		return 1;
	}

	int y = 0;
	int i = 0;
	char letter = NULL;
	for(y = 1; y < argc; y++)
	{
		for(i = 0; letter = to_lower(argv[y][i]), argv[y][i] != '\0'; i++)
		{
			//char letter = letter = argv[y][i];
			switch(letter)
			{
				case 'a':
				case 'e':
				case 'i':
				case 'o':
				case 'u':
					printf("%d: %c\n", i, letter);
					break;
				case 'y':
					if(i > 2)
					{
						// it's only sometimes Y
						printf("%d: %c\n", i, letter);
					}
					break;
				default:
					printf("%d: %c is not a vowel\n", i, letter);
			}
		}
	}

	return 0;
}
