#include <stdio.h> 
#include "server/server.h"

int main() {
    struct server_config sv_conf;
    sv_conf.listen_addr = "0.0.0.0";
    sv_conf.listen_port = 8080;
    struct server* sv = create_server(&sv_conf);
    run_server(sv);
}