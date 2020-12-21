#pragma once
#include <sys/epoll.h>
#include "../server/server.h"

#define POLL_BIND 0
#define POLL_CONN 1

struct epoll_loop {
    int num_polls;
    int num_ready_polls;
    int current_ready_poll;
    int epoll;
    struct epoll_event ready_polls[1024];
    struct server* sv;
    char* recv_buffer;
};
struct poll_data {
    int type;
    int fd;
    char* buff;
    int buff_size;
    int last_buff_size;
};

struct epoll_loop* create_loop();
struct epoll_event create_poll(int fd, int type);
void run_loop(struct epoll_loop* loop);
void add_poll(struct epoll_loop* loop, struct epoll_event* poll);
void remove_poll(struct epoll_loop* loop, struct epoll_event* poll);