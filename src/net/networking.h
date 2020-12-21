#pragma once
#include "../loop/epoll_loop.h"
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <fcntl.h>

int accept_conn(int fd, struct sockaddr_in* address);
int read_poll(struct poll_data* data);
void set_nonblock_socket(int socket);