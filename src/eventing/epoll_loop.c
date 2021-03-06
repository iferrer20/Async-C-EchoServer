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
    loop->read_buff = malloc(65536 + 1); 
    loop->write_buff = malloc(65536 + 1);
    return loop;
}
void set_read_buffer(struct epoll_loop* loop, char* buff) {
    loop->read_buff = buff;
}
void run_loop(struct epoll_loop* loop) {
    while (!loop->stop) {
        loop->num_ready_polls = epoll_wait(loop->epoll, loop->ready_polls, MAX_EVENTS, -1);
        for (loop->current_ready_poll = 0; loop->current_ready_poll < loop->num_ready_polls; loop->current_ready_poll++) {
            struct epoll_event* poll = &loop->ready_polls[loop->current_ready_poll];
            int events = poll->events;
            struct poll_data* data = poll->data.ptr;

            if (data->fd == loop->sv->fd) {
                if (events & EPOLLIN) {
                    struct sockaddr_in clientaddr;
                    int conn_fd = accept_conn(data->fd, &clientaddr); 
                    struct epoll_event new_poll = create_poll(conn_fd);
                    add_poll(loop->epoll, &new_poll);
                    loop->num_polls++;
                    char ipv4[20];
                    get_client_ipv4(&clientaddr, ipv4);
                    printf("New client: %s\n", ipv4);
                }
            } else {
                if (events == (EPOLLIN|EPOLLOUT)) {
                    int size = read_poll(poll, loop->read_buff);
                    if (size) { 
                        write_poll(poll, loop->read_buff, size);
                    } else { 
                        close(data->fd);
                        remove_poll(loop->epoll, poll);
                        loop->num_polls--;
                        printf("Client disconnected\n");
                    }
                }
            }
        }
    }
}

void stop_loop(struct epoll_loop* loop) {
    loop->stop = 1;
}
