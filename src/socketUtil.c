/* 
    MUST COMPILE AS: gcc -o socketClient socketClient.c socketUtil.c
*/

#include "socketUtil.h"

int createSocket(){
    return socket(AF_INET, SOCK_STREAM, 0);
}

struct sockaddr_in* createAddress(char* IP){
    /* Apartamos memoria para nuestra dirección */
    struct sockaddr_in *address = malloc(sizeof(struct sockaddr_in));

    /* Llenamos de ceros a la dirección */
    bzero(address, sizeof(struct sockaddr_in));

    /* Inicializamos la dirección */
    address->sin_family = AF_INET;
    address->sin_port = htons(SERVER_PORT);

    if(strlen(IP) == 0){
        address->sin_addr.s_addr = htonl(INADDR_ANY);
    }
    else if(inet_pton(AF_INET, IP, &address->sin_addr) <= 0){
        exit_Error("inet_pton error por %s", IP);
    }
    return address;
}

void exit_Error(const char *format, ...){
    int error_No;
    va_list args;
    error_No = errno;
    
    va_start(args, format);
    vfprintf(stdout, format, args);
    fprintf(stdout, "\n");
    fflush(stdout);

    if(error_No != 0){
        fprintf(stdout, "(Error no. = %d) : %s \n", error_No, strerror(error_No));
        fflush(stdout);
    }
    va_end(args);
    exit(1);
}

char *bin2hex(const unsigned char *input, size_t len){
    char *result;
    char *hex_Its = "0123456789ABCDEF";

    if(input == NULL || len <= 0){
        return NULL;
    }
    int result_Len = (len * 3) + 1;

    result = malloc(result_Len);
    bzero(result, result_Len);

    for(int i = 0; i < len; i++){
        result[i * 3] = hex_Its[input[i] >> 4];
        result[ (i * 3) + 1 ] = hex_Its[input[i] & 0x0F];
        result[ (i * 3) + 2 ] = ' ';
    }
    
    return result;
}