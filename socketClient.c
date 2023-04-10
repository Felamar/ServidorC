/* 
    MUST COMPILE AS: gcc -o socketClient socketClient.c socketUtil.c
*/

#include "socketUtil.h"

int main(int argc, char **argv){
  
    /*Init*/
    int socket_FD;
    int send_Bytes;
    int n_Read;
    struct sockaddr_in* server_Addr;
    char send_Line[MAX_LINE];
    char recv_Line[MAX_LINE];

    /* Check if the number of arguments is correct */
    if(argc != 2){
        exit_Error("Uso: %s <dirección del servidor>", argv[0]);
    }

    socket_FD = createSocket();
    if(socket_FD < 0){
        exit_Error("Hubo un error al crear el socket");
    }

    server_Addr = createAddress(argv[1]);
    if (connect(socket_FD, (S_A *) server_Addr, sizeof(*server_Addr)) < 0){
        exit_Error("Conexión fallida");
    }

    /*
      GET         -> comando para solicitar una página
      /"..."      -> especifica la ruta, en este caso "root"
      HTTP/1.1    -> versión HTTP que estamos usando
      \r\n\r\n    -> fin de solicitud
    */
    sprintf(send_Line, "GET / HTTP/1.1\r\n\r\n");
    send_Bytes = strlen(send_Line);

    if(write(socket_FD, send_Line, send_Bytes) == -1){
        exit_Error("Error de escritura");
    }else while(write(socket_FD, send_Line, send_Bytes) != send_Bytes);

    memset(recv_Line, 0, MAX_LINE);
    n_Read = read(socket_FD, recv_Line, MAX_LINE - 1);
    
    while(n_Read > 0){
        printf("%s", recv_Line);
        memset(recv_Line, 0, MAX_LINE);
        n_Read = read(socket_FD, recv_Line, MAX_LINE - 1);
    }
    if(n_Read < 0){
        exit_Error("Error de lectura");
    }

    exit(0);
}