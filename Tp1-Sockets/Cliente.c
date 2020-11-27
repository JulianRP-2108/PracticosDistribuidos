#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define PORT 3000       
#define MAXDATASIZE 100 

//TODO: ESPECIFICAR TCP EN LOS SOCKETS

int main(int argc, char *argv[])
{
    int mySocket, numbytes;
    char buffer[MAXDATASIZE];
    struct sockaddr_in their_addr; /* información de la dirección de destino */
    if (argc != 2)
    {
        printf("\nSe necesita una direccion destino\n");
        exit(1);
    }
    
    if ((mySocket = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        printf("\nError al crear el socket");
        exit(1);
    }
    their_addr.sin_family = AF_INET;
    their_addr.sin_port = htons(PORT); 
    inet_aton(argv[1], &(their_addr.sin_addr));

    memset(&(their_addr.sin_zero), '\0', 8); /* poner a cero el resto
de la estructura */
    if (connect(mySocket, (struct sockaddr *)&their_addr,
                sizeof(struct sockaddr)) == -1)
    {
        printf("\nError al conectar");
        exit(1);
    }
    if ((numbytes = recv(mySocket, buffer, MAXDATASIZE - 1, 0)) == -1)
    {
        printf("\n Error al enviar datos recv");
        exit(1);
    }
    buffer[numbytes] = '\0';
    printf("Mensaje recibido: %s", buffer);
    close(mySocket);
    return 0;
}