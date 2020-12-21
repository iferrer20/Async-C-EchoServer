#pragma once
#include "../loop/epoll_loop.h"
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <fcntl.h>

int accept_conn(int fd, struct sockaddr_in* address);
int read_poll(struct epoll_event* poll);
void set_nonblock_socket(int socket);