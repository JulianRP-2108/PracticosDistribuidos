/*
 * This is sample code generated by rpcgen.
 * These are only templates and you can use them
 * as a guideline for developing your own functions.
 */

#include "my_FS.h"
#include <pthread.h>
#include <dirent.h>
#define MAXNOMBRESIZE 200

// crear el mutex
pthread_mutex_t m = PTHREAD_MUTEX_INITIALIZER;

str_t getContent(char *nombreArchivo)
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
        printf("\n Dentro del getContent: %ld",length);
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


str_t buscarEnCache(str_t nombreArchivo,struct archivo cache[]){
    str_t contenido;
    int encontro=0;
    for(int i=0;i<CANTCACHE-1;i++){
        if(cache[i].nombreArchivo!=NULL){
            if(strcmp(cache[i].nombreArchivo,nombreArchivo)==0){
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



str_t* getfile_1_svc(argumento *argp, struct svc_req *rqstp)
       {
	    static str_t  result;
		pthread_mutex_lock(&m);

	    result=buscarEnCache(argp->nombreArchivo,cache);

	    if(result!=NULL){
		return &result;
	    }
	    if (buscarEnDirectorio(argp->nombreArchivo) == 1)
	    {
		result=getContent(argp->nombreArchivo);///esta no se si tengo que agregarla
		long total=strlen(result)+strlen(argp->nombreArchivo);  //Cada char pesa un byte
		
		if(total<=512000000){
		                                         //cargarlo en cache
		    insertarEnCache(argp->nombreArchivo,result,cache);
		}
	    }else{
		result=NULL;
	    }

	 
		pthread_mutex_unlock(&m);

	return &result;
        }

int *
wait_1_svc(void *argp, struct svc_req *rqstp)
{
	static int  result;

	/*
	 * insert server code here
	 */

	return &result;
}

int *
signal_1_svc(void *argp, struct svc_req *rqstp)
{
	static int  result;

	/*
	 * insert server code here
	 */

	return &result;
}



