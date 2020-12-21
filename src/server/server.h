#pragma once
#include <netinet/in.h> 

struct server {
    struct sockaddr_in listen_addr_sock;
    int fd;
    struct server_config* sv_config;
};
struct server_config {
    char *listen_addr;
    int listen_port;
};

int run_server(struct server* sv);
struct server* create_server(struct server_config* sv_conf);
void stop_server(struct server* sv);