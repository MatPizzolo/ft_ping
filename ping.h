#ifndef PING_H
# define PING_H

#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>     // For sockaddr_in and inet_pton
#include <unistd.h>        // For close()
#include <errno.h>         // For errno
#include <stdlib.h>
#include <sys/time.h>      // For gettimeofday()
#include <netdb.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>
#include <ctype.h>

#define ICMP_HDR_SIZE sizeof(t_icmphdr)
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
    int     verbose;
    int     p_transmitted;
}   t_infop;

typedef struct {
    uint8_t  type;       // ICMP message type (8 for Echo Request)
    uint8_t  code;       // Sub-type code (0 for Echo Request)
    uint16_t checksum;   // Packet checksum
    union {
        struct {
            uint16_t id;        // Process ID to match requests/replies
            uint16_t sequence;  // Sequence number of this request
        } echo;
        uint32_t gateway;        // For gateway-related messages
        struct {
            uint16_t unused;
            uint16_t mtu;        // Path MTU for destination unreachable
        } frag;
    } un;
} t_icmphdr;


/* Args Checker */
int		check_if_ip_address(char *ipAddress);
char    *check_program_arguments(int argc, char **argv, t_infop *arg_addr);

/* Utils */
unsigned short checksum(void *b, int len);

/* Getting Address Info */
int     get_host_ip(char *addr, t_infop *arg_addr);
int     get_ip_from_hostname(char *addr, t_infop *arg_addr);

/* ICMP Handler */
int sendRequests(t_infop *arg_addr);

/* Socket Handler */
int setupSocket(t_infop *arg_addr);


#endif