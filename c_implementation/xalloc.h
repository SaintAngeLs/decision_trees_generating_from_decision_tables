#ifndef MINIML_XALLOC_H
#define MINIML_XALLOC_H
#include <stddef.h>

extern int enable_forking_allocation;

void *xmalloc(size_t memsz, size_t line, char *file);
void *xcalloc(size_t nmemb, size_t sz, size_t line, char *file);
void *xrealloc(void *mem, size_t memsz, size_t line, char *file);

#endif