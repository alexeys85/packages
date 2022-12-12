#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>
#ifdef __GLIBC__
#include <getopt.h>
#endif

#define MAX_DATA_SIZE (1500 - 20 - 8)

uint8_t hex(char ch) {
    if(('0' <= ch) && (ch <= '9'))
        return (ch - '0');
    else if(('A' <= ch) && (ch <= 'F'))
        return (ch - 'A' + 10);
    else if(('a' <= ch) && (ch <= 'f'))
        return (ch - 'a' + 10);
    else {
        fprintf(stderr, "HEX string contains invalid char(s)\n");
        exit(EXIT_FAILURE);
    }
}

int create_sockaddr(struct sockaddr_in* sockaddr, const char* ip, int port) {
    memset(sockaddr, 0, sizeof(*sockaddr));
    sockaddr->sin_family = AF_INET;
    sockaddr->sin_port = htons(port);
    if(inet_pton(AF_INET, ip, &(sockaddr->sin_addr)) != 1)
        return 1;
    return 0;
}

int main(int argc, char* argv[]) {
    char ip_dst[INET_ADDRSTRLEN], ip_src[INET_ADDRSTRLEN] = "0.0.0.0";
    uint8_t tx_data[MAX_DATA_SIZE], rx_data[MAX_DATA_SIZE];
    char* result = NULL;
    struct sockaddr_in sockaddr_dest, sockaddr_src;
    size_t in_len;

    int dport = 0, sport = 0;
    int wait_answer = 0;
    int opt;
    ssize_t rx, tx;

    memset(ip_dst, 0, sizeof(ip_dst));

    while((opt = getopt(argc, argv, "d:p:b:s:a:")) != -1) {
        switch(opt) {
            case 'd':
                strncpy(ip_dst, optarg, sizeof(ip_dst) - 1);
                break;
            case 'p':
                dport = atoi(optarg);
                break;
            case 'b':
                strncpy(ip_src, optarg, sizeof(ip_src) - 1);
                break;
            case 's':
                sport = atoi(optarg);
                break;
            case 'a':
                wait_answer = atoi(optarg);
                break;
            default: /* '?' */
                fprintf(stderr,
                        "Usage: %s -d dst_ip -p dst_port [-b src_ip] [-s src_port] [-a wait_answ_ms] <HEX string>\n",
                        argv[0]);
                exit(EXIT_FAILURE);
                break;
        }
    }

    if(strlen(ip_dst) == 0 || dport == 0) {
        fprintf(stderr, "Dest ip address and port expected\n");
        exit(EXIT_FAILURE);
    }

    if(optind >= argc) {
        fprintf(stderr, "Expected HEX string to send\n");
        exit(EXIT_FAILURE);
    }

    //входная строка дожна иметь чётную длину и не превышать макс размер
    in_len = strlen(argv[optind]);
    if((in_len & 1) != 0) {
        fprintf(stderr, "HEX string must be multiple of 2\n");
        exit(EXIT_FAILURE);
    } else if(in_len * 2 > MAX_DATA_SIZE) {
        fprintf(stderr, "To long HEX string\n");
        exit(EXIT_FAILURE);
    }

    int sock = socket(AF_INET, SOCK_DGRAM | SOCK_CLOEXEC, 0);
    if(sock < 0) {
        fprintf(stderr, "Failed to create socket: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }

    if(strlen(ip_src) > 0 || sport != 0) {
        //биндимся на конкретный адрес (и порт)
        if(create_sockaddr(&sockaddr_src, ip_src, sport) != 0) {
            fprintf(stderr, "Failed to convert src ip '%s'\n", ip_src);
            close(sock);
            exit(EXIT_FAILURE);
        }

        if(bind(sock, (struct sockaddr*)&sockaddr_src, sizeof(sockaddr_src)) != 0) {
            fprintf(stderr, "Failed to bind to %s:%d: %s\n", ip_src, sport, strerror(errno));
            close(sock);
            exit(EXIT_FAILURE);
        }
    }

    if(create_sockaddr(&sockaddr_dest, ip_dst, dport) != 0) {
        fprintf(stderr, "Failed to convert dst ip '%s'\n", ip_dst);
        close(sock);
        exit(EXIT_FAILURE);
    }

    if(wait_answer > 0) {
        //задаём время ожидания ответа
        struct timeval tv;
        tv.tv_sec = wait_answer / 1000;
        tv.tv_usec = (wait_answer % 1000) * 1000;
        if(setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
            fprintf(stderr, "Failed to setsockopt: %s\n", strerror(errno));
            close(sock);
            exit(EXIT_FAILURE);
        }
    }

    //преобразуем HEX строку в байты
    for(size_t i = 0; i < in_len; i += 2)
        tx_data[i / 2] = hex(argv[optind][i + 0]) << 4 | hex(argv[optind][i + 1]);

    //делаем connect, чтобы сразу выходить из ожидания приёма при получении ICMP unreach сообщения
    if(connect(sock, (struct sockaddr*)&sockaddr_dest, sizeof(sockaddr_dest)) != 0) {
        fprintf(stderr, "Connect to %s:%d failed: %s\n", ip_dst, dport, strerror(errno));
        close(sock);
        exit(EXIT_FAILURE);
    }

    tx = send(sock, tx_data, in_len / 2, 0);
    if(tx < 0) {
        fprintf(stderr, "Failed to send data to %s:%d: %s\n", ip_dst, dport, strerror(errno));
        close(sock);
        exit(EXIT_FAILURE);
    }

    if(wait_answer > 0) {
        //ждём ответных данных
        rx = recv(sock, rx_data, sizeof(rx_data), 0);
        if(rx < 0) {
            if(errno == EAGAIN)
                fprintf(stderr, "Receive timeout\n");
            else if(errno == ECONNREFUSED)
                fprintf(stderr, "Remote host refused connection\n");
            else
                fprintf(stderr, "Receive error: %s\n", strerror(errno));
            close(sock);
            exit(EXIT_FAILURE);
        }

        //преобразуем ответные данные в HEX строку + '\0'
        result = malloc(rx * 2 + 1);
        if(!result) {
            fprintf(stderr, "Failed to allocate result buffer\n");
            close(sock);
            exit(EXIT_FAILURE);
        }
        const char xx[] = "0123456789ABCDEF";
        for(ssize_t i = 0; i < rx; ++i) {
            result[2 * i + 0] = xx[(rx_data[i] >> 4) & 0x0f];
            result[2 * i + 1] = xx[(rx_data[i] >> 0) & 0x0f];
        }
        result[rx * 2] = '\0';

        fprintf(stdout, "%s\n", result);

        free(result);
    }

    close(sock);

    exit(EXIT_SUCCESS);
}
