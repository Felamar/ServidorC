#include "socketUtil.h"

int main(int argc, char const *argv[])
{
    /* Se crea el socket del servidor. */
    int server_fd, client_socket, client_request;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[MAX_LINE] = {0};


    /* Se crea un descriptor de archivo. */
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    /* Se configura el descriptor de archivo para que reutilice la dirección y el puerto. */
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    /* Se configura la dirección y el puerto del socket. */
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    /* Se enlaza el socket a la dirección y el puerto. */
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0){
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    /* Se configura el socket para que escuche conexiones entrantes. */
    if (listen(server_fd, 10) < 0){
        perror("listen");
        exit(EXIT_FAILURE);
    }

    /* Se espera a que lleguen conexiones. */
    while (true){

        /* Se acepta la conexión entrante. */
        if ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0){
            perror("accept");
            exit(EXIT_FAILURE);
        }

        /* Se lee la petición del cliente. */
        memset(buffer, 0, sizeof(buffer));
        client_request = read(client_socket, buffer, MAX_LINE);

        /* Se maneja la petición del cliente. */
        handleRequest(client_socket, buffer);

        /* Se cierra la conexión con el cliente. */
        close(client_socket);
    }

    return 0;
}
