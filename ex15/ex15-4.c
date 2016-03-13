#include <stdio.h>

int main(int argc, char *argv[])
{
	int ages[] = {23, 43, 12, 89, 2};
	char *names[] = {
		"Alan", "Frank",
		"Mary", "John", "Lisa"
	};

	// safely get the size of ages
	int count = sizeof(ages) / sizeof(int);
	int i = 0;

	// first way using indexing
	for(i = 0; i < count; i++)
	{
		printf("%s has %d years alive.\n", 
			names[i], ages[i]);
	}

	printf("---\n");

	// setup the pointers to the start of the arrays
	int *cur_age = ages;
	char **cur_name = names;

	// second way usng pointers
	for(i = 0; i < count; i++)
	{
		printf("%s is %d years old.\n",
			cur_name[i], cur_age[i]);
	}

	printf("---\n");

	// third way, pointers are just arrays
	for(i = 0; i < count; i++)
	{
		printf("%s is %d yeras old again.\n",
			cur_name[i], cur_age[i]);
	}

	printf("---\n");
	char* name = NULL;
	int age = 0;
	// fourth way with pointers in a stupid complex way
	for(cur_name = names, cur_age = ages, name = *cur_name, age = *cur_age;
		(cur_age - ages) < count;
		cur_name++, cur_age++, name = cur_name[0], age = cur_age[0])
	{
		printf("%s lived %d years so far.\n",
			name, age);
	}

	return 0;
}
