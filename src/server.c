#include "socketUtil.h"

int main(int argc, char const *argv[])
{
    int server_fd, client_socket, client_request;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};

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

    char response[1024];

    // Create socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0){
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set socket options
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))){
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    // Set address parameters
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Bind socket to address and port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0){
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for incoming connections
    if (listen(server_fd, 10) < 0){
        perror("listen");
        exit(EXIT_FAILURE);
    }
    setHttpHeader(httpHeader);

    // Wait for incoming connections and handle them
    while (true){
        // Accept incoming connection
        if ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0){
            perror("accept");
            exit(EXIT_FAILURE);
        }

        // Read incoming data
        memset(buffer, 0, sizeof(buffer));
        client_request = read(client_socket, buffer, 1024);
        printf("Received request: %s\n", buffer);

        // Check if request is for movie titles
        if (strcmp(buffer, "movie_titles") == 0)
        {
            // Build response string
            strcpy(response, movie_titles);

            // Send response to client
            send(client_socket, response, strlen(response), 0);
            printf("Sent response: %s\n", response);
        }
        else
        {
            // Send error message to client
            char *error_msg = "Invalid request";
            send(client_socket, error_msg, strlen(error_msg), 0);
            printf("Sent error message: %s\n", error_msg);
        }

        // Close socket
        close(client_socket);
    }

    return 0;
}
