#ifndef PING_H
# define PING_H

# include <stdio.h>
# include <string.h>
# include <stdlib.h>
# include <netdb.h>
# include <sys/socket.h>

typedef struct infop
{
    char    *hostname;
    char    *ip;
    int     error;
    char    *error_msg;
}   t_infop;

/* Args Checker */
int		check_if_ip_address(char *ipAddress);
char	*check_program_arguments(int argc, char **argv);

/* Utils */



/* Getting Address Info */
int    get_host_ip(char *addr, t_infop *arg_addr);
void    get_ip_from_hostname(char *addr, t_infop *arg_addr);



#endif