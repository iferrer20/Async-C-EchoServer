#pragma once
#include <sys/epoll.h>

#include "../server/server.h"

#define MAX_EVENTS 1024

struct epoll_loop {
    char stop;
    int num_polls;
    int num_ready_polls;
    int current_ready_poll;
    int epoll;
    struct server* sv;
    struct epoll_event ready_polls[MAX_EVENTS];
    char* read_buff;
    char* write_buff;
};

struct epoll_loop* create_loop();
void run_loop(struct epoll_loop* loop);
void stop_loop(struct epoll_loop* loop);
