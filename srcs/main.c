#include "../ping.h"


int main(int argc, char *argv[]) {
    t_infop arg_addr;

    const char *msg = check_program_arguments(argc, argv, &arg_addr);
    if (msg != NULL)
        return (printf("%s\n", msg), 1);

    if (!get_host_ip(argv[1], &arg_addr))
        return (2);

    printf("PING %s (%s): 56 data bytes\n", arg_addr.hostname, arg_addr.ip);

    if (!setupSocket(&arg_addr))
        return (printf("%s\n", arg_addr.error_msg), arg_addr.error);
    
    if (!sendRequests(&arg_addr))
        return (printf("%s\n", arg_addr.error_msg), arg_addr.error);


    // close(arg_addr.sockfd);

}