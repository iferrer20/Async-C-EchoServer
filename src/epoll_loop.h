

struct epoll_loop {
    struct loop_data data;
    int num_polls;
    int num_ready_polls;
    int current_ready_poll;
    int fd;
    struct epoll_event events[1024];
    char* recv_buffer;
}

struct epoll_loop* create_loop();
void run_loop(struct epoll_loop* loop);