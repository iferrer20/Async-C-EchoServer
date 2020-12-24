#include "poll.h"
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

struct epoll_event create_poll(int fd, int type) {
    struct epoll_event poll;
    poll.events = EPOLLIN | EPOLLERR | EPOLLHUP;

    struct poll_data* pd = (struct poll_data*) malloc(sizeof(struct poll_data));
    pd->fd = fd;
    pd->type = type;
    pd->buff_size = 0;
    pd->last_buff_size = 0;

    poll.data.ptr = pd;
    return poll;
}
int read_poll(struct epoll_event* poll, char* buff) { 
    struct poll_data* data = poll->data.ptr;
    int read_size = read(data->fd, buff, 65536); // Read data in recv_buff and set size in recv_size
    buff[read_size] = 0;
    data->last_buff_size = data->buff_size;
    data->buff_size += read_size;
    return read_size; 
}
int get_poll_fd(struct epoll_event* poll) {
    return ((struct poll_data*) (poll->data.ptr))->fd;
}
void add_poll(int epoll, struct epoll_event* poll) {
    epoll_ctl(epoll, EPOLL_CTL_ADD, get_poll_fd(poll), (struct epoll_event*) &poll->events);
}
void remove_poll(int epoll, struct epoll_event* poll) {
    epoll_ctl(epoll, EPOLL_CTL_DEL, get_poll_fd(poll), 0);
    free(poll->data.ptr);
}
void modify_poll(int epoll, struct epoll_event* poll) {
    epoll_ctl(epoll, EPOLL_CTL_MOD, get_poll_fd(poll), (struct epoll_event*) &poll->events);
}
