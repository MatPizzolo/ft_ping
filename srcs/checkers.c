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

// check flags 
// ipAddress = argv[argc - 1]
char    *check_program_arguments(int argc, char **argv) {
    char *response;

    /*if (argc == 1)
        response = "ping: usage error: Destination address required";
    else if (!check_if_ip_address(argv[1]))
        response = "ping: IP: Name or service not known";
    else 
        response = NULL;*/

    if (argc == 1)
        response = "ping: usage error: Destination address required";
    else 
        response = NULL;

    return response;
}