#include "networking.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int accept_conn(int fd, struct sockaddr_in* address) {
    int addrlen = sizeof(struct sockaddr_in);
    int new_fd = accept(fd, (struct sockaddr *)address, (socklen_t*)&addrlen);
    set_nonblock_socket(new_fd); // Set nonblock to new fd
    return new_fd;
}
int read_poll(struct epoll_event* poll) { // Read incoming data
    char recv_buff[1024];
    struct poll_data* data = poll->data.ptr;

    int recv_size = recv(data->fd, recv_buff, sizeof(recv_buff), 0); // Recv data in recv_buff and set size in recv_size
    if (data->buff) { 
        data->buff = realloc(data->buff, data->buff_size + recv_size + 1); // Reallocate buffer for insert incoming data
    } else {
        data->buff = malloc(recv_size + 1); // First malloc
    }
    memcpy(data->buff + data->buff_size, recv_buff, recv_size); // Copy incoming data to poll_data buffer
    data->buff[data->buff_size + recv_size] = 0;
    data->last_buff_size = data->buff_size;
    data->buff_size += recv_size;
    return recv_size; 
}
void set_nonblock_socket(int socket) {
    fcntl(socket, F_SETFL, fcntl(socket, F_GETFL, 0) | O_NONBLOCK);
}