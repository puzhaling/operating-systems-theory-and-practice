#include <syslog.h>
#include <errno.h>
#include "../essentials.h"

int 
main(int argc, char **argv) {
        if (argc == 1)        
                cmdLineErr("not enough arguments");        
        int level;
        Boolean levelProvided;
        
        if (argc == 2) {
                level = LOG_INFO;
                levelProvided = FALSE;
        } else {
                errno = 0;
                level = atoi(argv[2]);
                if (level == 0 && errno != 0)
                        usageErr("level is NaN");
        }
        
        openlog(argv[0], LOG_PID, LOG_USER);
        syslog(level, "%s", argv[1]);
        closelog();

        exit(EXIT_SUCCESS);
}
