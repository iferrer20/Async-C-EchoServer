#pragma once
#include <sys/epoll.h>

struct poll_data {
    int fd;
    uint64_t read_buff_pos;
    uint64_t write_buff_pos;
};

struct epoll_event create_poll(int fd);
int get_poll_fd(struct epoll_event* poll);
void add_poll(int epoll, struct epoll_event* poll);
int read_poll(struct epoll_event* poll, char* buff);
int write_poll(struct epoll_event* poll, char* buff, int size);
void remove_poll(int epoll, struct epoll_event* poll);
void poll_events(struct epoll_event* poll);
