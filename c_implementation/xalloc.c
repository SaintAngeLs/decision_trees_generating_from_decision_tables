#include "xalloc.h"
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

/*#define MINIML_ALLOC_DEBUG*/

int enable_forking_allocation = 1;


static int is_mock = 0;
static size_t mock_line = 0;
static char* mock_file = NULL;


void* xmalloc(size_t memsz, size_t line, char* file) {

#ifdef MINIML_ALLOC_DEBUG

    if (is_mock) {
        fprintf(stderr, "Freeing process failed: attempted to allocate "
            "(failed line: %lu, failed file: %s, attempt line: %lu, attempt file: %s)\n", mock_line, mock_file, line, file);
        exit(EXIT_FAILURE);
    }

    if (enable_forking_allocation) {
        pid_t childpid = fork();
        if (childpid == 0) {
            /* parent */
            waitpid(childpid, NULL, 0);
            struct timespec tk = {0, 1000000};
            nanosleep(&tk, NULL);
            return malloc(memsz);
        } else if (childpid < 0) {
            /* error */
            fprintf(stderr, "Fork failed\n");
            return NULL;
        }

        /* child: mock allocation failure */
        is_mock = 1;
        mock_file = file;
        mock_line = line;
        fprintf(stderr, "Mock mem failure fork (file: %s, line: %lu)\n", file, line);
        return NULL;
    }

#endif

    return malloc(memsz);
}

void* xcalloc(size_t nmemb, size_t sz, size_t line, char* file) {

#ifdef MINIML_ALLOC_DEBUG

    if (is_mock) {
        fprintf(stderr, "Freeing process failed: attempted to allocate "
            "(failed line: %lu, failed file: %s, attempt line: %lu, attempt file: %s)\n", mock_line, mock_file, line, file);
        exit(EXIT_FAILURE);
    }

    if (enable_forking_allocation) {
        pid_t childpid = fork();
        if (childpid == 0) {
            /* parent */
            waitpid(childpid, NULL, 0);
            struct timespec tk = {0, 1000000};
            nanosleep(&tk, NULL);
            return calloc(nmemb, sz);
        } else if (childpid < 0) {
            /* error */
            fprintf(stderr, "Fork failed\n");
            return NULL;
        }

        /* child: mock allocation failure */
        is_mock = 1;
        mock_file = file;
        mock_line = line;
        fprintf(stderr, "Mock mem failure fork (file: %s, line: %lu)\n", file, line);
        return NULL;
    }

#endif

    return calloc(nmemb, sz);

}

void* xrealloc(void* mem, size_t memsz, size_t line, char* file) {

#ifdef MINIML_ALLOC_DEBUG

    if (is_mock) {
        fprintf(stderr, "Freeing process failed: attempted to allocate "
            "(failed line: %lu, failed file: %s, attempt line: %lu, attempt file: %s)\n", mock_line, mock_file, line, file);
        exit(EXIT_FAILURE);
    }

    if (enable_forking_allocation) {
        pid_t childpid = fork();
        if (childpid == 0) {
            /* parent */
            waitpid(childpid, NULL, 0);
            struct timespec tk = {0, 1000000};
            nanosleep(&tk, NULL);
            return realloc(mem, memsz);
        } else if (childpid < 0) {
            /* error */
            fprintf(stderr, "Fork failed\n");
            return NULL;
        }

        /* child: mock allocation failure */
        is_mock = 1;
        mock_file = file;
        mock_line = line;
        fprintf(stderr, "Mock mem failure fork (file: %s, line: %lu)\n", file, line);
        return NULL;
    }

#endif

    return realloc(mem, memsz);

}