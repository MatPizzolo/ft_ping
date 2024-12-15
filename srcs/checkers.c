#include "../ping.h"

int check_if_ip_address(char *ipAddress) {
    char    copy[15];
    char    *ptr;
    int     num, dots = 0;

    strncpy(copy, ipAddress, sizeof(copy));
    ptr = strtok(copy, ".");
    
    while (ptr != NULL) {
        num = atoi(ptr);
        if (num < 0 || num > 255)
            return 0;
        
        ptr = strtok(NULL, ".");
        if (ptr != NULL)
            dots++;
    }

    return (dots == 3);
}

char *check_program_arguments(int argc, char **argv, t_infop *arg_addr) {
    int i;

    arg_addr->hostname = NULL;
    arg_addr->ip = NULL;
    arg_addr->error = 0;
    arg_addr->error_msg = NULL;
    arg_addr->verbose = 0; // Default: verbose mode off

    if (argc < 2) {
        return "ping: usage error: Destination address required";
    }

    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-v") == 0) {
            arg_addr->verbose = 1;
        } else if (strcmp(argv[i], "-?") == 0) {
            return "Usage: ping [options] <destination>\n"
                   "Options:\n"
                   "  -v       Verbose output\n"
                   "  -?       Show help and exit";
        } else if (argv[i][0] == '-') {
            return "ping: invalid option -- 'unknown'";
        } else {
            arg_addr->hostname = argv[i];
        }
    }

    if (arg_addr->hostname == NULL) {
        return "ping: usage error: Destination address required";
    }

    return NULL;
}