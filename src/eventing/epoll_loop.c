#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "epoll_loop.h"
#include "poll.h"
#include "../net/networking.h"

struct epoll_loop* create_loop() {
    struct epoll_loop *loop = (struct epoll_loop*) malloc(sizeof(struct epoll_loop)); // Create new epoll_loop
    loop->stop = 0;
    loop->num_polls = 0;
    loop->num_ready_polls = 0;
    loop->current_ready_poll = 0;
    loop->epoll = epoll_create1(0);
    loop->buff = malloc(65536 + 1); 

    return loop;
}

void run_loop(struct epoll_loop* loop) {
    while (!loop->stop) {
        loop->num_ready_polls = epoll_wait(loop->epoll, loop->ready_polls, 1024, -1);
        for (loop->current_ready_poll = 0; loop->current_ready_poll < loop->num_ready_polls; loop->current_ready_poll++) {
            struct epoll_event* poll = &loop->ready_polls[loop->current_ready_poll];
            int events = poll->events;
            struct poll_data* data = poll->data.ptr;
            int type = data->type;
            
            switch (type) {
                case POLL_LISTEN:
                    if (events & EPOLLIN) {
                        struct sockaddr_in clientaddr;
                        int conn_fd = accept_conn(data->fd, &clientaddr); 
                        struct epoll_event new_poll = create_poll(conn_fd, POLL_CONN);
                        add_poll(loop->epoll, &new_poll);
                        loop->num_polls++;
                    }
                    break;
                case POLL_CONN:
                    if (events & EPOLLIN) {
                        if (read_poll(poll, loop->buff)) { 
                            printf(loop->buff);
                        } else { // If read_poll returns 0 close connection
                             close(data->fd);
                             remove_poll(loop->epoll, poll);
                             loop->num_polls--;
                        }
                    }
                    break;
            }
        }
    }
}

void stop_loop(struct epoll_loop* loop) {
    loop->stop = 1;
}