#include <unistd.h>
#include <signal.h>
#include <sys/types.h>

#include "../essentials.h"

int main(void)
{
    /* suppose they all returned successfully */
    fork(); /* total number of processes: 2 */
    fork(); /* total number of processes: 4 */
    fork(); /* total number of processes: 8 */

    /* new processes were created: 7 */

    exit(EXIT_SUCCESS);
}
