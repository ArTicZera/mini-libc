#ifndef _STDLIB_H
#define _STDLIB_H

#include <stddef.h>

/* Memory Calls */
void *malloc(size_t nbytes);
void free(void *ap);
void *calloc(size_t nmemb, size_t size);
void *realloc(void *ptr, size_t size);

int abs(int x);

int atoi(const char *s);
double atof(const char *s);

#endif
