#include "socketUtil.h"

movie movies[] = {
    {"The Godfather (1972)", "The aging patriarch of an organized crime dynasty transfers control of his clandestine empire to his reluctant son."},
    {"Pulp Fiction (1994)", "The lives of two mob hitmen, a boxer, a gangster and his wife, and a pair of diner bandits intertwine in four tales of violence and redemption."},
    {"The Big Lebowski (1998)", "Jeff \'The Dude\' Lebowski, mistaken for a millionaire of the same name, seeks restitution for his ruined rug and enlists his bowling buddies to help get it."},
    {"Blade Runner (1982)", "A blade runner must pursue and terminate four replicants who stole a ship in space, and have returned to Earth to find their creator."},
    {"Fight Club (1999)", "An insomniac office worker and a devil-may-care soapmaker form an underground fight club that evolves into something much, much more."}
};
char* movieToJSON(movie m) {
    char *json = malloc(MAX_LINE * sizeof(char));
    sprintf(json, "{\"title\":\"%s\",\"description\":\"%s\"}", m.title, m.description);
    return json;
}

void sendMovieData(movie movies[], int client_fd)
{   
    char *data = malloc(MAX_LINE * sizeof(char));
    memset(data, 0, MAX_LINE);
    data = strcpy(data, "[");
    for (int i = 0; i < 5; i++)
    {
        char *json = movieToJSON(movies[i]);
        data = strcat(data, json);
        free(json);
        if (i < 4)
        {
            data = strcat(data, ",");
        }
    }
    data = strcat(data, "]");
    char headers[100];
    sprintf(headers, "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: %ld\r\n\r\n", strlen(data));
    sendResponse(client_fd, headers);
    sendResponse(client_fd, data);
    free(data);
}

void handleRequest(int client_fd, char *request)
{
    // Parse the request
    char method[10], path[100], protocol[10];
    sscanf(request, "%s %s %s", method, path, protocol);
    printf("\n#######################\n\nReceived request: %s\n\n#######################\n", path);

    // Check if the request is for the index page
    if (strcmp(path, "/") == 0)
    {
        serveFile(client_fd, "../html/index.html");
    }

    else if (strcmp(path, "/css/index_style.css") == 0)
    {
        serveFile(client_fd, "../css/index_style.css");
    }

    else if (strcmp(path, "/js/index_script.js") == 0)
    {
        serveFile(client_fd, "../js/index_script.js");
    }

    else if (strcmp(path, "/movies") == 0)
    {
        // Send the movie data
        sendMovieData(movies, client_fd);
    }

    else
    {
        // Send a 404 Not Found response
        sendResponse(client_fd, "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n");
    }

}

void serveFile(int client_fd, char *filename)
{
    // Open the file for reading
    FILE *file = fopen(filename, "r");

    if (file != NULL)
    {
        // Get the file size
        fseek(file, 0, SEEK_END);
        int size = ftell(file);
        fseek(file, 0, SEEK_SET);

        // Send the HTTP headers
        char headers[100];
        sprintf(headers, "HTTP/1.1 200 OK\r\nContent-Type: %s\r\nContent-Length: %d\r\n\r\n", getMimeType(filename), size);
        sendResponse(client_fd, headers);

        // Send the file contents
        char buffer[MAX_LINE];
        int bytesRead;
        while ((bytesRead = fread(buffer, 1, MAX_LINE, file)) > 0)
        {
            send(client_fd, buffer, bytesRead, 0);
        }

        // Close the file
        fclose(file);
    }
    else
    {
        // Send a 404 Not Found response
        sendResponse(client_fd, "HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n");
    }
}

char *getMimeType(char *filename)
{
    char *extension = strrchr(filename, '.');
    if (extension != NULL)
    {
        if (strcmp(extension, ".html") == 0)
        {
            return "text/html";
        }
        else if (strcmp(extension, ".css") == 0)
        {
            return "text/css";
        }
        else if (strcmp(extension, ".js") == 0)
        {
            return "application/javascript";
        }
    }
    return "application/octet-stream";
}

void sendResponse(int client_fd, char *response)
{
    send(client_fd, response, strlen(response), 0);
}
