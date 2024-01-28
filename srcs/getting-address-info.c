#include "../ping.h"

int get_host_ip()
if (!check_if_ip_address(argv[1])) {
        get_ip_from_hostname(argv[1], &arg_addr);
        if (arg_addr.error)
            return (printf("ping: %s: %s\n", argv[1], arg_addr.error_msg), 2);
    } else {
        arg_addr.hostname = strdup(argv[1]);
        arg_addr.ip = strdup(argv[1]);
    }

void get_ip_from_hostname(char *addr, t_infop *arg_addr) {
    struct addrinfo hints;
    struct addrinfo *res, *tmp;
    char host[256];

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;

    int ret = getaddrinfo(addr, NULL, &hints, &res);
    if (ret != 0)
    {
        arg_addr->error = 1;
        arg_addr->error_msg = (char *)gai_strerror(ret);
        return ;
    }

   // Vale la pena el for loop?
    for (tmp = res; tmp != NULL; tmp = tmp->ai_next)
    {
        getnameinfo(tmp->ai_addr, tmp->ai_addrlen, host, sizeof(host), NULL, 0,
                    NI_NUMERICHOST);
        arg_addr->ip = strdup(host);
    }

    arg_addr->hostname = addr;

    freeaddrinfo(res);
}