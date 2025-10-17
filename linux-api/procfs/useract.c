#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <unistd.h>
#include <pwd.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>

#define STRUID_MAX_SIZE 1000

void wtchprocstatus(const char *stfilepath, const char *struid) 
{
    FILE *file = fopen(stfilepath, "r");
    if (file == NULL) {
        perror("fopen");
        return; 
    }
    char buf[1000];
    char programname[1000] = ""; 
    char uidline[1000] = "";
    pid_t pid;

    sscanf(stfilepath, "/proc/%d/status", &pid);

    while (fgets(buf, sizeof(buf), file) != NULL) {
        if (strncmp(buf, "Name:", 5) == 0) {
            strncpy(programname, buf + 6, sizeof(programname) - 1);
            programname[strcspn(programname, "\n")] = '\0';
        }

        if (strncmp(buf, "Uid:", 4) == 0) {
            strncpy(uidline, buf + 5, sizeof(uidline) - 1);
            uidline[strcspn(uidline, "\n")] = '\0';

            char *uid_str = strtok(uidline, "\t");
            if (uid_str && strcmp(uid_str, struid) == 0) {
                printf("PID: %d, Program: %s\n", pid, programname);
            }
            break; 
        }
    }

    fclose(file);
}

uid_t uid_from_name(const char *name) {
    struct passwd *pwd = getpwnam(name);
    return pwd ? pwd->pw_uid : (uid_t)-1;
}

int 
main(int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "usage: %s <username>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    DIR *dirp_ext;
    struct dirent *dirp_ext_ent;
    char path[PATH_MAX];
    char struid[STRUID_MAX_SIZE];
    uid_t target_uid = uid_from_name(argv[1]);

    if (target_uid == (uid_t)-1) {
        fprintf(stderr, "user not found\n");
        exit(EXIT_FAILURE);
    }

    snprintf(struid, STRUID_MAX_SIZE, "%ld", (long)target_uid);

    dirp_ext = opendir("/proc");
    if (dirp_ext == NULL) {
        perror("opendir");
        exit(EXIT_FAILURE);
    }

    while ((dirp_ext_ent = readdir(dirp_ext)) != NULL) {
        if (strcmp(dirp_ext_ent->d_name, ".")  == 0 || 
            strcmp(dirp_ext_ent->d_name, "..") == 0)
        {
            continue;
        }

        pid_t pid = strtol(dirp_ext_ent->d_name, NULL, 10);
        if (pid == 0) {
            continue; 
        }

        snprintf(path, sizeof(path), "/proc/%s/status", dirp_ext_ent->d_name);
        wtchprocstatus(path, struid);
    }

    closedir(dirp_ext);

    return EXIT_SUCCESS;
}
