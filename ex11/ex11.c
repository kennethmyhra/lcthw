#include <stdio.h>

int main(int argc, char* argv[])
{
	// go through each string in argv
	int i = 0;
	while(i < argc)
	{
		printf("arg %d: %s\n", i, argv[i]);
		i++;
	}

	// let's make our own array of strings
	char *states[] = {
		"California", "Oregon",
		"Washington", "Texas"
	};
	int num_states = 4;
	i = 0; // watch for this
	while(i < num_states)
	{
		printf("state: %d: %s\n", i, states[i]);
		i++;
	}

	// Original code from top down to here.
	// Under here I am copying values and printing
	// out pointer addresses to verify when they 
	// match and not match.

	int y = argc > num_states ? num_states : argc;

	// Print original pointer addresses
	printf("\n");
	printf("Original pointer addresses:\n");
	i = 0;
	while(i < y)
	{
		printf("Address of argv[%d]: %p and states[%d]: %p\n", i, argv[i], i, states[i]);
		i++;
	}

	// Copy values from argv into states
	i = 0;
	while(i < y)
	{
		states[i] = argv[i];
		i++;
	}

	// Print pointer addresses after copy operation
	printf("\n");
	printf("Pointer addresses after copy operation:\n");
	i = 0;
	while(i < y)
	{
		printf("Address of argv[%d]: %p and states[%d]: %p\n", i, argv[i], i, states[i]);
		i++;
	}

	// Change value of states at index 0
	printf("\n");	
	printf("Changing value of states[0]\n");
	states[0] = "North Carolina";
	// Print pointer addresses after changing value of states at index 0
	printf("Pointer addresses after copy operation:\n");
	i = 0;
	while(i < y)
	{
		printf("Address of argv[%d]: %p and states[%d]: %p\n", i, argv[i], i, states[i]);
		i++;
	}

	return 0;
}
