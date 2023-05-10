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
    {"Tiempos Violentos (1994)", "Las vidas de dos sicarios de la mafia, un boxeador, un gángster y su esposa, y un par de bandidos comensales se entrelazan en cuatro historias de violencia y redención."},
    {"El Gran Lebowski (1998)", "Jeff 'The Dude' Lebowski, confundido con un millonario del mismo nombre, busca la restitución de su alfombra arruinada y recluta a sus compañeros de bolos para que lo ayuden a conseguirla."},
    {"Blade Runner (1982)", "Un corredor de cuchillas debe perseguir y acabar con cuatro replicantes que robaron una nave en el espacio y han regresado a la Tierra para encontrar a su creador."},
    {"El Club de la Pelea (1999)", "Un oficinista insomne y un fabricante de jabón despreocupado forman un club de lucha clandestino que se convierte en algo mucho, mucho más."}
};

void imprimir(char* texto) {
    refresh();
    // Alternar entre el color rosa y azul
    time_t now = time(NULL);
    struct tm *tm_now = localtime(&now);
    char time_str[20];
    strftime(time_str, sizeof(time_str), "%H:%M:%S", tm_now);    


    static int color = COLOR_WHITE;

    if (color == COLOR_WHITE) {
        color = COLOR_YELLOW;
    } else {
        color = COLOR_WHITE;
    }

    // Aplicar el color a la cadena y luego imprimir la cadena
    refresh();
    attron(COLOR_PAIR(color));
    printw("[%s] %s \n",time_str, texto);
    attroff(COLOR_PAIR(color));
    refresh();
    // getch();
    //endwin();
}


void handleRequest(int client_fd, char *request, movie movies[]){
    char *request_string = malloc(MAX_LINE);
    char *response_string = malloc(MAX_LINE);
    /* Interpreta la petición HTTP del cliente y separa sus tres componentes */ 

    char method[10], path[100], protocol[10];
    sscanf(request, "%s %s %s", method, path, protocol);
    sprintf(request_string, "Petición recibida: %s %s", method, path);
    imprimir(request_string);
    refresh();


    /* Verifica que tipo de petición se recibió y responde adecuadamente */

    if (strcmp(path, "/") == 0){                          /* Indice o página principal */
        serveFile(client_fd, "../html/index.html");
        imprimir("Enviando: index.html");
        refresh();
    }

    else if (strcmp(path, "/css/index_style.css") == 0){  /* Hoja de estilos */
        serveFile(client_fd, "../css/index_style.css");
        imprimir("Enviando: index_style.css");
        refresh();
    }

    else if (strcmp(path, "/js/index_script.js") == 0){   /* Script de la página principal */
        serveFile(client_fd, "../js/index_script.js");
        imprimir("Enviando: index_script.js");
        refresh();
    }

    else if (strncmp(path, "/booking.html", 8) == 0){
        serveFile(client_fd, "../html/booking.html");
        imprimir("Enviando: booking.html");
        refresh();
    }

    else if (strcmp(path, "/css/booking_style.css") == 0){
        serveFile(client_fd, "../css/booking_style.css");
        imprimir("Enviando: booking_style.css");
        refresh();
    }

    else if (strcmp(path, "/js/booking_script.js") == 0){
        serveFile(client_fd, "../js/booking_script.js");
        imprimir("Enviando: booking_script.js");
        refresh();
    }

    else if (strncmp(path, "/media/", 7) == 0){           /* Archivos multimedia */
        char *filename = malloc(MAX_LINE * sizeof(char));
        memset(filename, 0, MAX_LINE);
        filename = strcpy(filename, "..");
        filename = strcat(filename, path);
        char *decodedFN = decodeURL(filename);
        serveFile(client_fd, decodedFN);
        response_string = strcpy(response_string, "");
        response_string = strcpy(response_string, "Enviando poster de: ");
        response_string = strcat(response_string, &decodedFN[9]);
        imprimir(response_string);
        refresh();
    }

    else if (strcmp(path, "/movies") == 0){               /* Datos de las películas */
        sendMovieData(movies, client_fd, 5);
        imprimir("Enviando datos de las películas");
        refresh();
    }

    else if (strncmp(path, "/movies/", 8) == 0){
        char *movie_name = malloc(MAX_LINE * sizeof(char));
        memset(movie_name, 0, MAX_LINE);
        movie_name = strcpy(movie_name, "");
        movie_name = strcat(movie_name, &path[8]);
        char *decodedFN = decodeURL(movie_name);
        int index;
        for(int i = 0; i < 5; i++){
            if(strcmp(decodedFN, movies[i].title) == 0){
                index = i;
                break;
            }
        }
        movie temp[1];
        memcpy(temp, movies + index, sizeof(movie));
        sendMovieData(temp, client_fd, 1);
        response_string = strcpy(response_string, "");
        sprintf(response_string, "Enviando datos de: %s", decodedFN);
        imprimir(response_string);
        refresh();
    }

    else if (strncmp(path, "/checkout/", 10) == 0){
        char *chckout_details = malloc(MAX_LINE * sizeof(char));
        memset(chckout_details, 0, MAX_LINE);
        chckout_details = strcpy(chckout_details, "");
        chckout_details = strcat(chckout_details, &path[10]);
        char* details[3];
        char* token;
        token = strtok(chckout_details, "/");
        int i = 0;
        while( token != NULL ) {
            details[i] = token;
            token = strtok(NULL, "/");
            i++;
        }
        details[0] = strcpy(details[0], decodeURL(details[0]));
        setSeatsValues(details, client_fd, movies);
        imprimir("Enviando datos de la compra");
        refresh();
    }
    else if (strcmp(path, "/favicon.ico") == 0){          /* Icono de la página */
        serveFile(client_fd, "../media/favicon.ico");
        imprimir("Enviando: favicon.ico");
        refresh();
    }
    else{                                                 /* Cualquier otro path */
        sendResponse(client_fd, "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n");
        imprimir("Enviando: 404 Not Found");
        refresh();
    }
}



int main(int argc, char const *argv[]){
    initscr();
    start_color();
    init_pair(COLOR_WHITE, COLOR_WHITE, COLOR_BLACK);
    init_pair(COLOR_YELLOW, COLOR_YELLOW, COLOR_BLACK);

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
    endwin();
    return 0;
}
