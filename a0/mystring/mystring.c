#include "mystring.h"
#include <stdio.h>
#include <stdlib.h>

/*
 *   Implement the following functions: 
 * 
 *   You are NOT allowed to use any standard string functions such as 
 *   strlen, strcpy or strcmp or any other string function.
 */

/*
 *  mystrlen() calculates the length of the string s, 
 *  not including the terminating character '\0'.
 *  Returns: length of s.
 */
int mystrlen (const char *s) 
{
	/* Complete the body of the function */
	int length = 0;
	int i = 0;
	char c = s[i];
	while(c != '\0'){
		length++;
		i++;
		c = s[i];
	}
	return length;
}

/*
 *  mystrcpy()  copies the string pointed to by src (including the terminating 
 *  character '\0') to the array pointed to by dst.
 *  Returns: a  pointer to the destination string dst.
 */
char  *mystrcpy (char *dst, const char *src)
{
	/* Complete the body of the function */
	int length = mystrlen(src);
	for(int i = 0; i < length; i++){
		dst[i] = src[i];
	}
	dst[length] = '\0';
	return dst;
}

/*
 * mystrcmp() compares two strings alphabetically
 * Returns: 
 * 	-1 if s1  < s2
 *  	0 if s1 == s2
 *  	1 if s1 > s2
 */
int mystrcmp(const char *s1, const char *s2)
{
	/* Complete the body of the function */
	int i = 0;
	while(s1[i] != '\0' || s2[i] != '\0'){
		if(s1[i] == '\0'){
			return -1;
		}
		else if(s2[i] == '\0'){
			return 1;
		}
		if(s1[i] < s2[i]){
			return -1;
		}
		else if(s1[i] > s2[i]){
			return 1;
		}
		i++;
	}
	return 0;
}

/*
 * mystrdup() creates a duplicate of the string pointed to by s1. 
 * The space for the new string is obtained using malloc.  
 * If the new string can not be created, a null pointer is returned.
 * Returns:  a pointer to a new string (the duplicate) 
 	     or null If the new string could not be created for 
	     any reason such as insufficient memory.
 */
char *mystrdup(const char *s1)
{
	/* Complete the body of the function */
	char *duplicate = malloc(sizeof(s1)*mystrlen(s1)+1);
	if(!duplicate){
		return NULL;
	}
	else{
		mystrcpy(duplicate,s1);
		return duplicate;
	}
}