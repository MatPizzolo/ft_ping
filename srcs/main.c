#include "../ping.h"

// -v Verbose output. Do not suppress DUP replies when pinging multicast address.



// PING 8.8.8.8 (8.8.8.8) 56(84) bytes of data.
// PING google.com (142.251.134.46) 56(84) bytes of data.
// 64 bytes from 8.8.8.8: icmp_seq=1 ttl=58 time=652 ms

int main(int argc, char *argv[]) {
    t_infop arg_addr;

    const char *msg = check_program_arguments(argc, argv);
    if (msg != NULL)
        return (printf("%s\n", msg), 1);

    if (!get_host_ip(argv[1], &arg_addr))
        return 2;
    

    // Print First Line
    printf("PING %s (%s) xx(xx) bytes of data.\n", arg_addr.hostname, arg_addr.ip);
}