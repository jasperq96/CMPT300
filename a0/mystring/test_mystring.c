#include "mystring.h"
#include <assert.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * The following are simple tests to help you.
 * You should develop more extensive tests to try and find bugs.
 */
int main()
{
	const int SIZE = 100;
	char buffer[SIZE];

	assert(mystrlen("cmpt 300") == 8);
	assert(mystrlen("") == 0);
	assert(mystrlen(" ") == 1);

	mystrcpy(buffer, "I am testing my string functions!");
	assert(strcmp(buffer, "I am testing my string functions!") == 0);
	assert(strcmp(buffer, "I Am testing my string functions!") == 32);

	char *dupStr = mystrdup(buffer);
	assert (!strcmp(buffer, dupStr));
	assert (buffer != dupStr);

	mystrcpy(buffer, "");
	assert(strcmp(buffer, "") == 0);
	dupStr = mystrdup(buffer);
	assert (!strcmp(buffer, dupStr));
	assert (buffer != dupStr);

	mystrcpy(buffer, " ");
	assert(strcmp(buffer, " ") == 0);
	dupStr = mystrdup(buffer);
	assert (!strcmp(buffer, dupStr));
	assert (buffer != dupStr);

	assert(mystrcmp ("I love coding", "I love coding") == 0);
	assert(mystrcmp ("I love coding", "i love coding") == -1);
	assert(mystrcmp ("i love coding", "I love coding") == 1);
	assert(mystrcmp ("I love code", "I love coding") == -1);
	assert(mystrcmp ("I love coding", "I love code") == 1);
	assert(mystrcmp ("", "") == 0);
	assert(mystrcmp ("", " ") == -1);

	if (dupStr != NULL)
		free (dupStr);

	printf ("\nPassed ALL tests!\n");

	return 0;
}

