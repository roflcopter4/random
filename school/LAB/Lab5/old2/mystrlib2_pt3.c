/*#include <stdio.h>*/
#include <stdlib.h>
#include "mystrlib2.h"
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
		saferFree(temp);
	} else  {
		*line = temp;
	}

	/* Return the size of the string - unusual but useful. */
	return it;
}
