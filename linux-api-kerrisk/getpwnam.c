#include <stdio.h>
#include <pwd.h>
#include <string.h>

void copy(struct passwd *dest, const struct passwd *src)
{
    dest->pw_name = strdup(src->pw_name);
    dest->pw_passwd = strdup(src->pw_passwd);
    dest->pw_uid = src->pw_uid;
    dest->pw_gid = src->pw_gid;
    dest->pw_gecos = strdup(src->pw_gecos);
    dest->pw_dir = strdup(src->pw_dir);
    dest->pw_shell = strdup(src->pw_shell);
}

struct passwd *_getpwnam(const char *name)
{
    static struct passwd retv;
    struct passwd *pw;
    int found;
    
    found = 0;
    while ((pw = getpwent()) != NULL) {
        if (strcmp(pw->pw_name, name) == 0) {
            found = 1;
            copy(&retv, pw);
        }
    }
    endpwent();

    if (found) {
        return &retv;
    } else {
        return NULL;
    }
}

int main(void)
{
    struct passwd *pw_;
    struct passwd *pw = _getpwnam("puzhaling");
    printf("%s\n", pw->pw_name);
    printf("%s\n", pw->pw_passwd);
    printf("%s\n", pw->pw_shell);
    printf("%s\n", pw->pw_dir);

    putchar('\n');

    pw_ = _getpwnam("root");
    printf("%s\n", pw->pw_name);
    printf("%s\n", pw->pw_passwd);
    printf("%s\n", pw->pw_shell);
    printf("%s\n", pw->pw_dir);

}
