#include "socketUtil.h"


/*  Esta función interpreta el método HTTP utilizado, el path solicitado y el protocolo enviado por el clienta.
    Luego, llama a la función correspondiente para manejar la petición.                                         */

void handleRequest(int client_fd, char *request, movie movies[]){

    /* Interpreta la petición HTTP del cliente y separa sus tres componentes */ 

    char method[10], path[100], protocol[10];
    sscanf(request, "%s %s %s", method, path, protocol);
    printf("\n\n##########################################################################################");
    printf("\n\nReceived request: %s  %s\n\n", method, path);

    /* Verifica que tipo de petición se recibió y responde adecuadamente */

    if (strcmp(path, "/") == 0){                          /* Indice o página principal */
        serveFile(client_fd, "../html/index.html");
    }

    else if (strcmp(path, "/css/index_style.css") == 0){  /* Hoja de estilos */
        serveFile(client_fd, "../css/index_style.css");
    }

    else if (strcmp(path, "/js/index_script.js") == 0){   /* Script de la página principal */
        serveFile(client_fd, "../js/index_script.js");
    }

    else if (strncmp(path, "/booking.html", 8) == 0){
        serveFile(client_fd, "../html/booking.html");
    }

    else if (strcmp(path, "/css/booking_style.css") == 0){
        serveFile(client_fd, "../css/booking_style.css");
    }

    else if (strcmp(path, "/js/booking_script.js") == 0){
        serveFile(client_fd, "../js/booking_script.js");
    }

    else if (strncmp(path, "/media/", 7) == 0){           /* Archivos multimedia */
        char *filename = malloc(MAX_LINE * sizeof(char));
        memset(filename, 0, MAX_LINE);
        filename = strcpy(filename, "..");
        filename = strcat(filename, path);
        char *decodedFN = decodeURL(filename);
        serveFile(client_fd, decodedFN);
    }

    else if (strcmp(path, "/movies") == 0){               /* Datos de las películas */
        sendMovieData(movies, client_fd, 5);
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
        printf("%s\n", temp[0].title);
        for(int i = 0; i < 4; i++){
            printf("%s\n", temp[0].showtimes[i].time);
            for(int j = 0; j < 10; j++){
                printf("%d ", temp[0].showtimes[i].seats[j]);
            }
        }
        sendMovieData(temp, client_fd, 1);
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

    }
    else if (strcmp(path, "/favicon.ico") == 0){          /* Icono de la página */
        serveFile(client_fd, "../media/favicon.ico");
    }
    else{                                                 /* Cualquier otro path */
        sendResponse(client_fd, "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n");
    }
}

void setSeatsValues(char **details, int client_fd, movie movies[]){
    // printf("Setting seats values...\n");
    int index_of_movie, index_of_time, index_of_seat;
    char *token;
    for (int i = 0; i < 5; i++){
        if (strcmp(details[0], movies[i].title) == 0){
            index_of_movie = i;
            break;
        }
    }
    for (int i = 0; i < 4; i++){
        if (strcmp(details[1], movies[index_of_movie].showtimes[i].time) == 0){
            index_of_time = i;
            break;
        }
    }
    token = strtok(details[2], ",");
    movie *movie_ptr = movies + index_of_movie;

    while (token != NULL){
        index_of_seat = ((token[0] - 'A') * 5) + (token[1] - '0' - 1);
        movie_ptr->showtimes[index_of_time].seats[index_of_seat] = false;
        token = strtok(NULL, ",");
    }
    movie temp[1];  
    memcpy(temp, movies + index_of_movie, sizeof(movie));
    sendMovieData(movies, client_fd, 1);
}

/* Esta función envía interpreta el path del archivo solicitado y envía el archivo al cliente. */

void serveFile(int client_fd, char *filename){

    printf("Serving file: %s\n", filename);

    FILE *file;

    /* Se verifica el tipo de path solicitado. Si es un archivo multimedia, se abre en modo binario. */
    if (strstr(filename, "media") != NULL){  file = fopen(filename, "rb");  }
    else{  file = fopen(filename, "r");  }

    if (file != NULL){

        /*Se obtiene el tamaño del archivo */
        fseek(file, 0, SEEK_END);
        int size = ftell(file);
        fseek(file, 0, SEEK_SET);

        /* Se envía el header HTTP con el tipo de contenido y el tamaño del archivo. */
        char headers[100];
        sprintf(headers, "HTTP/1.1 200 OK\r\nContent-Type: %s\r\nContent-Length: %d\r\n\r\n", getMimeType(filename), size);
        sendResponse(client_fd, headers);

        /* Se envía el archivo al cliente en bloques de 4096 bytes. */
        char buffer[MAX_LINE];
        int bytesRead;
        while ((bytesRead = fread(buffer, 1, MAX_LINE, file)) > 0){
            send(client_fd, buffer, bytesRead, 0);
        }
        fclose(file);
    }
    else{
        /* Si el archivo no existe, se envía un header HTTP con código 404. */
        sendResponse(client_fd, "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n");
    }
}

/* Esta función envía los datos de las películas al cliente en formato json. */
void sendMovieData(movie movies[], int client_fd, int no_movies){

    char *data = malloc(MAX_LINE * sizeof(char));
    memset(data, 0, MAX_LINE);
    data = strcpy(data, "[");

    for (int i = 0; i < no_movies; i++){
        char *json = movieToJSON(movies[i]);
        data = strcat(data, json);
        free(json);
        if (i < no_movies - 1){  data = strcat(data, ",");  }
    }

    data = strcat(data, "]");

    printf("Sending movie data:\n%s\n", data);
    /* Se envía el header HTTP con el tipo de contenido y el tamaño de los datos. */
    char headers[100];
    sprintf(headers, "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: %ld\r\n\r\n", strlen(data));
    sendResponse(client_fd, headers);

    /* Se envían los datos de las películas al cliente. */
    sendResponse(client_fd, data);
    // printf("%s\n", data);
    free(data);
}


/* Esta función asigna los horarios de las películas y la disponibilidad de asientos. */
void assignShowTimes(movie *m, showtime showtimes[4]){
    for (int i = 0; i < 4; i++){
        m->showtimes[i] = showtimes[i];
    }
}

/* Esta función re-escribe la información de movies[] y retorna una cadena con formato json. */
char *movieToJSON(movie m){

    char *json = malloc(MAX_LINE * sizeof(char));
    char *showtimes_json = malloc(200 * sizeof(char));

    showtimes_json = strcpy(showtimes_json, "");
    for(int i = 0; i < 4; i++){
        char *showtime_json = showtimeToJSON(m.showtimes[i]);
        showtimes_json = strcat(showtimes_json, showtime_json);
        free(showtime_json);
        if(i < 3){
            showtimes_json = strcat(showtimes_json, ",");
        }
    }

    sprintf(json, "{\"title\":\"%s\",\"description\":\"%s\",\"showtimes\":[%s]}", m.title, m.description, showtimes_json);
    return json;
}

/* Esta función re-escribe la información de showtimes[] y retorna una cadena con formato json. */
char *showtimeToJSON(showtime s){
    char *json = malloc(MAX_LINE * sizeof(char));
    char *seats_json = malloc(30 * sizeof(char));

    seats_json = strcpy(seats_json, "");
    for(int j = 0; j < 10; j++){
        seats_json = strcat(seats_json, (s.seats[j] == true ? "1" : "0"));
        if(j < 9){
            strcat(seats_json, ",");
        }
    }
    sprintf(json, "{\"time\":\"%s\",\"seats\":[%s]}", s.time, seats_json);

    free(seats_json);
    return json;
}

/* Esta función retorna el tipo de contenido de un archivo según su extensión. */
char *getMimeType(char *filename){

    char *extension = strrchr(filename, '.');
    if (extension != NULL){
        if      (strcmp(extension, ".html") == 0){ return "text/html"               ;}
        else if (strcmp(extension, ".css")  == 0){ return "text/css"                ;}
        else if (strcmp(extension, ".js")   == 0){ return "application/javascript"  ;}
        else if (strcmp(extension, ".jpg")  == 0){ return "image/jpeg"              ;}
        else if (strcmp(extension, ".png")  == 0){ return "image/png"               ;}
    }
    /* Si no se reconoce la extensión, se retorna el tipo de contenido por defecto. */
    return "application/octet-stream";
}

/* Esta función envía una respuesta al cliente con el cuerpo especificado. */
void sendResponse(int client_fd, char *response){   send(client_fd, response, strlen(response), 0)  ;}


/* Esta función decodifica una URL y reemplaza los caracteres "%20" por espacios. */
char *decodeURL(char *url){

    /* Se reserva memoria para la cadena resultante. */
    char *result = malloc(strlen(url) + 1);
    if (!result) return NULL;

    int i, j = 0;
    for (i = 0; url[i] != '\0'; i++){
        if (strncmp(&url[i], "%20", 3) == 0){
            result[j++] = ' ';
            i += 2;
        }
        else{
            result[j++] = url[i];
        }
    }

    result[j] = '\0';
    return result;
}