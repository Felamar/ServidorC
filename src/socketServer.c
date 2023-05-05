/* 
    MUST COMPILE AS: 
    gcc -o ../build/socketServer socketServer.c socketUtil.c
*/

#include "socketUtil.h"

void setHttpHeader(char httpHeader[])
{
    // File object to return
    FILE *htmlData = fopen("../html/index.html", "r");

    char line[100];
    char responseData[8000];
    while (fgets(line, 100, htmlData) != 0) {
        strcat(responseData, line);
    }
    // char httpHeader[8000] = "HTTP/1.1 200 OK\r\n\n";
    strcat(httpHeader, responseData);
}

int main(int argc, char **argv){

    char *movie_titles = 
        "The Godfather (1972)\n
        Pulp Fiction (1994)\n
        The Big Lebowski (1998)\n
        Blade Runner (1982)\n
        Fight Club (1999)"
    ;

    char *movie_description = 
        "The aging patriarch of an organized crime dynasty transfers control of his clandestine empire to his reluctant son.\n
        The lives of two mob hitmen, a boxer, a gangster and his wife, and a pair of diner bandits intertwine in four tales of violence and redemption.\n
        Jeff \"The Dude\" Lebowski, mistaken for a millionaire of the same name, seeks restitution for his ruined rug and enlists his bowling buddies to help get it.\n
        A blade runner must pursue and terminate four replicants who stole a ship in space, and have returned to Earth to find their creator.\n
        An insomniac office worker and a devil-may-care soapmaker form an underground fight club that evolves into something much, much more."
    ;

    char *get_type[] = {
        "GET / ",
        "GET /data",
        "GET /set?"
    };

    char httpHeader[8000] = "HTTP/1.1 200 OK\r\n\n";
    int client_SocketFD, connection_FD, n;
    struct sockaddr_in *server_Address;
    uint8_t buff[MAX_LINE + 1];
    uint8_t recv_Line[MAX_LINE + 1];
    uint8_t reqType_Line[MAX_LINE + 1];

    client_SocketFD = createSocket();
    if(client_SocketFD < 0){
        exit_Error("Error al crear socket.");
    }

    server_Address = createAddress("");
    if((bind(client_SocketFD, (S_A *) server_Address, sizeof(*server_Address))) < 0){
        exit_Error("Error de unión");
    }

    if(listen(client_SocketFD, 10) < 0){
        exit_Error("Error de escucha.");
    }
    setHttpHeader(httpHeader);

    while(true){
        struct sockaddr_in addr;
        socklen_t addr_Len;
        char client_address[MAX_LINE + 1];


        printf("Esperando por una conexión en el puerto %d\n", SERVER_PORT);
        fflush(stdout);
        connection_FD = accept(client_SocketFD, (S_A *) &addr, &addr_Len);
        
        inet_ntop(AF_INET, &addr, client_address, MAX_LINE);
        printf("Client connection: %s\n", client_address);

        memset(recv_Line, 0, MAX_LINE);
        n = read(connection_FD, recv_Line, MAX_LINE-1);
        char* x;
        int type;
        for(int i = 0; i < 3; i++){
            x = strstr(recv_Line, get_type[i]);
            if(x != NULL){
                type = i;
                break;
            }
        }
        while(n > 0){
            fprintf(stdout, "\n\n%s", /*bin2hex(recv_Line, n) */ recv_Line);
            if(recv_Line[n-1] == '\n'){
                break;
            }
            memset(recv_Line, 0, MAX_LINE);
            n = read(connection_FD, recv_Line, MAX_LINE-1);
        }
        if(n < 0){
            exit_Error("Error de lectura");
        }

        //snprintf((char*) buff, sizeof(buff), "HTTP/1.0 200OK\r\n\r\nHELLO!\n\n");
        switch(type){
            case 0:
                send(connection_FD, httpHeader, sizeof(httpHeader), 0);
                break;
            case 1:
                for(int i = 0; i < 6; i++){
                    write(connection_FD, movies[i], strlen(movies[i]));
                    write(connection_FD, ", ", strlen(", "));
                    if(i % 2 != 0){
                        send(connection_FD, "\n", strlen("\n"), 0);
                    }
                }
                break;
            case 2:
                char *token = strtok(recv_Line, " ");
                char *movie;
                while(token != NULL){
                    movie = strstr(token, "set?");
                    if(movie != NULL){break;}
                    token = strtok(NULL, " "); 
                }
                movie = strtok(movie, "?");
                movie = strtok(NULL, "?"); 

                for(int i = 0; i < 6; i+= 2){
                    if(strcmp(movies[i], movie) == 0){

                        write(connection_FD, movies[i+1] + strlen(movies[i+1]) - 1, sizeof(char));
                        
                        fprintf(stdout, "\n\n%s", "well done 2");
                        movies[i+1][strlen(movies[i+1]) - 1] = '\0';

                        fprintf(stdout, "\n\n%s", "well done 3");
                    }
                }

                break;
            default:
                break;
        }

        close(connection_FD);
    }
}