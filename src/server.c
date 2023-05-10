#include "socketUtil.h"
showtime showtimes[4] = {
    /* columns    1     2     3     4     5                */
    {"12:10", {
                  true, true, true, true, true, /*  row A  */
                  true, true, true, true, true  /*  row B  */
              }},
    {"15:20", {
                  true, true, true, true, true, /*  row A  */
                  true, true, true, true, true  /*  row B  */
              }},
    {"18:30", {
                  true, true, true, true, true, /*  row A  */
                  true, true, true, true, true  /*  row B  */
              }},
    {"21:40", {
                  true, true, true, true, true, /*  row A  */
                  true, true, true, true, true  /*  row B  */
              }}};

movie movies[] = {
    {"El Padrino (1972)", "El anciano patriarca de una dinastía del crimen organizado transfiere el control de su imperio clandestino a su reacio hijo."},
    {"Tiempos Violentos(1994)", "Las vidas de dos sicarios de la mafia, un boxeador, un gángster y su esposa, y un par de bandidos comensales se entrelazan en cuatro historias de violencia y redención."},
    {"El Gran Lebowski (1998)", "Jeff 'The Dude' Lebowski, confundido con un millonario del mismo nombre, busca la restitución de su alfombra arruinada y recluta a sus compañeros de bolos para que lo ayuden a conseguirla."},
    {"Blade Runner (1982)", "Un corredor de cuchillas debe perseguir y acabar con cuatro replicantes que robaron una nave en el espacio y han regresado a la Tierra para encontrar a su creador."},
    {"El Club de la Pelea (1999)", "Un oficinista insomne y un fabricante de jabón despreocupado forman un club de lucha clandestino que se convierte en algo mucho, mucho más."}
};

int main(int argc, char const *argv[]){

    for(int i = 0; i < 5; i++){
        assignShowTimes(&movies[i], showtimes);
    }
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
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))){
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
        handleRequest(client_socket, buffer, movies);

        /* Se cierra la conexión con el cliente. */
        close(client_socket);
    }

    return 0;
}
