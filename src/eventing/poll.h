#pragma once
#include <sys/epoll.h>

#define POLL_LISTEN 0
#define POLL_CONN 1

struct poll_data {
    int type;
    int fd;
    uint64_t buff_size;
    uint64_t last_buff_size;
};

struct epoll_event create_poll(int fd, int type);
int get_poll_fd(struct epoll_event* poll);
void add_poll(int epoll, struct epoll_event* poll);
int read_poll(struct epoll_event* poll, char* buff);
void remove_poll(int epoll, struct epoll_event* poll);
void poll_events(struct epoll_event* poll);