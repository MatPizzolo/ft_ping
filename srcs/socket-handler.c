#include "../ping.h"

int setupSocket(t_infop *arg_addr) {
    arg_addr->sockfd = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (arg_addr->sockfd < 0) {
        arg_addr->error_msg = "Error opening socket";
        arg_addr->error = 3;
        return -1;
    }

    memset(&arg_addr->serverAddr, 0, sizeof(arg_addr->serverAddr));
    arg_addr->serverAddr.sin_family = AF_INET;
    
    arg_addr->serverAddr.sin_port = htons(0);

    if (inet_pton(AF_INET, arg_addr->ip, &(arg_addr->serverAddr.sin_addr)) <= 0) {
        arg_addr->error_msg = "Error converting address";
        arg_addr->error = 3;
        close(arg_addr->sockfd);
        return -1;
    }

    return 1;
}


