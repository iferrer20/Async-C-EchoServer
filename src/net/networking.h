#pragma once
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <fcntl.h>

int accept_conn(int fd, struct sockaddr_in* address);
void set_nonblock_socket(int socket);