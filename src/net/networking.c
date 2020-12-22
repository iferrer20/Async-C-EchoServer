#include "networking.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>

int accept_conn(int fd, struct sockaddr_in* address) {
    int addrlen = sizeof(struct sockaddr_in);
    int new_fd = accept(fd, (struct sockaddr *)address, (socklen_t*)&addrlen);
    set_nonblock_socket(new_fd); // Set nonblock to new fd
    return new_fd;
}
void set_nonblock_socket(int socket) {
    fcntl(socket, F_SETFL, fcntl(socket, F_GETFL, 0) | O_NONBLOCK);
}