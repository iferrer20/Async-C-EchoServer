#include "networking.h"

int accept_conn(int fd, struct sockaddr_in* address) {
    int addrlen = sizeof(struct sockaddr_in);
    //return accept4(fd, (struct sockaddr *)address, (socklen_t*)&addrlen, SOCK_CLOEXEC | SOCK_NONBLOCK);
    return accept(fd, (struct sockaddr *)address, (socklen_t*)&addrlen);
//     accept4(fd, (struct sockaddr *) addr, &addr->len, SOCK_CLOEXEC | SOCK_NONBLOCK);
}
void set_nonblock_socket(int socket) {
    fcntl(socket, F_SETFL, fcntl(socket, F_GETFL, 0) | O_NONBLOCK);
}