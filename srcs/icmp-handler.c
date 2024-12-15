/* Create a raw socket for ICMP.
Construct an ICMP Echo Request packet.
Send the packet to the target IP.
Listen for and receive the ICMP Echo Reply.
Parse the reply and ensure it matches the request.
Calculate and display the results, including round-trip time. */

#include "../ping.h"

/**
 * Create and send an ICMP Echo Request packet, then receive and display the ICMP Echo Reply.
 */
int create_send_packet(const char *dest_ip) {
    ssize_t nb_bytes;
    int sock;
    struct sockaddr_in dest_addr;
    char packet[ICMP_HDR_SIZE + ICMP_BODY_SIZE];
    char buffer[1024];
    struct icmphdr *icmp_hdr = (struct icmphdr *)packet;
    struct timeval start_time, end_time;
   // socklen_t addr_len = sizeof(dest_addr);

    // 1. Create the RAW socket
    sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sock < 0) {
        printf("hola\n");
        printf("socket: %s\n", strerror(errno));
        printf("hola\n");
        return -1;
    }

    // 2. Initialize destination address
    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.sin_family = AF_INET;
    if (inet_pton(AF_INET, dest_ip, &dest_addr.sin_addr) <= 0) {
        perror("inet_pton");
        close(sock);
        return -1;
    }

    // 3. Create ICMP Echo Request
    memset(packet, 0, sizeof(packet));
    icmp_hdr->type = ICMP_ECHO;             // Type 8 = ICMP Echo Request
    icmp_hdr->code = 0;                     // Code 0 for Echo Request
    icmp_hdr->un.echo.id = getpid();        // Use PID as identifier
    icmp_hdr->un.echo.sequence = 1;         // Sequence number (could increment for multiple pings)
    icmp_hdr->checksum = 0;                 // Initially set checksum to 0
    gettimeofday(&start_time, NULL);        // Get the start time for calculating RTT
    memcpy(packet + ICMP_HDR_SIZE, &start_time, sizeof(start_time));  // Add timestamp in the body

    // 4. Compute ICMP checksum
    icmp_hdr->checksum = checksum(packet, ICMP_HDR_SIZE + ICMP_BODY_SIZE);

    // 5. Send ICMP Echo Request
    nb_bytes = sendto(sock, packet, ICMP_HDR_SIZE + ICMP_BODY_SIZE, 0, (struct sockaddr*)&dest_addr, sizeof(dest_addr));
    if (nb_bytes < 0) {
        perror("sendto");
        close(sock);
        return -1;
    }
    printf("Sent %ld bytes to %s\n", nb_bytes, dest_ip);

    // 6. Receive ICMP Echo Reply
    nb_bytes = recvfrom(sock, buffer, sizeof(buffer), 0, NULL, NULL);
    if (nb_bytes < 0) {
        perror("recvfrom");
        close(sock);
        return -1;
    }
    gettimeofday(&end_time, NULL);  // Get the end time to calculate round-trip time
    printf("Received %ld bytes from %s\n", nb_bytes, dest_ip);

    // 7. Display Round-Trip Time (RTT)
    double rtt = (end_time.tv_sec - start_time.tv_sec) * 1000.0;      // sec to ms
    rtt += (end_time.tv_usec - start_time.tv_usec) / 1000.0;          // us to ms
    printf("RTT: %.3f ms\n", rtt);

    close(sock);
    return 0;
}