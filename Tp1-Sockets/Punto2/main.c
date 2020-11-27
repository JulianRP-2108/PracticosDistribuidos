/*
    HAY QUE HACER UN CLIENTE QUE SOLICITE ARCHIVOS Y EL SERVIDOR RESPONDA ENVIANDOSELOS.

    EL SERVIDOR DEBE TENER UNA CACHE DONDE BUSCAR PRIMERO LOS ARCHIVOS Y ENVIARLOS POR LA MISMA CONEXION
    SI NO LO ENCUENTRA AHI, DEBE BUSCAR EN UN DIRECTORIO

    CONSIDERACIONES: CACHE MAX 512MB
                    SI EL ARCHIVO SOL ES MAYOR A 512 NO SE LO PONE EN CACHE
                    POR NINGUN MOTIVO SE PUEDE SUPERAR LOS 512 MB, UTILIZAR
                    ALGORITMOS DE REEMPLAZO DE PAGINA VISTOS EN S.O
            SE PODRIA USAR UN MAP PARA IMPLEMENTAR LA CACHE {NOMBRE: CONTENIDO}

*/

//este va a ser el servidor

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/wait.h>
#include <dirent.h>
//#include <map>

#define MYPORT 3000
#define IP "0.0.0.0"
#define BACKLOG 5
#define RUTA "storage"
#define MAXNOMBRESIZE 200
#define CANTCACHE 50
struct archivo
{
    char *nombreArchivo;
    char *contenido;
};

int sendall(int s, char *buf, int len)
{
    int total = 0;
    int bytesleft = len;
    int n;
    printf("\n El total a enviar es: %d",len);
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
void sigchld_handler(int s)
{
    while (wait(NULL) > 0)
        ;
}

int buscarEnDirectorio(char *nombreArchivo)
{
    int encontro = 0;
    struct dirent *de; // Pointer for directory entry
    // opendir() returns a pointer of DIR type.
    DIR *dr = opendir(RUTA);

    if (dr == NULL) // opendir returns NULL if couldn't open directory
    {
        printf("\nNo se pudo abrir el directorio");
        return -1;
    }
    while ((de = readdir(dr)) != NULL)
    {
        if (strcmp(de->d_name, nombreArchivo) == 0)
        {
            encontro = 1;
        }
    }

    closedir(dr);
    return encontro;
}

int getSizeCache(struct archivo cache[]){
    int total=0;
    for(int i=0;i<CANTCACHE-1;i++){
        if(cache[i].nombreArchivo!=NULL){
            total+=strlen(cache[i].nombreArchivo);
            total+=strlen(cache[i].contenido);
        }
    }
    return total;
}
//Inserta donde primero encuentra un espacio nulo
int insertarEnCache(char* nombreArchivo,char*contenido,struct archivo cache[]){
    
    long cacheSize=getSizeCache(cache);
    long archivoSize=strlen(contenido)+strlen(nombreArchivo);
    while((cacheSize + archivoSize) > 512000000){  //sizeof devuelve el valor en bytes
        //Voy a eliminar el elemento mas grande que resida en cache hasta que haya espacio
        long mayor=strlen(cache[0].nombreArchivo)+strlen(cache[0].contenido);
        int pos=0;
        for(int i=1;CANTCACHE-1;i++){        //busco el mayor
            if(cache[i].nombreArchivo!=NULL){
                if(strlen(cache[i].nombreArchivo)+strlen(cache[i].contenido)>mayor){
                    mayor=strlen(cache[i].nombreArchivo)+strlen(cache[i].contenido);
                    pos=i;
                }
            }
        }
        cache[pos].nombreArchivo=NULL;
        cache[pos].contenido=NULL;            //"borro" el struct mas grande (pongo los campos en null)
        cacheSize=getSizeCache(cache);    //actualizo el tamaño
    }
    //una vez que ya tengo espacio
    struct archivo nulo;
    for(int i=0;CANTCACHE-1;i++){
        if(cache[i].nombreArchivo==NULL){
                cache[i].nombreArchivo=nombreArchivo;
                cache[i].contenido=contenido;
                break;
        }
    }
    return 0;
}
char* buscarEnCache(char *nombreArhivo,struct archivo cache[]){
    char* contenido;
    int encontro=0;
    for(int i=0;i<CANTCACHE-1;i++){
        if(cache[i].nombreArchivo!=NULL){
            if(strcmp(cache[i].nombreArchivo,nombreArhivo)==0){
                contenido=cache[i].contenido;  
                encontro=1;
                break;
            }
        }
    }
    if(encontro){
        return contenido;
    }else{
        return NULL;
    }
}

char* getContent(char *nombreArchivo)
{
    char *buffer;
    long length;
    char rutaCompleta[300];
    strcpy(rutaCompleta,RUTA);
    strcat(rutaCompleta,"/");
    strcat(rutaCompleta,nombreArchivo);
    FILE *f = fopen(rutaCompleta, "rb");

    if (f)
    {
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        printf("\n Dentro del getContent: %d",length);
        fseek(f, 0, SEEK_SET);
        buffer = malloc(length);
        if (buffer)
        {
            fread(buffer, 1, length, f);
        }
        fclose(f);
    }
    return buffer;
}

char* getFile(char *nombreArchivo,struct archivo cache[])
{
    char* resultado;
    resultado= buscarEnCache(nombreArchivo,cache);
    if(resultado!=NULL){
        return resultado;
    }
    if (buscarEnDirectorio(nombreArchivo) == 1)
    {
        resultado=getContent(nombreArchivo);
        long total=strlen(resultado)+strlen(nombreArchivo);  //Cada char pesa un byte
        printf("\n El total del tamaño es: %d",strlen(resultado));
        if(total<=512000000){
            //cargarlo en cache
            insertarEnCache(nombreArchivo,resultado,cache);
        }
    }else{
        resultado=NULL;
    }
    return resultado;
}

int main(int argc, char *argv[])
{

    //armo la cache
    struct archivo cache[CANTCACHE];
    for(int i=0;i<CANTCACHE-1;i++){
        cache[i].nombreArchivo=NULL;
        cache[i].contenido=NULL;
    }

    printf("\nServidor iniciado");
    int mySocket, socketAux;

    struct sockaddr_in my_addr;
    struct sockaddr_in they_addr;

    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(MYPORT);
    inet_aton("0.0.0.0", &(my_addr.sin_addr));
    memset(&(my_addr.sin_zero), '\0', 8);

    //Este es el socket que estara escuchando
    mySocket = socket(my_addr.sin_family, SOCK_STREAM, IPPROTO_TCP); //creando el socket
    if (mySocket == -1)
    {
        printf("\nOcurrio un error creando el socket");
        return;
    }
    else
    {
        printf("\nSocket creado");
    }
    if (bind(mySocket, &my_addr, sizeof(struct sockaddr_in)) == -1)
    {
        printf("\nOcurrio un error al bindear el socket con el puerto");
        return;
    }
    else
    {
        printf("\nSocket bindeado");
    }
    printf("\nEscuchando en el puerto: %d", MYPORT);
    printf("\nDirectorio a buscar: %s", RUTA);
    if (listen(mySocket, BACKLOG) == -1)
    { //EL 5 ES EL NUMERO MAXIMO DE PERSONAS QUE PUEDEN ESTAR ESPERANDO
        printf("\nOcurrio un error al empezar a escuchar");
        return -1;
    }
   
    unsigned int sin_size;
    while (1)
    {
        sin_size = sizeof(struct sockaddr_in);
        //Cuando se acepta una conexion se usa el socketAux para responder
        if ((socketAux = accept(mySocket, (struct sockaddr *)&they_addr, &sin_size)) == -1)
        {
            printf("\nOcurrio un error al aceptar una conexion");
            return;
        }

        printf("\nSe obtuvo una conexion desde: %s", inet_ntoa(they_addr.sin_addr));

        //if (!fork())
        //{
            close(mySocket);

            
            int numbytes;
            char *nombreArchivo = malloc(MAXNOMBRESIZE);
            if ((numbytes = recv(socketAux, nombreArchivo, MAXNOMBRESIZE - 1, 0)) == -1)
            {
                perror("recv servidor");
                exit(1);
            }

            nombreArchivo[numbytes] = '\0';
            printf("\nEl archivo pedido es: %s", nombreArchivo);

            char* contenido=getFile(nombreArchivo,cache);
            int len;
            if(contenido==NULL){
                len=0;
                sendall(socketAux,contenido,len);
            }else{
                len=strlen(contenido);
                sendall(socketAux,contenido,len);
            }
            close(socketAux); //ACA LO SACO AL CLIENTE
            exit(0);
        //}
        close(socketAux);
    }
}
