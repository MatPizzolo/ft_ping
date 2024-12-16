#include "../ping.h"

int get_host_ip(char *addr, t_infop *arg_addr)
{
    const char* addrs = strdup(addr);

    if (!check_if_ip_address(addr)) {
        if (!get_ip_from_hostname(addr, arg_addr)) {
            printf("ping: %s: %s\n", addrs, arg_addr->error_msg);
            free((char *)addrs);
            return 0;
        }
    } else {
        free((char *)addrs);
        arg_addr->hostname = strdup(addr);
        arg_addr->ip = strdup(addr);
    }
    return 1;
}

int get_ip_from_hostname(char *addr, t_infop *arg_addr) {
    struct addrinfo hints;
    struct addrinfo *res, *tmp;
    char host[256];

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;        // IPv4
    hints.ai_socktype = SOCK_RAW;     // Raw socket
    hints.ai_protocol = IPPROTO_ICMP; // ICMP protocol
    int ret = getaddrinfo(addr, NULL, &hints, &res);
    if (ret != 0)
    {
        arg_addr->error = 1;
        arg_addr->error_msg = strdup((char *)gai_strerror(ret));
        return 0;
    }

    for (tmp = res; tmp != NULL; tmp = tmp->ai_next)
    {
        getnameinfo(tmp->ai_addr, tmp->ai_addrlen, host, sizeof(host), NULL, 0,
                    NI_NUMERICHOST);
        arg_addr->ip = strdup(host);
    }

    arg_addr->hostname = addr;
    freeaddrinfo(res);
    return 1;
}