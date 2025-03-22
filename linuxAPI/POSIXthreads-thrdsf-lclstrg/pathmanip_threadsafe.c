#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <limits.h>
#include <stdio.h>
#include "../essentials.h"

// Static storage for basename and dirname results
static char basename_result[256];
static char dirname_result[256];

static pthread_key_t my_basename_key;
static pthread_once_t my_basename_once = PTHREAD_ONCE_INIT;

/* 
static pthread_key_t my_dirname_key;
static pthread_once_t my_dirname_once = PTHREAD_ONCE_INIT;
*/

void destructor(void *buf)
{
        free(buf);
}

void my_basename_create_key(void)
{
        pthread_key_create(&my_basename_key, destructor);        
}

// Custom implementation of basename
const char *my_basename(const char *path) {
        int rc;
        char *buf;

        rc = pthread_once(&my_basename_once, my_basename_create_key);
        if (rc != 0)
                return NULL;

        buf = pthread_getspecific(my_basename_key);
        if (buf == NULL) {
                buf = (char *) malloc(PATH_MAX);
                if (buf == NULL)
                        return NULL;
                
                rc = pthread_setspecific(my_basename_key, buf);
                if (rc != 0)
                        return NULL;
        }

        /* FIXME: some troubles with algorithm, but
        *         main purpose is achieved - 
        *         function became threadsafe 
        */
        if (path == NULL || *path == '\0') {
                buf[0] = '.';
                buf[1] = '\0';
                return buf;
        }

        // Find the end of the string
        const char *end = path + strlen(path) - 1;

        // Remove trailing slashes
        while (end > path && *end == '/') {
                end--;
        }

        // Find the last slash
        const char *last_slash = strrchr(path, '/');

        if (last_slash == NULL) {
                // No slash found, return the entire path
                strncpy(buf, path, PATH_MAX);
                buf[PATH_MAX] = '\0';
                return buf;
        }

        // Copy the basename portion
        strncpy(buf, last_slash + 1, PATH_MAX);
        buf[PATH_MAX] = '\0';
        return buf;
}

/* FIXME: make it threadsafe (look at my_basename)
// Custom implementation of dirname
const char *my_dirname(const char *path) {
        if (path == NULL || *path == '\0') {
                dirname_result[0] = '.';
                dirname_result[1] = '\0';
                return dirname_result;
        }

        // Find the end of the string
        const char *end = path + strlen(path) - 1;

        // Remove trailing slashes
        while (end > path && *end == '/') {
            end--;
        }

        // Find the last slash
        const char *last_slash = strrchr(path, '/');

        if (last_slash == NULL) {
                // No slash found, return "."
                dirname_result[0] = '.';
                dirname_result[1] = '\0';
                return dirname_result;
        }

        // Remove any trailing slashes before the last slash
        while (last_slash > path && *last_slash == '/') {
                last_slash--;
        }

        // Copy the dirname portion
        if (last_slash == path) {
                // If the last slash is the first character, return "/"
                dirname_result[0] = '/';
                dirname_result[1] = '\0';
        } else {
                size_t len = last_slash - path;
                strncpy(dirname_result, path, len);
                dirname_result[len] = '\0'; // Ensure null-termination
        }

        return dirname_result;
}
*/

void *thread_routine(void *arg)
{
        char *processed_path = (char *) my_basename((char *) arg);

        pthread_exit(processed_path);
}

int main() {
        const char *path1 = "/usr/local/bin/aaa";
        const char *path2 = "/usr/local/bin/bbb";
        char *processed_path1, *processed_path2;
        pthread_t t1, t2;
        int rc;

        rc = pthread_create(&t1, NULL, thread_routine, (void *) path1);
        if (rc != 0)
                errExitEN(rc, "pthread_create");

        rc = pthread_create(&t2, NULL, thread_routine, (void *) path1);
        if (rc != 0)
                errExitEN(rc, "pthread_create");


        rc = pthread_join(t1, (void **) &processed_path1);
        if (rc != 0)
                errExitEN(rc, "pthread_join");
        
        rc = pthread_join(t2, (void **) &processed_path2);
        if (rc != 0)
                errExitEN(rc, "pthread_join");

        
        printf("path1: %s\n", path1);
        printf("processed_path1: %s (%p)\n", processed_path1, 
                                processed_path1);
        printf("processed_path2: %s (%p)\n", processed_path2, 
                        processed_path2);
            
        return 0;
}
