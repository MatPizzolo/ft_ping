#include "../ping.h"

t_infop *global_arg_addr;

void print_ping_stats(int signal) {
    printf("\n--- %s ping statistics ---\n", global_arg_addr->hostname);
    printf("%d packets transmitted, %d packets received, %.1f%% packet loss\n", global_arg_addr->p_transmitted, 0, 1.1);
    printf("round-trip min/avg/max/stddev = %.3f/%.3f/%.3f/%.3f ms\n", 1.1, 1.1, 1.1, 1.1);
    exit(signal);
}


int getTTL(int sockfd) {
    int ttl;
    socklen_t len = sizeof(ttl);
    
    if (getsockopt(sockfd, IPPROTO_IP, IP_TTL, &ttl, &len) < 0) {
        perror("getsockopt");
        return -1;
    }

    return ttl;
}

int sendRequests(t_infop *arg_addr) {
    t_icmphdr icmpHeader;
    struct timeval start, end;
    double  rtt;
    int     ttl;
    char payload[56];

    signal(SIGINT, print_ping_stats);

    int i = 1;
    arg_addr->p_transmitted = 1;
    while(1) {
        // Prepare ICMP Echo Request packet
        memset(&icmpHeader, 0, sizeof(t_icmphdr));
        icmpHeader.type = ICMP_ECHO;
        icmpHeader.code = 0;
        icmpHeader.checksum = 0;
        icmpHeader.un.echo.id = getpid();
        icmpHeader.un.echo.sequence = i;


        // Prepare 56-byte payload with dummy data
        memset(payload, 'A', sizeof(payload));

        gettimeofday(&start, NULL);

        // Total packet size: ICMP header + payload
        int packet_size = sizeof(t_icmphdr) + sizeof(payload);
        unsigned char packet[packet_size];

        // Copy ICMP header and payload into the packet
        memcpy(packet, &icmpHeader, sizeof(t_icmphdr));
        memcpy(packet + sizeof(t_icmphdr), payload, sizeof(payload));

        // Recalculate checksum for the entire packet (header + payload)
        icmpHeader.checksum = checksum(packet, packet_size);

        // Recopy the header with the correct checksum
        memcpy(packet, &icmpHeader, sizeof(t_icmphdr));

        // Send ICMP Echo Request
        if (sendto(arg_addr->sockfd, &icmpHeader, sizeof(t_icmphdr), 0,
                   (struct sockaddr *)&arg_addr->serverAddr, sizeof(arg_addr->serverAddr)) <= 0) {
            perror("Error sending request");
            return 0;
        }

        // Calculate and print Round-Trip Time (RTT)
        gettimeofday(&end, NULL);
        rtt = (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_usec - start.tv_usec) / 1000.0;

        ttl = getTTL(arg_addr->sockfd);
        if (ttl == -1) {
            printf("Failed to get TTL\n");
            return 0;
        }

        printf("64 bytes from %s (%s): icmp_seq=%d ttl=%d time=%.2f ms\n",
               arg_addr->hostname, arg_addr->ip, i, ttl, rtt);

        i+=1;
        arg_addr->p_transmitted = i;
        global_arg_addr = arg_addr;
        usleep(1000000);
    }

    return 1;
}