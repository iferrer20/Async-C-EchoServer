#include "epoll_loop.h"
#include "../net/networking.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

struct epoll_loop* create_loop() {
    struct epoll_loop *loop = (struct epoll_loop*) malloc(sizeof(struct epoll_loop)); // Create new epoll_loop
    loop->num_polls = 0;
    loop->num_ready_polls = 0;
    loop->current_ready_poll = 0;
    loop->epoll = epoll_create1(0);
    return loop;
}

void run_loop(struct epoll_loop* loop) {
    while (loop->num_polls) {
        loop->num_ready_polls = epoll_wait(loop->epoll, loop->ready_polls, 1024, -1);
        for (loop->current_ready_poll = 0; loop->current_ready_poll < loop->num_ready_polls; loop->current_ready_poll++) {
            struct epoll_event poll = loop->ready_polls[loop->current_ready_poll];
            int events = poll.events;
            struct poll_data* data = poll.data.ptr;
            int type = data->type;

            // int error = events & (EPOLLERR | EPOLLHUP);
            // if (error) {
            //     printf("err\n");
            //     continue;
            // }
            
            if (type == POLL_BIND) {
                if (events & EPOLLIN) {
                    int conn_fd = accept_conn(loop->sv->fd, &loop->sv->listen_addr_sock);
                    printf("New client\n");
                    struct epoll_event new_poll = create_poll(conn_fd, POLL_CONN);
                    add_poll(loop, &new_poll);
                }
            } else if(type == POLL_CONN) {
                if (events & EPOLLIN) {
                    if (!read_poll(&poll)) {
                        close(data->fd);
                        remove_poll(loop, &poll);
                        printf("A client left\n");
                    } else {
                        printf(data->buff + data->last_buff_size);
                    }
                }
            }
        }
    }
}

struct epoll_event create_poll(int fd, int type) {
    struct epoll_event poll;
    poll.events = EPOLLIN | EPOLLOUT | EPOLLERR | EPOLLHUP;

    struct poll_data* pd = (struct poll_data*) malloc(sizeof(struct poll_data));
    pd->fd = fd;
    pd->type = type;
    pd->buff = NULL;
    pd->buff_size = 0;
    pd->last_buff_size = 0;

    poll.data.ptr = pd;
    return poll;
}

void add_poll(struct epoll_loop* loop, struct epoll_event* poll) {
    epoll_ctl(loop->epoll, EPOLL_CTL_ADD, ((struct poll_data*) (poll->data.ptr))->fd, (struct epoll_event*) &poll->events);
    loop->num_polls++;
}
void remove_poll(struct epoll_loop* loop, struct epoll_event* poll) {
    epoll_ctl(loop->epoll, EPOLL_CTL_DEL, ((struct poll_data*) (poll->data.ptr))->fd, NULL);
    loop->num_polls--;
    free(((struct poll_data*) (poll->data.ptr))->buff);
    free(poll->data.ptr);
}