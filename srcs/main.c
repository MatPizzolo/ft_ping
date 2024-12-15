#include "../ping.h"

int setupSocket(t_infop *arg_addr) {
    arg_addr->sockfd = socket(AF_INET, SOCK_STREAM, 0);
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

// int connectSocket(t_infop* arg_addr) {
//     if (connect(arg_addr->sockfd, (struct sockaddr*)&arg_addr->serverAddr, sizeof(arg_addr->serverAddr)) < 0) {
//         arg_addr->error_msg = "Error connecting to the server";
//         arg_addr->error = 4;
//         close(arg_addr->sockfd);
//         return 0;
//     }
//     return 1;
// }

unsigned short checksum(void *b, int len) {
    unsigned short *buf = b;
    unsigned int sum = 0;
    unsigned short result;

    for (sum = 0; len > 1; len -= 2) {
        sum += *buf++;
    }

    if (len == 1) {
        sum += *(unsigned char *)buf;
    }

    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;

    return result;
}

void handleInterrupt(int signal) {
    printf("\nPing interrupted.\n");
    exit(signal);
}

int sendRequests(t_infop *arg_addr) {
    int i;
    struct icmphdr icmpHeader;
    struct timeval start, end;
    double rtt;

    signal(SIGINT, handleInterrupt);

    for (i = 0; i < 4; i++) {
        // Prepare ICMP Echo Request packet
        memset(&icmpHeader, 0, sizeof(struct icmphdr));
        icmpHeader.type = ICMP_ECHO;
        icmpHeader.code = 0;
        icmpHeader.checksum = 0;
        icmpHeader.un.echo.id = getpid();
        icmpHeader.un.echo.sequence = i + 1;

        gettimeofday(&start, NULL);

        // Calculate ICMP checksum
        icmpHeader.checksum = checksum(&icmpHeader, sizeof(struct icmphdr));

        // Send ICMP Echo Request
        if (sendto(arg_addr->sockfd, &icmpHeader, sizeof(struct icmphdr), 0,
                   (struct sockaddr *)&arg_addr->serverAddr, sizeof(arg_addr->serverAddr)) <= 0) {
            perror("Error sending request");
            return 0;
        }

        // Receive ICMP Echo Reply
        // Implement this part to receive and process the reply
        // You may need to use recvmsg and handle ICMP replies

        gettimeofday(&end, NULL);

        // Calculate and print Round-Trip Time (RTT)
        rtt = (double)(end.tv_sec - start.tv_sec) * 1000.0 + (double)(end.tv_usec - start.tv_usec) / 1000.0;
        printf("64 bytes from %s (%s): icmp_seq=%d ttl=%d time=%.2f ms\n",
               arg_addr->hostname, arg_addr->ip, i + 1, 64, rtt);

        usleep(1000000); // Sleep for 1 second between requests
    }

    return 1;
}

void printHostStatistics(t_infop arg_addr) {

    printf("Host statistics here.\n%s", arg_addr.error_msg);
}

int main(int argc, char *argv[]) {
    t_infop arg_addr;

    const char *msg = check_program_arguments(argc, argv);
    if (msg != NULL)
        return (printf("%s\n", msg), 1);

    if (!get_host_ip(argv[1], &arg_addr))
        return (2);

    // // Print First Line
    printf("PING %s (%s): xx(xx) bytes of data.\n", arg_addr.hostname, arg_addr.ip);

    if (!setupSocket(&arg_addr))
        return (printf("%s\n", arg_addr.error_msg), arg_addr.error);

    // if (!connectSocket(&arg_addr))
    //     return (printf("%s\n", arg_addr.error_msg), arg_addr.error);
    
    if (!sendRequests(&arg_addr))
        return (printf("%s\n", arg_addr.error_msg), arg_addr.error);

    // printHostStatistics(arg_addr);

    // close(arg_addr.sockfd);

}