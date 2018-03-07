#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <errno.h>
#include "mystrlib2.h"

#define CHARSIZE sizeof(char)
#define BUFSIZE 256


void noMem()
{
	puts("Failed to allocate enough memory, exiting...");
	exit(EXIT_FAILURE);
}


/* If string not null, free the memory first, then allocate just enough memory
 * memory to hold the whole string and copy the string to the new buffer. */
void my_strcpy(String *strto, char *strfrom)
{
	size_t len;
	len = strlen(strfrom) + 1;

	if (strto->str != NULL)
		free(strto->str);

	my_xmalloc(strto->str, len);
	memcpy(strto->str, strfrom, len);
	strto->len = len;
}


/* Concatenate two string literals to a String opject in addition to assigning
 * their total length to it. */
void my_strcat(String *strto, char *str1, char *str2)
{
	size_t len1, len2;
	len1 = strlen(str1);
	len2 = strlen(str2) + 1;
	strto->len = len1 + len2;

	if (strto->str != NULL)
		free(strto->str);

	my_xmalloc(strto->str, strto->len);
	memcpy(strto->str, str1, len1);
	memcpy(&strto->str[len1], str2, len2);
}


void my_strapp(String *str_orig, char *str_to_append)
{
	if (str_orig->str == NULL) {
		my_strcpy(str_orig, str_to_append);
	} else {
		size_t len2 = strlen(str_to_append);
		if (!(str_orig->str = realloc(str_orig->str, str_orig->len + len2)))
			noMem();
		memcpy(&str_orig->str[str_orig->len-1], str_to_append, len2+1);
		str_orig->len += len2;
	}
}


/* ========================================================================= */
/* ========================================================================= */


void my_strtcpy(String *strto, String *strfrom)
{
	if (strto->str != NULL)
		free(strto->str);
	if (strfrom->str == NULL)
		exit(EXIT_FAILURE);

	my_xmalloc(strto->str, strfrom->len);
	memcpy(strto->str, strfrom->str, strfrom->len);
	strto->len = strfrom->len;
}


void my_strtcat(String *strto, String *str1, String *str2)
{
	strto->len = str1->len + str2->len - 1;

	if (strto->str != NULL)
		free(strto->str);
	if (str1->str == NULL || str2->str == NULL)
		exit(EXIT_FAILURE);

	my_xmalloc(strto->str, strto->len);
	memcpy(strto->str, str1->str, str1->len);
	memcpy(&strto->str[str1->len-1], str2->str, str2->len);
}


void my_strtapp(String *str_orig, String *str_to_append)
{
	if (str_orig->str == NULL) {
		my_strtcpy(str_orig, str_to_append);
	} else {
		if (!(str_orig->str = realloc(str_orig->str, (str_orig->len + str_to_append->len)-1)))
			noMem();
		memcpy(&str_orig->str[str_orig->len-1], str_to_append->str, str_to_append->len);
		str_orig->len += str_to_append->len - 1;
	}
}


/* ========================================================================= */
/* ========================================================================= */


void SA_init(StringArray *str_array)
{
	int iter;

	if (str_array->arr == NULL) {
		my_xcalloc(str_array->arr, str_array->num, sizeof(String));
	} else {
		puts("StrArray not null, refusing to allocate. Exiting...");
		exit(EXIT_FAILURE);
	}

	/* The Array must be very carefully dereferenced or things will implode. */
	for (iter = 0; iter < str_array->num; ++iter)
		str_array->arr[iter].str = NULL;
}


void SA_free(StringArray *str_array)
{
	int i;

	for (i = 0; i < str_array->num; ++i) {
		if (str_array->arr[i].str != NULL) {
			free(str_array->arr[i].str);
			str_array->arr[i].str = NULL;
		}
	}
	free(str_array->arr);
	str_array->arr = NULL;
}


StringArray SA_append(StringArray *str_array, char *str)
{
	String tmp = {NULL, 0};
        my_strcpy(&tmp, str);
	return SA_append_string(str_array, tmp);
}


StringArray
SA_append_string(StringArray *str_array, String str)
{
	int i;
	StringArray new_array = {NULL, str_array->num + 1};

	my_xcalloc(new_array.arr, new_array.num, sizeof(String));
	for (i = 0; i < str_array->num; ++i) {
		new_array.arr[i] = str_array->arr[i];
	}
	new_array.arr[new_array.num-1] = str;
	free(str_array->arr);
	return new_array;
}


/* ========================================================================= */
/* ========================================================================= */


void get_stdin(StringArray *str_array)
{
	int x;
	String buf = {NULL, 0};
	
	str_array->num = 0;
	while ((x = my_getline(&buf.str)) != EOF) {
		if (x < 0) {
			noMem();
		} else {
			++x;
			buf.len = (size_t)x;
			*str_array = SA_append_string(str_array, buf);
		}
	}
}


int get_all_lines(StringArray *str_array, char *filename)
{
	int ch, it;
	FILE *fp;

	fp = fopen(filename, "r");
	if (errno) {
		char tmp[BUFSIZE];
		sprintf(tmp, "Error opening file '%s'", filename);
		perror(tmp);
		errno = 0;
		return -1;
	}

	str_array->num = 0;
	while ((ch = getc(fp)) != EOF) {
		if (ch == '\n')
			++str_array->num;
	}

	fclose(fp);
	fp = fopen(filename, "r");
	SA_init(str_array);

	for (it = 0; it < str_array->num; ++it) {
		int x;
		if ((x = my_fgetline(&(str_array->arr[it].str), fp)) < 0) {
			noMem();
		} else {
			++x;
			str_array->arr[it].len = (size_t)(x);
		}
	}

	fclose(fp);
	return 0;
}


/* ========================================================================= */
/* ========================================================================= */


#define STARTSIZE 128
#define INCSIZE 16

int my_fgetline(char **line, FILE *f)
{
	int ch, it, size;
	char *buf, *temp;
	buf = temp = NULL;

	if ((buf = malloc(STARTSIZE)) == NULL)
		return -100;
	size = STARTSIZE;

	it = 0;
	while ((ch = getc(f)) != EOF && ch != '\n') {
		/* If the buffer is full, extend it with realloc */
		if (it >= (size - 1)) {
			size += INCSIZE;
			/* If the operation fails, return the partial line and an error */
			if ((temp = realloc(buf, size)) == NULL) {
				buf[it] = '\0';
				*line = buf;
				return -200;
			}
			buf = temp;
		}
		buf[it++] = ch;
	}

	/* If the line was entirely empty clear buf and return EOF as an error */
	if (ch == EOF && it == 0) {
		free(buf);
		return EOF;
	}

	/* Make certain the string has its null terminator. */
	buf[it] = '\0';

	/* Attempt to reduce the size of the string to the bare minimum. if that
	 * fails then just give up and return the oversized string. */
	if ((temp = realloc(buf, it + 1)) == NULL) {
		*line = buf;
		my_free(temp);
	} else  {
		*line = temp;
	}

	/* Return the size of the string - unusual but useful. */
	return it;
}

/* vim: nolist
 */
