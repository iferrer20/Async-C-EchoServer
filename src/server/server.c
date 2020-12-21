#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "server.h"
#include "../loop/epoll_loop.h"
#include "../net/networking.h"

struct server* create_server(struct server_config* sv_conf) {
    struct server *sv = (struct server*) malloc(sizeof(struct server));
    sv->sv_config = sv_conf;
    return sv;
}

int run_server(struct server* sv) {
    int opt = 1; 
    struct epoll_loop* loop = create_loop();
    
    // Creating socket file descriptor 
    if ((sv->fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) { 
        perror("socket failed"); 
        return 1;
        
    }
    // Forcefully attaching socket to the port 8080 
    if (setsockopt(sv->fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        return 1;
    }
    set_nonblock_socket(sv->fd);

    sv->listen_addr_sock.sin_family = AF_INET; 
    sv->listen_addr_sock.sin_addr.s_addr = inet_addr(sv->sv_config->listen_addr);
    sv->listen_addr_sock.sin_port = htons(sv->sv_config->listen_port); 

    // Forcefully attaching socket to the port 8080 
    if (bind(sv->fd, (struct sockaddr *)&sv->listen_addr_sock, sizeof(sv->listen_addr_sock))<0) { 
        perror("bind failed"); 
        return 1; 
    } 
    if (listen(sv->fd, 3) < 0) { 
        perror("listen"); 
        return 1;
    }
    loop->sv = sv;
    struct epoll_event new_poll = create_poll(sv->fd, POLL_BIND);
    add_poll(loop, &new_poll);
    run_loop(loop);
    return 0;
}