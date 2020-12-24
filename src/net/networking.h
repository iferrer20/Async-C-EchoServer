#pragma once
#include <netinet/in.h>

int accept_conn(int fd, struct sockaddr_in* address);
void set_nonblock_socket(int socket);
void get_client_ipv4(struct sockaddr_in* address, char* buff);