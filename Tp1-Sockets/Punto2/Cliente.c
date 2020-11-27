#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define MAXDATASIZE 1900000

int sendall(int s, char *buf, int len)
{
    int total = 0;
    int bytesleft = len;
    int n;
    while (total < len)
    {
        n = send(s, buf + total, bytesleft, 0);
        if (n == -1)
        {
            perror("send");
            break;
        }
        total += n;
        bytesleft -= n;
    }
    return n == -1 ? -1 : 0;
}

int crearArchivo(char* buffer,char *nombreArchivo, char *rutaDestino)
{
    char* rutaFinal=strcat(rutaDestino,"/");
    rutaFinal=strcat(rutaFinal,nombreArchivo);
    printf("\nLa ruta destino es: %s",rutaFinal);
    int resultado = 0;
    FILE *fp;
    int i;
    fp = fopen(rutaFinal, "w+");

    fwrite(buffer , 1 , strlen(buffer) , fp );
    fclose(fp);
    printf("\nSe copio el archivo con exito.");
    return 0;
}

// cliente.c 0.0.0.0 3000 archivo.txt /Escritorio
int main(int argc, char *argv[])
{
    int socketCliente, numbytes;
    char *puertoDado;
    char *archivoPedido;
    char *rutaDestino;
    char buffer[MAXDATASIZE];
    struct hostent *he;
    struct sockaddr_in their_addr; /* información de la dirección de destino */
    if (argc < 5)
    {
        printf("\nFaltan parametros\n");
        printf("\n Ejemplo: Cliente PUERTO ARCHIVO RUTA_DESTINO");
        exit(1);
    }

    if ((he = gethostbyname(argv[1])) == NULL)
    { /* obtener información de máquina */
        perror("gethostbyname");
        exit(1);
    }

    //puertoDado = argv[2];
    archivoPedido = argv[3];
    rutaDestino = argv[4];

    if ((socketCliente = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1)
    {
        printf("\nError al crear el socket");
        exit(1);
    }
    their_addr.sin_family = AF_INET;
    their_addr.sin_port = htons(atoi(argv[2]));
    their_addr.sin_addr = *((struct in_addr *)he->h_addr);

    memset(&(their_addr.sin_zero), '\0', 8); /* poner a cero el resto
de la estructura */
    if (connect(socketCliente, (struct sockaddr *)&their_addr,
                sizeof(struct sockaddr)) == -1)
    {
        printf("\nError al conectar");
        exit(1);
    }
    int len = strlen(archivoPedido);
    if (sendall(socketCliente, archivoPedido, len) == -1)
    {
        printf("\nError al enviar los parametros al servidor");
        exit(-1);
    }

    if ((numbytes = recv(socketCliente, buffer, MAXDATASIZE - 1, 0)) == -1)
    {
        perror("recv");
        printf("\n Error al recibir datos cliente");
        exit(1);
    }
    if(numbytes==0){
        printf("\nNo se encontro el archivo en el servidor");
    }else{
        buffer[numbytes] = '\0';
        
        printf("%s", buffer);
        crearArchivo(buffer,archivoPedido,rutaDestino);
    }

    close(socketCliente);
    return 0;
}