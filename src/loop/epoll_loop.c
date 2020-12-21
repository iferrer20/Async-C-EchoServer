#include "epoll_loop.h"
#include "../net/networking.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

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
            struct poll_data* data = &loop->ready_polls[loop->current_ready_poll].data.u64;
            
            int type = data->type;
            int fd = data->fd;

            if (type == POLL_BIND) {
                printf("testing\n");
                if (events & EPOLLIN) {
                    int conn_fd = accept_conn(loop->sv->fd, &loop->sv->listen_addr_sock);
                    //printf("poll_bind: %d\n", conn_fd);
                    add_poll(loop, conn_fd, POLL_CONN);
                    char hello[] = "hello";
                    send(conn_fd, hello, sizeof(hello), 0);
                }
            } else if(type == POLL_CONN) {
                
                //printf("poll_conn: %d\n", loop->num_ready_polls);
                if (events & EPOLLIN) {
                    close(fd);
                }
            }
            
        }
    }
}



void add_poll(struct epoll_loop* loop, int socket, int type) {
    struct epoll_event event;
    struct poll_data pd;
    event.events = EPOLLIN | EPOLLOUT;
    pd.fd = socket;
    pd.type = type;

    struct poll_data* buff = &event.data.u64;
    *buff = pd;

    epoll_ctl(loop->fd, EPOLL_CTL_ADD, socket, &event);
    loop->num_polls++;
}