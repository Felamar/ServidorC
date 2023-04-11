/* 
    MUST COMPILE AS: gcc -o ../build/socketServer socketServer.c socketUtil.c
*/

#include "socketUtil.h"

int main(int argc, char **argv){

    int listen_SocketFD, connection_FD, n;
    struct sockaddr_in *server_Address;
    uint8_t buff[MAX_LINE + 1];
    uint8_t recv_Line[MAX_LINE + 1];

    listen_SocketFD = createSocket();
    if(listen_SocketFD < 0){
        exit_Error("Error al crear socket.");
    }

    server_Address = createAddress("");
    if((bind(listen_SocketFD, (S_A *) server_Address, sizeof(*server_Address))) < 0){
        exit_Error("Error de unión");
    }
    if(listen(listen_SocketFD, 10) < 0){
        exit_Error("Error de escucha.");
    }

    for(;;){
        struct sockaddr_in addr;
        socklen_t addr_Len;
        char client_address[MAX_LINE + 1];


        printf("Esperando por una conexión en el puerto %d\n", SERVER_PORT);
        fflush(stdout);
        connection_FD = accept(listen_SocketFD, (S_A *) &addr, &addr_Len);
        
        inet_ntop(AF_INET, &addr, client_address, MAX_LINE);
        printf("Client connection: %s\n", client_address);

        memset(recv_Line, 0, MAX_LINE);
        n = read(connection_FD, recv_Line, MAX_LINE-1);
        while(n > 0){
            fprintf(stdout, "\n%s\n\n%s", bin2hex(recv_Line, n), recv_Line);
            if(recv_Line[n-1] == '\n'){
                break;
            }
            memset(recv_Line, 0, MAX_LINE);
            n = read(connection_FD, recv_Line, MAX_LINE-1);
        }
        if(n < 0){
            exit_Error("Error de lectura");
        }
        snprintf((char*) buff, sizeof(buff), "HTTP/1.0 200OK\r\n\r\nHELLO!\n\n");

        write(connection_FD, (char*) buff, strlen((char*) buff));
        close(connection_FD);
    }
}