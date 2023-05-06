#include "socketUtil.h"

showtime showtimes[4] = {
    /* columns     1     2     3     4     5               */
    {"12:00", { {true, true, true, true, true}  /*  row A  */
                {true, true, true, true, true}    /*  row B  */
            }
    },
    {"15:00", { {true, true, true, true, true}  /*  row A  */
                {true, true, true, true, true}  /*  row B  */
            }
    },
    {"18:00", { {true, true, true, true, true}  /*  row A  */
                {true, true, true, true, true}  /*  row B  */
            }
    },
    {"21:00", { {true, true, true, true, true}  /*  row A  */
                {true, true, true, true, true}  /*  row B  */
            }
    }
};

movie movies[] = {
    {"The Godfather (1972)", "The aging patriarch of an organized crime dynasty transfers control of his clandestine empire to his reluctant son.", showtimes},
    {"Pulp Fiction (1994)", "The lives of two mob hitmen, a boxer, a gangster and his wife, and a pair of diner bandits intertwine in four tales of violence and redemption.", showtimes},
    {"The Big Lebowski (1998)", "Jeff \'The Dude\' Lebowski, mistaken for a millionaire of the same name, seeks restitution for his ruined rug and enlists his bowling buddies to help get it.", showtimes},
    {"Blade Runner (1982)", "A blade runner must pursue and terminate four replicants who stole a ship in space, and have returned to Earth to find their creator.", showtimes},
    {"Fight Club (1999)", "An insomniac office worker and a devil-may-care soapmaker form an underground fight club that evolves into something much, much more.", showtimes},
};


/*  Esta función interpreta el método HTTP utilizado, el path solicitado y el protocolo enviado por el clienta. 
    Luego, llama a la función correspondiente para manejar la petición.                                         */

void handleRequest(int client_fd, char *request){

    /* Interpreta la petición HTTP del cliente y separa sus tres componentes */ 

    char method[10], path[100], protocol[10];
    sscanf(request, "%s %s %s", method, path, protocol);
    printf("\n#######################\n\nReceived request: %s\n\n#######################\n", path);

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

    else if (strncmp(path, "/media/", 5) == 0){           /* Archivos multimedia */
        char *filename = malloc(MAX_LINE * sizeof(char));
        memset(filename, 0, MAX_LINE);
        filename = strcpy(filename, "..");
        filename = strcat(filename, path);
        char *decodedFN = decodeURL(filename);
        serveFile(client_fd, decodedFN);
    }

    else if (strcmp(path, "/movies") == 0){               /* Datos de las películas */
        sendMovieData(movies, client_fd);
    }

    else{                                                 /* Cualquier otro path */
        sendResponse(client_fd, "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n");
    }
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
void sendMovieData(movie movies[], int client_fd){
    
    char *data = malloc(MAX_LINE * sizeof(char));
    memset(data, 0, MAX_LINE);
    data = strcpy(data, "[");

    for (int i = 0; i < 5; i++){
        char *json = movieToJSON(movies[i]);
        data = strcat(data, json);
        free(json);
        if (i < 4){  data = strcat(data, ",");  }
    }

    data = strcat(data, "]");

    /* Se envía el header HTTP con el tipo de contenido y el tamaño de los datos. */
    char headers[100];
    sprintf(headers, "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: %ld\r\n\r\n", strlen(data));
    sendResponse(client_fd, headers);

    /* Se envían los datos de las películas al cliente. */
    sendResponse(client_fd, data);
    free(data);
}


/* Esta función re-escribe la información de movies[] y retorna una cadena con formato json. */
char *movieToJSON(movie m){

    char *json = malloc(MAX_LINE * sizeof(char));
    sprintf(json, "{\"title\":\"%s\",\"description\":\"%s\"}", m.title, m.description);
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