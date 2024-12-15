#ifndef PING_H
# define PING_H

#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>     // For sockaddr_in and inet_pton
#include <netinet/ip_icmp.h>  // For ICMP headers
#include <unistd.h>        // For close()
#include <errno.h>         // For errno
#include <stdlib.h>
#include <sys/time.h>      // For gettimeofday()
#include <netdb.h>
# include <stdlib.h>
# include <sys/types.h>
# include <netinet/in.h>

#define ICMP_HDR_SIZE sizeof(struct icmphdr)
#define ICMP_BODY_SIZE 56  // Standard ping body size in bytes
# define BUFFER_SIZE 1024


typedef struct infop
{
    char    *hostname;
    char    *ip;
    int     error;
    char    *error_msg;
    struct sockaddr_in serverAddr;
    int     sockfd;
}   t_infop;

typedef struct icmp_header {
    uint8_t type;       // 8 for Echo Request
    uint8_t code;       // Always 0 for Echo Request
    uint16_t checksum;  // Error-checking value, calculated later
    uint16_t id;        // Identifier for this process
    uint16_t sequence;  // Sequence number for this request
}   t_icmph;


/* Args Checker */
int		check_if_ip_address(char *ipAddress);
char	*check_program_arguments(int argc, char **argv);

/* Utils */



/* Getting Address Info */
int     get_host_ip(char *addr, t_infop *arg_addr);
int    get_ip_from_hostname(char *addr, t_infop *arg_addr);

/* ICMP Handler */
int create_send_packet(const char *dest_ip);


#endif