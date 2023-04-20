
#include "socketUtil.h"

void handle_Connection(int client_socket);
int check(int expression, int error_Condition, const char *msg);

int main(int argc, char **argv){

    int server_FD, client_FD, no_Bytes;
    struct sockaddr_in *server_Address, *client_Address;

    server_Address = createAddress("");
    

    exit(0);
}