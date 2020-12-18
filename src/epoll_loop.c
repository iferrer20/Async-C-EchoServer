#include "epoll_loop.h"

struct epoll_loop* create_loop() {
    struct epoll_loop *loop = (struct epoll_loop*) malloc(sizeof(struct epoll_loop)); // Create new epoll_loop
    loop->num_polls = 0;
    loop->num_ready_polls = 0;
    loop->current_ready_poll = 0;
    loop->fd = epoll_create1(0);
    return loop;
}

void run_loop(struct epoll_loop* loop) {
    while (loop->num_polls) {
        loop->num_ready_polls = epoll_wait(loop->fd, loop->ready_polls, 1024, -1);
        
        for (loop->current_ready_poll = 0; loop->current_ready_poll < loop->num_ready_polls; loop->current_ready_poll++) {
            int events = loop->ready_polls[loop->current_ready_poll].events;
            int error = events & (EPOLLERR | EPOLLHUP);
        }
    }
}