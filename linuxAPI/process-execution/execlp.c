#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <stdarg.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <limits.h>
#include <sys/stat.h>

void errExit(const char *msg) 
{
    perror(msg);
    exit(EXIT_FAILURE);
}

int PATHContains(const char *filename, char *fullpath) {
    const char *PATHval;
    char *path, *dir;
    struct stat st;

    if ((PATHval = getenv("PATH")) == NULL)
        errExit("getenv");

    path = strdup(PATHval);
    if (path == NULL)
        errExit("strdup");

    dir = strtok(path, ":");
    while (dir != NULL) {
        snprintf(fullpath, PATH_MAX, "%s/%s", dir, filename);
        if (stat(fullpath, &st) == 0 && S_ISREG(st.st_mode) && 
           (st.st_mode & S_IXUSR)) 
        {
            free(path);
            return 1; 
        }
        dir = strtok(NULL, ":");
    }

    free(path);
    return 0; 
}

void _execlp(const char *pathname, const char *arg, ...) 
{
    extern char **environ;
    const char *currentArg;
    char **argv;
    int argc;
    va_list args;
    char fullpath[PATH_MAX];

    va_start(args, arg);

    argc = 1;
    currentArg = arg;
    while (currentArg != NULL) {
        argc++;
        currentArg = va_arg(args, const char *);
    }
    va_end(args);

    argv = malloc((argc + 1) * sizeof(char *));
    if (argv == NULL)
        errExit("malloc");

    argv[0] = (char *) pathname;
    va_start(args, arg);
    currentArg = arg;
    for (int i = 1; i < argc; i++) {
        argv[i] = (char *) currentArg;
        currentArg = va_arg(args, const char *);
    }
    argv[argc] = (char *) NULL; 
    va_end(args);

    if (strchr(pathname, '/') != NULL) {
        execve(pathname, argv, environ);
    } else {
        if (PATHContains(pathname, fullpath)) {
            execve(fullpath, argv, environ);
        } else {
            fprintf(stderr, "%s: command not found\n", pathname);
            free(argv);
            exit(EXIT_FAILURE);
        }
    }

    free(argv);
    errExit("execve");
}

int main() {
    _execlp("ls", NULL);

    perror("_execlp");
    exit(EXIT_FAILURE);
}
